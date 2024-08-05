/*
 * sdi_cc.c
 *
 *	SDI for Cell Controller Link Setup and monitoring
 *
 *
 */

#include "xio.h"
#include "xbasic_types.h"
#include <stdio.h>

#include "xparameters.h"  
#include "fpga_system.h"
#include "sdi_cc.h"
#include "sys_eeprom.h"
#include "cmd_modbus.h"


#define	EEPROM_MASK		0

//
//Cell controller local packet size is 26+2(head,crc)
//
#define	MAX_LOCAL_BPM_NODE		13		//6 bpm, 1 cc each bpm has two data
#define	CC_PACKET_LENGTH 		MAX_LOCAL_BPM_NODE*2 + 2	//Data Length + Head(1) + CRC(1)


// SlaveReg 4 for Mask Controls
typedef struct {
	Xuint32 addr    : 10;		//[0:9]
	Xuint32 en 	    : 1;		//[10]
	Xuint32 m 		: 1;		//[11]
	Xuint32 space   : 4;
	Xuint32 ReHeadAddr : 16;
}SDI_CC_MASK;
SDI_CC_MASK 	O_SDI_CC_MASK;

void SDI_CC_Mask(Xuint32 mask)
{
	*(volatile unsigned int*)(SDI2CC_BASE +  (4 * 4) ) = mask;
}

typedef struct {
	Xuint32 delay    : 16;		//[0:15]
	Xuint32 width 	    : 8;		//[16:23]
	Xuint32 space 		: 8;
}cc_slv_reg26;
cc_slv_reg26 	cc_slv_reg26_MASK;

void SDI_CC_Reg26_Mask(Xuint32 mask)
{
	*(volatile unsigned int*)(SDI2CC_BASE +  (26 * 4) ) = mask;
}


//////////////////////////////
// 4/10/2013
// for DPRAM max read length control
typedef struct {
	Xuint32 dpram64bit_max_len  : 16;		//slv_reg6[0:15]
	Xuint32 dpram32bit_max_len  : 16;		//slv_reg6[16:31]
}cc_slv_reg6;
cc_slv_reg6 	CC_DPRAM_RLEN_CTRL;

void SDI_CC_Reg6_mask(Xuint32 mask)
{
	*(volatile unsigned int*)(SDI2CC_BASE +  (6 * 4) ) = mask;
}


/*
 *  BpmAddres:
 *  	1, 2, 3, 4, 5, 6, 7, 8
 *	Packet length:   4 or 6
 *	NumMaxLink   : Number of local BPM
 *
 */
