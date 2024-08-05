
#include <stdio.h>
#include <string.h>

#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwipopts.h"

#include "mb.h"
#include "uart_print.h"
#include "fpga_system.h"
#include "pscEpics.h"


#include "mbtcp.h"


#define PROG  "FreeModbus"


#define Set_u8(addr, val)  	(* (Xuint8 *) 	(addr)) = (val)


void dBpm_TcpModbusSrv_Thread()
{
eMBErrorCode    xStatus;
    	
	xil_printf("\r\n----- Starting Modbus TCP server thread -----\r\n");
	
	while (1)
    {
        if( eMBTCPInit( MB_TCP_PORT_USE_DEFAULT ) != MB_ENOERR )
        {
            fprintf( stderr, "%s: can't initialize modbus stack!\r\n", PROG );
			sleep(1000);            
        }
        else if( eMBEnable(  ) != MB_ENOERR )
        {
            fprintf( stderr, "%s: Can't enable modbus stack!\r\n", PROG );
            sleep(1000);
        }
        else
        {
            do
            {
                xStatus = eMBPoll();
            }
            while( xStatus == MB_ENOERR );
        }
        /* An error occured. Maybe we can restart. */
        ( void )eMBDisable(  );
        ( void )eMBClose(  );
    }


}

/*
 *
 */
void dBpm_TcpPscSrv_Thread()
{
eMBErrorCode    xStatus;
int	totalSendByte;
static volatile  char *pDdrRegTxAddr;
static volatile  char *pDdrSaRegTxAddr;


	xil_printf("\r\n----- Starting User TCP server thread -----\r\n");

	while (1)
    {
#if 1
        if( __eMBTCPInit( 600 ) != MB_ENOERR )	//mb.c
        {
            fprintf( stderr, "\r\n%s: can't initialize User stack!\r\n", "pscServer" );
			sleep(1000);
        }
        else if( eMBEnable(  ) != MB_ENOERR )
        {
            fprintf( stderr, "%s: Can't enable User stack!\r\n", "pscServer" );
            sleep(2000);
        }
        else
        {
            do
            {
            	xStatus = __eMBPoll();
            	// rad register
            	read_regs_10Hz_rate();

        		/*
        		 * SEND CONTROL REGISTER
        		 *	DDR2_CIO_BASE
        		 */
            	if(xStatus == MB_ENOERR ) {
					totalSendByte = 1024;
					Set_u8( DDR3_TX_REG-8, 'P');
					Set_u8( DDR3_TX_REG-7, 'S');
					Set_uInt16_DDR2(DDR3_TX_REG-6, TX_REG_MSG_ID);
					Set_uInt32_DDR2(DDR3_TX_REG-4, totalSendByte-HEAD_SIZE);
					pDdrRegTxAddr = ( char *) (DDR3_TX_REG-8);
					__eMBTCPPacketSend( 0, pDdrRegTxAddr, totalSendByte );

					/*
					 * SEND SA REGISTER
					 *
					 */
#if 0
					totalSendByte = 240;	// 60
					Set_u8( DDR2_SA_BASE-8, 'P');
					Set_u8( DDR2_SA_BASE-7, 'S');
					Set_uInt16_DDR2(DDR2_SA_BASE-6, TX_REG_MSG_ID+1);		//msg 31
					Set_uInt32_DDR2(DDR2_SA_BASE-4, totalSendByte-HEAD_SIZE);	//data length
					pDdrSaRegTxAddr = ( char *) (DDR2_SA_BASE-8);
					__eMBTCPPacketSend( 0, pDdrSaRegTxAddr, totalSendByte );
#endif

            	}

            	sleep(200);		//5 Hz rate

            }
            while( xStatus == MB_ENOERR );
        }
        /* An error occured. Maybe we can restart. */
        ( void )eMBDisable(  );
        ( void )eMBClose(  );

#else
	sleep(1000);
#endif
    }


}
