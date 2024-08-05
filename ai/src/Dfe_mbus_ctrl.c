/*
 * Dfe_mbus_ctrl.c
 *
 *  Created on: Feb 9, 2011
 *      Author: kha
 *
 *	Dec 15, 2011
 *		Added EEPROM Write function by EPICS 
 *		Database data type must be Float64 because Microblaze part assigned Float type
 *
 *	
 */
#include "xio.h"
#include "fpga_system.h"
#include "types.h"

/* ------------------------ FreeModbus includes --------------------------- */
#include "mb.h"

#include "data_converter.h"
#include "cmd_modbus.h"
#include "sys_eeprom.h"

#include "xparameters.h"
#include "evr.h"
#include "sdi_cc.h"
#include "console.h"
#include "fofb.h"

#include "cmd_modbus.h"
#include "fofb_bram_copy.h"
#include "Dfe_ctrl.h"
#include "mps.h"


/* - Defines - */
#define PROG                    "FreeModbus"
#define REG_INPUT_START         1			/* starting address 1 */
#define REG_INPUT_NREGS         8000
#define REG_HOLDING_START       1			/* starting address 1 */
#define REG_HOLDING_NREGS       0xFFFF
#define	REG_COIL_START			1
#define	REG_COIL_N_MAX			512



#define	PACKET_MAX_RECEIVE		360		//360*4 = 1440 bytes

/* ------------------ Static variables ----------------------------*/
/* unsinged short type */
static USHORT   usRegInputStart   = REG_INPUT_START;
static USHORT   usRegHoldingStart = REG_HOLDING_START;
//Memory space for Modbus
//static USHORT   usRegInputBuf[REG_INPUT_NREGS];			// Input register
static USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];		// Output register
static USHORT   usRegCoileBuf[REG_COIL_N_MAX];			// Output register
static USHORT   usRegInputStatusBuf[REG_COIL_N_MAX];		// input register
static Xuint32	rxData32[370];	//max 360


static ULONG		ddr2_MbusInRegs_addressOffset = DDR2_INPUT16_BASE;
static volatile unsigned char *ptrEEPROM = DDR2_EEPROM_SYSDATA_BASE;	
static volatile unsigned char *ptrSaDDR;	


typedef struct {
		Xuint32 none2  			: 24;	
		Xuint32 none1   		: 3;	
        Xuint32 pm   			: 1;	/* post morterm detected */
		Xuint32 evr_link   		: 1;	/* evr SFP loss ? */
		Xuint32 evr_1hz_ts 		: 1;	/* evr time stemp clock */	
		Xuint32 evr_fifo_full   : 1;	
		Xuint32 sa_10hz 		: 1;	/* 10 Hz trigger alive ? */
    } Sys_status;
Sys_status	DbpmSys_Status;

extern SSP_Config;

typedef struct EE_Ctrl_str
{
    Xfloat32    data;
} EE_Ctrl;
EE_Ctrl	*EE_Ctrl_W;



/*
 *
 */
int cmd_Void( int chn, int data)
{

	xil_printf("cmd_Void: %d, %d\r\n", chn, data);
	return(0);
}

/*
 *  EPICS control Memory Map commands.
 *	Mapping with EPICS Database
 *	BO
 *	AO
 */