#define	DWORD	4
void	SdiCCLink_setup (unsigned int CellAddress, unsigned int packet_len, int NumMaxLink)
{
    int i;
	int	cc_offset_address;
	unsigned char	rbuf[16];
    int	ee_addr;    
    unsigned int MaskRead;
	int		CellNo;
	
#if 0    
    dfe_EE_Ctrl		*dfe_EE_Ctrl_R;
    dfe_EE_Ctrl_R = (dfe_EE_Ctrl *)rbuf;


    ee_addr = EEPROM_USER_0_OFFSET_ADDRESS+(2*4);
    if(0 != EE_Read( ee_addr, &rbuf, 4) ) {
		xil_printf("EERPM read error  %d\r\n", ee_addr);
		return -1;
	}

    
    CellAddress = (unsigned int) (dfe_EE_Ctrl_R->data);	//eeprom data
    
    ee_addr = EEPROM_USER_0_OFFSET_ADDRESS+(6*4);
    if(0 != EE_Read( ee_addr, &rbuf, 4) ) {
		xil_printf("EERPM read error  %d\r\n", ee_addr);
		return -1;
	}
    MaskRead = (unsigned int) (dfe_EE_Ctrl_R->data);
#endif
    
    //read eeprom data, It's from DDR3, when boot time copied EEPROM to DDR3
	CellNo = ee_addr  = DFE_CONTROL_REGS->sysInfo[2]-1;	//Cell Number
	MaskRead = DFE_CONTROL_REGS->sysInfo[6];	//Mask
	cc_offset_address = CellNo * (MAX_LOCAL_BPM_NODE*2);	// 0 , 2, 4, 6, n,  Cell 1 (n+2)
	
	xil_printf("Eeprom Remote CC Device ID = %d\r\n", cc_offset_address);	    
	xil_printf("\r\n*** Remote Cell Node Mask= 0x%x ***\r\n", MaskRead );
		
    
    *(volatile unsigned int*)(SDI2CC_BASE+0) 		= cc_offset_address;   // Reg 0: bpm address, start address of packet payload
    *(volatile unsigned int*)(SDI2CC_BASE+4) 		= CC_PACKET_LENGTH;   	// Reg 1: packet length ( x,y 4 words + 2 )
    //first Cell controller is 0 , 2nd cell CEll+BPM
    *(volatile unsigned int*)(SDI2CC_BASE+8) 	= 0;   	// Reg 2: cell controller, default is 1    .cellController(slv_reg2) 1 bit,
    *(volatile unsigned int*)(SDI2CC_BASE+0xc) 	= 0x1;     	// Reg 3: number of local packets			.numLocalPackets(slv_reg3)8 bit,
    *(volatile unsigned int*)(SDI2CC_BASE+0x10) = cc_offset_address;    	// Reg 4: stop address, same as target address for BPM
	//
	*(volatile unsigned int*)(SDI2CC_BASE+(7*4) ) = 0;	//CRC Clear
	*(volatile unsigned int*)(SDI2CC_BASE+(8*4) ) = 5;	//Local Delay from Trigger

	//Default Head address
	O_SDI_CC_MASK.ReHeadAddr = cc_offset_address;
	SDI_CC_Mask( *((Xuint32 *) & O_SDI_CC_MASK) );
	//

    // SDI mask setup using memory map, but without DDR memory it make error , MB hang
	/*
    for (i=0; i < 1024; i++) {
    	*(volatile unsigned int*)(SDI2CC_PACKET_MASK + (i * DWORD)) = 0xffffffff; 
    }
	*/
	

    Xuint32 addr, tmp, slv_reg4;
    for(addr=0; addr < 1024; addr++) {
    	O_SDI_CC_MASK.en = 1;
    	O_SDI_CC_MASK.m = 1;
    	O_SDI_CC_MASK.addr = addr;
    	SDI_CC_Mask( *((Xuint32 *) & O_SDI_CC_MASK) );
        delay_1ms(1);
        //xil_printf("addr=%x.............\r\n", addr );
    }
    O_SDI_CC_MASK.en = 0;
    SDI_CC_Mask( *((Xuint32 *) & O_SDI_CC_MASK) );
    
    CcRemoteNode_Mask(0, MaskRead);

    
#if(EEPROM_MASK)
    //SET MASK BIT, Same as Start Address
    // BPM LOCAL : 0, 2, 4, 6, 8  only 2 datas (x,y)
    // Cell SDI  Cell Number(start 0)  * Number of BPM (7)
    //   0, 7*1,  7*2
    //
    
    ee_addr = 100+(4 * 6);
    if(XST_SUCCESS != EE_Read( ee_addr, &rbuf, 4) ) {
		xil_printf("EERPM read error  %d\r\n", ee_addr);
		return -1;
	}
	xil_printf("\r\n*** Remote Cell Node Mask= 0x%x ***\r\n", dfe_EE_Ctrl_R->data);

    int	Maddr;
	int mask_buf[32];
	Xuint32 mask; //=1 << 31;
	mask = 1 << 31;	
	
	for(CellNo=1; CellNo<=32; CellNo++) {
		mask_buf[CellNo-1] = dfe_EE_Ctrl_R->data & mask ? '1' : '0';
		outbyte(mask_buf[CellNo-1]);
		dfe_EE_Ctrl_R->data <<= 1;
	}
	xil_printf("\r\n");
	
    for(i=0; i<=31; i++)
    {	    
		if(mask_buf[i] == '1') {
			CellNo = (i-31)*-1;
    		Maddr=(MAX_LOCAL_BPM_NODE*2) * CellNo;
    		*(volatile unsigned int*)(SDI2CC_PACKET_MASK + ( Maddr * DWORD))  = 0x0;
    		xil_printf("Cell_Number=%2d, Masking Address = %4d\r\n", CellNo,  Maddr);
		}
    }

#endif

    //Reg 9
    *(volatile unsigned int*)(SDI2CC_BASE+(9*DWORD) ) = 1;			//0: cw write to DPRAM,  1: CW and CCW dual data copy to DPRAM
    
    //Reg 5 : Dpram Start address 0, same a
    //if(CellNo < 0 || CellNo > 31) CellNo = 0;
    //*(volatile unsigned int*)(SDI2CC_BASE+  (5*DWORD) ) = CellNo;	//default start 0
    CC_Sdi_DataStartAddressForDDR(0, CellNo+1);
    
	// Reg 6 DPRAM Output Length
	// Data output is 64-bit(x,y), so Length is same as bpm numbers
    //*(volatile unsigned int*)(SDI2CC_BASE+  (6*DWORD) ) = (MAX_LOCAL_BPM_NODE*MAX_CELL_NUMBER) << 16; 	//Length of DPRAM, 30 * 7
    //*(volatile unsigned int*)(SDI2CC_BASE+  (6*DWORD) ) = MAX_LOCAL_BPM_NODE << 16;		//One CELL 7

	CC_DPRAM_RLEN_CTRL.dpram64bit_max_len = MAX_LOCAL_BPM_NODE;		// FPGA test result
	//CC_DPRAM_RLEN_CTRL.dpram32bit_max_len = 13;					//Not used for CC SDI because interface with FOFB
	SDI_CC_Reg6_mask( *((Xuint32 *) & CC_DPRAM_RLEN_CTRL) );

	//Reg26
	//.myDelay( {16'd0, slv_reg26[0:15] } ),
	//.myWidth( {24'd0, slv_reg26[16:23] } ),
	cc_slv_reg26_MASK.delay = 500;	// trigger delay same as Cell controller LOCAL SDI delay
	cc_slv_reg26_MASK.width = 20; //5;
	SDI_CC_Reg26_Mask( *((Xuint32 *) & cc_slv_reg26_MASK) );


    //*(volatile unsigned int*)(SDI2CC_BASE+  (26*DWORD) ) = 500;	//DPRAM start delay 20 us from trigger
    *(volatile unsigned int*)(SDI2CC_BASE+  (27*DWORD) ) = 0x1;		// 1: User Delay, 0:CalStart delay
    *(volatile unsigned int*)(SDI2CC_BASE+  (30*DWORD)) = 0x00000000;

    //assign LocalDataValid_i = (slv_reg25 == 1'b1) ? CWLocalDataValid :	bpmTestDataValid;
    *(volatile unsigned int*)(SDI2CC_BASE+  (25*DWORD)) = 0;	/* 1:Local BPM Data or 0:bpm test Data */
    /* NPI strobe control */
    *(volatile unsigned int*)(SDI2CC_BASE+  (28*DWORD)) = 1;	/* 1: User control, 0: default */
    //
	*(volatile unsigned int*)(SDI2CC_BASE+  (29*DWORD)) = 1;	/* DPRAM output read by FOFB module */
    *(volatile unsigned int*)(SDI2CC_BASE+  (31*DWORD)) =  62500000 * 2;	//1 sec @ 62.5 MHz


}



