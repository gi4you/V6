/*
 * pscEpics_sock_tx.c
 *
 *  Created on: Sep 12, 2013
 *      Author: kha
 *
 *      read_regs_10Hz_rate() - register values every 5 Hz rate
 *      pscEpicsProcess_tx_request() - DDR waveform data
 *
 *
 *	11/07/14
 *		pSrvTxDdrOffAddr->data[250] = 0;  epics db address (250x4=1000)for cod y error count
 *		Y cod delta value waveform record
 *		zero error count
 *
 *	11/21/2014
 *		pSrvTxDdrOffAddr->data[251] for BM bpm number of errors count
 *		if number of errors bigger than 5 it assume one cell bpm's wrong.
 *
 *	06/28/16
 *		dcct_sys_fault detected ? beam dump
 *
 *	07/10/15
 *		DFE_CONTROL_REGS->ioReg[72]  if set number of BM bpm's zero
 *		DCCT PLC system fault ? beam dump
 *
 *	11/06/2015
 *		if(RF_TripStatus == 0)  added for uBlaze trip protection, if already triped then bypass
 *
 *
 *	01/24/17
 *		uBlze trip when ID dump enabled
 *
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

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

#include "cmd_modbus.h"

#include "ssp_eeprom.h"
#include "fofb.h"

#include "mps.h"


///////////////////////////////////
// OPERATION 1 for real Install
//           0 for Lab test
//////////////////////////////////
#define	OPERATION			1
#define	LAB_TEST			0

//#define	OPERATION_MODE		OPERATION
#define	OPERATION_MODE		LAB_TEST
#define	DCCT_PLC_READ		OPERATION_MODE	//1: Operation, 0: Lab






#define	MAX_BM_BPM			180
#define	MAX_ID_NUM			60

/* db data */
#define	DATA_ADDR_BM_ZERO_ERR_CNT		250	//1000
#define	DATA_ADDR_BM_N_OF_ERR_CNT		251	//1004
#define	DATA_ADDR_BM_N_OF_FAULT_CNT		252	//1008


#define	PM_FREE_RUN				0x8
#define	PM_DETECTED				0
#define	FINAL_FREE_DUMP_OUT		0x20000		//sysStatus[17]
#define	SYS_RF_DUMP_STS_BIT		0x20
#define CANTING_STATUS_BIT		0x10000000	//sysStatus[28]


#define	PLC_WD_ERR_CNT_VAL		30
#define	UBLAZE_ERR_CNT			50
#define	UBLAZE_ERR_CNT_WAIT		200		// 0.2
#define	PM_WFM_LENGTH_SEC		20100	// 2 seconds


// Number of PLCs Installed
//          F                 F                 F                   7
// #3, #5, #10, #11, #23, #28, #8, #18, #16, #17,   #4, #12,   "#21, #19, #02"
//#define	CELL_PLC_WD_MASK		0x3FF
#define	CELL_PLC_WD_MASK		0x7FFF



#define	EVR_10KHZ_TRIG_TIMEOUT_REG	11
#define	SDI_SFP_LOSS_REG			4
#define	SDI_CW_ERROR_REG			20
#define	SDI_CCW_ERROR_REG			21


// uBlaze error code
//record(longin, "SR:C31-{AI}ID:UBLAZE_ERR-I")
//record(mbbi, "SR:C31-{AI}UBLAZE_ErrorSum-Sts")
#define ERR_OK				0
#define ERR_DCCT_PLC_WD_TO	0x1
#define	ERR_EVR_TO			0x2
#define	ERR_SFP_LOSS		0x4
#define ERR_SDI				0x8
#define ERR_CELL_PLC_WD_TO	0x10
#define	ERR_DCCT_FAULT		0x20
#define	ERR_ZERO_POSITION	0x40
#define ERR_CANTING_FAULT	0x80

unsigned long	loop_cnt=0;
extern SSP_Config_t SSP_Config;

u16_t tx_port = 20;
int	pm_cnt=0;
int	err_cnt=0;
int	dcct_plc_wd_err_cnt=0;
int cell_plc_wd_err_cnt=0;
int	evr_ts_sec, evr_ts_ns;
int	pm_update_done = 0;


#define Set_u8(addr, val)  		(* (Xuint8 *) 	(addr)) = (val)


Xuint32	pre_pm_ddr_data;
Xuint32	pre_pos_ddr_data;
Xuint32	pre_trig_cnt;
Xint32	pre_cod_y_pos_k0[MAX_BM_BPM];
Xint32	pre_cod_y_pos_k1[MAX_BM_BPM];
Xint32	pre_cod_y_pos_k2[MAX_BM_BPM];
Xint32  pre_cod_y_pos_err[MAX_BM_BPM];
Xint8  	pre_cod_y_pos_err_mask[MAX_BM_BPM];
Xint32	beam_0_2mAIsValid, beam_50m_AIsValid, uBlaze_err_code, RF_TripStatus;
Xuint32	pos_y_delta_zero_cnt = 0;


void long2bin(Xuint32 mask_set);
static int lwip_writeall(int s, const void *data, int size);


/*
 *
 */
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



double rms(float *v, int dim)
{
int i;
double rval = 0;
	for ( i=0; i<dim; i++ )
		rval += v[i]*v[i];
	return sqrt(rval/(double)dim);
}


/*
 *
 *	Call from thread_modbus.c
 *	every 5 Hz
 *
 *	SDI
 *	EVR
 *	EEPROM
 */
int	read_regs_10Hz_rate()
{
	int	i;
	int	cw_loss, ccw_loss;
	Xuint32 reg23, reg24;
	int	dcct_plc_wd_timeout;
	int LinkStatus, sfp1, sfp2;
	int	dump_status;
	int	dcct_sys_fault;
	int canting_fault;

	///////////////////////////////////////////////////////////
	if(mps_plb_Reg13.bm_bpm_err_cnt == 1006) goto __end_loop__;

	//check beam current status 0.2 mA
	//beam_0_2mAIsValid = pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+23] & 0x08000000;	//[27] [2]
	// 2 mA status
	beam_0_2mAIsValid = pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+23] & 0x4;		//[27] [2]   , 0100
	beam_50m_AIsValid = pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+23] & 0x8;		//50 mA
	RF_TripStatus     = pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+23] & SYS_RF_DUMP_STS_BIT;		//1000  RF dump status

	// reg28 input is BM BPM error count
	pSrvTxDdrOffAddr->data[DATA_ADDR_BM_N_OF_FAULT_CNT] = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (28*4));
	/*
	if(DFE_CONTROL_REGS->ioReg[66] >= pSrvTxDdrOffAddr->data[DATA_ADDR_BM_N_OF_FAULT_CNT]) {
	}
	*/

	///////////////////
	//1. SDI Registers
	//////////////////
	//Get_uInt32_DDR2(XPAR_SDI2_CC_BASEADDR+(11*4));   -- 10 kHz trigger timeout
	//18 - cw rx fraim error
	//19 - ccw rx fraime error
	for(i=0; i<32; i++) {
		pSrvTxDdrOffAddr->data[DB_SDI_OFFSET+i] = Get_uInt32_DDR2(XPAR_SDI2_CC_BASEADDR+ (i*4) );
	}
	pSrvTxDdrOffAddr->data[4] =  pSrvTxDdrOffAddr->data[4] & 0xFFFF;
	pSrvTxDdrOffAddr->data[32] = (Get_uInt32_DDR2(XPAR_SDI2_CC_BASEADDR+ (4*4)) >> 16) & 0xFFFF;
	pSrvTxDdrOffAddr->data[33] = Get_uI32_PLB( XPAR_EVR_0_BASEADDR + (24 * 4) );	//ts sec
	pSrvTxDdrOffAddr->data[34] = Get_uI32_PLB( XPAR_EVR_0_BASEADDR + (25 * 4) );	//ts offset
	//xil_printf("Reg15=%d\r\n", pSrvTxDdrOffAddr->data[15]);

	//2. FOFB start 50
	//plb_r_reg20 = {BM_LATCHED_OUT, 17'd0, BM_IL_ADDRESS};	// 1+17+14=32
	pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+20] = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (20*4));				// BM Interlock Status
	pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+22] = Get_uInt32_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR+ (22*4) );			// reg22
	pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+23] = Get_uInt32_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR+ (23*4) );			// Sys status
	reg24 = Get_uInt32_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR+ (24*4) );
	//pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+24] = Get_uInt32_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR+ (24*4) ) & 0xFFFF ;	//ExtDin_pin
	// reg24 is digital input
	pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+24] = reg24;	// & 0x7FFFF;
	pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+27] = Get_uInt32_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR+ (27*4) );			// FPGA Version

	//Digital Input status
