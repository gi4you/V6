/*
 * fofb.h
 *
 *	SDI Link Setup and monitoring
 *
 *
 */

#ifndef FOFB_H
#define FOFB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "xbasic_types.h"		/* contains basic types for Xilinx software IP */
#include "xparameters.h"
#include "fpga_system.h"


#define	PS_SIZE				90
#define	BPM_SIZE			180
#define	SR_BPM_XY_SIZE		360

#define	CCSDI_PACKET_LENGTH			26	//14
#define	FOFB_CCSDI_DATA_LENGTH		780			/* 6 bpm, 1 cc (7*2) * 30*/
#define	FOFB_U_CAL_LENGTH			360+5		/* x (180), y (180) + 5 clock latancy */
#define	FOFB_V_CAL_LENGTH			45*3+5		/* 45 Mode, each mode three signals */
#define	FOFB_EIGEN_CAL_NODE			45

//typedef int matrix_Ut[PS_SIZE][BPM_SIZE];
//typedef int matrix_V[3][PS_SIZE];

//IP Core Register Map
/* Kp,i,d Egeing, PID control */
#define	CS1_DATA_ADDR	XPAR_FOFB_COEFF_SET_0_BASEADDR + (0*4)
#define	CS1_CTRL_ADDR	XPAR_FOFB_COEFF_SET_0_BASEADDR + (1*4)

/* Ref orbit, x,y V1, V2, V3 */
#define	CS2_DATA_ADDR	XPAR_FOFB_COEFF_SET_0_BASEADDR + (2*4)
#define	CS2_CTRL_ADDR	XPAR_FOFB_COEFF_SET_0_BASEADDR + (3*4)

/* Ut H/V */
#define	CS3_DATA_ADDR	XPAR_FOFB_COEFF_SET_0_BASEADDR + (4*4)
#define	CS3_CTRL_ADDR	XPAR_FOFB_COEFF_SET_0_BASEADDR + (5*4)

#define	FOFB_REG4_BASE	XPAR_FOFB_COEFF_SET_0_BASEADDR + (4*4)
#define	FOFB_REG5_BASE	XPAR_FOFB_COEFF_SET_0_BASEADDR + (5*4)
#define	FOFB_REG15_BASE	XPAR_FOFB_COEFF_SET_0_BASEADDR + (15*4)

//Read register
#define	READ_K_REG		XPAR_FOFB_COEFF_SET_0_BASEADDR + (25*4)
#define	FIRMWARE_VER	XPAR_FOFB_COEFF_SET_0_BASEADDR + (27*4)	//firmware version [15:0]

#define REG7_WR			XPAR_FOFB_COEFF_SET_0_BASEADDR + (7*4)
#define REG10_WR		XPAR_FOFB_COEFF_SET_0_BASEADDR + (10*4)
//#define	FOFB_REG31_BASE	XPAR_FOFB_COEFF_SET_0_BASEADDR + (31*4)

///////
#define	FOFB_REG_REF_ORBIT_READ		31


//////////////
#if 1
//////////////
#define	BASE_REF_ORBIT					DDR3_FOFB_DATA_BASE+0				//DDR2_MPMC_BASEADDR+0x3E100000
#define	BASE_DDR_SRBPM_OFFSET			DDR3_FOFB_DATA_BASE+0x0007FF		
#define	BASE_DDR_SRBPM_MASK_OFFSET		DDR3_FOFB_DATA_BASE+0x001000
#define	BASE_DDR_SRBPM_BBA_OFFSET		DDR3_FOFB_DATA_BASE+0x002000		//Feb/12/2015

#define	BASE_UT_X						DDR3_FOFB_DATA_BASE+0x100000		//space 32400 * 4
#define	BASE_UT_Y						DDR3_FOFB_DATA_BASE+0x120000
#define	BASE_V_X						DDR3_FOFB_DATA_BASE+0x140000
#define	BASE_V_Y						DDR3_FOFB_DATA_BASE+0x150000
#define	BASE_AIP_MUX_DATA				DDR3_FOFB_DATA_BASE+0x160000		// 10/08/14 added for