int (*Epics_InterpreterPtr[250])(int, int) =
{
	//0.
	&cc_epics_Reset,		//0
	&CC_Sdi_InputDataMode,	//4
	&cmd_Void,				//8
	&cc_epics_NpiTrig,		//12
	&fofb_DpramSimDataIsuBlaze,		//16
	&sdi_cw_pkt_delay,				//fofb_LocalLinkDataType,
	&sdi_ccw_pkt_delay,
	&cc_trigger_delay,		// Local link trig delay from CC
	&fofb_PosXyDataSim,		//32
	&fofb_PosXyDataSimDataMode,
	//10	cc sdi
	&mps_beamC_sim,			//10
	&mps_beamC_2mA,			//11
	&mps_beamC_50mA,		//12
	&mps_FaultTest,			//sdi_cc.c, console cd command  1 is Local BPM data to Global memory
	&mps_OutputLatchClear,	//14
	&mps_Npi_0_Run,			//15, Post Morterm Rn/Stop
	&mps_Npi_Run,			//16  Global Position Data write to NPI base 1
	&mps_OutputLatchClear,
	&mps_ID_dump_out_enable,	//18
	&mps_BM_dump_out_enable,	//19
	//20
	&mps_ai_PlcBmBeamDump_enable,	//20
	&mps_ai_PlcIdBeamDump_enable,	//21
	&fofb_user_fifo_trig,			//22	10/07/2014
	&mps_npi_data_type_sel,			//	0: position, 1:filtered...  NOT used
	&mps_npi_pos_width,				//	0 - 24
	&mps_read_single_pos,			//25
	&mps_ID_gap_sim,				//26  added 03/24/2014
	&mps_id_mask_srtting_1to30,		//27 * 4 = 108
	&mps_id_mask_srtting_31to60,	//28             ID Enable/Disable
	&Evr_SetFdbk_EventCode,	//feedback event code change
	//30	feedback			
	&mps_soft_trig,
	&mps_soft_trig,
	//
	//32 --- AO
	&CC_Sdi_ID_Set,
	&CcRemoteNode_Mask,
	&CC_Sdi_DataStartAddressForDDR,	//34*4=136
	&cc_epics_NpiWidth,	//35
	&EvrTrig1_DlyCtrl,	//36 = 144
	&EvrTrig1_WidthCtrl,	//packet delay control
	&cc_epics_LinkDir,	//38
	&mps_select_sr_bpm_address,		//04/11/2014
	//40
	&mps_select_sr_bpm_enable,
	&cmd_Void,
	&mps_SrBpm_Xoffset,	//42
	&mps_SrBpm_Yoffset,	//mps_BM_Offset1,
	&fofb_SimRamSinglePositionAddressSet,
	&fofb_SimRamSinglePositionDataSet,
	&DAC_ChA_SetData,	//46
	&mps_ext_out0,		//47 Digital Out bit
	&mps_ext_out1,		//48
	&mps_ext_out2,		//49
	//50
	&myAieSim_MemClear,
	&mps_ID_PS_sim,		//51x4 204  10/08/14
	&mps_BM_PS_sim,		//52x4 208
	&mps_ID_BpmFaltEnable,	//53
	&mps_ID_FaltReset,		//54
	&mps_RF_UserDump,		//55	11/17/2014  RF user dump
	&mps_IOWatchDog_Reset,
	&mps_BM_Offset0,		//57 * 4 = 228
	&cmd_Void,
	&mps_ID_ResetSourceSel,	//59 * 4 = 236, Canting status reset
	//60	
	&mps_npi_1_ddr_wfm_tx_size,		//[60]	10/11/14 added
	&cmd_Void,
	&mps_npi_0_ddr_pm_wfm_tx_offset,
	&mps_Clear_BBA_Offset,			//63
	&cmd_Void,						//64
	&mps_BM_bpm_error_dump,			//65
	&mps_BM_bpm_error_dump_count,	//66
	&mps_BM_bpm_enable_bm_zero_detection,	//67	NOT USED
	&mps_GlitchDetectionModeSel,			//68
	&mps_BM_CalModeDispSel,						//69
	//70
	&AI_BmMode_Select,			//70	280
	&mps_PM_PeriodeSetting,		//71
	&mps_BM_NofBpmsZeroSet,		//72
	&mps_Ai_ID_GlitchDetectionModeSel,	//73	292
	&mps_dac_a_output,			//74
	&mps_SdiWfmSel,				//75
	&mps_active_intlock_processClear,	//76
	&cmd_Void,&cmd_Void,&cmd_Void,
	//80
	&cmd_Void,&cmd_Void,&cmd_Void,&cmd_Void,&cmd_Void,
	&cmd_Void,&cmd_Void,&cmd_Void,&cmd_Void,&cmd_Void,
	//90
	&cmd_Void,&cmd_Void,&cmd_Void,&cmd_Void,&cmd_Void,
	&cmd_Void,&cmd_Void,&cmd_Void,&cmd_Void,&cmd_Void,
	
	//100
	&cmd_Void,				//27. DirectMbbo Mask setting 111:1111  0x7F
	&cmd_Void,
	&cmd_Void,		//28. Local/CC Link same
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,&cmd_Void,&cmd_Void,&cmd_Void,&cmd_Void,
	//110
	&CcRemoteNode_Mask,	
	&CC_Sdi_DataStartAddressForDDR,	// 1...31 same as Cell Number, 		
	&cmd_Void,	
	&cmd_Void,	
	&cmd_Void,	
	&cmd_Void,&cmd_Void,&cmd_Void,&cmd_Void,&cmd_Void,
	//120
	&cmd_Void,
	&fofb_DacOutputSelect,
	&fofb_UtUserInjectionAddressSel,	
	&cmd_Void,	
	&cmd_Void,	
	//125
	&fofb_SystemControl,&fofb_SystemControl,&fofb_SystemControl,&fofb_SystemControl,&fofb_SystemControl,	
	//130
	&fofb_SystemControl,&fofb_SystemControl,&fofb_SystemControl,&fofb_SystemControl,&fofb_SystemControl,
	&fofb_SystemControl,&fofb_SystemControl,&fofb_SystemControl,&fofb_SystemControl,&fofb_SystemControl,
	//140
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,
	//150
	&cmd_Void,
	&cmd_Void,
	// Corrector Control point AO	
	&cmd_Void,	//H1
	&cmd_Void,	//V1
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,
	//150
	&cmd_Void,	//H6
	&cmd_Void,	//V6
	&cmd_Void,
	&cmd_Void,	
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,	
	&cmd_Void,
	&cmd_Void,
	//160
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,	
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,	
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,
	//170
	&cmd_Void,
	&cmd_Void,	
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,	
	&cmd_Void,
	&cmd_Void,
	&cmd_Void,		
	&Reboot,	
};



