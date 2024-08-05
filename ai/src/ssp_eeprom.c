/*
 *	This code based on http://forge.abcd.harvard.edu/gf/project/arp-fpga/scmsvn/?action=browse&path=%2F
 *	
 * This file will abstract the read/write interfaces to the 24LC04B EEPROM on the XPS IIC Bus.
 * It was originally written and tested in a standalone environment, so the interrupts were
 * connected directly to the interrupt controller. Since it will be used in applications that
 * also require TCP/IP, it will need to use the xilkernel interfaces instead.
 *
 * 	01 March, 2010
 * 		uint8_t		-> uint8_T		"types.h"
 * 
 *	06, June	2010
 * 	Write block after sleep(100) for safe block writing.
 *
 *
 */

#include <string.h>
#include "config.h"
#include "uart_print.h"
#include "ssp_status.h"
#include "ssp_eeprom.h"
//SDK14_1
//#include <sleep.h>

#include "xparameters.h"
#include "xiic.h"
#include "types.h"

#include "fpga_system.h"


extern net_config_t cfg_opts[];
extern SSP_Config_t SSP_Config;

#if(VIRTEX6)
#define IIC_EEPROM_DEVICE_ID                XPAR_IIC_EEPROM_DEVICE_ID
#define	IIC_EEPROM_IIC2INTC_IRPT_INTR		XPAR_INTERRUPT_CNTLR_IIC_EEPROM_IIC2INTC_IRPT_INTR
#else
#define	IIC_EEPROM_DEVICE_ID				XPAR_XPS_IIC_EEPROM_0_DEVICE_ID
#define	IIC_EEPROM_IIC2INTC_IRPT_INTR		XPAR_XPS_INTC_0_XPS_IIC_EEPROM_0_IIC2INTC_IRPT_INTR
#endif


#ifdef __XMK__
  #include "sys/timer.h"
#else
  #include "xintc.h"
  #include "xtmrctr.h"
  #include "mb_interface.h"

  XIntc Intc;
  XTmrCtr TmrCtr;
#endif
/*
 M24C08_Block0 ==> ADDRESS 0xA8
 M24C08_Block1 ==> ADDRESS 0xAA
 M24C08_Block2 ==> ADDRESS 0xAC
 M24C08_Block3 ==> ADDRESS 0xAE
*/

//#if(VIRTEX6)
//#define ADDR_24LC04B 	0x54		/* ML605 */
//#else
  /* 0x50 	A2(1), A1(0), A0(0)   --- 0x54 */
#define ADDR_24LC04B 	0x50		/* DFE-2 */
//#endif

#define RBUFSIZE 		64
#define NPERROW 		10
#define EE_PAGE_SIZE 	16 			// 8 works
#define EE_TOTAL_SIZE 	1024		//512  -- 04 Model
#define EE_BLOCK_SIZE 	256


XIic Iic;
static volatile char send_complete, recv_complete, status_events;

static void Iic_StatusHandler(void *CallBackRef, int StatusEvent ) {
  status_events = 1;
}

static void Iic_RecvHandler(void *CallBackRef, int ByteCount ) {
	recv_complete = 1;
}

static void Iic_SendHandler(void *CallBackRef, int ByteCount ) {
	send_complete = 1;
}


// return non-zero if error reported
#if(AAA)
static int check_return( int rv, char *where, unsigned char *codes ) 
{
	if ( rv != XST_SUCCESS ) {
  		uart_mutex_Lock();
		safe_printf(("Error from %s: %d\r\n", where, rv));
    	uart_mutex_Ulock();
    	status_error(codes);
    	return 1;
  	}
  	else{
	  	uart_mutex_Lock();
	  	safe_printf(("EEPROM: OK from %s: %d\r\n", where, rv));
	  	uart_mutex_Lock();
  	}
  
  return 0;
}
#endif


/* Iic_Write() writes n_bytes of data from buf to the Iic device.
 * The first byte of buf is the address inside the device. */
int Iic_Write( XIic *Iicp, unsigned char *rbuf, int n_bytes ) 
{
	int rv, cnt = 0;
  	send_complete = 0;
  	status_events = 0;
  	rv = XIic_MasterSend( Iicp, rbuf, n_bytes );
  	if ( rv != XST_SUCCESS ) {
		xil_printf("error: XIic Master Send\r\n");
		return rv;
	}
  
  	while ( !send_complete && !status_events );
  /* Arbitrary post-write delay. The limit here was empirically tested
   * and worked without a hitch down to 1<<9. I backed off to 1<<10.
   * It might be possible to go much lower, but it really isn't
   * necessary, because we don't intend to be reading or writing
   * this device very much at all. */
  #ifdef __XMK__
    // This is a guess, may need to be extended
    sleep(10);
  #else
    // ### This may be optimized out of existence!
    for ( cnt = 0; cnt < (1<<10); ++cnt )
      rv += cnt;
  #endif
  return send_complete ? XST_SUCCESS : XST_FAILURE;
}

