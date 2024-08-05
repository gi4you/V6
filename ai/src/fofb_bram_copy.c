/*
 * fofb_bram_copy.c
 *
 *  Created on: Jun 6, 2013
 *      Author: kha
 */


#include <stdlib.h>		/* strtod, strtof, strtold  */
#include <math.h>

#include "xio.h"
#include "fpga_system.h"
#include "types.h"
#include "xbasic_types.h"

#include "fofb.h"
#include "fofb_bram_copy.h"
#include "Dfe_ctrl.h"
#include "cmd_modbus.h"

#define		HV_V_LEN	PS_SIZE*3	//V_LEN/2
#define		V_BRAM_BLOCK_SIZE	1024
#define		X_BASE_ADDR		0
#define		Y_BASE_ADDR		1



void	Ref_Init(int nm)
{
int i;
int	ref[365];
	
	for(i=0; i<365; i++) {
		//hex = i+1;
		ref[i] = nm;
	}
	// 2. Reference orbit Write and Read : nm Unit
	fofb_ReferenceOrbitWrite(0, 365, &ref[0]);
}	

/*
 *
 * x : 3*90 = 270
 * y : 3*90 = 270
 *
 */
void	VRAM_Init()
{
	int i,j;
	Xuint32 rData[540];
	float f1;
	Xuint32 hex;

	for(i=0; i<12; i++) {
		xil_printf("addr=%d\r\n", i*HV_V_LEN);

		for(j=0; j<HV_V_LEN; j++) {
			//f1 = ;
			//hex = Float2DWordReg(f1);

			pVM_X->data[i*HV_V_LEN+j] = i*HV_V_LEN+(j);			//270
			pVM_Y->data[i*HV_V_LEN+j] = i*HV_V_LEN+(j+1000) ;	//270
		}
	}

	//Write to BRAM and Readback
	xil_printf("\r\n----- V RAM WRITE -----\r\n");


	for(i=0; i<12; i++) {
		//X
		fofb_V_VectorWrite(0, i, 0, 270, &pVM_X->data[i*HV_V_LEN] );
		//Y
		fofb_V_VectorWrite(0, i, 1, 270, &pVM_Y->data[i*HV_V_LEN] );
		//xil_printf("%d = 0x%x\r\n", i, hex);
	}

}


/*
 *   RAM Block 45
 * 		each block has 512 * 4 sectors for Ut data copy
 *
 */
void	URAM_Init(int offset)
{
	int i,j;
	Xuint32 rData[360];
	//float f1;
	Xuint32 x, y;

	for(i=0; i<PS_SIZE*2; i++) {
		xil_printf("%d, addr=%d\r\n", i, i*BPM_SIZE);

		for(j=0; j<BPM_SIZE; j++) {
#if 0			
			x = Float2DWordReg((float)(j));
			y = Float2DWordReg((float)(j+1));
#else
			x = Float2DWordReg((float)(offset));
			y = Float2DWordReg((float)(offset+1));
#endif			
			//x = i*BPM_SIZE+(j);
			//y = i*BPM_SIZE+(j+1000);
			pUT_X->data[i*BPM_SIZE+j] = x;	
			pUT_Y->data[i*BPM_SIZE+j] = y;	

			//xil_printf("%d,%d = %d\r\n", i, j, y);
		}
	}

	//Write to BRAM and Readback
	xil_printf("\r\n----- U RAM WRITE -----\r\n");

#if 0
	int k=0;
	for(i=0; i<45; i++)
	{
		for(j=0; j<4; j++) {
			//xil_printf("k=%d, j=%d\r\n", k++, i*4+j);
			fofb_UtVectorBRAM_Write(0, k, 0, pUT_X->data);
			fofb_UtVectorBRAM_Write(0, k, 1, pUT_X->data);
			k++;
		}
	}
#else
	//Ut RAM, x and y different block
	//
	/*
	fofb_UtVectorBRAM_Write(0, 0, X_BASE_ADDR, &pUT_X->data[0*BPM_SIZE]);

	fofb_Ut_X_VectorRamRead(0, 0, rData);
	for(i=0; i<180; i++) {
		if(pUT_X->data[i] != rData[i])
			xil_printf("%d= %d, %d\r\n", i, pUT_X->data[i], rData[i]);
	}	
	*/
	
	
	int x_err=0;
	int y_err=0;
	int activeRAM = 0;
	for(i=0; i<PS_SIZE*2; i++)
	{
		///// XXX
		fofb_UtVectorBRAM_Write(activeRAM, i, X_BASE_ADDR, &pUT_X->data[i*BPM_SIZE]);
		fofb_Ut_Xy_VectorRamRead(0, i, X_BASE_ADDR, rData);
		for(j=0; j<180; j++) {
			if(pUT_X->data[i*BPM_SIZE+j] != rData[j]) {
				xil_printf("%d : %d= %d, %d\r\n", i, j, pUT_X->data[i*BPM_SIZE+j], rData[j]);
				x_err++;
			}
		}	
		
		///// YYY	
		fofb_UtVectorBRAM_Write(activeRAM, i, Y_BASE_ADDR, &pUT_Y->data[i*BPM_SIZE]);
		fofb_Ut_Xy_VectorRamRead(0, i, Y_BASE_ADDR, rData);
		for(j=0; j<180; j++) {
			if(pUT_Y->data[i*BPM_SIZE+j] != rData[j]) {
				xil_printf("%d : %d= %d, %d\r\n", i, j, pUT_Y->data[i*BPM_SIZE+j], rData[j]);
				x_err++;
			}
		}			
	}
	
	if(x_err == 0 ) xil_printf("X Uram all pass...!!!\r\n");
	if(y_err == 0 ) xil_printf("Y Uram all pass...!!!\r\n");	
	
#endif

}




