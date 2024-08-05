/*
 *	Active Interlock System Firmware
 *
 *	       NSLS-II Controls Group
 *	       kha@bnl.gov, 631-344-8058
 *
 *
 *	C/C++ Setting : elf to srec conversion   post build step
 *		mb-objcopy -O srec cc.elf mb_boot1.srec
 *
 *
 *	Downloadble binary file generation
 *		data2mem -bm system_bd.bmm -bd boot.elf -bt system.bit -o b test.bit
 *		Generate bin file using iMPACT : test.bin -> fpgabit1.bin
 *			8-bit BIN SWAP ON
 *
 *
 *	SDK 14.41
 *	4/17/2013
 *		Added "dBpm_UserTimer_Thread"
 *
 *		Wavefrom receiving from EPICS IOC, Python CA are working fine, but C cade CA it microblaze hang.
 *		Mabey looks C based CA are fast so Microblaze can't handle large array receiving.
 *
 *
 *	14.71 directory
 *	March 06, 2014
 *		1) tested wit 5 of Cell controller Node.
 *		2) reboot error
 *
 *	05/19/2014
 *		1) AI-BM limit and offset x, y indivisual PV
 *
 *	10/07/2014
 *		1) manual trig "t" added
 *
 *
 *	10/11/2014		2'nd Beam/ID gap commissioning
 *		1) added variable AI-BM (360) waveform tx size 1 sec ~ 10 sec
 *		2) for waveform tx first check head data if new head data will send to IOC
 *
 *
 *	10/15/2014
 *		1) Position original offset control fixed at epics_sock_rx.c
 *		2) Added manual setting function
 *
 *	10/17/2014
 *
 *		1) Firmware load to C31 and BMPS logic test OK.
 *		2) ddr trig and tx test
 *
 *
 *
 *	11/12/2014	: Firmware load to C31 and test OK.
 *		1) fixed firmware upgrade mode eeprom read fail.
 *		2) simpos input unit mm to um
 *		3) Y COD delta error count
 *
 *
 *	11/17/2014
 *		1) mps_RF_UserDump  added
 *		2) evg trig/sdi gtx loss/cell loss will be RF trip
 *
 *
 *	03/22/2015
 *		1) added BM BPM fault count logic
 *		2) reg28 input is fault error count value
 *
 *
 *	04/24/2015
 *		Beam test OK at 200 mA
 *	1) Known issue
 *		. BM trip -> Clearing -> RF trip by uBlaze PLC watchdog output
 *
 *	2) Fixing this problem
 *		.Added watchdog reset
 *		.Added counter check
 *
 *
 *	07/07/2016
 *		. uBlaze fail safe logic changed
 *
 *	6/25/20
 *		mps_FastDumpDelayEnable()
 *
 *
 *
 *************************************************************
 *************************************************************
 *************************************************************
	Heap size  512000
	Stack size 512000


	C:\home\FOFB\PA14.6\mps_111413\mps_111413.runs\impl_1>data2mem -bm "Mps_top_bd.bmm" -bt "Mps_top.bit"  -bd "bootload.elf" tag microblaze_0 -o b "aa.bit" -p xc6vlx240tff1156-1


  ***************** PROGRAMING NOTE **************************
  	Step 1: create mps_fpga.bit file with boot elf

	05/29/2014  did Mps_top.bmm is assigned ./hw
	Xilinx Tools -> Launch Shell
	data2mem -bm "Mps_top_bd.bmm" -bt "Mps_top.bit"  -bd "bootload.elf" tag system_i_microblaze_0 -o b "mps_fpga.bit" -p xc6vlx240tff1156-1

	step 2: create bin file
	iMPACT configuration
		BPI Flash
			Configure Single FPGA
			128 M Virtex-6

		fpgabit1.bin
			BIN x8 Swap Bits ON

 */



#include "xmk.h"
#include "os_config.h"
#include "sys/ksched.h"
#include "sys/init.h"
#include "config/config_param.h"
#include "stdio.h"
#include "xparameters.h"
#include "platform.h"
#include "platform_config.h"
#include <pthread.h>
#include <sys/types.h>
#include <xintc.h>
#include <sys/intr.h>