// Deail with block boundaries
int EE_Write( unsigned addr, unsigned char *rbuf, int n_bytes ) 
{
	unsigned char pbuf[EE_PAGE_SIZE+1];
	int rv;
	
  	if ( addr > EE_TOTAL_SIZE ) return XST_FAILURE;
  	if ( addr + n_bytes > EE_TOTAL_SIZE ) n_bytes = EE_TOTAL_SIZE - addr;
  	rv = XIic_SetAddress( &Iic, XII_ADDR_TO_SEND_TYPE, ADDR_24LC04B +
    	(addr < EE_BLOCK_SIZE ? 0 : 1));
  	if(rv != XST_SUCCESS) return rv;
    //check_return( rv, "EE_Write, XIic_SetAddress", "\001\003" );
	while (n_bytes > 0) 
	{
		int i;
		int p_bytes = EE_PAGE_SIZE - (addr & (EE_PAGE_SIZE-1));
		if ( n_bytes < p_bytes ) p_bytes = n_bytes;

		pbuf[0] = addr & (EE_BLOCK_SIZE-1);
		for ( i = 1; i <= p_bytes; i++ ) {
			pbuf[i] = *rbuf++;
		}
		//xil_printf("Iic_Write\r\n");
		i = Iic_Write(&Iic, pbuf, p_bytes+1);
		if ( i != XST_SUCCESS ) return i;
		
		sleep(100);		/* wait for next block */
		n_bytes -= p_bytes;
		addr += p_bytes;
    	if ( n_bytes > 0 && addr == EE_BLOCK_SIZE ) {
      		rv = XIic_SetAddress( &Iic, XII_ADDR_TO_SEND_TYPE, ADDR_24LC04B + 1 );
      		//check_return( rv, "EE_Write, XIic_SetAddress", "\001\003\001" );
    	}
	}
	return XST_SUCCESS;
}

static int EE_SetReadAddress( XIic *Iicp, unsigned char addr ) {
	return Iic_Write( Iicp, &addr, 1 );
} 

/* This needs to be expanded to deal with the two pages of memory and note the limited size of memory
 */
int EE_Read( unsigned addr, unsigned char *rbuf, int n_bytes ) 
{
	int rv, cnt = 0;
	int r_bytes;
  	recv_complete = 0;
  	status_events = 0;
  
  	if ( addr > EE_TOTAL_SIZE ) return XST_FAILURE;
  	if ( addr + n_bytes > EE_TOTAL_SIZE ) n_bytes = EE_TOTAL_SIZE - addr;
  	
  	//xil_printf("\r\n--EEPROM READ--\r\n");	//foR escapre forever loop
  	while ( n_bytes > 0 ) 
  	{    	
	  	//xil_printf("XIic_SetAddress \r\n");
    	rv = XIic_SetAddress( &Iic, XII_ADDR_TO_SEND_TYPE, ADDR_24LC04B +
      			(addr < EE_BLOCK_SIZE ? 0 : 1));
		if(rv != XST_SUCCESS) return rv;      		
    	//check_return( rv, "EE_Read, XIic_SetAddress", "\001\002" );
    	r_bytes = (addr < EE_BLOCK_SIZE && addr+n_bytes > EE_BLOCK_SIZE) ?
      				EE_BLOCK_SIZE - addr : n_bytes;
      	//xil_printf("EE_SetReadAddress \r\n");
    	XIic_SetOptions( &Iic, XII_REPEATED_START_OPTION );			//ALS Eric sugeetion
    	rv = EE_SetReadAddress( &Iic, addr & (EE_BLOCK_SIZE-1) );
    	 XIic_SetOptions( &Iic, 0 );								//ALS Eric
    	if ( rv != XST_SUCCESS ) {
			xil_printf("eeprom read fail...\r\n");
			return rv;
		}
    	
    	//xil_printf("EEPROM data read ...\r\n");
    	rv = XIic_MasterRecv( &Iic, rbuf, n_bytes );
    	if ( rv != XST_SUCCESS ) return rv;
    	
    	//xil_printf("EEPROM Check read complete???\r\n");

    	int timeout=0;
    	while ( !recv_complete && !status_events ) {
    		//; //xil_printf("EEPROM recv_complete\r\n");
    		sleep(10);
    		if(timeout > 100) break;
    	}
    	

    	if ( !recv_complete ) return XST_FAILURE;
    	addr += r_bytes;
    	rbuf += r_bytes;
    	n_bytes -= r_bytes;
    	
    	sleep(1);	/* must to give sleep */
    	
  	}

  	return XST_SUCCESS;
}