//
void	V_P_I_RAM_Init(int RamN)
{
	int i,j;
	Xuint32 rData[90];
	float f1;
	Xuint32 hex;
		
	for(i=0; i<12; i++) {
		xil_printf("addr=%d\r\n", i*HV_V_LEN);

		for(j=0; j<90; j++) {
			//x
			FOFB_DATA->H_Kp[i*90+j] = i*90+j;
			FOFB_DATA->H_Kd[i*90+j] = i*90+100+j;
			FOFB_DATA->H_Ki[i*90+j] = i*90+200+j;
			//y
			FOFB_DATA->V_Kp[i*90+j] = i*90+(j+1000) ;	//270
			FOFB_DATA->V_Kd[i*90+j] = i*90+(j+2000);
			FOFB_DATA->V_Ki[i*90+j] = i*90+(j+3000);
		}
	}


	//Write to BRAM and Readback
	xil_printf("\r\n----- V RAM WRITE ----- %d \r\n", RamN);
	///////////////////
	//Copy DDR to BRAM
	///////////////////

	for(i=0; i<12; i++) {
		fofb_V_P_I_VectorWrite(0, i, 0, &FOFB_DATA->H_Kp[i*90] );	//Vx
		
		fofb_V_P_I_VectorRamRead(0, i, 0, &rData[0]);
		for(j=0; j<90; j++) {
			if(FOFB_DATA->H_Kp[i*90+j] != rData[j])
				xil_printf("P%d : %d= %d, %d\r\n", i, j, FOFB_DATA->H_Kp[i*90+j], rData[j]);
		}
		
		////////		
		fofb_V_P_I_VectorWrite(0, i, 1, &FOFB_DATA->H_Kd[i*90] );	//Px
		
		fofb_V_P_I_VectorRamRead(0, i, 1, &rData[0]);
		for(j=0; j<90; j++) {
			if(FOFB_DATA->H_Kd[i*90+j] != rData[j])
				xil_printf("D%d : %d= %d, %d\r\n", i, j, FOFB_DATA->H_Kd[i*90+j], rData[j]);
		}
		////////		
		fofb_V_P_I_VectorWrite(0, i, 2, &FOFB_DATA->H_Ki[i*90] );	//Ix
		
		fofb_V_P_I_VectorRamRead(0, i, 2, &rData[0]);
		for(j=0; j<90; j++) {
			if(FOFB_DATA->H_Ki[i*90+j] != rData[j])
				xil_printf("I%d : %d= %d, %d\r\n", i, j, FOFB_DATA->H_Ki[i*90+j], rData[j]);
		}
				
		//////////
		fofb_V_P_I_VectorWrite(0, i, 3, &FOFB_DATA->V_Kp[i*90] );	//Vy
		fofb_V_P_I_VectorRamRead(0, i, 3, &rData[0]);
		for(j=0; j<90; j++) {
			if(FOFB_DATA->V_Kp[i*90+j] != rData[j])
				xil_printf("I%d : %d= %d, %d\r\n", i, j, FOFB_DATA->V_Kp[i*90+j], rData[j]);
		}
		///////////		
		fofb_V_P_I_VectorWrite(0, i, 4, &FOFB_DATA->V_Kd[i*90] );	//Py
		fofb_V_P_I_VectorRamRead(0, i, 4, &rData[0]);
		for(j=0; j<90; j++) {
			if(FOFB_DATA->V_Kd[i*90+j] != rData[j])
				xil_printf("I%d : %d= %d, %d\r\n", i, j, FOFB_DATA->V_Kd[i*90+j], rData[j]);
		}
		///////////		
		fofb_V_P_I_VectorWrite(0, i, 5, &FOFB_DATA->V_Ki[i*90] );	//Iy
		fofb_V_P_I_VectorRamRead(0, i, 5, &rData[0]);
		for(j=0; j<90; j++) {
			if(FOFB_DATA->V_Ki[i*90+j] != rData[j])
				xil_printf("I%d : %d= %d, %d\r\n", i, j, FOFB_DATA->V_Ki[i*90+j], rData[j]);
		}		
	}
	
/*
	fofb_V_P_I_VectorRamRead(0, 0, 1, &rData[0]);
	for(i=0; i<90; i++) {
		if(FOFB_DATA->H_Kd[i] != rData[i])
			xil_printf("%d= %d, %d\r\n", i, FOFB_DATA->H_Kd[i], rData[i]);
	}
*/	
	
	
////////////////////////////////

#if 0
	//This test compare W/R data, all function passed
	int ut_data[540];
	for(i=0; i<540; i++)
		ut_data[i] = i;
					
	fofb_V_VectorWrite(0, RamN, 0, 270, &ut_data[0]);  /* channel, WR, len, data */
	fofb_V_VectorWrite(0, RamN, 1, 270, &ut_data[270]);	//y
	xil_printf("Write Done.. %d\r\n", RamN);
	
	for(i=0; i<12; i++) {
		fofb_V_VectorWrite(0, i, 0, 270, &ut_data[0]);  	/* channel, WR, len, data */
		fofb_V_VectorWrite(0, i, 1, 270, &ut_data[270]);	//y
		xil_printf("Write Done.. %d\r\n", i);
	}	
#endif
	
}