//FIFO
#define	DDR3_KICK_FIFO_BASE				DDR3_FOFB_DATA_BASE+0x200000		//0x3E300000
#define	DDR3_ERR_FIFO_BASE				DDR3_FOFB_DATA_BASE+0x210000		//0x3E310000
#define	DDR3_EIGEN_FIFO_BASE			DDR3_FOFB_DATA_BASE+0x220000		//0x3E320000
#define	DDR3_ALLPOS_REG					DDR3_FOFB_DATA_BASE+0x230000

typedef struct _FOFB_BUFFERs {
	int			H_RefOrbit[BPM_SIZE], V_RefOrbit[BPM_SIZE];
	int			H_OrbitMask[BPM_SIZE], V_OrbitMask[BPM_SIZE];
	int			H_Kp[PS_SIZE*12], V_Kp[PS_SIZE*12];
	int			H_Ki[PS_SIZE*12], V_Ki[PS_SIZE*12];
	//
	int			H_Kd[PS_SIZE], V_Kd[PS_SIZE];
	int			Xy_Pos_SimData[360];
	int			UserWfmData[20000];
} FOFB_BUFFERs, *FOFB_DATA;

#define FOFB_DATA  	( DDR2_MEM_CAST(FOFB_DATA) 	(DDR3_FOFB_DATA_BASE + 0) ) 	

//
typedef struct _FOFB_BUFFERs2 {
	int			data[32400];		//180*180=32400
}	 FOFB_BUFFERs2, *FOFB_DATA2;
#define pUT_X  	( DDR2_MEM_CAST(FOFB_DATA2) 	(BASE_UT_X) )

typedef struct _FOFB_BUFFERs3 {
	int			data[32400];
}	 FOFB_BUFFERs3, *FOFB_DATA3;
#define pUT_Y  	( DDR2_MEM_CAST(FOFB_DATA3) 	(BASE_UT_Y) )


typedef struct _FOFB_BUFFERs4 {
	int			data[8100];		//90*121080
}	 FOFB_BUFFERs4, *FOFB_DATA4;
#define pVM_X  	( DDR2_MEM_CAST(FOFB_DATA4) 	(BASE_V_X) )

typedef struct _FOFB_BUFFERs5 {
	int			data[8100];		//90*121080
}	 FOFB_BUFFERs5, *FOFB_DATA5;
#define pVM_Y  	( DDR2_MEM_CAST(FOFB_DATA5) 	(BASE_V_Y) )

#endif


typedef struct _EPICS_RDATA {
	int			data[100];		
}	 EPICS_RDATAs, *EPICS_RDATA;
#define EPICS_R_REGS  	( DDR2_MEM_CAST(EPICS_RDATA) 	(DDR3_EPICS_DATA_BASE) )
////////////////

/*
 *  Mem write
 #define XPAR_FOFB_COEFF_SET_0_BASEADDR 0x8B000000
 */
// plb_Addr0
typedef struct {
		//Xuint32 _trig    : 1;		//[31]	//Not used
		//Xuint32 space 	: 9;
		//Xuint32 regSel 	: 4;		//[21:18]
		Xuint32 space 	: 8;
		Xuint32 sdiLinkWfmSel 	: 1;
		Xuint32 regSel 	: 5;		//[22:18]	//01/21/17 added +1
		Xuint32 wr 	    : 1;		//[17]
		Xuint32 ram_cs 	: 4;		//[16.15.14.13]
		Xuint32 addr 	: 13;		//[12:0]
    } FOFB_PLB;
FOFB_PLB	FOFB_PLB_CS1, FOFB_PLB_CS3;

