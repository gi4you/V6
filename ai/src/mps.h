/*
 * mps.h
 *
 *  Created on: Aug 14, 2013
 *      Author: kha
 */

#ifndef MPS_H_
#define MPS_H_

#include "xbasic_types.h"		/* contains basic types for Xilinx software IP */
#include "xparameters.h"
#include "fpga_system.h"

#define	FIX_32_31		2147483647

typedef struct {
		Xuint32 fault_test		: 1;	//[31]
		Xuint32 pm_usr_rst		: 1;	//[30]
		Xuint32 npi_run			: 1;	//[29]	//npi for ddr run/stop
			Xuint32 OutLatchClear	: 1;	//[28]	//Latch CLEAR command
		Xuint32 FOFB_RUN		: 1;	//[27]
		Xuint32 npi_0_run		: 1;	//[26]
		Xuint32 mps_set_addr_sel	: 4;	//[25:22]
		Xuint32 mps_cal_mode	: 2;		//[21:20]
		//
		Xuint32 bm_bpm_err_cnt	: 10;		//[19:10]
		//Xuint32 U_length		: 2;		//[9:8]
		Xuint32 FastDelayedEnable	: 1;	//[9]  6/17/20
			Xuint32 active_intlock_process_rst		: 1;	//[8]
		Xuint32 enable_bm_zero_detection	: 1;	//[7]	//NOT USED
		Xuint32 fast_dump_ublaz	: 1;	// [6] uBlaze fast dump
		Xuint32 id_bpm_falt_en	: 1;	// [5]
		Xuint32 id_falt_rst		: 1;	// [4]
		Xuint32 bm_ps_sim		: 1;	// [3] 10/08/14
		Xuint32 id_ps_sim		: 1;	// [2] 10/08/14
		Xuint32 id_gap_sim		: 1;	// [1] 3/23/14	gap status simulation
		Xuint32 mps_AIE_muxDir	: 1;	//[0]  0:INternal, 1:external
    } mps_W_reg13;
    mps_W_reg13	mps_plb_Reg13;


    /*
    * [31:25]
    * 	plb_w_Reg12[27:24]	- SetRamReadDoutA read
    * 	plb_w_Reg12[23:8]	- periode PM
    * 	[7:4]
    * 	plb_w_Reg12[3:0]	-
    * 	plb_w_Reg12[0]		- RUN
    */
typedef struct {
		Xuint32	beam_sim      	: 1;	//31
		Xuint32	set2mA      	: 1;	//30
		Xuint32	set50mA      	: 1;	//29
		Xuint32	npi_0_in_sel  	: 1;				//3/23/14
		Xuint32	ref_mux_sel    	: 4;		//[24:27]
		Xuint32 npi_rst_peride	: 16;		//[23:8]
		Xuint32 io_wd_clear 	: 1;		//[7]
		Xuint32 cal_rst_mux_sel	: 3;	//[6:4]
		Xuint32 set_beam_0_2mA 			: 1;		//[3:0]
		Xuint32 dout2 			: 1;
		Xuint32 dout1 			: 1;
		Xuint32 dout0 			: 1;
	} mps_W_reg12;
mps_W_reg12	mps_plb_Reg12;


typedef struct {
		Xuint32 dac_data			: 16;
		//
		Xuint32 dac_a_sel			: 1;	//[15]
		Xuint32 ai_ID_glitch_mode	: 1;	//[14]
		Xuint32 bm_calModeDispSel	: 1;	//[13]
		Xuint32 ai_BM_glitch_mode   : 1;	//[12]
		Xuint32 bram_ref_rd_sel		: 2;	//[11:10]
		Xuint32 pos_sel_addr		: 10;	//[9:0]
    } mps_W_reg15;
    mps_W_reg15	 mps_plb_Reg15;


    typedef struct {
    		Xuint32 factor		: 16;		//[31:16]
    		Xuint32 i_gain 		: 16;		//[9:0]
    	} mps_W_reg8;
    mps_W_reg8		mps_plb_Reg8;

typedef struct {
		Xuint32 y16				: 14;		//[31:18]
		//Xuint32 npi_0_data_sel	: 2;		//[11:10]
		Xuint32 fastDumpDly	    : 8;        //[17:10]
		Xuint32 npi_0_data_size : 10;		//[9:0]
	} mps_W_reg11;
mps_W_reg11		mps_plb_Reg11;


