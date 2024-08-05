/*
 * fofb.c
 *
 *  Created on: Jan 2, 2013
 *      Author: kha
 *
 *	4/17/2013
 *		plb_Reg10_SetBpmLength.maddr = 182;	  read reference and position data from DPRAM
 *
 *
 */


#include "fpga_system.h"
#include "xparameters.h"
#include "fofb.h"

#include "xbasic_types.h"
#include "Dfe_ctrl.h"
#include "cmd_modbus.h"

#include "mps.h"

#include "ref_table.h"

#define	MAX_LOCAL_BPM_NODE		13


//RAM CS2
#define	RAM_CS2__PSOUT_FIFO		2
#define	RAM_CS2__EIGEN_FIFO		8


int	ut_data[2048];

/*
 *	This function for epics floating point communication
 *	Microblaze Float -> Long    --------> EPICS Long -> Float
 */
Xuint32	Float2DWordReg(float val)
{
Xuint32* pRegValue;
Xuint32 hex_val;

	pRegValue= (Xuint32*)(&val);
	hex_val = *(pRegValue+0);
	//xil_printf("0x%X\r\n", hex_val);
	return *(pRegValue+0);
}
//
float _Hex32_to_Float ( unsigned int pawSrc, float *padDest)
{
    unsigned int *pwDest;
	unsigned int pwDestBuff[1];

	pwDest = pwDestBuff;
    pwDest = (unsigned int *)(padDest+0);
    *(pwDest+0) = pawSrc;

	return padDest[0];
}

//
void fofb_Reg1_Mask(Xuint32 mask)
{
	*(volatile unsigned int*)(XPAR_FOFB_COEFF_SET_0_BASEADDR + 4) = mask;
}

//reg3-mask
void fofb_Reg3_Mask(Xuint32 mask)
{
	*(volatile unsigned int*)(XPAR_FOFB_COEFF_SET_0_BASEADDR + 12) = mask;
}
//
void fofb_DpRamDataWrite(Xint32 data)
{
	Set_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + 8, data );
}

void fofb_Reg13_Mask(Xuint32 mask)
{
	*(volatile unsigned int*)(XPAR_FOFB_COEFF_SET_0_BASEADDR + 13*4) = mask;
}

void fofb_Reg15_Mask(Xuint32 mask)
{
	*(volatile unsigned int*)(XPAR_FOFB_COEFF_SET_0_BASEADDR + 15*4) = mask;
}

void fofb_Reg9_Mask(Xuint32 mask)
{
	*(volatile unsigned int*)(XPAR_FOFB_COEFF_SET_0_BASEADDR + 9*4) = mask;
}



void fofb_ReferenceOrbitSingleWrite(int start_addr, int Data)
{
int i;
int	wData;

	fofb_dpram.wr = 0;		//disable write
	fofb_dpram.ram_cs = 0;	//cs[0] for x,y
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));

	if(Data > 25) Data = 25;	// 25 mm
	else if(Data < 0) Data = 0;

	{
		wData = Data * 1000000;		// convert to mm
		fofb_dpram.addr   = start_addr;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
		fofb_DpRamDataWrite( wData);	//*pData++ );	//Data write
		fofb_dpram.wr = 1;		//write
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
		fofb_dpram.wr = 0;		//write
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
		//write to DDR for epics reading
		Set_I_DDR2(BASE_REF_ORBIT+(start_addr*4), wData);
	}
	printf("sr-bpm ref set : %d (mm) \r\n", start_addr, wData);
}


/*
 * start address:
 * 	x = 0
 * 	y = 180
 *
 */
void fofb_ReferenceOrbitWrite(int start_addr, int end_addr, int *pData)
{
int i;
int	wData;

	fofb_dpram.wr = 0;		//disable write
	fofb_dpram.ram_cs = 0;	//cs[0] for x,y
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));

	for(i=start_addr; i<end_addr; i++)
	{
		wData = *pData++;
		fofb_dpram.addr   = i;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
		fofb_DpRamDataWrite( wData);	//*pData++ );	//Data write
		fofb_dpram.wr = 1;		//write
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
		fofb_dpram.wr = 0;		//write
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
		//
		//write to DDR for epics reading
		Set_I_DDR2(BASE_REF_ORBIT+(i*4), wData);
	}	
	xil_printf("!!! Reference write Done...\r\n");
}

/*
 *
 */
void mps_SrBPM_XyOffsetWrite(int start_addr, int end_addr, int *pData)
{
int i;
int	org_offset;
int	r_data;

	fofb_dpram.wr = 0;		//disable write
	fofb_dpram.ram_cs = 5;	//cs[5] for offset
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));

	for(i=start_addr; i<end_addr; i++)
	{		
		//get origin offset
		r_data = Get_I_DDR2(BASE_DDR_SRBPM_BBA_OFFSET+(i*4));
		//	
		org_offset = *pData++;
		fofb_dpram.addr   = i;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
		fofb_DpRamDataWrite( org_offset + r_data );	
		fofb_dpram.wr = 1;		//write
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
		fofb_dpram.wr = 0;		//write
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
		//
		//write to DDR for epics reading
		Set_I_DDR2(BASE_DDR_SRBPM_OFFSET+(i*4), org_offset);
	}
	xil_printf("!!! Sr BPM Offset write Done...\r\n");
}



/*
 *
 */
void mps_SrBPM_XyOffsetWithBbaOffsetWrite(int start_addr, int end_addr, int *pData)
{
int i;
int	bba_offset;
int r_data;

	fofb_dpram.wr = 0;		//disable write
	fofb_dpram.ram_cs = 5;	//cs[5] for offset
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));

	for(i=start_addr; i<end_addr; i++)
	{
		//get origin offset
		r_data = Get_I_DDR2(BASE_DDR_SRBPM_OFFSET+(i*4));
		//
		bba_offset = *pData++;
		fofb_dpram.addr   = i;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
		fofb_DpRamDataWrite( bba_offset + r_data);
		fofb_dpram.wr = 1;		//write
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
		fofb_dpram.wr = 0;		//write
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
		//
		//write to DDR for epics reading
		Set_I_DDR2(BASE_DDR_SRBPM_BBA_OFFSET+(i*4), bba_offset);
	}
	xil_printf("!!! Sr BPM BBA Offset write Done...\r\n");
}



/*
 * 6/3/13
 *	compare w/r data for each data.
 *	Dual port RAM
 *
 */
void fofb_MaskRamWrite(int addr, Xuint8 data)
{
int i, r;

	fofb_dpram.wr = 0;		//disable write
	fofb_dpram.ram_cs = 1;	//cs[1]
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));

	//for(i=0; i<MaxAddr; i++) 
	//do {
		fofb_dpram.addr   = addr;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
		//
		fofb_DpRamDataWrite( data & 0x1);	//int write
		//
		fofb_dpram.wr = 1;		//write
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
		fofb_dpram.wr = 0;		//write
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));

		//write to DDR for epics reading
		Set_I_DDR2(BASE_DDR_SRBPM_MASK_OFFSET+(addr*4), data);

		//read
		//r = fofb_MaskRamOneDataRead(addr);
	//}while(data != r);
	
	//xil_printf("!!! Mask RAM WRITE addr=%d data=%d\r\n", addr, data);
}


//	Reference Orbit is stored at Dual Port RAM
//
void	fofb_ReferenceOrbitRead(int cs_n, int *pData)
{
	int i;
	//Xint32 data;

	//select mux
	mps_plb_Reg15.bram_ref_rd_sel = cs_n & 0x3;
	mps_Reg_Set(15, *((Xuint32 *)&mps_plb_Reg15 ));

	//cs[0] ref
	//cs[5]	Offset
	fofb_dpram.ram_cs = cs_n;
	fofb_dpram.en = 1;
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
	
	for(i=0; i<SR_BPM_XY_SIZE; i++) {
		fofb_dpram.addr = i;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
    	//
		*pData++ = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (31*4));
		//Copy to DDR
		//Set_I_DDR2(BASE_REF_ORBIT+(i*4), data);
		//xil_printf("%d=%d\r\n", i, data);
	}
	fofb_dpram.en = 0;
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
}	






//
int	fofb_MaskRamOneDataRead(int addr)
{
	int i;
	int data;

	fofb_dpram.ram_cs = 1;	//cs[1]
	fofb_dpram.en = 1;
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
	
	{
		fofb_dpram.addr = addr;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
    	//
		data = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (29*4));
	}
	fofb_dpram.en = 0;
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
	return data;
}

//
void	fofb_MaskRamRead()
{
	int i;
	Xint32 data;

	fofb_dpram.ram_cs = 1;	//cs[1]
	fofb_dpram.en = 1;
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
	
	for(i=0; i<100; i++) {
		fofb_dpram.addr = i;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
    	//
		data = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (29*4));
		xil_printf("%d=%d\r\n", i, data);
	}
	fofb_dpram.en = 0;
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
}	


/*
 *
 */
void fofb_UOutputMaskRamWrite(int addr, Xuint8 data)
{
int i;

	fofb_dpram.wr = 0;		//disable write
	fofb_dpram.ram_cs = 6;	//cs[6]
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));

	//for(i=0; i<MaxAddr; i++)
	{
		fofb_dpram.addr   = addr;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
		//
		fofb_DpRamDataWrite( data & 0x3);	//int write
		//
		fofb_dpram.wr = 1;		//write
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
		fofb_dpram.wr = 0;		//write
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
	}

}



