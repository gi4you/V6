/*
 *
 *	CHIP : Intel JS28F256
 *
 * 
 *	
 * Feb 25 : 16_bit, 32_bit R/W functions are not worked.
 *			8-bit R/W works.
 *
 *	Flash_32bit_Write(FLASH_CONFIG_PARM_ADDRESS, Size*2);	 word size 
 *	Flash_32bit_read (FLASH_CONFIG_PARM_ADDRESS, Size);		 float size 
 *
 *		
 */
 
#include <stdio.h>
 
#include <xilflash.h>
#include <xbasic_types.h>
#include <pthread.h>
#include "xstatus.h"
#include "xio.h"
#include "xparameters.h"

#include "data_converter.h"
#include "flash_config.h"
#include "fpga_system.h"
#include "ssp_eeprom.h"



#define FLASH_BASE_ADDRESS	XPAR_EMC_0_MEM0_BASEADDR	
#define FLASH_MEM_WIDTH		2


#define	FLASH_INIT			1


static volatile unsigned char *pDdrCTRL;

DDR_CtrlData_t		DDR_CtrlData;

static pthread_mutex_t 	flash_mutex = PTHREAD_MUTEX_INITIALIZER;


static u8 ReadBuffer[FLASH_TEST_SIZE]; /* Buffer used to store the data read. */
static u8 WriteBuffer[FLASH_TEST_SIZE]; /* Write buffer. */

/*
 *
 */
void	flash_mutex_Create()
{
	pthread_mutex_init(&flash_mutex, NULL);
}

/*
 *
 */
void	flash_mutex_Lock()
{
	pthread_mutex_lock(&flash_mutex);	
}

/*
 *
 */
void	flash_mutex_Ulock()
{
	pthread_mutex_unlock(&flash_mutex);
}


/*
 *
 * */
//void Flash_initialize(XFlash *ConfigFlashInstance)
void BPI_Flash_initialize()
{
	u32 StartAddress;
	int Status;
	u32 Index;

DeviceCtrlParam IoctlParams;

	/*
	 * Initialize the Flash Library.
	 */
	//Status = XFlash_Initialize(&ConfigFlashInstance);
	Status = XFlash_Initialize(&ConfigFlashInstance, FLASH_BASE_ADDRESS, 2, 0);
	if(Status != XST_SUCCESS) {
        xil_printf("\n\r-----xflash:Error - XFlash_Initialize-----\n\r");

	    if(Status == XFLASH_PART_NOT_SUPPORTED)
			xil_printf("\n\r-----XFLASH_PART_NOT_SUPPORTED-----\n\r");
	    if(Status == XFLASH_CFI_QUERY_ERROR)
	    	xil_printf("\n\r-----XFLASH_CFI_QUERY_ERROR-----\n\r");

		return XST_FAILURE;
	}

	/*
	 * Reset the Flash Device. This clears the Status registers and puts
	 * the device in Read mode.
	 */
	Status = XFlash_Reset(&ConfigFlashInstance);
	if(Status != XST_SUCCESS) {
	        xil_printf("\n\rxflash:Error - XFlash_Reset\n\r");
		return XST_FAILURE;
	}

	/*
	 * Fetch the flash device properties and display.
	 */
	Status = XFlash_DeviceControl(&ConfigFlashInstance,
				XFL_DEVCTL_GET_PROPERTIES, &IoctlParams);
	if(Status != XST_SUCCESS) {
	        xil_printf("\n\rxflash:Error - XFlash_DeviceControl\n\r");
		return XST_FAILURE;
	}


	return XST_SUCCESS;
}


/*
 *
 */