typedef struct {
		Xuint32 ai_id32	: 1;
		Xuint32 ai_id31	: 1;
		Xuint32 ai_id30	: 1;
		Xuint32 ai_id29	: 1;
		Xuint32 ai_id28	: 1;
		Xuint32 ai_id27	: 1;
		Xuint32 ai_id26	: 1;
		Xuint32 ai_id25	: 1;
		Xuint32 ai_id24	: 1;
		Xuint32 ai_id23	: 1;
		Xuint32 ai_id22	: 1;
		Xuint32 ai_id21	: 1;
		Xuint32 ai_id20	: 1;
		Xuint32 ai_id19	: 1;
		Xuint32 ai_id18	: 1;
		Xuint32 ai_id17	: 1;
		Xuint32 ai_id16	: 1;
		Xuint32 ai_id15	: 1;
		Xuint32 ai_id14	: 1;
		Xuint32 ai_id13	: 1;
		Xuint32 ai_id12	: 1;
		Xuint32 ai_id11	: 1;
		Xuint32 ai_id10	: 1;
		Xuint32 ai_id9		: 1;
		Xuint32 ai_id8		: 1;
		Xuint32 ai_id7		: 1;
		Xuint32 ai_id6		: 1;
		Xuint32 ai_id5		: 1;
		Xuint32 ai_id4		: 1;
		Xuint32 ai_id3		: 1;		//
		Xuint32 ai_id2		: 1;		//[1]
		Xuint32 ai_id1	 	: 1;		//[0]
	} mps_W_reg4;
mps_W_reg4		mps_plb_Reg4;


/*
 * Reading registers
 */
#define	MPS_BM_IL_RESULT_BRAM_RD_REG	(20*4)
#define	MPS_REF_BRAM_RD_REG				(21*4)
#define	MPS_FIFO_STATUS_RD_REG			(22*4)
#define	MPS_SYSTEM_STATUS_RD_REG		(23*4)
#define	MPS_FIRMWARE_EXT_IN_RD_REG		(24*4)
#define	MPS_CAL_RESULT_BRAM_RD_REG		(30*4)

//
#define	MPS_DUMP_STATUS_REG				(30*4)





/* Database
 *
    Each unit has the following pvs with current consideration:

    'name'		: [], # str, active interlock device name
    'units'		: [], # str, units for each columns. Empty string if it does not have one.
    'code'		: [], # int, active interlock logic code
    'offset'	: [], # double, offset relative to the center of a straight section
    'safecurent': [], # double, allowed beam current for safe operation.
    'aihol'		: [], # double, allowed horizontal offset limit
    'aivol'		: [], # double, allowed vertical offset limit
    'aihal'		: [], # double, allowed horizontal angle limit
    'aival'		: [], # double, allowed vertical angle limit
    'up_name'	: [], # str, upstream BPM name involved in this active interlock unit
    'up_offset'	: [], # double, offset of upstream BPM relative to the center of a straight section
    'up_aihol'	: [], # double, allowed horizontal offset limit of upstream BPM
    'up_aivol'	: [], # double, allowed vertical offset limit of upstream BPM
    'down_name'	: [], # str, downstream BPM name involved in this active interlock unit
    'down_offset': [], # double, offset of downstream BPM relative to the center of a straight section
    'down_aihol'	: [], # double, allowed horizontal offset limit of downstream BPM
    'down_aivol'	: [], # double, allowed vertical offset limit of downstream BPM
	
	
	Logic :
	> if code == 10:
	>       do logic 10
	> elif code == 20:
	>       do logic 20
	> elif code == 21:
	>       do logic 21
	> elif code == 22:
	>       do logic 22
	> else:
	>       handle unexpected logic
	
 */


//
typedef struct _MPS_BUFFERs {
	int	code[64];

}	 mps_BUFFERs2, *MPS_DATA;

#define AIE  	( DDR2_MEM_CAST(MPS_DATA) 	(BASE_UT_X) )

//DPRAM BLOCK Chip selection
#define 	CS_H1_SP 				0		//Diamond x1
#define 	CS_V1_SP 				1
#define 	CS_H2_SP 					2
#define 	CS_V2_SP 					3
#define 	CS_H3_SP 					4
#define 	CS_HV3_SP 					5
//
#define 	CS_SA_SP 					8		//S for Angle
#define 	CS_SM_SP 					9		//S for mm
#define 	CS_ADDR_IXS_LOGIC_SP 		10		//Logic


// AI LOGIC TYPEs for CS_ADDR_IXS_LOGIC_SP DPRAM

#define		LOGIC_AIE_ID_A__RECTANGLAR				20
#define		LOGIC_AIE_ID_B__DIAMOND					21
#define		LOGIC_AIE_ID_D__RECT_SMALLOFFSET		22
#define		LOGIC_AIE_ID_C__RECT_OPTIMALOFFSET		23