//
void	fofb_PsMonFifoRead(int type)
{
	int Index, Index_mod12, a;
	Xint32 data;
	int ps_out[16];
	int	abcd, length;
	
	fofb_dpram.ram_cs = 2;	//cs[2]
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));

	sleep(5);
	data = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (27*4));
	xil_printf("fifo status = %X\r\n", data & 0xC0000000);

	if(type == 1 ) {
		for(Index=0; Index<2046; Index++) {
			//send read output data
			fofb_dpram.wr = 1;
			fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
			fofb_dpram.wr = 0;
			fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
	    	//
			data = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (27*4));
			//Copy to DDR			
			Set_I_DDR2(DDR3_KICK_FIFO_BASE+(Index*4), data);
			/*	
			if( (data & 0x80000000) == 0x80000000) {
				xil_printf("FIFO EMPTY\r\n");
				break;
			}
			else
				xil_printf("%d=%x\r\n", Index, data&0xFFFFFFFF);
			*/	
		}
#if 0		
		for(Index = 0; Index < 2046; Index++)
		{
			Index_mod12 = Index % 12;
			if( (Index != 0) && (Index_mod12 == 0) )
			{
				xil_printf ("%x %x %x %x %x %x %x %x %x %x %x %x\r\n", ps_out[0], ps_out[1], ps_out[2], ps_out[3], ps_out[4], ps_out[5], ps_out[6], ps_out[7], ps_out[8], ps_out[9], ps_out[10], ps_out[11] );
				//clear and increment
				abcd=0;
				length++;
			}					    	
			ps_out[abcd] = Get_I_DDR2(DDR3_KICK_FIFO_BASE + (Index*4)) & 0x0FFFFFFF;	// 27*4=0x6C			
			abcd++;
		}
		xil_printf("DDR3 Base = 0x%X\r\n", DDR3_KICK_FIFO_BASE);			
#endif		
	}
	else 
	{
		abcd=0;
		length=0;
		
		// 9 point are not alinged
		/*
		for(Index=0; Index<9; Index++) {
			fofb_dpram.wr = 1;
			fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
			fofb_dpram.wr = 0;
			fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
		}			
		*/
		for(Index = 0; Index < 2046; Index++)
		{
			Index_mod12 = Index % 12;
			if( (Index != 0) && (Index_mod12 == 0) )
			{
				xil_printf ("%x %x %x %x %x %x %x %x %x %x %x %x\r\n", ps_out[0], ps_out[1], ps_out[2], ps_out[3], ps_out[4], ps_out[5], ps_out[6], ps_out[7], ps_out[8], ps_out[9], ps_out[10], ps_out[11] );
				//clear and increment
				abcd=0;
				length++;
			}				
			
			//read
			fofb_dpram.wr = 1;
			fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
			fofb_dpram.wr = 0;
			fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
	    	
			ps_out[abcd] = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + 0x6C) & 0x0FFFFFFF;	// 27*4=0x6C

			
			/*
			if( (ps[abcd] & 0x80000000) == 0x80000000) {
				xil_printf("FIFO EMPTY\r\n");
				break;
			}								
			else
				xil_printf("%d=%x\r\n", Index, ps[abcd]&0xFFFFFFFF);
				*/
			
			//xil_printf("%d=%x\r\n", Index, ps[abcd]);
			
			abcd++;
		}	
	}
	fofb_dpram.en = 0;
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
	
	//data = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (27*4));
	//xil_printf("fifo status = %X, length=%d\r\n", data & 0xC0000000, length);
}


//
//
void	fofb_EigenFifoRead(int dbg, int len)
{
	int i;
	Xint32 data;
	int ps_out[3];
	
	fofb_dpram.ram_cs = 8;	//cs[2]
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));

	if(len > 2048) len = 540;
	else if(len ==0) len = 540;
	//
	if(dbg == 0) len = 2048;	//epics mode

	int abcd=0;
	int length=0;
	for(i=0; i<len; i++) {							
		int mod = i % 3;
		if( (i != 0) && (mod == 0) )
		{
			if(dbg)
				xil_printf ("%3d : %x %x %x\r\n", length, ps_out[0], ps_out[1], ps_out[2] );
			//clear and increment
			abcd=0;
			length++;
		}		
		fofb_dpram.wr = 1;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
		fofb_dpram.wr = 0;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
    	//
		ps_out[abcd] = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (27*4));
		//Copy to DDR			
		Set_I_DDR2(DDR3_EIGEN_FIFO_BASE+(i*4), ps_out[abcd]);	
							    					
		abcd++;							
	}

	fofb_dpram.en = 0;
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));

}

// Position error FIFO read and DDR Write
// Read RF bpm 30 signals only
//
void	fofb_PosErrorFifoRead(int dbg, int len)
{
	int i;
	Xint32 data;

	//fofb_dpram.ram_cs = 9;	//cs[2]
	fofb_dpram.ram_cs = 8;	//bram
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));

	if(len > 2048) len = 2048;
	else if(len ==0) len = 360;

	if(dbg == 0) len = 2048;	//epics mode

#if FIFO
	for(i=0; i<len; i++) {
		fofb_dpram.wr = 1;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
		fofb_dpram.wr = 0;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
    	//
		data = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (26*4));
		//Copy to DDR			
		Set_I_DDR2(DDR3_ERR_FIFO_BASE+(i*4), data);		
		if(dbg)	{
			if(i%6 == 0) xil_printf("\r\n");
			xil_printf("%d = %d\r\n", i, data);
		}
	}
#endif
	//DPRAM
	for(i=0; i<len; i++) {
		fofb_dpram.addr = i;	//address
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
    	//get data
		data = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (26*4));
		//Copy to DDR
		Set_I_DDR2(DDR3_ERR_FIFO_BASE+(i*4), data);
		if(dbg)	{
			if(i%6 == 0) xil_printf("\r\n");
			xil_printf("%d = %d\r\n", i, data);
		}
	}

	//
	data = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (23*4));
	xil_printf("Reg23 = 0x%x\r\n", data);

	fofb_dpram.en = 0;
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
}




/*
 * n_Ut is 0 - 89
 *
 */
void	fofb_UtVectorBRAM_Write(int activeRAM, int n_Ut, int xy, Xuint32 *pData)
{
int i, n;
int UtRamOffset;
int addr;

	//Ut RAM 0
	plb_Reg13.U_activeRam = activeRAM&0x1;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));

	//Disable all RAM write mode
	for(n=0; n<45; n++)		//max node is 45
		fofb_CS1_WRITE(0, n, 0);	//all read mode
#if 0
	int ram_block_num = n_Ut / 4;	//RAM block number
	int ram_num = n_Ut % 4;			// sector number 0, 1, 2, 3
#endif
	int ram_block_num = n_Ut % 45;	//RAM block number
	int ram_num = n_Ut / 45;			// sector number 0, 1, 2, 3
	
	//1. select RAM cs 1 - 45
	fofb_CS1_WRITE(0, ram_block_num, 1);	//cs[0] write
	fofb_CS1_WRITE(2, ram_block_num, 1);	//cs[2] enable/disable

	//1
	UtRamOffset = ram_num * 512;
	//xil_printf("RAM blk :%d, sector=%d, offset=%d\r\n", ram_block_num, ram_num, UtRamOffset);
	if(xy == 0) {
		for(i=0; i<180; i++)
		{
			addr=i+UtRamOffset;
			Set_I_DDR2( CS3_CTRL_ADDR, addr);
			Set_I_DDR2( CS3_DATA_ADDR, *pData++ );
		}
	}
	else {
		for(i=0; i<180; i++)
		{
			addr=i+UtRamOffset;
			Set_I_DDR2( CS3_CTRL_ADDR, addr + 180);
			Set_I_DDR2( CS3_DATA_ADDR, *pData++ );
			//xil_printf("%d, %d\r\n", addr, *pData-1);
		}
	}

	//Disable
	fofb_CS1_WRITE(0, ram_block_num, 0);	//cs[0] write
	fofb_CS1_WRITE(2, ram_block_num, 0);	//cs[2] enable/disable
	//RAM
	if(activeRAM == 0)
		plb_Reg13.U_activeRam = 1;
	else plb_Reg13.U_activeRam = 0;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));
}




