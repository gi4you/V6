/*
 *
 * 	Module : console_cmds.c
 *	Descriptions:
 *		remote console commands for debugging and excution.
 * 
 * 	Jan 20, fixed Mac address dynamic setting.
 * 
 */


#include <stdlib.h>		//dtos
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <sys/timer.h>
#include "sys/types.h"
#include "xuartlite_l.h"


#include "types.h"
#include "uart_print.h"
#include "ssp_eeprom.h"
#include "version.h"
#include "flash_config.h"

//#include "dfe_comm.h"

#include "fpga_system.h"
#include "get_paramters.h"
//#include "sig_lib.h"

#include "sys_eeprom.h"
#include "flash_config.h"
#include "dfe_ctrl.h"
//#include "accelerator.h"


extern net_config_t cfg_opts[];
extern SSP_Config_t SSP_Config;
extern sTrig;
extern SaPositionCalMode;
extern MachineType;



#if 0
void	cmd_Consol_BeamTrigDelay(int argc, char *argv[])
{ 
int     len;
Xint32	 	ticks;
	
	ticks = strtol(argv[1], (char **)0, 10);
	Dfe_BeamTrigDly(ticks);
}


void	cmd_Console_Dfe_KxScalFactort(int argc, char *argv[])
{
	Xuint32 	kx;

	kx = strtol(argv[1], (char **)0, 10);			
	cmd_Dfe_KxScalFactor( 0, kx);
	xil_printf("Kx= %d (nm)\r\n", kx);
}

void	cmd_Console_Dfe_KyScalFactort(int argc, char *argv[])
{
	Xuint32 	ky;

	ky = strtol(argv[1], (char **)0, 10);			
	cmd_Dfe_KyScalFactor( 0, ky);
	xil_printf("Ky= %d (nm)\r\n", ky);
}


void	cmd_Console_Dfe_SA_PosX_Offset(int argc, char *argv[])
{
	Xuint32 	offset;

	offset = strtol(argv[1], (char **)0, 10);			
	cmd_Dfe_SA_PosX_Offset( 0, offset);
	xil_printf("X offset = %d (nm)\r\n", offset);
}

void	cmd_Console_Dfe_SA_PosY_Offset(int argc, char *argv[])
{
	Xuint32 	offset;

	offset = strtol(argv[1], (char **)0, 10);			
	cmd_Dfe_SA_PosY_Offset( 0, offset);
	xil_printf("X offset = %d (nm)\r\n", offset);
}


void	cmd_Console_TempRead(int argc, char *argv[])
{
	Dfe_ADT7410_TempSensRead(1);
}

void	cmd_Console_SfpDataRead(int argc, char *argv[])
{
	int addr;
	addr = strtol(argv[1], (char **)0, 10);
	Dfe_SFP_DataRead(addr & 0xFFF ,1);
}



void	cmd_Console_MachineType(int argc, char *argv[])
{
	Xuint32 	mode;
	
	mode = strtol(argv[1], (char **)0, 10);			
	cmd_Dfe_MachineType( 0, mode);
}

void	cmd_Console_ADC_DataSelect(int argc, char *argv[])
{
int	dt;

	dt = strtol(argv[1], (char **)0, 10);
	Dfe_ADC_DATA_MUX_SEL( dt);
}

// npi_data_gen IP code data input type select
//	1: Button signals copy to DDR memory
//	0: PT signals copy to DDR memory
void	cmd_Console_NPI_DataSelect(int argc, char *argv[])
{
int	dt;

	dt = strtol(argv[1], (char **)0, 10);
	NPI_DataSelect( dt & 1);
}

void	cmd_Console_PmReset(int argc, char *argv[])
{
int	pm;

	pm = strtol(argv[1], (char **)0, 10);	
	PM_Reset( pm & 1);
}
/*
 */