/**/
int	EpicsCmdsInterpreter( int addr, int data)
{
	int	rv;
	{
		addr = addr & 0xFF;
		DFE_CONTROL_REGS->ioReg[addr] = data;		/* same structure with slow */
		Epics_InterpreterPtr[addr](addr, data);

		return 0;
	}
}


/**/
/********************************************************
* Function : MODBUS function code 0x4
*
* Parameters  :
*
* Return value :
*
* Description :
*
* Set_Int16_DDR2( (DDR2_ADC_RAW_FFT_WFM + offset) + (i*2), (Xint16)pResults[i] );
********************************************************/

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
    Xuint16			rValue=0;
    
    //Copy to DDR
 	// 0...4  eeprom infomation
    Set_I_DDR2 ( DDR2_EEPROM_SYSDATA_BASE+(5*4), epics_read_data.TS_SECOND );		//event timestamp
	Set_I_DDR2 ( DDR2_EEPROM_SYSDATA_BASE+(6*4), epics_read_data.TS_OFFSET );
	Set_I_DDR2 ( DDR2_EEPROM_SYSDATA_BASE+(7*4), epics_read_data.EVR_TRIG_CNT );

		
	//xil_printf("evr interrupt %d\r\n", epics_read_data.EVR_TRIG_CNT);
	/*
    xil_printf("InputReg : start=%d, len=%d\r\n", usAddress, usNRegs);
	*/
    if( ( usAddress >= REG_INPUT_START )
        && ( (usAddress + usNRegs) <= (REG_INPUT_START + REG_INPUT_NREGS)  ) )	/* address range checking */
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        
       // xil_printf("InputReg : start=%d, len=%d,  %d\r\n", usAddress, usNRegs, iRegIndex);
                
		/* 16-bit range */
        //ptrDDR =  (volatile unsigned char *) (DDR2_EEPROM_SYSDATA_BASE + iRegIndex*2);
        //memcpy(pucRegBuffer, ptrDDR, usNRegs*sizeof(unsigned short) );
        /* read eeprom data */
        // iRegIndex == 000: eeprom block0 for ip info
        // iRegIndex == 100: eeprom control parameters
        if(iRegIndex == 0) {
        	//memcpy( pucRegBuffer, (unsigned char *)ptrEEPROM,   usNRegs*sizeof(unsigned int) );        	
	        while( usNRegs > 0 )
	        {        	
        		*pucRegBuffer++ = Get_UCHAR_DDR2(DDR2_EEPROM_SYSDATA_BASE + iRegIndex++);
        		*pucRegBuffer++ = Get_UCHAR_DDR2(DDR2_EEPROM_SYSDATA_BASE + iRegIndex++);
	            //iRegIndex++;
	            usNRegs--;        		
    		}
    	}
		else {            
	        while( usNRegs > 0 )
	        {
	
	            *pucRegBuffer++ = ( unsigned char )( rValue >> 8 );		/* 16-bit high byte to buffer */
	            *pucRegBuffer++ = ( unsigned char )( rValue & 0xFF );   /* 16-bit low byte to buffer */
	
	            iRegIndex++;
	            usNRegs--;
	        }
		}              
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}


//volatile unsigned char *ptrDDR2_CTRL_MEM = DDR2_CIO_BASE;

#define	REG_MODE		0