//
typedef struct {
		Xuint32 muxModeIsRun    : 1;	//[31]
		Xuint32 mux1_sel 	: 5;		//[30:26]
		Xuint32 mux1_se2 	: 5;		//[21:25]
		Xuint32 user_fifo_npi_trig 	: 1;	//[20]
		Xuint32 space 	: 1;		//[19]
		Xuint32 en   	: 1;		//[18]		
		Xuint32 wr 	    : 1;		//[17]
		Xuint32 ram_cs 	: 4;		//[16.15.14.13]
		Xuint32 addr 	: 13;		//[12:0]
    } w_reg3;
w_reg3	fofb_dpram;


typedef struct {
		Xuint32 space 	: 23;
		Xuint32 addr 	: 9;		//[8:0]
    } w_reg5;
w_reg5	fofb_UtRam;


/* control regs */
typedef struct {
		Xuint32 bpm_pos_sim		    : 1;		//[31] 0: 0-359 count mode, 1: selected bpm address
		Xuint32 CcSim_data_type		: 1;		//[30]  CC Link Data Output type: 1:tmpLocalData, 0:cos
		Xuint32 PosSimulation 	    : 1;		//[29]	Position is simulation mode ?
		Xuint32 IntTrig     	    : 1;		//[28]  11/14/13
		Xuint32 regAddr				: 6;		//plb_Control_0[27:22]
		Xuint32 sim_addr 			: 12;		//[21:10] single channel selection
		Xuint32 bpm2dac_addr 		: 10;		//[9:0]
    } w_reg6;	//w_reg6
w_reg6	plb_Control_0;


/* control regs */
#if 0
typedef struct {
		Xuint32 DataSel		    : 1;		//[31]
		Xuint32 LocalMaxCnt		: 15;
		Xuint32 LocalKx 		: 16;		//[15:0]
    } w_reg8;	//w_reg8
w_reg8	plb_LocalLinkDataCtrl;
#endif


/* control regs */
typedef struct {
		Xuint32 RefOrbitRamSel  : 1;		//31
		Xuint32 PsSdi_PlbMode   : 1;		//30
		Xuint32 SimRamp			: 1;		//29
		Xuint32 SimMbInterface  : 1;		//28
		Xuint32 DacLocalSDI		: 1;
		Xuint32 Reboot		    : 1;		//26
		Xuint32 sfp_trig	    : 1;		//25
		Xuint32 sfp_ee_addr		: 9;		//[24:16]
		Xuint32 DacChASet 		: 16;		//[15:0]
    } W_reg7;	//w_reg7
W_reg7	plb_Reg7Control_1;




/*

 *
 */
#if 0
typedef struct {
		Xuint32	spare2      : 12;
		Xuint32 Ut_InjAddr	: 10;		//[19:10]
		Xuint32 spare1		: 6;		//[9:6]
		Xuint32 dout 		: 4;		//[5:0]
    } W_reg12;
W_reg12	plb_Reg12_Ctrl;
#endif

typedef struct {
		Xuint32 lemo1_mux_sel			: 4;	//[31:28]
		Xuint32 lemo2_mux_sel			: 4;	//[27:24]
		Xuint32 CcTestPacktLength		: 8;		//[23:16] .maxCount( {plb_Reg10[23:16], 8'd0, 16'd10000} ),
		//Xuint32 sdi_bpm_max_len		: 8;		//[15:8]
		Xuint32 bm_dump_en   			: 1;	//[15]
		Xuint32 id_dump_en   			: 1;	//[14]
		Xuint32 CellPLC_IdBeamDump   	: 1;	//13
		Xuint32 CellPLC_BmBeamDump   	: 1;	//12
		Xuint32 watchdog_rst   			: 1;	//11
		Xuint32 ai_bm_mode_sel   		: 2;	//[10:9]
		Xuint32 bm_error_dump_enable   	: 1;	//[8]	(BM_ERR_DUMP && plb_Reg10[8])
		Xuint32 maddr 					: 8;	//[7:0]
    } W_reg10;	//w_reg11
W_reg10	plb_Reg10_SetBpmLength;

