/*
 * console.c
 *
 *  Created on: Dec 14, 2012
 *      Author: kha
 *
 *  Commands are not compatible with cell controller
 *
 *	11/12/14
 *		simpos unit is um (it was mm )
 *
 *
 */

#include "time.h"
#include "sys/time.h"
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "xparameters.h"
#include "sdi_cc.h"
#include "sys_eeprom.h"
#include "ddr.h"

#include "flash_config.h"
#include "console_cmds.h"
#include "cmd_modbus.h"
#include "evr.h"

#include "sin_table.h"
#include "data_converter.h"
#include "fofb_bram_copy.h"

#include "fofb.h"
#include "mps.h"
#include "pscEpics.h"


float	ids[] = {3, 5.1, 5.2, 10, 11, 23.1, 23.2, 28, 8, 18, 0, 0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };


typedef struct
{
  unsigned int		ts_seconds;
  unsigned int		ts_offset;
  char				cachename[256];	/* Tile cache filename */
} _Ts_t;


int ref_table[];

int   	cmd_unknown();
int  	ver();
int     help(int argc, char *argv[]);


/**/
BYTE*   skipwh (BYTE *s);
BYTE*   scan (BYTE *s, BYTE *d);

static 	BYTE *tail;
struct 	table *lookup(struct table *, BYTE *);
void 	strlow(char *str);
void  	dBpm_ConsoleCmd_Processer(char	*rx_buff);

void	cmd_SDI_BpmMemSelect(int argc, char *argv[]);
void	cmd_SDI_BpmTestDataSelect(int argc, char *argv[]);
void	cmd_Cs2_DpRamWrite(int argc, char *argv[]);
void	cmd_K_Read(int argc, char *argv[]);
void	cmd_SFP_EEPROM_Read(int argc, char *argv[]);
void	cmd_DAC_CHA_Setting(int argc, char *argv[]);
void	cmd_DAC_Source_Setting(int argc, char *argv[]);
//
void	cmd_GlobalBpmSimulationMode(int argc, char *argv[]);
void	cmd_GlobalBpmSimulationDataIsuBlaze(int argc, char *argv[]);
void	cmd_GlobalBpmSimulationSignlData(int argc, char *argv[]);
void	cmd_GlobalBpmSimulationSignlWfm(int argc, char *argv[]);
void	cmd_fofb_Bpm2SimBpmAddrSelect(int argc, char *argv[]);
//
void	cmd_ConsoleSoftReboot(int argc, char *argv[]);

void	cmd_Bpm2DacManualTest(int argc, char *argv[]);
void	cmd_Bpm2DacSdiDpRamData(int argc, char *argv[]);

void	cmd_CorrPsManualControl(int argc, char *argv[]);
void	cmd_SysCmd_Setting(int argc, char *argv[]);

void	cmd_fofb_ReferenceOrbitSingleWrite(int argc, char *argv[]);
void	cmd_fofb_RefOrbit_ActiveRam(int argc, char *argv[]);
void	cmd_fofb_U_ActiveRam(int argc, char *argv[]);
void	cmd_fofb_V_ActiveRam(int argc, char *argv[]);
void	cmd_fofb_U_PID_Reset(int argc, char *argv[]);
void	cmd_fofb_V_PID_Reset(int argc, char *argv[]);
void	cmd_fofb_U_Length(int argc, char *argv[]);
void	cmd_fofb_V_Length(int argc, char *argv[]);
void	cmd_CorrPsRemoteSetting(int argc, char *argv[]);

void	cmd_fofb_EigenFifoRead(int argc, char *argv[]);
void	cmd_fofb_CorrPsSetFifoRead(int argc, char *argv[]);
void	cmd_fofb_PosXyErrorFifoRead(int argc, char *argv[]);
void	cmd_fofb_Sim_PositionConstDataSet(int argc, char *argv[]);
void	cmd_fofb_RefOrbit_ConstSet(int argc, char *argv[]);
void	cmd_fofb_ReferenceOrbitRead(int argc, char *argv[]);	//08/01
void	cmd_FOFB_U_RamRead(int argc, char *argv[]);
void	cmd_FOFB_EVR_TrigerModeSet (int argc, char *argv[]);
void	cmd_FOFB_FeedbackRun(int argc, char *argv[]);
void	cmd_FOFB_FeedbackKickSelect(int argc, char *argv[]);
void	cmd_FOFB_FeedbackKickEnable(int argc, char *argv[]);

void	cmd_FOFB_VRam_Read(int argc, char *argv[]);
void	cmd_fofb_U_RamWe_Test(int argc, char *argv[]);
void	cmd_fofb_V_Ram_WE_Test(int argc, char *argv[]);
void	cmd_FOFB_NpiPortSelect(int argc, char *argv[]);
void 	cmd_EpicsControlTest(int argc, char *argv[]);
void	cmd_fofb_RefOrbit_Test(int argc, char *argv[]);
//
void	cmd_FOFB_UtUserInjAddress(int argc, char *argv[]);
void	cmd_FOFB_UserInjRamDataWrite(int argc, char *argv[]);
void	cmd_FOFB_UserInjRamDataRead(int argc, char *argv[]);
void	cmd_FOFB_UserInjTest(int argc, char *argv[]);

void	cmd_MPS_ReadSdiData(int argc, char *argv[]);
void	cmd_mps_npi_run(int argc, char *argv[]);
void	cmd_mps_npi_0_run(int argc, char *argv[]);
void	cmd_mps_IlLatchClear(int argc, char *argv[]);
void	cmd_mps_IlFaultTest(int argc, char *argv[]);
void	cmd_mps_BmOffsetSetting(int argc, char *argv[]);
void	cmd_mps_DpRamOffsetRead(int argc, char *argv[]);
void	cmd_mps_Sim_beamC_2mA_set(int argc, char *argv[]);
void	cmd_mps_Sim_beamC_50mA_set(int argc, char *argv[]);
void	cmd_mps_IO_OutControl(int argc, char *argv[]);
void	cmd_mps_TrigIsSim(int argc, char *argv[]);
void	cmd_mps_ChipScopeAddrSel(int argc, char *argv[]);
void	cmd_mps_calc_mode(int argc, char *argv[]);
void	cmd_mps_Set_S_values(int argc, char *argv[]);
void	cmd_mps_BpmPosReadAddress(int argc, char *argv[]);
void	cmd_mps_BpmPosRead(int argc, char *argv[]);
void	cmd_mps_Read_FPGA_CalcResultBram(int argc, char *argv[]);
void	cmd_mps_ID_Bpm_SimSetting(int argc, char *argv[]);
void	cmd_mps_ID_DumpStatusRead(int argc, char *argv[]);
void	cmd_mps_ReadSettingValue(int argc, char *argv[]);
void	cmd_Test(int argc, char *argv[]);

void	cmd_mps_IlReadTimeStamp(int argc, char *argv[]);
void	cmd_mps_CellCtrl_SimRAM_Set(int argc, char *argv[]);
void	cmd_mps_read_AIE_Status(int argc, char *argv[]);
void	cmd_sdi_cw_pkt_delay (int argc, char *argv[]);
void	cmd_sdi_ccw_pkt_delay (int argc, char *argv[]);
void	cmd_mps_LemoOut1MuxSel (int argc, char *argv[]);
void	cmd_mps_LemoOut2MuxSel (int argc, char *argv[]);
void	cmd_mps_read_single_pos (int argc, char *argv[]);
void	cmd_GapSimulationMode(int argc, char *argv[]);
void	cmd_mps_IL_TimeStamp(int argc, char *argv[]);
void	cmd_mps_SrBpmMaskBit_Set(int argc, char *argv[]);

void	cmd_fofb_SrBpm_XyOffsetSingleWrite(int argc, char *argv[]);
void	cmd_mps_Read_XyPositionRAM(int argc, char *argv[]);
void	cmd_mps_SystemStatus(int argc, char *argv[]);
void	cmd_mps_mux_fifo_read(int argc, char *argv[]);
void	cmd_TestDDR_Tx2IOC(int argc, char *argv[]);

void	cmd_BRAM_Setting(int argc, char *argv[]);
extern void	cmd_mps_GetStatus(int argc, char *argv[]);

void	cmd_mps_bm_fault_cnt(int argc, char *argv[]);
int		cmd_Diag_SdiDataLinkByFault(int argc, char *argv[]);
int		cmd_FastDumpDelayEnable(int argc, char *argv[]);
int		cmd_FastDumpDelayControl(int argc, char *argv[]);

///////////////////////////////////////////////////////////
int  	scpi_idn();


struct table
{
        char    *entry;
        int     (*func)(int, char **);
};



struct table  commands[] =
{
	{"*IDN?", 		scpi_idn},
	//
    {"h", 		help},
    {"?", 		help},
    {"ver", 	ver},
    {"v", 	    ver},
    {"reboot",	cmd_ConsoleSoftReboot},
    {"ipset", 	cmd_EEPROM_IP_Lookup_Setup},
    {"setip", 	cmd_EEPROM_IP_Lookup_Setup},
    {"ipget", 	cmd_ConsolReadEEPROM_IP_Info},
    {"getip", 	cmd_ConsolReadEEPROM_IP_Info},
    {"eeconfig",cmd_Get_EEPROM_ConfigureFromFile},
	{"ee", 		cmd_ConsoleEEPROM__RW},				//parameters write to EEPROM
    //{"ts", 		EvrGetTimeStamp},
    {"time",    GetEvRTime},
    {"boot",    GetBootUpTime},	//read bootup time

    //
    {"epics", 	cmd_EpicsControlTest},
    {"sys", 	cmd_SysCmd_Setting},			//Cmd Register setting
    //
	//ddr read
	{"c", 		cmd_Clear_DDR2},
	{"dc1",		cmd_DDR_CellController_DATAs},
    {"db", 		cmd_DDR_FA_WFM_DATAs},	//C200:0000
    {"dc",      cmd_DDR_CC_WFM_DATAs},	//D000:0000