int	Flash_FloatType_Write( Xuint32  offsetAddress, float *data, int Size32)
{
	int Status;
	u32 Index;
	u16	WriteBuffer[Size32*2];
	u16	ReadBuffer[Size32*2];
	

	Float2Word(&data[0], Size32, &WriteBuffer[0], 1);
	
	flash_mutex_Lock();  
#if(FLASH_INIT)	
	/*
	 * Initialize the Flash Library.
	 */
	//Status = XFlash_Initialize(&ConfigFlashInstance);
	Status = XFlash_Initialize(&ConfigFlashInstance, offsetAddress, 2, 0);
	if(Status != XST_SUCCESS)
	{
		xil_printf("FlashWriteBytes: Failed at initializing the Flash\r\n");
		goto FAIL_ULOCK;
	}
#endif
	
	/*
	 * Reset the Flash Device. This clears the Status registers and puts
	 * the device in Read mode.
	 */		
	Status = XFlash_Reset(&ConfigFlashInstance);
	
	if(Status != XST_SUCCESS) {
		xil_printf("\n\rxflash:Error - XFlash_Reset\n\r");
		goto FAIL_ULOCK;
	}

	/*
	 * Unlock Flash block before the Erase operation.
	 */
	 
	Status = XFlash_Unlock(&ConfigFlashInstance, offsetAddress, Size32*4);
	if(Status != XST_SUCCESS) {
	    xil_printf("\n\rxflash:Error - XFlash_Unlock\n\r");
		goto FAIL_ULOCK;
	}
	Status = XFlash_Unlock(&ConfigFlashInstance, offsetAddress+BLOCK_SIZE, Size32*4);
	if(Status != XST_SUCCESS)
	{
		xil_printf("FlashWriteInc: Unlock error\r\n");
		goto FAIL_ULOCK;
	}
	
	/*
	 * Perform the Erase operation.
	 */
	Status = XFlash_Erase(&ConfigFlashInstance, offsetAddress, Size32*4);
	if(Status != XST_SUCCESS) {
	    xil_printf("\n\rxflash:Error - XFlash_Erase\n\r");
		goto FAIL_ULOCK;
	}

	/* View word WriteBuffer */
/*	
	for(Index = 0; Index < Size32*2; Index++)
	{
		xil_printf("WriteBuffer[%02d] = 0x%04X\r\n", Index, WriteBuffer[Index]);
	}
*/
	/*
	 * Perform the Write operation. Pass in Byte Count to Write. Uses Out16.
	 */
	Status = XFlash_Write(&ConfigFlashInstance, offsetAddress, (Size32*4), WriteBuffer);
	if(Status != XST_SUCCESS) {
	    xil_printf("\n\rxflash:Error - XFlash_Write\n\r");
		goto FAIL_ULOCK;
	}

	/*
	 * Perform the read operation. Pass in Byte Count to Read. Uses In16.
	 */
	Status = XFlash_Read(&ConfigFlashInstance, offsetAddress, (Size32*4), ReadBuffer);
		if(Status != XST_SUCCESS) {
	        flash_mutex_Ulock(); 
			xil_printf("\n\rxflash:Error - XFlash_Read\n\r");
			goto FAIL_ULOCK;
	}
	goto	PASS;

FAIL_ULOCK:		
	flash_mutex_Ulock(); 
	return XST_FAILURE;

PASS:
	flash_mutex_Ulock();			
	/* View word ReadBuffer */
/*
	for(Index = 0; Index < Size32*2; Index++)
	{
		xil_printf("ReadBuffer[%02d] = 0x%04X\r\n", Index, ReadBuffer[Index]);
	}
*/
	for(Index = 0; Index < Size32*2; Index++) {
		if(ReadBuffer[Index] != WriteBuffer[Index]) {
	                xil_printf("\n\rxflash:Error - Write/Read Comparison Mismatch at Index = %d\n\r", Index);
		        xil_printf("\n\r(ReadBuffer[%02d] = %04X, WriteBuffer[%02d] = %04X\r\n", Index, ReadBuffer[Index], Index, WriteBuffer[Index]);
			return XST_FAILURE;
		}

	}
	xil_printf("FLASH R/W SUCCESS!!!\r\n");
	return XST_SUCCESS;			  		
  		
}

