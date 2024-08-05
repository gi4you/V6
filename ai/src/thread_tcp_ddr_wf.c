/*
 * tcp_ddr2_waveform support
 *
 *  Created on: Nov 10, 2010
 *      Author: kha
 *
 *	Jan 23, 2012
 *		added for booster fa mode and wavefrom read.
 *	Jun 12/13
 *		delay_1ms(500);	//500 ~ 1000  for block RX/TX time optimizations
 *
 *	8/28/13
 *		tx/rx added evr timestamp data.
 */


#include <stdio.h>
#include <string.h>
#include "xparameters.h"

#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwipopts.h"
#include <sys/timer.h>
#include "fpga_system.h"

#include "port.h"

#include "mb.h"
#include "mbframe.h"
#include "mbproto.h"
#include "mbconfig.h"


#include "fpga_system.h"
#include "dfe_comm.h"
#include "Dfe_ctrl.h"
#include "evr.h"
#include "cmd_modbus.h"

#define MAX_PACKET_SIZE 		1024	//(1024+9)
#define SEND_BUF_SIZE 			1024
#define	SEND_BUF_SIZE_INT		256		//(1024/4)		

u16_t 	DDR_WF_PORT = 5001;


#define MB_PDU_FUNC_READ_REGCNT_MAX        	 	(64000000)	//32 Mbytes

#define	ML_READ_DDR2_MEM_RRQ	0x01
#define	ML_CTRL_DATA_RRQ			3
#define	ML_CTRL_DATA_RRQ2			6	/* access DFE_CONTROL_REGS->ioReg for V42*/
#define	REQ_BOOST_FA_WFM		0x07


