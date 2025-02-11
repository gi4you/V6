/* 
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2006 Christian Walter <wolti@sil.at>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: mbtcp.c,v 1.3 2006/12/07 22:10:34 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbconfig.h"
#include "mbtcp.h"
#include "mbframe.h"
#include "mbport.h"

#include "xparameters.h"	//for XIO



#if MB_TCP_ENABLED > 0

/* ----------------------- Defines ------------------------------------------*/

/* ----------------------- MBAP Header --------------------------------------*/
/*
 *
 * <------------------------ MODBUS TCP/IP ADU(1) ------------------------->
 *              <----------- MODBUS PDU (1') ---------------->
 *  +-----------+---------------+------------------------------------------+
 *  | TID | PID | Length | UID  |Code | Data                               |
 *  +-----------+---------------+------------------------------------------+
 *  |     |     |        |      |                                           
 * (2)   (3)   (4)      (5)    (6)                                          
 *
 * (2)  ... MB_TCP_TID          = 0 (Transaction Identifier - 2 Byte) 
 * (3)  ... MB_TCP_PID          = 2 (Protocol Identifier - 2 Byte)
 * (4)  ... MB_TCP_LEN          = 4 (Number of bytes - 2 Byte)
 * (5)  ... MB_TCP_UID          = 6 (Unit Identifier - 1 Byte)
 * (6)  ... MB_TCP_FUNC         = 7 (Modbus Function Code)
 *
 * (1)  ... Modbus TCP/IP Application Data Unit
 * (1') ... Modbus Protocol Data Unit
 */

#define MB_TCP_TID          0
#define MB_TCP_PID          2
#define MB_TCP_LEN          4
#define MB_TCP_UID          6
#define MB_TCP_FUNC         7

#define MB_TCP_PROTOCOL_ID  0   /* 0 = Modbus Protocol */

#define SEND_BUF_SIZE 			1024	/* Max is 8192 */


                             

 
 
/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode
eMBTCPDoInit( USHORT ucTCPPort )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    if( xMBTCPPortInit( ucTCPPort ) == FALSE )
    {
        eStatus = MB_EPORTERR;
    }
    return eStatus;
}

void
eMBTCPStart( void )
{
}

void
eMBTCPStop( void )
{
    /* Make sure that no more clients are connected. */
    vMBTCPPortDisable( );
}

eMBErrorCode
//eMBTCPReceive( UCHAR * pucRcvAddress, UCHAR ** ppucFrame, USHORT * pusLength )
eMBTCPReceive( USHORT * pucRcvAddress, UCHAR ** ppucFrame, USHORT * pusLength )		//K.Ha USHORT
{
    eMBErrorCode    eStatus = MB_EIO;
    UCHAR          *pucMBTCPFrame;
    USHORT          usLength;
    USHORT          usPID;

    if( xMBTCPPortGetRequest( &pucMBTCPFrame, &usLength ) != FALSE )
    {
        usPID = pucMBTCPFrame[MB_TCP_PID] << 8U;
        usPID |= pucMBTCPFrame[MB_TCP_PID + 1];

        if( usPID == MB_TCP_PROTOCOL_ID )
        {
            *ppucFrame = &pucMBTCPFrame[MB_TCP_FUNC];
            *pusLength = usLength - MB_TCP_FUNC;
            eStatus = MB_ENOERR;

            /* Modbus TCP does not use any addresses. Fake the source address such
             * that the processing part deals with this frame.
             */
            *pucRcvAddress = MB_TCP_PSEUDO_ADDRESS;
        }
    }
    else
    {
        eStatus = MB_EIO;
    }
    return eStatus;
}