//	pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+33] = (Get_uInt32_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR+ (24*4) ) >> 16) & 0xFFFF;

	//3. EEPROM 38..
	for(i=0; i<7; i++) {
		pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+i] = (Xuint32) (DFE_CONTROL_REGS->sysInfo[i]);
	}

	//4. EVR register read
	//for(i=0; i<31; i++) {
	//	pSrvTxDdrOffAddr->data[DB_EVR_OFFSET+i] = Get_uI32_PLB( XPAR_EVR_0_BASEADDR + (i * 4) );
	//}
	pSrvTxDdrOffAddr->data[DB_EVR_OFFSET+2]  = Get_I_DDR2( XPAR_EVR_0_BASEADDR + (2*4)  );	//8
	pSrvTxDdrOffAddr->data[DB_EVR_OFFSET+3]  = Get_I_DDR2( XPAR_EVR_0_BASEADDR + (3*4) );	//12  trig 1 width, packet delay 163
	//timestamp
	evr_ts_sec =  Get_I_DDR2( XPAR_EVR_0_BASEADDR + (24*4)  );	//ts sec
	evr_ts_ns  =  Get_I_DDR2( XPAR_EVR_0_BASEADDR + (25*4) );	//ts offset
	pSrvTxDdrOffAddr->data[DB_TIME_STAMP+0]  = evr_ts_sec;
	pSrvTxDdrOffAddr->data[DB_TIME_STAMP+1]  = evr_ts_ns;

	//pSrvTxDdrOffAddr->data[DB_TIME_STAMP+0]  = Get_I_DDR2( XPAR_EVR_0_BASEADDR + (24*4)  );	//ts sec
	//pSrvTxDdrOffAddr->data[DB_TIME_STAMP+1]  = Get_I_DDR2( XPAR_EVR_0_BASEADDR + (25*4) );	//ts offset
	//ts latched
	pSrvTxDdrOffAddr->data[DB_TIME_STAMP+2]  = Get_I_DDR2( XPAR_EVR_0_BASEADDR + (26*4)  );	//ts sec
	pSrvTxDdrOffAddr->data[DB_TIME_STAMP+3]  = Get_I_DDR2( XPAR_EVR_0_BASEADDR + (27*4) );	//ts offset


	//pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+7] = 77;
	//pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+8] = 78;
	pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+9]  = 79;
	pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+10] = uBlaze_err_code;	//192 "SR:C31-{AI}ID:UBLAZE_ERR-I"
	pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+11] = pm_update_done;		// end of PM update

#if 0
	/*
	 * regSel 2 is Gap status
	 */
	FOFB_PLB_CS1.regSel = 2;
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	//read GAP status
	for(i=0; i<64; i++) {
		plb_Reg7Control_1.DacChASet = i;	//select address
		Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
		data8[i] = (char)Get_I_DDR2(READ_K_REG);	//read register
	}

#endif
	///////////////////////
	//5. SysMonData
	// 170+i
	FOFB_PLB_CS1.regSel = 7;
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	//Read GAP status
	//0--30
	for(i=0; i<30; i++) {
		plb_Reg7Control_1.DacChASet = i+16;	//select address
		Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
		pSrvTxDdrOffAddr->data[DB_IL_OFFSET+i] = Get_I_DDR2(READ_K_REG);	//read register start 170
		//xil_printf("%d, %d\r\n", (DB_IL_OFFSET+i)*4, i);
	}

	//
	/* 0ffset 170+30   epics db 800
	 * SysMonData[5:0] sfp eeprom data writing from time_thread
	 *
	 * [6]  IDAI_BeamDumpControlReg[31:0];	206 * 4 = 824
	 * [7]  IDAI_BeamDumpControlReg[63:32];
	 * [8]	Gap_Status[31:0];				832 Aie:Gs_0-I
	 * [9]	Gap_Status[63:31];					Aie:Gs_1-I
	 * [10]
	 * [11]
	 * [12] evr_0_TsLatched[31:0];
	 * [13] evr_0_TsLatched[63:32];
	 * [14]	TS_time[31:0];	nsec		214 * 4 = 856
	 * [15]	TS_time[63:32];	sec
	 */
	//for(i=0; i<16; i++) {
	for(i=0; i<12; i++) {
		plb_Reg7Control_1.DacChASet = i;	//select address
		Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
		pSrvTxDdrOffAddr->data[DB_IL_OFFSET+30+i] = Get_I_DDR2(READ_K_REG);	//read register
		//xil_printf("%d, %d\r\n", DB_IL_OFFSET+30+i, i);
	}
	plb_Reg7Control_1.DacChASet = 12;	
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	pSrvTxDdrOffAddr->data[DB_IL_OFFSET+30+13] = Get_I_DDR2(READ_K_REG); //212
	plb_Reg7Control_1.DacChASet = 13;
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	pSrvTxDdrOffAddr->data[DB_IL_OFFSET+30+12] = Get_I_DDR2(READ_K_REG); //213
	plb_Reg7Control_1.DacChASet = 14;
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	pSrvTxDdrOffAddr->data[DB_IL_OFFSET+30+15] = Get_I_DDR2(READ_K_REG); //214
	plb_Reg7Control_1.DacChASet = 15;
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	pSrvTxDdrOffAddr->data[DB_IL_OFFSET+30+14] = Get_I_DDR2(READ_K_REG); //215
	
	//
	// SysMonData[47]..63
	// 170 + 47 = 217 *4 = db[868]
	/*
	0 SysMonData[47] <= Aie_bm_o[31:0];		db[868]
	1 SysMonData[48] <= Aie_bm_o[63:32];	db[872]
	2 SysMonData[49] <= Aie_A_o[31:0];		db[876] Aie:A0-I
	3 SysMonData[50] <= Aie_A_o[63:32];		db[880]	Aie:A1-I
	4 SysMonData[51] <= Aie_C_o[31:0];		884 Aie:C0-I
	5 SysMonData[52] <= Aie_C_o[63:32];
	6 SysMonData[53] <= Aie_D_o[31:0];		892 Aie:D0-I
	7 SysMonData[54] <= Aie_D_o[63:32];		896
	8 SysMonData[55] <= aie_DumpStatus[31:0];		900 Aie:Dump-D0-I
	9 SysMonData[56] <= aie_DumpStatus[63:32];		904 Aie:Dump-D1-I
	10 SysMonData[57] <= PhotonShutters_Status_info[31:0];
	11 SysMonData[58] <= PhotonShutters_Status_info[63:32];
	12 SysMonData[59] <= IDAI_RunStopStatusReg[31:0];
	13 SysMonData[60] <= IDAI_RunStopStatusReg[63:32];
	14 SysMonData[61] <= BM_PhotonShutters_Status_info[31:0];
	15 SysMonData[62] <= BM_PhotonShutters_Status_info[63:32];
	//
	16 SysMonData[63] <= FaPmTrigCnt_out;				$(P)Aie:PmFaTrigCnt-I
	17 SysMonData[64] <= CantedMagnets_Status[31:0];
	18 SysMonData[65] <= CellPLCWdStatus;			//09/22/14
	19 SysMonData[66] <= CellPLC_IdBeamDump_Status;
	20 SysMonData[67] <= CellPLC_BmBeamDump_Status;
	21 SysMonData[68] <= ID_BpmPosValueStatus[31:0];
	22 SysMonData[69] <= ID_BpmPosValueStatusReg2[31:0];
	23 SysMonData[70] <=							960
	24 SysMonData[71] <=							964
	25 SysMonData[72] <= Cell_PLC_WD_Status;		242*4 = 968  //11/22/14
	26 SysMonData[73] <= cell_plc_wd_timeout;		243x4 = 972
	27		SysMonData[74] <= {20'd0, ext_dout_buf}	976

	28		SysMonData[75] <= x_orbit_rms			980
	29		SysMonData[76] <= y_orbit_rms;			984
	30		SysMonData[77] <= TS_time[63:32];	//sec	988
	31		SysMonData[78] <= TS_time[31:0];	//nsec		992
	32		SysMonData[79] <= bm_xx_pos_out_cnt				996


	#### must assign different address because uBlaze used 1000,1004, 1008
	//08/08/2016 assigned to new address for EPICS interface
	33		SysMonData[80] <= bm_yy_pos_out_cnt				440  _  110
	34		SysMonData[81] <= `FIRMWARE_VER;				444
	35		SysMonData[82] <= CantedMagnets_Status[31:0];	448
	36		SysMonData[83] <= 32'h00000083;					452
	37		SysMonData[84] <= CM_SdiRawBitStatus_Reg[31:0];	456
	//
	38		SysMonData[85] <= 32'h00000085;					460


            // 9/7/22 change sec/nsec register
            SysMonData[86] <= ID_FaultTimestampOut[63:32];	//sec
			SysMonData[87] <= ID_FaultTimestampOut[31:0];	//nsec
			SysMonData[88] <= BM_FaultTimestampOut[63:32];	//sec
            SysMonData[89] <= BM_FaultTimestampOut[31:0];	//nsec

            //

            SysMonData[90] <= 32'd90;
            SysMonData[91] <= 32'd91;
            SysMonData[92] <= 32'd92;
            SysMonData[93] <= 32'd93;
            SysMonData[94] <= 32'd94;
            SysMonData[95] <= 32'd95;
            SysMonData[96] <= 32'd96;

	 */
	//data[ 217 .. 264 ]
	for(i=0; i<32; i++) {
		plb_Reg7Control_1.DacChASet = i+47;	//select address  47 - 78
		Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
		pSrvTxDdrOffAddr->data[DB_IL_OFFSET+47+i] = Get_I_DDR2(READ_K_REG);	//read register 25
		//xil_printf("%d, %d : 0x%x\r\n", DB_IL_OFFSET+47+i, plb_Reg7Control_1.DacChASet, pSrvTxDdrOffAddr->data[DB_IL_OFFSET+47+i]);
	}
	// 170 + 84 = 254
	// 217 .....
	// DB 868 .....

	// 08/08/2016 added for new address assignment for epics interface.
	//pSrvTxDdrOffAddr->data[DB_TIME_STAMP+10]
	// from  33		SysMonData[80]
	//data[110 ... 119]
	//for(i=0; i<10; i++) {  //80 - 89
	//data[110 ... 125]
	for(i=0; i<16; i++) {    //80 - 95    //09/07/22
		plb_Reg7Control_1.DacChASet = i+80;	//select address  80 - 89
		Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
		pSrvTxDdrOffAddr->data[DB_TIME_STAMP+10+i] = Get_I_DDR2(READ_K_REG);	//read register 25
		//xil_printf("%d, %d : 0x%x\r\n", DB_TIME_STAMP+10+i, plb_Reg7Control_1.DacChASet, pSrvTxDdrOffAddr->data[DB_TIME_STAMP+10+i]);
	}

	// read sysStatus for RF dump status
	reg23  = Get_uInt32_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR+ (23*4));
	dump_status = reg23 & FINAL_FREE_DUMP_OUT ;	//[17]


	if(mps_plb_Reg13.bm_bpm_err_cnt == 1010)
	{
		xil_printf("reg23=0x%x : 0x%X\r\n", reg23, dump_status);
	}

	/*
	 * 0x20000 : normal
	 * 0x00000 : dump
	 *
	 */