void	cmd_Console_FaAvgVal(int argc, char *argv[])
{
float	val;
int	hex;
int	avg;
	
	avg = strtol(argv[1], (char **)0, 10);
	if(avg > 300 || avg < 1 ) avg = 38;
	val = (float) ( 2.0f / (avg+1) * 2.1475e+009 );		//2^31
	hex = (int) val;
	//Set_I_DDR2(XPAR_BPM_CTRL_MON_PLBW_0_MEMMAP_FAAVGCOEFF, hex);
	
	xil_printf("Fa Averaging : Avg=%d , hex = 0x%X\r\n", avg, hex);
}


/*
 *
 	Fs = 117.2931e6
	DDSFreq = 30.4e6; %31.8e6;
	NumPhaseBits = 32;

    %%%%%   0x427EB480  ,  1,115,600,000
    %%%%%	hex2dec('427EB480')
    % Write DDS frequency (xilinx model only)
    DDSDin = (DDSFreq * 2^NumPhaseBits / Fs) / 2^NumPhaseBits;
    DDSDin * 2^32    %%%  4.2950e+009   0x425A1580
 */
int cmd_Console_DDS_FreqCtrl(int argc, char *argv[])
{
//double 	Fs = 117.2931;		//ALS
double	DDSFreq = 30.4; 	//%31.8e6;

	
	if(strcmp("-h", argv[1]) == 0) {
		xil_printf("Input parameter is digital IF frequency value, ex) 30.4 is IF setting 30.4 MHz\r\n");		
	}
	
	DDSFreq = strtod(argv[1], (char **)0);
	
	if(DDSFreq < 29.0 || DDSFreq > 33.0) {
		DDSFreq = 30.4;
	}
	/*
	Fs *= 1000000;
	DDSFreq *= 1000000;
	
	DDSDin = (DDSFreq * 4294967295.f / Fs) / 4294967295.f;
	DDS_hex = (Xuint32)(DDSDin * 4294967295.f);
	printf("IF=%.3f [MHz],  dds set = 0x%X\n", DDSFreq/1000000, DDS_hex);
	Def_DDS_DinCtrl(  DDS_hex );
	*/
	Def_DDS_DinCtrl(  DDSFreq );
	return 0;	
}



/*
 *
 */
void	cmd_ConsoleReadRF_Power(int argc, char *argv[])
{
	int 	i, turns;
	Xint16	adc0;
	Xint16  ReadBuffer;

	if(MachineType != LTB)	{
		turns = strtol(argv[1], (char **)0, 10);
		if(turns < 10) turns = 20;
		if(turns > 52) turns = 52;
	}
	else
		turns = 1;	/* LTB is single turn */
	RF_PowerCalulation(turns, 1);
}


/*
 *
 */
void	cmd_Console_AdcOffsetCalibration(int argc, char *argv[])
{
	int 	mode;
	mode = strtol(argv[1], (char **)0, 10);
	xil_printf("1 is TBT, 2 is FA 0 is PTC S21\r\n");	
	if( mode == 0)
		PTC_meausred_S_Paramters_zero_position();
	else	
	AdcOffsetClibration(mode);	
}



/*
	FIX_24_23
	
		1.0 = 0x7FFFFF
		0.5 = 0x400000
		0.0 = 0
		-0.5= 0xC00000
		-1.0= 0x800000

 */

