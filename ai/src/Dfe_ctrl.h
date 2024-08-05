
#ifndef DFE_CTRL_H_	/* prevent circular inclusions */
#define DFE_CTRL_H_	/* by using protection macros */


#include "xbasic_types.h"
#include "types.h"

int		EpicsCmdsInterpreter( int addr, int data);


#if 0
#define	LO_BASE		100
#define	READ_BASE	0

///////////////////////////////////////////////
/// BO CONTROL REGISTERS
///////////////////////////////////////////////
//LOCAL SDI
#define	SDI_BPM_RESET					0
#define	SDI_BPM_NPI_TRIGGER				1
#define	SDI_BPM_TRIGGER_OUTPUT			2
#define	SDI_BPM_PACKET_LINK_DIR			3
#define	SDI_BPM_DATA_INPUT_MUX1			4
#define	SDI_BPM_DATA_INPUT_MUX2			5

//CC SDI
#define SDI_CC_RESET					10
#define	SDI_CC_NPI_TRIGGER				11
#define	SDI_CC_DATA_INPUT_MUX1			12	
#define	SDI_CC_DATA_INPUT_MUX2			13		//1:BPM Data
//BPM Position
#define	FOFB_POS_XY_DATA_SIM_MODE		20
#define	FOFB_POS_SIM_DATA_INTERFACE		21
#define	FOFB_POS_SIM_DATA_MODE			22		//single or block
#define	FOFB_POS_SIM_DATA_TYPE			22		//cos or ramp
//FEEDBACK
#define	FOFB_EVR_TRIG_ENABLE			30	
#define	FOFB_FEEDBACK_RUN_STOP			31
#define	FOFB_U_ACTIVE_RAM				32
#define	FOFB_V_ACTIVE_RAM				33
#define	FOFB_U_PID_RESET				34
#define	FOFB_V_PID_RESET				35
#define	FOFB_U_COEFF_COPY_DDR2BRAM		36
#define	FOFB_V_COEFF_COPY_DDR2BRAM		37
#define	FOFB_FIFO_TRIG					38		//fifo err, eigen, vout, npi
#define	FOFB_NPI_DATA_SEL				39		//NPI for ERR or EIGEN
//CORRECTOR
#define	FOFB_CORR_PS_EN1				40
#define	FOFB_CORR_PS_EN2				41
#define	FOFB_CORR_PS_EN3				42
#define	FOFB_CORR_PS_EN4				43
#define	FOFB_CORR_PS_EN5				44
#define	FOFB_CORR_PS_EN6				45
#define	FOFB_CORR_PS_EN7				46
#define	FOFB_CORR_PS_EN8				47
#define	FOFB_CORR_PS_EN9				48
#define	FOFB_CORR_PS_EN10				49
#define	FOFB_CORR_PS_EN11				50
#define	FOFB_CORR_PS_EN12				51
#define	FOFB_CORR_PS_KICK_MODE1			52
#define	FOFB_CORR_PS_KICK_MODE2			53
#define	FOFB_CORR_PS_KICK_MODE3			54
#define	FOFB_CORR_PS_KICK_MODE4			55
#define	FOFB_CORR_PS_KICK_MODE5			56
#define	FOFB_CORR_PS_KICK_MODE6			57
#define	FOFB_CORR_PS_KICK_MODE7			58
#define	FOFB_CORR_PS_KICK_MODE8			59
#define	FOFB_CORR_PS_KICK_MODE9			60
#define	FOFB_CORR_PS_KICK_MODE10		61
#define	FOFB_CORR_PS_KICK_MODE11		62
#define	FOFB_CORR_PS_KICK_MODE12		63

///////////////////////////////////////////////
/// LO CONTROL REGISTERS
///////////////////////////////////////////////

#define	SDI_BPM_MASK								LO_BASE+0
#define	SDI_BPM_USER_TRIG_DLY						LO_BASE+1
#define	SDI_BPM_CC_NPI_ENABLE_LEN					LO_BASE+2