#if (OPERATION_MODE == OPERATION)

	/*
	 *  12/03/2014  test
	 *	CELL PLC Watch Dog
	 *
	 *	FINAL_FREE_DUMP_OUT   1 : RUN
	 *						  0 : BEAM DUMP
	 *
	 *	AI system FPGA logic is not detected then start uBlaze processing
	 */
	if(dump_status == FINAL_FREE_DUMP_OUT)
	{
		if( (pSrvTxDdrOffAddr->data[243] & CELL_PLC_WD_MASK) != 0) {
			xil_printf("cell plc_wd timeout (%d) = %x\r\n", evr_ts_sec, pSrvTxDdrOffAddr->data[243]);
			displayBits(pSrvTxDdrOffAddr->data[243]);
			xil_printf("\r\n");
			// 04/24/2015 added PLC_WD_ERR_CNT_VAL
			if( (beam_0_2mAIsValid !=0) && (cell_plc_wd_err_cnt > PLC_WD_ERR_CNT_VAL) ) {
				//RF DUMP
				if(RF_TripStatus == 0 )
				{
					if( plb_Reg10_SetBpmLength.id_dump_en == 1) { //AI-ID dump enabled?
						mps_plb_Reg13.fast_dump_ublaz = 1;		//AI-ID DUMP
						mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
						uBlaze_err_code = ERR_CELL_PLC_WD_TO;
					}
				}
			}
			//
			cell_plc_wd_err_cnt += 1;
			if(cell_plc_wd_err_cnt > UBLAZE_ERR_CNT) {
				//mps_OutputLatchClear(0, 1);
				mps_IOWatchDog_Reset(0,0);
				cell_plc_wd_err_cnt = 0;
				xil_printf("cell plc WD reset... (%d)\r\n", evr_ts_sec);
			}
			sleep (UBLAZE_ERR_CNT_WAIT);
		}


		//////////////////////////////
		// Check canting status
		// 8/05/2016 added
		/*
		canting_fault = reg23 & CANTING_STATUS_BIT;	//bit 28
		if( canting_fault != 0) {
			if(RF_TripStatus == 0)
			{
				mps_plb_Reg13.fast_dump_ublaz = 1;
				mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
				uBlaze_err_code = ERR_CANTING_FAULT;
			}
		}
		*/
		////////////////////////////
	}

	//***************************************
	// 11/14/2014
	//  10 kHz evg trigger timeout ? RF dump
	//	SFP loss
	//	CW/CCW gtx frame error
	//***************************************

	//DCCT PLC Watch timeout 12/03/14 tested
#if (DCCT_PLC_READ == 1)

	//DCCT PLC system fault,                  x000:FFFF
	dcct_sys_fault = reg24 & 0x00040000;	// [18]
	if(dcct_sys_fault == 0x00040000)
	{
		if(RF_TripStatus == 0)
		{
			if( plb_Reg10_SetBpmLength.id_dump_en == 1) {
				mps_plb_Reg13.fast_dump_ublaz = 1;				//AI-ID DUMP
				mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
				xil_printf("DCCT PLC system fault [18] : 0x%x\r\n", reg24);
				uBlaze_err_code = ERR_DCCT_FAULT;
			}
		}
	}


	if(dump_status == FINAL_FREE_DUMP_OUT)
	{	//beam is valid ?
		dcct_plc_wd_timeout = reg24 & 0x00020000;	// [17] read watch dog

		if(mps_plb_Reg13.bm_bpm_err_cnt == 1011)
		{
			xil_printf("reg24=0x%x : %X\r\n", reg24, dcct_plc_wd_timeout);
		}

		if(dcct_plc_wd_timeout == 0x00020000)
		{
			// 04/24/2015 added PLC_WD_ERR_CNT_VAL
			// beam is live and error count is over ?
			if( (beam_0_2mAIsValid !=0) && (dcct_plc_wd_err_cnt > PLC_WD_ERR_CNT_VAL) )
			{
				if(RF_TripStatus == 0)
				{
					if( plb_Reg10_SetBpmLength.id_dump_en == 1) {
						mps_plb_Reg13.fast_dump_ublaz = 1;				//AI-ID DUMP
						mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
						uBlaze_err_code = ERR_DCCT_PLC_WD_TO;
					}
				}
			}

			if(mps_plb_Reg13.bm_bpm_err_cnt == 1012)
				xil_printf("Cell 3, DCCT watchdog timeout (%d) = %d\r\n", evr_ts_sec, dcct_plc_wd_err_cnt);

			dcct_plc_wd_err_cnt += 1;
			if(dcct_plc_wd_err_cnt > UBLAZE_ERR_CNT) {
				//mps_OutputLatchClear(0, 1);
				mps_IOWatchDog_Reset(0,0);
				dcct_plc_wd_err_cnt = 0;
				xil_printf("DCCT WD clear... (%d)\r\n", evr_ts_sec);
				TsCalc(evr_ts_sec, 0);
			}

			sleep(UBLAZE_ERR_CNT_WAIT);

		}
	}
