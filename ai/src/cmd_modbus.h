#ifndef CMD_MODBUS_H_
#define CMD_MODBUS_H_

#include "types.h"
#include "fpga_system.h"



 
int 	cmd_Void( int chn, int data);
int 	cmd_Copy( int chn, int data);


/* control registers for DDR memory */
typedef struct _DDR2_CONTROLs {
	DDR2_UI32BIT_REG	ioReg[250];			/* control Regs : 4 byte * 20 signals = 80 */
	//DDR2_FLOAT32_REG	sysInfo[64];	
	
	DDR2_UI32BIT_REG	sysInfo[64];
} DDR2_CONTROLs, *DFE_CONTROL_REGS;

//used flash
typedef struct {
	DDR2_UI32BIT_REG	ioReg[250];
} __attribute__((packed)) DDR_CtrlData_t;

extern DDR_CtrlData_t		DDR_CtrlData;


/* DDR-2 base address is Modbus base address *2 times */
/* 0x0E000000 + OFFSET */
#define DFE_CONTROL_REGS  	( DDR2_MEM_CAST(DFE_CONTROL_REGS) 	(DDR2_CIO_BASE + 0) ) 	


typedef struct{
	Xuint32	TS_SECOND, TS_OFFSET;			/* 64-bit time stamp */		
	Xuint32 EVR_TRIG_CNT;
	char	bi[32];	
	Xuint32 Ver;	
	Xuint32	LocalSdi_ID;
	Xuint32	LocalSdi_MASK;
	Xuint32	RemoteSdi_ID;
	Xuint32	RemoteSdi_MASK;	
} epics_read_data_type;
epics_read_data_type epics_read_data;



/*
 *
 */
void	cmd_ModbusCtrlRegs(int argc, char *argv[]);


/*
1) DDR-2 Address select 0x0100:0000
000024-Tx:00 00 00 00 00 09 01 10 00 34 00 01 02 01 00 
000025-Rx:00 00 00 00 00 06 01 10 00 34 00 01 

2) DDR-2 Address select 0x0000:0000
000030-Tx:00 00 00 00 00 09 01 10 00 34 00 01 02 00 00 
000031-Rx:00 00 00 00 00 06 01 10 00 34 00 01 

3) External Trigger
000036-Tx:00 00 00 00 00 08 01 0F 00 00 00 01 01 01 
000037-Rx:00 00 00 00 00 06 01 0F 00 00 00 01 

4) Internal Trigger
000040-Tx:00 00 00 00 00 08 01 0F 00 00 00 01 01 00 
000041-Rx:00 00 00 00 00 06 01 0F 00 00 00 01 

5) Soft Trigger
000048-Tx:00 00 00 00 00 08 01 0F 00 01 00 01 01 01 
000049-Rx:00 00 00 00 00 06 01 0F 00 01 00 01

6) Data reading 100 counts (200 bytes)
000070-Tx:00 00 00 00 00 06 01 04 00 00 00 64 
000071-Rx:08 68 00 00 00 CB 01 04 C8 00 03 00 03 00 03 00 03 00 04 00 04 00 04 00 04 00 05 00 05 00 05 00 05 00 06 00 06 00 06 00 06 00 07 00 07 00 07 00 07 00 08 00 08 00 08 00 08 00 09 00 09 00 09 00 09 00 0A 00 0A 00 0A 00 0A 00 0B 00 0B 00 0B 00 0B 00 0C 00 0C 00 0C 00 0C 00 0D 00 0D 00 0D 00 0D 00 0E 00 0E 00 0E 00 0E 00 0F 00 0F 00 0F 00 0F 00 10 00 10 00 10 00 10 00 11 00 11 00 11 00 11 00 12 00 12 00 12 00 12 00 13 00 13 00 13 00 13 00 14 00 14 00 14 00 14 00 15 00 15 00 15 00 15 00 16 00 16 00 16 00 16 00 17 00 17 00 17 00 17 00 18 00 18 00 18 00 18 00 19 00 19 00 19 00 19 00 1A 00 1A 00 1A 00 1A 00 1B 00 1B 00 1B 00 1B 

6) Data reading 512 counts (1024 bytes)
000070-Tx:00 00 00 00 00 06 01 04 00 00 02 00 

*/
#endif /*CMD_MODBUS_H_*/
