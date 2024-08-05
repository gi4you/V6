/*
 * mps.c
 *
 *  Created on: Jul 2, 2013
 *      Author: kha
 */

#include "fofb.h"
#include "mps.h"

//11/07/14 added for pSrvTxDdrOffAddr->data[250] = 0;
#include "pscEpics.h"
#include "cmd_modbus.h"	//07/10/15


extern 	uBlaze_err_code;
extern	dcct_plc_wd_err_cnt;
extern 	cell_plc_wd_err_cnt;


/*
 *
c3 = 0.191498, 0.486978
c5_1 = 0.375375, 0.500000
c5_2 = 0.392773, 0.500000
c10 = 0.126135, 0.491801
c11 = 0.191498, 0.486978
c23 = 0.199880, 0.440736
c28 = 0.136240, 0.498910


Position Address generator:

k = 29;
for  addr = 0:k
    fprintf('(#Cell %2d)  ? { cc=%3d, y2=%3d, x2=%3d, y1=%3d, x1=%3d} :\n', addr+1, 24+addr*26, 15+addr*26, 14+addr*26, 13+addr*26, 12+addr*26);
end

(#Cell  1)  ? { cc= 24, y2= 15, x2= 14, y1= 13, x1= 12} :
(#Cell  2)  ? { cc= 50, y2= 41, x2= 40, y1= 39, x1= 38} :
(#Cell  3)  ? { cc= 76, y2= 67, x2= 66, y1= 65, x1= 64} :
(#Cell  4)  ? { cc=102, y2= 93, x2= 92, y1= 91, x1= 90} :
(#Cell  5)  ? { cc=128, y2=119, x2=118, y1=117, x1=116} :
(#Cell  6)  ? { cc=154, y2=145, x2=144, y1=143, x1=142} :
(#Cell  7)  ? { cc=180, y2=171, x2=170, y1=169, x1=168} :
(#Cell  8)  ? { cc=206, y2=197, x2=196, y1=195, x1=194} :
(#Cell  9)  ? { cc=232, y2=223, x2=222, y1=221, x1=220} :
(#Cell 10)  ? { cc=258, y2=249, x2=248, y1=247, x1=246} :
(#Cell 11)  ? { cc=284, y2=275, x2=274, y1=273, x1=272} :
(#Cell 12)  ? { cc=310, y2=301, x2=300, y1=299, x1=298} :
(#Cell 13)  ? { cc=336, y2=327, x2=326, y1=325, x1=324} :
(#Cell 14)  ? { cc=362, y2=353, x2=352, y1=351, x1=350} :
(#Cell 15)  ? { cc=388, y2=379, x2=378, y1=377, x1=376} :
(#Cell 16)  ? { cc=414, y2=405, x2=404, y1=403, x1=402} :
(#Cell 17)  ? { cc=440, y2=431, x2=430, y1=429, x1=428} :
(#Cell 18)  ? { cc=466, y2=457, x2=456, y1=455, x1=454} :
(#Cell 19)  ? { cc=492, y2=483, x2=482, y1=481, x1=480} :
(#Cell 20)  ? { cc=518, y2=509, x2=508, y1=507, x1=506} :
(#Cell 21)  ? { cc=544, y2=535, x2=534, y1=533, x1=532} :
(#Cell 22)  ? { cc=570, y2=561, x2=560, y1=559, x1=558} :
(#Cell 23)  ? { cc=596, y2=587, x2=586, y1=585, x1=584} :
(#Cell 24)  ? { cc=622, y2=613, x2=612, y1=611, x1=610} :
(#Cell 25)  ? { cc=648, y2=639, x2=638, y1=637, x1=636} :
(#Cell 26)  ? { cc=674, y2=665, x2=664, y1=663, x1=662} :
(#Cell 27)  ? { cc=700, y2=691, x2=690, y1=689, x1=688} :
(#Cell 28)  ? { cc=726, y2=717, x2=716, y1=715, x1=714} :
(#Cell 29)  ? { cc=752, y2=743, x2=742, y1=741, x1=740} :
(#Cell 30)  ? { cc=778, y2=769, x2=768, y1=767, x1=766} :

 */

#define	MAX_ID_NO		57

//#define	FIX_32_31		2147483647

struct AI_DB_Setting
{
	int cell_no;
    //int d_x_limit, d_y_limit, r_mm_x_limit, r_mm_y_limit, r_mrad__x_limit, r_mrad_y_limit;
	int	h1_limit, v1_limit;
	int	h2_limit, v2_limit;
	int	h3_limit, v3_limit;
	int	r_mrad__x_limit, r_mrad_y_limit;
    double	s_angle;
    double  s_offset;
    int logic;
};
const struct AI_DB_Setting tAI_DB_Setting[MAX_ID_NO] =
{	//Lgic   H1     V1      H2       V2      
    { 3,   600000, 600010, 500000, 500010, 600000, 600010,  250000, 250010, 0.191498, 0.486978, LOGIC_AIE_ID_A__RECTANGLAR  },
    //{ 3,   600000, 600010, 500000, 500010, 600000, 600010, 250000, 250010, 0.5, 0.5, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 51,  600001, 600001, 500011, 500011, 600000, 600020,  250001, 250011, 0.375375, 0.500000, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 52,  600002, 600002, 500012, 500012, 250002, 600030, 250002, 250012, 0.392773, 0.500000, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 10,  600003, 600003, 500013, 500013, 250003, 600040, 250003, 250013, 0.126135, 0.491801, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 11,  600004, 600004, 500014, 500014, 250004, 600050, 250004, 250014, 0.191498, 0.486978, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 23,  600005, 600005, 500015, 500015, 250005, 600060, 250005, 250015, 0.199880, 0.440736, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 232, 600005, 600005, 500015, 500015, 250005, 600060, 250005, 250015, 0.199880, 0.440736, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  600006, 600006, 500016, 500016, 250006, 600070, 250006, 250016, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    {  8,  600007, 600007, 500016, 500017, 250007, 600080, 250006, 250016, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 18,  600008, 600008, 500016, 500018, 250008, 600090, 250006, 250016, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    //
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    //
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    //
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    //
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    //
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    { 28,  0, 0, 0, 0, 0, 0, 0, 0, 0.136240, 0.498910, LOGIC_AIE_ID_A__RECTANGLAR  },
    //
};



struct PosId_Cc
{
	int cell_no;
	int	cc_d1, cc_d0;
	int	y3,x3;
	int	y2,x2;
	int	y1,x1;
};

const struct PosId_Cc tPosId_Cc[31] =
{
	{  0,  0,  0,   0,  0,   0,  0,   0,  0 },
	{  1,  25,  24,   17,  16,   15,  14,   13,  12 },
	{  2,  51,  50,   43,  42,   41,  40,   39,  38 },
	{  3,  77,  76,   69,  68,   67,  66,   65,  64 },
	{  4, 103, 102,   95,  94,   93,  92,   91,  90 },
	{  5, 129, 128,  121, 120,  119, 118,  117, 116 },
	{  6, 155, 154,  147, 146,  145, 144,  143, 142 },
	{  7, 181, 180,  173, 172,  171, 170,  169, 168 },
	{  8, 207, 206,  199, 198,  197, 196,  195, 194 },
	{  9, 233, 232,  225, 224,  223, 222,  221, 220 },

	{ 10, 259, 258,  251, 250,  249, 248,  247, 246 },	//
	{ 11, 285, 284,  277, 276,  275, 274,  273, 272 },	//

	{ 12, 311, 310,  303, 302,  301, 300,  299, 298 },
	{ 13, 337, 336,  329, 328,  327, 326,  325, 324 },
	{ 14, 363, 362,  355, 354,  353, 352,  351, 350 },
	{ 15, 389, 388,  381, 380,  379, 378,  377, 376 },
	{ 16, 415, 414,  407, 406,  405, 404,  403, 402 },
	{ 17, 441, 440,  433, 432,  431, 430,  429, 428 },

	{ 18, 467, 466,  459, 458,  457, 456,  455, 454 },	//

	{ 19, 493, 492,  485, 484,  483, 482,  481, 480 },
	{ 20, 519, 518,  511, 510,  509, 508,  507, 506 },
	{ 21, 545, 544,  537, 536,  535, 534,  533, 532 },
	{ 22, 571, 570,  563, 562,  561, 560,  559, 558 },

	{ 23, 597, 596,  589, 588,  587, 586,  585, 584 },	//
	{ 24, 623, 622,  615, 614,  613, 612,  611, 610 },
	{ 25, 649, 648,  641, 640,  639, 638,  637, 636 },
	{ 26, 675, 674,  667, 666,  665, 664,  663, 662 },
	{ 27, 701, 700,  693, 692,  691, 690,  689, 688 },

	{ 28, 727, 726,  719, 718,  717, 716,  715, 714 },	//
	{ 29, 753, 752,  745, 744,  743, 742,  741, 740 },
	{ 30, 779, 778,  771, 770,  769, 768,  767, 766 },
};