#endif

	////////////////////////
	// 11/20/14 Tested OK.
	// EVR timing error checking
	if(dump_status == FINAL_FREE_DUMP_OUT)
	{	//beam is valid ?

		if(pSrvTxDdrOffAddr->data[EVR_10KHZ_TRIG_TIMEOUT_REG] > 1000) {
			err_cnt += 1;
			if(beam_0_2mAIsValid !=0) {
				if(RF_TripStatus == 0)
				{
					if( plb_Reg10_SetBpmLength.id_dump_en == 1) {
						mps_plb_Reg13.fast_dump_ublaz = 1;		//AI-ID DUMP
						mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
						xil_printf("evr trigger timeout BEAM DUMP (%d)\r\n", evr_ts_sec);
						TsCalc(evr_ts_sec, 0);
						uBlaze_err_code = ERR_EVR_TO;
					}
				}
			}
			else
				xil_printf("evr trigger timeout (%d) = %d\r\n", evr_ts_sec, pSrvTxDdrOffAddr->data[EVR_10KHZ_TRIG_TIMEOUT_REG]);

			if(err_cnt > UBLAZE_ERR_CNT) {
				//CC_Sdi_Reset();
				mps_OutputLatchClear(0, 1);
				err_cnt = 0;
			}
			sleep(UBLAZE_ERR_CNT_WAIT);

		}

		//////////////////////
		// 11/20 tested OK
		// SFP loss detection
		cw_loss  = pSrvTxDdrOffAddr->data[SDI_SFP_LOSS_REG];
		ccw_loss = pSrvTxDdrOffAddr->data[32];	//ccw
		if(cw_loss > 100 && ccw_loss > 100) {
			LinkStatus = (Get_I_DDR2(XPAR_SDI2_CC_BASEADDR+(14*4)) >> 24)&0xFF;
			sfp1 = LinkStatus & 0x04;
			sfp2 = LinkStatus & 0x10;
			//xil_printf("SFP6=%d,  SFP5=%d\r\n\r\n", sfp1>>2, sfp2>>4);
			if(sfp1==0 && sfp2 == 0) {
				if(beam_0_2mAIsValid !=0) {
					if(RF_TripStatus == 0)
					{
						if( plb_Reg10_SetBpmLength.id_dump_en == 1) {
							mps_plb_Reg13.fast_dump_ublaz = 1;		//AI-ID DUMP
							mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
							xil_printf("SDI both loss and RF DUMP with BEAM! (%d)\r\n", evr_ts_sec);
							TsCalc(evr_ts_sec, 0);
							uBlaze_err_code = ERR_SFP_LOSS;
						}
					}
				}
				else
					xil_printf("SDI both loss !  (%d)\r\n", evr_ts_sec);

				err_cnt += 1;
				// clear error
				if(err_cnt > UBLAZE_ERR_CNT) {
					//CC_Sdi_Reset();
					mps_OutputLatchClear(0, 1);
					err_cnt = 0;
				}
				sleep(UBLAZE_ERR_CNT_WAIT);

			}

		}

		////////////////////////////
		// 11/20/14 Tested OK
		// CW/CCW ERROR detection
		// Cell controller timing error, SDI error
		if( (pSrvTxDdrOffAddr->data[SDI_CW_ERROR_REG]) > 200 && (pSrvTxDdrOffAddr->data[SDI_CCW_ERROR_REG] > 200) ) {
			{
				if(beam_0_2mAIsValid !=0) {
					if( plb_Reg10_SetBpmLength.id_dump_en == 1) {
						//mps_plb_Reg13.fast_dump_ublaz = 1;		//AI-ID DUMP
						//mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
						xil_printf("Sdi error and RF Dump! (%d)\r\n", evr_ts_sec);
						TsCalc(evr_ts_sec, 0);
						uBlaze_err_code = ERR_SDI;
					}
				}
				else
					xil_printf("Sdi error (%d)  : cw=%d, ccw=%d\r\n", evr_ts_sec, pSrvTxDdrOffAddr->data[SDI_CW_ERROR_REG], pSrvTxDdrOffAddr->data[SDI_CCW_ERROR_REG]);

				err_cnt += 1;
				if(err_cnt > UBLAZE_ERR_CNT) {
					CC_Sdi_Reset();
					//mps_OutputLatchClear(0, 1);
					err_cnt = 0;
				}
				sleep(UBLAZE_ERR_CNT_WAIT);

			}
		}

	}
#endif

__end_loop__:
	return 0;
}



/*
 *
 */
char	txHeadBuf[4096];
void pscEpicsProcess_tx_request(void *p)
{
	int sd = (int)p;
	int totalSendByte=0;
	int	DataSendBytes;
	int	i;
	int	_wfm_size;
	int	bytes_sent;
	int	evr_ts_sec;
	int	pm_status, pm_status23;

	psc_TxHead_t	*psc_TxHead;

	static volatile  int *pDdrRegTxAddr;
	static volatile  int *pDdr0TxAddr;
	static volatile  int *pDdr1TxAddr;

	static volatile  int *pFifoPosTxAddr;
	static volatile  int *pDdrRefOrbitTxAddr;
	static volatile  int *pDdrSrBpmOffsetTxAddr;
	static volatile  int *pDdrRegAllXyPosTxAddr;
	static volatile  int *pDdrGlobalXyPosAddr;		//0xD000:0000
	static volatile  int *pDdrGlobalPmAddr;
	static volatile  int *pDdrAI_MuxDataAddr;

	static volatile unsigned char *ptrEEPROM = DDR2_EEPROM_SYSDATA_BASE;

	int 	count=0;
	int		trig_cnt;
	unsigned int ts_sec, ts_nsec, ts_sec_end;
	Xuint32		trig_ts_sec, pre_trig_ts_sec;
	Xuint32		pos_ddr_data_head, pm_ddr_data_head;
	Xuint32	ddr_start;
	Xuint32	d2; // SysStatus;
	int	nOfBpmErrorCnt=0;

	ts_sec  = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*24);
	ts_nsec = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*25);

	EE_ReadConfig(0,  &SSP_Config);


	//Send eeprom data
#if 1
	int n =  __strlen(SSP_Config.notes);
	memcpy(ptrEEPROM, SSP_Config.notes, n);
	//
	totalSendByte = n;
	Set_u8( DDR2_EEPROM_SYSDATA_BASE-8, 'P');
	Set_u8( DDR2_EEPROM_SYSDATA_BASE-7, 'S');
	Set_uInt16_DDR2(DDR2_EEPROM_SYSDATA_BASE-6, TX_REG_MSG_ID+3 );
	Set_uInt32_DDR2(DDR2_EEPROM_SYSDATA_BASE-4, totalSendByte);
	pDdrRegTxAddr = ( char *) (DDR2_EEPROM_SYSDATA_BASE-8);

	totalSendByte += 8;
	if (lwip_writeall(sd, pDdrRegTxAddr, totalSendByte) != totalSendByte) {
		xil_printf("Closing TX socket place 1 %d\n\r", sd);
		goto __end;
	}

	//ip address
  	sprintf(txHeadBuf, "IP: %d.%d.%d.%d\r\n",	SSP_Config.net_cfg.ip_address[0],
			    SSP_Config.net_cfg.ip_address[1],
			    SSP_Config.net_cfg.ip_address[2],
			    SSP_Config.net_cfg.ip_address[3] );

  	n =  __strlen(txHeadBuf);
  	memcpy(ptrEEPROM, txHeadBuf, n);
	totalSendByte = n;
	Set_u8( DDR2_EEPROM_SYSDATA_BASE-8, 'P');
	Set_u8( DDR2_EEPROM_SYSDATA_BASE-7, 'S');
	Set_uInt16_DDR2(DDR2_EEPROM_SYSDATA_BASE-6, TX_REG_MSG_ID+4 );
	Set_uInt32_DDR2(DDR2_EEPROM_SYSDATA_BASE-4, totalSendByte);
	pDdrRegTxAddr = ( char *) (DDR2_EEPROM_SYSDATA_BASE-8);

	totalSendByte += 8;
	if (lwip_writeall(sd, pDdrRegTxAddr, totalSendByte) != totalSendByte) {
		xil_printf("Closing TX socket place 1 %d\n\r", sd);
		goto __end;
	}
	xil_printf("IP = %s\r\n", txHeadBuf);