#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "netif/xadapter.h"

#include "ssp_eeprom.h"
#include "uart_print.h"
#include "ssp_status.h"


//evr and sdi
#include "evr.h"
#include "sdi_cc.h"
#include "fofb.h"
#include "fpga_system.h"
#include "console.h"
#include "version.h"
#include "cmd_modbus.h"

#include "fofb_bram_copy.h"
#include "pscEpics.h"


#define	FLASH		1

#if(FLASH)
#include <xilflash.h>
#endif


#define	THREAD_STACKSIZE	32768
#define FLASH_BASE_ADDRESS	XPAR_FLASH_MEM0_BASEADDR


//User Thread define
void* 	dBpm_Master_Thread(void *arg);
void 	dBpm_Network_Thread(void *p);
void	dBpm_ShellDebug_Thread();
void 	dBpm_TcpModbusSrv_Thread();
void 	dBpm_Epics_ddrWF_CommServer_Thread();
void 	dBpm_Matlab_Comm_Thread(void *);
extern void 	dBpm_UserTimer_Thread(void* arg);

extern void pscEpicsRx_thread();
extern void pscEpicsTx_thread();
extern void dBpm_TcpPscSrv_Thread();


#if(FLASH)
XFlash ConfigFlashInstance; /* XFlash Instance. */
#endif


SSP_Config_t SSP_Config;
struct netif server_netif;
net_config_t cfg_opts[7] = {
  /*       MAC                           IP                   SUBMASK                 GW         */
	{ { 0,0,0x35,0x55,0x55,0 },{ 10, 0, 142,  101 }, { 255, 255, 255, 0 }, { 10, 0, 142, 254 } },
    { { 0,0,0x35,0x55,0x55,1 },{ 10, 0, 142,  101 }, { 255, 255, 255, 0 }, { 10, 0, 142, 254 } },
    { { 0,0,0x35,0x55,0x55,2 },{ 10, 0, 142,  102 }, { 255, 255, 255, 0 }, { 10, 0, 142, 254 } },
    { { 0,0,0x35,0x55,0x55,3 },{ 10, 0, 142,  103 }, { 255, 255, 255, 0 }, { 10, 0, 142, 254 } },
    { { 0,0,0x35,0x55,0x55,4 },{ 10, 0, 142,  104 }, { 255, 255, 255, 0 }, { 10, 0, 142, 254 } },
    { { 0,0,0x35,0x55,0x55,5 },{ 10, 0, 142,  105 }, { 255, 255, 255, 0 }, { 10, 0, 142, 254 } },
    { { 0,0,0x35,0x55,0x55,6 },{ 10, 0, 142,  106 }, { 255, 255, 255, 0 }, { 10, 0, 142, 254 } },
};
static void define_ip4addr( struct ip_addr *dest, unsigned char a, unsigned char b, unsigned char c, unsigned char d ) {
  IP4_ADDR( dest, a, b, c, d );
}

unsigned int	evr_1HzEventCNT = 0;
/*
 *	Loss of connection 100 kHz Interrupt
 *	Normal 1 Hz
 *
 */
void	dBpm_EVR_ISR_Handler()
{	
	// must be check EVR SFP status
	epics_read_data.TS_SECOND = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*24);
	epics_read_data.TS_OFFSET = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*25);
	epics_read_data.EVR_TRIG_CNT = evr_1HzEventCNT++;
	
	//xil_printf("evr interrupt %d\r\n",epics_read_data.EVR_TRIG_CNT);
}




/*
 *
 */