void mps_Reg_Set(int addr, Xuint32 value)
{
	*(volatile unsigned int*)(XPAR_FOFB_COEFF_SET_0_BASEADDR + addr*4) = value;
}


void mps_int32Reg_Set(int addr, Xint32 value)
{
	*(volatile unsigned int*)(XPAR_FOFB_COEFF_SET_0_BASEADDR + addr*4) = value;
}

// 7/1/13
void fofb_InjFifoWrite()
{
	int i, status;

	fofb_dpram.wr = 0;		//disable write
	fofb_dpram.ram_cs = 3;	//cs2[3]
	Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));

RESET:
	//1. FIFO RESET, Clear FIFO
	//plb_Reg13.FOFB_RUN = 1;
	//fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));
	//sleep(100);
	//plb_Reg13.FOFB_RUN = 0;
	//fofb_Reg13_Mask( *((Xuint32 *)&plb_Reg13 ));

	//reset fifo
	mps_plb_Reg13.id_gap_sim = 1;
	mps_plb_Reg13.FOFB_RUN = 1;
	mps_plb_Reg13.FastDelayedEnable = 0;
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
	sleep(10);
	mps_plb_Reg13.FOFB_RUN = 0;
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));

	sleep(100);
	//assign plb_r_reg22 = {28'd0,npi_wr, fifo_we_envelop, wfm_fifo_full,wfm_fifo_empty};
	status = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (22*4) );
	//if(status != 1) goto RESET;

	xil_printf("fifo_status = %x\r\n", status);
	i = 0;
	for(;;) {
		status = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (22*4)) & 0x3;	//if full
		if(status != 0x2)
		{
			Set_I_DDR2( CS2_DATA_ADDR, i*10);		//data write
			//
			fofb_dpram.wr = 1;		//write FIFO
			Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
			fofb_dpram.wr = 0;		//write
			Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
			i++;
			//xil_printf("~");
			if(i>10000) break;
		}
		else break;
	}
	xil_printf("Write -> %d\r\n", i);

	//2. Send TRIG
	fofb_dpram.user_fifo_npi_trig = 1;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));
	fofb_dpram.user_fifo_npi_trig = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram ));

	int j=0;
	int loop=0;

	for(loop=0; loop<500; loop++)
	{

		for(i=0; i<2000; i++) {
			status = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (22*4)) & 0x7;
			if(status != 0x6) {
				Set_I_DDR2( CS2_DATA_ADDR, i*10);		//data write
				//
				fofb_dpram.wr = 1;		//write FIFO
				Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
				fofb_dpram.wr = 0;		//write
				Set_I_DDR2( CS2_CTRL_ADDR, *((Xuint32 *) & fofb_dpram));
			}
			else {
				j++;
				do {
					status = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (22*4)) & 0x7;
					sleep(1);
				}while (status == 0x6);	//FULL
			}
		}
		xil_printf(".");
	}
DONE:
	xil_printf("\r\nFIFO Ramp Done... %d\r\n", j);
}



// BO
void	mps_Npi_Run(int a, int data)
{
	mps_plb_Reg13.npi_run = data&0x1;
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
	xil_printf("NPI-1 SDI-RAM (0xD000:0000)  %s\r\n",
				(data == 1)? "RUN":"STOP");
}

void	mps_Npi_0_Run(int a, int data)
{
	mps_plb_Reg13.npi_0_run = data&0x1;
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
	xil_printf("NPI-0 PM (0xC200:0000) %s\r\n",
				(data == 1)? "RUN":"STOP");
}



/*
 * Clearing Latch and Registers
 * BO
 */
void	mps_OutputLatchClear(int a, int data)
{
	if(data == 1 )
	{
		// 04/24/2015 added
		// clear uBlaze error count
		dcct_plc_wd_err_cnt=0;
		cell_plc_wd_err_cnt=0;
		//PLC watchdog count reset
		mps_IOWatchDog_Reset(0,0);
		//

		int sec = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*24);
		xil_printf("Get timestamp : %d\r\n", sec);
		//TsCalc( sec, 0);
		//
		mps_plb_Reg13.pm_usr_rst    = data&0x1;
		mps_plb_Reg13.OutLatchClear = data&0x1;
		mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
		xil_printf("OutLatchClear...\r\n");
		sleep(100);
		mps_plb_Reg13.pm_usr_rst    = 0;
		mps_plb_Reg13.OutLatchClear = 0;
		mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
		xil_printf("OutLatchClear ready...\r\n");

		//clear BM 180 bpm delta zero register at pscEpics_sock_tx.c
		pSrvTxDdrOffAddr->data[250] = 0;
		pSrvTxDdrOffAddr->data[11]  = 0;	//10 kHz evg trigger timeout
		// 11/14/2014
		mps_plb_Reg13.fast_dump_ublaz = 0;		//ID DUMP clear
		mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));

		//11/18/2014
		pSrvTxDdrOffAddr->data[DB_SDI_OFFSET+4] = 0;	//SFP loss count clear
		pSrvTxDdrOffAddr->data[11] = 0;					//evg trig err count clear
		pSrvTxDdrOffAddr->data[18] = 0;					//SDI CW error
		pSrvTxDdrOffAddr->data[19] = 0;					//SDI CCW error
		//clear SDI error count
		CC_Sdi_Reset();

		uBlaze_err_code = 0;
		xil_printf("Global Reset is DONE!...\r\n");
	}
}

/*
 *
 */
void	mps_active_intlock_processClear(int a, int data)
{
	if(data == 1 )
	{
		mps_plb_Reg13.active_intlock_process_rst = 1;
		mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
		sleep(1);
		mps_plb_Reg13.active_intlock_process_rst = 0;
		mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
		xil_printf("active_intlock_process_rst RESET\r\n");
	}
}


// BO
void	mps_FaultTest(int a, int data)
{
	mps_plb_Reg13.fault_test = data&0x1;
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
	xil_printf("Fault Test = %d\r\n", data);
}


void mps_Reg15_Set(Xuint32 value)
{
	*(volatile unsigned int*)(XPAR_FOFB_COEFF_SET_0_BASEADDR + 15*4) = value;
}

void mps_Reg14_Mask(Xuint32 mask)
{
	*(volatile unsigned int*)(XPAR_FOFB_COEFF_SET_0_BASEADDR + 14*4) = mask;
}

//mask 4 for ID enable/disable
void mps_Reg4_Mask(Xuint32 mask)
{
	*(volatile unsigned int*)(XPAR_FOFB_COEFF_SET_0_BASEADDR + 4*4) = mask;
}


void	mps_ext_out0(int a, int data)
{
	mps_plb_Reg12.dout0 = data & 1;
	mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));
	xil_printf("Ext out 0 = %d\r\n", data);
}

void	mps_ext_out1(int a, int data)
{
	mps_plb_Reg12.dout1 = data & 1;
	mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));
	xil_printf("Ext out 1 = %d\r\n", data);
}

void	mps_ext_out2(int a, int data)
{
	mps_plb_Reg12.dout2 = data & 1;
	mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));
	xil_printf("Ext out 2 = %d\r\n", data);
}


//assign bm_limit_low_set = plb_w_reg[8];
//assign bm_limit_hi_set  = plb_w_reg[11];
	
void	mps_BM_Offset0(int a, int data)
{
int	nm;
	nm = data * 1000;
	if (nm < 8000000) nm = 8000000;			//Min 8 mm
	else if (nm > 12000000) nm = 12000000;  //Max 12 mm

	mps_int32Reg_Set(8, nm );	//low -
	xil_printf("AI-BM POS_LIMIT = %d [nm]\r\n", nm);
}


void	mps_BM_Offset1(int a, int data)
{
	int	nm;

	nm = data;// * 1000000;
	mps_int32Reg_Set(11, nm );	//high +
}


// 03/27/2014 added BRAM for SR bpm reference
void	mps_SrBpm_Xoffset(int a, int data)
{
	int i;
	int	ut_data[180];

	xil_printf("Sr-bpm offset RAM x = %d (nm)\r\n", data);
	for(i=0; i<180; i++) {
		ut_data[i] = data;
	}
	// 2. Reference orbit Write and Read : nm Unit
	fofb_ReferenceOrbitWrite(0, 180, &ut_data[0]);


}

void	mps_SrBpm_Yoffset(int a, int data)
{
	int i;
	int	ut_data[180];

	xil_printf("Sr-bpm offset RAM y = %d (nm)\r\n", data);
	for(i=0; i<180; i++) {
		ut_data[i] = data;
	}
	// 2. Reference orbit Write and Read : nm Unit
	fofb_ReferenceOrbitWrite(180, 360, &ut_data[0]);


}