	//evr
	{"ed", 		cmd_EVR_DelayControls},				//trig n and value
	{"eg",      cmd_Console_GetTrigSet },			//trigger 1 delay and width get
	{"pdly", 	cmd_Console_CcPacketDelay},
	{"erst", 	cmd_Console_EvrGtxReset},
	//cw/ccw new packet delay
	{"cwdly", 	cmd_sdi_cw_pkt_delay},
	{"ccwdly",  cmd_sdi_ccw_pkt_delay},

	//{"bs", 		BpmSdiLinkStatusMonitoring},		//SDI status monitoring for BPM
	{"cs", 		CcSdiLinkStatusMonitoring},				//SDI status monitoring for CC
	//{"r0", 		Bpm_Sdi_Reset},						//Reset
	{"r1", 		CC_Sdi_Reset},

	//{"t", 		cmd_SDI_NpiTrigger},
	{"t1", 		cmd_SDICC_NpiTrigger},
	{"s-dr",	cmd_SDICC_DpRamReadModeCtrl},

	
	{"sim", 	cmd_GlobalBpmSimulationMode},	//FOFB
	{"simi", 	cmd_GlobalBpmSimulationDataIsuBlaze},	//FOFB
	{"sims", 	cmd_GlobalBpmSimulationSignlData},
	{"simw", 	cmd_GlobalBpmSimulationSignlWfm},
	{"simaddr", cmd_fofb_Bpm2SimBpmAddrSelect},
	{"simdac", 	cmd_SDI_BpmMemSelect},				//for DAC Output Test 0 - 359
	{"ts", 		cmd_mps_TrigIsSim},
	{"gsim", 	cmd_GapSimulationMode},

	{"npi_w",	cmd_SDICC_NpiEnableWidth},		//SDI : local/cc both
	//	//cc-ts trig source

	//{"bm", 		cmd_Console_SdiLink_MaskSetup},
	//{"bid", 	cmd_Console_SDI_ID_Setting},
	//{"s-data",	cmd_SDI_InputDataMode},
	//{"s-ramp",	cmd_SDI_BpmTestDataSelect},
	{"bx",		cmd_FOFB_LocalTestDataSet},
	{"cm", 		cmd_Console_SdiCCLink_MaskSetup},
	{"cc-sa",	cmd_Console_SDICC_DpRamReadStartAddress},
	{"cid", 	cmd_Console_SDI_CC_ID_Setting},
	{"cd", 		cmd_Console_SDI_CC_SDI_Input},



	{"k", 		cmd_K_Read},
	{"sfp", 	cmd_SFP_EEPROM_Read},
	{"dac", 	cmd_Bpm2DacSdiDpRamData},	//cmd_DAC_CHA_Setting},
	{"dacm", 	cmd_Bpm2DacManualTest},			//DAC manual output
	{"dacb", 	cmd_Bpm2DacSdiDpRamData},		//DAC output is Global SDI DPRAM output data
	
	//{"cc-pdly", 	cmd_Console_SDI_CC_CalStartTriggerDelay},

	{"ctm", 	cmd_Console_SDI_CC_CalStartTriggerMode},


    {"rr", 		cmd_mps_Read_FPGA_CalcResultBram},
    {"aie", 	cmd_mps_read_AIE_Status},

    
    {"rc",      cmd_DDR_CoeffDataRead},

    {"tsi", 	cmd_mps_IL_TimeStamp},

    {"m", 		cmd_mps_SrBpmMaskBit_Set},
	//BRAM read
	{"rdref", 	cmd_fofb_ReferenceOrbitRead},
	{"dr", 		cmd_mps_ID_DumpStatusRead},		//dump status
	{"rds", 	cmd_mps_ReadSettingValue},
	{"rsys", 	cmd_mps_SystemStatus},

	{"pos1", 	cmd_mps_read_single_pos},
	{"pos", 	cmd_fofb_PosXyErrorFifoRead},	/* fifo xy position data read */
	{"p", 		cmd_mps_BpmPosRead},
	{"rm", 		cmd_mps_mux_fifo_read},		// 10/07/2014
	//{"vout", 	cmd_fofb_CorrPsSetFifoRead},

	{"simpos", 	cmd_fofb_Sim_PositionConstDataSet},
	{"ipos", 	cmd_mps_ID_Bpm_SimSetting},

	{"refset", 	cmd_fofb_RefOrbit_ConstSet},		//sin wave
	{"ref", 	cmd_fofb_RefOrbit_Test},			//const value

	{"refs", 	cmd_fofb_ReferenceOrbitSingleWrite},
	{"off", 	cmd_fofb_SrBpm_XyOffsetSingleWrite},
	//
	{"evr", 	cmd_FOFB_EVR_TrigerModeSet},
	{"run", 	cmd_FOFB_FeedbackRun},
	{"npi", 	cmd_FOFB_NpiPortSelect},
	{"evt", 	cmd_Console_Evr_SetFdbk_EventCode},	//event code change

	{"w", 		cmd_DAC_Source_Setting},
	{"t", 		cmd_FOFB_UserInjTest},
	{"f", 		fofb_InjFifoWrite},		//10 second waveform write to DAC

	//used simulation mode only
	{"posg", 	cmd_MPS_ReadSdiData},	//SDI global data read 0 - 779
	{"posw",	cmd_Cs2_DpRamWrite},	//write DRAM
	{"bp", 		cmd_mps_BpmPosReadAddress},	//position read address
	//
	{"npi0",  	cmd_mps_npi_0_run},		// NPI-0 Post moterm continuce RUN
	{"npi1",  	cmd_mps_npi_run},		// NPI-1 Global position write
	{"r",  		cmd_mps_IlLatchClear},	// Clear Latch sta
	{"ft",  	cmd_mps_IlFaultTest},	//Generate Intrlock for PM timing test
	{"bm",		cmd_mps_BmOffsetSetting},	//BM offset
	{"rdoff",	cmd_mps_DpRamOffsetRead},	//read mps setpoint reference
	{"rsoff",	cmd_BRAM_Setting},			// 10/15/14


	//{"bmsim",		cmd_mps_BmOffsetSetting},
	{"bm2ma",	cmd_mps_Sim_beamC_2mA_set},
	{"bm50ma",	cmd_mps_Sim_beamC_50mA_set},
	
	{"out",		cmd_mps_IO_OutControl},
	{"mode",	cmd_mps_calc_mode},
	{"caddr", 	cmd_mps_ChipScopeAddrSel},
	{"si", 		cmd_mps_Set_S_values},


	{"t-ts", 	cmd_mps_IlReadTimeStamp},
	{"pt", 		position_table},
	{"cdata", 	cmd_mps_CellCtrl_SimRAM_Set},

	{"lm1", 	cmd_mps_LemoOut1MuxSel},
	{"lm2", 	cmd_mps_LemoOut2MuxSel},

	{"test", 	cmd_Test},
	{"cod", 	cmd_mps_Read_XyPositionRAM},	//X,y position ram read
	{"tddr", 	cmd_TestDDR_Tx2IOC},
	{"ms", 		cmd_mps_GetStatus},
	{"bmf", 	cmd_mps_bm_fault_cnt},

	{"sdiag", 	cmd_Diag_SdiDataLinkByFault},
	{"fastde", 	cmd_FastDumpDelayEnable},
	{"fastdly", cmd_FastDumpDelayControl},

	{"",  		cmd_unknown}			/* command not fount */
};



/**/
struct table *lookup(p, token)
struct table *p;
BYTE *token;
{
    while(*(p -> entry) != '\0')
    {
            if(strcmp(p -> entry, token) == 0)
                    break;
            else
                    ++p;
    }
    return p;
}

/**/
BYTE*   skipwh (BYTE *s)
{
    while(*s && (*s == '\r' || *s == '\n' || *s == ' ' || *s == '\t'))
            ++s;
    return s;
}

/**/
BYTE*   scan (BYTE *s, BYTE *d)
{
    s = skipwh(s);
    while(*s &&
     !( *s == '\r'
     || *s == '\n'
     || *s == ' '
     || *s == '\t'
     || *s == '='))
            *d++ = *s++;
    *d = '\0';
    return s;
}

/**/
void strlow(char *str) {

  int i;

  for ( i = 0; i < (signed)strlen(str) ; i++)
    str[i] = tolower(str[i]);

}

/*
 *
 */
int   cmd_unknown()
{
 	print(("\r\nUnknown command...\r\n"));
    return TRUE;
}


/*
 * Version information.
 */
int  ver()
{
	NSLS2FOFB_StartUpMessage();
    return TRUE;
}

/*
 * Help.
 */
