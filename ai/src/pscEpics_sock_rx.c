/*
 * pscEpics_sock_rx.c
 *
 *  Created on: Sep 12, 2013
 *      Author: kha
 */

#include <stdio.h>
#include <string.h>
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwipopts.h"
#include "xparameters.h"
#include "xbasic_types.h"
#include "time.h"

#include "fpga_system.h"
#include "pscEpics.h"
#include "types.h"

#include "Dfe_ctrl.h"

#include "mps.h"


#define	RX_DDR		1

// BRAM ADDRESS
#define	H1_CHAMBER_OFFSET		11
#define	V1_CHAMBER_OFFSET		12
#define	H2_CHAMBER_OFFSET		13
#define	V2_CHAMBER_OFFSET		14




u16_t rx_port = PSC_RX_SOCKET_PORT;

extern	psc_RxBuffs_t	psc_RxBuffs;
Xuint32	usAddressIndex;
unsigned char mask_buf[32], prev_mask_buf[32];
Xuint32	ao_revData[64];


char *logic[] = {
		"",
		"",
};


static int
lwip_writeall(int s, const void *data, int size)
{
	int sent = 0;
	int ret;
	do {
		ret = lwip_write(s,data+sent, size-sent);
		if(ret<1)
			return ret;
		sent += ret;
	} while(sent<size);
	return size;
}


/*
 *
 */
int 	processRxBuffData( psc_RxBuffs_t *prx )
{
	int	i;
	int	len32bit;
	int	addr;

	xil_printf("Process Rx Data\n\r");

	xil_printf("MsgId       = %d\r\n", prx->MsgId);
	xil_printf("Data Length = %d\r\n", prx->bodyLen);

	len32bit = prx->bodyLen/4;

	if(prx->MsgId == 1) {
		for(i=0; i<100; i++)
			xil_printf("%d = %d (0x%x)\r\n", i, prx->data[i], prx->data[i] );
	}
}

/*
 *
 *
 */