void	mps_beamC_sim(int a, int data)
{
	mps_plb_Reg12.beam_sim = data & 1;
	mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));
	if(data == 1)
		xil_printf("Beam Current is Simulation Mode\r\n");
	else	
		xil_printf("Beam Current is Operation current from DCCT\r\n");
}

void	mps_beamC_2mA(int a, int data)
{
	mps_plb_Reg12.set2mA = data & 1;
	mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));
	if(data == 1) {
		xil_printf("Sim 2 mA beam Current is high\r\n");
		//mps_plb_Reg12.beam_sim = 1;
		//mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));
	}
	else {	
		xil_printf("2 mA Beam Current is Operation current from DCCT\r\n");
		//mps_plb_Reg12.beam_sim = 0;
		//mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));
	}
}

void	mps_beamC_50mA(int a, int data)
{
	mps_plb_Reg12.set50mA = data & 1;
	mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));
	if(data == 1) {
		xil_printf("Sim 0.2mA and 50 mA beam Current is high\r\n");
		//mps_plb_Reg12.beam_sim = 1;
		//mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));
	}
	else {	
		xil_printf("0.2mA and 50 mA Beam Current is Operation current from DCCT\r\n");
		//mps_plb_Reg12.beam_sim = 0;
		//mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));
	}
}



/*
 *	CONTROL PARAMETERS FROM DATA BASE
 * //DPRAM BLOCK Chip selection
#define 	CS_DO_X1_SP 				0		//Diamond x1
#define 	CS_DO_X2_SP 				1
#define 	CS_RM_X_SP 					2
#define 	CS_RM_Y_SP 					3
#define 	CS_RA_X_SP 					4
#define 	CS_RA_Y_SP 					5
#define 	CS_SM_SP 					7		//S for mm
#define 	CS_SA_SP 					6		//S for Angle
#define 	CS_ADDR_IXS_LOGIC_SP 		8		//Logic

	cs1[0] - cs1[8]

		S_A       S_MM
c3 = 0.191498, 0.486978
c5_1 = 0.375375, 0.500000
c5_2 = 0.392773, 0.500000
c10 = 0.126135, 0.491801
c11 = 0.191498, 0.486978
c23 = 0.199880, 0.440736
c28 = 0.136240, 0.498910


0.191498 * 4294967295;
 */
void mps_DpRamOffsetWrite(int cs, int addr, int Data)
{
	//DPRAM Write
	FOFB_PLB_CS1.wr = 0;		//disable write
	FOFB_PLB_CS1.ram_cs = cs;	//CS 0
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));

	//write
	//for(i=0; i<64; i++)
	{
		FOFB_PLB_CS1.addr   = addr;			//. select address
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
		Set_I_DDR2( CS1_DATA_ADDR, Data );	// write Data
		//
		FOFB_PLB_CS1.wr = 1;		//write
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
		FOFB_PLB_CS1.wr = 0;		//write
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	}
}




/*
 *
 *
 * 1. Select Mux for read dual port memory
 *
 *	cs1
 */
void	mps_DpRamOffsetRead(int cs, int	*pData)
{
	int i;

	// select mux address
	mps_plb_Reg12.ref_mux_sel = cs;	//0.1....
	mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));

	FOFB_PLB_CS1.wr     = 0;	//disable write
	FOFB_PLB_CS1.ram_cs = cs;	//cs = 0
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));

	//2. write address and data
	for(i=0; i< MAX_ID_NO; i++)
	{
		FOFB_PLB_CS1.addr   = i;
		Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
		*pData++ = Get_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (21 * 4) );
		//xil_printf("%d=%d\r\n", i, d);
	}
}


//
/*
  // address table
`define	ADDR_H1					0	//AIO_H1
`define	ADDR_V1					1	//AIO_V1
`define	ADDR_H2					2
`define	ADDR_V2					3
`define	ADDR_H3					4
`define	ADDR_V3					5
`define	ADDR_ANGLE_H			6	//AIAL_H
`define	ADDR_ANGLE_V			7	//AIAL_V
`define	ADDR_MRAD_S				8
`define	ADDR_MM_S				9
`define	ADDR_LOGIC				10
`define	H1_CHAMBER_OFFSET		11		//added 03/29/14
`define	V1_CHAMBER_OFFSET		12
`define	H2_CHAMBER_OFFSET		13		//added 03/31/14
`define	V2_CHAMBER_OFFSET		14

 */
void	mps_init()
{
	int	i;
	int	data[128];
	int fix_angle;
	int fix_offset;


	mps_OutputLatchClear(0, 0);
	mps_FaultTest(0, 0);
	mps_Npi_0_Run(0, 0);
	mps_plb_Reg12.io_wd_clear  = 1;
	mps_plb_Reg12.npi_0_in_sel = 0;		// 0: SDI RAM DATA, 1: MUX
	mps_plb_Reg12.io_wd_clear  = 0;		//(plb_w_Reg12[28] == 1'b0) ? xy_pos_read : mux_Data_o;
	//mps_plb_Reg12.npi_rst_peride = 30000;	//3 sec
	mps_plb_Reg12.npi_rst_peride = 20000;		//06/15/15 changed
	mps_plb_Reg12.set_beam_0_2mA = 0;
	mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));

	//reset
	mps_plb_Reg13.OutLatchClear  = 1;
	mps_plb_Reg13.pm_usr_rst = 1;
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
	mps_plb_Reg13.pm_usr_rst = 0;
	mps_plb_Reg13.OutLatchClear = 0;
	mps_plb_Reg13.enable_bm_zero_detection = 0;		//disable
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
	/*
	 * 	Mask 0 mean AIE disabled
	 *
	 */
	//aie_USET_MASK_SETTING, IDevice Masking
	mps_Reg_Set(4, 0x000000FF);
	mps_Reg_Set(5, 0x000000FF);



	//mps_int32Reg_Set(11, +5000000);

	//ID parameters setting
	//int d_x_limit, d_y_limit, r_mm_x_limit, r_mm_y_limit, r_mrad__x_limit, r_mrad_y_limit;
	for(i=0; i<MAX_ID_NO; i++) {
		fix_angle  = (int)(tAI_DB_Setting[i].s_angle  * FIX_32_31);
		fix_offset = (int)(tAI_DB_Setting[i].s_offset * FIX_32_31);

		//fix_angle  = (int)(1.0  * FIX_32_31);
		//fix_offset = (int)(1.0 * FIX_32_31);
		//xil_printf("%d : %x : a=0x%x, o=0x%x\r\n", i, tAI_DB_Setting[i].logic, fix_angle, fix_offset);

		mps_DpRamOffsetWrite(0, i, tAI_DB_Setting[i].h1_limit );
		mps_DpRamOffsetWrite(1, i, tAI_DB_Setting[i].v1_limit );
		mps_DpRamOffsetWrite(2, i, tAI_DB_Setting[i].h2_limit );
		mps_DpRamOffsetWrite(3, i, tAI_DB_Setting[i].v2_limit );
		mps_DpRamOffsetWrite(4, i, tAI_DB_Setting[i].h3_limit );
		mps_DpRamOffsetWrite(5, i, tAI_DB_Setting[i].v3_limit );
		//
		mps_DpRamOffsetWrite(6, i, tAI_DB_Setting[i].r_mrad__x_limit );
		mps_DpRamOffsetWrite(7, i, tAI_DB_Setting[i].r_mrad_y_limit );
		mps_DpRamOffsetWrite(8, i, fix_angle );	//6
		mps_DpRamOffsetWrite(9, i, fix_offset );	//7
		//
		//Write Logic values to DPRAM
		mps_DpRamOffsetWrite(CS_ADDR_IXS_LOGIC_SP, i, tAI_DB_Setting[i].logic );

		/*
		 *  ID BPM OFFSET, 04/01/2014 added
		 */
		mps_DpRamOffsetWrite(11, i, 1 );
		mps_DpRamOffsetWrite(12, i, 2 );
		mps_DpRamOffsetWrite(13, i, 3 );
		mps_DpRamOffsetWrite(14, i, 4 );

	}

	mps_DpRamOffsetRead(CS_H1_SP, &data[0]);

	// 2:  mode[0]=mm, mode[1] = cal
	mps_plb_Reg13.mps_cal_mode     = 2;		//all angle calculation for test, 2: for normal, 0: for all mm
	mps_plb_Reg13.mps_set_addr_sel = 0;
	mps_plb_Reg13.mps_AIE_muxDir   = 0;	//0:internal, 1:external
	mps_plb_Reg13.id_gap_sim  = 0;	//ID gap from DIN
	mps_plb_Reg13.id_ps_sim   = 0;
	mps_plb_Reg13.bm_ps_sim   = 0;
	mps_plb_Reg13.id_falt_rst = 0;
	mps_plb_Reg13.fast_dump_ublaz = 0;
	mps_plb_Reg13.id_bpm_falt_en = 0;
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));

	//Cell 3 position set for testing
	mps_IDbpmPosSettingForSimCal(3,  1000, -1000, 2000, -2000, 3000, -3000);

	//single position read
	Set_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + 15*4, 0);
	//
	mps_IOWatchDog_Reset(0,0);

	//1'st order filter
	mps_plb_Reg8.factor = 1;
	mps_plb_Reg8.i_gain = 10000;
	mps_Reg_Set(8, *((Xuint32 *)&mps_plb_Reg8 ));
	//
	mps_plb_Reg11.y16 = 0;
	//mps_plb_Reg11.npi_0_data_sel  = 0;	// 0: xy_pos_read
	mps_plb_Reg11.fastDumpDly     = 10;		//06/25/20
	mps_plb_Reg11.npi_0_data_size = 0;	// 9 for 10
	mps_Reg_Set(11, *((Xuint32 *)&mps_plb_Reg11 ));


	//uBlaze watchDog
	mps_plb_Reg12.io_wd_clear  = 0;
	mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));

	mps_ai_PlcBmBeamDump_enable(0, 0);
	mps_ai_PlcIdBeamDump_enable(0, 1);

	//10/07/14 added for reset
	mps_OutputLatchClear(0, 1);

	//////////////////////
	//10/07/2014
	// 20 ns * 20000000 = 40000000
	// fifo envelop width
	//mps_plb_Reg15.ai_BM_glitch_mode  = 1;		//default mode
	//mps_plb_Reg15.ai_ID_glitch_mode  = 0;
	//mps_Reg_Set(15, *((Xuint32 *)&mps_plb_Reg15 ));

	//NPI 1 RUN
	mps_Npi_Run(0, 1);


	/////////////////////////////////////
	//06/11/2015
	// BPM position limit
	mps_BM_Offset0(0, 12000);			// +/- 12 mm
	mps_GlitchDetectionModeSel(0, 1);	//5 cycle debounce mode
	mps_BM_CalModeDispSel(0, 0);		//display default
	//
	mps_SrBpm_Xoffset(0, 5000000);
	mps_SrBpm_Yoffset(0, 3000000);

	///////////