#define	SDI_CC_MASK									LO_BASE+10
#define	SDI_CC_DPRAM64_DATA_START_ADDR_2_DDR3		LO_BASE+11
#define	SDI_CC_DPRAM64_DATA_2_NPI_LEN				LO_BASE+12

#define	FOFB_POS_SIM_ADDR_SELECT					LO_BASE+20
#define FOFB_POS_NUM2DACOUT							LO_BASE+21

#define	FOFB_U_LENGTH								LO_BASE+30
#define	FOFB_V_LENGTH								LO_BASE+31
#define	FOFB_UT_USER_INJ_ADDR						LO_BASE+31		//Ut output mux injection address 0, 3, 6 ....

#define	FOFB_CORR_PS_ID1							LO_BASE+40
#define	FOFB_CORR_PS_ID2							LO_BASE+41
#define	FOFB_CORR_PS_ID3							LO_BASE+42
#define	FOFB_CORR_PS_ID4							LO_BASE+43
#define	FOFB_CORR_PS_ID5							LO_BASE+44
#define	FOFB_CORR_PS_ID6							LO_BASE+45
#define	FOFB_CORR_PS_ID7							LO_BASE+46
#define	FOFB_CORR_PS_ID8							LO_BASE+47
#define	FOFB_CORR_PS_ID9							LO_BASE+48
#define	FOFB_CORR_PS_ID10							LO_BASE+49
#define	FOFB_CORR_PS_ID11							LO_BASE+50
#define	FOFB_CORR_PS_ID12							LO_BASE+51

#define	FOFB_CORR_PS_ISET1							LO_BASE+52
#define	FOFB_CORR_PS_ISET2							LO_BASE+53
#define	FOFB_CORR_PS_ISET3							LO_BASE+54
#define	FOFB_CORR_PS_ISET4							LO_BASE+55
#define	FOFB_CORR_PS_ISET5							LO_BASE+56
#define	FOFB_CORR_PS_ISET6							LO_BASE+57
#define	FOFB_CORR_PS_ISET7							LO_BASE+58
#define	FOFB_CORR_PS_ISET8							LO_BASE+59
#define	FOFB_CORR_PS_ISET9							LO_BASE+60
#define	FOFB_CORR_PS_ISET10							LO_BASE+61
#define	FOFB_CORR_PS_ISET11							LO_BASE+62
#define	FOFB_CORR_PS_ISET12							LO_BASE+63


//////// READ
#define	FOFB_FPGA_VER								READ_BASE+0		//READ_BASE= 0
#define	FOFB_SYS_STATUS								READ_BASE+1		//epics 244
#define	FOFB_SYS_DIN								READ_BASE+2		//epics 248
#define	FOFB_UT_BRAM_WERR_CNT						READ_BASE+3
#define	FOFB_PID_BRAM_WERR_CNT						READ_BASE+4

#define	DFE_SFP1_RX_POWER							READ_BASE+5
#define	DFE_SFP2_RX_POWER							READ_BASE+6
#define	DFE_SFP3_RX_POWER							READ_BASE+7
#define	DFE_SFP4_RX_POWER							READ_BASE+8
#define	DFE_SFP5_RX_POWER							READ_BASE+9
#define	DFE_SFP6_RX_POWER							READ_BASE+10	//mbus 20

#endif


						
#if 0
//Waveforms
#define	FOFB_WFM_REF_ORBIT_X		//180*1
#define	FOFB_WFM_REF_ORBIT_Y		//180*1
#define	FOFB_WFM_UT_X				//180*180
#define	FOFB_WFM_UT_Y				//180*180
#define	FOFB_WFM_V1_X
#define	FOFB_WFM_V2_X
#define	FOFB_WFM_V3_X
#define	FOFB_WFM_V1_Y
#define	FOFB_WFM_V2_Y
#define	FOFB_WFM_V3_Y

#define	FOFB_WFM_POS_MASK			//360
#define	FOFB_WFM_V123_MASK			//540
#define FOFB_WFM_USER_KICK			//10000

#endif



#endif