//SDI_CC_NODE_MASK_SET
void	CcRemoteNode_Mask(int a, Xuint32 mask_set)
{
    unsigned int	i, CellNo, Maddr;
	unsigned int mask_buf[32];
	Xuint32 mask; //=1 << 31;

	//clear
    Xuint32 addr, tmp, slv_reg4;

    mask_set |= 0x40000000;	//Cell 31 enabled
    xil_printf("\r\n*** Remote Cell Node Mask= 0x%x ***\r\n", mask_set);

    for(addr=0; addr < 1024; addr++) {
    	O_SDI_CC_MASK.en = 1;
    	O_SDI_CC_MASK.m = 1;
    	O_SDI_CC_MASK.addr = addr;
    	SDI_CC_Mask( *((Xuint32 *) & O_SDI_CC_MASK) );
        delay_1ms(1);
        //xil_printf("addr=%x.............\r\n", addr );
    }
    O_SDI_CC_MASK.en = 0;
    SDI_CC_Mask( *((Xuint32 *) & O_SDI_CC_MASK) );

	mask = 1 << 31;

	for(CellNo=1; CellNo<=32; CellNo++) {
		mask_buf[CellNo-1] = mask_set & mask ? '1' : '0';
		outbyte(mask_buf[CellNo-1]);
		mask_set <<= 1;
		if( (CellNo % 8) == 0 && CellNo < 30 ) outbyte(':');
	}
	xil_printf("\r\n");


    for(i=0; i<31; i++)
    {
		if(mask_buf[i] == '1') {
			CellNo = (i-31)*-1;
    		Maddr=(MAX_LOCAL_BPM_NODE*2) * CellNo ;

        	O_SDI_CC_MASK.en = 1;
        	O_SDI_CC_MASK.m = 0;	//masking
        	O_SDI_CC_MASK.addr = Maddr;	//address
        	SDI_CC_Mask( *((Xuint32 *) & O_SDI_CC_MASK) );
            delay_1ms(1);
            xil_printf("Cell No =%2d, Masking Address = %4d\r\n", CellNo+1,  Maddr);
        }
    }

	O_SDI_CC_MASK.en = 0;
	SDI_CC_Mask( *((Xuint32 *) & O_SDI_CC_MASK) );
}