/********************************************************
* Function : MODBUS function code 0x6
*
* Parameters  :
*
* Return value :
*
* Description :
*		WRITE: FC 6, FC 0x10
*
*	CLIENT address 1  -> SLAVE address 0
*
*	Holding register support only 32-bit data format
*
* --------------------------------------------------------------------------------	
*	Modbus Client : Modbus Poll  4/7/2011
*		F 03 Read Holding registers   Long
*				Incrress offset 0..2..4   .... 2 steps
*			DDR2_SLOW->space[0] , memory address  18  ->  Modbus offset 36
*			DFE_CONTROL_REGS->ioReg[32],                 52  ->  Modbus offset 18
*
*
*
********************************************************/
eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             wRegIndex, iRegIndex, usAddressIndex, ret, cmdAddr, length32;
    Xuint16			i, rVal, wVal;
	short			base=0;
	Xuint32			data32, rd_start_address, ddr2_addressOffset;
    Xuint32			eeAddr, offset, r32bitVal;
    int	addr=0;	
	Xuint8	dat[4];
	float	fval;
	unsigned char	wbuf[10];
	unsigned int base_addr;
	EE_Ctrl_W = (EE_Ctrl *)wbuf;
			
	int	DataType;
	
	
    if( ( usAddress >= REG_HOLDING_START ) && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        usAddressIndex = wRegIndex = ( int )( usAddress - usRegHoldingStart );

        length32 = usNRegs/2;	

		if(usAddressIndex == 0x1000 && length32 == 180) {
			DataType = 1;	//ref-x
			base_addr = usAddressIndex;
		}
		else if(usAddressIndex == 0x1200 && length32 == 180) {
			DataType = 2;	//ref-y
			base_addr = usAddressIndex;
		}  /* P.I.D  90 * 12 */
		else if( (usAddressIndex >= 0x1300 && usAddressIndex < 0x1400) && length32 == PACKET_MAX_RECEIVE) {
			DataType = 3;	//V1-x
			base_addr = usAddressIndex-0x1300;
		}
		else if( (usAddressIndex >= 0x1400 && usAddressIndex < 0x1500) && length32 == PACKET_MAX_RECEIVE) {
			DataType = 4;	//V1-y
			base_addr = usAddressIndex-0x1400;
		}
		else if( (usAddressIndex >= 0x1500 && usAddressIndex < 0x1600) && length32 == PACKET_MAX_RECEIVE) {
			DataType = 5;	//V2-x
			base_addr = usAddressIndex-0x1500;
		}
		else if( (usAddressIndex >= 0x1600 && usAddressIndex < 0x1700) && length32 == PACKET_MAX_RECEIVE) {
			DataType = 6;	//V2-y
			base_addr = usAddressIndex-0x1600;
		}
		else if( (usAddressIndex >= 0x1700 && usAddressIndex < 0x1800) && length32 == PACKET_MAX_RECEIVE) {
			DataType = 7;	//V3-x
			base_addr = usAddressIndex-0x1700;
		}
		else if( (usAddressIndex >= 0x1800 && usAddressIndex < 0x1900) && length32 == PACKET_MAX_RECEIVE) {
			DataType = 8;	//V3-y
			base_addr = usAddressIndex-0x1800;
		}

		else if(usAddressIndex == 0x1C00 && length32 == 360) {
			DataType = 9;	//BPM position simulation X,y 
			base_addr = usAddressIndex;
		}
		else if(usAddressIndex == 0x1D00 && length32 == 360) {
			DataType = 10;	//Bpm X,y Mask
			base_addr = usAddressIndex;
		}
		else if(usAddressIndex == 0x1E00 && length32 == 540) {
			DataType = 11;	//Ut Mask
			base_addr = usAddressIndex;
		}
		// Multiple waveforms
		else if( (usAddressIndex < 0x8200 && usAddressIndex >= 0x8000) && length32 == PACKET_MAX_RECEIVE) {
			DataType = 12;	//Ut-x
			base_addr = usAddressIndex-0x8000;
		}
		else if( (usAddressIndex < 0x8500 && usAddressIndex >= 0x8200) && length32 == PACKET_MAX_RECEIVE) {
			DataType = 13;	//Ut-y
			base_addr = usAddressIndex-0x8200;
		}
		else if( (usAddressIndex < 0x8600 && usAddressIndex >= 0x8500) && length32 == PACKET_MAX_RECEIVE) {
			//xil_printf("Vx\r\n", usAddressIndex, length32);
			DataType = 14;	//V-x
			base_addr = usAddressIndex-0x8500;
		}	
		else if( (usAddressIndex < 0x8700 && usAddressIndex >= 0x8600) && length32 == PACKET_MAX_RECEIVE) {
			DataType = 15;	//V-y
			base_addr = usAddressIndex-0x8600;
		}				
		else if( (usAddressIndex < 0x8800 && usAddressIndex >= 0x8700) && length32 == PACKET_MAX_RECEIVE) {
			DataType = 16;	//User Waveform for Eigen injection
			base_addr = usAddressIndex-0x8700;
		}		
		else DataType = 0;
			
						               
        switch ( eMode )
        {
            /* Pass current register values to the protocol stack. */       		
	        case MB_REG_READ:	        
				usNRegs >>= 1;		//for 32-bit register																									
#if 0									
		        while( usNRegs > 0 )
		        {	
			        //ddr2_addressOffset = rd_start_address + (iRegIndex * 4);			        		        										
			        if(usNRegs < 100)
						r32bitVal =  DFE_CONTROL_REGS->ioReg[usAddressIndex+iRegIndex];
					else r32bitVal = 0;	
		        	//r32bitVal = Get_uInt32_DDR2( ddr2_addressOffset);
		            *pucRegBuffer++ = ( unsigned char )( r32bitVal >> 8 );
		        	*pucRegBuffer++ = ( unsigned char )( r32bitVal >> 0  );
		        	*pucRegBuffer++ = ( unsigned char )( r32bitVal >> 24 );
		        	*pucRegBuffer++ = ( unsigned char )( r32bitVal >> 16  );			        		            
					//xil_printf("%d\t0x%x = 0x%X \r\n", iRegIndex, ddr2_addressOffset, r32bitVal);
					iRegIndex++;
		            usNRegs--;
		        }	
#endif		    
				//xil_printf("read %d signals\r\n", usNRegs);  
				iRegIndex = 0;  
		        while( usNRegs > 0 )
		        {        	
	        		*pucRegBuffer++ = Get_UCHAR_DDR2( DDR3_EPICS_DATA_BASE + iRegIndex++);
	        		*pucRegBuffer++ = Get_UCHAR_DDR2( DDR3_EPICS_DATA_BASE + iRegIndex++);       	
		            usNRegs--;        		
	    		}
    				        	            	      
	            break;
            	
#define	_16BIT	0	            
            /* Update current register values with new values from the
             * protocol stack. */
	        case MB_REG_WRITE:	   
     		        	
	  			if( ( usAddress >= REG_HOLDING_START ) && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) ) 
	  			{
		  			xil_printf("Start = %x, base=%d, Len = %d, type=%d\r\n", usAddressIndex, base_addr, length32, DataType);
#if (!_16BIT)		  			
		  			usNRegs >>= 1;
		  			wRegIndex = 0;	
#endif		  			
		            while( usNRegs > 0 )
		            {
#if (_16BIT)			            
		                usRegHoldingBuf[wRegIndex]  = *pucRegBuffer++ << 8;
		                usRegHoldingBuf[wRegIndex] |= *pucRegBuffer++;
#else	//32-bit
		                rxData32[wRegIndex]  = *pucRegBuffer++ << 8;
		                rxData32[wRegIndex] |= *pucRegBuffer++;		                		                
		                rxData32[wRegIndex] |= *pucRegBuffer++ << 24;
		                rxData32[wRegIndex] |= *pucRegBuffer++ << 16;	               
#endif		                		                	               		                
		                switch( DataType ) {
			                case 1:
								//Hex32_to_Float(rxData32[wRegIndex], &fval);	//convert floating point
								//FOFB_DATA->H_RefOrbit[wRegIndex] = (int)(fval*1000000000);	//convert nm  180
			                	FOFB_DATA->H_RefOrbit[wRegIndex] = rxData32[wRegIndex];
		                	break;
			                case 2:
								//Hex32_to_Float(rxData32[wRegIndex], &fval);
								//FOFB_DATA->V_RefOrbit[wRegIndex] = (int)(fval*1000000000);	//nm  180
			                	FOFB_DATA->V_RefOrbit[wRegIndex] = rxData32[wRegIndex];
		                	break;
							
			                case 3:		
			                	FOFB_DATA->H_Kp[base_addr*360+wRegIndex] = rxData32[wRegIndex];            			                	
			                	break;
			                case 4:		
			                	FOFB_DATA->H_Ki[base_addr*360+wRegIndex] = rxData32[wRegIndex];                			                	
			                	break;	
			                case 5:		
			                	FOFB_DATA->H_Kd[base_addr*360+wRegIndex] = rxData32[wRegIndex];                			                	
			                	break;	
			                case 6:		
			                	FOFB_DATA->V_Kp[base_addr*360+wRegIndex] = rxData32[wRegIndex];               			                	
			                	break;
			                case 7:		
			                	FOFB_DATA->V_Ki[base_addr*360+wRegIndex] = rxData32[wRegIndex];              			                	
			                	break;	
			                case 8:		
			                	FOFB_DATA->V_Kd[base_addr*360+wRegIndex] = rxData32[wRegIndex];          			                	
			                	break;	
			                	
			                case 9:		
			                	FOFB_DATA->Xy_Pos_SimData[wRegIndex] =  rxData32[wRegIndex];              			                	
			                	break;	
			                case 10:					                		                	
			                	break;	
							
			                // multiple matrix		                			                			                		                
			                case 12:	//URAM
			                	pUT_X->data[base_addr*360+wRegIndex] = rxData32[wRegIndex];  	//180
			                	break;
			                case 13:	//URAM
			                	pUT_Y->data[base_addr*360+wRegIndex] = rxData32[wRegIndex];  		//180
			                	break;		
			                case 14:	//V
			                	//xil_printf("%d= %x\r\n", wRegIndex, rxData32[wRegIndex] );
			                	pVM_X->data[base_addr*360+wRegIndex] = rxData32[wRegIndex];
			                	break;	
			                case 15:	//V
			                	pVM_Y->data[base_addr*360+wRegIndex] = rxData32[wRegIndex];
			                	break;
			                case 16:	//User Waveform
			                	FOFB_DATA->UserWfmData[base_addr*360+wRegIndex] = rxData32[wRegIndex];
			                	break;			                				   			                	
	                	}	
	                	
		                wRegIndex++;
		                usNRegs--;			               			                            
		            } // end of while
		               
		            
		            //xil_printf("addr=%d,  length32 = %d\r\n", usAddressIndex, length32);		      
		            
		            if(length32 == 1) {	/* EPICS Control single 32-bit */
		            	data32 = rxData32[0]; //usRegHoldingBuf[usAddressIndex+1] * 0x10000 + usRegHoldingBuf[usAddressIndex] ;	/* 32-bit format */		            	
		            	if(wRegIndex < EEPROM_PARAMETER_OFFSET) {				            
				            /* Controls */				            
				            cmdAddr = usAddressIndex;
				            ret = EpicsCmdsInterpreter( cmdAddr, data32); 		            
				            //
				            xil_printf(">>> Client Cmd addr=%d, data=%d (0x%x) \r\n", cmdAddr, data32, data32 );	
			            }
			            else {	/* eeprom Write function */
				            Hex32_to_Float( data32, &fval);
				            /*
				            offset= (usAddressIndex-EEPROM_PARAMETER_OFFSET)/2;
				            eeAddr = DDR2_EEPROM_PARAMETER_BASE + (offset*4);
				            Set_F_DDR2(eeAddr, fval);
				            EE_Ctrl_W->data = fval;
							if(0 != EE_Write( EEPROM_USER_0_OFFSET_ADDRESS+(offset*4), wbuf, 4 ) ) {
								xil_printf("EERPM Write error  %d\r\n", offset);
							}				            							
				            printf("DDR addr=0x%x, eeprom off=%d, data32=0x%x, %f\r\n", eeAddr, offset, data32, fval );
				            */
			            }			            			            					
		            }
		            else {	/* MULTIPLE Waveform Write */
		            	//Write to FPGA RAM and Registers	            	
		                switch( DataType ) {
			                case 1:	//Reference Orbit - X
								fofb_ReferenceOrbitWrite(0, 180, &FOFB_DATA->H_RefOrbit[0]); 	//to FPGA DPRAM
		                	break;
			                case 2:	//Reference Orbit - Y
								fofb_ReferenceOrbitWrite(180, 360, &FOFB_DATA->V_RefOrbit[0]);
								break;
							case 9:	
								xil_printf("Position Sim Copy to BRAM ......\r\n");	
								XyPositionSimDataSet( &FOFB_DATA->Xy_Pos_SimData[0] );
		                		break;	
		                	case 16:		
		                		xil_printf("User Waveform Copy to BRAM......\r\n");	
		                		fofb_WavefromBRamWrite(0, &FOFB_DATA->UserWfmData[0]);
		                		break;
		                		
	                	}	
		                			      
	                			                	      
			            for(i=0; i<length32; i++) {
				            
				           // ret = EpicsCmdsInterpreter( i, data32);
#if (_16BIT)			            
				            data32 = usRegHoldingBuf[usAddressIndex+(i*2)+1] * 0x10000 + usRegHoldingBuf[usAddressIndex+(i*2)] ;
#else    
							//data32 = rxData32[i];		           			            
#endif				       
							//xil_printf("%d=  %x\r\n", i, rxData32[i]);
				            //Set_F_DDR2( (0xCF000000+usAddressIndex)+(i*4), fval);	//Write to DDR Memory				            
				            //printf("%d : %x , %f\r\n", i, data32, Get_F_DDR2((0xCF000000+usAddressIndex)+(i*4)) );				            
				            
			                switch( DataType ) {
				                case 1:
									//printf("%d= %d\r\n", i, FOFB_DATA->H_RefOrbit[i] );
			                	break;
				                case 2:
									//printf("%d= %d\r\n", i, FOFB_DATA->V_RefOrbit[i] );
			                	break;

				                case 13:
				                	//Hex32_to_Float(pUT_Y->data[i], &fval);
				                	//printf("%d = %f\n", i, fval);
				                	//printf("%d= %x\r\n", i, pUT_Y->data[i] );
				                break;
			                	//default:
			                		//printf("%d= %d\r\n", i, rxData32[i] );			                	                	
		                	}	
		                					            
				           				            				           				            				            
			            }
		            }
	            }
			    else
			    {
			        eStatus = MB_ENOREG;
			    }            
	            break;                                 
        }	/* END OF SWITCH */
		
    }

    return eStatus;
}