#endif
	//end


	xil_printf("pscEpicsProcess TX Waveform port connected... Ts=%d:%d\r\n", ts_sec, ts_nsec);


	pDdrRegTxAddr         = ( int *) (DDR3_TX_REG);
	pDdr0TxAddr           = ( int *) (DDR3_NPI0 - 8);
	pDdr1TxAddr           = ( int *) (DDR3_NPI1);	// - 8);

	pFifoPosTxAddr        = ( int *) (DDR3_ERR_FIFO_BASE);
	pDdrRefOrbitTxAddr	  = ( int *) (BASE_REF_ORBIT);
	pDdrSrBpmOffsetTxAddr = ( int *) (BASE_DDR_SRBPM_OFFSET);	//04/01/2014 added
	//
	pDdrRegAllXyPosTxAddr = (int *) (DDR3_ALLPOS_REG);
	pDdrGlobalXyPosAddr   = (int *) (0xD0000000);		//DDR global  Data
	pDdrGlobalPmAddr	  = (int *) (0xC2000000);

	pDdrAI_MuxDataAddr	  = (int *) (BASE_AIP_MUX_DATA);

	//clear
	for(i=0; i<180; i++) {
		pre_cod_y_pos_k0[i] = 0;
		pre_cod_y_pos_k1[i] = 0;
		pre_cod_y_pos_k2[i] = 0;
		pre_cod_y_pos_err[i] = 0;
		pre_cod_y_pos_err_mask[i] = 0;
	}
	pre_trig_ts_sec = 0;
	pre_trig_cnt = 0;
	pre_pos_ddr_data = 0;
	DFE_CONTROL_REGS->ioReg[62] = 0;	//PM ddr offset address

	pSrvTxDdrOffAddr->data[DATA_ADDR_BM_ZERO_ERR_CNT] = 0;	//cod error counter
	pSrvTxDdrOffAddr->data[DATA_ADDR_BM_N_OF_ERR_CNT] = 0;
	pSrvTxDdrOffAddr->data[DATA_ADDR_BM_N_OF_FAULT_CNT] = 0;

	nOfBpmErrorCnt  = 0;
	uBlaze_err_code = ERR_OK;
	RF_TripStatus   = 0;

	while (1)
	{
		//sleep(20);
		sleep(100);		// reduce scan rate for AA
		//trig timestamp
		evr_ts_sec = trig_ts_sec = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*24);

		///////////////////////
		// 10/08/14 added
		//
		// AI ID MUX DATA
		///////////////////////
		if(mps_plb_Reg13.bm_bpm_err_cnt != 1000) {
			i = mps_mux_fifo_read();
			if(i) {	//if FIFO valid ? send
				DataSendBytes = (2000 * 4);
				// send head
				psc_TxHead = (psc_TxHead_t *)txHeadBuf;
				psc_TxHead->idA     = 'P';
				psc_TxHead->idB     = 'S';
				psc_TxHead->MsgId   = TX_AIP_MUX_MSG_ID;	/* 32-bit address */
				psc_TxHead->bodyLen = DataSendBytes;		// N of DATA

				//1. Send HEAD
				if ((bytes_sent = lwip_write(sd, txHeadBuf, HEAD_SIZE)) != HEAD_SIZE) {
					xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE);
					goto __end;
				}
				//2. send DDR-3 DATA
				if (lwip_writeall(sd, pDdrAI_MuxDataAddr, DataSendBytes) != DataSendBytes) {
					xil_printf("Closing Wfm TX Msg ID= %d, len=%d\n\r", psc_TxHead->MsgId, DataSendBytes );
					goto __end;
				}
			}
		}



		loop_cnt++;


		if(mps_plb_Reg13.bm_bpm_err_cnt == 1013){
			xil_printf("wfm loop = %d\r\n", loop_cnt);
		}

		/////////////////////////////////////
		/// SETTING WAVEFORM from DATABASE
		/////////////////////////////////////
		if(loop_cnt % 300 == 0)   // 10 second update reference values
		{
			//xil_printf(".");
			/////////////////////////////////
			///// SR-BPM X,Y Limit Value
			///// PV : SR:C31-{AI}SR-BPM:AIOLHV-I
			////////////////////////////////
			//
			totalSendByte = 360*4;
			// send head
			psc_TxHead = (psc_TxHead_t *)txHeadBuf;
			psc_TxHead->idA     = 'P';
			psc_TxHead->idB     = 'S';		/* 32-bit address */
			psc_TxHead->MsgId   = TX_FIFO_REF_ORBIT_MSG_ID;		//37
			psc_TxHead->bodyLen = totalSendByte;			// N of DATA

			if ((bytes_sent = lwip_write(sd, txHeadBuf, HEAD_SIZE)) != HEAD_SIZE) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE);
				goto __end;
			}
			//send DDR DATA
			if (lwip_writeall(sd, pDdrRefOrbitTxAddr, totalSendByte) != totalSendByte) {
				xil_printf("Closing Wfm TX Msg ID= %d, len=%d\n\r", psc_TxHead->MsgId, totalSendByte );
				goto __end;
			}


			/////////////////////////////////
			///// Sr-BPM X,y Offset Value
			///// PV : SR:C31-{AI}SR-BPM:AIORHV-I
			////////////////////////////////
			psc_TxHead = (psc_TxHead_t *)txHeadBuf;
			for(i=0; i<SR_BPM_XY_SIZE; i++) {
				psc_TxHead->data[i] = Get_I_DDR2(BASE_DDR_SRBPM_OFFSET+(i*4));
				//if(i<10)
				//	xil_printf("%d\r\n", psc_TxHead->data[i]);
			}
			psc_TxHead->MsgId   = TX_FIFO_SRBPM_OFFSET_MSG_ID;
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}

			// PV : SR:C31-{AI}SR-BPM:MASK-I
			//SR-BPM Mask status read
			totalSendByte = 180 * 4;
			psc_TxHead->bodyLen = totalSendByte;			// N of DATA
			psc_TxHead = (psc_TxHead_t *)txHeadBuf;
			for(i=0; i<180; i++) {
				psc_TxHead->data[i] = Get_I_DDR2(BASE_DDR_SRBPM_MASK_OFFSET+(i*4));
			}
			psc_TxHead->MsgId   = TX_FIFO_SRBPM_MASK_MSG_ID;	//39
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}

			/*
			 * Read AIE-ID Reference waveform
			 // address table from FPGA
					`define	ADDR_H1					0	//AIO_H1
					`define	ADDR_V1					1	//AIO_V1
					`define	ADDR_H2					2
					`define	ADDR_V2					3
					`define	ADDR_H3					4
					`define	ADDR_V3					5
					`define	ADDR_ANGLE_H			6	//AIAL_H
					`define	ADDR_ANGLE_V			7	//AIAL_V
					`define	ADDR_MRAD_S				8
					`define	ADDR_MM_S				9
					`define	ADDR_LOGIC				10
					`define	H1_CHAMBER_OFFSET		11		//added 03/29/14
					`define	V1_CHAMBER_OFFSET		12
					`define	H2_CHAMBER_OFFSET		13		//added 03/31/14
					`define	V2_CHAMBER_OFFSET		14*
			 */
			totalSendByte = MAX_ID_NUM * 4;
			psc_TxHead = (psc_TxHead_t *)txHeadBuf;
			psc_TxHead->idA     = 'P';
			psc_TxHead->idB     = 'S';		/* 32-bit address */
			psc_TxHead->bodyLen = totalSendByte;
			//
			psc_TxHead->MsgId   = TX_AIE_REF_SET_WFM_MSG_ID+0;
			//used cs1
			mps_DpRamOffsetRead(0, &psc_TxHead->data[0]);
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}

			////////////
			psc_TxHead->MsgId   = TX_AIE_REF_SET_WFM_MSG_ID+1;
			//used cs1
			mps_DpRamOffsetRead(1, &psc_TxHead->data[0]);
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}
			////////////
			psc_TxHead->MsgId   = TX_AIE_REF_SET_WFM_MSG_ID+2;
			//used cs1
			mps_DpRamOffsetRead(1, &psc_TxHead->data[0]);
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}
			////////////
			psc_TxHead->MsgId   = TX_AIE_REF_SET_WFM_MSG_ID+3;
			//used cs1
			mps_DpRamOffsetRead(1, &psc_TxHead->data[0]);
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}
			////////////
			psc_TxHead->MsgId   = TX_AIE_REF_SET_WFM_MSG_ID+4;
			//used cs1
			mps_DpRamOffsetRead(4, &psc_TxHead->data[0]);
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}
			////////////
			psc_TxHead->MsgId   = TX_AIE_REF_SET_WFM_MSG_ID+5;
			//used cs1
			mps_DpRamOffsetRead(5, &psc_TxHead->data[0]);
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}
			////////////
			psc_TxHead->MsgId   = TX_AIE_REF_SET_WFM_MSG_ID+6;
			//used cs1
			mps_DpRamOffsetRead(6, &psc_TxHead->data[0]);
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}
			////////////
			psc_TxHead->MsgId   = TX_AIE_REF_SET_WFM_MSG_ID+7;
			//used cs1
			mps_DpRamOffsetRead(7, &psc_TxHead->data[0]);
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}

			////////////
			psc_TxHead->MsgId   = TX_AIE_REF_SET_WFM_MSG_ID+8;
			//used cs1
			mps_DpRamOffsetRead(8, &psc_TxHead->data[0]);
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}
			////////////
			psc_TxHead->MsgId   = TX_AIE_REF_SET_WFM_MSG_ID+9;
			//used cs1
			mps_DpRamOffsetRead(9, &psc_TxHead->data[0]);
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}
			////////////
			psc_TxHead->MsgId   = TX_AIE_REF_SET_WFM_MSG_ID+10;
			//used cs1
			mps_DpRamOffsetRead(10, &psc_TxHead->data[0]);
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}
			////////////
			psc_TxHead->MsgId   = TX_AIE_REF_SET_WFM_MSG_ID+11;
			//used cs1
			mps_DpRamOffsetRead(11, &psc_TxHead->data[0]);
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}
			////////////
			psc_TxHead->MsgId   = TX_AIE_REF_SET_WFM_MSG_ID+12;
			//used cs1
			mps_DpRamOffsetRead(12, &psc_TxHead->data[0]);
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}
			////////////
			psc_TxHead->MsgId   = TX_AIE_REF_SET_WFM_MSG_ID+13;
			//used cs1
			mps_DpRamOffsetRead(13, &psc_TxHead->data[0]);
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}
			////////////
			psc_TxHead->MsgId   = TX_AIE_REF_SET_WFM_MSG_ID+14;
			//used cs1
			mps_DpRamOffsetRead(14, &psc_TxHead->data[0]);
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}
			//xil_printf(",");

		}  // end of % 25
		/////////////////////////////////////////////



		/********************************************
		 * Tx FPGA Calculation result waveform
		 * ******************************************
		 */
		//x cal offset @ s3
		totalSendByte = MAX_ID_NUM * 4;
		psc_TxHead = (psc_TxHead_t *)txHeadBuf;
		psc_TxHead->idA     = 'P';
		psc_TxHead->idB     = 'S';		/* 32-bit address */
		psc_TxHead->MsgId   = TX_AIE_FPGA_CAL_MSG_ID;
		psc_TxHead->bodyLen = totalSendByte;
		//read result data, used cs2
		mps_Read_FPGA_Calc_ResultData ( 10, &psc_TxHead->data[0], MAX_ID_NUM);
		if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
			xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
			goto __end;
		}

		// y cal offset @ s3
		psc_TxHead = (psc_TxHead_t *)txHeadBuf;
		psc_TxHead->idA     = 'P';
		psc_TxHead->idB     = 'S';		/* 32-bit address */
		psc_TxHead->MsgId   = TX_AIE_FPGA_CAL_MSG_ID+1;
		psc_TxHead->bodyLen = totalSendByte;
		//read result data, used cs2
		mps_Read_FPGA_Calc_ResultData ( 11, &psc_TxHead->data[0], MAX_ID_NUM);
		if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
			xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
			goto __end;
		}

		//2: x cal angle
		psc_TxHead = (psc_TxHead_t *)txHeadBuf;
		psc_TxHead->idA     = 'P';
		psc_TxHead->idB     = 'S';		/* 32-bit address */
		psc_TxHead->MsgId   = TX_AIE_FPGA_CAL_MSG_ID+2;
		psc_TxHead->bodyLen = totalSendByte;
		//read result data, used cs2
		mps_Read_FPGA_Calc_ResultData ( 12, &psc_TxHead->data[0], MAX_ID_NUM);
		if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
			xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
			goto __end;
		}

		//y cal angle
		psc_TxHead = (psc_TxHead_t *)txHeadBuf;
		psc_TxHead->idA     = 'P';
		psc_TxHead->idB     = 'S';		/* 32-bit address */
		psc_TxHead->MsgId   = TX_AIE_FPGA_CAL_MSG_ID+3;
		psc_TxHead->bodyLen = totalSendByte;
		//read result data, used cs2
		mps_Read_FPGA_Calc_ResultData ( 13, &psc_TxHead->data[0], MAX_ID_NUM);
		if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
			xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
			goto __end;
		}

		//  ID dump result
		psc_TxHead = (psc_TxHead_t *)txHeadBuf;
		psc_TxHead->idA     = 'P';
		psc_TxHead->idB     = 'S';		/* 32-bit address */
		psc_TxHead->MsgId   = TX_AIE_FPGA_CAL_MSG_ID+4;
		psc_TxHead->bodyLen = totalSendByte;
		//read result data, used cs2
		mps_Read_FPGA_Calc_ResultData ( 14, &psc_TxHead->data[0], MAX_ID_NUM);
		if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
			xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
			goto __end;
		}

		// BP dump result
		psc_TxHead = (psc_TxHead_t *)txHeadBuf;
		psc_TxHead->idA     = 'P';
		psc_TxHead->idB     = 'S';		/* 32-bit address */
		psc_TxHead->MsgId   = TX_AIE_FPGA_CAL_MSG_ID+5;
		psc_TxHead->bodyLen = totalSendByte;
		//read result data, used cs2
		mps_Read_FPGA_Calc_ResultData ( 15, &psc_TxHead->data[0], MAX_ID_NUM);
		if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
			xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
			goto __end;
		}


		///////////////////////////////////////////////////
		//DDR3_ERR_FIFO_BASE  -- RF BPM position waveform
		//TX_FOFO_POS_MSG_ID
		// RF BPM POSITION

		/*
		//read by uBlaze
		.rstb(reset),
		.clkb(clk), 	
		.enb(ram_cs2[8] ), 	
		.addrb(ram_addr2[8:0]), 		
		.doutb( xy_pos_fifo_out ) 	
		*/	
		////////////////////////////////
		/// READ X and Y POSITION
		////////////////////////////////
		if(mps_plb_Reg13.bm_bpm_err_cnt != 1002) {
			//read result data
			fofb_dpram.ram_cs = 8;	//ram_cs2[8]
			fofb_dpram.wr = 0;
			fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
			// read data from DPRAM
			int	data;
			for(i=0; i<MAX_BM_BPM; i++) {
				fofb_dpram.addr = i;	//address
				fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
				//get data
				psc_TxHead->data[i] = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (26*4));
			}

			//RF-BPM X
			totalSendByte = MAX_BM_BPM * 4;
			psc_TxHead = (psc_TxHead_t *)txHeadBuf;
			psc_TxHead->idA     = 'P';
			psc_TxHead->idB     = 'S';		/* 32-bit address */
			psc_TxHead->MsgId   = TX_FIFO_POS_X_MSG_ID;
			psc_TxHead->bodyLen = totalSendByte;			// N of DATA
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}
		}


		//////////////////////////////////
		//
		// RF-BPM Y read waveform and compare any zero position ?
		//	if y(n-1) - y(n) is zero mean data is bpm data update error
		//
		//////////////////////////////////

