/*
 * sys_eeprom.h
 *
 *  Created on: Apr 23, 2011
 *      Author: kiman
 *
 *	Dec 17, 2011
 *		PTC_S21_dB
 */

#ifndef SYS_EEPROM_H_
#define SYS_EEPROM_H_

#include "fpga_system.h"


/* 4 byte * 30 = 120 byte */
#define	EEPROM_USER_0_OFFSET_ADDRESS		400
#define	EEPROM_USER_1_OFFSET_ADDRESS		600
#define	EEPROM_USER_2_OFFSET_ADDRESS		800

/*

 *	Data format for EEPROM
 */
typedef struct dfe_EE_Ctrl_str
{
    //Xfloat32    data;
    Xuint32    data;
} dfe_EE_Ctrl;

typedef struct {
	Xfloat32	FirmwareVersion;
	Xfloat32	Location;
	Xfloat32	RfFrequency;
	Xfloat32	HarmonicNumber;
	Xfloat32	TbtDecimation;
	Xfloat32	Frev;			/* */
	Xfloat32	VcxoFs;			/* Fs = 117 MHz */
	Xfloat32	NcoIF;
	Xfloat32	FaDecimation;
	Xfloat32	SaDecimation;
	Xfloat32	Kx;		/* 10000000 */
	Xfloat32	Ky;
	Xfloat32	SaPositionXoffset;
	Xfloat32	SaPositionYoffset;
	Xfloat32	PositionQoffset;
	Xfloat32	TbtGateDelay;
	Xfloat32	TbtGateWidth;
	Xfloat32	RfInputAtt;
	Xfloat32	PtOutputAtt;
	Xfloat32	ADC_DDR_burstLength;
	Xfloat32	TBT_DDR_burstLength;
	Xfloat32	FA_DDR_burstLength;
	Xfloat32	ADC_Gain[4];
	Xfloat32	PTC_S21_dB[4];
	Xfloat32	DlyBeam;
	Xfloat32	UsrOffsetX;
	Xfloat32	UsrOffsetY;
	Xfloat32	SdiCmds[5];
} __attribute__((packed)) EepromSysDef_date_t;

extern EepromSysDef_date_t		EepromSysDef;
//extern volatile EepromSysDef_date_t  *pEpromSys_Reg = ( EepromSysDef_date_t *) DDR2_EEPROM_SYSDATA_BASE ;


int EEprom_SysDefRead( unsigned eepromAddr, EepromSysDef_date_t *eeCfg );
int	cmd_ConsoleEEPROM__RW(int argc, char *argv[]);
int	EepromDataRead(int block);

#endif /* SYS_EEPROM_H_ */