int   help(int argc, char *argv[])
{
	Xuint32 	kx;
	kx = strtol(argv[1], (char **)0, 10);

	print(("\r\n-------------------- MPS CONTROLLER HELP --------------------\r\n"));
	xil_printf(

        " h :              help\r\n"
        " ? :              help\r\n"        
        " ver :            version info\r\n"
		" reboot :         System Rebooting\r\n"
		"-------------------------------------------------\r\n"
    	" ipset or setip,  IP address setting and stre to EEPROM \r\n"
    	" ipget or getip,  Read IP information from EEPROM \r\n"	
		" ee,              EEPROM configuration try ee -h for help\r\n"
		" eeconfig,        EEPROM parametes write from PC Host\r\n"
		" mon,             FPGA inside Temprature and Voltage reading (SysMon)\r\n"
		" time,            Read EVR Timestamp\r\n"
		"\r\n"	
		"\r\n"

		"***** EVR  *****\r\n"
		" ed,              Set Trig delay Use : ed 1 dly \r\n"
		" eg,              Read delay\r\n"
		" erst,            Reset Evr Gtx\r\n"
		" pdly,            Set Cc Packet delay\r\n"
		" cwdly,           CW Packet delay\r\n"
		" ccwdly,          CCW Packet delay\r\n"

#if 0
		" t,               BPM NPI soft trigger\r\n"	
		" s-data,          Local Sdi Data type(0: fa, 1:count, 2:sin wave) \r\n"
//		" s-ramp,          Local Data type\r\n"
		" s-dr,            Dpram Data Output trigger mode (0: Internal, 1: external fofb control mode)\r\n"		
		" bx,              Local Test Data set\r\n"
		" cc-ts,           BPM Node CalStart Trigger source (0: Internal Mask, 1: External Event delay)\r\n"
		" cc-tdly,         BPM Node CalStart Trigger source Delay for CC trigger\r\n"
#endif
		"\r\n"
		"\r\n"
		"***** CC LINK *****\r\n"					
		" cid,             CC ID setting Cell Number (1,2,3....31)\r\n"
		" cm,              CC  Sdi Mask setting\r\n"
		" cs,              CC  Sdi link status monitoring\r\n"
		" r1,              CC  Sdi regsters clearing\r\n"
		" t1,              CC  NPI soft trigger\r\n"
		" ctm,             CC trigger source mode 1:User Delay, 0:CalStart\r\n"
		" cc-sa,           CC Link Data start address for NPI write \r\n"
		"\r\n"
		"\r\n"
		"***** MPS TEST *****\r\n"
		" gsim,            Gap simulation)\r\n"
		" rdref,           Read reference BRAM \r\n"
		" dr,              Dump ID status \r\n"
		" rds,             Read reference setting value \r\n"
		" pos,             Read position Block RAM \r\n"
		" p,               Read single position RAM \r\n"
		" simpos,          Setting sim position RAM \r\n"
		" ipos,            Setting sim ID bpm position setting \r\n"
		"\r\n"
		"\r\n"
		" posg,            Select position address for ddr and chipscope \r\n"
		" posw,            Write data to sim RAM (0-779), Valid only simulation RAM \r\n"
		" npi0,            NPI-0 Post moterm continuce RUN \r\n"
		" npi1,            NPI-1 Global position write RUN \r\n"
		" r,               IL Latch Clearing \r\n"
		" ft,              IL Fault Test set fault signal \r\n"
		" bm,              Bending ID offset setting \r\n"
		" rdoff,           Read Offset setting value \r\n"
		" bm2ma,           Set sim Beam is 2 mA \r\n"
		" bm50ma,          Set sim Beam is 50 mA \r\n"
		" out,             External Out Control \r\n"
		" si,              Set S values \r\n"
		" bp,              cmd_mps_BpmPosReadAddress \r\n"
		" caddr,           cmd_mps_ChipScopeAddrSel, for Calc output RAM read \r\n"
		"\r\n"
		"\r\n"
		" rr,             Read FPGA cal result, 0..5 \r\n"

		"\r\n"
		"\r\n"
		"***** DDR MEMORY LOCAL/CC LINK, DATA *****\r\n"
		" npi_w,           LOCAL\CC NPI Enable width, unit is (SEC)\r\n"
		" db,              Reading DDR3 0xC2000000 for PostMorterm Data \r\n"
		" dc,              Reading DDR3 0xD0000000 for Global Position ata\r\n"
		" pos,             Read Position error values from FIFO\r\n"
		" pos1,            Select SDI position address for NPI and Chipscope\r\n"
		//" eigen,           Read Eigen values from FIFO\r\n"
		" npi,             switch 0xC2000000 space NPI input data\r\n"
		"\r\n"
		"\r\n"
		" cd,              CC Sdi Input Data selection (0:Test Data, 1:Normal BPM Data)\r\n"
		" sim,             Global Bpm Position Data is Simulation Mode (1:Global Position is Sim DPRAM, 0:BPM Data)\r\n"
		"\r\n"
		"***** DAC  CHANNEL OUTPUT  *****\r\n"
		" mem,             Global Memoey address select It's BPM number 0 ~ 359 for DAC output mapping\r\n"
		" dac,             DAC manual setting\r\n"
		" dacm,            DAC manual test\r\n"
		" dacb,            DAC mapping Local Data or Global Data\r\n"
		"\r\n"
		" simd,            \r\n"
		" sfp,             SFP power reading\r\n"
		" c,               Clearing DDR memory space (0, 1, 2, 10)\r\n"
		" k,               Read Register (Kp, Ki, Kd, Eigen...)\r\n"
			"\r\n"
		" t-ts,             Read trigger timestamp\r\n"
		" pt,               position table display\r\n"
		" cdata,            Set CC data at pos RAM 'cdata 1 7ff' cell 1 data 7ff\r\n"
			"\r\n"
		" lm1,              LEMO1 Output signal Select\r\n"
		" lm2,              LEMO2 Output signal Select\r\n"

		" fastde,           Fast Dump delay enable? 1:En, 0:Disable\r\n"
		" fastdly,          Fast Dump delay control 1step is 100 us, max 63 step\r\n"

#if 0
		"***** CORRECTOR PS SDI LINK *****\r\n"
		" pset,            Corrector PS manual control, channel number (0....12)\r\n"
		"***** BRAM *****\r\n"
		" uw,              Ut BRAM data test value write 1..179\r\n"
		" vw,              V BRAM data test value write 1..12\r\n"
		" ru,              Read Ut RAM, ru 1 for RAM1 \r\n"
		" rv,              Read V RAM \r\n"
		" rc,              Read DDR space Coefficents\r\n"
		" uinjd,           User Injection Data write\r\n"
		" uinja,           User Injection address 1 .. 540\r\n"
		" uinjr,           read User Injection memory data with start address\r\n"
#endif

		"\r\n"
		" bmf,             Read BM bpm fault count \r\n"
		"sdiag             Read SDI latched data \r\n"
		"epics 14          Clear all register\r\n"
		"epics 65          mps_BM_bpm_error_dump enable/disable\r\n"
		"epics 66          mps_BM_bpm_error_dump_count setting [1 - 100], default 10 \r\n"
		"epics 67          NOT USED, mps_BM_bpm_enable_bm_zero_detection\r\n"
		"epics 57          mps_BM_ID_LIMIT Setting [nm] \r\n"
		"epics 68          BM Glitch mode enable :1, disable : 0 \r\n"
		"epics 69          BM Display mode select 1:FIFO, 0:Default \r\n"
		"epics 70          BM Cal Mode select 3: 2 comp 3 point, 2: 2 comp, 1: 3 point, 0:Default \r\n"
		"epics 71          mps_PM_PeriodeSetting : 20000(2sec) \r\n"
		"\r\n"
		"\r\n"
		"AI-ID epics command config \r\n"
		"epics 59          mps_ID_ResetSourceSel : 0:user trig, 1:DspTrig \r\n"
		"\r\n"
		"epics 73          mps_Ai_ID_GlitchDetectionModeSel : 0:disable, 1:enable \r\n"
		"epics 74          mps_Ai_ID glitch count (20 ns step) 5000=100us, default is 7500=150us \r\n"
		"epics 76          mps_active_intlock_processClear\r\n"
		"\r\n"
		"epics 48          plb_w_Reg12[1] 1: PM trigger out to EVG, 0: disable \r\n"
		"epics 49          plb_w_Reg12[2] 1: ai_id_dump2, 0: ID_DUMP \r\n"
		"epics 27          delay, Mode 0:user trig (20 ns step) \r\n"
		"epics 75          1:SDI live data disp, 0:SDI latched data disp\r\n"
		"epics 38          1:CW/CCW, 0:CCW only\r\n"

		"\r\n"
	);
    		
}




/**/
/*
 *
 */
void  dBpm_ConsoleCmd_Processer(char	*rx_buff)
{
register struct table *p;
struct table *lookup();
char    lbuf[128];
char   	*lp=lbuf;
BYTE   	*scan(), *skipwh();
int   	argc;
char 	*argv[128];
char 	args[5][128];

	for(argc = 0; argc < 5; argc++) {
    	argv[argc] = (BYTE *)0;
        args[argc][0] = '\0';
    }

    strlow(rx_buff);
	lp = scan(rx_buff, args[0]);

    argv[0] = args[0];
    tail = skipwh(lp);

    for(argc = 1; ; argc++) {
    	lp = scan(lp, args[argc]);
        if(*args[argc] == '\0')
        	break;
        else {
        	argv[argc] = args[argc];
        }
	}

    if(*argv[0] != '\0') {
    	p = lookup(commands, argv[0]);
		(*(p->func)) (argc, argv);
    }

}



/*
 *
 */
void cmd_EpicsControlTest(int argc, char *argv[])
{
int cmdAddr, Data32, ret;	

	cmdAddr = strtol(argv[1], (char **)0, 10);
	Data32  = strtol(argv[2], (char **)0, 10);
	
	if(cmdAddr < 165 ) {
		ret = EpicsCmdsInterpreter( cmdAddr, Data32);
	}
	else {
		xil_printf("epics control addre is less then 165 .....\r\n");
	}
}




// 1: count, 0: sin
void	cmd_SDI_BpmTestDataSelect(int argc, char *argv[])
{
	Xuint32 Mode;
	Mode = strtol(argv[1], (char **)0, 10);
	fofb_LocalLinkDataType( 0, Mode );

}

//
//	Select simulation mode or normal mode
//	Simulation Mode 1: Position DPRAM is external DPRAM
//
void	cmd_GlobalBpmSimulationMode(int argc, char *argv[])
{
	int sim;
	sim = strtol(argv[1], (char **)0, 10);

	fofb_PosXyDataSim(0, sim);

}

void	cmd_GapSimulationMode(int argc, char *argv[])
{
	int sim;
	sim = strtol(argv[1], (char **)0, 10);

	mps_ID_gap_sim(0, sim);		//gap simulation mode
}

//
//	DPRAM for X,Y position   cs 7 0-360 DATA
//	cmd : posw
//
void	cmd_Cs2_DpRamWrite(int argc, char *argv[])
{
	int cs, addr, Data;
	
	cs = 7;
	addr = strtol(argv[1], (char **)0, 10);
	Data = strtol(argv[2], (char **)0, 10);

	CS2_DpRamWriteSigleAddr(cs, addr, Data);
}



/*
 * Read Kp,i,d Eigen registers
 */