#if (OPERATION_MODE == OPERATION)
		if(mps_plb_Reg13.bm_bpm_err_cnt != 1003)
		{
			for(i=0; i<MAX_BM_BPM; i++) {
				fofb_dpram.addr = i + MAX_BM_BPM;	// Y address
				fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
				//get data
				psc_TxHead->data[i] = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (26*4));

				// k2 == k1 ? and k2 == k0 ?
				pre_cod_y_pos_err[i] = pre_cod_y_pos_k2[i] - pre_cod_y_pos_k0[i];

				//if( (pre_cod_y_pos_k2[i] == pre_cod_y_pos[i] ) && (pre_cod_y_pos_k2[i] == psc_TxHead->data[i]) ) {
				if( (psc_TxHead->data[i] == pre_cod_y_pos_k0[i] ) && (psc_TxHead->data[i] == pre_cod_y_pos_k1[i] ) && (psc_TxHead->data[i] == pre_cod_y_pos_k2[i] ) ) {
					// y(k-2), y(k-1), y(k)
					nOfBpmErrorCnt += 1;	// N of BPMs
					pSrvTxDdrOffAddr->data[DATA_ADDR_BM_ZERO_ERR_CNT] += 1;
				}
				else nOfBpmErrorCnt = 0;

			}	// end of for loop

			// if zero position bpms more than 5 it may one cell problem
			if(nOfBpmErrorCnt >= DFE_CONTROL_REGS->ioReg[72] )	//07/10/15
			{
				if(beam_50m_AIsValid == 0x8) 	{// BEAM current is > 50 mA then start count
					//
					// 01/28/15 : found sometimes error during injection time
					//
					if(mps_plb_Reg13.enable_bm_zero_detection == 1)
					{
						if(mps_plb_Reg13.fast_dump_ublaz == 0) {
							if(RF_TripStatus == 1) {
								xil_printf("Already RF Dumped...\r\n");
							}
							else
								xil_printf("Many (%d) BPMs Data zero difference RF Dump! (%d) \r\n", nOfBpmErrorCnt, evr_ts_sec);
						}
						if(RF_TripStatus == 0)
						{
							if( plb_Reg10_SetBpmLength.id_dump_en == 1) {
								xil_printf("Send RF Dumped signal from uBlaze...\r\n");
								mps_plb_Reg13.fast_dump_ublaz = 1;		//AI-ID DUMP
								mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
								uBlaze_err_code = ERR_ZERO_POSITION;
							}
						}
					}
				}

			}

		}