//SDI_CC_DPRAM64_DATA_START_ADDR_2_DDR3
void	CC_Sdi_DataStartAddressForDDR(int a, int CellNo)
{
	int addr, reg_set;
	
	if (CellNo > 31 || CellNo <= 0 ) CellNo = 1;	//max cell is 31
	addr = (CellNo-1) * MAX_LOCAL_BPM_NODE;

	reg_set = (addr << 16);		//.MaxLenReg( slv_reg5[0:15]   ,  [0:15]  --- Microblaze[31:16]
	*(volatile unsigned int*)(SDI2CC_BASE +  (5*4) ) = reg_set;		//Reg5
	xil_printf("DPRAM offset Star Address = %d , %x \r\n", addr, addr);
}

	
//SDI_CC_NPI_TRIGGER
//Trigger
void	cmd_SDICC_NpiTrigger()
{
	*(volatile unsigned int*)(SDI2CC_BASE+  (30*4) ) 	= 1;		//Reg30
	xil_printf("--> CC NPI Trigger set\r\n");
		
	*(volatile unsigned int*)(SDI2CC_BASE+  (30*4) ) 	= 0;
	xil_printf("--> CC NPI Trigger zero\r\n");
}


//Enable
void	cmd_SDICC_NpiEnableWidth(int argc, char *argv[])
{
	Xuint32 len, tmp;
	tmp = strtol(argv[1], (char **)0, 10);
	if(tmp >= 50) tmp = 10;
	else if(tmp <= 0) tmp = 1;
	//8 ns step
	*(volatile unsigned int*)(SDI2CC_BASE+  (31*4) ) = tmp * 62500000;	//Reg31
	xil_printf("Npi Enable Width = %d (sec) \r\n", tmp);
}


/*
 * SDI_CC_RESET
 * reset FIFO and crc count
 */
void	CC_Sdi_Reset()
{
	int i;

	portNOP();
	xil_printf("CC SDI Regster Reset...\r\n");
	*(volatile unsigned int*)(SDI2CC_BASE+(7*4) ) = 1;
	*(volatile unsigned int*)(SDI2CC_BASE+(7*4) ) = 0;
}

//BPM Node ID setting
void	CC_Sdi_ID_Set(int a, int CellNo)
{
int addr, Start_Addr;
	
	addr = CellNo - 1;
	Start_Addr = addr * (MAX_LOCAL_BPM_NODE*2) ;				// 0 , 2, 4, 6, n,  Cell 1 (n+2)
	*(volatile unsigned int*)(SDI2CC_BASE+0)    = Start_Addr;   // Reg 0: bpm address, start address of packet payload
	*(volatile unsigned int*)(SDI2CC_BASE+0x10) = Start_Addr;
	
	xil_printf("Cell %d, Mem start address = %d\r\n", CellNo, Start_Addr);
}	


//SDI_CC_DATA_INPUT_MODE
//1: Local BPM, 0:Test Data
void	CC_Sdi_InputDataMode( int a, int Mode )
{

	 xil_printf("March 19, 2014 disabled form SDI_2 IP Core...\r\n\r\n");

	*(volatile unsigned int*)(SDI2CC_BASE+  (25*DWORD)) = Mode & 0x1;	
	if(Mode == 1) xil_printf("BPM Data Input, Now Operation Mode\r\n");
	else xil_printf("Test ramp Input, Only TEST Mode\r\n");
		
}


//SDI_CC_USER_TRIG_DLY
void	cc_trigger_delay(int a, int dly)
{
	if(dly > 5000) {
		xil_printf("delay is tow larger...\r\n");
	}
	else {
		cc_slv_reg26_MASK.delay = dly;
		SDI_CC_Reg26_Mask( *((Xuint32 *) & cc_slv_reg26_MASK) );
		xil_printf("delay is %d (ns)\r\n", dly * 20);
	}
}