void	cmd_K_Read(int argc, char *argv[])
{
	int addr, len;
	static char UserResponse[8];

	xil_printf("----------- MENU -------------\r\n");
	xil_printf("0: Position Reg 0 - 800 \r\n");
	xil_printf("1: BM IL status 0 - 359 \r\n");
	xil_printf("2: Gap Status\r\n");
	xil_printf("3: Photon Shutter status\r\n");
	xil_printf("4: ID Dump Control Reg\r\n");
	xil_printf("5: ID Run/Stop Status\r\n");
	xil_printf("7: SysMon\r\n");
	xil_printf("8: aie_USET_MASK_SETTING_REG\r\n");
	xil_printf("9: CellPLC_IdBeamDump_Status\r\n");
	xil_printf("10: CellPLC_BmBeamDump_Status\r\n");
	xil_printf("11: CellPLCWdStatus\r\n");
	xil_printf("12: BM_PhotonShutters_Status\r\n");
	xil_printf("13: y1_pos_buff\r\n");
	xil_printf("14: y2_pos_buff\r\n");

	xil_printf("\r\nSelect signal type : ");

	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	addr = strtoul(UserResponse, NULL, 10);

	xil_printf("\r\nEnter Length : ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	len = strtoul(UserResponse, NULL, 10);
	xil_printf("\r\n");

	ReadKregs(addr, len);
}

/*
 *
 */
void	cmd_SFP_EEPROM_Read(int argc, char *argv[])
{
	int i, addr;
	int	data[10];
	int	*pData=data;
	
	addr = strtol(argv[1], (char **)0, 10);
	if(addr == 0) addr = 360;	//rx power

	ReadSfpEepromData(addr, pData);
	for(i=0; i<6; i++) {
		if(pData[i] == 0xffff) pData[i] = 0;
		printf("sfp %d = %.1f (uW)\r\n", i, pData[i]*0.1);
	}
}

void	cmd_DAC_CHA_Setting(int argc, char *argv[])
{
	int val;
	val = strtol(argv[1], (char **)0, 10);
	//DAC_ChA_SetData(val);
	mps_dac_a_output(0, val);
}


/*
 *	DAC Link with Local SDI or CC SDI
 */
void	cmd_DAC_Source_Setting(int argc, char *argv[])
{
	int val;
	val = strtol(argv[1], (char **)0, 10);
	DAC_DataSourceSec(0, val);
}



/*
 * reboot 1 for remote firmware upgrade
 *
 * */
void	cmd_ConsoleSoftReboot(int argc, char *argv[])
{
	int	i;
	Xuint32 	reboot;
	int mode;
	int	status;


	mode = strtol(argv[1], (char **)0, 10);

	flash_mutex_Lock();
	if(mode == 1){
		status = UsrFLASH_Write(FLASH_BOOTINFO_BASEADDR, "update:", 7);
		if(status == XST_SUCCESS) {
			xil_printf("Flash Update Enabled\r\n");
			Reboot();
		}
		else {
			xil_printf("Flash Write error\r\n");
		}
	}
	else if(mode == 2){
		status = UsrFLASH_Write(FLASH_BOOTINFO_BASEADDR, "app...:", 7);
		if(status == XST_SUCCESS) {
			xil_printf("Flash Update Disabled\r\n");
			Reboot();
		}
		else {
			xil_printf("Flash Write error\r\n");
		}
	}
	else {

		xil_printf("Flash Update Not configured... Soft reboot\r\n");
		sleep(1000);
		Reboot();
	}
	flash_mutex_Ulock();


	xil_printf("Reboot command fail.....Use 1 or 2\r\n");
}



/*
 * This routine maunal test which is bpm data to DAC
 *	BPM data is selected by microblaze
 *
 */
void	cmd_Bpm2DacManualTest(int argc, char *argv[])
{
	int addr, Data;
	int ValidInput = 0;
	static char UserResponse[64];
		
	while(ValidInput != 1)
	{
		xil_printf("\r\nEnter BPM address (0-%d): ", 360 );

		while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
		addr = strtoul(UserResponse, NULL, 10); 	
		if(addr >= 0 && addr <= 360 )
		{
			ValidInput = 1;
		}
		else
		{
			xil_printf("Invalid address offset entered\r\n");
		}
	}
	while(ValidInput != 2)
	{
		xil_printf("Enter Data (0-%d): ", 32767);
		while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
		Data = strtoul(UserResponse, NULL, 10);
		if (Data > 32767)
		{
			Data = 32767;
		}
		else
		{
			ValidInput = 2;
		}
	}	
	
	*(volatile unsigned int*)(SDI2CC_BASE+  (25* 4)) = 0x0;	//1: Local BPM, 0:Test Data
	DAC_DataSourceSec(0, 0);			// 0-360	Select BPM Global Data
	fofb_PosXyDataSim(0, 1);			// Simulation Mode
	fofb_PosXyDataSimDataMode(0, 1);	// Simulation Mode is single Data
	fofb_DacOutputSelect(0, addr);		// DAC output matched Address
	
	CS2_DpRamWriteSigleAddr(7, addr, Data);	
}	
	

void	cmd_Bpm2DacSdiDpRamData(int argc, char *argv[])
{
	int addr = strtol(argv[1], (char **)0, 10);

	//fofb_PosXyDataSim(0, 0);		//0: fofb data is SDI, 1: fofb data is Sim
	//*(volatile unsigned int*)(SDI2CC_BASE+  (25* 4)) = 0x1;	//1: Local BPM, 0:Test Data

	//DAC_DataSourceSec(0, 0);			// 0-360	Select BPM Global Data
	fofb_DacOutputSelect(0, addr);		// DAC output matched Address
	
}	
	
/*
 *
 */
void	cmd_CorrPsManualControl(int argc, char *argv[])
{
	int addr;
	int Data32;
	Xuint32 DDS_hex;
	double DDSDin;
	//static char UserResponse[64];

	//xil_printf("enter ps address : ");
	//while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	//addr = strtoul(UserResponse, NULL, 10);
	
	addr = strtol(argv[1], (char **)0, 10);
	Data32 = strtol(argv[2], (char **)0, 10);

	xil_printf("Info: addr=0-12 for Power supply setting\r\n");
	xil_printf("Info: addr=13 for Sinewave Amplitude (500000), It's NCO mode\r\n");
	xil_printf("Info: addr=14 for Sinewave Frequency (1Hz ~ 500 Hz), It's NCO mode\r\n");

	if(addr == 13)	//NCO sine wave amplitude
		fofb_SystemControl(13, Data32);		//Kx
	else if(addr == 14) {	//frequency
		DDSDin = (Data32 * 4294967295.f / 10000) / 4294967295.f;
		DDS_hex = (Xuint32)(DDSDin * 4294967295.f);
		fofb_SystemControl(12, DDS_hex);	//frequency
		xil_printf("freq val = %X\r\n", (int)DDS_hex);
	}
	else
		fofb_CorrPsOffsetControl(addr, Data32);
	xil_printf("Ps addr=%d :  %d \r\n", addr, Data32);
	

	/* ID1  ID2 	ID3
	 *  0	4		8
	 *  2	6		10
	 *
	 */


#if 0
	while(1) 
	{
		//addr = 0;
		fofb_CorrPsOffsetControl(2,  524200);
		fofb_CorrPsOffsetControl(0,  0xAFFFF);
		xil_printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\r\n");
		fofb_CorrPsOffsetControl(2,   0);
		fofb_CorrPsOffsetControl(0,  0);

		xil_printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\r\n");
	}
#endif

}

/*
 *
 */
void	cmd_SysCmd_Setting(int argc, char *argv[])
{
	int addr;
	int Data32;
	Xuint32 DDS_hex;
	double DDSDin;

	addr   = strtol(argv[1], (char **)0, 10);
	Data32 = strtol(argv[2], (char **)0, 10);

	xil_printf("Info: addr=0-1023 for Command Register setting\r\n");

	fofb_SystemControl(addr, Data32);
	xil_printf("Sys addr = %d , %d \r\n", addr, Data32);
}



//////////////////////////////////////////////////////////////////////
/*  FOFB_POS_XY_DATA_SIM
 *	1: bpm position data are simulation 
 *	0: bpm position data are from SDI link
 */
void	cmd_BpmPositionDtaMode(int argc, char *argv[])
{
int	data;
	data = strtol(argv[1], (char **)0, 10);	
	fofb_PosXyDataSim(0, data&1);
}

// FOFB_POS_SIM_DATA_MODE
// simulation data type
void	cmd_GlobalBpmSimulationDataIsuBlaze(int argc, char *argv[])
{
	int Data;
	Data = strtol(argv[1], (char **)0, 10);
	//1:uBlaze 0:count
	fofb_DpramSimDataIsuBlaze(0, Data);
}

void	cmd_GlobalBpmSimulationSignlData(int argc, char *argv[])
{
	int Data;
	Data = strtol(argv[1], (char **)0, 10);
	//1: single output 0: block output
	fofb_PosXyDataSimDataMode(0, Data);

}
void	cmd_GlobalBpmSimulationSignlWfm(int argc, char *argv[])
{
	int Data;
	Data = strtol(argv[1], (char **)0, 10);
	//1: ramp 0: cos
	fofb_PosXyDataSimWfmType(0, Data);

}

// FOFB_POS_SIM_ADDR_SELECT
// sim position address
void	cmd_fofb_Bpm2SimBpmAddrSelect(int argc, char *argv[])
{
	int Data;
	Data = strtol(argv[1], (char **)0, 10);

	Bpm2SimBpmAddrSelect(0, Data);
}

// FOFB_POS_NUM_DACOUT
// w_reg6	plb_Control_0[9:0]
// mem
void	cmd_SDI_BpmMemSelect(int argc, char *argv[])
{
	Xuint32 bpm_addr;
	bpm_addr = strtol(argv[1], (char **)0, 10);
	fofb_DacOutputSelect(0, bpm_addr);
}

//FOFB_CORR_PS_SET_H1
//FOFB_CORR_PS_SET_V1
void	cmd_CorrPsRemoteSetting(int argc, char *argv[])
{
	Xuint32 addr, val;
	addr = strtol(argv[1], (char **)0, 10);
	val = strtol(argv[2], (char **)0, 10);
	
	CorrPsRemoteSetting(addr, val);
}

	
// A or B
void	cmd_fofb_RefOrbit_ActiveRam(int argc, char *argv[])
{
int	data;
	data = strtol(argv[1], (char **)0, 10);
	
	fofb_RefOrbit_ActiveRam(0, data & 0x1);
	xil_printf("Reference ActiveRam Select = %d\r\n", data);
}
// BO
void	cmd_fofb_U_ActiveRam(int argc, char *argv[])
{
int	data;
	data = strtol(argv[1], (char **)0, 10);
	
	fofb_U_ActiveRam(0, data & 0x1);
	//xil_printf("Ut ActiveRam Select = %d\r\n", data);
}

// BO
void	cmd_fofb_V_ActiveRam(int argc, char *argv[])
{
int	data;
	data = strtol(argv[1], (char **)0, 10);
	
	cmd_fofb_V_ActiveRam(0, data & 0x1);
	//xil_printf("V ActiveRam Select = %d\r\n", data);
}



// BO
void	cmd_fofb_V_PID_Reset(int argc, char *argv[])
{
int	data;
	data = strtol(argv[1], (char **)0, 10);
	
	fofb_V_PID_Reset(0, data & 0x1);
	//xil_printf("V PIDreg Reset = %d\r\n", data);
}

// LongOut
void	cmd_fofb_U_Length(int argc, char *argv[])
{
int	data;
	data = strtol(argv[1], (char **)0, 10);
	if(data != 0) 
		fofb_U_Length(0, data);
	else
		xil_printf("Input length range error...\r\n");
	//xil_printf("Ut Length = %d\r\n", data);
}

// LongOut
void	cmd_fofb_V_Length(int argc, char *argv[])
{
int	data;
	data = strtol(argv[1], (char **)0, 10);
	if(data != 0) 
		fofb_V_Length(0, data);
	else
		xil_printf("Input length range error...\r\n");
	//xil_printf("V Length = %d\r\n", data);
}



#define	U_RAM_WE_CSn		0
#define	V_RAM_WE_CSn		1
#define	U_RAM_ENABLE_CSn	2
#define	V_RAM_ENABLE_CSn	3



/*
 *
 */
void	cmd_fofb_RefOrbit_Test(int argc, char *argv[])
{
	int data;

	data = strtol(argv[1], (char **)0, 10);
	Ref_Init( data );
	xil_printf("sr-bpm ref set : %d (nm) \r\n", data);
}


void	cmd_fofb_ReferenceOrbitSingleWrite(int argc, char *argv[])
{
	int addr, data;

	addr = strtol(argv[1], (char **)0, 10);
	data = strtol(argv[2], (char **)0, 10);

	fofb_ReferenceOrbitSingleWrite( addr, data );

}

void	cmd_fofb_SrBpm_XyOffsetSingleWrite(int argc, char *argv[])
{
	int addr, data;
	int i;
	int	ut_data[365];
	int	rd_data[365];

	data = strtol(argv[1], (char **)0, 10);

	for(i=0; i<360; i++) {
		ut_data[i] = data+i;
	}
	mps_SrBPM_XyOffsetWrite(0, 360, &ut_data[0]);
	fofb_ReferenceOrbitRead(5, rd_data);
	for(i=0; i<SR_BPM_XY_SIZE; i++) {
		if(rd_data[i] != ut_data[i])
		xil_printf("bram compare error %3d = %d : %d\r\n", i, rd_data[i], ut_data[i]);
	}
}

/*
 *
 */
void	cmd_fofb_RefOrbit_ConstSet(int argc, char *argv[])
{
	int i,  hex;
	int	ut_data[365];
	int	rd_data[365];

	hex = strtol(argv[1], (char **)0, 10);
	if(hex == 0) hex = 1;

	for(i=0; i<SR_BPM_XY_SIZE; i++) {
		ut_data[i] = hex+i;
		//ut_data[i] = ref_table[i] * hex;
	}
	// 2. Reference orbit Write and Read : nm Unit
	fofb_ReferenceOrbitWrite(0, SR_BPM_XY_SIZE, &ut_data[0]);
	fofb_ReferenceOrbitRead(0, rd_data);
	for(i=0; i<SR_BPM_XY_SIZE; i++) {
		if(rd_data[i] != ut_data[i])
		xil_printf("bram compare error %3d = %d : %d\r\n", i, rd_data[i], ut_data[i]);
	}
}





void	cmd_fofb_ReferenceOrbitRead(int argc, char *argv[])
{
	int len, cs, i;
	static char UserResponse[32];
	int	ut_data[365];

	len = strtol(argv[1], (char **)0, 10);
	xil_printf("Enter Data type Ref(0) or Offset(1) : ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	cs = strtoul(UserResponse, NULL, 10);

	if(len > 360 ) len = 360;
	else if(len == 0) len = 180;
	if(cs != 0 ) {
		fofb_ReferenceOrbitRead(5, ut_data);
		for(i=0; i<SR_BPM_XY_SIZE; i++) {
			xil_printf("%3d = %d : %d\r\n", i, ut_data[i], Get_I_DDR2(BASE_DDR_SRBPM_OFFSET+(i*4)) );
		}
	}
	else {	//
		fofb_ReferenceOrbitRead(0, ut_data);
		for(i=0; i<SR_BPM_XY_SIZE; i++) {
			xil_printf("%3d = %d : %d\r\n", i, ut_data[i], Get_I_DDR2(BASE_REF_ORBIT+(i*4)) );
		}
	}

}

/*
 *
 */
void	cmd_fofb_U_RamWe_Test(int argc, char *argv[])
{
	int data;
	int j;
	
	data = strtol(argv[1], (char **)0, 10);

	for(j=0; j<180; j++) {
		fofb_UtVectorFloatDataWrite( 0, j, j+1 );
	}
				
	//URAM_Init( data );
	xil_printf("URAM Write Done...\r\n");
}

//
void	cmd_fofb_V_Ram_WE_Test(int argc, char *argv[])
{
	Xuint32 data;
	int n;
	
	data = strtol(argv[1], (char **)0, 10);

	V_P_I_RAM_Init(data);
}


/*
 *
 */
void	cmd_fofb_EigenFifoRead(int argc, char *argv[])
{
int n;
	n = strtol(argv[1], (char **)0, 10);

	if(n == 0) n = 1080;
	xil_printf("Trigger FIFO start..........\r\n");
	fofb_dpram.user_fifo_npi_trig = 1;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
	fofb_dpram.user_fifo_npi_trig = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));

	sleep(100);
	fofb_EigenFifoRead(1, n);
}