#if 1
	mps_plb_Reg13.bm_bpm_err_cnt = 13;	//3/19/2015 bpm zero error count
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
	//enable dump
	plb_Reg10_SetBpmLength.bm_error_dump_enable = 1;
	Set_I_DDR2(REG10_WR, *((Xuint32 *)&plb_Reg10_SetBpmLength) );
#endif

	//assign	AI_ID_ResetTrig       = (plb_w_reg[13][27] == 1'b1) ? AI_ID_ResetTrig1 : AI_ID_ResetTrig0;
	//mps_plb_Reg13.FOFB_RUN = 1;		// 1 : DspTrig
	//mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
	
	//AI reset delay
	mps_id_mask_srtting_1to30(0, 1000);	//20000 us
	mps_ext_out0(0, 0);		//debounce select
	mps_ext_out1(0, 1);		// PM trigger output to EVG
	mps_ext_out2(0, 1);		//
	mps_Ai_ID_GlitchDetectionModeSel(0, 1);		// 1: AI-ID 3 cycle mode, 0: one cycle
	mps_dac_a_output(0, 7500);					// 150 us, 1 cycle, 20 ns * 7500 @ 50 MHz SysClock
	//SDI waveform
	mps_SdiWfmSel(0, 1);						// Live data default


	//04/09/19
	mps_active_intlock_processClear(0, 0);
}


void	mps_soft_trig(int a, int data)
{
	xil_printf("mps_soft trig 1\r\n");
}


/*
 * I/O board output control
 */
void	mps_IO_OutCtrl(int a, int data)
{
	//mps_plb_Reg12.dout0 = data & 1;
	mps_plb_Reg12.dout1 = ~data & 01;
	//mps_plb_Reg12.dout2 = data & 0x4;
	//mps_plb_Reg12.dout3 = data & 0x8;
	mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));

}

//
void	mps_TrigIsSim(int a, int sim)
{
	if(sim == 1) {
		xil_printf("Trigger is Internal\r\n");
		}
	else {
		xil_printf("Trigger is Ever External\r\n");
	}
	plb_Control_0.IntTrig       = sim & 0x1;	//[28]
	Set_I_DDR2( XPAR_FOFB_COEFF_SET_0_BASEADDR + (6*4), *((Xuint32 *) & plb_Control_0));
}

void	mps_ChipScope_MuxAddr(int a, int data)
{

	mps_plb_Reg13.mps_set_addr_sel = data & 0xF;
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
	xil_printf("Chip scope mux addr = %d\r\n", mps_plb_Reg13.mps_set_addr_sel);
}


void	mps_calc_mode(int a, int data)
{

	mps_plb_Reg13.mps_cal_mode     = data&0x3;		//all angle calculation for test, 2: for normal, 0: for all mm
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
	switch(data) {
	case 0:
		xil_printf("both mm calculation -- test mode\r\n"); break;
	case 1:
		xil_printf("angle, mm calculation -- test mode\r\n"); break;
	case 2:
		xil_printf("mm, angle calculation -- Operation mode\r\n"); break;
	case 3:
		xil_printf("both angle calculation -- test mode\r\n"); break;
	defaule:
		xil_printf("Use 0 - 3 \r\n");
	}
}


/*
 *
 */
void	mps_Set_S_values(int a, double angle, double offset)
{
int i;
int	fix_angle, fix_offset;

	printf("angle = %.3f, offset = %.3f \r\n", angle, offset);
	for(i=0; i<MAX_ID_NO; i++) {
		fix_angle  = (int)(angle * FIX_32_31);
		fix_offset = (int)(offset * FIX_32_31);
		xil_printf("%d : a=0x%x, o=0x%x\r\n", i, fix_angle, fix_offset);

		mps_DpRamOffsetWrite(CS_SA_SP, i, fix_angle );	//7
		mps_DpRamOffsetWrite(CS_SM_SP, i, fix_offset );	//8
	}

}

/*
 * 	assign xy_pos_read = xy_pos_Reg[ plb_w_reg[15] ];
 *
 */
void	mps_read_single_pos(int a, int bpm_pos)
{
	//single position address setting for read
	if(bpm_pos > 800) bpm_pos = 0;
	mps_plb_Reg15.pos_sel_addr = bpm_pos;
	mps_Reg_Set(15, *((Xuint32 *)&mps_plb_Reg15 ));
}

	
void	mps_npi_pos_width(int a, int data)
{
	mps_plb_Reg11.npi_0_data_size = data-1;	// 9 for 10
	mps_Reg_Set(11, *((Xuint32 *)&mps_plb_Reg11 ));
}


void	mps_npi_data_type_sel(int a, int data)
{
	//mps_plb_Reg11.npi_0_data_sel = data;	//6/25/20 remove
	mps_plb_Reg11.fastDumpDly = data;
	mps_Reg_Set(11, *((Xuint32 *)&mps_plb_Reg11 ));
	xil_printf("fastDumpDly = %d*100 us\r\n", mps_plb_Reg11.fastDumpDly);
	/*
	assign npi_burst_0_data =  	(plb_w_reg[11][11:10] == 2'b00)  ? x_um_o      :
								(plb_w_reg[11][11:10] == 2'b01)  ? xy_pos_read :
								(plb_w_reg[11][11:10] == 3'b02)  ? mux_Data_o  :
										*/
}

/*
 * Read block ram for simulation result values.
 *
 * 	cs2[10]	- x_mm_cal result
 * 	cs2[11]	- y_mm_cal result
 * 	cs2[12]	- x_mrad_cal result
 * 	cs2[13]	- y_mrad_cal result
 * 	cs2[14]	- id bpm result
 *
 */
void	mps_Read_FPGA_CalcResultBram(int cs,  int len)
{
	int i;
	Xint32 data;

	//mux select
	mps_plb_Reg12.cal_rst_mux_sel = cs-10;	//0,1,2,3,4
	mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));

	//
	fofb_dpram.ram_cs = cs;		//cs[0]
	fofb_dpram.en = 1;
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));

#if 0
	if(cs == 15) {	//RM Interlock status reading
		for(i=0; i<len; i++) {
			fofb_dpram.addr = i;
			fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
			//read BRAM
			data = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + MPS_BM_IL_RESULT_BRAM_RD_REG );
			//Copy to DDR
			//Set_I_DDR2(BASE_REF_ORBIT+(i*4), data);
			xil_printf("%d = 0x%x\r\n", i, data);
		}
	}