#if 1
typedef struct {
		Xuint32 U_activeRam		: 1;	//[31]
		Xuint32 V_activeRam		: 1;	//[30]
		Xuint32 U_pidReset		: 1;	//[29]
		Xuint32 V_pidReset		: 1;	//[28]
		Xuint32 FOFB_RUN		: 1;	//[27]
		Xuint32 EVR_TRIG_MODE	: 1;	//[26]
		Xuint32 spare			: 6;
		Xuint32 V_length		: 10;		//[19:10]
		Xuint32 U_length		: 10;		//[9:0]
    } W_reg13;
W_reg13	plb_Reg13;
#endif


#if 0
typedef struct {
		Xuint32 spare		: 4;		//[28..31]
		Xuint32 npi_sel		: 1;		//27
		Xuint32 eigen_sel_12: 1;		//[26]
		Xuint32 eigen_mux2	: 5;		//25:21
		Xuint32 eigen_mux1	: 5;		//[20:16]
		Xuint32 ccKx 		: 16;		//[15:0]
    } W_reg9;	//w_reg9
W_reg9	plb_Reg9;
#endif

typedef struct {
		Xuint32 ccw_pkt_dly 	: 16;		//[31:16]	NpiStrobe_gen 	CC_LocalPktDelay
		Xuint32 cw_pkt_dly 		: 16;		//[15:0]
    } W_reg9;	//w_reg9
W_reg9	plb_Reg9;


typedef struct {
		Xuint32 spare		: 2;			//[31..30]
		Xuint32 bpm_all_valid_num: 10;		//29:20
		Xuint32 sdi_max_sig_num	: 10;		//19:10
		Xuint32 cell_bpm_num	: 5;		//[9:5]
		Xuint32 cell_max_num	: 5;		//[4:0]
    } W_reg14;	//w_reg14
W_reg14	plb_Reg14;

/*
 *
 */
#define W_REG7	 	0xC400

typedef struct {
  volatile Xuint32  prescaler_lo;	// r/w
  volatile Xuint32  prescaler_hi;	// r/w
  volatile Xuint32  ctrl;			// r/w
  volatile Xuint32  data;			// wr = transmit reg; rd = receive reg
  volatile Xuint32  cmd_status;		// wr = command reg;  rd = status reg
} wreg_regs_t;

#define wreg_regs ((wreg_regs_t *) W_REG7)

Xuint32	Float2DWordReg(float val);
void 	fofb_RegInit();
void 	fofb_Reg1_Mask(Xuint32 mask);
void 	fofb_Reg3_Mask(Xuint32 mask);
void 	fofb_DpRamDataWrite(Xint32 data);
/* reference orbit */
void 	fofb_ReferenceOrbitSingleWrite(int start_addr, int Data);
void 	fofb_ReferenceOrbitWrite(int start_addr, int end_addr, int *pData);
void 	mps_SrBPM_XyOffsetWrite(int start_addr, int end_addr, int *pData);
void 	mps_SrBPM_XyOffsetWithBbaOffsetWrite(int start_addr, int end_addr, int *pData);


void	fofb_ReferenceOrbitRead(int cs_n, int *pData);
void 	fofb_MaskRamWrite(int addr, Xuint8 data);
int		fofb_MaskRamOneDataRead(int addr);
void 	fofb_UOutputMaskRamWrite(int addr, Xuint8 data);
void 	fofb_PsMonFifoWrite(int addr, Xuint32 data);
void	fofb_PsMonFifoRead(int type);
void	fofb_EigenFifoRead(int dbg, int len);
void	fofb_PosErrorFifoRead(int dbg, int len);
/*U-BRAM*/
void	fofb_UtVectorFloatDataWrite(int activeRam, int n_Ut, float Data);
void	fofb_UtVectorBRAM_Write(int activeRAM, int uRamBlock, int xy, Xuint32 *pData);
void	fofb_UtVectorConstDataWrite(int uRamBlock, int Data);
void	fofb_UtVectorConstFloatDataWrite(int n_Ut, float Data);
void	fofb_UtVectorRamRead(int activeRam, int n_Ut, int MaxAddr, int *pData);
void	fofb_Ut_Xy_VectorRamRead(int activeRam, int n_Ut, int xy, int *pData);
/*V-BRAM*/
void	fofb_V_VectorWrite(int activeRam, int n_Ut, int xy, int MaxAddr, int *pData);
void	fofb_V_P_I_VectorWrite(int activeRam, int n_Ut, int vpi_xy, int *pData);
void	fofb_V_VectorConstWrite(int n_V, int MaxAddr, int hex_data);
void	fofb_V_VectorFloatConstWrite(int n_V, int MaxAddr, float Data);
void	fofb_V_VectorRamRead(int n_Ut, int start, int end, Xuint32 *pData);
void	fofb_V_P_I_VectorRamRead(int activeRam, int n_Ut, int vpi_xy, Xuint32 *pData);