void	cmd_Console_PositionCompLimitValues(int argc, char *argv[])
{
	Xuint32 	no, value;
	char *end;
	float	tmp;
	
	no = strtol(argv[1], (char **)0, 10);
	if(no != 4) {
		//value = strtol(argv[2], (char **)0, 10);
		tmp = strtof (argv[2], &end);
		value = (Xint32)(tmp);
	}
	else {	 /* set value +- 1.0 */
		tmp = strtof (argv[2], &end);
		if(tmp > 1.0 || tmp < -1.0) {
			xil_printf("Input range +/- 1.0 for FIX24_23 format\r\n");
			return ;
		}
		else {
			value = (Xint32)(tmp * 8388607.);
			value &= 0xFFFFFF; /* 24 bit */
		}
	}
	
	switch(no) {
		case 0:
			/* + 10,000  is + 10 mm*/
			cmd_Dfe_XposCompHighLimit(0, value);
			break;
		case 1:	
			/* - 10,000  is - 10 mm*/
			cmd_Dfe_XposCompLowLimit( 0,  value);
			break;
		case 2:	
			cmd_Dfe_YposCompHighLimit( 0,  value);
			break;
		case 3:	
			cmd_Dfe_YposCompLowLimit( 0,  value);
			break;
		case 4:				
			cmd_Dfe_ADC_OverflowLimit( 0,  value);
			break;
	}
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
		if(status == XST_SUCCESS) xil_printf("Flash Update Enabled\r\n");
		else {
			xil_printf("Flash Write error\r\n");
			goto Fail;
		}
	}
	else if(mode == 2){
		status = UsrFLASH_Write(FLASH_BOOTINFO_BASEADDR, "app...:", 7);
		if(status == XST_SUCCESS) xil_printf("Flash Update Disabled and BPM mode\r\n");
		else {
			xil_printf("Flash Write error\r\n");
			goto Fail;
		}
	}
	else {
		xil_printf("Flash Update Not configured... use 1 is Update, 2 for Normal firmware\r\n");
		flash_mutex_Ulock();
		goto Fail;
	}
	flash_mutex_Ulock();


	//Dfe_Reboot();

Fail:
	xil_printf("Reboot command fail.....\r\n");
}

/**/
void	cmd_SoftTriggerSet(int argc, char *argv[])
{
	int	i;
	Xuint32 	mode;

	mode = strtol(argv[1], (char **)0, 10);
	sTrig = mode;

	if(mode == 0) {
		/*
		xil_printf("clear... -> \r\n");
	    for(i=0; i<1000000; i++) {
		    Set_Int16_DDR2_ADC1(i*2, 0);
    	}
    	*/
    	Dfe_SoftTrigSet();

	}
}


/**/
/*
 *	input n is us unit
 */
void	cmd_ADC_enableWidth(int argc, char *argv[])
{
	int     len;
	Xuint32 	width;
//char	*e;

    len = strlen(argv[1]);
	width = strtol(argv[1], (char **)0, 10);

	Dfe_ADC_enableWidth(117, width);
}

/**/
/*
 *	input n is us unit
 */
void	cmd_npi_TBT_enableWidth(int argc, char *argv[])
{
	int     len;
	Xuint32 	width;
//char	*e;

    len = strlen(argv[1]);
	width = strtol(argv[1], (char **)0, 10);

	Dfe_TbT_enableWidth(0, width);
}

/**/
/*
 *	input n is us unit
 */
void	cmd_Console_npi_FaWfm_enableWidth(int argc, char *argv[])
{
	int     len;
	Xuint32 	width;

    len = strlen(argv[1]);
	width = strtol(argv[1], (char **)0, 10);

	Dfe_FaWfm_enableWidth(0, width);
}


/**/
/*
 *	input n is tets mode
 *
 *	0: adc raw data to DDR
 *	1: count values to DDR ( ADC, TBT, FA )
 *	------------ Mode 2 and 3 is only Test project
 *	2: ADC channel input is ADC raw data
 *	3: ADC channel input is SIN, COS
 *
 */
void	cmd_npi_TestMode(int argc, char *argv[])
{
	int     len;
	Xuint32 	mode;

	len = strlen(argv[1]);
	mode = strtol(argv[1], (char **)0, 10);
	switch(mode) {
		case 0:
		case 1:
			Dfe_SetNpiTestMode(mode);
			break;
		case 2:	
			xil_printf("ADC Channels input is ADC raw data\r\n");
			//Set_I_DDR2(XPAR_BPM_CTRL_MON_PLBW_0_MEMMAP_ADC_DEBUG_SEL, 0);
			break;
		case 3:
			xil_printf("ADC Channels input is SIN, COS, QERR\r\n");
			//Set_I_DDR2(XPAR_BPM_CTRL_MON_PLBW_0_MEMMAP_ADC_DEBUG_SEL, 1);
			break;
		default:
			xil_printf("Mode error...\r\n");
		}
}