static int processRxData(short msgID, unsigned int LenBytes)
{
	int	i;
	int	len32bit;
	int addr;
	Xuint32	mask, mask_set;
	int	ret;
	int	regAddr;
	int offset, base;
	int	Data;
	float	f32[2];
	Xint32	W32data;
    unsigned int *pwDest;
	unsigned int pwDestBuff[8];
	float	*padDest;

	Xuint32	d;
	float f;

	pwDest = pwDestBuff;
	pwDest = (unsigned int *)(padDest+0);

	//char *rxBuf;
	//rxBuf=(char *)DDR3_FOFB_DATA_BASE;

	//xil_printf("ID=%d, Len=%d\r\n" , msgID, LenBytes);
	//
	//xil_printf("MsgId       = %d\r\n", pSrvRxDdrOffAddr->MsgId);
	//xil_printf("Data Length = %d\r\n", pSrvRxDdrOffAddr->bodyLen);

	//len32bit = pSrvRxDdrOffAddr->bodyLen/4;
	len32bit = LenBytes/4;

#if 0
	base = DDR3_RX_REG;
	offset = 0;
	for(i=0; i<len32bit; i++) {
		Data = Get_uInt32_DDR2(DDR3_RX_REG+i*4);
		//Data = pSrvRxDdrOffAddr->data[i];
		xil_printf("%d = %d\r\n", i, Data);
	}
#endif
	/*
	for(i=0; i<12; i++)
		xil_printf("%d, ", pRxData[i]);
	xil_printf("\r\n");
	*/


	if( msgID == 1) {	//single write
		//regAddr = pSrvRxDdrOffAddr->data[0]/4;
		regAddr = Get_uInt32_DDR2(DDR3_RX_REG)/4;
		xil_printf("id1 addr = %2d, data = %d\r\n",  regAddr,   Get_uInt32_DDR2(DDR3_RX_REG+4) );
		if(regAddr<100) {
			//ret = EpicsCmdsInterpreter(regAddr, pSrvRxDdrOffAddr->data[1] );
			ret = EpicsCmdsInterpreter(regAddr, Get_uInt32_DDR2(DDR3_RX_REG+4)  );
		}
		else {

		}
	}
	else if( msgID == 2) {	//single write float 32
		//regAddr = pSrvRxDdrOffAddr->data[0]/4;
		regAddr = Get_uInt32_DDR2(DDR3_RX_REG)/4;

		/* 'data' holds IEEE-754 single-precision floating value */
	    d = Get_uInt32_DDR2(DDR3_RX_REG+4);
	    f = *(float *)&d;

		if(regAddr == 42 || regAddr == 43 ) {
			W32data = (int)( f * 1000000);	//nm
			ret = EpicsCmdsInterpreter(regAddr,  W32data );
			xil_printf("id2 f addr = %2d, data = %d (nm)\r\n",  regAddr,   W32data );
		}
		else if(regAddr>=150 && regAddr < 900) {
			//W32data = padDest[0] * 1000000;	//nm
			//f = *(float *)&d;
			W32data = (int)( f * 1000000);	//nm
			xil_printf("id2 addr=%d,  %d\r\n", regAddr, W32data );
			//H1,V1
			if(regAddr >= 150 && regAddr < 225)	{//space=70
				mps_DpRamOffsetWrite(0, regAddr-150, W32data );
			}
			else if(regAddr >= 225 && regAddr < 300){
				mps_DpRamOffsetWrite(1, regAddr-225, W32data );
			}
			//H2, V2
			else if(regAddr >= 300 && regAddr < 375) {
				mps_DpRamOffsetWrite(2, regAddr-300, W32data );
			}
			else if(regAddr >= 375 && regAddr < 450) {
				mps_DpRamOffsetWrite(3, regAddr-375, W32data );
			}
			//H3,V3
			else if(regAddr >= 450 && regAddr < 525) {
				mps_DpRamOffsetWrite(4, regAddr-450, W32data );
			}
			else if(regAddr >= 525 && regAddr < 600)
				mps_DpRamOffsetWrite(5, regAddr-525, W32data );
			// AIAL-H, AIAL-V  (Angle)
			else if(regAddr >= 600 && regAddr < 675)
				mps_DpRamOffsetWrite(6, regAddr-600, W32data );
			else if(regAddr >= 675 && regAddr < 750)
				mps_DpRamOffsetWrite(7, regAddr-675, W32data );
			//S-mm and S-mrad
			else if(regAddr >= 750 && regAddr < 825) {
			    //if(d > 0x3f000000) {	//IEEE754 0.5
				if(f > 0.9) {
			    	//xil_printf("data overflow...0x%x\r\n", d);
			    	f = 0.9;
			    	printf("data overfolw...0x%x, %f\r\n", (unsigned int)d, f);
			    }
				mps_DpRamOffsetWrite(8, regAddr-750, (int)(f * FIX_32_31) );
			}
			else if(regAddr >= 825 && regAddr < 900) {
			    //if(d > 0x3f000000) {	//IEEE754 0.5
				if (f > 0.9) {
			    	f = 0.9;
			    	printf("data overfolw...0x%x, %f\r\n", (unsigned int)d, f);
			    }
				mps_DpRamOffsetWrite(9, regAddr-825, (int)(f * FIX_32_31) );
			}

			else { xil_printf("address err\r\n"); }
		}
		else if( regAddr >= 900 && regAddr < 975) {	//LOGIC
			offset = regAddr-900;
			xil_printf("Logic set %d, %d = %d\r\n", regAddr, offset, d);
			mps_DpRamOffsetWrite(10, offset, d);
		}

		////////////////////////////////////////////////
		// OFFSET
		// Added 03/31/2014 new offset added
		// 10/15/14 Test, fixed problem
		//
		//#define	H1_CHAMBER_OFFSET		11		//added 03/29/14
		//#define	V1_CHAMBER_OFFSET		12
		//#define	H2_CHAMBER_OFFSET		13		//added 03/31/14
		//#define	V2_CHAMBER_OFFSET		14

		////////////////////////////////////////////////
		else if (regAddr >= 975 && regAddr < 1050) {	// BRAM11  H1
			W32data = (int)( f * 1000000);	//nm
			offset = regAddr-975;
			xil_printf("H1 Origin Offset %d (%d) = %d nm \r\n", regAddr, offset, W32data);
			mps_DpRamOffsetWrite(H1_CHAMBER_OFFSET, offset, W32data );
		}
		else if (regAddr >= 1050 && regAddr < 1125) {
			W32data = (int)( f * 1000000);	//nm
			xil_printf("H2 Origin Offset %d\r\n", regAddr);
			//mps_DpRamOffsetWrite(12, regAddr-1050, W32data );	// BRAM12  V1
			mps_DpRamOffsetWrite(H2_CHAMBER_OFFSET, regAddr-1050, W32data );	// BRAM12  H2
		}
		else if (regAddr >= 1125 && regAddr < 1200) {
			W32data = (int)( f * 1000000);	//nm
			xil_printf("V1 Origin Offset %d\r\n", regAddr);
			//mps_DpRamOffsetWrite(13, regAddr-1125, W32data );	// BRAM13  H2
			mps_DpRamOffsetWrite(V1_CHAMBER_OFFSET, regAddr-1125, W32data );	//V1

		}
		else if (regAddr >= 1200 && regAddr < 1275) {
			W32data = (int)( f * 1000000);	//nm
			xil_printf("V2 Origin Offset %d\r\n", regAddr);
			mps_DpRamOffsetWrite(V2_CHAMBER_OFFSET, regAddr-1200, W32data );	// BRAM15  V2
		}
			/*
			150
			225
			300
			375
			450
			525
			600
			675
			750
			825
			900
			1050
			1125
			1200

			*/
	}

	// 05/20/2014 added for AI-ID enable/disable
#if 0
	else if( msgID == 3) {
		regAddr = Get_uInt32_DDR2(DDR3_RX_REG)/4;
	    d = Get_uInt32_DDR2(DDR3_RX_REG+4);
	    xil_printf("msgID=3, %d, %d\r\n", regAddr, d);

	    //if( regAddr == 120 )
	    {
	    	// 0 - 30
	    	mps_ai_id_enable(regAddr, d);
	    }
	}
#endif

	/*
		Waveform received
		1) reference orbit
		2) bpm mask bit
		3)
	 */
	else if( msgID >= 70 || msgID < 86)
	{
		 switch( msgID ) {
		 	 case 70:	//sr-bpm x,y limit value
		 		 xil_printf("write sr-bpm x,y limit %d\r\n", len32bit);
		 		 fofb_ReferenceOrbitWrite(0, 360, &pSrvRxDdrOffAddr->data[0]);	// 180 SR-BPM x, and 180 SR-BPM Y
		 	 //sr-bpm mask setting
		 	 case 71:
		 		 xil_printf("Write use or not use %d\r\n", len32bit);
		 		 /*
		 		  * number of BPM is 180, x and y together
		 		  */
		 		 if(len32bit>180) len32bit = 180;
		 		 for(i=0; i<len32bit; i++) {
		 			fofb_MaskRamWrite(i,     pSrvRxDdrOffAddr->data[i]);		// SR-BPM enable and disable x
		 			fofb_MaskRamWrite(i+180, pSrvRxDdrOffAddr->data[i]);		// y
		 		 }
		 		 break;


		 	 //05/19/2014
		 	 //x and y Limit setting
		 	 case 72:	//sr-bpm	x,y offset
		 		 xil_printf("sr-bpm x AIOLH %d\r\n", len32bit);
		 		 fofb_ReferenceOrbitWrite(0, 180, &pSrvRxDdrOffAddr->data[0]);	// 180 SR-BPM x, and 180 SR-BPM Y
		 		 break;

		 	 case 73:
		 		 xil_printf("sr-bpm y AIOLV %d\r\n", len32bit);
		 		 fofb_ReferenceOrbitWrite(180, 360, &pSrvRxDdrOffAddr->data[0]);	// 180 SR-BPM x, and 180 SR-BPM Y
		 		 break;

		 	 //Origin Offset
		 	 case 74:	//sr-bpm	x,y offset
		 		xil_printf("sr-bpm x AIORH %d\r\n", len32bit);
		 		mps_SrBPM_XyOffsetWrite(0, 180, &pSrvRxDdrOffAddr->data[0]);	// 180 SR-BPM x, and 180 SR-BPM Y
		 		break;

		 	 case 75:
		 		xil_printf("sr-bpm y AIORV %d\r\n", len32bit);
		 		mps_SrBPM_XyOffsetWrite(180, 360, &pSrvRxDdrOffAddr->data[0]);	// 180 SR-BPM x, and 180 SR-BPM Y
		 		break;

			///////////////////////////////////////	
			// Feb/12/2015
		 	 case 76:	//sr-bpm	BBA x,y offset
		 		xil_printf("sr-bpm x BBA-H %d\r\n", len32bit);
		 		mps_SrBPM_XyOffsetWithBbaOffsetWrite(0, 180, &pSrvRxDdrOffAddr->data[0]);	// 180 SR-BPM x, and 180 SR-BPM Y
		 		break;				
		 	 case 77:
		 		xil_printf("sr-bpm y BBA-V %d\r\n", len32bit);
		 		mps_SrBPM_XyOffsetWithBbaOffsetWrite(180, 360, &pSrvRxDdrOffAddr->data[0]);	// 180 SR-BPM x, and 180 SR-BPM Y
		 		break;

				
				
		 }	// end of switch

	}		//end of else if

}




