/*
 * Microblaze
 */

#ifndef	DFE_COMM_H
#define DFE_COMM_H

/*Dfe COMM opcodes */
#define OP_CTRL_DATA_WRQ			0xA0		/* op code for control commands */
#define OP_READ_DDR_DATA_RRQ		0xA1		/* op code for ddr memory read commands */
#define OP_SA_DATA_RRQ 				0xA2
#define DDR2_CLEAR_WRQ				0xA3
#define TRIG_WRQ   					0xA4
#define	OP_DDR_MEM_RRQ				0xA5



    
/* Pack all structures defined here on 1-byte boundaries */
#pragma pack(1)


/*
 *	SA dfe comm header
 */
typedef struct dfe_Comm_Response_str
{
    unsigned int    fcode;
    unsigned int    startReg;		/* 32-bit address */
    unsigned int    numRead;		/* 32-bit count */
    unsigned int    Ts0;			/* time stamp */
    unsigned int    Ts1;
} dfe_Comm_Response;


/* Revert to packing that was in effect when compilation started */
#pragma pack()

#endif 