//
// U vector RAM ant value write function fr test
// input
//		n_Ut 0 - 179
//		Data  :  floating to Hex vale, Ut Data format is floating
//
void	fofb_UtVectorDataWrite(int uRamBlock, int Data)
{
int i, n;
int UtRamOffset;
int addr;


	//Disable all RAM write mode
	for(n=0; n<45; n++)		//max node is 45
		fofb_CS1_WRITE(0, n, 0);	//all read mode

	//int ram_block_num = uRamBlock / 4;	//RAM block number
	//int ram_num = uRamBlock % 4;			// sector number 0, 1, 2, 3
	
	int ram_block_num = uRamBlock % 45;	//RAM block number
	int ram_num = uRamBlock / 45;			// sector number 0, 1, 2, 3
	
	//1. select RAM cs 1 - 45
	fofb_CS1_WRITE(0, ram_block_num, 1);	//cs[0] write
	fofb_CS1_WRITE(2, ram_block_num, 1);	//cs[2] enable/disable

	//1
	UtRamOffset = ram_num * 512;
	//xil_printf("RAM blk :%d, sector=%d, offset=%d\r\n", ram_block_num, ram_num, UtRamOffset);
	for(i=0; i<365; i++)
	{
		addr=i+UtRamOffset;
		Set_I_DDR2( CS3_CTRL_ADDR, addr);
		Set_I_DDR2( CS3_DATA_ADDR, Data );
	}

	//Disable
	fofb_CS1_WRITE(0, ram_block_num, 0);	//cs[0] write
	fofb_CS1_WRITE(2, ram_block_num, 0);	//cs[2] enable/disable
}

//
void	fofb_UtVectorFloatDataWrite(int activeRam, int n_Ut, float Data)
{
int i, n;
int UtRamOffset;
int addr;
int	hex;

	hex = Float2DWordReg(Data);

	//Ut RAM 0
	plb_Reg13.U_activeRam = activeRam&0x1;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));
		
	//Disable all RAM write mode
	for(n=0; n<45; n++)		//max node is 45
		fofb_CS1_WRITE(0, n, 0);	//all read mode

	//int ram_block_num = n_Ut / 4;	//RAM block number
	//int ram_num = n_Ut % 4;			// sector number 0, 1, 2, 3
	int ram_block_num = n_Ut % 45;	//RAM block number
	int ram_num = n_Ut / 45;		// sector number 0, 1, 2, 3	
	
	//1. select RAM cs 1 - 45
	fofb_CS1_WRITE(0, ram_block_num, 1);	//cs[0] write
	fofb_CS1_WRITE(2, ram_block_num, 1);	//cs[2] enable/disable

	UtRamOffset = ram_num * 512;
	xil_printf("RAM blk :%d, sector=%d, offset=%d\r\n", ram_block_num, ram_num, UtRamOffset);	
	
	for(i=0; i<365; i++)
	{
		addr=i+UtRamOffset;
		Set_I_DDR2( CS3_CTRL_ADDR, addr);
		Set_I_DDR2( CS3_DATA_ADDR, hex );
	}
	//Disable
	fofb_CS1_WRITE(0, ram_block_num, 0);	//cs[0] write
	fofb_CS1_WRITE(2, ram_block_num, 0);	//cs[2] enable/disable
		
	//Ut RAM 0
	if(activeRam == 0)
		plb_Reg13.U_activeRam = 1;
	else 	
		plb_Reg13.U_activeRam = 0;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));	
}

//
void	fofb_UtVectorRamRead(int activeRam, int n_Ut, int MaxAddr, int *pData)
{
}


/*
 *
 */
void	fofb_Ut_Xy_VectorRamRead(int activeRam, int n_Ut, int xy, int *pData)
{

}




//
void	fofb_myUtVectorRamRead(int n_Ut, int MaxAddr, int *pData)
{
int i, n;
int data;
	//Disable all RAM write mode
	for(n=0; n<90; n++)
		fofb_CS1_WRITE(0, n, 0);	//all read mode
	
	//1. select RAM cs 1 - 90
	fofb_CS1_WRITE(0, n_Ut, 0);	//cs[0] read mode
	fofb_CS1_WRITE(2, n_Ut, 1);	//cs[2] enable/disable
	
	//2. write address and data	
	for(i=0; i<MaxAddr; i++) 
	{		
		fofb_UtRam.addr = i;
		Set_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (5*4), *((Xuint32 *) & fofb_UtRam));	
		//		
		data = Get_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (29*4) );
		xil_printf("%d = %x\r\n", i, data);
	}
	
	//Disable
	//fofb_CS1_WRITE(0, n_Ut, 0);	//cs[0] write
	fofb_CS1_WRITE(2, n_Ut, 0);	//cs[2] enable/disable
}



/* --------------------------------------
 * 4/30/2013
 *	n_Ut : 0 -89
 *	Max Address < 360
 */
void	fofb_V_VectorWrite(int activeRam, int n_Ut, int xy, int MaxAddr, int *pData)
{
int i, n;
int rd;
Xuint32	wData[270];
Xuint32 rData[270];

	//RAM
	plb_Reg13.V_activeRam = activeRam&0x1;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));

	//Disable all RAM write mode
	for(n=0; n<12; n++)
		fofb_CS1_WRITE(1, n, 0);	//all read mode
	
	//1. select RAM cs 1 - 12
	fofb_CS1_WRITE(1, n_Ut, 1);	//cs[1] write
	fofb_CS1_WRITE(3, n_Ut, 1);	//cs[3] enable/disable

	//2. write address and data
	if(xy == 0) {	//X = 0 ... 269
		for(i=0; i<MaxAddr; i++)
		{
			wData[i] = *pData++;
			Set_I_DDR2( CS3_CTRL_ADDR, i);
			//Set_I_DDR2( CS3_DATA_ADDR, *pData++ );
			Set_I_DDR2( CS3_DATA_ADDR, wData[i] );	
		}
		
		//READ
		fofb_CS1_WRITE(1, n_Ut, 0);	//read mode
		fofb_V_VectorRamRead(n_Ut, 0, 270, &rData[0] );
		for(i=0; i<270; i++) {
			if(rData[i] != wData[i]  )
				printf("ram=%d, %d = %d, %d\r\n", n_Ut, i, rData[i], wData[i] );		
		}
			
	}
	else {		//Y = 270 ... 539
		for(i=0; i<MaxAddr; i++)
		{			
			wData[i] = *pData++;
			Set_I_DDR2( CS3_CTRL_ADDR, i + 270 );
			//Set_I_DDR2( CS3_DATA_ADDR, *pData++ );
			Set_I_DDR2( CS3_DATA_ADDR, wData[i] );			
		}
		//READ
		fofb_CS1_WRITE(1, n_Ut, 0);	//read mode
		fofb_V_VectorRamRead(n_Ut, 270, 540, &rData[0] );
		for(i=0; i<270; i++) {
			if(rData[i] != wData[i]  )
				printf("ram=%d, %d = %d, %d\r\n", n_Ut, i, rData[i], wData[i] );		
		}		
	}

	//Disable
	fofb_CS1_WRITE(1, n_Ut, 0);	//cs[0] write
	fofb_CS1_WRITE(3, n_Ut, 0);	//cs[2] enable/disable
		
	//RAM
	if( activeRam == 1)
		plb_Reg13.V_activeRam = 0;
	else plb_Reg13.V_activeRam = 1;

	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));
}



/*
 *
 */
void	fofb_V_VectorFloatWrite(int n_V, int MaxAddr, float Data)
{
int i, n;
int	hex;

	hex = Float2DWordReg(Data);

	//Disable all RAM write mode
	for(n=0; n<12; n++)
		fofb_CS1_WRITE(1, n, 0);	//all read mode

	//1. select RAM cs 1 - 90
	fofb_CS1_WRITE(1, n_V, 1);	//cs[1] write
	fofb_CS1_WRITE(3, n_V, 1);	//cs[3] enable/disable

	//2. write address and data
	for(i=0; i<MaxAddr; i++)
	{
		Set_I_DDR2( CS3_CTRL_ADDR, i);
		Set_I_DDR2( CS3_DATA_ADDR, hex );
	}

	//Disable
	fofb_CS1_WRITE(1, n_V, 0);	//cs[0] write
	fofb_CS1_WRITE(3, n_V, 0);	//cs[2] enable/disable
}


/*
 *
 *
 */
void	fofb_V_P_I_VectorWrite(int activeRam, int n_Ut, int vpi_xy, int *pData)
{
int i, n;
int	offset;
int rd;
//Xuint32	rData[540];
//Xuint32	wData[90];

	//select Active RAM
	plb_Reg13.V_activeRam = activeRam&0x1;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));

	//Disable all RAM write mode
	for(n=0; n<12; n++)
		fofb_CS1_WRITE(1, n, 0);	//all read mode

	//1. select RAM cs 1 - 12
	fofb_CS1_WRITE(1, n_Ut, 1);	//cs[1] write
	fofb_CS1_WRITE(3, n_Ut, 1);	//cs[3] enable/disable

	//2. write address and data
	switch(vpi_xy) {
		case 0:		//Vx
			offset = 0;
			break;
		case 1:		//Px
			offset = 1;
			break;
		case 2:		//Ix
			offset = 2;
			break;
		case 3:		//Vy
			offset = 270;
			break;
		case 4:		//Py
			offset = 271;
			break;
		case 5:		//Iy
			offset = 272;
			break;
	}

	for(i=0; i<90; i++)
	{
		//wData[i] = *pData++;
		Set_I_DDR2( CS3_CTRL_ADDR, 3*i+offset);	//address write
		Set_I_DDR2( CS3_DATA_ADDR,  *pData++);	//write
		//Set_I_DDR2( CS3_DATA_ADDR,  wData[i] );
		//xil_printf("%d=%d\r\n", 3*i+offset, wData[i]);
#if 0	//READBACK
		fofb_CS1_WRITE(1, n_Ut, 0);	//read mode
		rd = Get_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (28 * 4) );	//read
		if(*(pData-1) != rd) xil_printf("RAM %d : addr=%d, w=%d, r=%d data Fail..!!!\r\n", n_Ut, 3*i+offset, *(pData), rd);
		fofb_CS1_WRITE(1, n_Ut, 1);	//write mode
