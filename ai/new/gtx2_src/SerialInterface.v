`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    16:01:27 02/17/2010 
// Design Name: 
// Module Name:    serialInterface 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
`timescale 1ns / 1ps
`define DLY #1

`define EDK_IPCORE_MODE

//***********************************Entity Declaration************************

module SerialInterface #
(
	parameter EXAMPLE_SIM_MODE                          =   "FAST",  // Set to Fast Functional Simulation Model
	parameter EXAMPLE_SIM_GTXRESET_SPEEDUP              =   1,   // simulation setting for MGT smartmodel
	parameter EXAMPLE_SIM_PLL_PERDIV2                   =   9'h0c8, // simulation setting for MGT smartmodel
	parameter DEBUG										=   0
)
(
	GtxRefClock,		
	tile0_txoutclk0_i,
	tile0_txusrclk0_i,
	tile0_txusrclk20_i,
	txoutclk_dcm0_locked_i,
	tile0_plllkdet_i,
	CCWRxUsrClock,
	CWRxUsrClock,
	trigger,
	CWTxDataIn,
	CWTxCharIsKin,    
	CCWTxDataIn,
	CCWTxCharIsKin, 
	CWRxDataOut,
	CCWRxDataOut,
	CWRxCharIsKOut,
	CCWRxCharIsKOut,
	GTXRESET_IN,
	AllGTXReset,
	LinkStatus,
	RXN_IN,
	RXP_IN,
	TXN_OUT,
`ifdef	EDK_IPCORE_MODE
	TXP_OUT
`else	
	TXP_OUT,	
	CONTROL0
`endif	
);

//***********************************Ports Declaration*******************************

	input 					GtxRefClock;		 		// reference to all GTX's
	output				 	tile0_txoutclk0_i;		// to ref clock DCM
	input				 	tile0_txusrclk0_i;		// from ref clock DCM
	input					tile0_txusrclk20_i;		// from ref clock DCM
	input					txoutclk_dcm0_locked_i; // from ref clock DCM
	output					tile0_plllkdet_i;			// high if all GTX clocks are locked
	output		 	        CCWRxUsrClock;				// from RX recovered clock GTX0
	output					CWRxUsrClock;				// from RX recovered clock GTX1
	input					trigger;
	
	input           		GTXRESET_IN;				// used to establish byte order only
	input 		[31:0] 		CWTxDataIn;
	input   	[3:0]   	CWTxCharIsKin;    
	input   	[31:0]  	CCWTxDataIn;
	input   	[3:0]   	CCWTxCharIsKin;
	/* output */
	output reg [31:0] 		CWRxDataOut;
	output reg [31:0]  		CCWRxDataOut;	 
	output reg	[3:0]   	CWRxCharIsKOut;
	output reg	[3:0]   	CCWRxCharIsKOut;
	output  				AllGTXReset;				// goes low after all GTX resets complete
	output		[7:0]		LinkStatus;
	input   	[1:0]   	RXN_IN;
	input   	[1:0]   	RXP_IN;
	output  	[1:0]   	TXN_OUT;
	output  	[1:0]   	TXP_OUT;
`ifndef	EDK_IPCORE_MODE	
	inout [35 : 0] CONTROL0;
`endif
    
//************************** Register Declarations ****************************

	reg             tile0_tx_resetdone0_r;
	reg             tile0_tx_resetdone0_r2;
	reg             tile0_rx_resetdone0_r;
	reg             tile0_rx_resetdone0_r2;
	reg				tile0_rx_resetdone0_r3;
	reg             tile0_tx_resetdone1_r;
	reg             tile0_tx_resetdone1_r2;
	reg             tile0_rx_resetdone1_r;
	reg             tile0_rx_resetdone1_r2;
	reg				tile0_rx_resetdone1_r3;
	reg 			ByteOrder;
	reg 			ByteOrder2;
	reg 		[31:0]  	CCWRxDataBuffer1;
	reg 		[31:0]  	CCWRxDataBuffer2;
	reg 		[31:0]  	CWRxDataBuffer1;
	reg 		[31:0]  	CWRxDataBuffer2;
	reg			[3:0]	  	CCWRxCharIsKOutBuffer1;
	reg			[3:0]	  	CCWRxCharIsKOutBuffer2;
	reg			[3:0]	  	CWRxCharIsKOutBuffer1;
	reg			[3:0]	  	CWRxCharIsKOutBuffer2;

//**************************** Wire Declarations ******************************

    //------------------------ MGT Wrapper Wires ------------------------------
    

    //________________________________________________________________________
    //________________________________________________________________________
    //TILE0   (X0Y0)

	//---------------------- Loopback and Powerdown Ports ----------------------
	wire    [2:0]   tile0_loopback0_i;
	wire    [2:0]   tile0_loopback1_i;
	//--------------------- Receive Ports - 8b10b Decoder ----------------------
	wire		[3:0]	  tile0_rxcharisk0_i;
	wire		[3:0]	  tile0_rxcharisk1_i;
	wire    [3:0]   tile0_rxdisperr0_i;
	wire    [3:0]   tile0_rxdisperr1_i;
	wire    [3:0]   tile0_rxnotintable0_i;
	wire    [3:0]   tile0_rxnotintable1_i;
	//----------------- Receive Ports - Clock Correction Ports -----------------
	wire    [2:0]   tile0_rxclkcorcnt0_i;
	wire    [2:0]   tile0_rxclkcorcnt1_i;
	//------------- Receive Ports - Comma Detection and Alignment --------------
	wire    [3:0]   tile0_rxchariscomma0_out;
	wire    [3:0]   tile0_rxchariscomma1_out;
	reg             tile0_rxenmcommaalign0_i;
	reg             tile0_rxenmcommaalign1_i;
	reg             tile0_rxenpcommaalign0_i;
	reg             tile0_rxenpcommaalign1_i;
	//----------------- Receive Ports - RX Data Path interface -----------------
	
	//----- Receive Ports - RX Driver,OOB signalling,Coupling and Eq.,CDR ------
	wire    [1:0]   tile0_rxeqmix0_i;
	wire    [1:0]   tile0_rxeqmix1_i;
	//------------- Receive Ports - RX Loss-of-sync State Machine --------------
	wire    [1:0]   tile0_rxlossofsync0_i;
	wire    [1:0]   tile0_rxlossofsync1_i;
	//------------------- Shared Ports - Tile and PLL Ports --------------------
	wire            tile0_gtxreset_i;
	wire    		  tile0_plllkdet_i;
	wire            tile0_refclkout_i;
	wire            tile0_resetdone0_i;
	wire            tile0_resetdone1_i;
	//-------------- Transmit Ports - 8b10b Encoder Control Ports --------------
	wire            tile0_txoutclk0_i;
	wire            tile0_txoutclk1_i;
	//------------- Transmit Ports - TX Driver and OOB signalling --------------
	wire    [2:0]   tile0_txdiffctrl0_i;
	wire    [2:0]   tile0_txdiffctrl1_i;
	wire    [2:0]   tile0_txpreemphasis0_i;
	wire    [2:0]   tile0_txpreemphasis1_i;


	//----------------------------- Global Signals -----------------------------
	wire            tile0_tx_system_reset0_c;
	wire            tile0_rx_system_reset0_c;
	wire            tile0_tx_system_reset1_c;
	wire            tile0_rx_system_reset1_c;
	wire            tied_to_ground_i;
	wire    [63:0]  tied_to_ground_vec_i;
	wire            tied_to_vcc_i;
	wire    [7:0]   tied_to_vcc_vec_i;
	wire            drp_clk_in_i;
	
	wire            tile0_refclkout_bufg_i;
		
	//--------------------------- User Clocks ---------------------------------
	wire            txoutclk_dcm0_locked_i;
	wire            txoutclk_dcm0_reset_i;
	wire            tile0_txoutclk0_to_dcm_i;
	wire            tile0_refclk_i;
	
	wire [31:0]  tile0_rxdata0_i;
	wire [31:0]  tile0_rxdata1_i;
	
	wire	GTX_CCRx0Clk;
	wire	GTX_CCWRXClk;
	
	//**************************** Main Body of Code *******************************
	
	//  Static signal Assigments    
	assign tied_to_ground_i             = 1'b0;
	assign tied_to_ground_vec_i         = 64'h0000000000000000;
	assign tied_to_vcc_i                = 1'b1;
	assign tied_to_vcc_vec_i            = 8'hff;

    //---------------------Dedicated GTX Reference Clock Inputs ---------------
    // The dedicated reference clock inputs you selected in the GUI are implemented using
    // IBUFDS instances.
    //
    // In the UCF file for this example design, you will see that each of
    // these IBUFDS instances has been LOCed to a particular set of pins. By LOCing to these
    // locations, we tell the tools to use the dedicated input buffers to the GTX reference
    // clock network, rather than general purpose IOs. To select other pins, consult the 
    // Implementation chapter of UG196, or rerun the wizard.
    //
    // This network is the highest performace (lowest jitter) option for providing clocks
    // to the GTX transceivers.


    //--------------------------------- User Clocks ---------------------------
    
    // The clock resources in this section were added based on userclk source selections on
    // the Latency, Buffering, and Clocking page of the GUI. A few notes about user clocks:
    // * The userclk and userclk2 for each GTX datapath (TX and RX) must be phase aligned to 
    //   avoid data errors in the fabric interface whenever the datapath is wider than 10 bits
    // * To minimize clock resources, you can share clocks between GTXs. GTXs using the same frequency
    //   or multiples of the same frequency can be accomadated using DCMs and PLLs. Use caution when
    //   using RXRECCLK as a clock source, however - these clocks can typically only be shared if all
    //   the channels using the clock are receiving data from TX channels that share a reference clock 
    //   source with each other.

    BUFG rxrecclk_mmcm1_bufg_i
    (
        .I                              (tile0_rxrecclk0_i),
        .O                              (gtx0_rxrecclk_to_mmcm_i)
    );

    assign  rxrecclk_dcm1_reset_i           =  !gtx0_rx_lock_detect;
    MGT_USRCLK_SOURCE_MMCM #
    (
        .MULT                           (8.0),
        .DIVIDE                         (2),
        .CLK_PERIOD                     (4.0),
        .OUT0_DIVIDE                    (8.0),
        .OUT1_DIVIDE                    (4),
        .OUT2_DIVIDE                    (1),
        .OUT3_DIVIDE                    (1)
    )
    rxrecclk_mmcm1_i
    (
        .CLK0_OUT                       (CCWRxUsrClock),	
        //.CLK0_OUT                       (GTX_CCWRx0Clk),        
        .CLK1_OUT                       (tile0_rxusrclk0_i),
        .CLK2_OUT                       (),
        .CLK3_OUT                       (),
        .CLK_IN                         (gtx0_rxrecclk_to_mmcm_i),
        .MMCM_LOCKED_OUT                (rxrecclk_dcm1_locked_i),
        .MMCM_RESET_IN                  (rxrecclk_dcm1_reset_i)
    );



    BUFG rxrecclk_mmcm2_bufg_i
    (
        .I                              (tile0_rxrecclk1_i),
        .O                              (gtx1_rxrecclk_to_mmcm_i)
    );

    assign  rxrecclk_dcm2_reset_i           =  !gtx1_rx_lock_detect;
    MGT_USRCLK_SOURCE_MMCM #
    (
        .MULT                           (8.0),
        .DIVIDE                         (2),
        .CLK_PERIOD                     (4.0),
        .OUT0_DIVIDE                    (8.0),
        .OUT1_DIVIDE                    (4),
        .OUT2_DIVIDE                    (1),
        .OUT3_DIVIDE                    (1)
    )
    rxrecclk_mmcm2_i
    (
        .CLK0_OUT                       (CWRxUsrClock),  
        //.CLK0_OUT                       (GTX_CCRx0Clk),
        .CLK1_OUT                       (tile0_rxusrclk1_i),
        .CLK2_OUT                       (),
        .CLK3_OUT                       (),
        .CLK_IN                         (gtx1_rxrecclk_to_mmcm_i),
        .MMCM_LOCKED_OUT                (rxrecclk_dcm2_locked_i),
        .MMCM_RESET_IN                  (rxrecclk_dcm2_reset_i)
    );



/////////////////////
`ifdef RX_GBUF
    BUFG GTX_CCrx_clk		// kha 125 MHz GTX Rx clock for remote fifo read
    (
       .O             (CWRxUsrClock),
       .I             (GTX_CCRx0Clk)
    );    
    BUFG GTX_CCWrx_clk		// kha 125 MHz GTX Rx clock for remote fifo read
    (
       .O             (CCWRxUsrClock),
       .I             (GTX_CCWRx0Clk)
    );    
`endif
    
    //--------------------------- The GTX Wrapper -----------------------------
    
    // Use the instantiation template in the project directory to add the GTX wrapper to your design.
    // In this example, the wrapper is wired up for basic operation with a frame generator and frame 
    // checker. The GTXs will reset, then attempt to align and transmit data. If channel bonding is 
    // enabled, bonding should occur after alignment.
 
    
    // Wire all PLLLKDET signals to the top level as output ports
    assign tile0_plllkdet_i = rxrecclk_dcm2_locked_i | rxrecclk_dcm1_locked_i | gtx0_rx_lock_detect
										| gtx0_tx_lock_detect | gtx1_rx_lock_detect | gtx1_tx_lock_detect;

    GTX2 #
    (
        .WRAPPER_SIM_GTXRESET_SPEEDUP   (0)      // Set this to 1 for simulation
    )
    gtx_i
    (
        //_____________________________________________________________________
        //_____________________________________________________________________
        //GTX0  (X0Y0)

        //---------------------- Loopback and Powerdown Ports ----------------------
        .GTX0_LOOPBACK_IN               (tile0_loopback0_i),
        //--------------------- Receive Ports - 8b10b Decoder ----------------------
		  .GTX0_RXCHARISCOMMA_OUT			 (tile0_rxchariscomma0_out),
        .GTX0_RXCHARISK_OUT             (tile0_rxcharisk0_i),
        .GTX0_RXDISPERR_OUT             (tile0_rxdisperr0_i),
        .GTX0_RXNOTINTABLE_OUT          (tile0_rxnotintable0_i),
        //------------- Receive Ports - Comma Detection and Alignment --------------
        .GTX0_RXCOMMADET_OUT            (),
        .GTX0_RXENMCOMMAALIGN_IN        (tile0_rxenmcommaalign0_i),
        .GTX0_RXENPCOMMAALIGN_IN        (tile0_rxenpcommaalign0_i),
        //----------------- Receive Ports - RX Data Path interface -----------------
        .GTX0_RXDATA_OUT                (tile0_rxdata0_i),
        .GTX0_RXRECCLK_OUT              (tile0_rxrecclk0_i),
        .GTX0_RXUSRCLK_IN               (tile0_rxusrclk0_i),
        .GTX0_RXUSRCLK2_IN              (CCWRxUsrClock),
        //----- Receive Ports - RX Driver,OOB signalling,Coupling and Eq.,CDR ------
        .GTX0_RXEQMIX_IN                (tile0_rxeqmix0_i),
        .GTX0_RXN_IN                    (RXN_IN[0]),
        .GTX0_RXP_IN                    (RXP_IN[0]),
        //------------- Receive Ports - RX Loss-of-sync State Machine --------------
        .GTX0_RXLOSSOFSYNC_OUT          (tile0_rxlossofsync0_i),
        //---------------------- Receive Ports - RX PLL Ports ----------------------
        .GTX0_GTXRXRESET_IN             (!txoutclk_dcm0_locked_i),
        .GTX0_MGTREFCLKRX_IN            (GtxRefClock),
        .GTX0_PLLRXRESET_IN             (1'b0),
        .GTX0_RXPLLLKDET_OUT            (gtx0_rx_lock_detect),
        .GTX0_RXRESETDONE_OUT           (gtx0_rx_reset_done),
        //-------------- Transmit Ports - 8b10b Encoder Control Ports --------------
        .GTX0_TXCHARISK_IN              (CWTxCharIsKin),
        //---------------- Transmit Ports - TX Data Path interface -----------------
        .GTX0_TXDATA_IN                 (CWTxDataIn),
        .GTX0_TXOUTCLK_OUT              (tile0_txoutclk0_i),
        .GTX0_TXUSRCLK_IN               (tile0_txusrclk0_i),
        .GTX0_TXUSRCLK2_IN              (tile0_txusrclk20_i),
        //-------------- Transmit Ports - TX Driver and OOB signaling --------------
        .GTX0_TXDIFFCTRL_IN             (tile0_txdiffctrl0_i),
        .GTX0_TXN_OUT                   (TXN_OUT[0]),
        .GTX0_TXP_OUT                   (TXP_OUT[0]),
        .GTX0_TXPOSTEMPHASIS_IN         (5'b00000),
        //------------- Transmit Ports - TX Driver and OOB signalling --------------
        .GTX0_TXPREEMPHASIS_IN          (tile0_txpreemphasis0_i),
        //--------------------- Transmit Ports - TX PLL Ports ----------------------
        .GTX0_GTXTXRESET_IN             (tile0_gtxreset_i),
        .GTX0_MGTREFCLKTX_IN            (GtxRefClock),
        .GTX0_PLLTXRESET_IN             (1'b0),
        .GTX0_TXPLLLKDET_OUT            (gtx0_tx_lock_detect),
        .GTX0_TXRESETDONE_OUT           (gtx0_tx_reset_done),


        //_____________________________________________________________________
        //_____________________________________________________________________
        //GTX1  (X0Y1)

        //---------------------- Loopback and Powerdown Ports ----------------------
        .GTX1_LOOPBACK_IN               (tile0_loopback1_i),
        //--------------------- Receive Ports - 8b10b Decoder ----------------------
		  .GTX1_RXCHARISCOMMA_OUT			 (tile0_rxchariscomma1_out),
        .GTX1_RXCHARISK_OUT             (tile0_rxcharisk1_i),
        .GTX1_RXDISPERR_OUT             (tile0_rxdisperr1_i),
        .GTX1_RXNOTINTABLE_OUT          (tile0_rxnotintable1_i),
        //------------- Receive Ports - Comma Detection and Alignment --------------
        .GTX1_RXCOMMADET_OUT            (),
        .GTX1_RXENMCOMMAALIGN_IN        (tile0_rxenmcommaalign1_i),
        .GTX1_RXENPCOMMAALIGN_IN        (tile0_rxenpcommaalign1_i),
        //----------------- Receive Ports - RX Data Path interface -----------------
        .GTX1_RXDATA_OUT                (tile0_rxdata1_i),
        .GTX1_RXRECCLK_OUT              (tile0_rxrecclk1_i),
        .GTX1_RXUSRCLK_IN               (tile0_rxusrclk1_i),
        .GTX1_RXUSRCLK2_IN              (CWRxUsrClock),
        //----- Receive Ports - RX Driver,OOB signalling,Coupling and Eq.,CDR ------
        .GTX1_RXEQMIX_IN                (tile0_rxeqmix1_i),
        .GTX1_RXN_IN                    (RXN_IN[1]),
        .GTX1_RXP_IN                    (RXP_IN[1]),
        //------------- Receive Ports - RX Loss-of-sync State Machine --------------
        .GTX1_RXLOSSOFSYNC_OUT          (tile0_rxlossofsync1_i),
        //---------------------- Receive Ports - RX PLL Ports ----------------------
        .GTX1_GTXRXRESET_IN             (!txoutclk_dcm0_locked_i),
        .GTX1_MGTREFCLKRX_IN            (GtxRefClock),
        .GTX1_PLLRXRESET_IN             (1'b0),
        .GTX1_RXPLLLKDET_OUT            (gtx1_rx_lock_detect),
        .GTX1_RXRESETDONE_OUT           (gtx1_rx_reset_done),
        //-------------- Transmit Ports - 8b10b Encoder Control Ports --------------
        .GTX1_TXCHARISK_IN              (CCWTxCharIsKin),
        //---------------- Transmit Ports - TX Data Path interface -----------------
        .GTX1_TXDATA_IN                 (CCWTxDataIn),
        .GTX1_TXOUTCLK_OUT              (tile0_txoutclk1_i),
        .GTX1_TXUSRCLK_IN               (tile0_txusrclk0_i),
        .GTX1_TXUSRCLK2_IN              (tile0_txusrclk20_i),
        //-------------- Transmit Ports - TX Driver and OOB signaling --------------
        .GTX1_TXDIFFCTRL_IN             (tile0_txdiffctrl1_i),
        .GTX1_TXN_OUT                   (TXN_OUT[1]),
        .GTX1_TXP_OUT                   (TXP_OUT[1]),
        .GTX1_TXPOSTEMPHASIS_IN         (5'b00000),
        //------------- Transmit Ports - TX Driver and OOB signalling --------------
        .GTX1_TXPREEMPHASIS_IN          (tile0_txpreemphasis1_i),
        //--------------------- Transmit Ports - TX PLL Ports ----------------------
        .GTX1_GTXTXRESET_IN             (tile0_gtxreset_i),
        .GTX1_MGTREFCLKTX_IN            (GtxRefClock),
        .GTX1_PLLTXRESET_IN             (1'b0),
        .GTX1_TXPLLLKDET_OUT            (gtx1_tx_lock_detect),
        .GTX1_TXRESETDONE_OUT           (gtx1_tx_reset_done)
    );
 

    //------------------------ User Module Resets -----------------------------
    // All the User Modules i.e. FRAME_GEN, FRAME_CHECK and the sync modules
    // are held in reset till the RESETDONE goes high. 
    // The RESETDONE is registered a couple of times on *USRCLK2 and connected 
    // to the reset of the modules
    
    always @(posedge tile0_txusrclk20_i or negedge gtx0_rx_reset_done)

    begin
        if (!gtx0_rx_reset_done )
        begin
            tile0_rx_resetdone0_r    <=    1'b0;
            tile0_rx_resetdone0_r2   <=    1'b0;
        end
        else
        begin
            tile0_rx_resetdone0_r    <=    gtx0_rx_reset_done;
            tile0_rx_resetdone0_r2   <=    tile0_rx_resetdone0_r;
        end
    end
    always @(posedge tile0_txusrclk20_i) tile0_rx_resetdone0_r3 <= tile0_rx_resetdone0_r2;
    
    always @(posedge tile0_txusrclk20_i or negedge gtx0_tx_reset_done)
    begin
        if (!gtx0_tx_reset_done )
        begin
            tile0_tx_resetdone0_r    <=    1'b0;
            tile0_tx_resetdone0_r2   <=    1'b0;
        end
        else
        begin
            tile0_tx_resetdone0_r    <=    gtx0_tx_reset_done;
            tile0_tx_resetdone0_r2   <=    tile0_tx_resetdone0_r;
        end
    end

    always @(posedge tile0_txusrclk20_i or negedge gtx1_rx_reset_done)
    begin
        if (!gtx1_rx_reset_done )
        begin
            tile0_rx_resetdone1_r    <=    1'b0;
            tile0_rx_resetdone1_r2   <=    1'b0;
        end
        else
        begin
            tile0_rx_resetdone1_r    <=    gtx1_rx_reset_done;
            tile0_rx_resetdone1_r2   <=    tile0_rx_resetdone1_r;
        end
    end
    always @(posedge tile0_txusrclk20_i) tile0_rx_resetdone1_r3 <= tile0_rx_resetdone1_r2;
    
    
    always @(posedge tile0_txusrclk20_i or negedge gtx1_tx_reset_done)

    begin
        if (!gtx1_tx_reset_done )
        begin
            tile0_tx_resetdone1_r    <=    1'b0;
            tile0_tx_resetdone1_r2   <=    1'b0;
        end
        else
        begin
            tile0_tx_resetdone1_r    <=    gtx1_tx_reset_done;
            tile0_tx_resetdone1_r2   <=    tile0_tx_resetdone1_r;
        end
    end


   // Drive the enamcommaalign port of the mgt for alignment
    always @(posedge tile0_txusrclk20_i)
        if(tile0_rx_system_reset0_c)   tile0_rxenmcommaalign0_i   <=      1'b0;
        else              					tile0_rxenmcommaalign0_i   <=      1'b1;

    // Drive the enapcommaalign port of the mgt for alignment
    always @(posedge tile0_txusrclk20_i)
        if(tile0_rx_system_reset0_c)  	tile0_rxenpcommaalign0_i   <=      1'b0;
        else              					tile0_rxenpcommaalign0_i   <=      1'b1;
   // Drive the enamcommaalign port of the mgt for alignment
    always @(posedge tile0_txusrclk20_i)
        if(tile0_rx_system_reset1_c)   tile0_rxenmcommaalign1_i   <=      1'b0;
        else              					tile0_rxenmcommaalign1_i   <=      1'b1;

    // Drive the enapcommaalign port of the mgt for alignment
    always @(posedge tile0_txusrclk20_i)
        if(tile0_rx_system_reset1_c)  	tile0_rxenpcommaalign1_i   <=      1'b0;
        else              					tile0_rxenpcommaalign1_i   <=      1'b1;

	 // need to detect an incomming comma and rearrange the data if the comma is
	 // aligned to byte 2 then set the Byte order bit and shift the data around
    always @(posedge CCWRxUsrClock)
	 begin
		if (GTXRESET_IN) ByteOrder <= 1'b0;
		else if (tile0_rxchariscomma0_out != 4'b0000)
			begin
				if (tile0_rxchariscomma0_out == 4'b0001) ByteOrder <= 1'b0;
				else ByteOrder <= 1'b1;
			end
		else ByteOrder <=ByteOrder;
	 end

    always @(posedge CWRxUsrClock)
	 begin
		if (GTXRESET_IN) ByteOrder2 <= 1'b0;
		else if (tile0_rxchariscomma1_out != 4'b0000)
			begin
				if (tile0_rxchariscomma1_out == 4'b0001) ByteOrder2 <= 1'b0;
				else ByteOrder2 <= 1'b1;
			end
		else ByteOrder2 <=ByteOrder2;
	 end
	 
	 // need to pipeline the data so that the bytes can be realigned as well as
	 // adjust the position of the character is k signal
    always @(posedge CCWRxUsrClock) 
	 begin
		CCWRxDataBuffer1 <= tile0_rxdata0_i; 
		CCWRxDataBuffer2 <= CCWRxDataBuffer1;
		
		CCWRxCharIsKOutBuffer1 <= tile0_rxcharisk0_i;
		CCWRxCharIsKOutBuffer2 <= CCWRxCharIsKOutBuffer1;
	 end
	 
    always @(posedge CWRxUsrClock) 
	 begin
		CWRxCharIsKOutBuffer1 <= tile0_rxcharisk1_i;
		CWRxCharIsKOutBuffer2 <= CWRxCharIsKOutBuffer1;
		
		CWRxDataBuffer1 <= tile0_rxdata1_i; 
		CWRxDataBuffer2 <= CWRxDataBuffer1;
		
	 end

	 // using the ByteOrder bit rearange the data and k character if needed
    always @(posedge CCWRxUsrClock)
	 begin
		CCWRxDataOut <= (ByteOrder) ? {CCWRxDataBuffer1[15:0],CCWRxDataBuffer2[31:16]} 
											: tile0_rxdata0_i;
											
		CCWRxCharIsKOut <= (ByteOrder) ? {CCWRxCharIsKOutBuffer1[1:0],CCWRxCharIsKOutBuffer2[3:2]}
											: tile0_rxcharisk0_i;
	 end
	 
    always @(posedge CWRxUsrClock)
	 begin
		CWRxDataOut <= (ByteOrder2) ? {CWRxDataBuffer1[15:0],CWRxDataBuffer2[31:16]} 
											: tile0_rxdata1_i;
											
		CWRxCharIsKOut <= (ByteOrder2) ? {CWRxCharIsKOutBuffer1[1:0],CWRxCharIsKOutBuffer2[3:2]}
											: tile0_rxcharisk1_i;
											
	 end
	 

    // If Chipscope is not being used, drive GTX reset signal
    // from the top level ports
    assign  tile0_gtxreset_i = GTXRESET_IN;

    // assign resets for frame_gen modules
    assign  tile0_tx_system_reset0_c = !tile0_tx_resetdone0_r2;
    assign  tile0_tx_system_reset1_c = !tile0_tx_resetdone1_r2;

    // assign resets for frame_check modules
    assign  tile0_rx_system_reset0_c = !tile0_rx_resetdone0_r3;
    assign  tile0_rx_system_reset1_c = !tile0_rx_resetdone1_r3;

	 assign  AllGTXReset = (tile0_rx_system_reset1_c | tile0_rx_system_reset0_c |
								  tile0_tx_system_reset1_c | tile0_tx_system_reset0_c);

	
    assign  gtxreset_i                      =  tied_to_ground_i;
    assign  user_tx_reset_i                 =  tied_to_ground_i;
    assign  user_rx_reset_i                 =  tied_to_ground_i;
    assign  tile0_loopback0_i               =  3'b000; //tied_to_ground_vec_i[2:0];
    assign  tile0_txdiffctrl0_i             =  tied_to_ground_vec_i[2:0];
    assign  tile0_txpreemphasis0_i          =  tied_to_ground_vec_i[2:0];
    assign  tile0_rxeqmix0_i                =  tied_to_ground_vec_i[1:0];
    assign  tile0_loopback1_i               =  3'b000; //tied_to_ground_vec_i[2:0];
    assign  tile0_txdiffctrl1_i             =  tied_to_ground_vec_i[2:0];
    assign  tile0_txpreemphasis1_i          =  tied_to_ground_vec_i[2:0];
    assign  tile0_rxeqmix1_i                =  tied_to_ground_vec_i[1:0];
	 
	 // status information
	 assign LinkStatus[0] = rxrecclk_dcm1_locked_i;
	 assign LinkStatus[1] = rxrecclk_dcm2_locked_i;
	 assign LinkStatus[3:2] = tile0_rxlossofsync0_i;
	 assign LinkStatus[5:4] = tile0_rxlossofsync1_i;
	 assign LinkStatus[6] = ByteOrder;
	 assign LinkStatus[7] = ByteOrder2;

`ifdef  AAAAA	 
	generate
	if (DEBUG) 
   begin : chipscope		
		reg [2:0]quickCount;
		always @(posedge tile0_rxrecclk0_i) quickCount <= quickCount + 1;

		reg [2:0]quickCount2;
		always @(posedge tile0_txusrclk20_i) quickCount2 <= quickCount2 + 1;
		
		wire [255:0] TRIG0;
		
		assign TRIG0[31:0] = CCWRxDataOut;
		assign TRIG0[35:32] = CCWRxCharIsKOut;
		assign TRIG0[67:36] = CWRxDataOut; 
		assign TRIG0[71:68] = CWRxCharIsKOut;
		assign TRIG0[103:72] = CCWTxDataIn; 
		assign TRIG0[107:104] = CCWTxCharIsKin;            
		assign TRIG0[139:108] = CWTxDataIn; 
		assign TRIG0[143:140] = CWTxCharIsKin;            
		assign TRIG0[144] = tile0_plllkdet_i;
		assign TRIG0[145] = tile0_resetdone0_i;
		assign TRIG0[146] = tile0_rxdisperr0_i;
		assign TRIG0[147] = tile0_rxnotintable0_i;
		assign TRIG0[149] = trigger;
		assign TRIG0[151:150] = tile0_rxlossofsync0_i;
		assign TRIG0[153:152] = tile0_rxlossofsync1_i;
		assign TRIG0[154] = rxrecclk_dcm1_locked_i;
		assign TRIG0[155] = rxrecclk_dcm2_locked_i;
		assign TRIG0[187:156] = tile0_rxdata0_i;
		assign TRIG0[191:188] = tile0_rxchariscomma0_out;
		assign TRIG0[223:192] = tile0_rxdata1_i;
		assign TRIG0[227:224] = tile0_rxchariscomma1_out;
		assign TRIG0[228] = ByteOrder;
		assign TRIG0[229] = ByteOrder2;
		assign TRIG0[255:230] = 0;
		ila myIla (
			 .CONTROL(CONTROL0), // INOUT BUS [35:0]
			 .CLK(tile0_rxrecclk0_i), // IN
			 .TRIG0(TRIG0) // IN BUS [99:0]
		);
	end
	endgenerate
`endif	 	 
	 
endmodule
