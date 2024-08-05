/*
 * sys_eeprom.c
 *
 *  Created on: Apr 23, 2011
 *      Author: kiman
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sys/types.h"
#include "xuartlite_l.h"

//#include "dfe_comm.h"
#include "sys_eeprom.h"
#include "ssp_eeprom.h"
#include "cmd_modbus.h"
#include "data_converter.h"
#include "version.h"


//EepromSysDef_date_t		EepromSysDef;	/* eeprom system parmeters */
#define	XST_SUCCESS	0

#define	D_I32		0
#define	D_F32		1
#define	D_H32		2

#define	EE_NUM_OF_DATA		8
/*
 *	EEPROM data types and descriptions
 */
struct CTRL_CMD_type
{
    int addr;
    int dataType;
    char *sType;
    Xuint32	defaltValue;
};
const struct CTRL_CMD_type tCTRLType[] =
{
    {0, D_I32, "Firmware version                     ",  10  },
	{1, D_I32, "SDI Local  (BPM)Device Address[0-9]  ",  0   },
	{2, D_I32, "SDI Remote (CC) Device Address[1-31] ",  1   },
	{3, D_I32, "SDI Local  (BPM)Packt Length         ",  4   },
	{4, D_I32, "SDI Remote (CC) Packt Length         ",  16  },
	//{5, D_H32, "SDI Local  (BPM)Node Mask)           ",  0x7f },
	{5, D_I32, "Packet Trigger delay settin          ",  160 },
	{6, D_H32, "SDI Remote (CC) Node Mask)           ",  0x3f },
	//{7, D_I32, "Number of the BPM Nodes              ",  6 },
	{7, D_I32, "TFTP SERVER IP",   35088  },	//bootbox01


};

//static volatile unsigned char *ptrEepromParameters = DDR2_EEPROM_PARAMETER_BASE;

/*
 *  EepromSysDef_date_t		EepromSysDef_date;
 *  EEprom_SysDefRead(100, &EepromSysDef_date);
 *
 */
int EEprom_SysDefRead( unsigned eepromAddr, EepromSysDef_date_t *eeCfg )
{

	printf("eeprm read size = %d bytes\n", sizeof(EepromSysDef_date_t));

    if(XST_SUCCESS != EE_Read( eepromAddr, (unsigned char *)eeCfg, sizeof(EepromSysDef_date_t)) ) 
    {
		xil_printf("EERPM read ERROR !!!  %d\r\n", eepromAddr);
		xil_printf("!!! System parameters default value !!! \r\n");
		return -1;
	}	
	else {	/* read eeprom data and copy to RAM */
		printf("\r\nFirmware Version : %.3f\n", eeCfg->FirmwareVersion);	
		EepromDataRead(0);
	}
	
	return XST_SUCCESS;
}



/*
 *
 *	options:
 *		-w  for Write
 *		-r fore read only
 */