static int
DDR_WF_SOCK_eMBFuncReadInputRegister( int socket, ULONG ucFunctionCode, ULONG usRegAddress, ULONG usLength);
/* thread spawned for each connection */
void DDR_WF_process_tcp_request(void *p)
{
	int sd = (int)p;    
    unsigned int *ptrCmds;    
	ULONG   ucFunctionCode;	
    ULONG	usRegAddress;	/* 32-bit */
    ULONG  	usLength;		/* 32-bit */    
    ULONG  	usData;

	UCHAR          pucMBTCPFrame[MAX_PACKET_SIZE];
	int	n,  ret;
	
	int	response_size;
	char	txHeadBuf[64];
	int	bytes_sent, total_bytes_sent;
	static volatile unsigned int *memptr;	
	int req_data;
	int prevcnt, cnt;					
	dfe_Comm_Response	*cmd_Comm_Res;		/* 32bit address range */
	
	xil_printf("DDR_WF Server Processing  port : %d\n\r", DDR_WF_PORT);

  	while (1)
  	{	   		
	  	//DDR2_SLOW->space[0] = 0;
		if(( n = recv(sd, pucMBTCPFrame, 16, 0)) <= 0) {
			xil_printf("WF recv error %d,  ", n);
			break;
		}		
		ptrCmds = (unsigned int *)&pucMBTCPFrame;
		ucFunctionCode   = ptrCmds[0];
		usRegAddress 	 = ptrCmds[1];
		usLength   		 = ptrCmds[2]; // -  (sizeof(dfe_Comm_Response));	  /* it's byte size, do not calculate */  
		usData   		 = ptrCmds[3];
		
		//xil_printf("recv: op=%X,  addr=0x%X,  byte=%d \r\n", ucFunctionCode, usRegAddress, usLength);
		
		switch (ucFunctionCode) {			
			/* DDR read and transmitte */
			case OP_READ_DDR_DATA_RRQ:	//read input
			case OP_DDR_MEM_RRQ:		//0xA5
			case OP_DDR_MEM_RRQ+1:
				usLength -= (sizeof(dfe_Comm_Response) );	//sub head size
				ret = DDR_WF_SOCK_eMBFuncReadInputRegister(sd, ucFunctionCode, usRegAddress, usLength);
				if(ret < 0) goto END;
				break;	
				
			case OP_CTRL_DATA_WRQ:	/* commands for control/monitoring */				
				ret = EpicsCmdsInterpreter( usRegAddress, usData);								
				/* Response data */
			    cmd_Comm_Res = (dfe_Comm_Response *)txHeadBuf;
			    cmd_Comm_Res->fcode    = ucFunctionCode;
			    cmd_Comm_Res->startReg = usRegAddress;		/* 32-bit address */
			    cmd_Comm_Res->numRead  = usLength;			/* 32-bit number of count */ 
			    cmd_Comm_Res->Ts0  =  Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*24);	//timestamp from evr
			    cmd_Comm_Res->Ts1  =  Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*25);
			    response_size = sizeof(dfe_Comm_Response);                                                      			
				ret = send( sd, txHeadBuf, response_size, 0 );
				break;
									
			/* new added Jan 23, 2012 */	
			/* BOOSTER PROTOCOLES for Socket Communication R42.03 */		
			case ML_CTRL_DATA_RRQ : // 3 read register word
				xil_printf("\r\nUSR_BUS:0x%x\tLength: %d\tData: %d\r\n",usRegAddress, usLength, usData);
				memptr = (unsigned int *) (usRegAddress); // + XPAR_USR_BUS_0_MEM0_BASEADDR);	//0xB0000000
				req_data = *memptr;
				if ((bytes_sent = lwip_write(sd, &req_data, 4)) != 4)
					xil_printf("%s: ERROR rcvd = %d, written = %d\n\r",__FUNCTION__, 4, bytes_sent);
				break;
															
			/* V42 extended register access */
			case ML_CTRL_DATA_RRQ2 : // 3 read register word
				xil_printf("USR_IO_REG:0x%x\tLength: %d\tData: %d\r\n", usRegAddress, usLength, usData);
				if ((bytes_sent = lwip_write(sd, &DFE_CONTROL_REGS->ioReg[usRegAddress], 4)) != 4)
					xil_printf("%s: ERROR rcvd = %d, written = %d\n\r",__FUNCTION__, bytes_sent, total_bytes_sent);
				break;
							
			/* ADC, TBT */	
			case ML_READ_DDR2_MEM_RRQ : //1 read memory block
				//if(ptrCmds[0] == 0x10000001)
					xil_printf("DDR: 0x%x\t Length: %d\r\n ", usRegAddress, usLength);	/* bytes */
				memptr = usRegAddress;
				total_bytes_sent = 0;
				if(usLength > SEND_BUF_SIZE) {
					while (total_bytes_sent < usLength) {
						if ((bytes_sent = lwip_write(sd, memptr, SEND_BUF_SIZE)) != SEND_BUF_SIZE) {
							xil_printf("%s: Tx ERROR rcvd = %d bytes, total sent = %d bytes\n\r",__FUNCTION__, bytes_sent, total_bytes_sent);
							break;
							}
						memptr += SEND_BUF_SIZE_INT; //bytes_sent;
						total_bytes_sent += bytes_sent;
					}
				}
				else {	/* V42.03 */
					total_bytes_sent = 4; //usLength;
					if ((bytes_sent = lwip_write(sd, memptr, total_bytes_sent)) != total_bytes_sent) {
						xil_printf("%s:Tx ERROR rcvd = %d bytes, total sent = %d bytes\n\r",__FUNCTION__, bytes_sent, total_bytes_sent);
						}
				}
				break;
								
			/* For Booster FA Wfm Tx timing */	
			/*
			                      '--------------------'
				NPI ENABLE _______|                    |______________________
				                                   -------------
				FA WFM TX ________________________|             |_____________
				
				                  0                    300ms
				                  0               200ms
			*/
			case REQ_BOOST_FA_WFM : 												
				while(1) 
				{
					//Wait end of trigger for Booster feedback mode
					memptr = usRegAddress;
					total_bytes_sent = 0;
					prevcnt = cnt = 0;
					//xil_printf("FA:0x%x\tLength: %d\r\n", usRegAddress, usLength);					
					
#if 0
					if(DFE_CONTROL_REGS->ioReg[DDR3_NPI_TRIG_SRC_SP] == 0) {	/* internal soft */
						sleep(1000);	/* fot just test */
					}
					else {	/* external event trig */
						prevcnt = DFE_CONTROL_REGS->ioReg[EVR_TRIG_CNT_I];
						while (DFE_CONTROL_REGS->ioReg[EVR_TRIG_CNT_I] == prevcnt) {
							sleep(20);	
							cnt++;
							//xil_printf("external event timeout cnt=%d\r\n", cnt);
							if(cnt > 150 ) {	/* 3 second time out */ 
								xil_printf("thread_tcp_ddr_wf.c__external event timeout %d, cnt=%d\r\n", DFE_CONTROL_REGS->ioReg[EVR_TRIG_CNT_I], cnt);
								break;	/* time out */
							}
						}													
					}											
					while (total_bytes_sent < usLength ) {				
						if ((bytes_sent = lwip_write(sd, memptr, SEND_BUF_SIZE)) != SEND_BUF_SIZE) {
							xil_printf("Tx ERROR rcvd = %d bytes, total sent = %d bytes\n\r", bytes_sent, total_bytes_sent);
							goto END;	//break;
						}
						memptr += SEND_BUF_SIZE_INT; 	//1024/4 = 256 bytes
						total_bytes_sent += bytes_sent;
					}
#endif
				}			
				xil_printf("Fa Wfm send end\r\n");

				break;
																
			default:
				xil_printf("Function code not match = %d\r\n", ucFunctionCode);
				ret = send( sd, "ER\r\n", 4, 0 );
				break;	
		}    
		sleep(1);
  	}	/* end of while */
  		