//SDI_CC_TRIGGER_OUTPUT
void	cc_trigger_output(int a, int mode)
{
	*(volatile unsigned int*)(SDI2CC_BASE+  (27*DWORD) ) = mode & 0x1;		// 1: User Delay, 0:CalStart delay
}


/*
 *
 */
void CcSdiLinkStatusMonitoring()
{
	int LinkStatus, CW_CRCErrorCount, CCW_CRCErrorCount;
	int	CW_LocalLoopbackDataErrorCnt,CCW_LocalLoopbackDataErrorCnt;
	unsigned int CW_LocalTxRxFrameErrorVal,CCW_LocalTxRxFrameErrorVal;
	unsigned int	CwRemoteTimeoutCnt, CCwRemoteTimeoutCnt, CWSDILinkBufferLen, CCWSDILinkBufferLen;
	unsigned int CW_ReLocalHead_wd_timeout_cnt, CCW_ReLocalHead_wd_timeout_cnt, WD_LocalTxPacketTimeout_cnt, DRAM_ReadStart_WD_cnt;
	unsigned int gtx1_loss_lock, gtx2_loss_lock;
	unsigned int  evr_trig_wd_timeout_cnt;

	int sfp1, sfp2;
	unsigned int trigErrCnt;

	CW_CRCErrorCount = Get_uInt32_DDR2(XPAR_SDI2_CC_BASEADDR+(12*4));
	CCW_CRCErrorCount = Get_uInt32_DDR2(XPAR_SDI2_CC_BASEADDR+(13*4));
	LinkStatus = (Get_I_DDR2(XPAR_SDI2_CC_BASEADDR+(14*4)) >> 24)&0xFF;

	trigErrCnt = Get_uInt32_DDR2(XPAR_SDI2_CC_BASEADDR+(15*4));
	CW_LocalLoopbackDataErrorCnt    = Get_uInt32_DDR2(XPAR_SDI2_CC_BASEADDR+(16*4));
	CCW_LocalLoopbackDataErrorCnt   = Get_uInt32_DDR2(XPAR_SDI2_CC_BASEADDR+(17*4));
	CW_LocalTxRxFrameErrorVal       = Get_uInt32_DDR2(XPAR_SDI2_CC_BASEADDR+(18*4));
	CCW_LocalTxRxFrameErrorVal      = Get_uInt32_DDR2(XPAR_SDI2_CC_BASEADDR+(19*4));
	CwRemoteTimeoutCnt              = Get_uInt32_DDR2(XPAR_SDI2_CC_BASEADDR+(20*4));
	CCwRemoteTimeoutCnt             = Get_uInt32_DDR2(XPAR_SDI2_CC_BASEADDR+(21*4));
	//
	CW_ReLocalHead_wd_timeout_cnt  	= Get_uInt32_DDR2(XPAR_SDI2_CC_BASEADDR+(22*4));
	CCW_ReLocalHead_wd_timeout_cnt  = Get_uInt32_DDR2(XPAR_SDI2_CC_BASEADDR+(23*4));
	WD_LocalTxPacketTimeout_cnt     = Get_uInt32_DDR2(XPAR_SDI2_CC_BASEADDR+(24*4));
	DRAM_ReadStart_WD_cnt           = Get_uInt32_DDR2(XPAR_SDI2_CC_BASEADDR+(25*4));
	//evr time out
	evr_trig_wd_timeout_cnt  	    = Get_uInt32_DDR2(XPAR_SDI2_CC_BASEADDR+(11*4));
	gtx1_loss_lock  	            = Get_uInt32_DDR2(XPAR_SDI2_CC_BASEADDR+(4*4));


	xil_printf("\r\n###---- CC LINK INFO ---------------------\r\n\r\n");
	int CellID = Get_uInt32_DDR2 ( DDR2_EEPROM_SYSDATA_BASE+(3*4));
	int CC_Mask = Get_uInt32_DDR2 ( DDR2_EEPROM_SYSDATA_BASE+(4*4));
	//DFE_CONTROL_REGS->sysInfo[6]

	xil_printf("Cell No : %d , Mask : 0x%x \r\n", CellID,  CC_Mask);

	xil_printf("CC Link = 0x%x\r\n", LinkStatus);
	sfp1 = LinkStatus & 0x04;
	sfp2 = LinkStatus & 0x10;
	xil_printf("SFP6=%d,  SFP5=%d\r\n\r\n", sfp1>>2, sfp2>>4);
	xil_printf("EVR Trig Count                    = %d\r\n", trigErrCnt);
	xil_printf("CW_CRCErrorCount                  = %d\r\n", CW_CRCErrorCount);
	xil_printf("CCW_CRCErrorCount                 = %d\r\n", CCW_CRCErrorCount);
	xil_printf("EVR_trig_wd_timeout_cnt           = %d\r\n", evr_trig_wd_timeout_cnt);
	xil_printf("CW_LocalLoopbackDataErrorCnt      = %d\r\n", CW_LocalLoopbackDataErrorCnt);
	xil_printf("CCW_LocalLoopbackDataErrorCnt     = %d\r\n", CCW_LocalLoopbackDataErrorCnt);
	xil_printf("CW_MaskStartCalTimeout            = %d\r\n", CW_LocalTxRxFrameErrorVal);
	xil_printf("CCW_MaskStartCalTimeout           = %d\r\n", CCW_LocalTxRxFrameErrorVal);
	xil_printf("CwRemoteTimeoutCnt                = %d\r\n", CwRemoteTimeoutCnt);
	xil_printf("CCwRemoteTimeoutCnt               = %d\r\n", CCwRemoteTimeoutCnt);
	//
	xil_printf("CW_Local Rcv Head WD count        = %d\r\n", CW_ReLocalHead_wd_timeout_cnt);
	xil_printf("CCW_Local Rcv Head WD count       = %d\r\n", CCW_ReLocalHead_wd_timeout_cnt);
	xil_printf("CW_LocalTx WD count Reg24[15:0]   = %d\r\n", WD_LocalTxPacketTimeout_cnt& 0xFFFF);
	xil_printf("CCW_LocalTx WD count Reg24[31:16] = %d\r\n", (WD_LocalTxPacketTimeout_cnt >> 16) & 0xFFFF);
	xil_printf("Usr DRAM Read Start (Reg25)       = %d\r\n", DRAM_ReadStart_WD_cnt);

	xil_printf("CW_GTX Loss                       = %d\r\n", gtx1_loss_lock & 0xFFFF);
	xil_printf("CCW_GTX_Loss                      = %d\r\n", (gtx1_loss_lock>>16) & 0xFFFF);
}


