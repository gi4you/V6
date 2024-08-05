/*
 *
 *	Matlab support communication thread.
 *
 *		TCP port = 65000
 *
 *	Test reading data
 *	Tested at ALS in Jan 24,25,26 2011
 *
 *	April 07, 2011
 *	tcp_write_reg(addr)		tested
 *
 *	V42.02 problem
 *		1) tcp_read_mem(addr) returns 1024 bytes
 *		2) sometimes socket connection fails
 *
 *	V42.03	solution
 *		1)  tcp_read_mem(addr)	4 byte, before 1024 byte.
 *		2)	blocking_lwip_recv() enable receive timeout
 *
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
#include "dfe_comm.h"

#include "cmd_modbus.h"
//#include "accelerator.h"


#define SEND_BUF_SIZE 			1024
#define	RECV_BUF_SIZE 			1024

#define	SEND_BUF_SIZE_INT		(1024/4)

#define SA_STREAM_DATA_SIZE 	(15*4)		/* 4 byte * 11 signals  + 8 byte*/
#define	TCP_COMM_PORT			65000

#define	TIMEOUT					5000	/* 5 sec */

//MATLAB COMM opcodes
#define ML_READ_DDR2_MEM_RRQ   		1
#define	ML_CTRL_DATA_WRQ			2
#define	ML_CTRL_DATA_RRQ			3
#define ML_DDR3_CLEAR_WRQ			4
#define ML_SA_STREAM_DATA_RRQ   	5
#define	ML_CTRL_DATA_RRQ2			6	/* access DFE_CONTROL_REGS->ioReg for V42*/

//extern sa_data_type sa_data;
extern int SaPositionCalMode;
extern MachineType;


static volatile unsigned char *ptrIO_CTRL;	
static volatile unsigned int *memptr;
static volatile unsigned char *ptrDDR2;

static int Mlab_conectionTime = 0;
static int	timoutCnt=0;

extern int	EpicsCmdsInterpreter();

//http://rt-thread.googlecode.com/svn-history/r860/branches/rtt_0_3_1/bsp/sam7x_ser2eth/ser2eth/apptasks.c
// return: 0  timeout.
//         >0 length
//         <0 socket error
int
blocking_lwip_recv(int s, void *mem, size_t len, int timeout) // the len param here is the length to recv.
{
    int readlen, offset = 0;
    fd_set fds;
    int ret;
    struct timeval to;

    to.tv_sec = timeout/1000;
    to.tv_usec = (timeout%1000)*1000;

    while(1)
    {
        FD_ZERO(&fds);
        FD_SET(s,&fds);
        ret = lwip_select(s+1,&fds,0,0,&to);
        if( ret == 0 )
            break;
        else if( ret < 0 )
            return ret;

        readlen = lwip_recvfrom(s, ((char*)mem)+offset, len-offset, 0, NULL, NULL);
        if( readlen == 0 ) // select is ok and readlen == 0 means connection lost.
            return -1;
        else if( readlen < 0 )
            return readlen;
        if( readlen == (len-offset) )
        {
            offset = len;
            break;
        }
        offset += readlen;
    }
    return offset;
}


