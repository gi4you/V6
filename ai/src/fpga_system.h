/**
*
*
*	NSLS2 DBPM Project
* 	@file fpga_system.h
*
*	
*
*	Xilinx data type
	typedef unsigned char  Xuint8 
	typedef char  Xint8 
	typedef unsigned short  Xuint16 
	typedef short  Xint16 
	typedef unsigned long  Xuint32 
	typedef long  Xint32 
	typedef float  Xfloat32 
	typedef double  Xfloat64 
	typedef unsigned long  Xboolean 


******************************************************************************/

#ifndef FPGA_SYSTEM_H_	/* prevent circular inclusions */
#define FPGA_SYSTEM_H_	/* by using protection macros */



#ifdef __cplusplus
extern "C" {
#endif

#include "xbasic_types.h"			/* contains basic types for Xilinx software IP */
#include "xparameters.h"

#define THREAD_STACKSIZE  	32768	//8192		/* Same size as BSP */

/* System constants... */

#define	LOCATION				0


#define	n8_BYTE		1
#define	n16_WORD	2
#define	n32_DWORD	4
/*
 *	DDR2
 *	0x2000:0000  -  0x2FFF:FFFF		(256 Mbytes)
 *	 
 *     Hex			Dec
 *	0x01000000 - 16777216 
 *
 *
 */
#define	DDR2_MPMC_BASEADDR		XPAR_MPMC_0_MPMC_BASEADDR				/* DFE bpm board 0xC0000000*/


#define	DDR3_NPI_0					DDR2_MPMC_BASEADDR+0x02000000		//0xC2000000
#define	DDR3_NPI_1					DDR2_MPMC_BASEADDR+0x10000000		//0xD0000000


#define	DDR_SA_OFFSET				0x1000
#define	EEPROM_PARAMETER_OFFSET		0x2000
#define DDR2_CIO_BASE				DDR2_MPMC_BASEADDR + 0x3E000000		/* space 64435 = 64 Kbytes  Modbus Holding register area */
#define	DDR2_SA_BASE				DDR2_MPMC_BASEADDR + 0x3E001000
#define	DDR2_EEPROM_PARAMETER_BASE	DDR2_MPMC_BASEADDR + 0x3E002000		/* EepromSysDef_date_t BPM system parameter */
#define	DDR2_EEPROM_SYSDATA_BASE	DDR2_MPMC_BASEADDR + 0x3E003000		/* eeprom network address */
#define DDR2_INPUT16_BASE			DDR2_MPMC_BASEADDR + 0x3E004000		/* Modbus input registers */
#define	DDR3_EPICS_DATA_BASE		DDR2_MPMC_BASEADDR + 0x3E008000

//FOFB Data 
// Reference, Ut, V ....
#define	DDR3_FOFB_DATA_BASE			DDR2_MPMC_BASEADDR + 0x3E100000

// Parameter's address
#define	BASE_DATA					DDR2_CIO_BASE				/* Size 32767 */
#define	BASE_PAR					DDR2_CIO_BASE + 0xFFFF		/* Size 32767 */






/*--------------------------------------------------------------------------------------*/
/* Macros: DDR2 memory access -> read/write direct from/to a memory address ... 		*/
/*--------------------------------------------------------------------------------------*/
#define Set_I_DDR2(addr, val)  	(* (volatile Xint32 *) 	(addr)) = (val)			/*.. for integer format */
#define Get_I_DDR2(addr)       	(* (volatile Xint32 *) 	(addr))					/*.. for integer format */
#define Set_F_DDR2(addr, val)  	(* (volatile Xfloat32 *) (addr )) = (val)		/*.. for floating point format */
#define Get_F_DDR2(addr)       	(* (volatile Xfloat32 *) (addr))				/*.. for floating point format */

#define Get_Int16(addr)       	(* (volatile Xint16 *) 	(addr))	
#define Get_UCHAR_DDR2(addr)       	(* (volatile Xuint8 *) 	(addr))	


//32-bit Unsigned Interger type
#define Get_uInt16_DDR2(addr)   		(* (volatile Xuint16 *) 	(addr ))				/*.. for integer format */
#define Get_uInt32_DDR2(addr)   		(* (volatile Xuint32 *) 	(addr ))				/*.. for integer format */
#define Set_uInt16_DDR2(addr, val)  	(* (volatile Xuint16 *) 	(addr )) = (val)		/*.. for integer format */
#define Set_uInt32_DDR2(addr, val)  	(* (volatile Xuint32 *) 	(addr )) = (val)		/*.. for integer format */

/* ADC RAW DATA SPACE */
#define Set_Int16_DDR2(addr, val)  		(* (volatile Xint16 *) 	(addr )) = (val)
#define Set_Int16_DDR2_ADC1(addr, val)  (* (volatile Xint16 *) 	(addr + DDR2_ADC1_BASE)) = (val)
#define Get_Int16_DDR2_ADC1(addr)   	(* (volatile Xint16 *) 	(addr + DDR2_ADC1_BASE))	
//
#define Get_uInt16_DDR2_TBT_I(addr)   	(* (volatile Xint16 *) 	(addr + DDR2_ADC1_BASE))
#define Get_uInt16_DDR2_TBT_Q(addr)   	(* (volatile Xint16 *) 	(addr + DDR2_ADC1_BASE))


/////////
//CC SDI
#define Get_uI32_DDR3_CC_SDI_DATA(addr)       	(* (volatile Xuint32 *) (addr + DDR3_NPI_1))
#define Set_uI32_DDR3_CC_SDI_DATA(addr, val) 	(* (volatile Xuint32 *) (addr + DDR3_NPI_1)) = (val)

//BPM SDI C200:0000 - EFFF:FFFF
#define Get_uI32_DDR3_BPM_SDI_DATA(addr)        (* (volatile Xuint32 *) (addr + DDR3_NPI_0))
#define Set_uI32_DDR3_BPM_SDI_DATA(addr, val) 	(* (volatile Xuint32 *) (addr + DDR3_NPI_0)) = (val)



#define Set_uI32_PLB_FIFO(OutputPtr, Value)\
    (*(volatile Xuint32 *)((OutputPtr + 0x00000400 )) = (Value))		/* format unsigned integer */
    
    
/*--- read/write direct from/to CIO memory address ... 		*/
// CONTROL I/O
#define Get_uInt16_DDR2_CIO(addr)   	(* (volatile Xuint16 *) 	(addr + DDR2_CIO_BASE))
#define Set_uInt16_DDR2_CIO(addr, val)  (* (volatile Xuint16 *) 	(addr + DDR2_CIO_BASE)) = (val)
//
#define Get_Int16_DDR2_CIO(addr)   		(* (volatile Xint16 *) 		(addr + DDR2_CIO_BASE))
#define Set_Int16_DDR2_CIO(addr, val)  	(* (volatile Xint16 *) 		(addr + DDR2_CIO_BASE)) = (val)
//
#define Get_Int32_DDR2_CIO(addr)   		(* (volatile Xint32 *) 		(addr + DDR2_CIO_BASE))
#define Set_Int32_DDR2_CIO(addr, val)  	(* (volatile Xint32 *) 		(addr + DDR2_CIO_BASE)) = (val)
//
#define Set_F32_DDR2_CIO(addr, val)  	(* (volatile Xfloat32 *) (addr + DDR2_CIO_BASE)) = (val)	/*.. for floating point format */
#define Get_F32_DDR2_CIO(addr)       	(* (volatile Xfloat32 *) (addr + DDR2_CIO_BASE))			/*.. for floating point format */
//8-bits
#define GetDDR_u8(addr)       	(* (Xuint8 *) 	(addr))			/*.. for 8bit format */
#define SetCIO_u8(addr, val)  	(* (Xuint8 *) 	(addr + BASE_DATA)) = (val)	/*.. for 8bit format */
#define GetCIO_u8(addr)       	(* (Xuint8 *) 	(addr + BASE_DATA))			/*.. for 8bit format */
//16-bits
#define SetCIO_I16(addr, val)  	(* (Xint16 *) 	(addr + BASE_DATA)) = (val)	/*.. for 16bit format */
#define GetCIO_I16(addr)       	(* (Xint16 *) 	(addr + BASE_DATA))			/*.. for 16bit format */
#define SetCIO_uI16(addr, val)  (* (Xuint16 *) 	(addr + BASE_DATA)) = (val)	/*.. for 16bit format */
#define GetCIO_uI16(addr)       (* (Xuint16 *) 	(addr + BASE_DATA))			/*.. for 16bit format */
// 32-bit
#define SetCIO_I32(addr, val)  	(* (Xint32 *) 	(addr + BASE_DATA)) = (val)	/*.. for 32bit format */
#define GetCIO_I32(addr)       	(* (Xint32 *) 	(addr + BASE_DATA))			/*.. for 32bit format */
#define SetCIO_uI32(addr, val)  (* (Xuint32 *) 	(addr + BASE_DATA)) = (val)	/*.. for 32bit format */
#define GetCIO_uI32(addr)       (* (Xuint32 *) 	(addr + BASE_DATA))			/*.. for 32bit format */
// floating format
#define SetCIO_F(addr, val)  	(* (Xfloat32 *) (addr + BASE_DATA)) = (val)	/*.. for floating point format */
#define GetCIO_F(addr)       	(* (Xfloat32 *) (addr + BASE_DATA))			/*.. for floating point format */

/*--- read/write direct from/to parameter sector memory address ... 		*/
#define SetParFlash_I(addr, val)   	(* (Xuint16 *) 	(addr + BASE_PAR)) = (val);	/*.. for integer format */
#define GetParFlash_I(addr)       	(* (Xuint16 *) 	(addr + BASE_PAR))			/*.. for integer format */
#define SetParFlash_F(addr, val)  	(* (Xfloat32 *) (addr + BASE_PAR)) = (val)	/*.. for floating point format */
#define GetParFlash_F(addr)       	(* (Xfloat32 *) (addr + BASE_PAR))			/*.. for floating point format */


/*
 *	PLB R/W
 *
 */
#define Set_I16_PLB(addr, val)  (* (volatile Xint16 *) 	(addr)) = (val)			/*.. for integer format */

#define Set_I32_PLB(addr, val)  (* (volatile Xint32 *) 	(addr)) = (val)			/*.. for integer format */
#define Get_I32_PLB(addr)       (* (volatile Xint32 *) 	(addr))					/*.. for integer format */
#define Set_uI32_PLB(addr, val) (* (volatile Xuint32 *) (addr)) = (val)		/*.. for integer format */
#define Get_uI32_PLB(addr)      (* (volatile Xuint32 *) (addr))				/*.. for integer format */
#define Set_F32_PLB(addr, val)  (* (volatile Xfloat32 *)(addr)) = (val)			/*.. for integer format */
#define Get_F32_PLB(addr)       (* (volatile Xfloat32 *)(addr))					/*.. for integer format */

/* I/O */
#define Get_IoCtrlReg(addr)       	(* (volatile Xint32 *) 	(XPAR_USR_BUS_0_MEM0_BASEADDR+addr))
#define Set_IoCtrlReg(addr, val)  	(* (volatile Xint32 *) 	(XPAR_USR_BUS_0_MEM0_BASEADDR+addr)) = (val)

/*
 *
 */
#define portENTER_CRITICAL()		{														\
										extern unsigned long uxCriticalNesting;	\
										microblaze_disable_interrupts();					\
										uxCriticalNesting++;								\
									}
									
#define portEXIT_CRITICAL()			{														\
										extern unsigned long uxCriticalNesting;	\
										/* Interrupts are disabled, so we can */			\
										/* access the variable directly. */					\
										uxCriticalNesting--;								\
										if( uxCriticalNesting == 0 )			\
										{													\
											/* The nesting has unwound and we 				\
											can enable interrupts again. */					\
											microblaze_enable_interrupts();					\
										}													\
									}
									
#define portNOP()					asm volatile ( "NOP" )


#if 0
/*
 *
 */
#define I2C_BASE 	0xC400

typedef struct {
  volatile Xuint32  prescaler_lo;	// r/w
  volatile Xuint32  prescaler_hi;	// r/w
  volatile Xuint32  ctrl;		// r/w
  volatile Xuint32  data;		// wr = transmit reg; rd = receive reg
  volatile Xuint32  cmd_status;	// wr = command reg;  rd = status reg
} i2c_regs_t;

#define i2c_regs ((i2c_regs_t *) I2C_BASE)
#endif


int	SysMon_Init();

#ifdef __cplusplus
}
#endif

#endif /* end of protection macro */