void 	XyPositionSimDataAllSet(Xuint32 Data32);
void 	XyPositionSimDataSet( Xuint32 *pData);
void	fofb_PosXyDataSim(int a, int sim);
void	fofb_DpramSimDataIsuBlaze(int a, int Data);
void	fofb_PosXyDataSimDataMode(int a, int type);
void	fofb_PosXyDataSimWfmType(int a, int type);

void	fofb_DacOutputSelect(int a, int bpm_num);
void	fofb_LocalLinkDataType(int a, int Mode);
int		ReadKregs( int addr, int len );
int		ReadSfpEepromData(int addr, int *pData);
int		DAC_ChA_SetData(int val);
void 	DAC_DataSourceSec(int a, int sdi);

void 	CS1_DpRamWriteMulti(int cs, int max_addr, Xuint32 *pData32);
void 	CS3_DpRamWriteMulti(int cs, int addr, Xuint32 Data32);
void	Reboot();

void 	fofb_CorrPsOffsetControl(int addr, Xint32 Data32);
void 	fofb_SystemControl(int addr, Xuint32 pData32);
void	fofb_CorrPs_KickEnable(int addr, Xuint8 data);
void	fofb_CorrPs_KickModeIsFofb(int addr, Xuint8 data);

void	CorrPsRemoteSetting(int a, int Data);
void	CorrPsID_Setting(int a, int Data);

void	ReadFPGA_Ver();

void	cmd_FOFB_LocalTestDataSet(int argc, char *argv[]);
void 	fofb_CS1_WRITE(int cs, int addr, Xuint32 Data32);

void	fofb_myUtVectorRamRead(int n_Ut, int MaxAddr, int *pData);

void	fofb_RefOrbit_ActiveRam(int a, int data);
void	fofb_U_ActiveRam(int a, int data);
void	fofb_V_ActiveRam(int a, int data);
void	fofb_U_PID_Reset(int a, int data);
void	fofb_V_PID_Reset(int a, int data);
void	fofb_U_Length(int a, int data);
void	fofb_V_Length(int a, int data);
void	fofb_EVR_TrigModeSet(int a, int data);
void	fofb_Feedback_Run(int a, int data);

void 	fofb_WavefromBRamWrite(int type, Xint32 *pData);
int		fofb_WavefromBRamRead(int addr);
void	fofb_npi_sel(int a, int data);

void 	Bpm2SimBpmAddrSelect(int a, int addr);
void	fofb_user_fifo_trig();
void	fofb_UtUserInjectionAddressSel(int a, int data);

void fofb_ControlTest(int addr, Xuint32 pData32);

void fofb_InjFifoWrite();
void fofb_Reg13_Mask(Xuint32 mask);
void mps_Reg14_Mask(Xuint32 mask);
void mps_Reg15_Set(Xuint32 value);


/**/
void fofb_SimRamSinglePositionAddressSet(int addr, Xuint32 Data32);
void fofb_SimRamSinglePositionDataSet(int addr, Xuint32 Data32);

void uBlazeWd(int set);
void AI_BmMode_Select(int addr, Xuint32 pData32);



#ifdef __cplusplus
}
#endif

#endif
