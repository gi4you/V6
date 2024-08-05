/*
 * 
 * 
 * 	Module : thread_timer.c
 * 
 *	SDK 12.1
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timer.h>
#include "xio.h"
#include "xiic.h" 
#include "xparameters.h" 
#include "xbasic_types.h"		/* contains basic types for Xilinx software IP */
#include "uart_print.h"
#include "fpga_system.h"


#include "cmd_modbus.h"
#include "dfe_comm.h"
#include "evr.h"
#include "version.h"
#include "fofb.h"
#include "Dfe_ctrl.h"

#include "pscEpics.h"

#include "mps.h"

//extern MachineType;
//extern int	SysMonADC_Read();
extern int	SysMonADC_Read2();


void	_Float2DWordReg(unsigned int RegAddr, double val)
{
Xuint32* pRegValue;
float	fval;
	fval = (float)val;
	pRegValue= (Xuint32*)(&fval);
	pSrvTxDdrOffAddr->data[RegAddr] = *(pRegValue+0);
	printf("0x%X ,  %.3f\r\n", pSrvTxDdrOffAddr->data[RegAddr], val);
}



#if 0
///////////////////////////////////////////////////
// Read DDR-3 0xD0000000 global position Data from DDR memory
//	each cell 8 bpms x 30 cell
// 	PV : SR:C31-{AI}Pos:Global
//
///////////////////////////////////////////////////
Xuint32 pre_pos_ddr_data, pos_ddr_data_head;
int	rev_tcnt=0;
void test()
{
	Xuint32	 pm_ddr_data_head;

	Xuint32 DataSendBytes;
	int _wfm_size;
	int	tcnt;

	//pre_pos_ddr_data = 0;
	Xuint32	offset;

	//tcnt  = (Get_uInt32_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR+ (24*4) ) >> 24) & 0xFF ;
	//if(tcnt != rev_tcnt)
	//	xil_printf("tcnt = %d\r\n", tcnt);
	//if(mps_plb_Reg13.npi_run == 1)  //if NPI-1 is ENABLED ?
	{
		offset = 5000 * 4;
		pos_ddr_data_head = Get_I_DDR2(0xD0000000 + offset );	//changed head by trigger
		if(pre_pos_ddr_data != pos_ddr_data_head)
		{
			//tx_done = 0;
			if(DFE_CONTROL_REGS->ioReg[60] < 10000 ) _wfm_size = 10000;			//1 sec
			else if(DFE_CONTROL_REGS->ioReg[60] > 100000) _wfm_size = 100000;	//10 sec
			else _wfm_size = DFE_CONTROL_REGS->ioReg[60];

			DataSendBytes = (480 * 4) * _wfm_size;	//5 sec, 72,000,000 byte ?
			xil_printf("TX DDR 0xD0000000, size = %d,  %d bytes  Wait...\r\n", _wfm_size, DataSendBytes);
#if 0
			// send head
			psc_TxHead = (psc_TxHead_t *)txHeadBuf;
			psc_TxHead->idA     = 'P';
			psc_TxHead->idB     = 'S';								/* 32-bit address */
			psc_TxHead->MsgId   = TX_NPI1_MSG_ID;			/* 53  */
			psc_TxHead->bodyLen = DataSendBytes;					// N of DATA
			//
			if ((bytes_sent = lwip_write(sd, txHeadBuf, HEAD_SIZE)) != HEAD_SIZE) {
				xil_printf("%s: ERROR Msg ID=%d,  rcvd = %d, written = %d\n\r", __FUNCTION__, psc_TxHead->MsgId, bytes_sent, HEAD_SIZE);
				goto __end;
			}
			//send DDR-3 0xD0000000 DATA
			if (lwip_writeall(sd, pDdrGlobalXyPosAddr, DataSendBytes) != DataSendBytes) {
				xil_printf("Closing Wfm TX Msg ID= %d, len=%d\n\r", psc_TxHead->MsgId, DataSendBytes );
				goto __end;
			}
#endif

			//clear ddr address
			//Set_I_DDR2(0xD0000000 + offset, 0 );
			//tx_done = 1;
			xil_printf("TX DDR npi1 DONE...\r\n");
		}
		Set_I_DDR2(0xD0000000 + offset, 0 );	//two time's transmitting
		pre_pos_ddr_data = pos_ddr_data_head;
	}

	rev_tcnt = tcnt;
}

#endif


/**/
/********************************************************
* Function : dBpm_UserTimer_Thread
*
* Parameters  :
*
* Return value : 
*
* Description :	timer thread
*
********************************************************/		
void dBpm_UserTimer_Thread(void* arg)
{
Xuint32 ticks = 0;
Xuint32 start, end;
int status;
static short SystemRunningFlag=1;
int saIntst=1;
int	data[10];
int	*pData=data;
int i;

double	sfp_rx_pwr[6];

	/* tick measurement */
	start = xget_clock_ticks();			// Debug timing
	sleep(1000);
	end   = xget_clock_ticks();			// Debug timing	
	xil_printf("\r\nKernel Timer One second is %d ticks \r\n", end-start);

	//boot timestamp from evg
	pSrvTxDdrOffAddr->data[BOOT_TIME] = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*24);
	
	while (SystemRunningFlag)
	{						
		ticks++;					// Count of times called		
		sleep(100);					// Wait 250 ms before calling tmr again  10 sec
		uBlazeWd(1);
		//dout
		//mps_plb_Reg12.dout3 = 1;
		//mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));
		//
		//mps_ext_out0(0, 1);	//Dout 0
		if ((ticks % 10) == 0)		// Every 10 secs put out '.'
		{			
			//dout
			//mps_plb_Reg12.dout3 = 0;
			//mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));

			//LED
			//mps_ext_out0(0, 0);

			uBlazeWd(0);
			//Ai: SFP rx power reading
			ReadSfpEepromData(360, pData);
			//SFP
#if 0
			for(i=0; i<6; i++) {
				sfp_rx_pwr[i] = (double)(pData[i]*0.1);
				_Float2DWordReg(DFE_SFP1_RX_POWER+i,   (float)sfp_rx_pwr[i] );
			//	printf("sfp %d = %.1f (uW)\r\n", i, sfp_rx_pwr[i]);
			}
#endif
			pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+11] = pData[0];	//epics 196
			pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+12] = pData[1];
			pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+13] = pData[2];
			pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+14] = pData[3];
			pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+15] = pData[4];
			pSrvTxDdrOffAddr->data[DB_FOFB_OFFSET+16] = pData[5];
			//
			SysMonADC_Read2();
			sleep(10);

			//test();

		}

		//saIntst = Get_uInt32_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR+ (24*4))  & 0xFFFF;	//ExtDin_pin
		//xil_printf("DIN = 0x%x\r\n", saIntst);
		
	}  /* End of while */
}