int	cmd_ConsoleEEPROM__RW(int argc, char *argv[])
{
unsigned int	addr, ee_addr;
unsigned char	rbuf[16];
unsigned char	wbuf[16];
int	i;
Xuint32 r_Val;
Xuint32 w_Val;
unsigned long u32_dat;
Xint16	eeprom_blk;
Xuint32	dat;
char character;
static char UserResponse[64]; /* User input for questions */

	dfe_EE_Ctrl	*dfe_EE_Ctrl_W;		/* 32bit address range */
	dfe_EE_Ctrl	*dfe_EE_Ctrl_R;

	dfe_EE_Ctrl_W = (dfe_EE_Ctrl *)wbuf;
	dfe_EE_Ctrl_R = (dfe_EE_Ctrl *)rbuf;

	if (argc > 1 && strcmp(argv[1],"-h") == 0) 
	{
		xil_printf("EEPROM Write/Read for system parmeters store to EEPROM\r\n"   \
				"Options:\r\n" \
				"\t-r or -d : Read only\r\n"	\
				"\t-w  : Write\r\n" \
				"\t-default  : default value write \r\n");
		return 0;
	}
	if(strcmp("-r", argv[1]) == 0) goto EE_READ_ONLY;
	else if(strcmp("-d", argv[1]) == 0) goto EE_READ_ONLY;
	else if(strcmp("-default", argv[1]) == 0) goto DEFAULT_SETUP;
    else if(strcmp("-w", argv[1]) == 0) {
		for(i=0; i<EE_NUM_OF_DATA; i++) {
			Xuint32 Val = (Xuint32) (tCTRLType[i].defaltValue);
			if(tCTRLType[i].dataType == D_I32)
	    		printf("%2d ,  %s : %d\r\n", tCTRLType[i].addr,	tCTRLType[i].sType, Val );
	    	else
	    		printf("%2d ,  %s : 0x%X\r\n", tCTRLType[i].addr,	tCTRLType[i].sType, Val );
    	}
	    safe_print(("\r\nEnter address : "));
	    while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	    addr = strtoul(UserResponse, NULL, 10);

	    if(addr > 200 ) return;
	    ee_addr = EEPROM_USER_0_OFFSET_ADDRESS+(addr*4);	/* eeprom address */
	    safe_print(("\r\nEnter data : "));
	    while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	    //if(addr == 5 || addr == 6){
	    if( addr == 6){
	    	u32_dat = strtoul(UserResponse, NULL, 16);	//mask
	    	dfe_EE_Ctrl_W->data = u32_dat;
	    	printf("get : 0x%x, %d, %x ( %x)\r\n", u32_dat, u32_dat, u32_dat,  dfe_EE_Ctrl_W->data);
	    }
	    else {
	    	dat = strtoul(UserResponse, NULL, 10);
	    	dfe_EE_Ctrl_W->data = dat;
	    }

	    safe_print("\r\nEnter 'Y' for Write, 'N' for exit : ");
	    character = inbyte();
	    if((character == 'Y') || (character == 'y')) {
	        outbyte(character);
	        printf("\r\n");
	        
	        Set_uInt32_DDR2 ( DDR2_EEPROM_PARAMETER_BASE+(addr*4), dat );
	        DFE_CONTROL_REGS->sysInfo[addr] = dat;
	        /* EEPROM write */
			if(XST_SUCCESS == EE_Write( ee_addr, wbuf, 4 ) ) {	
				if(tCTRLType[i].dataType)			
					printf("\nWrite OK to addr=%2d , %s , %x\n", tCTRLType[addr].addr, tCTRLType[addr].sType, dfe_EE_Ctrl_W->data);
				else
					printf("\nWrite OK to addr=%2d , %s , %x\n", tCTRLType[addr].addr, tCTRLType[addr].sType, dfe_EE_Ctrl_W->data);
					
				return XST_SUCCESS;

			}
			else {
				xil_printf("EEPROM Write error %d \r\n", addr);
				return -1;
			}
	    }
	    else {
	    	outbyte(character);
	    	safe_print("\r\nBy...!\r\n");
	    	return XST_SUCCESS;
	    }
	    if(XST_SUCCESS != EE_Read( ee_addr, &rbuf, 4) ) {
			xil_printf("EERPM read error  %d\r\n", ee_addr);
			return -1;
		}
	    r_Val = (Xuint32) (dfe_EE_Ctrl_R->data);
	    w_Val = (Xuint32) (dfe_EE_Ctrl_W->data);
		printf("ee addr=%d , W= %X    read = %X\n\n", ee_addr, r_Val, w_Val );
    }
	else {
		xil_printf("-h option for help !!!\r\n");
		return XST_SUCCESS;
	}
DEFAULT_SETUP:
	//Write default data to EEPROM
    safe_print("\r\nEnter 'Y' for Write, 'N' for Default setup : ");
    character = inbyte();
    if((character == 'Y') || (character == 'y')) {
        outbyte(character);
        safe_print("\r\n");

	    safe_print(("\r\nEnter EEPROM BLOCK 0, 1, 2 : "));
	    while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	    eeprom_blk = strtol(UserResponse, (char **)0, 10);
	    if(eeprom_blk > 2) { xil_printf("Address Range error\r\n"); return -1; }
        eeprom_blk *= 200;
		eeprom_blk += EEPROM_USER_0_OFFSET_ADDRESS;
		xil_printf("eeprom block start address = %d\r\n\r\n", eeprom_blk);
		//Write data to EEPROM
		for(i=0; i<EE_NUM_OF_DATA; i++) {
			ee_addr = eeprom_blk + (i*4);	/* address */
			dfe_EE_Ctrl_W->data = tCTRLType[i].defaltValue;	/* data */
			if(XST_SUCCESS != EE_Write( ee_addr, wbuf, 4 ) ) {
				xil_printf("EERPM Write error  %d\r\n", ee_addr);
				return -1;
			}
			DFE_CONTROL_REGS->sysInfo[i] = dfe_EE_Ctrl_W->data;
			Set_uInt32_DDR2(DDR2_EEPROM_PARAMETER_BASE+(i*4), dfe_EE_Ctrl_W->data);
			w_Val = (Xuint32) (dfe_EE_Ctrl_W->data);

			if(tCTRLType[i].dataType == D_F32)
				printf("%2d ,  %s : %d\r\n", tCTRLType[i].addr,	tCTRLType[i].sType, dfe_EE_Ctrl_R->data);
			else if( tCTRLType[i].dataType == D_I32 )
				printf("%2d ,  %s : %d\r\n", tCTRLType[i].addr, tCTRLType[i].sType, w_Val);
			else
				printf("%2d ,  %s : 0x%X\r\n", tCTRLType[i].addr, tCTRLType[i].sType, w_Val);
		}
		xil_printf("\r\n");
		return XST_SUCCESS;
    }
    else {
    	outbyte(character);
    	safe_print("\r\n\r\n");
    	return XST_SUCCESS;
    }
EE_READ_ONLY:
	EepromDataRead(0);
#if 0
	if(strcmp("0", argv[2]) == 0) eeprom_blk = EEPROM_USER_0_OFFSET_ADDRESS;
	else if(strcmp("1", argv[2]) == 0) eeprom_blk = EEPROM_USER_1_OFFSET_ADDRESS;
	else if(strcmp("2", argv[2]) == 0) eeprom_blk = EEPROM_USER_2_OFFSET_ADDRESS;
	else eeprom_blk = EEPROM_USER_0_OFFSET_ADDRESS;

	xil_printf("Current EEPROM System data : EE address = %d \r\n", eeprom_blk);
	xil_printf("****************************************************\r\n");
	for(i=0; i<EE_NUM_OF_DATA; i++) {
		ee_addr = eeprom_blk+(i*4);
	    if(XST_SUCCESS != EE_Read( ee_addr, &rbuf, 4) ) {
			xil_printf("EERPM read error  %d\r\n", ee_addr);
			return -1;
		}
	    Set_uInt32_DDR2(DDR2_EEPROM_PARAMETER_BASE+(i*4), dfe_EE_Ctrl_R->data);
	    r_Val = (Xuint32) (tCTRLType[i].sType, dfe_EE_Ctrl_R->data);

		if(tCTRLType[i].dataType == 1)
			printf("%2d ,  %s : %.3f\r\n", tCTRLType[i].addr,	tCTRLType[i].sType, dfe_EE_Ctrl_R->data);
		else if( tCTRLType[i].dataType == 0 )
			printf("%2d ,  %s : %d\r\n", tCTRLType[i].addr, tCTRLType[i].sType, r_Val);
		else
			printf("%2d ,  %s : 0x%X\r\n", tCTRLType[i].addr, tCTRLType[i].sType, r_Val);

	}
	xil_printf("****************************************************\r\n");
#endif
	
	return XST_SUCCESS;
}