void	cmd_Console_npi_PortSelect(int argc, char *argv[])
{
	Xuint32 	mode;
	mode = strtol(argv[1], (char **)0, 10);

	Dfe_NPI_PortSelect(mode);
}



void	cmd_Consol_TriggerSourceSelect(int argc, char *argv[])
{
char    str[10];    
int     len;
int	 	size;
	
    len = strlen(argv[1]);
    strncpy(str, argv[1], len);
	size = strtol(str, (char **)0, 10);
		
	Dfe_TrigSourceSelect( size );
}

void	cmd_Consol_RevFreqSelect(int argc, char *argv[])
{
Xint16	 	size;
	
	size = strtol(argv[1], (char **)0, 10);
		
	Dfe_RevFreqSelect( size );
}

void	cmd_Consol_Dfe_TbtGateDelay(int argc, char *argv[])
{ 
int     len;
Xint16	 	size;
	
	size = strtol(argv[1], (char **)0, 10);
#if(ALS)	
	if(size > 70) printf("delay step is less then 70 for ALS , Td=(1/Frev * N )\r\n"); 
#else
	if(size > 300) printf("delay step is less then 300 , Td=(1/Frev * N )\r\n"); 
#endif	
	else
		Dfe_TbtGateDelay(size);
}

void	cmd_Consol_Dfe_TbtGateWidth(int argc, char *argv[])
{
Xint16	 	size;

	size = strtol(argv[1], (char **)0, 10);
	
	if(size < 2 || size > 300) {
		printf("Gate Width is 10 - 300, Tw=(1/Frev * N )\r\n");
	}
	else {
		Dfe_TbtGateWidth(size);
	}
}
#endif





 /**/  
int	Mac_addr_get(char *data, unsigned char *pMacAddress)
{
char *tokenPtr, *endptr;
int	index, col;
unsigned char	mac[8];
int	len;
unsigned char *from, *to;
   
	for(len=0; len<8; len++)
		mac[len] = 0;
		
	len = strlen(data);
	if( len < 15 && len > 17) {
		printf("string length is shot or long\r\n");
		return -1;
	}
	index = 0;
	tokenPtr = strtok( &data[index], ":" );
	while ( tokenPtr != NULL )
	{
   		for(col=0; col<6; col++) {
			mac[col] = (unsigned char)strtol(tokenPtr, &endptr, 16);
   			tokenPtr = strtok( NULL, ":" );
   			if(tokenPtr == NULL) break;
		}
   		if(col > 6) {
   			printf("out of mac address\r\n");
   			return -2;
   		}
	}
	if(col < 5) return -3;
	//printf("New Mac Address = %X:%X:%X:%X:%X:%X \r\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	
	col += 1;
	if ( col > 0 )
	{
	  	from = (char *)mac;  to = (char *)pMacAddress;
	  	do *to++ = *from++; while (--col != 0);
	}
   	
	//strncpy(pMacAddress, mac, 6);   	
   	return 0;
}

 /**/  
