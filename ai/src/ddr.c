/*
 * ddr.c
 *
 *  Created on: Jan 6, 2011
 *      Author: kha
 *
 *	8/16/13
 *		cmd_DDR_CC_WFM_DATAs()  fixed buffer size for 26 signals
 *
 *
 */

#include <stdlib.h>		/* strtod, strtof, strtold  */
#include <math.h>

#include "xio.h"
#include "fpga_system.h"
#include "types.h"
//#include "sdi.h"
#include "sdi_cc.h"
#include "fofb.h"



#define	LOCAL_SDI_SIG_NUM		26		/* x, y */

#define	NPI0_PM_SIGNAL_N		16


/*
 * C000:0000 - uBlaze Code
 * C200:0000 - eigen, err history data
 * D000:0000 - bpm data
 *
 */

#define	XST_SUCCESS		0
 
#define	FA_READ_MAX			400000000*8
#define	FA_MAX_OFFSET		0x0A000000


extern TimerInst;
extern StartIF_TS;
extern FinishIF_TS;
extern TimeSA;


/*
 *
 *
 */
int cmd_DDR_CoeffDataRead(int argc, char *argv[])
{
int type, offset, addr, length;
int i;
int Data;
unsigned int base;
	
	type   = strtol(argv[1], (char **)0, 10);
	addr   = strtol(argv[2], (char **)0, 10);
	length = strtol(argv[3], (char **)0, 10);
	if(length < 10) length = 90;
	
	switch (type) {
		case 0:					
			base = BASE_REF_ORBIT;			
			offset = addr *4;
			for(i=0; i<length; i++) {
				Data = Get_uInt32_DDR2(base+offset+i*4);
				xil_printf("%d = %d\r\n", i, Data);
			}			
			break;
		case 1:
			base = BASE_UT_X;
			for(i=0; i<length; i++) {
				Data = pUT_X->data[i];
				xil_printf("%d = %d\r\n", i, Data);
			}			
			break;	
		case 2:
			base = BASE_UT_Y;
			for(i=0; i<length; i++) {
				Data = pUT_Y->data[i];
				xil_printf("%d = %d\r\n", i, Data);
			}				
			break;	
		case 3:
			base = BASE_V_X;
			offset = addr *4;
			for(i=0; i<length; i++) {
				Data = Get_uInt32_DDR2(base+offset+i*4);
				xil_printf("%d = %d\r\n", i, Data);
			}			
			break;	
		case 4:
			base = BASE_V_Y;
			offset = addr *4;
			for(i=0; i<length; i++) {
				Data = Get_uInt32_DDR2(base+offset+i*4);
				xil_printf("%d = %d\r\n", i, Data);
			}			
			break;			
										
		case 6:
			base = BASE_UT_Y;
			for(i=0; i<length; i++) {
				Data = FOFB_DATA->V_Kp[i];
				xil_printf("%d = %d\r\n", i, Data);
			}			
			break;	
		case 7:
			base = BASE_UT_Y;
			for(i=0; i<length; i++) {
				Data = FOFB_DATA->V_Ki[i];
				xil_printf("%d = %d\r\n", i, Data);
			}			
			break;	
						
	}
	xil_printf("Read DDR address 0x%x \r\n", base);
	
	xil_printf("Usage: 'rc 1 0 1000' for Ut-X start 0 length 1000\r\n");
	xil_printf("Options -> 0:Ref orbit, 1:Ut-X, 2:Ut-Y, 3:V-X, 4:V-Y\r\n");
	return 0;
}


/*
 *  	START Address : 0xD0000000 --- CC SDI
 *
 *		Cell 1:  0 - 103 (26x4)-1
 *		Cell 2:
 *
 *
 */