void	cmd_fofb_CorrPsSetFifoRead(int argc, char *argv[])
{
	int t;
		t = strtol(argv[1], (char **)0, 10);
			
	if(t == 0) t = 120;
	xil_printf("Trigger FIFO start..........\r\n");
	fofb_dpram.user_fifo_npi_trig = 1;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
	fofb_dpram.user_fifo_npi_trig = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));

	sleep(100);
	fofb_PsMonFifoRead(t);

}

/*
 *
 */
void	cmd_fofb_PosXyErrorFifoRead(int argc, char *argv[])
{
	int n;
	int bm_status;

		n = strtol(argv[1], (char **)0, 10);
	if(n == 0) n = 780;
	
		xil_printf("Trigger FIFO start..........\r\n");
		fofb_dpram.user_fifo_npi_trig = 1;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
		sleep(100);
		fofb_dpram.user_fifo_npi_trig = 0;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));


		fofb_PosErrorFifoRead(1, n);

		//[14:0]
		//BM_address <= {Relational_Operator3_out1, Relational_Operator2_out1, Relational_Operator1_out1, Relational_Operator_out1, posAddress};
		//[9:0] posAddress;
		//read status
		bm_status = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR +  (20*4));
		xil_printf("bm_status = 0x%x\r\n", bm_status & 0x7FFF);
		xil_printf("bpm address = %d\r\n", bm_status & 0x3FF);
}

/*
 * bpm position sim data write
 */
void	cmd_fofb_Sim_PositionConstDataSet(int argc, char *argv[])
{
	int i, menu, addr, data;
	static char UserResponse[8];
	Xuint32 Data[780];
	int mm;
	
	xil_printf("----------- MENU -------------\r\n");
	xil_printf("0: Xy position data for Single Bpm\r\n");
	xil_printf("1: Xy position data are all constant value\r\n");
	xil_printf("2: Xy position data are count value\r\n");
	xil_printf("\r\nSelect Data type : ");

	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	menu = strtoul(UserResponse, NULL, 10);

	if(menu == 1) {
		xil_printf("\r\nEneter position data [um] : ");
		while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
		//data = strtoul(UserResponse, NULL, 10);
		data = strtod(UserResponse, NULL);
		//mm = (int)(data * 1000000);
		mm = (int)(data * 1000);
		XyPositionSimDataAllSet( mm );
	}
	else if(menu == 2){
		xil_printf("\r\nEneter position data [um] : ");
		while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
		//data = strtoul(UserResponse, NULL, 10);
		data = strtod(UserResponse, NULL);
		//mm = (int)(data * 1000000);
		mm = (int)(data * 1000);
		for(i=0; i<780; i++) {
			Data[i] = i + mm;
		}
			
		XyPositionSimDataAllSet( 0 );		//clear 0
		XyPositionSimDataSet(&Data[0]);
	}
	else {
		int cell, plan, bpm_no;
		xil_printf("\r\nSingle bpm position : \r\n");
		xil_printf("Enter Cell Number [1..30] : ");
		while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
		cell = strtoul(UserResponse, NULL, 10);
		if(cell > 30) cell = 1;
		xil_printf("Enter X or Y [0 (x), 1 (y) ] : ");
		while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
		plan = strtoul(UserResponse, NULL, 10);
		if(plan > 1 ) plan = 0;
		xil_printf("Enter Bpm number [1-12], cc[13] : ");
		while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
		bpm_no = strtoul(UserResponse, NULL, 10);

		if(bpm_no < 13) {	// 1...12
			addr = (cell-1) * 26 + ( (bpm_no-1)*2+plan);
			xil_printf("\r\nEneter #%d-%d, addr is %d, position data [um] : ", cell, bpm_no, addr);
			while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
			//data = strtoul(UserResponse, NULL, 10);
			data = strtod(UserResponse, NULL);
			//mm = (int)(data * 1000000);
			mm = (int)(data * 1000);
		}
		else {	//CC data
			addr = (cell-1) * 26 + ( (bpm_no-1)*2+plan);
			xil_printf("\r\nGot #%d-%d, addr is %d, data hex 32bit : ", cell, bpm_no, addr);
			while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
			mm = strtoul(UserResponse, NULL, 16);	//hex type for cell controller address
		}
		//fofb_SimRamSinglePositionAddressSet(0,  addr);
		//fofb_SimRamSinglePositionDataSet(addr, mm);

		//xil_printf("Bpm Addr  = %d : Data = %d (nm) \r\n", addr, Data32);
		fofb_dpram.wr = 0;		//disable write
		fofb_dpram.ram_cs = 7;	//cs[7]
		Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));

		// total length is 420 because include cell data (1,2)
		// This DPRAM is same configuration with SDI inside Dual Port RAM
		//for(i=0; i<FOFB_CCSDI_DATA_LENGTH; i++)
		{
			//Write ADDRESS
			fofb_dpram.addr   = addr;
			Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
			//
			Set_I_DDR2( CS2_DATA_ADDR, mm);		//data write
			//
			fofb_dpram.wr = 1;		//write
			Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
			fofb_dpram.wr = 0;		//write
			Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
		}

	}
	xil_printf("\r\nWrite Done...\r\n");
}