#endif
	if (cs == 14 || cs == 15){
		for(i=0; i<len; i++) {
			fofb_dpram.addr = i;
			fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
			//read BRAM
			data = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (30*4));
			//Copy to DDR
			//Set_I_DDR2(BASE_REF_ORBIT+(i*4), data);

			xil_printf("%d=0x%x, addr= %d, rst=0x%x, mode=0x%x \r\n", i, data, data&0x3F, (data>>6)&0x3F, (data>>12)&0xF);
		}
	}

	else {	//cs = 10,11,12,13
		for(i=0; i<len; i++) {
			fofb_dpram.addr = i;
			fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
			//read BRAM
			data = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (30*4));
			//Copy to DDR
			//Set_I_DDR2(BASE_REF_ORBIT+(i*4), data);
			if(cs == 10 || cs == 11)
				printf("%d = %.3f (mm)\r\n", i, data*0.000001);
			else
				printf("%d = %.3f (mrad)\r\n", i, data*0.000001);
		}
	}
	fofb_dpram.en = 0;	//disable
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
}


/*
 *
 *
 */
void	mps_Read_FPGA_Calc_ResultData(int cs,  int *pData, int len)
{
	int i;
	Xint32 data;

	//mux select
	mps_plb_Reg12.cal_rst_mux_sel = cs-10;	//0,1,2,3,4
	mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));

	//
	fofb_dpram.ram_cs = cs;		//cs[0]
	fofb_dpram.en = 1;
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));

	if(cs == 15) {	//RM Interlock status reading
		for(i=0; i<len; i++) {
			fofb_dpram.addr = i;
			fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
			//read BRAM
			//*pData++ = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + MPS_BM_IL_RESULT_BRAM_RD_REG );
			*pData++ = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (30*4));
			//Copy to DDR
			//Set_I_DDR2(BASE_REF_ORBIT+(i*4), data);
			//xil_printf("%d=%d\r\n", i, data);
		}
	}
	else if (cs == 14 ){
		for(i=0; i<len; i++) {
			fofb_dpram.addr = i;
			fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
			//read BRAM
			*pData++ = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (30*4));
			//Copy to DDR
			//Set_I_DDR2(BASE_REF_ORBIT+(i*4), data);

			//xil_printf("%d=0x%x, addr= %d, rst=0x%x, mode=0x%x \r\n", i, data, data&0x3F, (data>>6)&0x3F, (data>>12)&0xF);
		}
	}
	else {	//cs = 10,11,12,13
		for(i=0; i<len; i++) {
			fofb_dpram.addr = i;
			fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
			//read BRAM
			*pData++ = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (30*4));
			//Copy to DDR
			//Set_I_DDR2(BASE_REF_ORBIT+(i*4), data);
			//xil_printf("%d=%d\r\n", i, data);
		}
	}
	fofb_dpram.en = 0;	//disable
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
}


/*
 *
 */
void	mps_IDbpmPosSettingForSimCal(int cell,  int x1, int y1, int x2, int y2, int x3, int y3)
{
	int i;
	int	pos;
	int	pos_x1, pos_x2, pos_x3;
	int	pos_y1, pos_y2, pos_y3;
	int	mm_x_offset, mrad_x_angle;
	int	mm_y_offset, mrad_y_angle;
	int	fix_offset, fix_angle;

	if(cell <1 ) cell =1;
	else if(cell > 30) cell = 29;

	i = cell;

	xil_printf("\r\n");
	xil_printf("x1,y1 = %d, %d\r\n", tPosId_Cc[i].x1, tPosId_Cc[i].y1 );
	xil_printf("x2,y2 = %d, %d\r\n", tPosId_Cc[i].x2, tPosId_Cc[i].y2 );
	xil_printf("x3,y3 = %d, %d\r\n", tPosId_Cc[i].x3, tPosId_Cc[i].y3 );

	fofb_SimRamSinglePositionDataSet(tPosId_Cc[i].x1, x1);  //
	fofb_SimRamSinglePositionDataSet(tPosId_Cc[i].y1, y1);
	//
	fofb_SimRamSinglePositionDataSet(tPosId_Cc[i].x2, x2);
	fofb_SimRamSinglePositionDataSet(tPosId_Cc[i].y2, y2);
	//
	fofb_SimRamSinglePositionDataSet(tPosId_Cc[i].x3, x3);
	fofb_SimRamSinglePositionDataSet(tPosId_Cc[i].y3, y3);

	//
	sleep(100);
	xil_printf("\r\n\r\nRead:\r\n");
	//read
	FOFB_PLB_CS1.regSel = 0;
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	//
	plb_Reg7Control_1.DacChASet = tPosId_Cc[i].x1;	//select address
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	pos_x1 = Get_I_DDR2(READ_K_REG);	//read register
	printf("bpm position addr= %d : %f (mm)\r\n", tPosId_Cc[i].x1, pos_x1*0.000001);

	plb_Reg7Control_1.DacChASet = tPosId_Cc[i].y1;	//select address
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	pos_y1 = Get_I_DDR2(READ_K_REG);	//read register
	printf("bpm position addr= %d : %f (mm)\r\n", tPosId_Cc[i].y1, pos_y1*0.000001);

	plb_Reg7Control_1.DacChASet = tPosId_Cc[i].x2;	//select address
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	pos_x2 = Get_I_DDR2(READ_K_REG);	//read register
	printf("bpm position addr= %d : %f (mm)\r\n", tPosId_Cc[i].x2, pos_x2*0.000001);

	plb_Reg7Control_1.DacChASet = tPosId_Cc[i].y2;	//select address
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	pos_y2 = Get_I_DDR2(READ_K_REG);	//read register
	printf("bpm position addr= %d : %f (mm)\r\n", tPosId_Cc[i].y2, pos_y2*0.000001);

	plb_Reg7Control_1.DacChASet = tPosId_Cc[i].x3;	//select address
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	pos_x3 = Get_I_DDR2(READ_K_REG);	//read register
	printf("bpm position addr= %d : %f (mm)\r\n", tPosId_Cc[i].x3, pos_x3*0.000001);

	plb_Reg7Control_1.DacChASet = tPosId_Cc[i].y3;	//select address
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	pos_y3 = Get_I_DDR2(READ_K_REG);	//read register
	printf("bpm position addr= %d : %f (mm)\r\n", tPosId_Cc[i].y3, pos_y3*0.000001);
	//
	//CC sid data-0
	plb_Reg7Control_1.DacChASet = tPosId_Cc[i].cc_d0;	//select address
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	pos = Get_I_DDR2(READ_K_REG);	//read register
	xil_printf("sdi cc addr= %d : %d\r\n", tPosId_Cc[i].cc_d0, pos);
	//CC sdi data-1
	plb_Reg7Control_1.DacChASet = tPosId_Cc[i].cc_d1;	//select address
	Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
	pos = Get_I_DDR2(READ_K_REG);	//read register
	xil_printf("sdi cc addr= %d : %d\r\n", tPosId_Cc[i].cc_d1, pos);

	int	cIndex;
	switch(cell) {
		case 3:
			cIndex = 0;
			break;
		case 5:
			cIndex = 1;
			break;

		case 10:
			cIndex = 3;
			break;
		case 11:
			cIndex = 4;
			break;
		case 23:
			cIndex = 5;
			break;
		case 28:
			cIndex = 6;
			break;
		case 8:
			cIndex = 7;
			break;
		case 18:
			cIndex = 8;
			break;
		default:
			cIndex = 0;
	}
	//---------------
	// Calculation
	//---------------
	if (cell == 5)	// 3 bpms per cell
	{
		//5-1
		xil_printf("\r\n*** Cell %d-1 uBlaze calculation result ****\r\n", cell);
		fix_angle  = (int)(tAI_DB_Setting[cIndex].s_angle  * FIX_32_31);
		fix_offset = (int)(tAI_DB_Setting[cIndex].s_offset * FIX_32_31);

		mm_x_offset  = (int)(  pos_x1 + (pos_x2-pos_x1) * tAI_DB_Setting[cIndex].s_offset );
		mrad_x_angle = (int)(  (pos_x2-pos_x1) * tAI_DB_Setting[cIndex].s_angle);
		printf("x mm= %f  : mrad=%f  ,  s_ang=0x%x,  s_off=0x%x\r\n", mm_x_offset*0.000001, mrad_x_angle*0.000001, fix_angle, fix_offset);

		mm_y_offset  = (int)( pos_y1 + (pos_y2-pos_y1) * tAI_DB_Setting[cIndex].s_offset);
		mrad_y_angle = (int)((pos_y2-pos_y1) * tAI_DB_Setting[cIndex].s_angle);
		printf("y mm= %f  : mrad=%f\r\n", mm_y_offset*0.000001, mrad_y_angle*0.000001);

		//5-2
		xil_printf("\r\n*** Cell %d-2 uBlaze calculation result ****\r\n", cell);
		fix_angle  = (int)(tAI_DB_Setting[cIndex+1].s_angle  * FIX_32_31);
		fix_offset = (int)(tAI_DB_Setting[cIndex+1].s_offset * FIX_32_31);

		mm_x_offset  = (int)(  pos_x2 + (pos_x3-pos_x2) * tAI_DB_Setting[cIndex+1].s_offset );
		mrad_x_angle = (int)(  (pos_x3-pos_x2) * tAI_DB_Setting[cIndex+1].s_angle);
		printf("x mm= %f  : mrad=%f  ,  0x%x,  0x%x\r\n", mm_x_offset*0.000001, mrad_x_angle*0.000001, fix_angle, fix_offset);

		mm_y_offset  = (int)( pos_y2 + (pos_y3-pos_y2) * tAI_DB_Setting[cIndex+1].s_offset);
		mrad_y_angle = (int)((pos_y3-pos_y2) * tAI_DB_Setting[cIndex+1].s_angle);
		printf("y mm= %f  : mrad=%f\r\n", mm_y_offset*0.000001, mrad_y_angle*0.000001);
	}
	else {
		xil_printf("\r\n*** Cell %d uBlaze calculation result ****\r\n", cell);
		fix_angle  = (int)(tAI_DB_Setting[cIndex].s_angle  * FIX_32_31);
		fix_offset = (int)(tAI_DB_Setting[cIndex].s_offset * FIX_32_31);

		mm_x_offset  = (int)(  pos_x1 + (pos_x2-pos_x1) * tAI_DB_Setting[cIndex].s_offset );
		mrad_x_angle = (int)(  (pos_x2-pos_x1) * tAI_DB_Setting[cIndex].s_angle);
		printf("x -> mm= %f  : mrad=%f  ,  s_ang==0x%x,  s_off=0x%x\r\n", mm_x_offset*0.000001, mrad_x_angle*0.000001, fix_angle, fix_offset);

		mm_y_offset  = (int)( pos_y1 + (pos_y2-pos_y1) * tAI_DB_Setting[cIndex].s_offset);
		mrad_y_angle = (int)((pos_y2-pos_y1) * tAI_DB_Setting[cIndex].s_angle);
		printf("y -> mm= %f  : mrad=%f\r\n", mm_y_offset*0.000001, mrad_y_angle*0.000001);

	}


}