///////////////////////////////////////////////////////////////////
void 	mps_Reg_Set(int addr, Xuint32 value);
void 	mps_int32Reg_Set(int addr, Xint32 value);
void 	mps_Npi_Run(int a, int data);
void	mps_Npi_0_Run(int a, int data);
void	mps_OutputLatchClear(int a, int data);
void	mps_FaultTest(int a, int data);
void	mps_ext_out0(int a, int data);
void	mps_ext_out1(int a, int data);
void	mps_ext_out2(int a, int data);
void 	mps_DpRamOffsetWrite(int cs, int addr, int Data);
void	mps_DpRamOffsetRead(int cs, int	*pData);
void	mps_init();

void	mps_beamC_sim(int a, int data);
void	mps_beamC_2mA(int a, int data);
void	mps_beamC_50mA(int a, int data);

void	mps_soft_trig(int a, int data);
void	mps_BM_Offset0(int a, int data);
void	mps_BM_Offset1(int a, int data);

void	mps_IO_OutCtrl(int a, int data);
void	mps_TrigIsSim(int a, int sim);
void	mps_ChipScope_MuxAddr(int a, int data);
void	mps_calc_mode(int a, int data);
void	mps_Set_S_values(int a, double angle, double offset);
void	mps_read_single_pos(int a, int bpm_pos);
void	mps_Read_FPGA_CalcResultBram(int cs,  int len);
void	mps_Read_FPGA_Calc_ResultData(int cs,  int *pData, int len);
void	mps_IDbpmPosSettingForSimCal(int cell,  int x1, int y1, int x2, int y2, int x3, int y3);
Xuint32	mps_ID_DumpStatusRead();
void	mps_ReadSettingValue();

//ID mask enable/disable
void	mps_id_mask_srtting_1to30(int a, int data);
void	mps_id_mask_srtting_31to60(int a, int data);

//SysMon Register read
int	ReadSysMon_TimeStamp(int *pData);
int	ReadSysMon_IdGapStatus(int *pData);
int	ReadSysMon_AieDumpResult(int *pData);
int	ReadSysMon_BmIlResult(int *pData);

void	position_table();
void	mps_ID_dump_out_enable(int a, int data);
void	mps_BM_dump_out_enable(int a, int data);
void	mps_dac_a_output(int a, int data);
void	mps_IOWatchDog_Reset(int a, int data);

//03/19/2014
void	sdi_cw_pkt_delay(int a, int data);
void	sdi_ccw_pkt_delay(int a, int data);
void	mps_LemoOut1MuxSel(int a, int data);
void	mps_LemoOut2MuxSel(int a, int data);
void	mps_ID_gap_sim(int a, int data);

void	mps_SrBpm_Xoffset(int a, int data);
void	mps_SrBpm_Yoffset(int a, int data);
void	mps_npi_pos_width(int a, int data);
void	mps_npi_data_type_sel(int a, int data);

void	myAieSim_MemClear(int a, int data);
void	mps_select_sr_bpm_address(int a, int data);
void	mps_select_sr_bpm_enable(int a, int data);

void	mps_ai_id_enable(int addr, int data);
void	mps_ai_PlcBmBeamDump_enable(int addr, int data);
void	mps_ai_PlcIdBeamDump_enable(int addr, int data);

Xuint32	mps_ID_SystemStatusRead();
//void	mps_mux_fifo_read(int *pData);
int	mps_mux_fifo_read();

void	mps_ID_PS_sim(int a, int data);
void	mps_BM_PS_sim(int a, int data);
void	mps_npi_1_ddr_wfm_tx_size(int addr, int data);
void	mps_ID_BpmFaltEnable(int a, int data);
void	mps_ID_FaltReset(int a, int data);
void	mps_npi_0_ddr_pm_wfm_tx_offset(int addr, int data);

void	mps_RF_UserDump(int a, int data);	//11/17/2014
void	mps_Clear_BBA_Offset(int a, int data);

void	mps_BM_bpm_error_dump (int a, int data);
void	mps_BM_bpm_error_dump_count (int a, int data);
void	mps_BM_bpm_enable_bm_zero_detection (int a, int data);

void	mps_GlitchDetectionModeSel(int a, int data);
void	mps_Ai_ID_GlitchDetectionModeSel(int a, int data);
void	mps_BM_CalModeDispSel(int a, int data);
//void	mps_BM_CalModeSel(int a, int data);

void	mps_PM_PeriodeSetting(int a, int data);
void	mps_ID_ResetSourceSel(int a, int data);
void	mps_BM_NofBpmsZeroSet(int a, int data);
void	mps_SdiWfmSel(int a, int data);
void	mps_active_intlock_processClear(int a, int data);
void	mps_FastDumpDelayEnable(int a, int data);	//6/17/20


#endif /* MPS_H_ */