void	cmd_FOFB_EVR_TrigerModeSet (int argc, char *argv[])
{
	int data;

	data = strtol(argv[1], (char **)0, 10);
	fofb_EVR_TrigModeSet(0, data);
}


void	cmd_FOFB_FeedbackRun(int argc, char *argv[])
{
	int data;

	data = strtol(argv[1], (char **)0, 10);
	fofb_Feedback_Run(0, data);
}


// 12 channel
void	cmd_FOFB_FeedbackKickSelect(int argc, char *argv[])
{
	int i, channel, data;

	channel = strtol(argv[1], (char **)0, 10);
	data = strtol(argv[2], (char **)0, 10);
	xil_printf("Corrector kick mode (0:NCO, 1:fofb): %x, %x\r\n", channel, data);
	xil_printf("'kick 13 1' for fofb, 'kick 13 0' for all channel NCO\r\n");

	if(channel == 13) {
		for(i=0; i<12; i++) {
			fofb_CorrPs_KickModeIsFofb(i, data);
			xil_printf("Corrector Kick mode : ch %d = %d\r\n", i, data);
		}
	}
	else
		fofb_CorrPs_KickModeIsFofb(channel, data&0x1);
}

// select uBlaze only or NCO/FOFB mode ?
void	cmd_FOFB_FeedbackKickEnable(int argc, char *argv[])
{
	int channel, data;
	int i;

	channel = strtol(argv[1], (char **)0, 10);
	data = strtol(argv[2], (char **)0, 10);
	xil_printf("Corr Ps : %x, %x\r\n", channel, data);
	xil_printf("'kicken 13 1' for all enable, 'kicken 13 0' for all disable\r\n");

	if(channel == 13) {
		for(i=0; i<12; i++) {
			fofb_CorrPs_KickEnable(i, data);
			xil_printf("Corrector Kick mode : ch %d = %d\r\n", i, data);
		}
	}
	else
		fofb_CorrPs_KickEnable(channel, data&0x1);
}

/*
 *	ram_n : 0 .. 179
 *	len   : 0 .. 360
 */
void	cmd_FOFB_U_RamRead(int argc, char *argv[])
{
int i, ramb_n, len;
	Xuint32 rData[540];
	
	ramb_n = strtol(argv[1], (char **)0, 10);
	len    = strtol(argv[2], (char **)0, 10);
	if(len == 0) len = 360;
	else if(len > 360) len = 360;
	
	//Read Ut RAM
	xil_printf("----- Re Ut Ram, Usage: ru ram number \r\n");
	fofb_UtVectorRamRead(0, ramb_n, len, &rData[0]);

}


/*
 *	ram_n : 0 .. 11
 *	len   : 0 .. 540
 */
void	cmd_FOFB_VRam_Read(int argc, char *argv[])
{
	int ram, len, i;
	Xuint32 rData[540];
	float f, pf;

	ram = strtol(argv[1], (char **)0, 10);
	len = strtol(argv[2], (char **)0, 10);

	if(len ==0 ) len = 540;
	else if(len > 540) len = 540;

	xil_printf("-----Read V%d RAM0 -----\r\n", ram);	//active RAM 0
	fofb_V_VectorRamRead(ram, 0, len, &rData[0] );

	for(i=0; i<540; i++) {
		f = Hex32_to_Float ( rData[i], &pf);
		printf("ram=%d, %d = %x, %d (%f)\r\n", ram, i, rData[i], rData[i], f);
	}
}


void	cmd_FOFB_NpiPortSelect(int argc, char *argv[])
{
	int i;

	i = strtol(argv[1], (char **)0, 10);
	fofb_npi_sel(0, i);

}

/*
 **
 */
void	cmd_FOFB_UtUserInjAddress(int argc, char *argv[])
{
	int addr;

	addr = strtol(argv[1], (char **)0, 10);
		
	fofb_UtUserInjectionAddressSel(0, addr);
}

/*
 **
 */
void	cmd_FOFB_UserInjRamDataWrite(int argc, char *argv[])
{
int	addr, Data, type;
int i;

	Data = strtol(argv[1], (char **)0, 10);
	type = strtol(argv[2], (char **)0, 10);
	
	xil_printf("Set Data = %d\r\n", Data);
	for(i=0; i<8001; i++) {
		FOFB_DATA->UserWfmData[i] = Data;
		Data += 1;
	}
	
	fofb_WavefromBRamWrite(type, &FOFB_DATA->UserWfmData[0] );
}


void	cmd_FOFB_UserInjRamDataRead(int argc, char *argv[])
{
	int i, data, start;
	start = strtol(argv[1], (char **)0, 10);
	
	for(i=start; i<start+100; i++) {
		data = fofb_WavefromBRamRead(i);
		xil_printf("%d = %d\r\n", i, data);
	}
		
}

void	cmd_FOFB_UserInjTest(int argc, char *argv[])
{
	//fofb_ControlTest(0,0);
	fofb_user_fifo_trig();
}



/*
 *
 *
 */
void	cmd_MPS_ReadSdiData(int argc, char *argv[])
{
	int	addr;
	addr = strtol(argv[1], (char **)0, 10);
	mps_Reg15_Set(addr);
	xil_printf("selected bpm number = %d\r\n", addr);
}


// BO
void	cmd_mps_npi_run(int argc, char *argv[])
{
int	data;
	data = strtol(argv[1], (char **)0, 10);

	mps_Npi_Run(0, data & 0x1);
}

void	cmd_mps_npi_0_run(int argc, char *argv[])
{
int	data;
	data = strtol(argv[1], (char **)0, 10);

	mps_Npi_0_Run(0, data & 0x1);
}

void	cmd_mps_IlLatchClear(int argc, char *argv[])
{
int	data;
	//data = strtol(argv[1], (char **)0, 10);

	mps_OutputLatchClear(0, 1);
	xil_printf(".....Clearing......\r\n");
	//sleep(100);
	//mps_OutputLatchClear(0, 0);
}

void	cmd_mps_IlFaultTest(int argc, char *argv[])
{
int	data;
	//data = strtol(argv[1], (char **)0, 10);

	mps_FaultTest(0, 1);
	xil_printf(".....Fault......\r\n");
	mps_FaultTest(0, 0);
}