//
void	cmd_Console_SDICC_FIFO_RST(int argc, char *argv[])
{
	CC_Sdi_Reset();
}


/*
 *  SDI_CC_DPRAM64_DATA_2_NPI_LEN
 *
 * 	DPRAM Port B Read for NPI Write
 *	Output format 64-bit (X,Y)
 *
 */
void	cmd_Console_SDICC_NOfBPMs(int argc, char *argv[])
{
	Xuint32 len, tmp;
	tmp = strtol(argv[1], (char **)0, 10);

	tmp *= MAX_LOCAL_BPM_NODE;	/* each cell has 6 mpm + cell sdi  so total 7 */

	//len = (tmp << 24);    //.MaxLenReg( slv_reg6[0:7]   ,  [0:7]  --- Microblaze[31:24]
	len = (tmp << 16);		//.MaxLenReg( slv_reg6[0:15]   ,  [0:15]  --- Microblaze[31:16]
	*(volatile unsigned int*)(SDI2CC_BASE+  (6*4) ) 	= len;		//Reg6
	xil_printf("DPRMA (64bit) Read Len = %d,  set = 0x%x\n", tmp, len);
}


/*
 *	Mask setting by manually
 *
 */
void	cmd_Console_SdiCCLink_MaskSetup (int argc, char *argv[])
{
    unsigned int	i, CellNo, Maddr;
	unsigned int 	mask_buf[32];
	Xuint32 mask; 
	unsigned char	rbuf[16];
    int	ee_addr;
    static char UserResponse[64]; /* User input for questions */
    long	dat;
/*
    dfe_EE_Ctrl		*dfe_EE_Ctrl_R;
    dfe_EE_Ctrl_R = (dfe_EE_Ctrl *)rbuf;
*/

    xil_printf(("\r\nEnter CC Mask data Hexdecimal : 0x"));
    while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
    dat = strtol(UserResponse, (char **)0, 16);

    CcRemoteNode_Mask(0, dat);

#if 0
/*
    ee_addr = 100+(4 * 6);
    if(XST_SUCCESS != EE_Read( ee_addr, &rbuf, 4) ) {
		xil_printf("EERPM read error  %d\r\n", ee_addr);
		return -1;
	}
	xil_printf("\r\n*** Remote Cell Node Mask= 0x%x ***\r\n", dfe_EE_Ctrl_R->data);
	dat = dfe_EE_Ctrl_R->data;
*/


    // SDI mask setup clear
    /*
    xil_printf("Clearing Mask to 0xFFFFFFFF\r\n");
    for (i=0; i < 1024; i++) {
    	*(volatile unsigned int*)(SDI2CC_PACKET_MASK + (i * DWORD)) = 0xffffffff;
    }
*/
	//clear
    Xuint32 addr, tmp, slv_reg4;
    for(addr=0; addr < 1024; addr++) {
    	O_SDI_CC_MASK.en = 1;
    	O_SDI_CC_MASK.m = 1;
    	O_SDI_CC_MASK.addr = addr;
    	SDI_CC_Mask( *((Xuint32 *) & O_SDI_CC_MASK) );
        delay_1ms(1);
        //xil_printf("addr=%x.............\r\n", addr );
    }
    O_SDI_CC_MASK.en = 0;
    SDI_CC_Mask( *((Xuint32 *) & O_SDI_CC_MASK) );

	mask = 1 << 31;

	for(CellNo=1; CellNo<=32; CellNo++) {
		mask_buf[CellNo-1] = dat & mask ? '1' : '0';
		outbyte(mask_buf[CellNo-1]);
		dat <<= 1;
		if( (CellNo % 8) == 0 && CellNo < 30 ) outbyte(':');
	}
	xil_printf("\r\n");

	/*
    for(i=0; i<=31; i++)
    {
		if(mask_buf[i] == '1') {
			CellNo = (i-31)*-1;
    		Maddr=(MAX_LOCAL_BPM_NODE*2) * CellNo;
    		*(volatile unsigned int*)(SDI2CC_PACKET_MASK + ( Maddr * DWORD))  = 0x0;
    		xil_printf("Cell_Number=%2d, Masking Address = %4d\r\n", CellNo,  Maddr);
		}
    }
*/
    for(i=0; i<=31; i++)
    {
		if(mask_buf[i] == '1') {
			CellNo = (i-31)*-1;
    		Maddr=(MAX_LOCAL_BPM_NODE*2) * CellNo ;

        	O_SDI_CC_MASK.en = 1;
        	O_SDI_CC_MASK.m = 0;	//masking
        	O_SDI_CC_MASK.addr = Maddr;	//address
        	SDI_CC_Mask( *((Xuint32 *) & O_SDI_CC_MASK) );
            delay_1ms(1);
            xil_printf("unsigned=%2d, Masking Address = %4d\r\n", CellNo,  Maddr);
        }
    }

	O_SDI_CC_MASK.en = 0;
	SDI_CC_Mask( *((Xuint32 *) & O_SDI_CC_MASK) );
#endif

}