/*
 *
 *
 */
int EE_Init(void) 
{
	int cnt, i, rv;
	unsigned char rbuf[RBUFSIZE+1];
		
  	rv = XIic_Initialize(&Iic, XPAR_IIC_EEPROM_DEVICE_ID );
  	if( rv != XST_SUCCESS) {
	  	xil_printf("\r\n!!!EEPROM Initialize ERROR!!!\r\n");
	  	return -1;
  	}
  	
  	rv = XIic_SelfTest( &Iic );
  	
  	XIic_SetStatusHandler(&Iic, NULL, Iic_StatusHandler );
  	XIic_SetSendHandler(&Iic, NULL, Iic_SendHandler );
  	XIic_SetRecvHandler(&Iic, NULL, Iic_RecvHandler );
  
 
  #ifdef __XMK__                              
	rv = register_int_handler( XPAR_INTERRUPT_CNTLR_IIC_EEPROM_IIC2INTC_IRPT_INTR, XIic_InterruptHandler, &Iic );
    enable_interrupt(XPAR_INTERRUPT_CNTLR_IIC_EEPROM_IIC2INTC_IRPT_INTR);
  #else
    microblaze_disable_interrupts();
    rv = XIntc_Initialize( &Intc, XPAR_XPS_INTC_0_DEVICE_ID );
    check_return( rv, "XIntc_Initialize", "\001\004\002" );
    rv = XIntc_Start(&Intc, XIN_REAL_MODE);
    check_return( rv, "XIntc_Start", "\001\004\003" );
    rv = XIntc_Connect(&Intc, IIC_EEPROM_IIC2INTC_IRPT_INTR, XIic_InterruptHandler, &Iic);
    check_return( rv, "XIntc_Connect", "\001\004\005" );
    microblaze_register_handler((XInterruptHandler)XIntc_InterruptHandler, &Intc);
    microblaze_enable_interrupts();
    XIntc_Enable(&Intc, IIC_EEPROM_IIC2INTC_IRPT_INTR);
  #endif

  	rv = XIic_Start(&Iic);
  	{ 
	  	u32 Options = XIic_GetOptions( &Iic );
  		XIic_SetOptions( &Iic, Options & ~XII_SEND_10_BIT_OPTION );
  	}

  	xil_printf("\r\n!!!EEPROM Initialize PASSED!!!\r\n");
  	return 0;
}
 
// Fletcher16 checksum algorithm from http://en.wikipedia.org/wiki/Fletcher%27s_checksum
unsigned short fletcher16( unsigned char *data, size_t len )
{
	unsigned short sum1 = 0xff, sum2 = 0xff;

  while (len) {
          size_t tlen = len > 21 ? 21 : len;
          len -= tlen;
          do {
                  sum1 += *data++;
                  sum2 += sum1;
          } while (--tlen);
          sum1 = (sum1 & 0xff) + (sum1 >> 8);
          sum2 = (sum2 & 0xff) + (sum2 >> 8);
  }
  /* Second reduction step to reduce sums to 8 bits */
  sum1 = (sum1 & 0xff) + (sum1 >> 8);
  sum2 = (sum2 & 0xff) + (sum2 >> 8);
  return ((sum1&0xff) << 8) + (sum2&0xff);
}

volatile unsigned char *ptrDDR3_EEPROM = DDR2_EEPROM_SYSDATA_BASE;

