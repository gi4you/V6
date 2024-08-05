/*
 * evr.c
 *
 *  Created on: Apr 22, 2011
 *      Author: kha
 *
 *	Event code for 10 Hz trigger and Post mortem test.
 *	Timestamp.
 *
 *	EVENT CODE
 *	DATA Bus
 *		- Remote reboot -- 1
 *		- Beam current	-- 1
 *		- Fill patten   -- 41
 *		-
 */

#include "time.h"
#include "sys/time.h"
#include <stdio.h>
#include <stdlib.h>

#include "fpga_system.h"
#include "evr.h"
#include "cmd_modbus.h"

#include "pscEpics.h"


typedef struct
{
  unsigned int		ts_seconds;
  unsigned int		ts_offset;
  char				cachename[256];	/* Tile cache filename */
} Ts_t;


#define	EV_CODE_TS0_32HZ		0x70
#define	EV_CODE_TS1_32HZ		0x71
#define	EV_CODE_TS_1HZ			0x7D


#define	EV_INTRRUPT_ENABLE		0x8000

#define	EVENT_LINAC_TRIGGER			16
#define	EVENT_LINAC_PRE_TRIGGER 	15 //	/* 10 ms before from event 16, ADC delay 1173200 */
#define	EVENT_FA_10KHZ				31
#define	EVENT_10HZ					30


// 01/25/2014
typedef struct {
	Xuint32 sp 				: 29;		//[28:0]
	Xuint32 Led		 		: 1;		//[29]
	Xuint32 localRst   		: 1;		//[30]
	Xuint32 gtx_rst			: 1;		//[31]
}evr_reg20;
evr_reg20 	O_EVR_MASK_reg20;

void EVR_Reg20_mask(Xuint32 mask)
{
	*(volatile unsigned int*)(XPAR_EVR_0_BASEADDR +  (20 * 4) ) = mask;
}


//
// EVR address map
//
//XPAR_EVR_0_BASEADDR
//XPAR_EVR_0_MEM0_BASEADDR
//XPAR_EVR_0_MEM1_BASEADDR
//XPAR_EVR_0_MEM2_BASEADDR
void	EvrInit()
{
	int	trig1_width;

	evr_gtx_reset(0, 1);
	sleep(500);

	//EvrSeqRamSetup(15,  EV_INTRRUPT_ENABLE + 1);		/* Trig 0 : 10 Hz */
	LTB_EventCode2Trig0( EVENT_LINAC_PRE_TRIGGER, 1);

	EvrSeqRamSetup(EVENT_FA_10KHZ, 0x02);		/* Trig 1 : 10 kHz */
	EvrSeqRamSetup(32,  0x04);		// Trig 2
	EvrSeqRamSetup(EVENT_10HZ,  0x08);			// Trig 3
	
	EvrSeqRamSetup(46,  0x20);		// Trig 5, PM trig   03/03/15 Not OK
	EvrSeqRamSetup(45,  0x40);		// Trig 6, PM reset, 03/03/15 OK
	//trig 7
	EvrSeqRamSetup(33, 0x80);		/* trig 7 for Soft event */

	//TS interrupt
	//EvrSeqRamSetup(0x7D, 0x8000);	/* 1 Hz Timestamp Trig 3 */

	trig1_width = (int)(DFE_CONTROL_REGS->sysInfo[5]);
	if( trig1_width < 1 || trig1_width > 3000) trig1_width = 1225;
	xil_printf("EVR trigger width is : %d\r\n", trig1_width);

	//Channel 0 delay, Width
	EvrDelayUsControl(0, 1);
	//EvrDelayUsControl(1, 160);	//01/08/14 find that give 160 for cell controller interface
	EvrDelayUsControl(1, 1 );
	EvrDelayUsControl(2, 5);
	EvrDelayUsControl(3, 1);
	EvrDelayUsControl(5, 1);
	EvrDelayUsControl(6, 1);
	EvrDelayUsControl(7, 1);

	
	EvrPulseWidthNsControl(0, 5000000);		/* 8 ms */
	EvrPulseWidthNsControl(1, trig1_width );		/* 8n * n */
	EvrPulseWidthNsControl(2, 12);		/* 8n * n */
	EvrPulseWidthNsControl(3, 1000);		/* 8us */
	EvrPulseWidthNsControl(5, 10000000);
	EvrPulseWidthNsControl(6, 10000000);	// soft trig
	EvrPulseWidthNsControl(7, 10000000);	/* soft trigger event */
	
	Set_uI32_PLB( XPAR_EVR_0_BASEADDR + 4*17, 0x04 );	//0100
}

/*
 *
 */
void	EvrSeqRamSetup( int ev_code, unsigned int trig_out_no) 
{	
	ev_code &= 0xFF;	/* 256 address for event code */
	Set_uI32_PLB( XPAR_EVR_0_MEM0_BASEADDR + (4 * ev_code),  trig_out_no );
}