//
//	Data Rx test for Active Interlock System
//
//	caput -a SR31-BI{AIE:01}Ref:Loc-s1 10 5 2 3 4 5 6 7 8 9 10
//
#if 0
void pscEpicsProcess_rx_request(void *p)
{
	int 	sd = (int)p;
	unsigned char 	*pRxBuf;
	int 	rdNum;
	int		i;
	unsigned int bufpos = 0;

	//used the fixed DDR3 for incoming data
#if	1
	pRxBuf = (char *)pSrvRxDdrOffAddr;
#else
	psc_RxBuffs_t	*psc_RxData;
	pRxBuf = (unsigned char *) psc_RxData;
#endif

	unsigned int ts_sec, ts_nsec;

	ts_sec  = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*24);
	ts_nsec = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*25);

	xil_printf("pscEpicsProcess RX connected... Ts=%d:%d\r\n", ts_sec, ts_nsec);

	usAddressIndex = 0;
	for(i=0; i<32; i++)
		mask_buf[i] = prev_mask_buf[i] = 0;
	for(i=0; i<64; i++)
		ao_revData[i] = 0;

	while (1)
	{
		xil_printf("->Receive wait..\r\n");
#if	1
		if ((rdNum = read(sd, pRxBuf, 1440)) <= 0) {
#else
		if ((rdNum = read(sd, pRxBuf, 1440)) <= 0) {
#endif
			xil_printf("%s: error reading from socket %d, closing socket  %d\n\r", __FUNCTION__, sd, rdNum) ;
			break;
		}
		bufpos += rdNum;
		{
			if ((pRxBuf[0]!=0x50) || (pRxBuf[1]!=0x53)  ) {
				xil_printf("Wrong header, received 0x%x, 0x%x\n\r", pRxBuf[0], pRxBuf[1]);
				break;
			} else {
#if	1
				processRxData(pRxBuf, rdNum);
#else
				processRxBuffData( psc_RxData );
#endif
			}

		}

	}

	/* close connection */
	close(sd);
	xil_printf("RxClose\n\r");
}