// return 0 on success, 1 on error
int EE_ReadConfig( int addr, SSP_Config_t *cfg ) 
{
  	int rv, i;
  	unsigned short check;
  	
  	//status_set( 1, "\001\002" );
  	rv = EE_Read( addr, (unsigned char *)cfg, sizeof(SSP_Config_hdr_t));
  	//if ( check_return( rv, "EE_Read", "\001\005" ) ) return 1;
  	if( rv != XST_SUCCESS) {
	  	printf("eeprom read error. \r\n");
	  	return 1;
  	}
  	if ( cfg->hdr.n_bytes > EE_TOTAL_SIZE ||
       			cfg->hdr.n_bytes < offsetof(SSP_Config_t, notes)) {
    	//status_set( 1, "\001\002\001" );
    	printf("eeprom read size %d bytes \r\n", cfg->hdr.n_bytes);
    	return 2;
  	}
  	rv = EE_Read( addr, (unsigned char *)cfg, cfg->hdr.n_bytes );
  	if( rv != XST_SUCCESS) {
	  	printf("eeprom read error.. \r\n");
	  	return 3;
  	}
  	check = fletcher16( (unsigned char *)&cfg->version, cfg->hdr.n_bytes - sizeof(cfg->hdr));
  	if ( check != cfg->hdr.checksum ) {
    		//status_set( 1, "\001\002\003" );
    		printf("eeprom read checksum error. \r\n");
    	return 4;
  	}
  		
	memcpy( ptrDDR3_EEPROM, (unsigned char *)cfg, cfg->hdr.n_bytes);
	/*
	for(i=0; i<cfg->hdr.n_bytes; i++)		
		xil_printf("%c", Get_UCHAR_DDR2(DDR2_EEPROM_SYSDATA_BASE+i) );
	xil_printf("***********END Display EEPROM CONFIG\r\n");
	*/
  	return 0;
}



static void store_IP4( unsigned char *addr, unsigned char a, unsigned char b, unsigned char c, unsigned char d ) {
  addr[0] = a;
  addr[1] = b;
  addr[2] = c;
  addr[3] = d;
}


static void store_mac( unsigned char *addr, unsigned char a, unsigned char b, unsigned char c,
		unsigned char d, unsigned char e, unsigned char f ) {
  addr[0] = a;
  addr[1] = b;
  addr[2] = c;
  addr[3] = d;
  addr[4] = e;
  addr[5] = f;
}

/*
 *
 */
void EE_DefaultConfig(SSP_Config_t *cfg) 
{
  	cfg->version = 1;
  	store_mac(cfg->net_cfg.mac_address, 00, 0xA, 0x35, 0x55, 0x55, 0x55 );
  	store_IP4(cfg->net_cfg.ip_address, 10, 0, 131, 10 );
  	store_IP4(cfg->net_cfg.ip_netmask, 255, 255, 255, 0 );
  	store_IP4(cfg->net_cfg.ip_gateway, 10, 0, 131, 254 );
  
  	cfg->serial = 2010;
  	cfg->fab_date.year = 2010;
  	cfg->fab_date.month = 1;
  	cfg->fab_date.day = 1;
  	cfg->cfg_date.year = 2010;
  	cfg->cfg_date.month = 1;
  	cfg->cfg_date.day = 4;
//  	cfg->notes[0] = '\0';
	strncpy(cfg->notes, "NSLS2 Digital BPM", 17);
  	cfg->hdr.n_bytes = offsetof(SSP_Config_t,notes) + 1;
  	cfg->hdr.checksum =
    		fletcher16( (unsigned char *)&cfg->version, cfg->hdr.n_bytes - sizeof(cfg->hdr));
/*
    cfg->SystData.Location = 0;
    cfg->SystData.CellNo   = 1;
    cfg->SystData.bootFlashStartAddress = 256;
 */   		
}

/*
 *
 *
 */
int EE_WriteConfig( int addr, SSP_Config_t *cfg ) 
{
  	int rv;
  	cfg->hdr.n_bytes = offsetof(SSP_Config_t,notes) + strlen(cfg->notes) + 1;
  	if ( cfg->hdr.n_bytes > EE_TOTAL_SIZE ) {
    	//status_error( "\001\006" );
    	return 1;
  	}
  	cfg->hdr.checksum =
    		fletcher16( (unsigned char *)&cfg->version, cfg->hdr.n_bytes - sizeof(cfg->hdr));
  	//status_set( 1, "\001\003" );
  	
  	rv = EE_Write( addr, (unsigned char *)cfg, cfg->hdr.n_bytes );
  	if ( rv != 0 ) status_error( "\001\006\001" );
  	return 0;
}

/*
 *
 */