#endif
	}
	
			
	//Disable
	fofb_CS1_WRITE(1, n_Ut, 0);	//cs[0] write
	fofb_CS1_WRITE(3, n_Ut, 0);	//cs[2] enable/disable

	//RAM
	if( activeRam == 1)
		plb_Reg13.V_activeRam = 0;
	else plb_Reg13.V_activeRam = 1;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));
}


/*
 *
 */
void	fofb_V_P_I_VectorRamRead(int activeRam, int n_Ut, int vpi_xy, Xuint32 *pData)
{
int i, n;
int offset;

	//mux selection for multiple RAM read
	fofb_dpram.mux1_se2 = n_Ut;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));

	//RAM 0
	plb_Reg13.V_activeRam = 0;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));

	//
	//Disable all RAM write mode
	for(n=0; n<12; n++)
		fofb_CS1_WRITE(0, n, 0);	//all read mode
	
	//1. select RAM cs 1 - 90
	fofb_CS1_WRITE(1, n_Ut, 0);	//cs[0] read mode
	fofb_CS1_WRITE(3, n_Ut, 1);	//cs[2] enable/disable

	
	//2. write address and data
	switch(vpi_xy) {
		case 0:		//Vx
			offset = 0;
			break;
		case 1:		//Px
			offset = 1;
			break;
		case 2:		//Ix
			offset = 2;
			break;
		case 3:		//Vy
			offset = 270;
			break;
		case 4:		//Py
			offset = 271;
			break;
		case 5:		//Iy
			offset = 272;
			break;
	}
				
	//2. write address and data	
	for(i=0; i<90; i++) 
	{
		Set_I_DDR2( CS3_CTRL_ADDR, 3*i+offset);	//address write
		//Set_I_DDR2( CS3_CTRL_ADDR, i);	//set address
		*(pData+(i)) = Get_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (28 * 4) );
	}	
	//Disable
	fofb_CS1_WRITE(3, n_Ut, 0);	//cs[2] enable/disable

	//RAM 1
	plb_Reg13.V_activeRam = 1;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));
}



//
void	fofb_V_VectorRamRead(int n_Ut, int start, int end, Xuint32 *pData)
{
int i, n;


	//mux selection for multiple RAM read
	fofb_dpram.mux1_se2 = n_Ut;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));

	//RAM 0
	plb_Reg13.V_activeRam = 0;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));

	//
	//Disable all RAM write mode
	for(n=0; n<12; n++)
		fofb_CS1_WRITE(0, n, 0);	//all read mode
	
	//1. select RAM cs 1 - 90
	fofb_CS1_WRITE(1, n_Ut, 0);	//cs[0] read mode
	fofb_CS1_WRITE(3, n_Ut, 1);	//cs[2] enable/disable
	
	//2. write address and data	
	for(i=start; i<end; i++) 
	{
		Set_I_DDR2( CS3_CTRL_ADDR, i);	//set address
		*(pData+(i-start)) = Get_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (28 * 4) );
		//data = Get_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (28 * 4) );
		//f = _Hex32_to_Float ( data, &pf);
		//printf("ram=%d, %d = %x (%f)\r\n", n_Ut, i, data, f);
	}	
	//Disable
	fofb_CS1_WRITE(3, n_Ut, 0);	//cs[2] enable/disable

	//RAM 1
	plb_Reg13.V_activeRam = 1;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));
}





/*
 *	Position simulation DRAM Data write
 */
void XyPositionSimDataAllSet(Xuint32 Data32)
{
int i, j;

	fofb_dpram.wr = 0;		//disable write
	fofb_dpram.ram_cs = 7;	//cs[7]
	Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));

	// total length is 420 because include cell data (1,2)
	// This DPRAM is same configuration with SDI inside Dual Port RAM
	//for(j=0; i<3; j++)
	{
		for(i=0; i<FOFB_CCSDI_DATA_LENGTH; i++) {
			fofb_dpram.addr   = i;
			Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
			Set_I_DDR2( CS2_DATA_ADDR, Data32);		//data write
			//
			fofb_dpram.wr = 1;		//write
			Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
			fofb_dpram.wr = 0;		//write
			Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
		}
	}

	xil_printf("Position simulation DPRAM %d points write done...\r\n", FOFB_CCSDI_DATA_LENGTH);
}

/*
 * X,y Position simulation Data
 *	0..11  bpm x,y positon
 *	12..13 cell controller data, this address skip
 *
 */
void XyPositionSimDataSet( Xuint32 *pData)
{
int i, j;
int	addr;
int Data;

	fofb_dpram.wr = 0;		//disable write
	fofb_dpram.ram_cs = 7;
	Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));

	for(i=0; i<FOFB_CCSDI_DATA_LENGTH; i++) {
		fofb_dpram.addr   = i;
		Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));

		Set_I_DDR2( CS2_DATA_ADDR, pData[i] );	//write
		//
		fofb_dpram.wr = 1;		//write
		Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
		fofb_dpram.wr = 0;		//write
		Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));

		//xil_printf("(%d) = %d\r\n", i, pData[i]);
	}
	
#if 0
	j = 0;
	for(i=0; i<360; i++) {
		if( (i%12) == 0 && (i>11) ) {
			j++;
			xil_printf("%d\r\n", j);
			if(j >= 1){
				 addr = i+(j*2)+11;

				xil_printf("c1(%d) = %d, %d\r\n", i, addr, pData[i+11]);
    			fofb_dpram.addr   = addr;	//BRAM address
    			Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
    			Set_I_DDR2( CS2_DATA_ADDR, pData[i+11] );
			}
		}
		else {
			if(j != 0) {
    			addr = i+(j*2)-1;
    			Data = pData[i-1];
			}
			else {	//0..11
    			addr = i;
    			Data = pData[i];
			}
			xil_printf("c(%d) = %d, %d\r\n", i, addr, Data);
			fofb_dpram.addr   = addr;	//BRAM address
			Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
			Set_I_DDR2( CS2_DATA_ADDR, Data );	//write
		}
		//
		fofb_dpram.wr = 1;		//write
		Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
		fofb_dpram.wr = 0;		//write
		Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
	}
#endif

}

/**/
void fofb_SimRamSinglePositionAddressSet(int addr, Xuint32 Data32)
{
	fofb_dpram.addr   = Data32;
	Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
}

/*
 * */
void fofb_SimRamSinglePositionDataSet(int addr, Xuint32 Data32)
{
	xil_printf("Bpm Addr  = %d : Data = %d (nm) \r\n", addr, Data32);
	fofb_dpram.wr     = 0;	//disable write
	fofb_dpram.ram_cs = 7;	//cs[7]
	Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));

	{
		//Write ADDRESS
		fofb_dpram.addr   = addr;
		Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
		//
		Set_I_DDR2( CS2_DATA_ADDR, Data32);		//data write
		//
		fofb_dpram.wr = 1;		//write
		Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
		fofb_dpram.wr = 0;		//write
		Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
	}
}



/********************************************************************
 *  FOFB_POS_XY_DATA_SIM
 *	plb_Control_0[29]
 *	Select Simulation Data or SDI Data ?
 *
 */
void	fofb_PosXyDataSim(int a, int sim)
{
	if(sim == 1) {
		xil_printf("BPM X,Y position data selected SIM mode\r\n");
		}
	else {
		xil_printf("BPM X,y position data connected BPM SDI Data, Now Operation Mode\r\n");
	}
	plb_Control_0.PosSimulation       = sim & 0x1;	//[29]
	Set_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (6*4), *((Xuint32 *) & plb_Control_0));
}


/*
 *	FOFB_POS_SIM_DATA_MODE
 *		PLB mode or COUNT mode ?
 *		plb_Control_1[28]
 *
 *	RAM block data is uBlaze control or Ramp data 0..359
 */
void	fofb_DpramSimDataIsuBlaze(int a, int Data)
{
	xil_printf("This command valid ONLY BPM Simulation Mode\r\n");
	if(Data == 1) {
		xil_printf("Simulation DPRAM Data control by Microblaze\r\n");
		}
	else {
		xil_printf("Simulation DPRAM Data is Count Mode\r\n");
	}
	plb_Reg7Control_1.SimMbInterface = Data;	//0: Default, 1: Microblaze
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );	
}



//FOFB_POS_SIM_DATA_TYPE
//	reg6 plb_Control_0[31]
void	fofb_PosXyDataSimDataMode(int a, int type)
{
	if(type == 1) {
		xil_printf("X,y SIM mode, uBlaze control mode\r\n");
		//1. select count mode
		plb_Reg7Control_1.SimMbInterface = 1;	//0: Default, 1: Microblaze
		Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	}
	else
		xil_printf("X,y SIM mode, 0-359 count type\r\n");

	plb_Control_0.bpm_pos_sim       = type & 0x1;
	Set_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (6*4), *((Xuint32 *) & plb_Control_0));
}