/*
 *
 *	Size: flash read size  (Max word data )
 */
int	Flash_FloatType_read (Xuint32  offsetAddress, float *data, int Size)
{
	int Status;
	u32 Index;
	float	cc[5];
	int	i;
	u16	ReadBuffer[Size*4];
		
	/*
	 * Perform the read operation. Pass in Byte Count to Read. Uses In16.
	 */
	flash_mutex_Lock(); 
	
#if(FLASH_INIT)	
	/*
	 * Initialize the Flash Library.
	 */
	//Status = XFlash_Initialize(&ConfigFlashInstance);
	Status = XFlash_Initialize(&ConfigFlashInstance, offsetAddress, 2, 0);
	if(Status != XST_SUCCESS)
	{
		xil_printf("FlashReadBytes: Failed at initializing the Flash\r\n");
		goto READ_FAIL_ULOCK;
	}
#endif	

	/*
	 * Reset the Flash Device. This clears the Status registers and puts
	 * the device in Read mode.
	 */
	Status = XFlash_Reset(&ConfigFlashInstance);
	if(Status != XST_SUCCESS)
	{
		xil_printf("FlashReadBytes: Failed at resetting the Flash\r\n");
		goto READ_FAIL_ULOCK;
	}
		
	Status = XFlash_Read(&ConfigFlashInstance, offsetAddress, (Size *4), ReadBuffer);	
	if(Status != XST_SUCCESS) {
		xil_printf("\n\rxflash:Error - XFlash_Read\n\r");
		goto READ_FAIL_ULOCK;
	}

	goto	READ_PASS;
	
READ_FAIL_ULOCK:		
	flash_mutex_Ulock(); 
	return XST_FAILURE;

READ_PASS:
	flash_mutex_Ulock();
			
	//xil_printf("\r\n---------------------------------------------\r\n\r\n");

	/* View ReadBuffer */
	for(Index = 0; Index < Size*2; Index++)
	{
		//xil_printf("ReadBuffer[%02d] = 0x%04X\r\n", Index, ReadBuffer[Index]);
	}
			
  	Word2Float(&ReadBuffer[0], &data[0], Size, 1);	 
  	/*
    for(i=0; i<Size; i++)	
  		printf("W2float = %f\r\n", data[i]);    	
  	*/	
  	return XST_SUCCESS;
}



/*
 *
 */