/*
 * Read ID bpms position data
 *
 */
void	mps_read_id_bpms()
{
int	i;
int	pos;

	xil_printf("\r\n\r\nRead ID bpms:\r\n");
	//read
	FOFB_PLB_CS1.regSel = 0;
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));

	for(i=0; i<10; i++) {
		plb_Reg7Control_1.DacChASet = tPosId_Cc[i].x1;	//select address
		Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
		pos = Get_I_DDR2(READ_K_REG);	//read register
		xil_printf("bpm position addr= %d : %d\r\n", tPosId_Cc[i].x1, pos);

		plb_Reg7Control_1.DacChASet = tPosId_Cc[i].y1;	//select address
		Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
		pos = Get_I_DDR2(READ_K_REG);	//read register
		xil_printf("bpm position addr= %d : %d\r\n", tPosId_Cc[i].y1, pos);
		//
		plb_Reg7Control_1.DacChASet = tPosId_Cc[i].x2;	//select address
		Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
		pos = Get_I_DDR2(READ_K_REG);	//read register
		xil_printf("bpm position addr= %d : %d\r\n", tPosId_Cc[i].x2, pos);

		plb_Reg7Control_1.DacChASet = tPosId_Cc[i].y2;	//select address
		Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
		pos = Get_I_DDR2(READ_K_REG);	//read register
		xil_printf("bpm position addr= %d : %d\r\n", tPosId_Cc[i].y2, pos);

		plb_Reg7Control_1.DacChASet = tPosId_Cc[i].x3;	//select address
		Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
		pos = Get_I_DDR2(READ_K_REG);	//read register
		xil_printf("bpm position addr= %d : %d\r\n", tPosId_Cc[i].x3, pos);

		plb_Reg7Control_1.DacChASet = tPosId_Cc[i].y3;	//select address
		Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
		pos = Get_I_DDR2(READ_K_REG);	//read register
		xil_printf("bpm position addr= %d : %d\r\n", tPosId_Cc[i].y3, pos);
	}
}


// read dump status
Xuint32	mps_ID_DumpStatusRead()
{
	Xuint32	status;
	status = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + MPS_DUMP_STATUS_REG) & 0xFFFF;
}

// system status reading
Xuint32	mps_ID_SystemStatusRead()
{
	Xuint32	status;
	status = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + MPS_SYSTEM_STATUS_RD_REG);
}

/*
 *
 */
void	mps_ReadSettingValue()
{
	int i;
	int	fix_angle, fix_offset;


	for(i=0; i<8; i++)
	{

		fix_angle  = (int)(tAI_DB_Setting[i].s_angle  * FIX_32_31);
		fix_offset = (int)(tAI_DB_Setting[i].s_offset * FIX_32_31);

		//xil_printf("%d : %x : a=0x%x, o=0x%x\r\n", i, tAI_DB_Setting[i].logic, fix_angle, fix_offset);

		printf("\r\nCell No : %d     logic   = %d\r\n",  tAI_DB_Setting[i].cell_no, tAI_DB_Setting[i].logic );
		
		printf("dx1      = %f, dy1      = %f\r\n",  (float)tAI_DB_Setting[i].h1_limit * 0.000001, (float)tAI_DB_Setting[i].v1_limit  * 0.000001);
		printf("dx2      = %f, dy2      = %f\r\n",  (float)tAI_DB_Setting[i].h2_limit * 0.000001, (float)tAI_DB_Setting[i].v2_limit  * 0.000001);
		printf("dx3      = %f, dy3      = %f\r\n",  (float)tAI_DB_Setting[i].h3_limit * 0.000001, (float)tAI_DB_Setting[i].v3_limit  * 0.000001);
		printf("mrad dx = %f, mrad dy = %f\r\n",  (float)tAI_DB_Setting[i].r_mrad__x_limit  * 0.000001, (float)tAI_DB_Setting[i].r_mrad_y_limit * 0.000001);
	}
}

//
void	mps_id_mask_srtting_1to30(int a, int data)
{
	//aie_USET_MASK_SETTING, IDevice Masking
	//if(data < 0xf) xil_printf("data must 1 - 0xffffffff\r\n");
	//else
	{
		mps_Reg_Set(4, data);	//max 0x7FFFFFFF
		xil_printf("ID 31 - 60 enable : 0x%x\r\n", data);
	}
}

//
void	mps_id_mask_srtting_31to60(int a, int data)
{
	//aie_USET_MASK_SETTING, IDevice Masking
	//if(data == 0) xil_printf("data must 1 - 0xffffffff\r\n");
	//else
	{
		mps_Reg_Set(5, data);
		xil_printf("ID 1 - 30 enable : 0x%x\r\n", data);
	}
}

/*
 *
 */
int	ReadSysMon_TimeStamp(int *pData)
{
	int i;

	//read data
	FOFB_PLB_CS1.regSel = 7; 	//cs address is 7
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	for(i=10; i<14; i++) {
		plb_Reg7Control_1.DacChASet = 10;	//select address
		Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
		pData[i] = Get_I_DDR2(READ_K_REG);	//read register
	}

	return 0;
}

/*
 *
 */
int	ReadSysMon_IdGapStatus(int *pData)
{
	int i;

	//read data
	FOFB_PLB_CS1.regSel = 7; 	//cs address is 7
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	for(i=8; i<10; i++) {
		plb_Reg7Control_1.DacChASet = 10;	//select address
		Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
		pData[i] = Get_I_DDR2(READ_K_REG);	//read register
	}

	return 0;
}

int	ReadSysMon_AieDumpResult(int *pData)
{
	int i;

	//read data
	FOFB_PLB_CS1.regSel = 7; 	//cs address is 7
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	for(i=47; i<55; i++) {
		plb_Reg7Control_1.DacChASet = 10;	//select address
		Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
		pData[i] = Get_I_DDR2(READ_K_REG);	//read register
	}

	return 0;
}

int	ReadSysMon_BmIlResult(int *pData)
{
	int i;

	//read data
	FOFB_PLB_CS1.regSel = 7; 	//cs address is 7
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	for(i=180; i<180+30; i++) {
		plb_Reg7Control_1.DacChASet = 10;	//select address
		Set_I_DDR2(REG7_WR, *((Xuint32 *)&plb_Reg7Control_1) );
		pData[i] = Get_I_DDR2(READ_K_REG);	//read register
	}

	return 0;
}

/*
 * position lookup table
 */
void	position_table()
{
int c;
	for  (c = 0; c<30; c++) {
		printf("(#Cell %2d)  ? { cc0=%3d, id-y2=%3d, id-x2=%3d, id-y1=%3d, id-x1=%3d} :\r\n", c+1, 24+c*26, 15+c*26, 14+c*26, 13+c*26, 12+c*26);
	}
}


/*
 *	Dump enable ?
 *		before clearing latch
 */