void	fofb_PosXyDataSimWfmType(int a, int type)
{
	if(type == 1) {
		xil_printf("Single mode wfm is Ramp\r\n");
	}
	else
		xil_printf("waveform type is COS \r\n");
	//2. waveform type
	plb_Reg7Control_1.SimRamp = type;	//0: cos, 1: ramp
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
}

//SimRamp
//		.DataTypeIsRamp(plb_Control_1[29]),	//1: Ramp, 0:sin



/* FOFB_POS_SIM_ADDR_SELECT
 *	reg6 plb_Control_0[31] = 1
 *	w_reg6	plb_Control_0[21:10] 
 */
void Bpm2SimBpmAddrSelect(int a, int addr)
{
	plb_Control_0.sim_addr = addr;
	Set_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (6*4), *((Xuint32 *) & plb_Control_0));
}


//FOFB_POS_NUM_DACOUT
//w_reg6	plb_Control_0[9:0]
//
void	fofb_DacOutputSelect(int a, int bpm_num)
{
	if(bpm_num >= 780)
		xil_printf("Bpm and CC mem number range over [0-779]\r\n");
	else
		xil_printf("BPM2DAC : %d\r\n", bpm_num);

	plb_Control_0.bpm2dac_addr       = bpm_num;
	Set_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (6*4), *((Xuint32 *) & plb_Control_0));
}

//
// This function work when Local SDI Reg25 set 1
//
void	fofb_LocalLinkDataType(int a, int Mode)
{

	if(Mode == 1)
		xil_printf("Bpm SDI Test Mode is Count Mode= %d\r\n", Mode);
	else
		xil_printf("Bpm SDI Test Mode is Sin wave = %d\r\n", Mode);

	//plb_LocalLinkDataCtrl.DataSel = Mode;
	//Set_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (8*4), *((Xuint32 *) & plb_LocalLinkDataCtrl));
}


/*
 * plb_Control_0[9:0]   =  BPM address for DAC Output
 */
void Bpm2DacAddrSelect(int addr)
{
	plb_Control_0.bpm2dac_addr = addr;
	Set_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (6*4), *((Xuint32 *) & plb_Control_0));

}


/*
 * Kpid, Eigen register write
 */
void SoftTrigCs1RegWrite()
{
	//FOFB_PLB_CS1.trig = 1;
	//Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	//FOFB_PLB_CS1.trig = 0;
	//Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	//xil_printf("soft trig...\r\n");
}


/*
 * Kp_x = ram_cs1[0]
 * Ki_x = ram_cs1[1]
 *
 *	slow_corr_ref = ram_cs1[10]
 *
 */
void CS1_DpRamWriteMulti(int cs, int max_addr, Xuint32 *pData32)
{
int i;
	FOFB_PLB_CS1.wr = 0;		//disable write
	FOFB_PLB_CS1.ram_cs = cs;
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));

	for(i=0; i<max_addr; i++) {
		FOFB_PLB_CS1.addr   = i;
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));

		Set_I_DDR2( CS1_DATA_ADDR, pData32[i] );	//write
		//
		FOFB_PLB_CS1.wr = 1;		//write
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
		FOFB_PLB_CS1.wr = 0;		//write
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	}

	//FOFB_PLB_CS1.ram_cs = 0;
	//Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	SoftTrigCs1RegWrite();
}

/* CS[n]
 *	0 : oneMode Activer Ram WE	1 - 90
 *	1 : V ram WE control
 *	2 : Ut RAM enable/disable
 *	3 : V Ram enable/disable
 *
 */
void fofb_CS1_WRITE(int cs, int addr, Xuint32 Data32)
{
int i;

	FOFB_PLB_CS1.wr = 0;			//disable write
	FOFB_PLB_CS1.ram_cs = cs & 0xF;	
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));

	FOFB_PLB_CS1.addr   = addr;
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));

	Set_I_DDR2( CS1_DATA_ADDR, Data32 );	//write

	FOFB_PLB_CS1.wr = 1;		//write enable
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	FOFB_PLB_CS1.wr = 0;		//write disable
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
#if 0
	//SoftTrigCs1RegWrite();
	FOFB_PLB_CS1.trig = 1;
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	FOFB_PLB_CS1.trig = 0;
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));	
#endif	
}

/*
 * Corrector PS setting
 *
 */
void fofb_CorrPsOffsetControl(int addr, Xint32 Data32)
{
int i;

	FOFB_PLB_CS1.wr = 0;		//disable write
	FOFB_PLB_CS1.ram_cs = 10;	//CS 10 is Power Supply
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));

	//for(i=0; i<BPM_SIZE; i++) 
	{
		FOFB_PLB_CS1.addr   = addr;
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));

		Set_I_DDR2( CS1_DATA_ADDR, Data32 );	//write
		//
		FOFB_PLB_CS1.wr = 1;		//write
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
		FOFB_PLB_CS1.wr = 0;		//write
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	}
	//SoftTrigCs1RegWrite();
}


/*
 * 	reg [31:0]  SysCtrlRegs [31:0];
 *	cs1[11]
 *
 *
 */
void fofb_SystemControl(int addr, Xuint32 pData32)
{
int i;

	FOFB_PLB_CS1.wr = 0;		//disable write
	FOFB_PLB_CS1.ram_cs = 11;	//CS 11 is Power Supply
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));

#if (FOFB)
	if(addr >= 125) addr -= 113;	//12,13,14,15,.... for system controls
#endif
	xil_printf("sys addr = %d\r\n", addr);
	{
		FOFB_PLB_CS1.addr   = addr;				//select address
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));

		Set_I_DDR2( CS1_DATA_ADDR, pData32 );	//write data
		//
		FOFB_PLB_CS1.wr = 1;		//write enable
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
		FOFB_PLB_CS1.wr = 0;		//write disable
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	}

}

/*
 *
 */
void	fofb_CorrPs_KickEnable(int addr, Xuint8 data)
{
	FOFB_PLB_CS1.wr = 0;		//disable write
	FOFB_PLB_CS1.ram_cs = 5;	//CS 10 5 kick enable
#if 0
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	{
		if(addr >= FOFB_CORR_PS_EN1) addr -= FOFB_CORR_PS_EN1;	//epics cmd
		else FOFB_PLB_CS1.addr   = addr;						//console cmd
		
		xil_printf("Kick en addr=%d, data=%d\r\n", addr,data );
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));

		Set_I_DDR2( CS1_DATA_ADDR, data );	//write
		//
		FOFB_PLB_CS1.wr = 1;		//write
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
		FOFB_PLB_CS1.wr = 0;		//write
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	}	
#endif
}

/*
 *
 */
void	fofb_CorrPs_KickModeIsFofb(int addr, Xuint8 data)
{
	FOFB_PLB_CS1.wr = 0;		//disable write
	FOFB_PLB_CS1.ram_cs = 6;	//CS 10 5 kick enable
#if 0
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	{
		if(addr >= FOFB_CORR_PS_KICK_MODE1) addr -= FOFB_CORR_PS_KICK_MODE1;	//epics cmd
		else FOFB_PLB_CS1.addr   = addr;										//console cmd
		xil_printf("Kick mode addr=%d, data=%d  (0:NCO, 1:FOFB)\r\n", addr,data );
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));

		Set_I_DDR2( CS1_DATA_ADDR, data );	//write
		//
		FOFB_PLB_CS1.wr = 1;		//write
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
		FOFB_PLB_CS1.wr = 0;		//write
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	}
#endif
}


/*
 *
 */
void CS1_DpRamWriteClear(int cs, Xuint32 Data32)
{
int i;
	FOFB_PLB_CS1.wr = 0;		//disable write
	FOFB_PLB_CS1.ram_cs = cs;
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));

	for(i=0; i<BPM_SIZE; i++) {
		FOFB_PLB_CS1.addr   = i;
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));

		Set_I_DDR2( CS1_DATA_ADDR, Data32 );	//write
		//
		FOFB_PLB_CS1.wr = 1;		//write
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
		FOFB_PLB_CS1.wr = 0;		//write
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	}

	FOFB_PLB_CS1.ram_cs = 0;
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
}


/*
 *
 */
void CS2_DpRamWriteMulti(int cs, int MaxAddr, Xuint32 Data32)
{
int i;
#if 1
	fofb_dpram.wr = 0;		//disable write
	fofb_dpram.ram_cs = cs;
	Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));

	for(i=0; i<MaxAddr; i++) {
		fofb_dpram.addr   = i;
		Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));

		Set_I_DDR2( CS2_DATA_ADDR, Data32+i );	//write
		//
		fofb_dpram.wr = 1;		//write
		Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
		fofb_dpram.wr = 0;		//write
		Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
	}
#endif
	
	xil_printf("write cs=%d n=%d, done...\r\n", cs, MaxAddr);
	//O_DPRAM_WR_CS2.ram_cs = 0;
	//Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
}




/*
 *
 */
void CS2_DpRamWriteSigleAddr(int cs, int addr, Xuint32 Data32)
{
int i;
#if 1
	fofb_dpram.wr = 0;		//disable write
	fofb_dpram.ram_cs = cs;
	Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));

	//for(i=0; i<BPM_SIZE; i++) 
	{
		fofb_dpram.addr   = addr;
		Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));

		Set_I_DDR2( CS2_DATA_ADDR, Data32 );	//write
		//
		fofb_dpram.wr = 1;		//write
		Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
		fofb_dpram.wr = 0;		//write
		Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
	}