/**/
/*
 * F1, multiple Coil status Write and reading
 *	eMode 0 : reading
 *	eMode 1 : writing
 */
eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
	eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex, usAddressIndex;
    unsigned char  *pCharOut, *pCharIn;
    unsigned short *pShortIn;
    unsigned char 	mask=0;
    int				ret, i, j;
	unsigned short	mData=0;

	
	
	xil_printf("Rcv Coils Reg = %d,  %d \r\n", usAddress, usNCoils);
    if( ( usAddress >= REG_COIL_START ) &&
        ( usAddress + usNCoils <= REG_COIL_START + REG_COIL_N_MAX ) )
    {
	    usAddressIndex = iRegIndex = ( int )( usAddress - REG_COIL_START );
		switch ( eMode ) {
			case 0:		//Multiple Reading
            	if( usNCoils > 1 )
            	{														           	
					/* Pack bits into output */
					for (i=0; i<usNCoils; i++) {
						usRegCoileBuf[i] =  (unsigned short) DFE_CONTROL_REGS->ioReg[i]&0xF;
					}
					pShortIn = (unsigned short *)usRegCoileBuf;	/* in  array */
					pCharOut = (unsigned char  *)pucRegBuffer;	/* out */
					/* Subtract 1 because it will be incremented first time */
					pCharOut--;
					for (i=0; i<usNCoils; i++) {
					    if (i%8 == 0) {
					        mask = 0x01;
					        pCharOut++;
					        *pCharOut = 0;
					    }
					    *pCharOut |= ((*pShortIn++ ? 0xFF:0) & mask);
					    mask = mask << 1;
					}
					xil_printf("len=%d , 0x%x\r\n", usNCoils, pucRegBuffer[0]);
            	}
            	else {	//Single reading
	            	pucRegBuffer[usAddressIndex] = usRegCoileBuf[usAddressIndex];
	            	xil_printf("len=%d , 0x%x\r\n", usNCoils, pucRegBuffer[usAddressIndex]);
            	}
				break;
			case 1:		//Multiple Writing
	            if( usNCoils > 1 )
	            {					
					/*
					for(i=0; i<usNCoils; i++)
						xil_printf("0x%x,", pucRegBuffer[i]);
					xil_printf("\n");
					*/
		            pCharIn = (unsigned char *)pucRegBuffer;
		            pCharIn--;

		            for (i=0, j=usNCoils-1; i<usNCoils; i++, j--) {
		                if (i%8 == 0) {
		                    mask = 0x01;
		                    pCharIn++;
		                }
		                usRegCoileBuf[i] = (*pCharIn & mask) ? 1:0;		                
						mData |= (usRegCoileBuf[i] << j) ;
		                mask = mask << 1;
		            }
		            /*
            		for(i=0; i<usNCoils; i++)
            			xil_printf("data[%d]=0x%x\r\n", i, usRegCoileBuf[i] );
					*/
					xil_printf("Received Address=%d, n=%d\r\n", usAddressIndex, usNCoils);		
					if(usAddressIndex >=100) {
						xil_printf("Setup multiple event RAM %d,  %d\r\n",usAddressIndex-32, usNCoils);
						//Event lookup table
						if(usNCoils > 254) usNCoils = 254;
						/* 0 is trig 0 */
						//LTB_MultipleEventCode2Trig0(usRegCoileBuf, 0, usNCoils);
					}
					else {
						/* Controls */
						ret = EpicsCmdsInterpreter( usAddressIndex, mData);
					}
	            }
	            else {	/* single write */
	            	if(usAddressIndex < 100) {
			            usRegCoileBuf[usAddressIndex] = *pucRegBuffer;	         
			            /* Controls */
			            ret = EpicsCmdsInterpreter( usAddressIndex, usRegCoileBuf[usAddressIndex]);
		            }
		            else {
			            xil_printf("BO address range error less then 32, received %d \r\n", usAddressIndex);
		            }
	            }
				break;
		}
	}

    return	eStatus;
}