void	mps_ID_dump_out_enable(int a, int data)
{
	if(data == 1) {
		//
		mps_plb_Reg13.OutLatchClear = data&0x1;
		mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
		xil_printf("OutLatchClear = %x\r\n", data);
		mps_plb_Reg13.OutLatchClear = 0;
		mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
		uBlaze_err_code = 0;
		sleep(10);
		//
		plb_Reg10_SetBpmLength.id_dump_en = 1;
		Set_I_DDR2(REG10_WR, *((Xuint32 *)&plb_Reg10_SetBpmLength) );
		xil_printf("AI-ID Dump Enabled...\r\n");
	}
	else {
		plb_Reg10_SetBpmLength.id_dump_en = 0;
		Set_I_DDR2(REG10_WR, *((Xuint32 *)&plb_Reg10_SetBpmLength) );
		xil_printf("AI-ID Dump DISABLED...\r\n");
	}
}



void	mps_BM_dump_out_enable(int a, int data)
{
	if(data == 1) {
		//
		mps_plb_Reg13.OutLatchClear = data&0x1;
		mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
		xil_printf("OutLatchClear = %x\r\n", data);
		mps_plb_Reg13.OutLatchClear = 0;
		mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
		uBlaze_err_code = 0;
		sleep(10);
		//
		plb_Reg10_SetBpmLength.bm_dump_en = 1;
		Set_I_DDR2(REG10_WR, *((Xuint32 *)&plb_Reg10_SetBpmLength) );
		xil_printf("AI-BM Dump Enabled...\r\n");
	}
	else {
		plb_Reg10_SetBpmLength.bm_dump_en = 0;
		Set_I_DDR2(REG10_WR, *((Xuint32 *)&plb_Reg10_SetBpmLength) );
		xil_printf("AI-BM Dump DISABLED...\r\n");
	}
}



void	mps_IOWatchDog_Reset(int a, int data)
{
	mps_plb_Reg12.io_wd_clear = 1;
	mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));
	sleep(2);
	mps_plb_Reg12.io_wd_clear = 0;
	mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));
}

/*
 * AI-ID glitch ignore cycle selection
 */
void	mps_dac_a_output(int a, int data)
{
	mps_plb_Reg15.dac_a_sel = 1;

	if(data > 50000) data = 50000;   //max 5 cycle
	else if(data < 5000) data = 7500;

	mps_plb_Reg15.dac_data  = data;
	mps_Reg_Set(15, *((Xuint32 *)&mps_plb_Reg15 ));
	xil_printf("AI-ID glitch count = %d, %d ns\r\n", data, data*20);
}

void	sdi_cw_pkt_delay(int a, int data)
{
	plb_Reg9.cw_pkt_dly = data & 0xffff;
	mps_Reg_Set(9, *((Xuint32 *)&plb_Reg9 ));
}

void	sdi_ccw_pkt_delay(int a, int data)
{
	plb_Reg9.ccw_pkt_dly = data & 0xffff;
	mps_Reg_Set(9, *((Xuint32 *)&plb_Reg9 ));
}

//
void	mps_LemoOut1MuxSel(int a, int data)
{
	plb_Reg10_SetBpmLength.lemo1_mux_sel = data;
	Set_I_DDR2(REG10_WR, *((Xuint32 *)&plb_Reg10_SetBpmLength) );
	xil_printf("LEMO-1 = %d\r\n", plb_Reg10_SetBpmLength.lemo1_mux_sel);
}
void	mps_LemoOut2MuxSel(int a, int data)
{
	plb_Reg10_SetBpmLength.lemo2_mux_sel = data;
	Set_I_DDR2(REG10_WR, *((Xuint32 *)&plb_Reg10_SetBpmLength) );
	xil_printf("LEMO-2 = %d\r\n", plb_Reg10_SetBpmLength.lemo2_mux_sel);
}

void	mps_ID_gap_sim(int a, int data)
{
	mps_plb_Reg13.id_gap_sim = data & 0x1;
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
	if(data == 1)
		xil_printf("Gap is simulation mode\r\n");
	else
		xil_printf("Gap is operation mode\r\n");
}

void	mps_ID_PS_sim(int a, int data)
{
	mps_plb_Reg13.id_ps_sim = data & 0x1;
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
	if(data == 1)
		xil_printf("ID PS is simulation mode\r\n");
	else
		xil_printf("ID PS is operation mode\r\n");
}

void	mps_BM_PS_sim(int a, int data)
{
	mps_plb_Reg13.bm_ps_sim = data & 0x1;
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
	if(data == 1)
		xil_printf("BM PS is simulation mode\r\n");
	else
		xil_printf("BM PS is operation mode\r\n");
}

	
	
/*
 * Clearing or Set Simulation BRAM
 */
void	myAieSim_MemClear(int a, int data)
{
	XyPositionSimDataAllSet( data );
}


/*
 * SR-BPM closed
 *
 */
int	mask_bpm_addr=0;
void	mps_select_sr_bpm_address(int a, int data)
{
	int addr;
	if(data > 179) {
		xil_printf("BPM address error %d\r\n", data);
	}
	else {
		mask_bpm_addr = data;
		//addr = (cell-1) * 6 + (bpm_no-1);
		xil_printf("BPM address %d\r\n", mask_bpm_addr);
	}

}

void	mps_select_sr_bpm_enable(int a, int data)
{
	if(data == 1) {	//enable
		fofb_MaskRamWrite(mask_bpm_addr, 1);
		xil_printf("BPM address %d = 1\r\n", mask_bpm_addr);
	}
	else{	//disable
		fofb_MaskRamWrite(mask_bpm_addr, 0);
		xil_printf("BPM address %d = 0\r\n", mask_bpm_addr);
	}
}


//
void	mps_ai_id_enable(int addr, int data)
{
	switch(addr) {
		case 0:		mps_plb_Reg4.ai_id1 = data & 0x1; break;
		case 1:		mps_plb_Reg4.ai_id2 = data & 0x1; break;
		case 2:		mps_plb_Reg4.ai_id3 = data & 0x1; break;
		case 3:		mps_plb_Reg4.ai_id4 = data & 0x1; break;
		case 4:		mps_plb_Reg4.ai_id5 = data & 0x1; break;
		case 5:		mps_plb_Reg4.ai_id6 = data & 0x1; break;
		case 6:		mps_plb_Reg4.ai_id7 = data & 0x1; break;
		case 7:		mps_plb_Reg4.ai_id8 = data & 0x1; break;
		case 8:		mps_plb_Reg4.ai_id9 = data & 0x1; break;
		case 9:		mps_plb_Reg4.ai_id10 = data & 0x1; break;
		case 10:		mps_plb_Reg4.ai_id11 = data & 0x1; break;
		case 11:		mps_plb_Reg4.ai_id12 = data & 0x1; break;
		case 12:		mps_plb_Reg4.ai_id13 = data & 0x1; break;
		case 13:		mps_plb_Reg4.ai_id14 = data & 0x1; break;
		case 14:		mps_plb_Reg4.ai_id15 = data & 0x1; break;
		case 15:		mps_plb_Reg4.ai_id16 = data & 0x1; break;
		case 16:		mps_plb_Reg4.ai_id17 = data & 0x1; break;
		case 17:		mps_plb_Reg4.ai_id18 = data & 0x1; break;
		case 18:		mps_plb_Reg4.ai_id19 = data & 0x1; break;
		case 19:		mps_plb_Reg4.ai_id20 = data & 0x1; break;
		case 20:		mps_plb_Reg4.ai_id21 = data & 0x1; break;
		case 21:		mps_plb_Reg4.ai_id22 = data & 0x1; break;
		case 22:		mps_plb_Reg4.ai_id23 = data & 0x1; break;
		case 23:		mps_plb_Reg4.ai_id24 = data & 0x1; break;
		case 24:		mps_plb_Reg4.ai_id25 = data & 0x1; break;
		case 25:		mps_plb_Reg4.ai_id26 = data & 0x1; break;
		case 26:		mps_plb_Reg4.ai_id27 = data & 0x1; break;
		case 27:		mps_plb_Reg4.ai_id28 = data & 0x1; break;
		case 28:		mps_plb_Reg4.ai_id29 = data & 0x1; break;
		case 29:		mps_plb_Reg4.ai_id30 = data & 0x1; break;
		case 30:		mps_plb_Reg4.ai_id31 = data & 0x1; break;
		case 31:		mps_plb_Reg4.ai_id32 = data & 0x1; break;
	}
	mps_Reg_Set(4, *((Xuint32 *)&mps_plb_Reg4 ));
}


void	mps_ai_PlcBmBeamDump_enable(int addr, int data)
{
	plb_Reg10_SetBpmLength.CellPLC_BmBeamDump = data&1;
	Set_I_DDR2(REG10_WR, *((Xuint32 *)&plb_Reg10_SetBpmLength) );
	xil_printf("CellPLC_BM BeamDump = %d\r\n", data);
}

void	mps_ai_PlcIdBeamDump_enable(int addr, int data)
{
	plb_Reg10_SetBpmLength.CellPLC_IdBeamDump = data&1;
	Set_I_DDR2(REG10_WR, *((Xuint32 *)&plb_Reg10_SetBpmLength) );
	xil_printf("CellPLC_ID BeamDump, plb_Reg10[13] = %d\r\n", data);
}