//
void	cmd_mps_BmOffsetSetting(int argc, char *argv[])
{
int	i;
char UserResponse[32];
double mm;
int nm;

	xil_printf("\r\nEneter x, y offset (mm) : ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	mm = strtod(UserResponse, NULL);
	nm = (int)(mm * 1000000);

	mps_SrBpm_Xoffset(0, nm);
	mps_SrBpm_Yoffset(0, nm+1);

	//mps_BM_Offset0(0, nm*-1);	//low
	//mps_BM_Offset1(0, nm);		//high
	//xil_printf("BP offset : %d, %d nm\r\n", nm, nm*-1);
}


// Read DPRAM DATA
// offset : 0, 1, 2, 3, 4, 5,6, 7, 8
//#define 	CS_DO_X1_SP 				0		//Diamond x1
//#define 	CS_DO_X2_SP 				1
//#define 	CS_RM_X_SP 					2
//#define 	CS_RM_Y_SP 					3
//#define 	CS_RA_X_SP 					4
//#define 	CS_RA_Y_SP 					5
//#define 	CS_SM_SP 					6		//S for mm
//#define 	CS_SA_SP 					7		//S for Angle
//#define 	CS_ADDR_IXS_LOGIC_SP 		8		//Logic

void	cmd_mps_DpRamOffsetRead(int argc, char *argv[])
{
int	offset;
int len;
int	data[128];
char	UserResponse[64];

	xil_printf("--- MENU (Reference setting read for AIE) --\r\n");
	xil_printf("0: H1 offset dp-ram\r\n");
	xil_printf("1: V1 offset dp-ram\r\n");
	xil_printf("2: H2 \r\n");
	xil_printf("3: V2 \r\n");
	xil_printf("4: H3 \r\n");
	xil_printf("5: V3 \r\n");
	xil_printf("6: mrad X \r\n");
	xil_printf("7: mrad Y \r\n");
	xil_printf("8: S for mrad(angle)\r\n");
	xil_printf("9: S for mm\r\n");
	xil_printf("10: Logic type\r\n");
	xil_printf("11: H1 Offset\r\n");
	xil_printf("12: V1 Offset\r\n");
	xil_printf("13: H2 Offset\r\n");
	xil_printf("14: V2 Offset\r\n");

	xil_printf("\r\nSelect number : ");

	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	offset = strtoul(UserResponse, NULL, 10);

	//xil_printf("\r\nEnter length : ");
	//while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	//len = strtoul(UserResponse, NULL, 10);

	len = 32;
	mps_DpRamOffsetRead(offset, &data[0]);


	if(len == 0) len = 10;
	else if(len > 57) len = 57;
	if(offset == 8 || offset == 9 || offset == 10 ) {	//logic
		for(offset=0; offset<len; offset++)
			xil_printf("addr=%2d : 0x%x\r\n", offset, data[offset]);
	}
	else {
		for(offset=0; offset<len; offset++)
			xil_printf("addr=%2d : %d\r\n", offset, data[offset]);
	}
	xil_printf("\r\n");
}


//
void	cmd_mps_Sim_beamC_2mA_set(int argc, char *argv[])
{
int	data;

	data = strtol(argv[1], (char **)0, 10);
	mps_beamC_2mA(1, data);
}	

void	cmd_mps_Sim_beamC_50mA_set(int argc, char *argv[])
{
int	data;

	data = strtol(argv[1], (char **)0, 10);
	mps_beamC_50mA(1, data);
}	
	

void	cmd_mps_IO_OutControl(int argc, char *argv[])
{
int	data;

	data = strtol(argv[1], (char **)0, 10);
	mps_IO_OutCtrl(1, data);
}

void	cmd_mps_TrigIsSim(int argc, char *argv[])
{
int	data;

	data = strtol(argv[1], (char **)0, 10);
	mps_TrigIsSim(1, data);
}

//cal result ram select
void	cmd_mps_ChipScopeAddrSel(int argc, char *argv[])
{
int	data;

	data = strtol(argv[1], (char **)0, 10);
	mps_ChipScope_MuxAddr(1, data);
}


/*
 *
 */
void	cmd_mps_BpmPosRead(int argc, char *argv[])
{
	int	pos;

	Xuint32 addr;

	addr = strtol(argv[1], (char **)0, 10);
	if(addr > 780) {
		addr = 780;
		xil_printf("Max bpm number signal is 780 \r\n");
	}

	//for(i=0; i<780; i++)
	{
		plb_Reg7Control_1.DacChASet = addr;	//select address
		Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
		pos = Get_I_DDR2(READ_K_REG);	//read register
	}
	xil_printf("bpm position addr= %d : %d\r\n", addr, pos);
}

/*
 *
 */
void	cmd_mps_AieDumpStatusRead(int argc, char *argv[])
{
	int	aie_dump_status;
	aie_dump_status = Get_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (28 * 4) );
	xil_printf("aie_dump_status = 0x%x\r\n", aie_dump_status);
}


/*
 *
 */