#endif
	//O_DPRAM_WR_CS2.ram_cs = 0;
	//Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
}




/*
 *
 */
void CS3_DpRamWriteMulti(int cs, int addr, Xuint32 Data32)
{
int i;
	FOFB_PLB_CS3.wr = 0;		//disable write
	FOFB_PLB_CS3.ram_cs = cs;

	Set_I_DDR2( CS3_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS3));

	for(i=0; i<BPM_SIZE; i++) {
		FOFB_PLB_CS3.addr   = i;
		Set_I_DDR2( CS3_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS3));

		Set_I_DDR2( CS3_DATA_ADDR, Data32+i );	//write
		//
		FOFB_PLB_CS3.wr = 1;		//write
		Set_I_DDR2( CS3_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS3));
		FOFB_PLB_CS3.wr = 0;		//write
		Set_I_DDR2( CS3_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS3));
	}

	//FOFB_PLB_CS3.ram_cs = 0;
	//Set_I_DDR2( CS3_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS3));
}

			
/*
 *
 */
int	ReadKregs( int addr, int len )
{
	Xuint16	a, b;
	Xuint32 d;
	int	i;
	int max;
	

	if(len > 1000) len = 100;
	FOFB_PLB_CS1.regSel = addr;
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	xil_printf("addr=%d\r\n", addr);
	
	if(addr == 6) max = 24;
	else if(addr == 0) max = len;
	else if(addr == 1) max = len;
	else if(addr == 2) max = len;
	else if(addr == 3) max = len;
	else if(addr == 4) max = len;
	else if(addr == 5) max = len;
	else if(addr == 6) max = len;
	else if(addr == 7) max = len;
	else if(addr == 8) max = len;	//ps en
	else if(addr == 9) max = len;	//ps mode
	else if(addr == 10) max = len;
	else if(addr == 11) max = len;
	else if(addr == 12) max = len;
	else if(addr == 13) max = len;
	else if(addr == 14) max = len;
	else max = 90;
	
	int regAddr;
	for(i=0; i<max; i++) {
		//Kp,i,d address selection
		//plb_Control_0.regAddr       = i;
		//Set_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (6*4), *((Xuint32 *) & plb_Control_0));

		plb_Reg7Control_1.DacChASet = i;	//select address
		Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );

		d = Get_I_DDR2(READ_K_REG);	//read register

		switch(addr) {
			case 0:
				xil_printf("%2d : %d, 0x%x\r\n", i, d, (Xuint32)d);
				break;
			case 1:
				xil_printf("%2d : %d\r\n", i, d);
				break;
			case 2:

				xil_printf("%2d : %d\r\n", i, d);
				break;
			case 3:

				xil_printf("%2X : %X\r\n", i, d);
				break;
			case 4:

				xil_printf("%2d :  %x\r\n", i, d);
				break;
			case 5:

				xil_printf("%2d :  %x\r\n", i, d);
				break;

			case 6:

				xil_printf("%2d :  %d\r\n", i, d);
				break;
			//
			case 7:
				if( i>= 6 || i<10)
					xil_printf("%2d :  0x%x\r\n", i, d);
				else
					xil_printf("%2d :  %d\r\n", i, d);
				break;

			default:

				xil_printf("%2d :  %d, 0x%x\r\n", i, d, (Xuint32)d );
		}
	}
	return 0;
}



#if 0
/* control regs */
typedef struct {
		Xuint32 Space		    : 6;
		Xuint32 Reboot		    : 1;
		Xuint32 sfp_trig	    : 1;
		Xuint32 sfp_ee_addr		: 9;
		Xuint32 DacChASet 		: 16;
    } w_reg7;	//w_reg7
w_reg7	plb_Control_1;
#endif


/*
 * 255 + eeprom 2 address offset
 *	addr 357 : tx power
 *	addr 359 : rx power
 *	bpm
 *	358 ) //tx power
 *	 360 ) //rx power
 *	 * 0.1; 		// 0.1 uW
 */
int	ReadSfpEepromData(int addr, int *pData)
{
	//Xint32	sfp_power[6];
	
	int i;

	plb_Reg7Control_1.sfp_ee_addr = addr;
	Set_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (7*4), *((Xuint32 *) & plb_Reg7Control_1));

	plb_Reg7Control_1.sfp_trig = 1;
	Set_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (7*4), *((Xuint32 *) & plb_Reg7Control_1));

	plb_Reg7Control_1.sfp_trig = 0;
	Set_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (7*4), *((Xuint32 *) & plb_Reg7Control_1));

	//read data
	FOFB_PLB_CS1.regSel = 7; 	//cs address is 7
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));	
	for(i=0; i<6; i++) {
		plb_Reg7Control_1.DacChASet = i;	//select address
		Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
		pData[i] = Get_I_DDR2(READ_K_REG);	//read register
	}

	//ReadKregs(7);
	return 0;
}


/*
 *	Manual test port, ADC CH1
 *
 */
int	DAC_ChA_SetData(int val)
{
	plb_Reg7Control_1.DacChASet = val;
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	xil_printf("DAC CHA = %d \r\n", val );

	return 0;
}

//
void	Reboot()
{
	plb_Reg7Control_1.Reboot = 1;
	//Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	Set_I_DDR2(REG7_WR, 0xffffffff);
	xil_printf("Microblaze Reboot!!! \r\n" );
}

// assigned only CC SDI because fofb clock and bpm sdi clock domain different.
//	DAC Input source
//  1: Local SDI, 0: CC SDI
void DAC_DataSourceSec(int a, int sdi)
{
	if(sdi == 1) xil_printf("Dac-A output is address \r\n" );
	else if(sdi == 0) xil_printf("DAC-A output is RAM port-B out \r\n" );

	
	plb_Reg7Control_1.DacLocalSDI = sdi & 1;
	Set_I_DDR2( REG7_WR, *((Xuint32 *) & plb_Reg7Control_1));
	
}

void	CorrPsID_Setting(int a, int Data)
{
//int	offset_addr;
#if 0
	if(a >= FOFB_CORR_PS_ID1) a -= FOFB_CORR_PS_ID1;
	//else offset_addr = a;
	xil_printf("PS ID %d= %d\r\n", a+1, Data);
	fofb_SystemControl(a, Data);
#endif
}

/*
 */
void	CorrPsRemoteSetting(int a, int Data)
{
#if 0
	if(a >= FOFB_CORR_PS_ISET1) a -= FOFB_CORR_PS_ISET1;
	//else offset_addr = a;
	xil_printf("PS %d = %d\r\n", a+1, Data);
	fofb_CS1_WRITE(10, a, Data);
#endif
}

/*
 *
 */
void	ReadFPGA_Ver()
{
int ver;
Xuint32 data, reg20, reg24, tcnt;
	ver = Get_I_DDR2(FIRMWARE_VER);
	xil_printf("***** FPGA Firmware Ver: %x *****\r\n", ver);	//[13:0]
	//xil_printf("***** IN: 0x%x *****\r\n", ver&0xFFFF0000);

	data  = Get_uInt32_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (23*4) );
	reg24 = Get_uInt32_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR+ (24*4) );	// >> 20) & 0x1FF ;
	//
	tcnt  = (Get_uInt32_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR+ (22*4) ) >> 24) & 0xFF ;

	reg20  = Get_uInt32_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR+ (20*4) );	//ID pos out error co

	xil_printf("\r\nreg20 ID bpm pos error = %d\r\n",reg20);

	xil_printf("\r\nSystem status= 0x%X,  trig = %d\r\n", data, tcnt);
	displayBits(data);

	xil_printf("\r\nreg24 = 0x%X\r\n",reg24);
	displayBits(reg24);
}


/*
LinkStatus = (Get_I_DDR2(XPAR_SDI2_0_BASEADDR+(14*4)) >> 24)&0xFF;
LinkStatus = (Get_I_DDR2(XPAR_SDI2_CC_BASEADDR+(14*4)) >> 24)&0xFF;
npi_status  = Get_I_DDR2(FIRMWARE_VER);

	bpm_link = Get_I_DDR2(XPAR_SDI2_0_BASEADDR+(14*4);
	cc_link  = Get_I_DDR2(XPAR_SDI2_CC_BASEADDR+(14*4);

	sfp1 = LinkStatus & 0x04;
	sfp2 = LinkStatus & 0x10;
	xil_printf("SFP1=%d,  SFP2=%d\r\n\r\n", sfp1>>2, sfp2>>4);
	
	bpm_link[28];
	bpm_link[30];
	cc_link[28];
	cc_link[30];
	npi_status[14];
	npi_status[15];

*/



/*
 * Cell con
 * 		 .fa_pos_x( {16'd0, plb_CCLinkData[15:0]} ), 			//Cell Controller Data 0
 *       .fa_pos_y( {16'd0, plb_CCLinkData[31:16]} ), 			//Cell Controller Data 1
 *
 */
void	cmd_FOFB_LocalTestDataSet(int argc, char *argv[])
{

}