#endif

		if(mps_plb_Reg13.bm_bpm_err_cnt != 1004) {
			pSrvTxDdrOffAddr->data[DATA_ADDR_BM_N_OF_ERR_CNT] = nOfBpmErrorCnt;
			//backup bm y position
			for(i=0; i<MAX_BM_BPM; i++) {
				pre_cod_y_pos_k2[i] = pre_cod_y_pos_k1[i];		//k1 -> k2
				pre_cod_y_pos_k1[i] = pre_cod_y_pos_k0[i];				
				pre_cod_y_pos_k0[i] = psc_TxHead->data[i];		//k0 -> k1
			}


			totalSendByte = MAX_BM_BPM * 4;
			psc_TxHead = (psc_TxHead_t *)txHeadBuf;
			psc_TxHead->idA     = 'P';
			psc_TxHead->idB     = 'S';		/* 32-bit address */
			psc_TxHead->MsgId   = TX_FIFO_POS_Y_MSG_ID;
			psc_TxHead->bodyLen = totalSendByte;			// N of DATA
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}
		}

		////////////////////////////////////////
		///// y COD error for bpm fault detect
		////////////////////////////////////////
		if(mps_plb_Reg13.bm_bpm_err_cnt != 1005) {
			for(i=0; i<MAX_BM_BPM; i++) {
				psc_TxHead->data[i] = pre_cod_y_pos_err[i];
			}

			totalSendByte = MAX_BM_BPM * 4;
			psc_TxHead = (psc_TxHead_t *)txHeadBuf;
			psc_TxHead->idA     = 'P';
			psc_TxHead->idB     = 'S';		/* 32-bit address */
			psc_TxHead->MsgId   = TX_POS_Y_DELTA_MSG_ID;
			psc_TxHead->bodyLen = totalSendByte;			// N of DATA
			if ((bytes_sent = lwip_write(sd, txHeadBuf, totalSendByte+HEAD_SIZE)) != HEAD_SIZE+totalSendByte) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+totalSendByte);
				goto __end;
			}
		}


		////////////////////  SEND SDI Data [779:0] ///////////////////////////////
		/*
		 * 	regSel 0 is position
		 * 	4'd0:  rd_reg25 <= xy_pos_Reg[k_addr];
		 *
		 *  PV :  SR:C31-{AI}Pos:AllXy-W
		 *
		 */
		if(mps_plb_Reg13.bm_bpm_err_cnt != 1001) {

			if(FOFB_PLB_CS1.sdiLinkWfmSel == 1) {	//01/20/17 added
				FOFB_PLB_CS1.regSel = 0;	//Live SDI
			}
			else {
				FOFB_PLB_CS1.regSel = 15;	//Latched SDI
			}
			Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));

			DataSendBytes = 780 * 4;
			// send head
			psc_TxHead = (psc_TxHead_t *)txHeadBuf;
			psc_TxHead->idA     = 'P';
			psc_TxHead->idB     = 'S';								/* 32-bit address */
			psc_TxHead->MsgId   = TX_ALL_XY_POS_WFM_MSG_ID;			/* TX_NPI1_MSG_ID */
			psc_TxHead->bodyLen = DataSendBytes;					// N of DATA

			//read position data
			for(i=0; i < 780; i++) {
				plb_Reg7Control_1.DacChASet = i;	//select address
				Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
				psc_TxHead->data[i] = Get_I_DDR2(READ_K_REG);			//read register
				//xil_printf("%d\r\n", psc_TxHead->data[i]);
			}

			if ((bytes_sent = lwip_write(sd, txHeadBuf, DataSendBytes+HEAD_SIZE)) != DataSendBytes+HEAD_SIZE) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, DataSendBytes+HEAD_SIZE);
				goto __end;
			}

		}



		/****************************************************************
		 * BM Interlock Status
		 * regSel 1 is BM interlock status : reg	 [780:0] bm_iL_info;
		 *
		 * PV : NOT IMPLEMENTED at IOC DATABASE
		 *
		 */
#if 0
		FOFB_PLB_CS1.regSel = 1;
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
		//read bending interlock status
		for(i=0; i<780; i++) {
			plb_Reg7Control_1.DacChASet = i;	//select address
			Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
			psc_TxHead->data[i] = Get_I_DDR2(READ_K_REG);	//read register 25
			//pos = Get_I_DDR2(READ_K_REG);
			//Set_I_DDR2(DDR3_ALLPOS_REG+(i*4), pos);
			//xil_printf("%d\r\n", psc_TxHead->data[i]);
		}

		DataSendBytes = 780*4;	//
		// send head
		psc_TxHead = (psc_TxHead_t *)txHeadBuf;
		psc_TxHead->idA     = 'P';
		psc_TxHead->idB     = 'S';								/* 32-bit address */
		psc_TxHead->MsgId   = TX_AIE_BM_IL_STATUS_MSG_ID;		/* TX_NPI1_MSG_ID */
		psc_TxHead->bodyLen = DataSendBytes;					// N of DATA

		if ((bytes_sent = lwip_write(sd, txHeadBuf, DataSendBytes+HEAD_SIZE)) != HEAD_SIZE+DataSendBytes) {
			xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE+DataSendBytes);
			goto __end;
		}