int	ip_addr_get(char *data, unsigned char *ipNo)
{
char *tokenPtr, *endptr;
int	index, col;
unsigned char	ip[4];
unsigned char *from, *to;

	if(strlen(data) > 15) {
		printf("string length is long\r\n");
		return -1;
	}
	index = 0;
	tokenPtr = strtok( &data[index], "." );
	while ( tokenPtr != NULL )
	{
   		for(col=0; col<4; col++) {
			ip[col] = (unsigned char)strtol(tokenPtr, &endptr, 10);
   			tokenPtr = strtok( NULL, "." );
   			if(tokenPtr == NULL) break;
		}
   		if(col >3) {
   			printf("out\r\n");
   			return -2;
   		}
	}
	if(col < 3) return -3;
	//strcpy(ipNo, ip);
	col += 1;
	if ( col > 0 )
	{
	  	from = (char *)ip;  to = (char *)ipNo;
	  	do *to++ = *from++; while (--col != 0);
	}
		
   	printf("get : %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
   	return 0;
}


/*
 *	IP address setup and new IP address write to EEPROM
 *
   	cfg->version = 1;
  	store_mac(cfg->net_cfg.mac_address, 00, 0xA, 0x35, 0x55, 0x55, 0x55 );
  	store_IP4(cfg->net_cfg.ip_address, 192, 168, 90, 1 );
  	store_IP4(cfg->net_cfg.ip_netmask, 255, 255, 255, 0 );
  	store_IP4(cfg->net_cfg.ip_gateway, 192, 168, 90, 168 );
  
  	cfg->serial = 2010;
  	cfg->fab_date.year = 2010;
  	cfg->fab_date.month = 1;
  	cfg->fab_date.day = 1;
  	cfg->cfg_date.year = 2010;
  	cfg->cfg_date.month = 1;
  	cfg->cfg_date.day = 4;
//  	cfg->notes[0] = '\0';
	strncpy(cfg->notes, "NSLS2 Digital BPM", 17);
	
 */
#define	EEPROM_OFFSET_ADDR	0 
int	cmd_EEPROM_IP_Lookup_Setup(int argc, char *argv[])
{
short	cfg_opt;
char    str[50];    
unsigned char macAddress[7];
int     cnt, len;
int 	i, rv;
int		tmp, memory;
char	c;
unsigned char character;
static char UserResponse[64]; /* User input for questions */

    net_config_t *cfg;	
    len = strlen(argv[1]);
    
    if(len > 7) len = 7;
    for(cnt=0; cnt < len; cnt++) {
    	str[cnt] = argv[1][cnt];
    }
    
	safe_print("\r\nFlash ? ");	
	c = inbyte();
	if( c == 'y') {
		memory = 0;
		xil_printf("%c\r\n", c);
	}
	else {
		memory = 1;	
		xil_printf("%c\r\n", c);
	}
    //memory = strtol(str, (char **)0, 10);
    if(memory == 0) {
	   	rv = FLASH_ReadConfig( FLASH_NETWORK_INFO_0_BASE, &SSP_Config);   	
	  	if ( rv != 0 ) {
		 	safe_print(("\r\n FLASH_ReadConfig Error, Will Write Default ...\r\n"));	 	
	  	}
	  	else {
		  	EE_print_config (&SSP_Config, "------DBPM BOARD IP INFOMATION-----");
	  	} 	    
    }
    else {
	   	rv = EE_ReadConfig( EEPROM_OFFSET_ADDR, &SSP_Config);   	
	  	if ( rv != 0 ) {
		 	safe_print(("\r\n EE_ReadConfig Error, Will Write Default ...\r\n"));	 	
	  	}
	  	else {
		  	EE_print_config (&SSP_Config, "------DBPM BOARD IP INFOMATION-----");
	  	}   
	}
  	  	
 	safe_print("\r\n\r\nDo you want setup IP address, correct? (y/n) : ");	
    // Loop until we get a valid response
    while(1) {
        character = inbyte();
        if((character == 'Y') || (character == 'y')) {
            outbyte(character);
            goto LOC_SETUP;
        }
        else if((character == 'N') || (character == 'n')) {
        	outbyte(character);
        	//EE_print_config(&SSP_Config, "IP INFO");
            safe_print("\r\nBuy!!!...\r\n");
            return FALSE;
        }
    }  
    
LOC_SETUP:
   	//SSP_Config.SystData.Location = 0;
    //SSP_Config.SystData.CellNo   = 1;
    safe_print(("\r\nEnter BPM ID number : ")); 
    while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
    tmp = strtoul(UserResponse, NULL, 10);    
    SSP_Config.SystData.BpmIDs = tmp;
        
    SSP_Config.SystData.bootFlashStartAddress = 0x92000000;	/* BPM application address */    
    safe_print(("\r\nEnter Location [ALS_SR[0], Linac[1], LTB[2], BOOST[3], BTS[4], SR[5] : ")); 
    while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
    tmp = strtoul(UserResponse, NULL, 10);    
    SSP_Config.SystData.Location = tmp;
          
    safe_print(("\r\nEnter Cell number : ")); 
    while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
    tmp = strtoul(UserResponse, NULL, 10);    
    SSP_Config.SystData.CellNo = tmp;
           
IP_SETUP:  
/*
    safe_print(("\r\nEnter Version number: ")); 
    while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
    tmp = strtoul(UserResponse, NULL, 10);
    if(tmp > 0) 
		SSP_Config.version = tmp;
*/
	/* FPGA Version */	
    //SSP_Config.version = (unsigned short) (DFE_CONTROL_REGS->ioReg[FPGA_VERSION] & 0xFFFF);

    safe_print(("\r\nEnter Serial number: ")); 
    while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
    tmp = strtoul(UserResponse, NULL, 10);
    if(tmp > 0)
    	SSP_Config.serial = tmp;
            
   	safe_print(("\r\nEnter Description text: ")); 
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));     	
    if(sizeof(UserResponse) > 2)	/* biger then two char */
		strcpy(SSP_Config.notes, UserResponse);
	else xil_printf("Pass\r\n");
	//Feb