void	EVR_InterruptSetup()
{
	int status;

	int_id_t int_EVR_id = XPAR_INTERRUPT_CNTLR_EVR_0_IP2INTC_IRPT0_EVENT_INTR;

    //Enable EVR IP Interrupts
    Set_I32_PLB( XPAR_EVR_0_BASEADDR+0x128, 0xF000000F);

	//Enable Global Interrupt Register
    //Set_I32_PLB( XPAR_EVR_0_BASEADDR+0x11C, 0xF000000F);
    //Register interrupt handler for sa_data
    if ((status = register_int_handler(int_EVR_id, dBpm_EVR_ISR_Handler, NULL)) != XST_SUCCESS)
		xil_printf ("EVR: Unable to register handler. Error code: %d.\r\n", status);
    else
		xil_printf ("EVR: Successfully registered a handler for EVR. \r\n");
	enable_interrupt(int_EVR_id);
}


/* Main Functions */
int main()
{
    init_platform();
    
    /* Initialize xilkernel */
    xilkernel_init();
    /* Create the dBpm_Master_Thread thread */
    xmk_add_static_thread(dBpm_Master_Thread, 0);    
    /* Start the kernel */
    xilkernel_start();    
    /* Never reached */
    cleanup_platform();
    
    return 0;
}



/* The master thread */
void* dBpm_Master_Thread(void *arg)
{
	
	NSLS2FOFB_StartUpMessage();

	dBpm_HardwareInit();
    
    /* initialize lwIP before calling sys_thread_new */
    lwip_init();

    /* any thread using lwIP should be created using sys_thread_new */
    sys_thread_new("NW_THREAD", dBpm_Network_Thread, NULL,
            THREAD_STACKSIZE,
            DEFAULT_THREAD_PRIO);
             	           
    return (void*)0;
}


/*
 *
 */
void dBpm_Network_Thread(void *p)
{
	    struct netif *netif;
		unsigned char *mac_addr, *ip_addr;
		unsigned char *ip_netmask, *ip_gateway;        
	    struct ip_addr ipaddr, netmask, gw;

	    //printf("Network thread start...>>>\r\n");
        netif = &server_netif;        
	  	mac_addr   = SSP_Config.net_cfg.mac_address;
	  	ip_addr    = SSP_Config.net_cfg.ip_address;
	  	ip_netmask = SSP_Config.net_cfg.ip_netmask;
	  	ip_gateway = SSP_Config.net_cfg.ip_gateway;
	  	  	
	  	define_ip4addr(&ipaddr, ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3] );
	  	define_ip4addr(&netmask, ip_netmask[0], ip_netmask[1], ip_netmask[2], ip_netmask[3] );
	  	define_ip4addr(&gw, ip_gateway[0], ip_gateway[1], ip_gateway[2], ip_gateway[3] );

#if 0
        /* the mac address of the board. this should be unique per board */
        //unsigned char mac_ethernet_address[] = { 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };
#if 0
        /* initliaze IP addresses to be used */
        IP4_ADDR(&ipaddr,  192, 168,  90, 10);
        IP4_ADDR(&netmask, 255, 255, 255,  0);
        IP4_ADDR(&gw,      192, 168,  90,  1);
#else
        IP4_ADDR(&ipaddr,  10, 0,   142, 11);
        IP4_ADDR(&netmask, 255, 255, 255,  0);
        IP4_ADDR(&gw,      10, 0,   142,  254);
#endif

#endif

        /* Add network interface to the netif_list, and set it as default */
        if (!xemac_add(netif, &ipaddr, &netmask, &gw, mac_addr, XPAR_HARD_ETHERNET_MAC_CHAN_0_BASEADDR)) {
            xil_printf("Error adding N/W interface\n\r");
            return;
        }

        netif_set_default(netif);

        /* specify that the network if is up */
        netif_set_up(netif);
                
        /* start packet receive thread - required for lwIP operation */
        sys_thread_new("xemacif_input_thread", (void(*)(void*))xemacif_input_thread, netif,
                THREAD_STACKSIZE,
                DEFAULT_THREAD_PRIO);
        sleep(20);
        sys_thread_new("console", dBpm_ShellDebug_Thread, 0,
             THREAD_STACKSIZE,
             DEFAULT_THREAD_PRIO);
			 

        sleep(20);
        sys_thread_new("timer_ts", dBpm_UserTimer_Thread, 0, 
            THREAD_STACKSIZE,
            DEFAULT_THREAD_PRIO);   
            
        sleep(20);
         //epics tx thread
         sys_thread_new("pscEpicsTx_thread", pscEpicsTx_thread, 0,
                  THREAD_STACKSIZE,
                  DEFAULT_THREAD_PRIO);

         sleep(20);
        //epics server thread for waveform
        sys_thread_new("pscEpicsRx_thread", pscEpicsRx_thread, 0,
            THREAD_STACKSIZE,            
            DEFAULT_THREAD_PRIO);

        sleep(20);
        //tx registers every 100 ms
        sys_thread_new("TcpPscSrv_Thread", (void *)&dBpm_TcpPscSrv_Thread, 0,
                 THREAD_STACKSIZE,
                 DEFAULT_THREAD_PRIO);

        return;
}