/* thread spawned for each connection */
void process_tcp_request(void *p)
{
	int sd = (int)p;
	char r_buf[32];
	int n, i, nwrote, prevcnt;
	int bytes_sent, total_bytes_sent;
	int req_op, req_addr, req_len, req_data;
	unsigned int *ptrCmds;
	int	response_size;
	int	block_num,size,  rsize;
	unsigned int ts_sec, ts_nsec;

	prevcnt = 0;
	ts_sec  = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*24); //Get_IoCtrlReg(TRIG_TS_SEC);
	ts_nsec = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*25); //Get_IoCtrlReg(TRIG_TS_NSEC);

	xil_printf("Matlab connected... %d times, Ts=%d:%d\r\n", Mlab_conectionTime++, ts_sec, ts_nsec);
	
	while(1) 
	{   		
#if 0
		/* read a max of RECV_BUF_SIZE bytes from socket */	
		if ((n = read(sd, r_buf, RECV_BUF_SIZE)) <= 0) 	
		{
			xil_printf("\r\nMatlab Socket %d, closing, received %d bytes\n\r", sd, n);
			close(sd);
			return;
		}
#else
		n = blocking_lwip_recv(sd, r_buf, 16, TIMEOUT);	/* v42.03 */
		if( n == 0 ) {
			timoutCnt++;
			xil_printf("Matlab socket timeout rcv=%d, %d times\r\n", n, timoutCnt);
			if(timoutCnt > 10) break;
			else 
				continue;
		}
        if( n != 16 ) {
        	if(n <0) ("\r\nMatlab socket Closed =%d\r\n", n);
        	else xil_printf("\r\nMatlab socket receive error =%d\r\n", n);
            break;
        }
#endif
		ptrCmds = (unsigned int *)&r_buf;	
		req_op	    = ptrCmds[0] & 0xF;
		req_addr 	= ptrCmds[1];
		req_len  	= ptrCmds[2];
		req_data 	= ptrCmds[3];
				
		switch (req_op)  {
#if 0
			case ML_READ_DDR2_MEM_RRQ :	//read memory
				if(req_len > SEND_BUF_SIZE)
				{
					block_num = (int)(req_len / SEND_BUF_SIZE);
					size = (int)(block_num * SEND_BUF_SIZE);
					rsize =  req_len - size;

					/**/
					xil_printf("Read Request : Start Addr:0x%x, Length: %d, blk=%d, rsize=%d\r\n ",req_addr, req_len, block_num, rsize);
					ptrDDR2 =  (volatile unsigned char *) (req_addr);
					total_bytes_sent = 0;

					//block send
					for(i=0; i<block_num; i++)
					{
						if ((bytes_sent = lwip_write(sd, ptrDDR2, SEND_BUF_SIZE)) != SEND_BUF_SIZE) {
							xil_printf("%s: ERROR rcvd = %d, written = %d\n\r",__FUNCTION__, bytes_sent, SEND_BUF_SIZE);
							break;
						}
						ptrDDR2 += SEND_BUF_SIZE; //bytes_sent;
						total_bytes_sent += bytes_sent;
						//
						//xil_printf("total_bytes_sent=%d\r\n", total_bytes_sent);
					}
					if ((bytes_sent = lwip_write(sd, ptrDDR2, rsize)) != rsize) {
						xil_printf("%s: ERROR rcvd = %d, written = %d\n\r",__FUNCTION__, bytes_sent, rsize);
						break;
					}
					total_bytes_sent += bytes_sent;
					printf("\r\nTotal bytes sent = %d bytes...\r\n", total_bytes_sent);
				}
				else {
					if(ptrCmds[0] == 0x10000001)
						xil_printf("DDR: 0x%x\t Length: %d\r\n ",req_addr, req_len);
					ptrDDR2 =  (volatile unsigned char *) (req_addr);
					bytes_sent = lwip_write(sd, ptrDDR2, req_len*4) ;
				}
				break;
#else

			case ML_READ_DDR2_MEM_RRQ : //1 read memory block
				if(ptrCmds[0] == 0x10000001)
					xil_printf("DDR: 0x%x\t Length: %d\r\n ",req_addr, req_len);
				memptr = req_addr;
				total_bytes_sent = 0;
				if(req_len > SEND_BUF_SIZE) {
					while (total_bytes_sent < req_len) {
						if ((bytes_sent = lwip_write(sd, memptr, SEND_BUF_SIZE)) != SEND_BUF_SIZE) {
							xil_printf("Tx ERROR rcvd = %d bytes, total sent = %d bytes\n\r",__FUNCTION__, bytes_sent, total_bytes_sent);
							break;
							}
						memptr += SEND_BUF_SIZE_INT; //bytes_sent;
						total_bytes_sent += bytes_sent;
					}
				}
				else {	/* V42.03 */
					total_bytes_sent = req_len*4;
					if ((bytes_sent = lwip_write(sd, memptr, total_bytes_sent)) != total_bytes_sent) {
						xil_printf("Tx ERROR rcvd = %d bytes, total sent = %d bytes\n\r",__FUNCTION__, bytes_sent, total_bytes_sent);
						}
				}
				break;
#endif

			case ML_CTRL_DATA_WRQ :  //2 write register word
				//memptr = (unsigned int *) (req_addr); // + XPAR_USR_BUS_0_MEM0_BASEADDR);
				xil_printf("Write Request : Start Addr: %x\tLength: %d\tData: %d\r\n ",memptr, req_len, req_data);
				//*memptr = req_data;
				EpicsCmdsInterpreter(req_addr, req_data);
				break;

			case ML_CTRL_DATA_RRQ : // 3 read register word
				xil_printf("\r\nUSR_BUS:0x%x\tLength: %d\tData: %d\r\n",req_addr, req_len, req_data);
				memptr = (unsigned int *) (req_addr); // + XPAR_USR_BUS_0_MEM0_BASEADDR);	//0xB0000000
				req_data = *memptr;
				if ((bytes_sent = lwip_write(sd, &req_data, 4)) != 4)
					xil_printf("%s: ERROR rcvd = %d, written = %d\n\r",__FUNCTION__, n, nwrote);
				break;

			/* V42 extended register access */
			case ML_CTRL_DATA_RRQ2 : // 3 read register word
				xil_printf("USR_IO_REG:0x%x\tLength: %d\tData: %d\r\n",req_addr, req_len, req_data);
				//memptr = (unsigned int *) (req_addr + DDR2_CIO_BASE);	//DDR2_MPMC_BASEADDR + 0x3E000000
				//req_data = *memptr;

				if ((bytes_sent = lwip_write(sd, &DFE_CONTROL_REGS->ioReg[req_addr], 4)) != 4)
					xil_printf("%s: ERROR rcvd = %d, written = %d\n\r",__FUNCTION__, n, nwrote);
				break;

				//DDR2_CIO_BASE
			case ML_SA_STREAM_DATA_RRQ : //stream SA Data OP=5
				while (1) {	

					//sfp6_LED1(0);
					sleep(20);
				}
				break;

			case ML_DDR3_CLEAR_WRQ : //4 clear SA memory
				xil_printf("Disable SA interrupts\r\n");
				//Disable IP Interrupt Enable Register
				//Set_I32_PLB(XPAR_SA_DATA_0_BASEADDR+0x128, 0x00000000);
				//Disable Global Interrupt Register
				//Set_I32_PLB(XPAR_SA_DATA_0_BASEADDR+0x11C, 0x00000000);
				//
				xil_printf("Clearing SA Memory\r\n");
			    //memptr = (unsigned int *) DDR_SA_RING_WFM_BASE;
				//for (i=0; i<58720256; i++)
				//	 *memptr++ = 0;
				xil_printf("...Done...\r\n");
				xil_printf("Clearing SA Memory Ptr");
				//Set_I32_PLB(DDR_SA_RING_WFM_PTR_ADDR, 0);
				//sa_data.intrcnt = 0;
				//sa_data.trigd = 0;
				xil_printf("...Done...\r\n");
				xil_printf("Enable SA interrupts\r\n");
				//
				//Enable IP Interrupt Enable Register
				//Set_I32_PLB(XPAR_SA_DATA_0_BASEADDR+0x128, 0xF000000F);
				//Enable Global Interrupt Register
				//Set_I32_PLB(XPAR_SA_DATA_0_BASEADDR+0x11C, 0xF000000F);
				xil_printf("...Done...\r\n");
				break;
																	
			default :
				xil_printf("\r\n****************************\r\n");
				xil_printf("Invalid Request OP Code\r\n");
				break;
		}
		sleep(2);
		
	}	/* end of while */
	
abort:
	xil_printf("\r\n****************************\r\n");
	xil_printf("Matlab thread closing socket %d\n\r", sd);
	timoutCnt = 0;	//clear
	close(sd);
	return;
}


/*
 *
 */
void dBpm_Matlab_Comm_Thread()
{
	int sock, new_sd;
	struct sockaddr_in address, remote;
	int size;

	xil_printf("\r\n***************************************************\n\r");
	    xil_printf("** Matlab TCP/IP communication thread port 65000 **\n\r");
	    xil_printf("***************************************************\n\r");
		
	if ((sock = lwip_socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return;

	address.sin_family = AF_INET;
	address.sin_port = htons( TCP_COMM_PORT );
	address.sin_addr.s_addr = INADDR_ANY;

	if (lwip_bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
		return;

	lwip_listen(sock, 5);
	size = sizeof(remote);
	
	while (1) {
		if ((new_sd = lwip_accept(sock, (struct sockaddr *)&remote, (socklen_t *)&size)) > 0)
			sys_thread_new("process_tcp_request", process_tcp_request,
				(void*)new_sd,
				THREAD_STACKSIZE,
				DEFAULT_THREAD_PRIO);
	}
}