#else

	//Received Ramping Table from IOC
void pscEpicsProcess_rx_request(void *p)
	{
		int sd = (int)p;
		char *rxBuf;
		int rdNum;
		unsigned int bufpos = 0;
		//int Total_Ramping_Table_Size = 87016;
		//int Total_Ramping_Table_Size = 43440*2+128+8;
		volatile Xuint32 *regR1;

		regR1= 0x50000004;
		//used the fixed DDR2 for incoming data
		rxBuf=(char *)pSrvRxDdrOffAddr;

		unsigned int grepBytes = 8;
		char frameID = 0;
		char frameHeader = 1;

		xil_printf("\r\n***** pscEpicsProcess RX connected...\r\n");

		//flow: read 8 byte --> check the header (head wrong: close) --> check length --> read full length --> length wrong (close) --> process --> repeat
		while (1) {
			/* read only 8 byte */
			if ((rdNum = read(sd, rxBuf + bufpos, grepBytes - bufpos)) < 0) {
				xil_printf("%s: error reading from socket %d, closing socket\n\r", __FUNCTION__, sd);
				close(sd);
				return;
			}
#if 0
			xil_printf("\r\nrdNum=%d rxBuf=0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n\r",rdNum,rxBuf[0],rxBuf[1],rxBuf[2],
					rxBuf[3],rxBuf[4],rxBuf[5],rxBuf[6],rxBuf[7]);
#endif

			/* break if client closed connection */
			//if (rdNum <= 0) {
			//allow 0 byte reading
			if (rdNum <= 0) {
				xil_printf("readError, rdNum=%d\n\r",rdNum);
				break;
			}

			bufpos += rdNum;

			if(bufpos < grepBytes)
				continue;

			if(bufpos != grepBytes) {
				xil_printf("Read wrong length table. Should never be here! %u %d\n\r", bufpos, grepBytes);
				close(sd);
				return;
			} else {
				/* check if this the frame header */
				if(frameHeader==1) {
					if ((rxBuf[0]!=0x50) || (rxBuf[1]!=0x53) || (rxBuf[2]!=0x0) ) { // || (rxBuf[3]>0x52) || (rxBuf[3]<0x50) ) {
						xil_printf("...Wrong header: 0x%x,0x%x,0x%x\n\r", rxBuf[0], rxBuf[1], rxBuf[2]);
						close(sd);
						return;
					} else {
						/* calculate the body length */
						frameID = rxBuf[3];
						grepBytes = (Xuint8)rxBuf[4]*256*256*256 +(Xuint8)rxBuf[5]*256*256 +(Xuint8)rxBuf[6]*256 +(Xuint8)rxBuf[7]*1;
						if(grepBytes==0) { //this is 0 byte frame -- so, don't expect any body. wait for new header
							//xil_printf("H/L=0x%x/%d\n\r",frameID, grepBytes);
							grepBytes = 8;
							bufpos =0;
							frameHeader =1;
						} else {
							bufpos =0;
							frameHeader =0;
						}
					}
				}  else {  /* this is the frame body */
					processRxData(frameID, grepBytes);
					//xil_printf("frameID=%d, grepBytes=%d\r\n", frameID, grepBytes);
					grepBytes = 8;
					bufpos =0;
					frameHeader =1;
				}
			}

		//clear both transfer table bits. It is already latch in hardware.
		*regR1 = ((Xuint32)(*regR1))& 0xFFFFFCFF;
		}

		/* close connection */
		close(sd);
		xil_printf("\r\n***** pscEpicsProcess_rx closed *****\n\r");
	}
#endif


/*
 *
 */
void pscEpicsRx_thread()
{
	int sock, new_sd;
	struct sockaddr_in address, remote;
	int size;

	xil_printf("\r\n**********************************************************\n\r");
	    xil_printf("** pscEpicsRx_thread TCP/IP communication thread port 7 **\n\r");
	    xil_printf("**********************************************************\n\r");

	/* Setup Socket: create */
	if ((sock = lwip_socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return;

	address.sin_family = AF_INET;
	address.sin_port = htons(rx_port);
	address.sin_addr.s_addr = INADDR_ANY;

	/* Setup Socket: bind */
	if (lwip_bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
		return;
	lwip_listen(sock, 5);
	size = sizeof(remote);
	/* accept */
	while (1) {
		if ((new_sd = lwip_accept(sock, (struct sockaddr *)&remote, (socklen_t *)&size)) > 0)
			sys_thread_new("Rx_server", pscEpicsProcess_rx_request,(void*) new_sd,THREAD_STACKSIZE,DEFAULT_THREAD_PRIO);
	}
}