END:	
	xil_printf("Closing WF socket %d\n\r", DDR_WF_PORT);
	close(sd);
	return;
}


/*
 *
 */
void dBpm_Epics_ddrWF_CommServer_Thread()
{
	int sock, new_sd;
	struct sockaddr_in address, remote;
	int size;

	xil_printf("\r\n***************************\n\r");
	    xil_printf("** TCP WF Thread **\n\r");
	    xil_printf("***************************\n\r");

	if ((sock = lwip_socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return;

	address.sin_family = AF_INET;
	address.sin_port = htons(DDR_WF_PORT);
	address.sin_addr.s_addr = INADDR_ANY;

	if (lwip_bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
		return;

	lwip_listen(sock, 5);
	size = sizeof(remote);

	while (1) {
		if ((new_sd = lwip_accept(sock, (struct sockaddr *)&remote, (socklen_t *)&size)) > 0)
			sys_thread_new("DDR_WF_process_server", DDR_WF_process_tcp_request,
				(void*)new_sd,
				THREAD_STACKSIZE,
				DEFAULT_THREAD_PRIO);
	}
}

#define	EPICS_HEAD_INCLUDE		1

/* ------------------------------------------------------------------------
 *
 *	Sep 14, 2010 K Ha added for 32-bit Multiple reading
 *
 *TX:
 *  | | | | | | |      ----       head            6 bytes
 *				| |    ------     slave           1 byte
 *				  | |    ------   function        1 byte
 *                  | | | | |   -------  address  (32-bit)
 * 
 *                          | | | | |   ------ length (32-bit)
 *
 *RX:
 *   | |            ---- function code
 *     | | | | |    ---- length
 *             | --------- Data |
 *    
 *
 *   TBT : 0x4000000, length = 1000000
 *		RECV: 0x0 0x1 0x0 0x0 0x0 0xA 0x1 0x4 0x4 0x0 0x0 0x0 0x0 0xF 0x42 0x40  
 *                                            --------------- -----------------   
 *							                     address          length
 *
 *
 *		Packet size is 1,024 byte
 *		10 k * 4 byte = 40,000 byte -- 39 loops
 *
 */
static int
DDR_WF_SOCK_eMBFuncReadInputRegister( int socket, ULONG ucFunctionCode, ULONG usRegAddress, ULONG usLength)
{

    eMBException    eStatus = MB_EX_NONE;
	UCHAR			bufFrame[MAX_PACKET_SIZE+1];    
    int	i;
    unsigned int	send_byte;    
    unsigned int rv;
	int	block_num, size, rsize;
	int bytes_sent, total_bytes_sent=0; 
	int	block_mem_send_byte;    		
	int	iRegIndex=0;
	ULONG	usRegCount;
	int	cp_bytes;
	
	int	offset;
	char	txHeadBuf[64];
	char	rxBuf[32];
	int	response_size;
	unsigned int *ptrAcks;
	int	TrigMode=0;
		
	dfe_Comm_Response	*cmd_Comm_Res;		/* 32bit address range */
	ptrAcks = (unsigned int *)&rxBuf;		
	static volatile unsigned char *ptrDDR2_MEM;								        

	usRegCount = usLength;			      		
    {                
        if( ( usLength >= 1 )
            && ( usLength < MB_PDU_FUNC_READ_REGCNT_MAX ) )
        {
			/* Response data */
		    cmd_Comm_Res = (dfe_Comm_Response *)txHeadBuf;
		    cmd_Comm_Res->fcode    = ucFunctionCode;
		    cmd_Comm_Res->startReg = usRegAddress;				/* 32-bit address */
		    cmd_Comm_Res->numRead  = usLength;					/* 32-bit number of count */
		    cmd_Comm_Res->Ts0      = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*24);	//timestamp from evr
		    cmd_Comm_Res->Ts1      = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*25);
		    response_size = sizeof(dfe_Comm_Response);             
            
            /* OP, ASSR, CNT, TS0, TS1 = 4*5 = 20 bytes */
            offset = response_size; 	/* 4 * 5 = 20 */                                             			
     		memcpy( bufFrame, txHeadBuf, response_size);
     		cp_bytes = send_byte = usLength; // 
     		
     		//xil_printf("DDR address=%X,  %X\r\n", DDR2_MPMC_BASEADDR + usRegAddress, usRegAddress );
     		
            block_mem_send_byte = send_byte;
            ptrDDR2_MEM =  (volatile unsigned char *) (DDR2_MPMC_BASEADDR + usRegAddress);		/* DDR-3 memory start address */
       		/* */
       		iRegIndex = 0;
      		if(send_byte < MAX_PACKET_SIZE) 
      		{	        
		        memcpy(&bufFrame[offset], ptrDDR2_MEM, send_byte);	  /* 0 - 10 is head information */    		
      			rv = send( socket, &bufFrame[0], send_byte, 0 );
      			if(rv < 0) {
	      			xil_printf("Tx error = %d\r\n", rv);
	      			return -1;
      			}
  			} 			
  			else {

#if(EPICS_HEAD_INCLUDE)	  			
      			rv = send( socket, &bufFrame[0], offset, 0 );	/* tx head data 20 bytes */
      			if(rv < 0) return -1;	  				  			 			      			
      			//
			    block_num = (int)(block_mem_send_byte / MAX_PACKET_SIZE);		
			    size = (int)(block_num * MAX_PACKET_SIZE);
			    rsize =  block_mem_send_byte - size;	      			
      			total_bytes_sent = 0;
      			    			
				//DDR-3 memory block send
				// 4096 * 2 is taken 2.2 ms, if less then 1024 it take 225 us								
		        for(i=0; i<block_num; i++) 
				{	
					iRegIndex = 0;
					usRegCount = MAX_PACKET_SIZE;	
			        /* WAIT ACK */	
					if(( rv = recv(socket, rxBuf, 8, 0)) <= 0) {	
						xil_printf("err received =%d ,  %d bytes\r\n", ptrAcks[0], ptrAcks[1]);
					}

					if ((bytes_sent = send( socket, ptrDDR2_MEM, MAX_PACKET_SIZE, 0 ) ) != MAX_PACKET_SIZE) {	
						xil_printf("%s: ERROR rcvd = %d, written = %d\n\r", __FUNCTION__, bytes_sent, MAX_PACKET_SIZE);
						return -1;
					}									
					ptrDDR2_MEM += MAX_PACKET_SIZE; //ddr-2 memory point increment
					total_bytes_sent += bytes_sent;

					//6/12/2013 for communication issue
					delay_1ms(500);	//500 ~ 1000
					//xil_printf("%d , %d,  bytes_sent=%d\r\n", i, bytes_sent, total_bytes_sent);
				}		
				
				iRegIndex = 0;
				usRegCount = rsize;	

		        /* WAIT ACK */	
				if(( rv = recv(socket, rxBuf, 8, 0)) <= 0) {	
					;//xil_printf("Received =%d ,  %d bytes\r\n", ptrAcks[0], ptrAcks[1]);	
				}		        
				if ((bytes_sent =  send( socket, ptrDDR2_MEM, rsize, 0 ) ) != rsize) {
					xil_printf("%s: ERROR2 rcvd = %d, written = %d\n\r",__FUNCTION__, bytes_sent, rsize);
					return -1;
				}			
				total_bytes_sent += bytes_sent;
		        /* WAIT ACK for END of DATA */	
				if(( rv = recv(socket, rxBuf, 8, 0)) <= 0) {	
					;//xil_printf("Received =%d ,  %d bytes\r\n", ptrAcks[0], ptrAcks[1]);	
				}
				else {					
					; //xil_printf("Received =%d ,  cnt=%d, %d bytes\r\n", rv, ptrAcks[0], ptrAcks[1]);
				}
				//xil_printf("%d , %d,  bytes_sent=%d\r\n", i, bytes_sent, total_bytes_sent);								
				//xil_printf("ddr=0x%X - 0x%X, loop=%d, sent=%d\r\n", DDR2_MPMC_BASEADDR + usRegAddress, (DDR2_MPMC_BASEADDR + usRegAddress) +total_bytes_sent , block_num, total_bytes_sent + 11);
#else
				/**/
			    block_num = (int)(block_mem_send_byte / MAX_PACKET_SIZE);		
			    size = (int)(block_num * MAX_PACKET_SIZE);
			    rsize =  block_mem_send_byte - size;			
			    /**/
				ptrDDR2_MEM =  (volatile unsigned char *) (DDR2_MPMC_BASEADDR + usRegAddress);		/* DDR-2 memory */
				total_bytes_sent = 0;
				
				//block send
		        for(i=0; i<block_num; i++) 
				{							
					if ((bytes_sent = lwip_write(socket, ptrDDR2_MEM, MAX_PACKET_SIZE)) != MAX_PACKET_SIZE) {
						xil_printf("%s: ERROR rcvd = %d, written = %d\n\r",__FUNCTION__, bytes_sent, MAX_PACKET_SIZE);
						break;
					}				
					ptrDDR2_MEM += MAX_PACKET_SIZE; 	//bytes_sent;
					total_bytes_sent += bytes_sent;
				}			
				if ((bytes_sent = lwip_write(socket, ptrDDR2_MEM, rsize)) != rsize) {
					xil_printf("%s: ERROR rcvd = %d, written = %d\n\r",__FUNCTION__, bytes_sent, rsize);
					return bytes_sent;
				}			
				total_bytes_sent += bytes_sent;
				xil_printf("total_bytes_sent=%d\r\n", total_bytes_sent);
#endif				  			
        	}
    	}
        else
        {
            eStatus = MB_EX_ILLEGAL_DATA_VALUE;
            xil_printf("\r\nDDR_WF_SOCK MB_EX_ILLEGAL_DATA_VALUE : %d\r\n", usRegCount);
        }
    }

    return eStatus;
}

/*
 * E N D  O F  F I L E
 *
 */