// A or B
void	fofb_RefOrbit_ActiveRam(int a, int data)
{
	xil_printf("Reference ActiveRam Select = %d\r\n", data);
}
// BO
void	fofb_U_ActiveRam(int a, int data)
{
	plb_Reg13.U_activeRam = data&0x1;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));
	xil_printf("Ut ActiveRam Select = %d\r\n", data);
}

// BO
void	fofb_V_ActiveRam(int a, int data)
{
	plb_Reg13.V_activeRam = data&0x1;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));
	xil_printf("V ActiveRam Select = %d\r\n", data);
}

// BO
void	fofb_U_PID_Reset(int a, int data)
{
	plb_Reg13.U_pidReset = data&0x1;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));
	xil_printf("U PIDreg Reset = %d\r\n", data);
	//plb_Reg13.U_pidReset = 0;
	//fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));	
}

// BO
void	fofb_V_PID_Reset(int a, int data)
{
	plb_Reg13.V_pidReset = data&0x1;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));
	xil_printf("V PIDreg Reset = %d\r\n", data);
	plb_Reg13.V_pidReset = 0;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));		
}

// LongOut
void	fofb_U_Length(int a, int data)
{
	Set_uI32_PLB( XPAR_FOFB_COEFF_SET_0_BASEADDR + (13*4), data );
	xil_printf("Ut Length = %d\r\n", data);
}

// LongOut
void	fofb_V_Length(int a, int data)
{
	Set_uI32_PLB( XPAR_FOFB_COEFF_SET_0_BASEADDR + (14*4), data );
	xil_printf("V Length = %d\r\n", data);
}


//EVR trigger Mode enable/disable
void	fofb_EVR_TrigModeSet(int a, int data)
{
	//plb_Reg13.EVR_TRIG_MODE = data&1;
	//fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));
	if(data == 1) xil_printf("EVR CONTROL TRIGGER ENABLED\r\n");
	else xil_printf("EVR CONTROL TRIGGER DISABLED, available Manual Control Mode\r\n");
}


//Manual control, feedback ON
void	fofb_Feedback_Run(int a, int data)
{
	//plb_Reg13.FOFB_RUN = data&1;
	//fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));
	if(data == 1) xil_printf("FEEDBACK RUN...\r\n");
	else xil_printf("FEEDBACK STOP...\r\n");	
}


/*
 *
 */
void fofb_WavefromBRamWrite(int type, Xint32 *pData)
{
int i, r;
Xuint32 hex;


	fofb_dpram.wr = 0;		//disable write
	fofb_dpram.en = 1;
	fofb_dpram.ram_cs = 5;	//cs[1]
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));


	for(i=0; i<8001; i++)
	{
		if(type == 0)
			hex = Float2DWordReg( (float)*pData++);		//floating value for real
		else	
			hex = *pData++;		//DAC test
		
		fofb_dpram.addr   = i;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));	//Write ADDRESS		
		fofb_DpRamDataWrite( hex );	//int write
		//Set_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + 8, hex );		//6/19
		//
		fofb_dpram.wr = 1;		//write
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
		fofb_dpram.wr = 0;		//write
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
	}
	/*
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
	for(i=0; i<100; i++) {
		fofb_dpram.addr = i;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));    
		r = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (26*4));
		xil_printf("addr=%d : %d, %d\r\n", i, FOFB_DATA->UserWfmData[i], r);
	}
	*/	
	fofb_dpram.en = 0;
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));	
}

int	fofb_WavefromBRamRead(int addr)
{
	int i;
	int data;

	fofb_dpram.ram_cs = 5;	//cs[1]
	fofb_dpram.en = 1;
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
	//for(i=0; i<100; i++)
	{
		fofb_dpram.addr = addr;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
    	//
		data = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (26*4));
		//xil_printf("addr=%d : %d\r\n", addr, data);
	}

	fofb_dpram.en = 0;
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
	return data;
}




void	fofb_npi_sel(int a, int data)
{
	//plb_Reg9.npi_sel   = data&0x1;
	//fofb_Reg9_Mask( *((Xuint32 *) &plb_Reg9) );
	if(data == 1)
		xil_printf("NPI is eigen data 540\r\n");
	else
		xil_printf("NPI is position erroe\r\n");
}

//user fifo memory for err, eigen, psout
void	fofb_user_fifo_trig()
{
		xil_printf("fifo trig...\r\n");
		fofb_dpram.user_fifo_npi_trig = 1;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
		sleep(2);
		fofb_dpram.user_fifo_npi_trig = 0;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));

		//wait 20 ms
		/*
		fofb_PsMonFifoRead(1);	
		fofb_EigenFifoRead(0, 2048);
		fofb_PosErrorFifoRead(0, 2048);
		*/
}		


void	fofb_UtUserInjectionAddressSel(int a, int data)
{
	//plb_Reg12_Ctrl.Ut_InjAddr = data;
	//Set_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (12*4), *((Xuint32 *)&plb_Reg12_Ctrl) );
}		


/*
 *
 */
void fofb_ControlTest(int addr, Xuint32 pData32)
{
int i;
Xuint32	DdrData;

	FOFB_PLB_CS1.wr = 0;		//disable write
	FOFB_PLB_CS1.ram_cs = 0;	//CS 11 is Power Supply
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));

	xil_printf("sys addr = %d\r\n", addr);
	for(i=0; i<100000; i++)
	{
		//FOFB_PLB_CS1.addr   = addr;
		//Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
		//read ddr3 data buffer
		DdrData = pUT_X->data[i];
		//Data
		Set_I_DDR2( CS1_DATA_ADDR, DdrData );	//write
		//
		FOFB_PLB_CS1.wr = 1;		//write
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
		FOFB_PLB_CS1.wr = 0;		//write
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	}
	FOFB_PLB_CS1.ram_cs = 0xf;	//CS 11 is Power Supply
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));

	xil_printf("Write Done..\r\n");
}


/*
 *
 */
void AI_BmMode_Select(int addr, Xuint32 pData32)
{
	switch(pData32) {
	case 0:
		xil_printf("default single point\r\n");
		break;
	case 1:
		xil_printf("3 bpms out of envelop count\r\n");
		break;
	case 2:
		//xil_printf("2 point compare\r\n");
		xil_printf("NOT\r\n");
		break;
	case 3:
		//xil_printf("2 point compare and 3 bpms out of envelop count\r\n");
		xil_printf("NOT\r\n");
		break;
	default:
		xil_printf("none\r\n");
	}
	plb_Reg10_SetBpmLength.ai_bm_mode_sel = pData32 & 0xF;
	Set_I_DDR2(REG10_WR, *((Xuint32 *)&plb_Reg10_SetBpmLength) );

}

/*
 *
 */

void	fofb_RegInit()
{
	int 	i, j, n, hex;
	float	f1;

	plb_Control_0.bpm_pos_sim       = 0;
	plb_Control_0.CcSim_data_type   = 1; //cc data type, count mode 0 - 13 will count data
	plb_Control_0.PosSimulation     = 0;
	plb_Control_0.sim_addr = 0;
	plb_Control_0.bpm2dac_addr = 0;
	Set_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (6*4), *((Xuint32 *) & plb_Control_0));


	//plb_LocalLinkDataCtrl.LocalKx = 1000;	//Kx, Ky
	//plb_LocalLinkDataCtrl.LocalMaxCnt = 10000;
	//plb_LocalLinkDataCtrl.DataSel = 1;	//bpm data type
	//Set_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (8*4), *((Xuint32 *) & plb_LocalLinkDataCtrl));

	/*
	 * 	bpm_pos_read, assign position read length
	 */
	//plb_Reg10_SetBpmLength.maddr = BPM_SIZE + 2;		//BPM_SIZE+2 for FIFO Latancy is 2 clock  4/17 tested with CHIP SCOPE
	plb_Reg10_SetBpmLength.ai_bm_mode_sel = 0;
	plb_Reg10_SetBpmLength.lemo1_mux_sel = 0;
	plb_Reg10_SetBpmLength.lemo1_mux_sel = 0;
	plb_Reg10_SetBpmLength.id_dump_en = 1;
	plb_Reg10_SetBpmLength.bm_dump_en = 1;
	plb_Reg10_SetBpmLength.CcTestPacktLength = CCSDI_PACKET_LENGTH;  //CC link count packet length
	Set_I_DDR2(REG10_WR, *((Xuint32 *)&plb_Reg10_SetBpmLength) );

    fofb_dpram.muxModeIsRun = 1;	//RUN Mode
    fofb_dpram.mux1_sel = 0;
    fofb_dpram.mux1_se2 = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));

	//clear sum value
