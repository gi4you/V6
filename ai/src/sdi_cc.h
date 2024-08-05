/*
 * sdi_cc.h
 *
 *	SDI CC Link Setup and monitoring
 *
 *
 */

#ifndef SDI_CC_H
#define SDI_CC_H

/* Definitions for peripheral SDI2_0 */
#include "xparameters.h"
//
#define	SDI2CC_BASE			XPAR_SDI2_CC_BASEADDR
#define	SDI2CC_PACKET_MASK	XPAR_SDI2_CC_MEM0_BASEADDR
#define	SDI2CC_DATA			XPAR_SDI2_CC_MEM1_BASEADDR


/*
#define	SDI_DATA_BASE									XPAR_SDI_DATA_PLBW_CC_BASEADDR
#define XPAR_SDI_DATA_PLBW_CC_MEMMAP_SOFT_CTRL 			SDI_DATA_BASE+0x000800
#define XPAR_SDI_DATA_PLBW_CC_MEMMAP_TRIG_SEL 			SDI_DATA_BASE+0x000804
#define XPAR_SDI_DATA_PLBW_CC_MEMMAP_Y_OFFSET 			SDI_DATA_BASE+0x000808
#define XPAR_SDI_DATA_PLBW_CC_MEMMAP_X_OFFSET 			SDI_DATA_BASE+0x00080C
#define XPAR_SDI_DATA_PLBW_CC_MEMMAP_SDI_CALADDR 		SDI_DATA_BASE+0x000810
#define XPAR_SDI_DATA_PLBW_CC_MEMMAP_FIFO_ENABLE 		SDI_DATA_BASE+0x000814
#define XPAR_SDI_DATA_PLBW_CC_MEMMAP_FIFO_Y 				SDI_DATA_BASE+0x000400
#define XPAR_SDI_DATA_PLBW_CC_MEMMAP_FIFO_Y_PERCENTFULL 	SDI_DATA_BASE+0x000408
#define XPAR_SDI_DATA_PLBW_CC_MEMMAP_FIFO_Y_EMPTY 			SDI_DATA_BASE+0x000410
#define XPAR_SDI_DATA_PLBW_CC_MEMMAP_FIFO_X 				SDI_DATA_BASE+0x000404
#define XPAR_SDI_DATA_PLBW_CC_MEMMAP_FIFO_X_PERCENTFULL 	SDI_DATA_BASE+0x00040C
#define XPAR_SDI_DATA_PLBW_CC_MEMMAP_FIFO_X_EMPTY 			SDI_DATA_BASE+0x000414
*/

#define	MAX_CELL_NUMBER			30

void	CC_Sdi_Reset();
void	CcRemoteNode_Mask(int a, Xuint32 mask_set);
void	CC_Sdi_InputDataMode( int a, int Mode );
void	CC_Sdi_ID_Set(int a, int CellNo);
void	CC_Sdi_DataStartAddressForDDR(int a, int CellNo);

void	cc_trigger_output(int a, int mode);
void	cc_trigger_delay(int a, int dly);
void	SdiCCLink_dataSet( unsigned int data);
void	SdiCCLink_setup (unsigned int CellAddress, unsigned int packet_len, int NumMaxLink);
void	CcRemoteNode_Mask(int a, Xuint32 mask_set);
void 	SDICC_DataFifo2DDR(int len);

void	cmd_Console_SdiCCLink_MaskSetup (int argc, char *argv[]);
void	cmd_Console_SDICC_NOfBPMs(int argc, char *argv[]);
void	cmd_Console_SDICC_DpRamReadStartAddress(int argc, char *argv[]);
void	cmd_SDICC_LinkDirectionCtrl(int argc, char *argv[]);
void	cmd_SDICC_NpiEnableWidth(int argc, char *argv[]);
void	cmd_SDICC_DpRamReadModeCtrl(int argc, char *argv[]);
void	cmd_SDICC_DpRamReadWidthCtrl(int argc, char *argv[]);
void	cmd_SDICC_NpiTrigger();
void 	CcSdiLinkStatusMonitoring();
void 	cmd_Console_SDI_CC_ID_Setting(int argc, char *argv[]);
void 	cmd_Console_SDI_CC_SDI_Input(int argc, char *argv[]);

void 	cmd_Console_SDI_CC_CalStartTriggerDelay(int argc, char *argv[]);
void 	cmd_Console_SDI_CC_CalStartTriggerMode(int argc, char *argv[]);
void	cc_epics_Reset(int a, int data);
void	cc_epics_NpiTrig(int a, int data);
void	cc_epics_NpiWidth(int a, int data);
void	cc_epics_LinkDir(int a, int data);
#endif