int	FLASH_ByteWrite( Xuint32  offsetAddress, unsigned char *data, int Size_byte)
{
	int Status;
	u32 Index;
	u8	WriteBuffer[Size_byte+1];
	u8	ReadBuffer[Size_byte+1];
	
	xil_printf("\r\n");
	for(Index=0; Index < Size_byte; Index++) 
		WriteBuffer[Index] = data[Index];

	flash_mutex_Lock();  
//#if(FLASH_INIT)
	/*
	 * Initialize the Flash Library.
	 */
	Status = XFlash_Initialize(&ConfigFlashInstance, FLASH_BASE_ADDRESS, 2, 0);
	if(Status != XST_SUCCESS)
	{
		xil_printf("FlashWriteBytes: Failed at initializing the Flash\r\n");
		goto FAIL_ULOCK;
	}
//#endif
	/*
	 * Reset the Flash Device. This clears the Status registers and puts
	 * the device in Read mode.
	 */		
	Status = XFlash_Reset(&ConfigFlashInstance);
	
	if(Status != XST_SUCCESS) {
		xil_printf("\n\rxflash:Error - XFlash_Reset\n\r");
		goto FAIL_ULOCK;
	}

#if(AAA)
	/*
	 * Unlock Flash block before the Erase operation.
	 */	 
	Status = XFlash_Unlock(&ConfigFlashInstance, offsetAddress, Size_byte);
	if(Status != XST_SUCCESS) {
	    xil_printf("\n\rxflash:Error - XFlash_Unlock\n\r");
		goto FAIL_ULOCK;
	}
	Status = XFlash_Unlock(&ConfigFlashInstance, offsetAddress+(BLOCK_SIZE/2), Size_byte); /* last 255 block */
	if(Status != XST_SUCCESS)
	{
		xil_printf("FlashWriteInc: Unlock error\r\n");
		goto FAIL_ULOCK;
	}
#endif
	/*
	 * Perform an unlock operation before the erase operation for the Intel
	 * Flash. The erase operation will result in an error if the block is
	 * locked.
	 */
	if ((ConfigFlashInstance.CommandSet == XFL_CMDSET_INTEL_STANDARD) ||
	    (ConfigFlashInstance.CommandSet == XFL_CMDSET_INTEL_EXTENDED)) {
		Status = XFlash_Unlock(&ConfigFlashInstance, offsetAddress, 0);
		if(Status != XST_SUCCESS) {
		    xil_printf("\n\XFlash_Unlock:Error - XFlash_Unlock\n\r");
			goto FAIL_ULOCK;
		}
	}
	
	/*
	 * Perform the Erase operation.
	 */
	Status = XFlash_Erase(&ConfigFlashInstance, offsetAddress, Size_byte);
	if(Status != XST_SUCCESS) {
	    xil_printf("\n\rxflash:Error - XFlash_Erase\n\r");
		goto FAIL_ULOCK;
	}

	/* View word WriteBuffer */	
	for(Index = 0; Index < Size_byte; Index++)
	{
		xil_printf("WriteBuffer[%02d] = 0x%04X\r\n", Index, WriteBuffer[Index]);
	}

	/*
	 * Perform the Write operation. Pass in Byte Count to Write. Uses Out16.
	 */
	Status = XFlash_Write(&ConfigFlashInstance, offsetAddress, Size_byte, WriteBuffer);
	if(Status != XST_SUCCESS) {
	    xil_printf("\n\rxflash:Error - XFlash_Write\n\r");
		goto FAIL_ULOCK;
	}

	/*
	 * Perform the read operation. Pass in Byte Count to Read. Uses In16.
	 */
	Status = XFlash_Read(&ConfigFlashInstance, offsetAddress, Size_byte,
								ReadBuffer);
		if(Status != XST_SUCCESS) {
	        //flash_mutex_Ulock();
			xil_printf("\n\rxflash:Error - XFlash_Read\n\r");
			goto FAIL_ULOCK;
	}
	goto	PASS;

FAIL_ULOCK:		
	flash_mutex_Ulock(); 
	return XST_FAILURE;

PASS:
	flash_mutex_Ulock();			
	/* View word ReadBuffer */

	for(Index = 0; Index < Size_byte; Index++)
	{
		xil_printf("ReadBuffer[%02d] = 0x%04X\r\n", Index, ReadBuffer[Index]);
	}

	for(Index = 0; Index < Size_byte; Index++) {
		if(ReadBuffer[Index] != WriteBuffer[Index]) {
	                xil_printf("\n\rxflash:Error - Write/Read Comparison Mismatch at Index = %d\n\r", Index);
		        xil_printf("\n\r(ReadBuffer[%02d] = %04X, WriteBuffer[%02d] = %04X\r\n", Index, ReadBuffer[Index], Index, WriteBuffer[Index]);
			return XST_FAILURE;
		}

	}
	xil_printf("\r\nFLASH R/W SUCCESS!!!\r\n");
	return XST_SUCCESS;			  		
  		
}


/*
 *
 *	Size: flash read size  (Max word data )
 */