#if 1
	plb_Reg13.EVR_TRIG_MODE = 0;
	plb_Reg13.FOFB_RUN = 1;
	plb_Reg13.U_activeRam = 0;
	plb_Reg13.V_activeRam = 0;
	plb_Reg13.U_pidReset = 0;
	plb_Reg13.V_pidReset = 1;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));

	plb_Reg13.U_pidReset = 0;
	plb_Reg13.V_pidReset = 0;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));

	/*
	 * U and V length, +5 clock for latency
	 */

	plb_Reg13.U_length = FOFB_U_CAL_LENGTH;
	plb_Reg13.V_length = FOFB_V_CAL_LENGTH;		//3*45
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));
#endif

	// DAC output is LOCAL SDI output
	plb_Reg7Control_1.DacLocalSDI = 0;
	Set_I_DDR2( REG7_WR, *((Xuint32 *) & plb_Reg7Control_1));
	//Reboot disable
	plb_Reg7Control_1.Reboot = 0;
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );

	//Ut address is 0
	//plb_Reg12_Ctrl.Ut_InjAddr = 0;
	//Set_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (12*4), *((Xuint32 *)&plb_Reg12_Ctrl) );
	
	//Simulation DPRAM Data control
	//plb_Reg7Control_1.SimMbInterface = 0;	//0 is default
	//Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	//

	/////////////////////////////////////////////
	// 1. BPM Position Simulation value : nm Unit
	fofb_PosXyDataSim(0, 0);				//X,y data is SDI
	fofb_DpramSimDataIsuBlaze(0, 1);		//uBlaze data write mode
	XyPositionSimDataAllSet( 0 );			//Clearing for test x,y position data are zero
	//CS2_DpRamWriteMulti(7, 780, 100);		// write 0 ~ 780

	//reference
	for(i=0; i<360; i++) {
		//hex = i+1;
		ut_data[i] = ref_table[i];
	}
	// 2. Reference orbit Write and Read : nm Unit
	fofb_ReferenceOrbitWrite(0, 360, &ut_data[0]);
	//fofb_ReferenceOrbitRead(180);

	// SR bpm offset write
	for(i=0; i<360; i++) {
		ut_data[i] = 0;
	}
	mps_SrBPM_XyOffsetWrite(0, 360, &ut_data[0]);

	//xy Mask all enabled

	fofb_MaskRamWrite(0, 1);
	for(i=1; i<180; i++) {
		//if(i%2)
		//	fofb_MaskRamWrite(i, 0x1);
		//else
		fofb_MaskRamWrite(i, 0x1);
	}
	for(i=180; i<360; i++) {
		fofb_MaskRamWrite(i, 0x1);
	}
	fofb_MaskRamWrite(180, 1);
	//fofb_MaskRamWrite(779, 1);
	//fofb_MaskRamRead();


#if 0
	for(i=0; i<135; i++) {
#if 0
		if(i > 2) {
			if(i%3 == 0)		
				fofb_UOutputMaskRamWrite(i, 1);
			else
				fofb_UOutputMaskRamWrite(i, 0);
		}
		//else if(i == 0)
		fofb_UOutputMaskRamWrite(0, 1);
		fofb_UOutputMaskRamWrite(1, 0);
		fofb_UOutputMaskRamWrite(2, 0);
#endif
		fofb_UOutputMaskRamWrite(i, 1);		//all 1
	}
	

	//ps control
	for(i=0; i<12; i++) {
		fofb_CorrPs_KickEnable(i, 0);		//1: fofb/NCO enable, 0: uBlaze ONLY
		fofb_CorrPs_KickModeIsFofb(i, 0);	//1: fofb, 0: NCO
	}

	//
	// 3. Ut Data Write to RAM : floating point
	//
	//Ut RAM 0
	plb_Reg13.U_activeRam = 0;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));

	f1 = (1.0 * 1);
	for(i=0; i<2048; i++) {
		//f1 = 0.0;
		hex = Float2DWordReg(f1);
		ut_data[i] = hex;
	}

	/* ram 0:1 - 44:45
	 *
	 *
	 * */
	for(j=0; j<180; j++) {
		fofb_UtVectorFloatDataWrite( 0, j, j+1 );
	}


	plb_Reg9.npi_sel   = 1;
	fofb_Reg9_Mask( *((Xuint32 *) &plb_Reg9) );

	//Read Ut RAM
	//xil_printf("----- Read Ut RAM0 -----\r\n");
	//fofb_UtVectorRamRead(0, 20, 0);

	//Ut RAM 1
	plb_Reg13.U_activeRam = 1;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));


	/* -----------------------------------------------
	 *	V RAM Write/Read 12 RAM block
	 * -----------------------------------------------
	 */
#if 1
	//RAM 0
	plb_Reg13.V_activeRam = 0;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));

	f1 = (1.0 * 1);
	for(i=0; i<540; i++) {
		//f1 = 0.0;
		hex = Float2DWordReg(f1);
		ut_data[i] = hex;
	}
		
	xil_printf("\r\n----- V RAM WRITE -----\r\n");
	for(i=0; i<12; i++) {
		fofb_V_VectorWrite(0, i, 0, 270, &ut_data[0]);  	/* channel, WR, len, data */
		fofb_V_VectorWrite(0, i, 1, 270, &ut_data[270]);	//y
		xil_printf("Write Done.. %d\r\n", i);
		xil_printf("%d = 0x%x\r\n", i, hex);
	}

	//RAM 1
	plb_Reg13.V_activeRam = 1;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));
#endif


#if 0
	for(i=0; i<12; i++) {
		fofb_V_VectorWrite(i, 540, &ut_data[0]);  /* channel, WR, len, data */
		xil_printf("Write Done.. %d\r\n", i);
	}
#endif


	//Now, swtich to the newly written table: active table =1; inactive =0
	plb_Reg13.U_activeRam = 1;
	plb_Reg13.V_activeRam = 1;
	plb_Reg13.U_pidReset = 0;
	plb_Reg13.V_pidReset = 1;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));


	//SysControl Setting
	fofb_CS1_WRITE(11, 0, 30000000);		//300 ms
	fofb_CS1_WRITE(11, 1,  1500000);			//x pos hi
	fofb_CS1_WRITE(11, 2, -1500000);			//x pos low
	fofb_CS1_WRITE(11, 3, 10000);			//y pos hi
	fofb_CS1_WRITE(11, 4, 00000);


    /* Corrector PS IDs */
    fofb_SystemControl(0, 1);
    fofb_SystemControl(1, 2);
    fofb_SystemControl(2, 3);
    fofb_SystemControl(3, 4);
    fofb_SystemControl(4, 5);
    fofb_SystemControl(5, 6);
    fofb_SystemControl(6, 7);
    fofb_SystemControl(7, 8);
    fofb_SystemControl(8, 9);
    fofb_SystemControl(9, 10);
    fofb_SystemControl(10, 11);
    fofb_SystemControl(11, 12);

    /* NCO */
    fofb_SystemControl(12, 0x028F5D70);	//frequency
    fofb_SystemControl(13, 500000);		//Kx
    fofb_SystemControl(14, 250000);		//--- Ky Not Used
    fofb_SystemControl(15, 0);			//Kx offset
    fofb_SystemControl(16, 250000);		//--- Ky offset Not Used

    /**/
    fofb_SystemControl(20, 1500000);	//x_h limit
    fofb_SystemControl(21, -1500000);	//x_l limit
    fofb_SystemControl(22, 250000);
    fofb_SystemControl(23, 250000);
    fofb_SystemControl(24, 0x7FFF);		//Avg Number
    fofb_SystemControl(25, 8000);		//User Waveform Max Length



	// MPS
    // position err setting
    fofb_SystemControl(28, 1);	//x1
    fofb_SystemControl(29, 2);	//x2
    fofb_SystemControl(30, 1);	//y1
    fofb_SystemControl(31, 2);	//y2
    fofb_SystemControl(52, 100);	//x,y limit
    //
//#3 mmpo  (x2-x1)*S
    fofb_SystemControl(36, 1);	//x1
    fofb_SystemControl(37, 2);	//x2
    fofb_SystemControl(38, 1);	//y1
    fofb_SystemControl(39, 2);	//y2
    fofb_SystemControl(153, 1);	//S
    fofb_SystemControl(63, 100);	//x,y limit
//	#3 mrad  (x2-x1)/S
    fofb_SystemControl(44, 10);	//x1
    fofb_SystemControl(45, 11);	//x2
    fofb_SystemControl(46, 12);	//y1
    fofb_SystemControl(47, 13);	//y2
    fofb_SystemControl(203, 1);	//S
    fofb_SystemControl(83, 100);	//x,y limit

#endif


    //fofb_SystemControl(17, 1000000000);	//fifo,npi enable width
    //fofb_SystemControl(1020, -10000);	//low
    //fofb_SystemControl(1021,  10000);	//high

    //x,y position read
    *(volatile unsigned int*)(XPAR_FOFB_COEFF_SET_0_BASEADDR + 15*4) = 1;
    //
    plb_Reg14.cell_max_num = 26;	// 6:rf bpm, 3 ID bpm, 3 x-bpm, 1 cell controller
    plb_Reg14.cell_bpm_num = 12;	// only 6 bpms
    plb_Reg14.sdi_max_sig_num = 780;
    plb_Reg14.bpm_all_valid_num = 180;
    mps_Reg14_Mask( *((Xuint32 *)&plb_Reg14 ));


	FOFB_PLB_CS1.regSel = 0;
	Set_I_DDR2( CS1_CTRL_ADDR, 0x0);

	plb_Control_0.regAddr       = 100;
	Set_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (6*4), 0x1);

	//disable npi
	plb_Reg13.U_pidReset = 0;
	fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));
}

//
void	uBlazeWd(int set)
{
	plb_Reg10_SetBpmLength.watchdog_rst=set;
	Set_I_DDR2(REG10_WR, *((Xuint32 *)&plb_Reg10_SetBpmLength) );
}