CFG_YEAR:	
    safe_print(("\r\nEnter Configuration Year: ")); 
    while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
    tmp = strtoul(UserResponse, NULL, 10);
    if(tmp > 2000 & tmp< 2015)
    	SSP_Config.cfg_date.year = tmp;	
	else if (tmp == 0) 	xil_printf("Pass\r\n");
    else {
	    xil_printf("*****Input Year error (2010 ~ 2015)\r\n");
	    goto CFG_YEAR;
    }	
    safe_print(("\r\nEnter Configuration Month [1 - 12] : ")); 
    while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
    tmp = strtoul(UserResponse, NULL, 10);
    if(tmp > 0 && tmp <= 12)
    	SSP_Config.cfg_date.month = tmp;
    	
    safe_print(("\r\nEnter Configuration Date [1- 31] : ")); 
    while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
    tmp = strtoul(UserResponse, NULL, 10);
    if(tmp > 0)
    	SSP_Config.cfg_date.day = tmp;
    	   	
IP:			
#if(IP_LOOKUP_TABLE)  	/* for Look-up table */
   	safe_print(("\r\nEnter IP Code [0 - 35] : ")); 	
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	cfg_opt = strtoul(UserResponse, NULL, 10);
	cfg_opt &= 0x1F;
	safe_printf(("\r\nIP Code=%d\r\n", cfg_opt)); 
	
  	if ( cfg_opt < N_CFG_OPTS ) 
  	{
  		cfg = &cfg_opts[cfg_opt];
  		xil_printf("\r\n\r\ncfg_opt=%d\r\n", cfg_opt);
   		for (i = 0; i < 6; i++ )
  			SSP_Config.net_cfg.mac_address[i] = cfg->mac_address[i];
			
  		for ( i = 0; i < 4; i++ ) {
  			SSP_Config.net_cfg.ip_address[i] = cfg->ip_address[i];
  			SSP_Config.net_cfg.ip_netmask[i] = cfg->ip_netmask[i];
  			SSP_Config.net_cfg.ip_gateway[i] = cfg->ip_gateway[i];
  		}			
	}
  	else {
	  	safe_print(("\r\nEEPROM Bad configuration option...Try again \n")); 
		goto IP;
	}
	