/*
 * command : cd
 */
void 	cmd_Console_SDI_CC_SDI_Input(int argc, char *argv[])
{
	Xuint32 cc_sdi, BpmNo;

	cc_sdi = strtol(argv[1], (char **)0, 10);
	if(cc_sdi == 1)
		xil_printf("CC SDI input is Local BPM Data, Normal operation\r\n");
	else {
		cc_sdi = 0;
		xil_printf("CC SDI input is Count Test Data, Test Mode\r\n");
	}
	//1: Local BPM, 0:Test Data
	CC_Sdi_InputDataMode(0, cc_sdi);
}


/*
 *	"ctd"
 */
void 	cmd_Console_SDI_CC_CalStartTriggerDelay(int argc, char *argv[])
{
	Xuint32 dly;

	dly = strtol(argv[1], (char **)0, 10);
	if(dly <10) dly = 10;
	xil_printf("Delay = %d (ns)\r\n", dly * 20);
    //*(volatile unsigned int*)(SDI2CC_BASE+  (26*DWORD) ) = dly;		//DPRAM start delay

	cc_trigger_delay(0, dly);

} 

/*
 * Node ID is same as Cell Number
 *
 */
void 	cmd_Console_SDI_CC_ID_Setting(int argc, char *argv[])
{
	Xuint32 len, addr, CellNo;
	int Start_Addr;
    static char UserResponse[64]; /* User input for questions */

    xil_printf(("\r\nEnter Cell Number [1-30] : "));
    while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
    CellNo = strtol(UserResponse, (char **)0, 10);
    if(CellNo <= 0 || CellNo > 32 ) xil_printf("Cell number error\r\n");
    else {
	    CC_Sdi_ID_Set(0, CellNo);
	
    }
}
	
