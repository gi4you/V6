/*
 * pscEpics.h
 *
 *  Created on: Sep 12, 2013
 *      Author: kha
 */

#ifndef PSCEPICS_H_
#define PSCEPICS_H_



#include "types.h"


#define	PSC_RX_SOCKET_PORT		7
#define	PSC_TX_SOCKET_PORT		20

#define	HEAD_SIZE			8
#define	DDR3_NPI0			0xC2000000
#define	DDR3_NPI1			0xD0000000
#define	DDR3_TX_REG			0xFFFE0000

#define	DDR3_RX_REG			0xE0000000


//message ID
#define	TX_REG_MSG_ID					30
#define	TX_NPI0_MSG_ID					31


#define	TX_FIFO_POS_X_MSG_ID			35
#define	TX_FIFO_POS_Y_MSG_ID			36

#define	TX_FIFO_REF_ORBIT_MSG_ID		37
#define	TX_FIFO_SRBPM_OFFSET_MSG_ID		38
#define	TX_FIFO_SRBPM_MASK_MSG_ID		39

#define	TX_ALL_XY_POS_WFM_MSG_ID		40

#define	TX_AIE_REF_SET_WFM_MSG_ID		60
#define	TX_AIE_BM_IL_STATUS_MSG_ID		42

#define	TX_AIE_FPGA_CAL_MSG_ID			45

#define	TX_NPI1_MSG_ID					53
#define	WFM_RX_ID						53

//
#define	TX_AIP_MUX_MSG_ID				70		//10/08/14 added
#define	TX_POS_Y_DELTA_MSG_ID			71


// epics database offset address
#define	DB_SDI_OFFSET		0		//0-37
#define	DB_FOFB_OFFSET		(152/4)	//38-
//#define	DB_EEPROM_OFFSET	(320/4)	//80-90

#define	DB_TIME_STAMP		100			//100 - 119
#define	DB_BPM_SDI_OFFSET	(480/4)		//120
#define	DB_EVR_OFFSET		(640/4)		//160-169
#define	DB_IL_OFFSET		(680/4)		//170 + 100



//
#define	READ_BASE					80	//80 (320)
// System Monitoring
#define	SYS_MON_TEMP								READ_BASE+0	//RO  (epics db 320)
#define	SYS_MON_VCCINT								READ_BASE+1	//RO	(324)
#define	SYS_MON_VCCAUX								READ_BASE+2	//RO	(328)
#define	SYS_MON_VCC5V								READ_BASE+3	//RO	(332)

#define	DFE_SFP1_RX_POWER							READ_BASE+5		//(340)
#define	DFE_SFP2_RX_POWER							READ_BASE+6		//(344)
#define	DFE_SFP3_RX_POWER							READ_BASE+7		//(348)
#define	DFE_SFP4_RX_POWER							READ_BASE+8		//(352)
#define	DFE_SFP5_RX_POWER							READ_BASE+9		//(356)
#define	DFE_SFP6_RX_POWER							READ_BASE+10	//(360)

#define	BOOT_TIME									READ_BASE+11




typedef struct {
    char  	idA;
    char  	idB;
    short 	MsgId;
    int 	bodyLen;
    //int		data[1024+12];		//data
    int		data[8192+12];
} __attribute__((packed)) psc_TxHead_t;


typedef struct _srvTxDatas
{
    char  	idA;
    char  	idB;
    short 	MsgId;
    int 	bodyLen;
    int		data[1200];
    int 	txLength;
} srvTxDatas, *pSRV_TXD_S;

#define pSrvTxDdrOffAddr  	( DDR2_MEM_CAST(pSRV_TXD_S) 	(DDR3_TX_REG-8) )


typedef struct {
    char  	idA;
    char  	idB;
    short 	MsgId;
    int 	bodyLen;
    int		data[1500];
} __attribute__((packed)) psc_RxBuffs_t;



typedef struct _srvTxDdrs0
{
    char  	idA;
    char  	idB;
    short 	MsgId;
    int 	bodyLen;
    int		data[58720000];	//C20000000 - C2000000  / 4, 4 bytes
    int 	txLength;
} srvTxDdrs0, *pSRV_TXDDR_0;

#define DDR3_TX0  	( DDR2_MEM_CAST(pSRV_TXDDR_0) 	(DDR3_NPI0 ) )


typedef struct _srvTxDdrs1
{
    char  	idA;
    char  	idB;
    short 	MsgId;
    int 	bodyLen;
    int		data[58720000];	//0xD0000000
    int 	txLength;
} srvTxDdrs1, *pSRV_TXDDR_1;

//#define DDR3_TX1  	( DDR2_MEM_CAST(pSRV_TXDDR_1) 	(DDR3_NPI1 - 8) )
#define DDR3_TX1  	( DDR2_MEM_CAST(pSRV_TXDDR_1) 	(DDR3_NPI1) )



// RX Space
typedef struct _srvRxDatas
{
    //char  	idA;
    //char  	idB;
    //short 	MsgId;
    //int 	bodyLen;
    int		data[64000];
} srvRxDatas, *pSRV_RX_DATA;
#define pSrvRxDdrOffAddr  	( DDR2_MEM_CAST(pSRV_RX_DATA) 	(DDR3_RX_REG) )






int	read_regs_10Hz_rate();	// 10 Hz data send to epics IOC
double rms(float *v, int dim);


#endif /* PSCEPICS_H_ */