#else	/* User setting for ALS */
MAC_SET:
   	printf("\r\nEnter MAC address hex value, example AA:BB:22:33:44:55   -> "); 
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	
	if (sizeof(UserResponse) < 2) { xil_printf("Pass\r\n"); goto IP_SET; }
	else if( 0 != Mac_addr_get(UserResponse, &macAddress[0]) ) {
		printf("\r\n!!!Mac address format error try again \r\n\r\n\r\n");
		goto MAC_SET;
	}
	printf("New Mac Address = %X:%X:%X:%X:%X:%X \r\n", macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
   	for (i = 0; i < 6; i++ ) {
  		SSP_Config.net_cfg.mac_address[i] = macAddress[i];	
	}
IP_SET:		
   	printf("\r\nEnter IP address, example 10.0.142.123 : ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	if( 0 != ip_addr_get(UserResponse, &SSP_Config.net_cfg.ip_address) ) {
		printf("\r\n!!!IP address format error try again \r\n\r\n\r\n");
		goto IP_SET;
	}
GW_SET:   	
	printf("\r\nEnter GateWay address, example 10.0.142.254 : ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	if( 0 != ip_addr_get(UserResponse, &SSP_Config.net_cfg.ip_gateway) ) {
		printf("\r\n!!!GateWay address format error try again \r\n\r\n\r\n");
		goto GW_SET;
	}
MASK_SET:	
   	printf("\r\nEnter net Mask, example 255.255.255.0  : "); 
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	if( 0 != ip_addr_get(UserResponse, &SSP_Config.net_cfg.ip_netmask) ) {
		printf("\r\n!!!MASK number format error try again \r\n\r\n\r\n");
		goto MASK_SET;	
	}
#endif				
	  

  	EE_print_config(&SSP_Config, "---DBPM IP INFO---");

 	safe_print("\r\n!!!!!Do you want WRITE to EEPROM ? (y/n) : ");	
    // Loop until we get a valid response
    while(1) {
        character = inbyte();
        if((character == 'Y') || (character == 'y')) {
            outbyte(character);
            goto EEPROM_WR;
        }
        else if((character == 'N') || (character == 'n')) {
        	outbyte(character);
            safe_print("\r\nBuy!...\r\n");
            return FALSE;
        }
    }   	
  	
EEPROM_WR:    
	if(memory == 0) {
		// Write to FLASH memory
		if( FLASH_WriteConfig( FLASH_NETWORK_INFO_0_BASE, &SSP_Config ) != XST_SUCCESS ) {
			safe_print(("\r\nFLASH Write ERROR ...\r\n"));
		}		

	}
	else {
		if ( EE_WriteConfig( EEPROM_OFFSET_ADDR, &SSP_Config) != 0)  {
			safe_print(("\r\nEE Write ERROR ...\r\n"));
		}
	}
	return TRUE;  		
}


/**/  
int	cmd_ConsolReadEEPROM_IP_Info(int argc, char *argv[])
{
	int rv;
	int	memory;
	char	c;
	
	safe_print("\r\nFlash ? ");	
	c = inbyte();
	if( c == 'y') {
		memory = 0;
		xil_printf("%c\r\n", c);
	}
	else {
		memory = 1;	
		xil_printf("%c\r\n", c);
	}
	//memory = strtol(argv[1], (char **)0, 10);
	
	if(memory == 0) {
		// Write to FLASH memory test
	  	/*
		if( FLASH_WriteConfig( FLASH_NETWORK_INFO_0_BASE, &SSP_Config) != XST_SUCCESS ) {
			safe_print(("\r\nFLASH Write ERROR ...\r\n"));
		}
		*/

#if 0
	  	rv = FLASH_ReadConfig( FLASH_NETWORK_INFO_0_BASE, &FlashSys_Config);
	  	if ( rv != 0 ) {
		 	safe_print(("\r\nFLASH_ReadConfig Error, Will Write Default ...\r\n"));
	  	}
	  	else {
	  		EE_print_config (&FlashSys_Config, "------DBPM BOARD IP INFOMATION-----");
	  	}
#endif

	}
	else {
	  	rv = EE_ReadConfig( 0, &SSP_Config);
	  	if ( rv != 0 ) {
		 	safe_print(("\r\nEE_ReadConfig Error, Will Write Default ...\r\n"));	 	
	  	}
	  	else {
		  	EE_print_config (&SSP_Config, "------DBPM BOARD IP INFOMATION-----");
	  	}	  	
	}
	
	return 0;
}  	



/*
 *	cmds :  ip 0   for view information
 *			ip 1   for setup
 */