/*
 *	SDI_CC_TRIGGER_OUTPUT
 *	"ctm"
 */
void 	cmd_Console_SDI_CC_CalStartTriggerMode(int argc, char *argv[])
{
	Xuint32 mode;

	mode = strtol(argv[1], (char **)0, 10);
	if(mode == 1) xil_printf("Start Cal signal is User Delay Mode\r\n");
	else xil_printf("Start Cal signal is Masked Output Mode\r\n");
    //
	cc_trigger_output(0, mode & 0x1);
    //*(volatile unsigned int*)(SDI2CC_BASE+  (27*DWORD) ) = mode & 0x1;		// 1: User Delay, 0:CalStart delay
}    

/*
 *
 */
void	cmd_Console_SDICC_DpRamReadStartAddress(int argc, char *argv[])
{
	Xuint32 len, CellNo, addr;

    static char UserResponse[64]; /* User input for questions */

    xil_printf(("\r\nEnter Cell Number [1: 31] : "));
    while(!GetUserInput(NULL, UserResponse, sizeof(UserResponse)-1));
    CellNo = strtol(UserResponse, (char **)0, 10);

	CC_Sdi_DataStartAddressForDDR(0, CellNo);
#if 0	
	if (CellNo > 31) CellNo = 0;	//max cell is 31
	addr = (CellNo-1) * MAX_LOCAL_BPM_NODE;

	len = (addr << 16);		//.MaxLenReg( slv_reg5[0:15]   ,  [0:15]  --- Microblaze[31:16]
	*(volatile unsigned int*)(SDI2CC_BASE +  (5*4) ) 	= len;		//Reg6
	xil_printf("DPRAM offset Star Address = %d , 0x%8x \r\n", addr, len, len);
#endif
	
}

/*
 *
 */
void	cmd_SDICC_LinkDirectionCtrl(int argc, char *argv[])
{
	Xuint32 len, tmp;
	tmp = strtol(argv[1], (char **)0, 10);
	
	if(tmp > 0) tmp = 0x00000001;
	*(volatile unsigned int*)(SDI2CC_BASE+  (9*4) ) = tmp;		//Reg9
	xil_printf("Link direction = 0x%8x\r\n", tmp);
}

void	cmd_SDICC_DpRamReadWidthCtrl(int argc, char *argv[])
{
	Xuint32 len, tmp;
	tmp = strtol(argv[1], (char **)0, 10);
	
	if(tmp > 0) tmp = 0x00000001;
	*(volatile unsigned int*)(SDI2CC_BASE+  (28*4) ) 	= tmp;		//Reg28
	xil_printf("DpRam Read Mode = %d  (1: by use setting, 0: default 1sec) \r\n", tmp);
}

//
void	cmd_SDICC_DpRamReadModeCtrl(int argc, char *argv[])
{
	Xuint32 len, tmp;
	tmp = strtol(argv[1], (char **)0, 10);
	
	if(tmp > 0) tmp = 0x00000001;
	*(volatile unsigned int*)(SDI2CC_BASE+  (29*4) ) 	= tmp;		//Reg28
	xil_printf("DpRam Data Output Mode = %d  (1: fofb dram control, 0: internal dram control) \r\n", tmp);
}


//
void	cc_epics_Reset(int a, int data)
{
	int i;

	xil_printf("CC SDI Regster Reset...%d\r\n", data);
	*(volatile unsigned int*)(SDI2CC_BASE+(7*4) ) = data;
}

void	cc_epics_NpiTrig(int a, int data)
{
	*(volatile unsigned int*)(SDI2CC_BASE+  (30*4) ) = data;		//Reg30
	xil_printf("--> CC NPI Trigger %d\r\n", data);
}

void	cc_epics_NpiWidth(int a, int data)
{
	if(data > 50) data = 50;
	*(volatile unsigned int*)(SDI2CC_BASE+  (31*4) ) = data * 62500000;	//Reg31
}


//0: cw write to DPRAM,  1: CW and CCW dual data copy to DPRAM
void	cc_epics_LinkDir(int a, int data)
{
	*(volatile unsigned int*)(SDI2CC_BASE+  (9*4) ) = 1&data;
	xil_printf("--> CC Link direction (1:both, 0:CCW) %d\r\n", data);
}


/*
 *
 */