/*
 *
 *
 */
void	dBpm_HardwareInit()
{	
    int	rv;
    int i;

#define	EEPROM_READ		1
#define	EE_ADDR			0x0

	EE_Init();

  	rv = EE_ReadConfig( EE_ADDR, &SSP_Config);

  	if ( rv != 0 ) {
	 	safe_print(("\r\nEE_ReadConfig Error, Will Write Default ...\r\n"));	 	
	  	EE_DefaultConfig(&SSP_Config);	  		 		  	
	  	if ( EE_WriteConfig( EE_ADDR, &SSP_Config ) != 0)  {
		  	safe_print(("\r\nEE Write ERROR ...\n"));
	  	}  		
	  	safe_print(("\r\nEEPROM Write PASSED ...\r\n"));
  	}
  	else {
	  	EE_print_config (&SSP_Config, "------DBPM BOARD IP INFOMATION-----");
	 	//safe_print(("\r\n IP reading by EEPROM ...\r\n")); 	
  	} 

  	EepromDataRead(0);
	
    EvrInit();
    EVR_InterruptSetup();
    SdiCCLink_setup(0, 14, 32);		
	
    //FOFB
	fofb_RegInit();
	mps_init();

	//simulation mode
#define	SIM_MODE	0
	fofb_PosXyDataSim(0, SIM_MODE);
	mps_TrigIsSim(0, 0);		//0: EVR

	// 03/19/2014 added
	//cw/ccw packet delay
	// trig1 is source
	int trig1_width = (int)(DFE_CONTROL_REGS->sysInfo[5]);
	if( trig1_width < 1 || trig1_width > 3000) trig1_width = 1225;
	sdi_cw_pkt_delay(0, 5);
	sdi_cw_pkt_delay(0, 5);

	//NPI 1 x,y waveform tx length is 1 sec
	DFE_CONTROL_REGS->ioReg[60] = 10000;
	DFE_CONTROL_REGS->ioReg[72] = 5;
    CC_Sdi_Reset();
    mps_IOWatchDog_Reset(0,0);
    mps_OutputLatchClear(0, 1);

    //int sec = Get_uI32_PLB(XPAR_EVR_0_BASEADDR + 4*24);
    //TsCalc( sec, 0);
	
	
	//Clear Origin offset and BBA offset
	for(i=0; i<1024; i++) {
		Set_I_DDR2(BASE_REF_ORBIT+(i*4), 0);
		Set_I_DDR2(BASE_DDR_SRBPM_BBA_OFFSET+(i*4), 0);
	}	
}


//
void	dBpm_ShellDebug_Thread()
{
static short SystemRunningFlag=1;
static char UserResponse[32]; /* User input for questions */
char str[33];
int	cc_addr;

	xil_printf(("\r\nStarting Console server. \r\n"));

	while(SystemRunningFlag)
	{
IDLE:
		cc_addr = (int) (Get_I_DDR2(DDR2_EEPROM_PARAMETER_BASE+(2*4)) ); //read cell number from eeprom
		sprintf(str, "Eps:%d#", cc_addr);
		//
		if( !GetUserInput(str, UserResponse, sizeof(UserResponse)-1) ) goto IDLE;
		dBpm_ConsoleCmd_Processer(&UserResponse[0]);

	}
}