#define	ORG_TEST		1
#if(ORG_TEST)
eMBErrorCode
eMBTCPSend( UCHAR _unused, const UCHAR * pucFrame, USHORT usLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    UCHAR          *pucMBTCPFrame = ( UCHAR * ) pucFrame - MB_TCP_FUNC;
    USHORT          usTCPLength = usLength + MB_TCP_FUNC;

    /* The MBAP header is already initialized because the caller calls this
     * function with the buffer returned by the previous call. Therefore we 
     * only have to update the length in the header. Note that the length 
     * header includes the size of the Modbus PDU and the UID Byte. Therefore 
     * the length is usLength plus one.
     */
	pucMBTCPFrame[MB_TCP_FUNC] &= 0x1F;		//function code , added 0x1F from K.Ha
	
    pucMBTCPFrame[MB_TCP_LEN] = ( usLength + 1 ) >> 8U;
    pucMBTCPFrame[MB_TCP_LEN + 1] = ( usLength + 1 ) & 0xFF;		
    if( xMBTCPPortSendResponse( pucMBTCPFrame, usTCPLength ) == FALSE )
    {
        eStatus = MB_EIO;
    }
    return eStatus;
}


#else
eMBErrorCode
eMBTCPSend( UCHAR _unused, const UCHAR * pucFrame, USHORT usLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    UCHAR          *pucMBTCPFrame = ( UCHAR * ) pucFrame - MB_TCP_FUNC;
    USHORT          usTCPLength = usLength + MB_TCP_FUNC;

	int	i, block_num, size, rsize, bytes_sent;
	int	total_byte_send=0;
	
    block_num = (int)(usLength / SEND_BUF_SIZE);		
    size = (int)(block_num * SEND_BUF_SIZE);
    rsize =  usLength - size;
        
    //xil_printf("usTCPLength=%d, block=%d,  size=%d,  r_size=%d\r\n\r\n", usLength, block_num, size, rsize);
    
    XGpio_WriteReg(XPAR_LEDS_8BIT_BASEADDR, XGPIO_DATA_OFFSET,0xff);
    
    /* The MBAP header is already initialized because the caller calls this
     * function with the buffer returned by the previous call. Therefore we 
     * only have to update the length in the header. Note that the length 
     * header includes the size of the Modbus PDU and the UID Byte. Therefore 
     * the length is usLength plus one.
     */
	pucMBTCPFrame[MB_TCP_FUNC] &= 0x1F;		//function code , added 0x1F from K.Ha
	
    pucMBTCPFrame[MB_TCP_LEN] = ( usLength + 1 ) >> 8U;
    pucMBTCPFrame[MB_TCP_LEN + 1] = ( usLength + 1 ) & 0xFF;	
    
    // Block copy
    for(i=0; i<block_num; i++) 
	{			    	
	    if( xMBTCPPortSendResponse( &pucMBTCPFrame[total_byte_send], SEND_BUF_SIZE ) == FALSE )
	    {
	        eStatus = MB_EIO;
	        return eStatus;
	    }	    
	    total_byte_send += SEND_BUF_SIZE;
	    //sleep(1);
    }
    if( xMBTCPPortSendResponse( &pucMBTCPFrame[total_byte_send], rsize ) == FALSE )
    {
        eStatus = MB_EIO;
    }    

    XGpio_WriteReg(XPAR_LEDS_8BIT_BASEADDR, XGPIO_DATA_OFFSET,0x0);
    return eStatus;
}
#endif


#endif




eMBErrorCode
__eMBTCPPacketSend( UCHAR _unused, const UCHAR * pucFrame, USHORT usLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;
#if 0
    UCHAR          *pucMBTCPFrame = ( UCHAR * ) pucFrame - MB_TCP_FUNC;
    USHORT          usTCPLength = usLength + MB_TCP_FUNC;

    /* The MBAP header is already initialized because the caller calls this
     * function with the buffer returned by the previous call. Therefore we
     * only have to update the length in the header. Note that the length
     * header includes the size of the Modbus PDU and the UID Byte. Therefore
     * the length is usLength plus one.
     */
	pucMBTCPFrame[MB_TCP_FUNC] &= 0x1F;		//function code , added 0x1F from K.Ha

    pucMBTCPFrame[MB_TCP_LEN] = ( usLength + 1 ) >> 8U;
    pucMBTCPFrame[MB_TCP_LEN + 1] = ( usLength + 1 ) & 0xFF;
    if( xMBTCPPortSendResponse( pucMBTCPFrame, usTCPLength ) == FALSE )
    {
        eStatus = MB_EIO;
    }
#else
    if( xMBTCPPortSendResponse( pucFrame, usLength ) == FALSE )
    {
        eStatus = MB_EIO;
    }
#endif
    return eStatus;
}