/*
 *
 *
 */
int	EepromDataRead(int block)
{
unsigned int	addr, ee_addr;
unsigned char	rbuf[16];
int	i;
Xint16	eeprom_blk;
dfe_EE_Ctrl	*dfe_EE_Ctrl_R;

	dfe_EE_Ctrl_R = (dfe_EE_Ctrl *)rbuf;
	eeprom_blk = EEPROM_USER_0_OFFSET_ADDRESS;
	
	xil_printf("***********************************************\r\n");
	for(i=0; i<EE_NUM_OF_DATA; i++) {
		ee_addr = eeprom_blk+(i*4);
	    if(XST_SUCCESS != EE_Read( ee_addr, &rbuf, 4) ) {
			xil_printf("EERPM read error  %d\r\n", ee_addr);
			return -1;
		}
		DFE_CONTROL_REGS->sysInfo[i] = dfe_EE_Ctrl_R->data;
		Set_uInt32_DDR2(DDR2_EEPROM_PARAMETER_BASE+(i*4), dfe_EE_Ctrl_R->data);
		//int Val = (int) (tCTRLType[i].sType, dfe_EE_Ctrl_R->data);
		int Val = (int) (dfe_EE_Ctrl_R->data);

		if(tCTRLType[i].dataType == 1)
			printf("%2d ,  %s : %.3f\r\n", tCTRLType[i].addr,	tCTRLType[i].sType, dfe_EE_Ctrl_R->data);
		else if( tCTRLType[i].dataType == 0 )
			printf("%2d ,  %s : %d\r\n", tCTRLType[i].addr, tCTRLType[i].sType, Val);
		else
			printf("%2d ,  %s : 0x%X\r\n", tCTRLType[i].addr, tCTRLType[i].sType, Val);
	}
	xil_printf("***********************************************\r\n");
	
	epics_read_data.Ver             = (Xuint32) (DFE_CONTROL_REGS->sysInfo[0]);
	epics_read_data.LocalSdi_ID     = (Xuint32) (DFE_CONTROL_REGS->sysInfo[1]);
	epics_read_data.LocalSdi_MASK   = (Xuint32) (DFE_CONTROL_REGS->sysInfo[5]);
	epics_read_data.RemoteSdi_ID    = (Xuint32) (DFE_CONTROL_REGS->sysInfo[2]);
	epics_read_data.RemoteSdi_MASK	= (Xuint32) (DFE_CONTROL_REGS->sysInfo[6]);
	//for epics read
	Set_I_DDR2 ( DDR2_EEPROM_SYSDATA_BASE+(0*4), epics_read_data.Ver );		
	Set_I_DDR2 ( DDR2_EEPROM_SYSDATA_BASE+(1*4), epics_read_data.LocalSdi_ID );
	Set_I_DDR2 ( DDR2_EEPROM_SYSDATA_BASE+(2*4), epics_read_data.LocalSdi_MASK );
	Set_I_DDR2 ( DDR2_EEPROM_SYSDATA_BASE+(3*4), epics_read_data.RemoteSdi_ID  );
	Set_I_DDR2 ( DDR2_EEPROM_SYSDATA_BASE+(4*4), epics_read_data.RemoteSdi_MASK );   	
		
	xil_printf( "Ver             = %d\r\n", (int) (DFE_CONTROL_REGS->sysInfo[0]));
	xil_printf( "LocalSdi_ID     = %x\r\n", (int) (DFE_CONTROL_REGS->sysInfo[1]));
	//xil_printf( "LocalSdi_MASK   = %x\r\n", (int) (DFE_CONTROL_REGS->sysInfo[5]));
	xil_printf( "Trigger delay   = %d\r\n", (int) (DFE_CONTROL_REGS->sysInfo[5]));	//Jan/13/14
	xil_printf( "RemoteSdi_ID    = %d\r\n", (int) (DFE_CONTROL_REGS->sysInfo[2]));
	xil_printf( "RemoteSdi_MASK  = %x\r\n", (int) (DFE_CONTROL_REGS->sysInfo[6]));
	
	return 0;	
}