int cmd_DDR_CellController_DATAs(int argc, char *argv[])
{
	char    str[10];
	int     len;
	int	type;
	int Status;
	int ValidInput = 0;
	u32 Index;
	u32 ddr2_addressOffset = 0;
	u32 NumBytes = 0;
	u32 MaxReadBytes = 0;
    int Index_mod8;
    u32	ddr_addr;
    static char UserResponse[64]; /* User input for questions */
	int	abcd, length;
	Xint32	adc, adc_old, tbtD[32], tbtD_BigE[32], a, b;
	//
	double	Xpos, Ypos;
	double mb_sum;
	char	pos;

	adc_old = 0;

	/*argument 0 is ascii, 1: hex */
    len = strlen(argv[1]);
    strncpy(str, argv[1], len);
	type = strtol(str, (char **)0, 10);

	//Dfe_SoftTrigSet();
	/*
	 * Ask user options.
	 */
	while(ValidInput != 1)
	{
		xil_printf("\r\nEnter FA offset address (0-%X): ", FA_MAX_OFFSET );	//0x00100000 * 4

		while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
		ddr2_addressOffset = strtoul(UserResponse, NULL, 16); 	// * ADC_RAW_DATA_SIZE;
		//ddr2_addressOffset *= 56;	/* 4 byte * 14 signals = 32 */

		ddr2_addressOffset *= (LOCAL_SDI_SIG_NUM *4);	//104
		if(ddr2_addressOffset >= 0 && ddr2_addressOffset <= FA_MAX_OFFSET )
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
		if(FA_MAX_OFFSET-ddr2_addressOffset+1 < FA_READ_MAX)
		{
			MaxReadBytes = FA_MAX_OFFSET-ddr2_addressOffset+1;
		}
		else
		{
			MaxReadBytes = FA_READ_MAX;
		}
		xil_printf("Enter number of word to read (0-%d): ", MaxReadBytes);
		while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
		NumBytes = strtoul(UserResponse, NULL, 10);
		//NumBytes *= 56;			/* 14 * 4 */
		if (NumBytes > MaxReadBytes)
		{
			xil_printf("%d is higher than %d, the maximum number of bytes allowed\r\n", NumBytes, MaxReadBytes);
		}
		else if (NumBytes == 0)
		{
			return XST_SUCCESS;
		}
		else
		{
			ValidInput = 2;
		}
	}

	xil_printf("Enter BPM Number 0,1,2,3,4,5,6,7,8,9,10,11,12,13,  15 for all signals display ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	pos = strtoul(UserResponse, NULL, 10);

	/*
	 * Print out the data
	 */
	abcd=0;
	length=0;

	//local
	if(pos >=0 && pos < 360) {

		for(Index = 0; Index < NumBytes; Index++)
		{
			ddr_addr = (pos*4) + (Index*4) * (14 * MAX_CELL_NUMBER);
			b = Get_uI32_DDR3_CC_SDI_DATA( ddr_addr  );
			xil_printf("%d\r\n",b);
			//xil_printf("%6d : %8d : %d\r\n", Index, ddr_addr, b);
		}
	}
	else {
		for(Index = 0; Index < NumBytes; Index++)
		{
			{
				//Index_mod8 = Index % NUM_FA_SIG;
				Index_mod8 = Index % 14;
				if(Index != 0 && Index_mod8 == 0)
				{
					for(a=0; a<26; a++) {	//MAX_LOCAL_BPM_NODE*2
						tbtD_BigE[a] = tbtD[a];
					}

					xil_printf("%8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d\r\n", \
							tbtD_BigE[0], tbtD_BigE[1], tbtD_BigE[2], tbtD_BigE[3], \
							tbtD_BigE[4], tbtD_BigE[5], tbtD_BigE[6], tbtD_BigE[7], \
							tbtD_BigE[8], tbtD_BigE[9], tbtD_BigE[10], tbtD_BigE[11], \
							tbtD_BigE[12], tbtD_BigE[13]);

					//clear and increment
					abcd=0;
					length++;
				}

				tbtD[abcd] = Get_uI32_DDR3_CC_SDI_DATA( ddr2_addressOffset+(Index*4) );

				abcd++;
			}

		}
	}

	//free(pInt16_rBuffer);
	xil_printf("\r\n");
	return XST_SUCCESS;
}




/*
 * START Address : 200:0000 -- BPM
 *
 * Endian conversion by FPGA
 *
 */
int cmd_DDR_FA_WFM_DATAs(int argc, char *argv[])
{
	char    str[10];
	int     len;
	int	type;
	int Status;
	int ValidInput = 0;
	u32 Index;
	u32 ddr2_addressOffset = 0;
	u32 NumBytes = 0;
	u32 MaxReadBytes = 0;
    int Index_mod8;
    u32	ddr_addr;
    static char UserResponse[64]; /* User input for questions */
	int	abcd, length;
	Xint32	adc, adc_old, tbtD[32], tbtD_BigE[32], a, b;
	//
	double	Xpos, Ypos;
	double mb_sum;
	char	pos;

	adc_old = 0;

	/*argument 0 is ascii, 1: hex */
    len = strlen(argv[1]);
    strncpy(str, argv[1], len);
	type = strtol(str, (char **)0, 10);

	while(ValidInput != 1)
	{
		xil_printf("\r\nEnter BPM FA offset address (0-%X): ", FA_MAX_OFFSET );	//0x00100000 * 4

		while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
		ddr2_addressOffset = strtoul(UserResponse, NULL, 16); 	// * ADC_RAW_DATA_SIZE;
		//ddr2_addressOffset *= 4;	/* 4 byte * 14 signals = 32 */
		if(ddr2_addressOffset >= 0 && ddr2_addressOffset <= FA_MAX_OFFSET )
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
		if(FA_MAX_OFFSET-ddr2_addressOffset+1 < FA_READ_MAX)
		{
			MaxReadBytes = FA_MAX_OFFSET-ddr2_addressOffset+1;
		}
		else
		{
			MaxReadBytes = FA_READ_MAX;
		}
		xil_printf("Enter number of word to read (0-%d): ", MaxReadBytes);
		while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
		NumBytes = strtoul(UserResponse, NULL, 10);
		//NumBytes *= 14;			/* read buffer = 4 channel * words */
		if (NumBytes > MaxReadBytes)
		{
			xil_printf("%d is higher than %d, the maximum number of bytes allowed\r\n", NumBytes, MaxReadBytes);
		}
		else if (NumBytes == 0)
		{
			return XST_SUCCESS;
		}
		else
		{
			ValidInput = 2;
		}
	}


	//Test
	for(Index = 0; Index < NumBytes; Index++)
	{
		ddr_addr = Index*4;
		b = Get_uI32_DDR3_BPM_SDI_DATA( ddr_addr  );
		//xil_printf("%d : %d\r\n", ddr_addr, b);
		xil_printf("%d : %d\r\n", Index, b);
	}

	return;


	xil_printf("Enter BPM Number 0,1,2,3,4,5,6,7,8,9,10,11,12,13,  26 for all signals display ");
	while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	pos = strtoul(UserResponse, NULL, 10); 


/*
	for(Index = 0; Index < 255; Index++)
	{
		ddr_addr = Index*4;
		b = Get_uI32_DDR3_BPM_SDI_DATA( ddr_addr  );
		xil_printf("%d : %d\r\n", ddr_addr, b);
	}
*/

	
	/*
	 * Print out the data
	 */
	abcd=0;
	length=0;
	
	if(pos >=0 && pos < NPI0_PM_SIGNAL_N) {
		NumBytes /= NPI0_PM_SIGNAL_N;
		for(Index = 0; Index < NumBytes; Index++)
		{
			ddr_addr = (pos*4) + (Index*4) * NPI0_PM_SIGNAL_N;
			b = Get_uI32_DDR3_BPM_SDI_DATA( ddr_addr  );
			xil_printf("%d\r\n",b);
		}
	}
	else {
		for(Index = 0; Index < NumBytes; Index++)
		{		
			{
				Index_mod8 = Index % NPI0_PM_SIGNAL_N;
				if(Index != 0 && Index_mod8 == 0)
				{
					for(a=0; a<NPI0_PM_SIGNAL_N; a++) {
						tbtD_BigE[a] = tbtD[a];
					}

					xil_printf("%8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d\r\n", \
							tbtD_BigE[0], tbtD_BigE[1], tbtD_BigE[2], tbtD_BigE[3], \
							tbtD_BigE[4], tbtD_BigE[5], tbtD_BigE[6], tbtD_BigE[7], \
							tbtD_BigE[8], tbtD_BigE[9], tbtD_BigE[10], tbtD_BigE[11], \
							tbtD_BigE[12], tbtD_BigE[13]);

					//clear and increment
					abcd=0;
					length++;
				}
				
				tbtD[abcd] = Get_uI32_DDR3_BPM_SDI_DATA( ddr2_addressOffset+(Index*4) );
				abcd++;
			}

		}
	}

	xil_printf("\r\n");
	return XST_SUCCESS;
}


/*
 *
 */
int cmd_DDR_CC_WFM_DATAs(int argc, char *argv[])
{
	char    str[10];
	int     len;
	int	type;
	int Status;
	int ValidInput = 0;
	u32 Index;
	u32 ddr2_addressOffset = 0;
	u32 NumBytes = 0;
	u32 MaxReadBytes = 0;
    int Index_mod8;
    u32	ddr_addr;
    static char UserResponse[64]; /* User input for questions */
	int	abcd, length;
	Xint32	adc, adc_old, tbtD[32], tbtD_BigE[32], a, b;
	//
	double	Xpos, Ypos;
	double mb_sum;
	char	pos;

	adc_old = 0;

	/*argument 0 is ascii, 1: hex */
    len = strlen(argv[1]);
    strncpy(str, argv[1], len);
	type = strtol(str, (char **)0, 10);

	while(ValidInput != 1)
	{
		xil_printf("\r\nEnter CC (0xD000:0000) FA offset address (0-%X): ", FA_MAX_OFFSET );	//0x00100000 * 4

		while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
		ddr2_addressOffset = strtoul(UserResponse, NULL, 16); 	// * ADC_RAW_DATA_SIZE;
		//ddr2_addressOffset *= 4;	/* 4 byte * 14 signals = 32 */
		if(ddr2_addressOffset >= 0 && ddr2_addressOffset <= FA_MAX_OFFSET )
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
		if(FA_MAX_OFFSET-ddr2_addressOffset+1 < FA_READ_MAX)
		{
			MaxReadBytes = FA_MAX_OFFSET-ddr2_addressOffset+1;
		}
		else
		{
			MaxReadBytes = FA_READ_MAX;
		}
		xil_printf("Enter number of word to read (0-%d): ", MaxReadBytes);
		while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
		NumBytes = strtoul(UserResponse, NULL, 10);
		//NumBytes *= 14;			/* read buffer = 4 channel * words */
		if (NumBytes > MaxReadBytes)
		{
			xil_printf("%d is higher than %d, the maximum number of bytes allowed\r\n", NumBytes, MaxReadBytes);
		}
		else if (NumBytes == 0)
		{
			return XST_SUCCESS;
		}
		else
		{
			ValidInput = 2;
		}
	}


	//Test
	for(Index = 0; Index < NumBytes; Index++)
	{
		ddr_addr = Index*4;
		b = Get_uI32_DDR3_CC_SDI_DATA( ddr_addr  );
		//xil_printf("%d : %d\r\n", ddr_addr, b);
		xil_printf("%d : %d\r\n", Index, b);
	}

	return;


	//xil_printf("Enter BPM Number 0,1,2,3,4,5,6,7,8,9,10,11,12,13,  [26] for all signals display ");
	//while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
	//pos = strtoul(UserResponse, NULL, 10);
	
	/*
	 * Print out the data
	 */
	abcd=0;
	length=0;
	
	/*
	if(pos >=0 && pos < LOCAL_SDI_SIG_NUM) {
		NumBytes /= LOCAL_SDI_SIG_NUM;
		for(Index = 0; Index < NumBytes; Index++)
		{
			ddr_addr = (pos*4) + (Index*4) * LOCAL_SDI_SIG_NUM;
			b = Get_uI32_DDR3_CC_SDI_DATA( ddr_addr  );
			xil_printf("%d\r\n",b);
		}
	}
	*/
	if(pos >=0 && pos < LOCAL_SDI_SIG_NUM) {
		//NumBytes /= LOCAL_SDI_SIG_NUM;
		for(Index = 0; Index < NumBytes; Index++)
		{
			ddr_addr = (pos*4) + (Index*4) * 2;

			//a = Get_uI32_DDR3_CC_SDI_DATA( ddr_addr  );
			b = Get_uI32_DDR3_CC_SDI_DATA( ddr_addr  );
			xil_printf("%d\r\n", b);
		}
	}
	else {
		for(Index = 0; Index < NumBytes; Index++)
		{		
			{
				Index_mod8 = Index % LOCAL_SDI_SIG_NUM;
				if(Index != 0 && Index_mod8 == 0)
				{
					for(a=0; a<LOCAL_SDI_SIG_NUM; a++) {

						tbtD_BigE[a] = tbtD[a];
					}

					xil_printf("%8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d\r\n", \
							tbtD_BigE[0], tbtD_BigE[1], tbtD_BigE[2], tbtD_BigE[3], \
							tbtD_BigE[4], tbtD_BigE[5], tbtD_BigE[6], tbtD_BigE[7], \
							tbtD_BigE[8], tbtD_BigE[9], tbtD_BigE[10], tbtD_BigE[11], \
							tbtD_BigE[12], tbtD_BigE[13]);

					//clear and increment
					abcd=0;
					length++;
				}
				
				tbtD[abcd] = Get_uI32_DDR3_CC_SDI_DATA( ddr2_addressOffset+(Index*4) );
				abcd++;
			}

		}
	}

	xil_printf("\r\n");
	return XST_SUCCESS;
}


/*
 *	Clear to 0 for ADC raw DDR2 space
 *	Mbyte unit
 */
void	cmd_Clear_DDR2(int argc, char *argv[])
{
int	index;
char    str[10];
int     channel, len;
Xuint32 size;

	channel = strtol(argv[1], (char **)0, 10);
	size    = strtol(argv[2], (char **)0, 10);
		
	if(size == 0 || size > 401 ) {
		xil_printf("size arg is 0, set size number, default is 8 Mbytes ...\r\n");
		size = 2;
	}	
	size *= 1000000;
	switch(channel) {
		case 0:
			for(index=0; index < size; index++) {
				Set_uI32_DDR3_BPM_SDI_DATA( index*4, 0x0 );
			}
			xil_printf("\r\nDDR-3 BPM SDI buffer cleared %d Mbytes\r\n", (index * 2) / 1000000);
		break;
		case 1:		
			for(index=0; index < size; index++) {
				Set_uI32_DDR3_CC_SDI_DATA( index*4, 0x0 );
			}								
			xil_printf("\r\nDDR-3 CC SDI buffer cleared %d bytes\r\n", index);
			break;
		case 2:		
			for(index=0; index < size; index++) {
				Set_uI32_DDR3_BPM_SDI_DATA( index*4, 0x0 );
			}			
			xil_printf("\r\nDDR-3 BPM SDI buffer cleared %d bytes\r\n", index);
			break;
		//////////
		case 10:
			for(index=0; index < size; index++) {
				Set_uI32_DDR3_BPM_SDI_DATA( index*4, index );
			}
			xil_printf("\r\nDDR-2 FA WFM buffer cleared %d bytes\r\n", index);
			break;
		case 11:
			for(index=0; index < size; index++) {
				Set_uI32_DDR3_CC_SDI_DATA( index*4, index );
			}
			xil_printf("\r\nDDR-2 CC WFM buffer cleared %d bytes\r\n", index);
			break;
			
		default:
			xil_printf("Memory selection error\r\n");	
	}
					
}




/*
 *	END
 */