int  cmd_Get_EEPROM_ConfigureFromFile(int argc, char *argv[])
{
    char 	c;
    int		row;
	int		cnt=0;
   	char	buf[520];		/* note : April 01,  size 4096, 1024, that time cpu crash, reduce to 1024. */
   	int	i;
    char    str[20];    
	int     len;
	int 	val;
                 				
    len = strlen(argv[1]);
    if(len > 7) len = 7;
    for(cnt=0; cnt < len; cnt++) {
    	str[cnt] = argv[1][cnt];
    }
    str[cnt] = '\0';        
    //len = strlen(str);        
	val = strtol(str, (char **)0, 10);
	
	
	row=0;
	if(val == 0) {
		safe_print("\r\n--------- CONFIG INFO ------------\r\n");			  				   	 
  		EE_print_config(&SSP_Config, "DFE IP INFO");	  		
	}
  	else {  	
		xil_printf ("\r\nIP Reconfiguration waiting file from Host:");
	
        buf[cnt] = '\0';
   		c = '\0';
        cnt = 0;
        //receive data from host computers until # charector found   		
		while (c != '#')
		{
			buf[cnt] =  c = inbyte();			
			cnt++;	
			if( cnt >= 500) break;    	
		}
		/*
		for(i=0; i<cnt; i++) {
			if(buf[i] == '\r') xil_printf("\n");
			xil_printf("%c", buf[i]);
		}
		*/
#if(AA)
		//empty check and clear buffer, not support 12.1
		while ( !XUartLite_mIsReceiveEmpty (UART_ADDRESS)) {
			//XUartLite_RecvByte(UART_ADDRESS);
			c = inbyte();
			//outbyte(c);				
		}
#endif
		xil_printf("\r\nReceived = %d bytes\r\n", cnt);		            	
        row = SystemInformation(buf);

      	//xil_printf("\r\ntotal = %d\r\n", row);     
	} 

      	return row;     	  	
}


/*
 *	EPICS commands test by manual consol mode
 *	
 */
void	cmd_EpicsCmds(int argc, char *argv[])
{
Xuint32 addr, data;

	addr = strtol(argv[1], (char **)0, 10);
	data = strtol(argv[2], (char **)0, 10);
		
	EpicsCmdsInterpreter( addr, data);
}

//
void	cmd_BramSetAddressSel(int argc, char *argv[])
{
Xuint32 addr;

	addr = strtol(argv[1], (char **)0, 10);

	switch(addr) {
		case 0:
		case 6:
		case 7:
			xil_printf("");

	}
	mps_ChipScope_MuxAddr( 1, addr);
}


void	cmd_mps_calc_mode(int argc, char *argv[])
{
Xuint32 addr;

	addr = strtol(argv[1], (char **)0, 10);
	mps_calc_mode( 1, addr);
}


/*
 *
 */
void	cmd_mps_Set_S_values(int argc, char *argv[])
{
	double angle, offset;
	char UserResponse[24];

   	printf("\r\nSet angle [0 - 0.5] :  ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	angle = strtod(UserResponse, (char **)0);
   	printf("\r\nSet offset [0 - 0.5]  :  ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	offset = strtod(UserResponse, (char **)0);

	mps_Set_S_values(1, angle, offset);
	xil_printf("s_i value (0 ~ 1.0)\r\n");
}

// for chip scope reading, it need select bpm position address
void	cmd_mps_BpmPosReadAddress(int argc, char *argv[])
{
	Xuint32 addr, width;

	addr = strtol(argv[1], (char **)0, 10);
	mps_read_single_pos( 1, addr);

	width = strtol(argv[2], (char **)0, 10);
	mps_npi_pos_width(0, width);

}

/*
 * 	assign npi_burst_0_data =  	(plb_w_reg[11][12:10] == 3'd0)  ? xy_pos_read :
								(plb_w_reg[11][12:10] == 3'd0)  ? x_um_filt_o :
								(plb_w_reg[11][12:10] == 3'd1)  ? x_um_o :
								(plb_w_reg[11][12:10] == 3'd2)  ? x_dt_nm_o :
								(plb_w_reg[11][12:10] == 3'd3)  ? mux_Data_o  :
								(plb_w_reg[11][12:10] == 3'd4)  ? 32'd0 :

 */
void	cmd_mps_npi_data_sel(int argc, char *argv[])
{
	Xuint32 addr;
	mps_npi_data_type_sel(0, addr);
}