int	FLASH_ByteRead (Xuint32  offsetAddress, unsigned char *data, int Size)
{
	int Status;
	int Index;
	//unsigned char	ReadBuffer[Size+1];
	u8 *pb;
	
	
	pb = (u8*) malloc(sizeof(u8));  
	if (pb == 0)
	{
		printf("ERROR: Out of memory\n");
		return 1;
	}

	
	flash_mutex_Lock(); 
	
#if(FLASH_INIT)		
	/*
	 * Initialize the Flash Library.  ver 3.0
	 */
	//Status = XFlash_Initialize(&ConfigFlashInstance); //ver 2.0
	Status = XFlash_Initialize(&ConfigFlashInstance, FLASH_BASE_ADDRESS, 2, 0);
	if(Status != XST_SUCCESS)
	{
		xil_printf("FlashReadBytes: Failed at initializing the Flash\r\n");
		goto READ_FAIL_ULOCK;
	}

	/*
	 * Reset the Flash Device. This clears the Status registers and puts
	 * the device in Read mode.
	 */
	Status = XFlash_Reset(&ConfigFlashInstance);
	if(Status != XST_SUCCESS)
	{
		xil_printf("FlashReadBytes: Failed at resetting the Flash\r\n");
		goto READ_FAIL_ULOCK;
	}
#endif
	/*
	 * Perform the read operation. Pass in Byte Count to Read. Uses In16.
	 */		
	Status = XFlash_Read(&ConfigFlashInstance, offsetAddress, (Size *2), pb);	
	if(Status != XST_SUCCESS) {
		xil_printf("\n\rxflash:Error - XFlash_Read\n\r");
		goto READ_FAIL_ULOCK;
	}
	
	goto	READ_PASS;

READ_FAIL_ULOCK:		
	flash_mutex_Ulock(); 
	free(pb);
	return XST_FAILURE;

READ_PASS:
	flash_mutex_Ulock();	
	
	//xil_printf("\r\n---------------------------------------------\r\n\r\n");

	/* View ReadBuffer */
	for(Index = 0; Index < Size; Index++)
	{
		data[Index] = pb[Index];
		//xil_printf("FLASH Magic Number[%02d] = 0x%04X, 0x%04X\r\n", Index, ReadBuffer[Index], data[Index] );
	}			 
	 	
	free(pb);
  	return XST_SUCCESS;
}


/*
 *
 *
 */
int	UsrFLASH_Write( Xuint32  offsetAddress, unsigned char *data, int Size_byte)
{
	int Status;
	u32 Index;
	/*
	 * Buffers used during read and write transactions.
	 */
	//u8 ReadBuffer[FLASH_TEST_SIZE]; /* Buffer used to store the data read. */
	//u8 WriteBuffer[FLASH_TEST_SIZE]; /* Write buffer. */

	if(Size_byte > FLASH_TEST_SIZE) {
		xil_printf("size is over %d\r\n", FLASH_TEST_SIZE);
		return -1;
	}
	/*
	 * Initialize the Flash Library.
	 */
	Status = XFlash_Initialize(&ConfigFlashInstance, FLASH_BASE_ADDRESS,
				   FLASH_MEM_WIDTH, 0);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	/*
	 * Reset the Flash Device. This clears the Status registers and puts
	 * the device in Read mode.
	 */
	Status = XFlash_Reset(&ConfigFlashInstance);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform an unlock operation before the erase operation for the Intel
	 * Flash. The erase operation will result in an error if the block is
	 * locked.
	 */
	if ((ConfigFlashInstance.CommandSet == XFL_CMDSET_INTEL_STANDARD) ||
	    (ConfigFlashInstance.CommandSet == XFL_CMDSET_INTEL_EXTENDED)) {
		Status = XFlash_Unlock(&ConfigFlashInstance, offsetAddress+BLOCK_OFFSET_ADDR, 0);
		if(Status != XST_SUCCESS) {
			xil_printf("---XFlash_Unlock error---\r\n");
			return XST_FAILURE;
		}
	}

	/*
	 * Perform the Erase operation.
	 */
	Status = XFlash_Erase(&ConfigFlashInstance, offsetAddress, Size_byte);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Prepare the write buffer. Fill in the data need to be written into
	 * Flash Device.
	 */

	xil_printf("\r\nFlash Addr=%8X , Flash Write = \r\n", offsetAddress);
	for(Index=0; Index < Size_byte; Index++) {
		WriteBuffer[Index] = data[Index];
		xil_printf("%x,",  data[Index]);
	}
	xil_printf("\r\n");
	/*
	 * Perform the Write operation.
	 */
	Status = XFlash_Write(&ConfigFlashInstance, offsetAddress, Size_byte,
								WriteBuffer);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform the read operation.
	 */
	Status = XFlash_Read(&ConfigFlashInstance, offsetAddress, Size_byte,
								ReadBuffer);
		if(Status != XST_SUCCESS) {
			return XST_FAILURE;
	}

	/*
	 * Compare the data read against the data Written.
	 */
	for(Index = 0; Index < Size_byte; Index++) {
		if(ReadBuffer[Index] != WriteBuffer[Index]) {
			return XST_FAILURE;
		}
	}

	xil_printf("Flash R/W OK...\r\n");
	return XST_SUCCESS;
}