/**/
/*
 *	F02, read_input_status_tcp
 */
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
	eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
    unsigned char  *pCharOut;
    unsigned short *pShortIn;
    unsigned char 	mask=0;
    int				i;
    int				status;
	
	int bpmLinkStatus, ccLinkStatus, sfp1, sfp2;
	//xil_printf("DiscreteCB=%d  addr=%d\r\n", usNDiscrete, usAddress);

	//SFP LINK Status
	//bpmLinkStatus = (Get_I_DDR2(XPAR_SDI2_0_BASEADDR+(14*4)) >> 24)&0xFF;
	ccLinkStatus  = (Get_I_DDR2(XPAR_SDI2_CC_BASEADDR+(14*4)) >> 24)&0xFF;
						
	epics_read_data.bi[0] = bpmLinkStatus & 0x10;
	epics_read_data.bi[1] = bpmLinkStatus & 0x04;
	epics_read_data.bi[2] = ccLinkStatus  & 0x10;
	epics_read_data.bi[3] = ccLinkStatus  & 0x04;
	//sys status
	status = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (23*4));
	epics_read_data.bi[4] = status & 0x1;
	epics_read_data.bi[5] = status & 0x2;
	epics_read_data.bi[6] = status & 0x4;
	epics_read_data.bi[7] = status & 0x8;
	epics_read_data.bi[8] = status & 0x10;
	epics_read_data.bi[9] = status & 0x20;
	epics_read_data.bi[10] = status & 0x40;
	epics_read_data.bi[11] = status & 0x80;
	epics_read_data.bi[12] = (status >> 8)& 0x1;
	epics_read_data.bi[13] = (status >> 8)& 0x2;
	epics_read_data.bi[14] = (status >> 8)& 0x4;	//PosSimulation ?
	epics_read_data.bi[15] = (status >> 8)& 0x8;
	

    if( ( usAddress >= REG_INPUT_START )
        && ( (usAddress + usNDiscrete) <= (REG_INPUT_START + REG_INPUT_NREGS)  ) )	/* address range checking */
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
		//LED_FP2(1);
    	if( usNDiscrete > 1 )	/* Multiple reading */
    	{
			/* Pack bits into output */
			for (i=0; i<usNDiscrete; i++) {
				usRegCoileBuf[i] =  (unsigned short) epics_read_data.bi[i];
			}
			/* Pack bits into output */
			pShortIn = (unsigned short *)usRegCoileBuf;  	//usRegInputStatusBuf;	/* in  array */
			pCharOut = (unsigned char  *)pucRegBuffer;		/* out */
			/* Subtract 1 because it will be incremented first time */
			pCharOut--;
			for (i=0; i<usNDiscrete; i++) {
			    if (i%8 == 0) {
			        mask = 0x01;
			        pCharOut++;
			        *pCharOut = 0;
			    }
			    *pCharOut |= ((*pShortIn++ ? 0xFF:0) & mask);
			    mask = mask << 1;
			}
			//xil_printf("len=%d , 0x%x\r\n", usNDiscrete, pucRegBuffer[0]);
    	}
    	else {	//Single reading
        	pucRegBuffer[iRegIndex] = usRegInputStatusBuf[iRegIndex];
        	//xil_printf("len=%d , 0x%x\r\n", usNDiscrete, pucRegBuffer[iRegIndex]);
    	}
		//LED_FP2(0);
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