#endif


		///////////////////////////////////////////////////
		// Read DDR-3 0xD0000000 global position Data from DDR memory
		//	each cell 8 bpms x 30 cell
		// 	PV : SR:C31-{AI}Pos:Global
		//
		///////////////////////////////////////////////////
		Xuint32	offset;
		if(mps_plb_Reg13.npi_run == 1)  //if NPI-1 is ENABLED ?
		{
			trig_cnt  = (Get_uInt32_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR+ (22*4) ) >> 24) & 0xFF ;
			offset = 10000 * 4;
			pos_ddr_data_head = Get_I_DDR2(0xD0000000 + offset );	//changed head by trigger
			//read time stamp
			ts_sec  = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*24);
			ts_nsec = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*25);

			if( (pre_pos_ddr_data != pos_ddr_data_head) && (pos_ddr_data_head != 0xFFFFFFFF ))
			{
				pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+8] = 1;	//tx start

				//tx_done = 0;
				if(DFE_CONTROL_REGS->ioReg[60] < 10000 ) _wfm_size = 10000;			//1 sec
				else if(DFE_CONTROL_REGS->ioReg[60] > 100000) _wfm_size = 100000;	//10 sec
				else _wfm_size = DFE_CONTROL_REGS->ioReg[60];
				// wait
				sleep(DFE_CONTROL_REGS->ioReg[60]/10);	//Wait 1 sec ~ 10 sec

				DataSendBytes = (480 * 4) * _wfm_size;	//5 sec, 72,000,000 byte ?
				xil_printf("TX DDR 0xD0000000, size = %d,  %d bytes   trig cnt = %d  ts = %d  Wait...\r\n", _wfm_size, DataSendBytes, ts_sec, trig_cnt);
				// send head
				psc_TxHead = (psc_TxHead_t *)txHeadBuf;
				psc_TxHead->idA     = 'P';
				psc_TxHead->idB     = 'S';								/* 32-bit address */
				psc_TxHead->MsgId   = TX_NPI1_MSG_ID;			/* 53  */
				psc_TxHead->bodyLen = DataSendBytes;					// N of DATA
				//
				if ((bytes_sent = lwip_write(sd, txHeadBuf, HEAD_SIZE)) != HEAD_SIZE) {
					xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE);
					goto __end;
				}
				//send DDR-3 0xD0000000 DATA
				if (lwip_writeall(sd, pDdrGlobalXyPosAddr, DataSendBytes) != DataSendBytes) {
					xil_printf("Closing Wfm TX Msg ID= %d, len=%d\n\r", psc_TxHead->MsgId, DataSendBytes );
					goto __end;
				}


				pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+8] = 0;	//Done
				ts_sec_end  = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*24);		//evr
				ts_nsec = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*25);
				xil_printf("TX DDR npi1 DONE.. %d (s)\r\n", ts_sec_end - ts_sec );
			}
			Set_I_DDR2(0xD0000000 + offset, 0xFFFFFFFF );	//two time's transmitting
			pre_pos_ddr_data = pos_ddr_data_head;
			pre_trig_cnt = trig_cnt;

			// trig count update
			pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+7] = trig_cnt;
		}


		///////////////////////////////////////
		//
		// Read PM position Data from DDR memory
		// DDR-3 0xC2000000
		//
		// PV : SR:C31-{AI}Npi:Pm
		////////////////////////////////////////

		//read PM status
		pm_status    = Get_uInt32_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR+ (22*4)) & 0x8 ;
		pm_status23  = Get_uInt32_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR+ (23*4)) & 0x800 ;

		if( mps_plb_Reg13.npi_0_run == 1 )	//if PM NPI is enabled ?
		{
			if( pm_status23 == PM_DETECTED )		//PM detected
			{
				//xil_printf("pm_status= 0x%x, 0x%x\r\n", pm_status, pm_status23);
				if( pm_update_done == 0) {	//check already transmitter is DONE ?
					xil_printf("pm_status= 0x%x, 0x%x\r\n", pm_status, pm_status23);

					//offset = 10000 * 4;
					//pm_ddr_data_head = Get_I_DDR2(0xC2000000 + offset );

					//if(pre_pm_ddr_data != pm_ddr_data_head && (pre_pm_ddr_data != 0xFFFFFFFF ) )
					{

						d2 = Get_uInt32_DDR2(0xC2000000+(2*4));
						if(d2 == 0) ddr_start = 0xC2000000 + 4;
						else ddr_start = 0xC2000000;

						//10/21/14  PM offset
						pDdrGlobalPmAddr	  = (int *) ( ddr_start + (DFE_CONTROL_REGS->ioReg[62]*1920)  );	// 480 x 4 = 1920
						for(i=0; i<20; i++) {
							sleep(100);
							xil_printf(".");
						}

						xil_printf("\r\n");
						//DataSendBytes = (480 * 4) * PM_WFM_LENGTH_SEC;	// 3sec = 57.6 Mbyte
						DataSendBytes = (480 * 4) * (mps_plb_Reg12.npi_rst_peride + 19200);
						xil_printf("TX 0xC2000000 pm waveform  %d bytes, pm_offset=%d, ddr_offset=%d, %d\r\n",	\
								DataSendBytes, pSrvTxDdrOffAddr->data[DB_IL_OFFSET+47+16], DFE_CONTROL_REGS->ioReg[62]*1920, pm_cnt++);

						// send head
						psc_TxHead = (psc_TxHead_t *)txHeadBuf;
						psc_TxHead->idA     = 'P';
						psc_TxHead->idB     = 'S';								/* 32-bit address */
						psc_TxHead->MsgId   = TX_NPI1_MSG_ID+1;			/*  */
						psc_TxHead->bodyLen = DataSendBytes;					// N of DATA
						//
						if ((bytes_sent = lwip_write(sd, txHeadBuf, HEAD_SIZE)) != HEAD_SIZE) {
							xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE);
							goto __end;
						}
						//send DDR-3 0xC2000000 DATA
						if (lwip_writeall(sd, pDdrGlobalPmAddr, DataSendBytes) != DataSendBytes) {
							xil_printf("Closing Wfm TX Msg ID= %d, len=%d\n\r", psc_TxHead->MsgId, DataSendBytes );
							goto __end;
						}

						xil_printf("!!! DDR PM wfm (npi0) TX %d bytes DONE !!!\r\n", DataSendBytes);
						//mps_OutputLatchClear(0, 1);
					}

					//Set_I_DDR2(0xC2000000 + offset, 0xFFFFFFFF );
					//pre_pm_ddr_data = pm_ddr_data_head;

					pm_update_done = 1;	//waveform Tx Done
				}
			}
			else {
				pm_update_done = 0;
			}
		}
		////////////////////////
__SKIP:
		sleep(1);

	}	//end of while

__end:
	close(sd);
	xil_printf("\r\n***** Closing Wfm Tx socket place %d\n\r", sd);
}



/*
 *
 */
void pscEpicsTx_thread()
{
	int sock, new_sd;
	struct sockaddr_in address, remote;
	int size;

	xil_printf("\r\n**********************************************************\n\r");
	    xil_printf("** pscEpicsTx_thread TCP/IP communication thread port 20 **\n\r");
	    xil_printf("**********************************************************\n\r");


	/* Setup Socket: create */
	if ((sock = lwip_socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return;

	address.sin_family = AF_INET;
	address.sin_port = htons(tx_port);
	address.sin_addr.s_addr = INADDR_ANY;

	/* Setup Socket: bind */
	if (lwip_bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
		return;
	lwip_listen(sock, 5);
	size = sizeof(remote);

	/* accept */
	while (1) {
		if ((new_sd = lwip_accept(sock, (struct sockaddr *)&remote, (socklen_t *)&size)) > 0)
				sys_thread_new("pscEpicsTx_server", pscEpicsProcess_tx_request,(void*) new_sd, THREAD_STACKSIZE, 3);
	}
}


//
void long2bin(Xuint32 mask_set)
{
int	addr;
char mask_buf[32];
Xuint32	mask;

	mask = 1 << 31;
	for(addr=1; addr <= 32; addr++) {
		 mask_buf[32-addr] = mask_set & mask ? 1 : 0;
		 mask_set <<= 1;
	 }
	for(addr=0; addr<32; addr++)
		xil_printf("addr=%d : %d\r\n", addr, mask_buf[addr]);
}


void	cmd_mps_GetStatus(int argc, char *argv[])
{
	TsCalc(evr_ts_sec, 0);
	xil_printf("2mA status : %d\r\n", beam_0_2mAIsValid);
	xil_printf("uBlaze err : %d\r\n", uBlaze_err_code);

}


/*
 *  END
 */