/*
 *
 */
int UsrFLASH_Read (unsigned int addr, unsigned char *rbuf, int n_bytes)
{
	unsigned char c;
    int	i;
	int Status;
	u32 Index;

/*
    unsigned char *pFlashBootInfo;

    pFlashBootInfo = (unsigned char *)addr;

    xil_printf("\r\n");
    for(i=0; i<n_bytes; i++) {
        c  = *pFlashBootInfo++;
        rbuf[i] = c;
        xil_printf("%x,", c);
    }
    xil_printf("\r\n");
   */

	/*
	 * Initialize the Flash Library.
	 */
	Status = XFlash_Initialize(&ConfigFlashInstance, FLASH_BASE_ADDRESS,
				   FLASH_MEM_WIDTH, 0);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	/*
	 * Reset the Flash Device. This clears the Status registers and puts
	 * the device in Read mode.
	 */
	Status = XFlash_Reset(&ConfigFlashInstance);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform an unlock operation before the erase operation for the Intel
	 * Flash. The erase operation will result in an error if the block is
	 * locked.
	 */
	if ((ConfigFlashInstance.CommandSet == XFL_CMDSET_INTEL_STANDARD) ||
	    (ConfigFlashInstance.CommandSet == XFL_CMDSET_INTEL_EXTENDED)) {
		Status = XFlash_Unlock(&ConfigFlashInstance, addr+BLOCK_OFFSET_ADDR, 0);
		if(Status != XST_SUCCESS) {
			xil_printf("---XFlash_Unlock error---\r\n");
			return XST_FAILURE;
		}
	}
	/*
	 * Perform the read operation.
	 */
	Status = XFlash_Read(&ConfigFlashInstance, addr, n_bytes, rbuf);
		if(Status != XST_SUCCESS) {
			return XST_FAILURE;
	}
	/*	
	xil_printf("\r\n");
	for(i=0; i<n_bytes; i++) {
		xil_printf("%x,", rbuf[i]);
	}
	xil_printf("\r\n");
	*/

	return XST_SUCCESS;
}

/*
 */
void	cmd_Console_FirmwareUpdate(int argc, char *argv[])
{
int mode;
int	status;
	mode = strtol(argv[1], (char **)0, 10);

	flash_mutex_Lock();
	if(mode == 1){
		status = UsrFLASH_Write(FLASH_BOOTINFO_BASEADDR, "update:", 7);
		if(status == XST_SUCCESS) xil_printf("Flash Update Enabled\r\n");
		else {
			xil_printf("Flash Write error\r\n");
		}
	}
	else if(mode == 2){
		status = UsrFLASH_Write(FLASH_BOOTINFO_BASEADDR, "app...:", 7);
		if(status == XST_SUCCESS) xil_printf("Flash Update Disabled\r\n");
		else {
			xil_printf("Flash Write error\r\n");
		}
	}
	else {
		xil_printf("Flash Update Not configured... use 1 is Update, 2 for Normal firmware\r\n");
	}
	flash_mutex_Ulock();


}


/*
 *
 *
 */