/*
 *
 *
 */
void	fofb_DDR2URAM_Copy(int a, int trig)
{
	//Ut RAM, x and y different block
	int i, j;
	Xuint32 rData[360];
	int x_err=0;
	int y_err=0;
	int activeRAM=0;
#if 0
	activeRAM = DFE_CONTROL_REGS->ioReg[FOFB_U_ACTIVE_RAM]&0x1;
	if(trig == 1) {
		for(i=0; i<PS_SIZE*2; i++)
		{
			fofb_UtVectorBRAM_Write(activeRAM, i, X_BASE_ADDR, &pUT_X->data[i*BPM_SIZE]);
			// read 
			fofb_Ut_Xy_VectorRamRead(activeRAM, i, X_BASE_ADDR, rData);
			for(j=0; j<BPM_SIZE; j++) {
				if(pUT_X->data[i*BPM_SIZE+j] != rData[j]) {
					//xil_printf("err_x %2d : %d= %d, %d\r\n", i, j, pUT_X->data[i*BPM_SIZE+j], rData[j]);
					x_err++;
				}
			}	
					
			fofb_UtVectorBRAM_Write(activeRAM, i, Y_BASE_ADDR, &pUT_Y->data[i*BPM_SIZE]);
			// read
			fofb_Ut_Xy_VectorRamRead(activeRAM, i, Y_BASE_ADDR, rData);
			for(j=0; j<BPM_SIZE; j++) {
				if(pUT_Y->data[i*BPM_SIZE+j] != rData[j]) {
					//xil_printf("err_y %2d : %d= %d, %d\r\n", i, j, pUT_Y->data[i*BPM_SIZE+j], rData[j]);
					x_err++;
				}
			}			
	
		}
		EPICS_R_REGS->data[FOFB_UT_BRAM_WERR_CNT] = x_err + y_err;
		xil_printf("\r\nUt activeRAM %d write/read Done... err_x=%d, err_y=%d\r\n", activeRAM, x_err, y_err);
	}
#endif

}


/*
 *
 */