//
// 10/07/2014
/*
 	assign u_fifo_we = (~u_fifo_full) && fifo_we_envelop && muxValid;	//10/07/14
	assign u_fifo_rd = (ram_cs2[6] && ram_WR2); // && ~u_fifo_empty;
	fifo_32x2K 	ID_CalMux_mon_fifo (
		.clk(clk),
		.rst(fifo_rst),
		.din(mux_Data_o), 	// input [31 : 0] din
		.wr_en(u_fifo_we), 		// input wr_en
		.rd_en(u_fifo_rd), 		// input rd_en
		.dout(u_fifo_out), 		// output [31 : 0] dout
		.full(u_fifo_full), 	// output full
		.empty(u_fifo_empty) 	// output empty
	);

 */
//void	mps_mux_fifo_read(int *pData)
int	mps_mux_fifo_read()
{
int i;
Xuint32	data;

	//select mux
	mps_plb_Reg15.bram_ref_rd_sel = 2;	//U_FIFO_OUT
	mps_Reg_Set(15, *((Xuint32 *)&mps_plb_Reg15 ));

	fofb_dpram.ram_cs = 6;		//cs[0]
	fofb_dpram.en = 1;
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));

	for(i=0; i<2000; i++) {
		//send read output data
		fofb_dpram.wr = 1;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
		fofb_dpram.wr = 0;
		fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));

		//read fifo from register 31
		data = Get_I_DDR2(XPAR_FOFB_COEFF_SET_0_BASEADDR + (31*4));
		if(i == 0) {
			if(data != 0x5C000000) return 0;	//fifo empty or data error
		}
		//*pData++ = data;
		//Copy to DDR
		Set_I_DDR2( BASE_AIP_MUX_DATA + (i*4), data );
		//xil_printf("%d=%d\r\n", i, data);
	}

	fofb_dpram.en = 0;	//disable
	fofb_dpram.wr = 0;
	fofb_Reg3_Mask( *((Xuint32 *)&fofb_dpram));
	return 1;
}


/*
 *
 */
void	mps_npi_1_ddr_wfm_tx_size(int addr, int data)
{
	xil_printf("(%d) NPI 1 DDR Tx Wfm Size = %d\r\n", addr, data);
}


void	mps_npi_0_ddr_pm_wfm_tx_offset(int addr, int data)
{
	xil_printf("(%d) NPI 0 PM DDR Tx Wfm offset = %d\r\n", addr, data);
}


void	mps_ID_BpmFaltEnable(int a, int data)
{
	mps_plb_Reg13.id_bpm_falt_en = data & 0x1;
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
	if(data == 1)
		xil_printf("ID Bpm Fault Enable\r\n");
	else
		xil_printf("ID Bpm Fault Disable\r\n");
}

void	mps_ID_FaltReset(int a, int data)
{
	mps_plb_Reg13.id_falt_rst = data & 0x1;
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
	if(data == 1)
		xil_printf("ID Fault Reset 1\r\n");
	else
		xil_printf("ID Fault Reset 0\r\n");
}

/*
 *
 */
void	mps_RF_UserDump(int a, int data)
{
	mps_plb_Reg13.fast_dump_ublaz = 1;		//ID DUMP clear
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
}


void	mps_Clear_BBA_Offset(int a, int data)
{
	int i;
	if(data == 1) { 
		for(i=0; i<1023; i++) {
			Set_I_DDR2(BASE_DDR_SRBPM_BBA_OFFSET+(i*4), 0);
		}
		xil_printf("bba offset ram cleared\r\n");
	}
}


void	mps_BM_bpm_error_dump (int a, int data)
{
	//enable dump
	plb_Reg10_SetBpmLength.bm_error_dump_enable = data&1;
	Set_I_DDR2(REG10_WR, *((Xuint32 *)&plb_Reg10_SetBpmLength) );
	xil_printf("mps_BM_bpm_error_dump = %d\r\n", data);

}


// x, y 180x2
void	mps_BM_bpm_error_dump_count (int a, int data)
{
	if(data > 1203) data = 1023;
	mps_plb_Reg13.bm_bpm_err_cnt = data;
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));

	xil_printf("mps_BM_bpm_error_dump count = %d\r\n", data);
}



//
void	mps_BM_bpm_enable_bm_zero_detection (int a, int data)
{
	mps_plb_Reg13.enable_bm_zero_detection = data & 0x1;	//only uBlaze
	//mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
	xil_printf("uBlaze wfm enable_bm_zero_detection = %d\r\n", data);
}


/*
 * assign	AI_BM_RfTripCmd    = (plb_w_reg[15][12] == 1'b1) ? AI_BM_RfTripCmdReg : BM_IL_mode_sel ;
 */
void	mps_GlitchDetectionModeSel(int a, int data)
{
	mps_plb_Reg15.ai_BM_glitch_mode  = data & 0x1;
	mps_Reg_Set(15, *((Xuint32 *)&mps_plb_Reg15 ));
	if(data == 1) xil_printf("AI-BM 5 cycle Glitch detect mode Enabled\r\n", data);
	else xil_printf("AI-BM Glitch detect mode Disabled\r\n", data);	
}


/*
 *
 */
void	mps_Ai_ID_GlitchDetectionModeSel(int a, int data)
{
	mps_plb_Reg15.ai_ID_glitch_mode  = data & 0x1;		//default mode
	mps_Reg_Set(15, *((Xuint32 *)&mps_plb_Reg15 ));
	if(data == 1) xil_printf("AI-ID Glitch detect mode Enabled (more than 2 cycles) \r\n", data);
	else xil_printf("AI-ID Glitch detect mode Disabled.\r\n", data);
}

// 1/20/17 disable
void	mps_BM_CalModeDispSel(int a, int data)
{
	// enable/disable BM error debounce

	/*
	mps_plb_Reg15.bm_calModeDispSel  = data & 0x1;
	mps_Reg_Set(15, *((Xuint32 *)&mps_plb_Reg15 ));
	if(data == 1) xil_printf("AI-BM Cal display Mode is FIFO\r\n", data);
	else xil_printf("AI-BM Cal display Mode is 5 Cycle count\r\n", data);
	*/
	mps_plb_Reg15.bm_calModeDispSel  = data & 0x1;
	mps_Reg_Set(15, *((Xuint32 *)&mps_plb_Reg15 ));
	if(data == 1) xil_printf("AI-BM Debounce enable\r\n", data);
	else xil_printf("AI-BM Debounce disable\r\n", data);

}


#if 0
/*
 * assign	BM_IL_mode_sel     = (plb_w_reg[15][14] == 1'b1) ? AI_BM_Fifo2CycleCompCmdReg : BM_LATCHED_OUT ;
 */
void	mps_BM_CalModeSel(int a, int data)
{
	mps_plb_Reg15.bm_calModeSel  = data & 0x1;		//default mode
	mps_Reg_Set(15, *((Xuint32 *)&mps_plb_Reg15 ));
	if(data == 1) xil_printf("AI-BM Cal Mode is FIFO\r\n", data);
	else xil_printf("AI-BM Cal Mode is single Cycle count\r\n", data);
	
}
#endif


void	mps_PM_PeriodeSetting(int a, int data)
{
	mps_plb_Reg12.npi_rst_peride = data;
	mps_Reg_Set(12, *((Xuint32 *)&mps_plb_Reg12 ));
	xil_printf("PM size = %d points\r\n", data);
}

//
void	mps_ID_ResetSourceSel(int a, int data)
{
	mps_plb_Reg13.FOFB_RUN = data&1;		//ID DUMP clear
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));

	if(data == 1) xil_printf("Canting status reset 1..\r\n");
	else xil_printf("Canting status reset 0..\r\n");
}


//
void	mps_BM_NofBpmsZeroSet(int a, int data)
{
	if(DFE_CONTROL_REGS->ioReg[72] > 10 ) DFE_CONTROL_REGS->ioReg[72] = 10;
	xil_printf("mps_BM_NofBpmsZeroSet = %d\r\n", data);
}

void	mps_SdiWfmSel(int a, int data)
{
	FOFB_PLB_CS1.sdiLinkWfmSel = data&1;
	Set_I_DDR2( CS1_CTRL_ADDR, *((Xuint32 *) & FOFB_PLB_CS1));
	if(FOFB_PLB_CS1.sdiLinkWfmSel == 1)
		xil_printf("SDI Link Live Waveform\r\n");
	else
		xil_printf("SDI Link Latched Waveform\r\n");
}

// 6/25/20
void	mps_FastDumpDelayEnable(int a, int data)
{
	mps_plb_Reg13.FastDelayedEnable = data&1;
	mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
	xil_printf("FastDump Delay enable ? %d\r\n", mps_plb_Reg13.FastDelayedEnable);
}