int FLASH_WriteConfig( unsigned int addr, SSP_Config_t *cfg )
{
  	int rv;
  	cfg->hdr.n_bytes = offsetof(SSP_Config_t,notes) + strlen(cfg->notes) + 1;
  	if ( cfg->hdr.n_bytes > 256 ) {
    	return 1;
  	}
  	cfg->hdr.checksum =
    	fletcher16( (unsigned char *)&cfg->version, cfg->hdr.n_bytes - sizeof(cfg->hdr));

  	xil_printf("\r\nFlash write : %d bytes\r\n", cfg->hdr.n_bytes);
  	flash_mutex_Lock();
  	rv = UsrFLASH_Write(addr, (unsigned char *)cfg, cfg->hdr.n_bytes);
  	flash_mutex_Ulock();
  	if ( rv != XST_SUCCESS ) return XST_FAILURE;
  	return XST_SUCCESS;
}


/*
 *	FLASH_SYS_INFO_1_BASE
 *
 */
static volatile unsigned char *pDDRControlData;
 
int FLASH_WriteCtrlData( unsigned int addr, int n_bytes )
{
  	int rv;

  	pDDRControlData = DDR2_CIO_BASE;
  	xil_printf("\r\nFlash write : %d bytes\r\n", n_bytes);
  	
  	flash_mutex_Lock();
  	rv = UsrFLASH_Write(addr, pDDRControlData, n_bytes);
  	flash_mutex_Ulock();
  	if ( rv != XST_SUCCESS ) return XST_FAILURE;
  	return XST_SUCCESS;
}

/*
 * read control parameters data 
 */
int FLASH_ReadCtrlData( unsigned int addr, DDR_CtrlData_t *cfg, int n_bytes )
{
  	int rv;
	int	i;
	
  	pDDRControlData = DDR2_CIO_BASE;
  	xil_printf("\r\nFlash write : %d bytes\r\n", n_bytes);
  	
  	flash_mutex_Lock();
  	rv = UsrFLASH_Read( addr, (unsigned char *)cfg, n_bytes );  	
  	flash_mutex_Ulock();
  	/*
	for(i=0; i<100; i++)
		xil_printf("%d , %d (0x%X) \r\n", addr, cfg->ioReg[i], cfg->ioReg[i]);
	*/	  	
  	if ( rv != XST_SUCCESS ) return XST_FAILURE;
  	return XST_SUCCESS;
}



// return 0 on success, 1 on error
int FLASH_ReadConfig( unsigned int addr, SSP_Config_t *cfg )
{
  	int rv, i;
  	unsigned short check;

  	flash_mutex_Lock();
  	UsrFLASH_Read( addr, (unsigned char *)cfg, sizeof(SSP_Config_hdr_t));
  	flash_mutex_Ulock();
  	if ( cfg->hdr.n_bytes > 256 ||
       	cfg->hdr.n_bytes < offsetof(SSP_Config_t, notes)) {
    	printf("flash read size %d bytes \r\n", cfg->hdr.n_bytes);
    	return 2;
  	}
  	flash_mutex_Lock();
  	UsrFLASH_Read( addr, (unsigned char *)cfg, cfg->hdr.n_bytes );
  	flash_mutex_Ulock();

  	check = fletcher16( (unsigned char *)&cfg->version, cfg->hdr.n_bytes - sizeof(cfg->hdr));
  	if ( check != cfg->hdr.checksum ) {
    	printf("flash read checksum error. \r\n");
    	return 4;
  	}
	//memcpy( ptrDDR3_EEPROM, (unsigned char *)cfg, cfg->hdr.n_bytes);
	/*
	for(i=0; i<cfg->hdr.n_bytes; i++)
		xil_printf("%c", Get_UCHAR_DDR2(DDR2_EEPROM_SYSDATA_BASE+i) );
	xil_printf("***********END Display EEPROM CONFIG\r\n");
	*/
  	return 0;
}


/*
 *
 */