void	cmd_mps_Read_FPGA_CalcResultBram(int argc, char *argv[])
{
	Xuint32 cs;
	int	len;
	int i, j;
	int	p2Data[6][60];
	static char UserResponse[32];
	int	data;

	xil_printf("0: x cal offset @ s3\r\n");
	xil_printf("1: y cal offset @ s3\r\n");
	xil_printf("2: x cal angle\r\n");
	xil_printf("3: y cal angle\r\n");
	xil_printf("4: ID dump result\r\n");
	xil_printf("5: BP dump result\r\n\r\n");

#if 0
	xil_printf("\r\nEneter output type : ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	cs = strtoul(UserResponse, NULL, 10);
	//cs  = strtol(argv[1], (char **)0, 10);
	len = strtol(argv[1], (char **)0, 10);
	if(len<10) len = 10;

	mps_Read_FPGA_CalcResultBram(10+cs,  len);
	//xil_printf("Usage : cs and len, 'rr 0 10'\r\n");
#endif


#if 1
	len = strtol(argv[1], (char **)0, 10);
	if(len < 10) len = 10;
	for(i=0; i<6; i++) {
		mps_Read_FPGA_Calc_ResultData(10+i, &p2Data[i][0], len);
	}
	for(i=0; i<len; i++) {
		//                x,        y           x            y
		printf("%2.1f = %.5f(mm), %.5f(mm), %.5f(mrad), %.5f(mrad)\r\n", ids[i], p2Data[0][i]*0.000001, p2Data[1][i]*0.000001, p2Data[2][i]*0.000001, p2Data[3][i]*0.000001);
	}

	xil_printf("\r\n4: ID dump result\r\n");
	for(i=0; i<len; i++) {
		data = p2Data[4][i];
		xil_printf("%d=0x%x, addr= %d, rst=0x%x, mode=0x%x \r\n", i, data, data&0x3F, (data>>6)&0x3F, (data>>12)&0xF);
	}

	xil_printf("\r\n5: BP dump result\r\n");
	for(i=0; i<len; i++) {
		data = p2Data[5][i];
		xil_printf("%d=0x%x, addr= %d, rst=0x%x, mode=0x%x \r\n", i, data, data&0x3F, (data>>6)&0x3F, (data>>12)&0xF);
	}

#endif

}


/*
 * ID Bpm position data write for simulation
 *
 */
void	cmd_mps_ID_Bpm_SimSetting(int argc, char *argv[])
{

	int i, idN, addr, data;
	static char UserResponse[8];
	int x1,  y1,  x2,  y2,  x3,  y3;
	double mm;
	xil_printf("\r\nEneter ID number (3, 5, 10, 11, 23, 28, 8, 18) : ");

	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	idN = strtoul(UserResponse, NULL, 10);

	xil_printf("\r\nEneter x1 pos (mm): ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	mm = strtod(UserResponse, NULL);
	x1 = (int)(mm * 1000000);

	xil_printf("\r\nEneter x2 pos (mm) : ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	mm = strtod(UserResponse, NULL);
	x2 = (int)(mm * 1000000);

	xil_printf("\r\nEneter x3 pos (mm) : ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	mm = strtod(UserResponse, NULL);
	x3 = (int)(mm * 1000000);

	xil_printf("\r\nEneter y1 pos (mm) : ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	mm = strtod(UserResponse, NULL);
	y1 = (int)(mm * 1000000);

	xil_printf("\r\nEneter y2 pos (mm) : ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	mm = strtod(UserResponse, NULL);
	y2 = (int)(mm * 1000000);

	xil_printf("\r\nEneter y3 pos (mm) : ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	mm = strtod(UserResponse, NULL);
	y3 = (int)(mm * 1000000);

	mps_IDbpmPosSettingForSimCal(idN,  x1, y1, x2,y2, x3,y3);

}


void	cmd_mps_ID_DumpStatusRead(int argc, char *argv[])
{
	Xuint32 dump_s;
	dump_s = mps_ID_DumpStatusRead();
	xil_printf("dump status  = 0x%x\r\n", dump_s);
	xil_printf("aie_ address = %d\r\n", dump_s & 0x1F);
}


void	cmd_mps_ReadSettingValue(int argc, char *argv[])
{
	mps_ReadSettingValue();
}



void _long2bin(Xuint32 mask_set)
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


void	cmd_Test(int argc, char *argv[])
{
long	d;
	d = strtol(argv[1], (char **)0, 10);
	_long2bin( (Xuint32)d );
}

/*
 *
 */
void	cmd_mps_IlReadTimeStamp(int argc, char *argv[])
{
	int	data[62];
	int *pData=data;

	ReadSysMon_TimeStamp(pData);
	xil_printf("reset : %d:%d\r\n", *(pData+1), *(pData+0));
	xil_printf("stop  : %d:%d\r\n", *(pData+3), *(pData+2));
}


/*
 *
 */
void	cmd_mps_CellCtrl_SimRAM_Set(int argc, char *argv[])
{
	int cell, addr, data;
	static char UserResponse[32];

	cell = strtol(argv[1], (char **)0, 10);
	data = strtol(argv[2], (char **)0, 16);
	xil_printf("Enter CELL number [1..30] : ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	cell = strtoul(UserResponse, NULL, 10);

	xil_printf("Enter Sim Data (hex 0 -fffe) : ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	data = strtoul(UserResponse, NULL, 16);
	data <<= 16;	//left shift [31:16]


	if(cell == 0) xil_printf("\r\nUse cel number 1...30\r\n");
	else {
		cell -= 1;
		xil_printf("\r\naddr = %d, epics db=%d data= 0x%x\r\n", 24+cell*26, (24+cell*26)*4, data);

		addr = 24+cell*26;
		CS2_DpRamWriteSigleAddr(7, addr, data);
	}
}


/*
SysMonData[47] <= Aie_bm_o[31:0];
SysMonData[48] <= Aie_bm_o[63:32];
SysMonData[49] <= Aie_A_o[31:0];
SysMonData[50] <= Aie_A_o[63:32];
SysMonData[51] <= Aie_C_o[31:0];
SysMonData[52] <= Aie_C_o[63:32];
SysMonData[53] <= Aie_D_o[31:0];
SysMonData[54] <= Aie_D_o[63:32];
*/

void	cmd_mps_read_AIE_Status(int argc, char *argv[])
{
	int	d;
	int	Aie_bm_o, Aie_A_o,  Aie_C_o, Aie_D_o;

	FOFB_PLB_CS1.regSel = 7;
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));


	plb_Reg7Control_1.DacChASet = 47;
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	Aie_bm_o = Get_I_DDR2(READ_K_REG);	//read register
	//
	plb_Reg7Control_1.DacChASet = 49;
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	Aie_A_o = Get_I_DDR2(READ_K_REG);	//read register
	//
	plb_Reg7Control_1.DacChASet = 51;
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	Aie_C_o = Get_I_DDR2(READ_K_REG);	//read register
	//
	plb_Reg7Control_1.DacChASet = 53;
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	Aie_D_o = Get_I_DDR2(READ_K_REG);	//read register
	//
	xil_printf(" BM=%X, A=%X, C=%X, D=%X\r\n", Aie_bm_o, Aie_A_o, Aie_C_o, Aie_D_o );

}


/*
 *
 */
void	cmd_sdi_cw_pkt_delay (int argc, char *argv[])
{
int	d;

	d = strtol(argv[1], (char **)0, 10);
	sdi_cw_pkt_delay( 0, d );
}


void	cmd_sdi_ccw_pkt_delay (int argc, char *argv[])
{
int	d;

	d = strtol(argv[1], (char **)0, 10);
	sdi_ccw_pkt_delay( 0, d );
}

void	cmd_mps_LemoOut1MuxSel (int argc, char *argv[])
{
int	d;

	d = strtol(argv[1], (char **)0, 10);
	mps_LemoOut1MuxSel( 0, d );
}

void	cmd_mps_LemoOut2MuxSel (int argc, char *argv[])
{
int	d;

	d = strtol(argv[1], (char **)0, 10);
	mps_LemoOut2MuxSel( 0, d );
}

/*
 *
 */
void	cmd_mps_read_single_pos (int argc, char *argv[])
{
int	d;

	d = strtol(argv[1], (char **)0, 10);
	mps_read_single_pos( 0, d );
}

/*
 * Get timestamp
 */
void	cmd_mps_IL_TimeStamp(int argc, char *argv[])
{
	int sec, nsec;
	time_t timestamp;
	unsigned int size;
	char	tbuf[64];
	char* c_time=tbuf;

	_Ts_t *evrTimsstamp;

	//read data
	FOFB_PLB_CS1.regSel = 7; 	//cs address is 7
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));

	plb_Reg7Control_1.DacChASet = 14;	//select address
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	nsec = Get_I_DDR2(READ_K_REG);	//read register

	plb_Reg7Control_1.DacChASet = 15;	//select address
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	sec = Get_I_DDR2(READ_K_REG);	//read register

	xil_printf("%d:%d\r\n", sec, nsec);


	evrTimsstamp->ts_seconds = sec;
	evrTimsstamp->ts_offset  = nsec;
	evrTimsstamp->ts_seconds -= (60*60* 3) ;	//for Easten time;

	timestamp = (time_t)(evrTimsstamp->ts_seconds);
	c_time = ctime(&timestamp);
	printf ("RF Dump Time time : %s\r\n", c_time);

}


/*
 *	0328/2014 added for SR-BPM Enable/Disable
 */
void	cmd_mps_SrBpmMaskBit_Set(int argc, char *argv[])
{
	int cell, addr, data;
	static char UserResponse[32];

/*
	cell = strtol(argv[1], (char **)0, 10);
	data = strtol(argv[2], (char **)0, 10);

	xil_printf("Enter CELL number [1..30] : ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	cell = strtoul(UserResponse, NULL, 10);

	xil_printf("Enter Data (0 - 1) : ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	data = strtoul(UserResponse, NULL, 10);


	if(cell == 0) xil_printf("\r\nUse cel number 1...30\r\n");
	else {
		cell -= 1;
		xil_printf("\r\naddr = %d, epics db=%d data= 0x%x\r\n", 24+cell*26, (24+cell*26)*4, data);

		addr = 24+cell*26;
		CS2_DpRamWriteSigleAddr(7, addr, data);
	}
	*/
	cell = strtol(argv[1], (char **)0, 10);
	data = strtol(argv[2], (char **)0, 10);
	fofb_MaskRamWrite(cell, data);
	xil_printf("set addr=%d = %d\r\n", cell, data);
	data = fofb_MaskRamOneDataRead( cell );
	xil_printf("read addr=%d = %d\r\n", cell, data);
}


/*
 *
 */
void	cmd_mps_Read_XyPositionRAM(int argc, char *argv[])
{
	int i, d;
	int data[420];
	double rms_o;
	float f_data[182];


	d = strtol(argv[1], (char **)0, 10);

	//READ X and Y POSITION
	//read result data
	fofb_dpram.ram_cs = 8;	//ram_cs2[8]
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));

	// read data from DPRAM
	for(i=0; i<360; i++) {
		fofb_dpram.addr = i;	//address
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
		//get data
		data[i] = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (26*4));
	}

	if(d == 0) {
		for(i=0; i<180; i++) {
			f_data[i] = data[i] * 0.001;
			xil_printf("%d = %d\r\n", i, data[i]);
		}
	}
	else {
		for(i=180; i<360; i++) {
			f_data[i-180] = data[i] * 0.001;
			xil_printf("%d = %d\r\n", i, data[i]);
		}
	}


	rms_o = rms ( &f_data[0], 180);
	printf("rms = %f\r\n", rms_o);

}


void	cmd_mps_SystemStatus(int argc, char *argv[])
{
	Xuint32	status;
	status = mps_ID_SystemStatusRead();
	xil_printf("sys status = 0x%X\r\n", status);
	displayBits(status);
}


/*
 * 10/07/2014
 */
void	cmd_mps_mux_fifo_read(int argc, char *argv[])
{
	int i;
	//int	p2Data[2048];
	Xuint32 data;
	//mps_mux_fifo_read(&p2Data);

	mps_mux_fifo_read();
	for(i=0; i<1024; i++) {
		data = Get_I_DDR2(BASE_AIP_MUX_DATA + (i*4));
		xil_printf("%d=0x%x\r\n", i, data);
	}
}

void	cmd_TestDDR_Tx2IOC(int argc, char *argv[])
{
	long d;
	d = strtol(argv[1], (char **)0, 10);

	Set_I_DDR2(0xD0000000, d);
	Set_I_DDR2(0xC2000000, d);
}


/*
 *
 *
 */
void	cmd_BRAM_Setting(int argc, char *argv[])
{
	long d, ram_cs;
	float f;
	//int regAddr;
	int offset;
	int W32data;



	char	UserResponse[64];

	xil_printf("--- MENU (Reference setting read for AIE) --\r\n");
	xil_printf("0: H1 offset dp-ram\r\n");
	xil_printf("1: V1 offset dp-ram\r\n");
	xil_printf("2: H2 \r\n");
	xil_printf("3: V2 \r\n");
	xil_printf("4: H3 \r\n");
	xil_printf("5: V3 \r\n");
	xil_printf("6: mrad X \r\n");
	xil_printf("7: mrad Y \r\n");
	xil_printf("8: S for mrad(angle)\r\n");
	xil_printf("9: S for mm\r\n");
	xil_printf("10: Logic type\r\n");
	xil_printf("11: H1 Offset\r\n");
	xil_printf("12: V1 Offset\r\n");
	xil_printf("13: H2 Offset\r\n");
	xil_printf("14: V2 Offset\r\n");

	xil_printf("\r\nSelect number : ");

	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	ram_cs = strtoul(UserResponse, NULL, 10);

	xil_printf("\r\nEnter memory offset : ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	offset = strtoul(UserResponse, NULL, 10);

	xil_printf("\r\nEnter Position Offset [nm] : ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	d = strtoul(UserResponse, NULL, 10);


	//ram_cs = strtol(argv[1], (char **)0, 10);
	//offset = strtol(argv[2], (char **)0, 10);
	//d  = strtol(argv[3], (char **)0, 10);

    //f = *(float *)&d;
    //W32data = (int)( f * 1000000);	//nm

    W32data = d;

	////////////////////////////////
	// OFFSET
//#define	H1_CHAMBER_OFFSET		11
//#define	V1_CHAMBER_OFFSET		12
//#define	H2_CHAMBER_OFFSET		13
//#define	V2_CHAMBER_OFFSET		14
	// Added 03/31/2014 new offset added
	if (ram_cs == 11) {	// BRAM11
		xil_printf("H1 Origin Offset (%d) = %d nm \r\n", offset, W32data);
		mps_DpRamOffsetWrite(11, offset, W32data );
	}
	else if (ram_cs == 13) {	// BRAM13
		xil_printf("H2 Origin Offset (%d) = %d nm \r\n", offset, W32data);
		mps_DpRamOffsetWrite(13, offset, W32data );
	}
	else if (ram_cs == 12) {	// BRAM12
		xil_printf("V1 Origin Offset (%d) = %d nm \r\n", offset, W32data);
		mps_DpRamOffsetWrite(12, offset, W32data );
	}
	else if (ram_cs == 14) {	// BRAM15
		xil_printf("V2 Origin Offset (%d) = %d nm \r\n", offset, W32data);
		mps_DpRamOffsetWrite(14, offset, W32data );
	}

}



/*
 * BM fault count every 10 kHz
 *
 */
void	cmd_mps_bm_fault_cnt(int argc, char *argv[])
{
	Xuint32	bm_fault_cnt;
	bm_fault_cnt = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (28*4));
	xil_printf("Reg28 BM fault count  = %d\r\n", bm_fault_cnt);
}



/*
 *
 */
int  	scpi_idn()
{
	xil_printf("PYRTECHCO,f460_1-REV0,0000004271,3.8.4/1.0.10\r");
	return 0;
}



/*
			// 01/20/17 added for fault detection
			if( BM_ERR_DUMP==1'b0 || ID_DUMP == 1'b1 ) begin
				SdiLinkData_Reg[ SdiRamAddr ] <= SdiOrSimGlobalBpmData;
			end
 */
typedef struct _sdiLinkData
{
    int		data[1024];
} sdiLinkData, *pSdiLinkData;
#define sdiLinkDataDiag  	( DDR2_MEM_CAST(pSdiLinkData) 	(0xEEEE0000) )


int		cmd_Diag_SdiDataLinkByFault(int argc, char *argv[])
{
	int i;
	int n;

	n = strtol(argv[1], (char **)0, 10);
	if(n > 780 ) n = 780;
	else if(n == 0) n = 780;

	{
		FOFB_PLB_CS1.regSel = 15;
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
		//read SDI data
		for(i=0; i < n; i++) {
			plb_Reg7Control_1.DacChASet = i;				//select address
			Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
			sdiLinkDataDiag->data[i] = Get_I_DDR2(READ_K_REG);			//read register

			xil_printf("%d: %d\r\n", i, sdiLinkDataDiag->data[i]);
		}
	}

}

//
int		cmd_FastDumpDelayEnable(int argc, char *argv[])
{
	int n;

	n = strtol(argv[1], (char **)0, 10);

	mps_FastDumpDelayEnable(0, n);
	return 0;
}

int		cmd_FastDumpDelayControl(int argc, char *argv[])
{
	int n;

	n = strtol(argv[1], (char **)0, 10);

	mps_npi_data_type_sel(0, n);
	return 0;
}