void EE_print_config( SSP_Config_t *cfg, char *heading ) 
{
  	uart_mutex_Lock();

  	//safe_printf(("head: %s\r\n", heading));
  	//safe_printf(("n_bytes: %d\r\n", cfg->hdr.n_bytes ));
  	//safe_printf(("checksum: %04X\r\n", cfg->hdr.checksum ));
  	safe_printf(("Config version: %d\r\n", cfg->version ));
  	safe_printf(("Board S/N: %d\r\n", cfg->serial ));
  	safe_printf(("Fab Date: %d/%d/%d\r\n", cfg->fab_date.year,
                  			cfg->fab_date.month, cfg->fab_date.day ));
  	safe_printf(("Cfg Date: %d/%d/%d\r\n", cfg->cfg_date.year,
                  cfg->cfg_date.month, cfg->cfg_date.day ));
  	safe_printf(("Notes: %s\r\n", cfg->notes ));
  	
  	safe_printf(("MAC ADDRESS: %02X:%02X:%02X:%02X:%02X:%02X\r\n",
			    cfg->net_cfg.mac_address[0],
			    cfg->net_cfg.mac_address[1],
			    cfg->net_cfg.mac_address[2],
			    cfg->net_cfg.mac_address[3],
			    cfg->net_cfg.mac_address[4],
			    cfg->net_cfg.mac_address[5] ));
  	safe_printf(("IP ADDRESS: %d.%d.%d.%d\r\n",
			    cfg->net_cfg.ip_address[0],
			    cfg->net_cfg.ip_address[1],
			    cfg->net_cfg.ip_address[2],
			    cfg->net_cfg.ip_address[3] ));
  	safe_printf(("IP NETMASK: %d.%d.%d.%d\r\n",
			    cfg->net_cfg.ip_netmask[0],
			    cfg->net_cfg.ip_netmask[1],
			    cfg->net_cfg.ip_netmask[2],
			    cfg->net_cfg.ip_netmask[3] ));
  	safe_printf(("IP GATEWAY: %d.%d.%d.%d\r\n\r\n",
			    cfg->net_cfg.ip_gateway[0],
			    cfg->net_cfg.ip_gateway[1],
			    cfg->net_cfg.ip_gateway[2],
			    cfg->net_cfg.ip_gateway[3] ));
/*
	safe_printf(("Location: %d\r\n", cfg->SystData.Location ));
	safe_printf(("Cell No: %d\r\n", cfg->SystData.CellNo ));
	safe_printf(("Flash Address: %d\r\n", cfg->SystData.bootFlashStartAddress ));
	*/
    uart_mutex_Ulock();
}



/*
 *	IP address setup and new IP address write to EEPROM
 *
 */
int	cmd_EEPROM_IP_Setup(int argc, char *argv[])
{
short	cfg_opt;
char    str[50];    
int     cnt, len;
int 	i, rv;

unsigned char character;

    net_config_t *cfg;	
    len = strlen(argv[1]);
    
    if(len > 7) len = 7;
    for(cnt=0; cnt < len; cnt++) {
    	str[cnt] = argv[1][cnt];
    }

 	safe_print("\r\nDo you want setup IP address, correct? (y/n) : ");	
    // Loop until we get a valid response
    while(1) {
	    fflush(stdout);
        character = inbyte();
        if((character == 'Y') || (character == 'y')) {
            outbyte(character);
            goto IP_SETUP;
        }
        else if((character == 'N') || (character == 'n')) {
        	outbyte(character);
            safe_print("\r\nBuy!...\r\n");
            return FALSE;
        }
    }     
     	
IP_SETUP:
   	safe_print(("\r\nEnter IP Code: ")); 
	character = inbyte();
	cfg_opt = character - '0' & 0xF;
	safe_printf(("\r\nIP Code=%d\r\n", cfg_opt)); 
	
  	if ( cfg_opt < N_CFG_OPTS ) 
  	{
  		cfg = &cfg_opts[cfg_opt];
  		xil_printf("\r\n\r\ncfg_opt=%d\r\n", cfg_opt);

   		for (i = 0; i < 6; i++ )
  			SSP_Config.net_cfg.mac_address[i] = cfg->mac_address[i];
  		for ( i = 0; i < 4; i++ ) {
  			SSP_Config.net_cfg.ip_address[i] = cfg->ip_address[i];
  			SSP_Config.net_cfg.ip_netmask[i] = cfg->ip_netmask[i];
  			SSP_Config.net_cfg.ip_gateway[i] = cfg->ip_gateway[i];
  		}
	}
  	else if ( cfg_opt == 15 ) 
  		cfg = &SSP_Config.net_cfg;
  	else {
	  	uart_mutex_Lock();
	  	safe_print(("\r\n  EEPROM Bad configuration option...\n")); 
	  	uart_mutex_Ulock();
	  	return(-1);
	}
	
  	EE_print_config(&SSP_Config, "IP INFO");
        	
  	
	if ( EE_WriteConfig( 0, &SSP_Config ) != 0)  {
		safe_print(("\r\nEE Write ERROR ...\r\n"));
	}
	
	
	return TRUE;  		
}