void	fofb_DDR2VRAM_Copy(int a, int trig)
{
int	i, j;
Xuint32 rData[90];
int err=0;	
int activeRAM=0;

	//Write to BRAM and Readback	
	///////////////////
	//Copy DDR to BRAM
	///////////////////
#if 0	
	for(i=0; i<12; i++) {
		fofb_V_P_I_VectorWrite(activeRAM, i, 0, &FOFB_DATA->H_Kp[i*90] );	//Vx
		fofb_V_P_I_VectorWrite(activeRAM, i, 1, &FOFB_DATA->H_Kd[i*90] );	//Px
		fofb_V_P_I_VectorWrite(activeRAM, i, 2, &FOFB_DATA->H_Ki[i*90] );	//Ix
		//
		fofb_V_P_I_VectorWrite(activeRAM, i, 3, &FOFB_DATA->V_Kp[i*90] );		//Vy
		fofb_V_P_I_VectorWrite(activeRAM, i, 4, &FOFB_DATA->V_Kd[i*90] );	//Py
		fofb_V_P_I_VectorWrite(activeRAM, i, 5, &FOFB_DATA->V_Ki[i*90] );	//Iy
	}
#endif
#if 0
	activeRAM = DFE_CONTROL_REGS->ioReg[FOFB_V_ACTIVE_RAM]&0x1;
	if(trig == 1) {
		for(i=0; i<12; i++) {
			fofb_V_P_I_VectorWrite(activeRAM, i, 0, &FOFB_DATA->H_Kp[i*90] );	//Vx
			
			fofb_V_P_I_VectorRamRead(activeRAM, i, 0, &rData[0]);
			for(j=0; j<90; j++) {
				if(FOFB_DATA->H_Kp[i*90+j] != rData[j]) {
					xil_printf("err P%d : %d= %d, %d\r\n", i, j, FOFB_DATA->H_Kp[i*90+j], rData[j]);
					err++;
				}
			}
			
			////////		
			fofb_V_P_I_VectorWrite(activeRAM, i, 1, &FOFB_DATA->H_Kd[i*90] );	//Px
			
			fofb_V_P_I_VectorRamRead(activeRAM, i, 1, &rData[0]);
			for(j=0; j<90; j++) {
				if(FOFB_DATA->H_Kd[i*90+j] != rData[j]) {
					//xil_printf("err D%d : %d= %d, %d\r\n", i, j, FOFB_DATA->H_Kd[i*90+j], rData[j]);
					err++;
				}
			}
			////////		
			fofb_V_P_I_VectorWrite(activeRAM, i, 2, &FOFB_DATA->H_Ki[i*90] );	//Ix
			
			fofb_V_P_I_VectorRamRead(activeRAM, i, 2, &rData[0]);
			for(j=0; j<90; j++) {
				if(FOFB_DATA->H_Ki[i*90+j] != rData[j]) {
					xil_printf("err I%d : %d= %d, %d\r\n", i, j, FOFB_DATA->H_Ki[i*90+j], rData[j]);
					err++;
				}
			}
					
			//////////
			fofb_V_P_I_VectorWrite(activeRAM, i, 3, &FOFB_DATA->V_Kp[i*90] );	//Vy
			fofb_V_P_I_VectorRamRead(activeRAM, i, 3, &rData[0]);
			for(j=0; j<90; j++) {
				if(FOFB_DATA->V_Kp[i*90+j] != rData[j]) {
					//xil_printf("err I%d : %d= %d, %d\r\n", i, j, FOFB_DATA->V_Kp[i*90+j], rData[j]);
					err++;
				}
			}
			///////////		
			fofb_V_P_I_VectorWrite(activeRAM, i, 4, &FOFB_DATA->V_Kd[i*90] );	//Py
			fofb_V_P_I_VectorRamRead(activeRAM, i, 4, &rData[0]);
			for(j=0; j<90; j++) {
				if(FOFB_DATA->V_Kd[i*90+j] != rData[j]) {
					xil_printf("err I%d : %d= %d, %d\r\n", i, j, FOFB_DATA->V_Kd[i*90+j], rData[j]);
					err++;
				}
			}
			///////////		
			fofb_V_P_I_VectorWrite(activeRAM, i, 5, &FOFB_DATA->V_Ki[i*90] );	//Iy
			fofb_V_P_I_VectorRamRead(activeRAM, i, 5, &rData[0]);
			for(j=0; j<90; j++) {
				if(FOFB_DATA->V_Ki[i*90+j] != rData[j]) {
					xil_printf("err I%d : %d= %d, %d\r\n", i, j, FOFB_DATA->V_Ki[i*90+j], rData[j]);
					err++;
				}
			}	
			xil_printf("Bram %d write Done...err = %d\r\n", i, err);	
		}
		
		EPICS_R_REGS->data[FOFB_PID_BRAM_WERR_CNT] = err;		
		xil_printf("V activeRAM %d write Done... err = %d\r\n", activeRAM, err );
	}
#endif

}

