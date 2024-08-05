/*
 *
 *	Kiman Ha	kha@bnl.gov
 *
 */

#ifndef FLASH_CONFIGURE_H_	/* prevent circular inclusions */
#define FLASH_CONFIGURE_H_	/* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

#include <xilflash.h>
#include "xparameters.h"
#include "types.h"

#include "ssp_eeprom.h"
#include "cmd_modbus.h"


#define	FLASH_USER					0x07FFC000
/* Network Infomation */
#define	FLASH_NETWORK_INFO_0_BASE	0x07FFD000
#define	FLASH_NETWORK_INFO_1_BASE	0x07FFE000
/* Control Register backup */
#define	FLASH_SYS_INFO_0_BASE		0x07FF0000
#define	FLASH_SYS_INFO_1_BASE		0x07FF1000
#define	FLASH_SYS_INFO_2_BASE		0x07FF2000
#define	FLASH_SYS_INFO_3_BASE		0x07FF3000
#define	FLASH_SYS_INFO_4_BASE		0x07FF4000
#define	FLASH_SYS_INFO_5_BASE		0x07FF5000
#define	FLASH_SYS_INFO_6_BASE		0x07FF6000
#define	FLASH_SYS_INFO_7_BASE		0x07FF7000
#define	FLASH_SYS_INFO_8_BASE		0x07FF8000
#define	FLASH_SYS_INFO_9_BASE		0x07FF9000

/*
 *	Flash full range address 0 - 0x01FF:FFFF
 */
/* Flash Device Dependent Values */
#define	FLASH_32MB				256
#define	FLASH_128MB				1024	//DFE-2
#define NUM_BLOCKS				FLASH_128MB
#define BLOCK_SIZE 				0x20000		// 131,072 bytes
#define MAX_OFFSET				NUM_BLOCKS*BLOCK_SIZE-1
 

#define FLASH_TEST_SIZE				1024	/* Max 1024 bytes */
#define FLASH_BOOTINFO_BASEADDR		0x03E00000
#define BLOCK_OFFSET_ADDR			0x1000

XFlash ConfigFlashInstance; 	/* XFlash Instance. */
extern SSP_Config_t 		FlashSys_Config;
extern DDR_CtrlData_t		DDR_CtrlData;

void	flash_mutex_Create();
void	flash_mutex_Lock();
void	flash_mutex_Ulock();

void 	BPI_Flash_initialize();
int		Flash_FloatType_read (Xuint32  offsetAddress, float *data, int Size);
int		Flash_FloatType_Write( Xuint32  offsetAddress, float *data, int Size32);
int		FLASH_ByteWrite( Xuint32  offsetAddress, unsigned char *data, int Size_byte);
int		FLASH_ByteRead (Xuint32  offsetAddress, unsigned char *data, int Size);

int		UsrFLASH_Write( Xuint32  offsetAddress, unsigned char *data, int Size_byte);
int 	UsrFLASH_Read ( unsigned int addr, unsigned char *rbuf, int n_bytes);
int 	FLASH_WriteConfig( unsigned int addr, SSP_Config_t *cfg );
int 	FLASH_ReadConfig( unsigned int addr, SSP_Config_t *cfg );
int 	FLASH_WriteCtrlData( unsigned int addr, int n_bytes );
int 	FLASH_ReadCtrlData( unsigned int addr, DDR_CtrlData_t *cfg, int n_bytes );

void	cmdConsoleCmdRegDump (int argc, char *argv[]);
#ifdef __cplusplus
}
#endif

#endif /* end of protection macro */