/*
 *	Channel  delay, 8 ns delay
 *	zero delay value is not output pulse so give minimum 1(8ns)
 */
void	EvrDelayUsControl(int chan, int dly_us )
{
	float	tmp;
	
	if(dly_us <= 1 ) tmp = 1;
	else tmp = (int) (dly_us/0.008);
	Set_uI32_PLB( XPAR_EVR_0_BASEADDR + (chan * 8), tmp );
}

void	EvrDelayNsControl(int chan, unsigned int dly_ns )
{
	if(dly_ns <= 0) dly_ns = 1;
	else
		Set_uI32_PLB( XPAR_EVR_0_BASEADDR + (chan * 8), dly_ns );
	xil_printf("Evr Dly %d = %d ns \r\n", chan, dly_ns);	
}

/*
 *
 */
void	EvrPulseWidthControl(int chan, int us)
{
	float	tmp;
	
	tmp = (int) (us/0.008);
	Set_uI32_PLB( XPAR_EVR_0_BASEADDR + (chan * 8 + 4) , tmp );
}

void	EvrPulseWidthNsControl(int chan, unsigned int ns)
{
	Set_uI32_PLB( XPAR_EVR_0_BASEADDR + (chan * 8 + 4) , ns );
}


/*
 *
 */
void	EvrPulseOutputPolarityControl(int chan, int value)
{		
}


#define	TRIG0	0x1
/*
 *
 */
void	LTB_EventCode2Trig0(int evt_no, int enable)
{
int	Index, iReg;	
	//first clear event map for trig 0
/*	
	for(Index=0; Index<255; Index++) {
		iReg = Get_uI32_PLB( XPAR_EVR_0_MEM0_BASEADDR + (4 * Index) );
		if(iReg & 0x1 == TRIG0)  
			Set_uI32_PLB( XPAR_EVR_0_MEM0_BASEADDR + (4 * Index), 0);
	}
*/	
	//Re setting new event code, Event code must be "1 - 244"
	if(evt_no > 0 && evt_no < 255 ) {
		xil_printf("Trig 0 Event code is %d, mode %d \r\n", evt_no, enable );
		if(enable == 0x1)
			EvrSeqRamSetup(evt_no & 0xFF,  EV_INTRRUPT_ENABLE + 1);		/* Trig 0 : for system trigger */
		else
			EvrSeqRamSetup(evt_no & 0xFF, 0);
	}
	else {
		xil_printf("!!!Event code is %d,  Not acceptable \r\n", evt_no);
	}

}	

/*
 *
 */
void	EvrGetTimeStamp()
{
	unsigned int ts_sec, ts_off, tl_l_sec, ts_l_off;
	ts_sec   = Get_uI32_PLB( XPAR_EVR_0_BASEADDR + (24 * 4) );
	ts_off   = Get_uI32_PLB( XPAR_EVR_0_BASEADDR + (25 * 4) );
	tl_l_sec = Get_uI32_PLB( XPAR_EVR_0_BASEADDR + (26 * 4) );
	ts_l_off = Get_uI32_PLB( XPAR_EVR_0_BASEADDR + (27 * 4) );	
	
	xil_printf("evr_ts = %d:%d,  %d:%d\r\n", ts_sec, ts_off, tl_l_sec, ts_l_off);
}


/*
 *  linux
 *		$date +%s
 *
 *	Get time stamp from EVG
 *		Time stemp second  - 32-bit
 *		Time stamp offset  - 32-bit
 */
int	GetEvRTime()
{
	time_t timestamp;
	unsigned int size;
	char	tbuf[64];
	char* c_time=tbuf;

	Ts_t *evrTimsstamp;

	evrTimsstamp->ts_seconds = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*24);
	evrTimsstamp->ts_offset  = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*25);
	evrTimsstamp->ts_seconds -= (60*60* 3) ;	//for Easten time;

	timestamp = (time_t)(evrTimsstamp->ts_seconds);
	c_time = ctime(&timestamp);
	printf ("Time stamp : %s\r\n", c_time);
	xil_printf("Ts sec=0x%x , offset=%d(0x%x)\r\n\r\n", evrTimsstamp->ts_seconds, evrTimsstamp->ts_offset, evrTimsstamp->ts_offset);

	
#if(AAA)	
	time_t  timeVal = time( (time_t*)NULL );
	ptr = ctime(&timeVal);
	//gettimeofday(&timeVal, NULL);
	strcpy(str, &ptr[11]);
		/* Fri Sep 13 00:00:00 1986\n\0 */
		/* 0123456789012345678901234 5  */
	//snprintf(str+8, sizeof(str)-8, ".%06ld", tv.tv_usec);
	xil_printf("\r\ntime=%s\r\n", str);
#endif
}



/*
 *
 */