void	aa(unsigned int FlashAddr )
{
	int	status;


	flash_mutex_Lock();
	pDdrCTRL = (volatile unsigned char *) ( DDR2_SA_BASE );
	status = UsrFLASH_Write(FlashAddr, pDdrCTRL, 120*4);
	if(status == XST_SUCCESS) xil_printf("Flash Update Enabled\r\n");
	flash_mutex_Ulock();

}


/*
 *
 */
void	cmdConsoleCmdRegDump (int argc, char *argv[])
{
	int	i, addr, rv, base, flash_base;
	static char UserResponse[64]; /* User input for questions */
	
	if(strcmp("-h", argv[1]) == 0) {
		printf("Syntax:\n");
     	printf("\tdump [options]... [string]...\n");	
     	printf("\tdump -h for help\n");	
     	printf("\tdump -w for flash write\n");	
     	printf("\tdump -d startAddr , for read\n");	
     	goto __Q;
	}
	
	if(strcmp("-w", argv[1]) == 0) {
		printf("Write to FLASH [0, 1, 2, 2, 3, 4, 5]: ");
		while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
		base = strtoul(UserResponse, NULL, 10);
     	goto __FLASH;
	}
	if(strcmp("-r", argv[1]) == 0)
	{
		printf("Read FLASH [0, 1, 2, 3, 4, 5]: ");
		while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
		base = strtoul(UserResponse, NULL, 10);

		switch(base) {
			case 0:
				flash_base = FLASH_SYS_INFO_1_BASE;
				break;
			case 1:
				flash_base = FLASH_SYS_INFO_2_BASE;
				break;
			case 2:
				flash_base = FLASH_SYS_INFO_3_BASE;
				break;
			case 3:
				flash_base = FLASH_SYS_INFO_4_BASE;
				break;
			case 4:
				flash_base = FLASH_SYS_INFO_5_BASE;
				break;
			case 5:
				flash_base = FLASH_SYS_INFO_6_BASE;
				break;				
			default :
				goto __Q;
		}
		
		rv = FLASH_ReadCtrlData( flash_base, &DDR_CtrlData, 128*4 );	
		for(i=0; i<128; i++)
			xil_printf("%d , %d (0x%X) \r\n", i, DDR_CtrlData.ioReg[i], DDR_CtrlData.ioReg[i]);		
     	goto __Q;
	}	
		
	if(strcmp("-d", argv[1]) == 0) {
		addr = strtol(argv[1], (char **)0, 10);	/* channel number */
		if(addr > 127) {
			addr = 127;
			printf("Address range 0 - 128 \r\n");
		}
		for(i=addr; addr<128; addr++)
			xil_printf("%03d(0x%02x) , %d (0x%X) \r\n", addr, addr, DFE_CONTROL_REGS->ioReg[addr], DFE_CONTROL_REGS->ioReg[addr] );		/* same structure with slow */
		goto __Q;
	}
	printf("\tdump [ -h option for help]\n");
	goto __Q;
		
__FLASH:	
		switch(base) {
			case 0:
				flash_base = FLASH_SYS_INFO_1_BASE;
				break;
			case 1:
				flash_base = FLASH_SYS_INFO_2_BASE;
				break;
			case 2:
				flash_base = FLASH_SYS_INFO_3_BASE;
				break;
			case 3:
				flash_base = FLASH_SYS_INFO_4_BASE;
				break;
			case 4:
				flash_base = FLASH_SYS_INFO_5_BASE;
				break;
			case 5:
				flash_base = FLASH_SYS_INFO_6_BASE;
				break;				
			default :
				goto __Q;
		}
		
	FLASH_WriteCtrlData(flash_base, 128*4 );		
	rv = FLASH_ReadCtrlData( flash_base, &DDR_CtrlData, 128*4 );	
	for(i=0; i<128; i++)
		xil_printf("%d , %d (0x%X) \r\n", i, DDR_CtrlData.ioReg[i], DDR_CtrlData.ioReg[i]);
__Q:
	return;
}