int	TsCalc( int sec, int nsec)
{
#if 0
	time_t timestamp;
	unsigned int size;
	char	c_time_buf[64];
	char* 	c_time=c_time_buf;

	Ts_t *evrTimsstamp;

	evrTimsstamp->ts_seconds = sec - (60*60*5);	//five hour
	evrTimsstamp->ts_offset  = nsec;
	//evrTimsstamp->ts_seconds -= 14400; //(60*60*4) for Easten time;

	timestamp = (time_t)(evrTimsstamp->ts_seconds);
	c_time = ctime(&timestamp);
	printf ("%s (%d)\r", c_time, strlen(c_time) );
	xil_printf("Timestamp Ts sec=%d(0x%x) , offset=%d(0x%x)\r\n", evrTimsstamp->ts_seconds, evrTimsstamp->ts_seconds,
			evrTimsstamp->ts_offset, evrTimsstamp->ts_offset);

#endif
}




/*
 * System bootup time
 */
int	GetBootUpTime()
{
	time_t timestamp;
	unsigned int size;
	char	tbuf[64];
	char* c_time=tbuf;

	Ts_t *evrTimsstamp;

	evrTimsstamp->ts_seconds = pSrvTxDdrOffAddr->data[BOOT_TIME];
	evrTimsstamp->ts_offset  = 0;
	evrTimsstamp->ts_seconds -= (60*60* 3) ;	//for Easten time;

	timestamp = (time_t)(evrTimsstamp->ts_seconds);
	c_time = ctime(&timestamp);
	printf ("Bootup time : %s\r\n", c_time);

}



/*
 *
 *
 */
int timestamp()
{
    time_t ltime;
    struct tm *Tm;

    ltime=time(NULL);
    Tm=localtime(&ltime);

    printf("[%d] %d %d %d, %d:%d:%d",
            Tm->tm_wday, /* Mon - Sun */
            Tm->tm_mday,
            Tm->tm_mon+1,
            Tm->tm_year=1900,
            Tm->tm_hour,
            Tm->tm_min,
            Tm->tm_sec);
    return 0;
}




/*
 * 9/20/13
 */
void	cmd_EVR_DelayControls(int argc, char *argv[])
{
	int trig_no,  delay_ns;

	trig_no = strtol(argv[1], (char **)0, 10);
	delay_ns = strtol(argv[2], (char **)0, 10);
	if(trig_no==0 && delay_ns == 0) {
		xil_printf("Usage: ed [trig NO] [delay], 1 1000 for sdi trig control\r\n");
	}else
		EvrDelayNsControl(trig_no, delay_ns );
}


void	cmd_Console_GetTrigSet(int argc, char *argv[])
{
int	dly, width;

	dly   = Get_I_DDR2( XPAR_EVR_0_BASEADDR + (1 * 8)  );		//8
	width = Get_I_DDR2( XPAR_EVR_0_BASEADDR + (1 * 8 + 4) );	//12
	xil_printf("Evr trig1(SDI trig) : dly = %d (ns), width = %d (ns)\r\n", dly*8, width*8);
}



void	EvrTrig1_DlyCtrl(int a, Xuint32 value)
{
	EvrDelayNsControl(1, value);
}

//Used for CC packet delay
void	EvrTrig1_WidthCtrl(int a, Xuint32 value)
{
	EvrPulseWidthNsControl(1, value);
}


void	evr_gtx_reset(int a, int data)
{
	O_EVR_MASK_reg20.gtx_rst  = 1&data;
	O_EVR_MASK_reg20.localRst = 1;
	O_EVR_MASK_reg20.Led = 1;
	EVR_Reg20_mask (* ((Xuint32 *) & O_EVR_MASK_reg20) );
	sleep(100);
	xil_printf("evr gtx reset...\r\n");
	O_EVR_MASK_reg20.gtx_rst = 0;
	O_EVR_MASK_reg20.localRst = 0;
	O_EVR_MASK_reg20.Led = 0;
	EVR_Reg20_mask (* ((Xuint32 *) & O_EVR_MASK_reg20) );
}

void	cmd_Console_EvrGtxReset(int argc, char *argv[])
{
	evr_gtx_reset(0,1);
}

//
void	cmd_Console_CcPacketDelay(int argc, char *argv[])
{
	int	width;
	width = strtol(argv[1], (char **)0, 10);
	if(width > 1) {
		EvrPulseWidthNsControl(1, width);
	}
	else {
		xil_printf("Delay values [1 - 5000] \r\n");
	}
}


/*
 *	31 : for 10 kHz
 *	other number for test , 1 Hz, 0.5 Hz ...
 */
void	Evr_SetFdbk_EventCode ( int a, int ev_code)
{
	ev_code &= 0xFF;	/* 256 address for event code */
	Set_uI32_PLB( XPAR_EVR_0_MEM0_BASEADDR + (4 * ev_code),  0x2 );
}

void	cmd_Console_Evr_SetFdbk_EventCode(int argc, char *argv[])
{
	int	ecode;
	ecode = strtol(argv[1], (char **)0, 10);
	Evr_SetFdbk_EventCode(0, ecode);
}
