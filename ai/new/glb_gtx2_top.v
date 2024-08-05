`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/16/2013 09:51:53 AM
// Design Name: 
// Module Name: glb_gtx2_top
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module glb_gtx2_top(
	input		Reset,
	input		trig,
	input		clk,
	input 		GtxRefClock,
	input [11:0] SourcePacketSizeIn,		// if the device is a source define packet size
	input [11:0] SourcePacketSAddrIn,		// cell controller address for packet, Kha think Local Start Address	
	input		LocalDataValid,
	input		[31:0] LocalData,	
	// RocketIO serial input / output
	input 		[1:0] 	RXN_IN,
	input 		[1:0] 	RXP_IN,
	output 		[1:0] 	TXN_OUT,
	output 		[1:0] 	TXP_OUT,
	output	TxClock125,
	output [15:0] LinkDataAddress,	// address from data packet being processed
	output [11:0] LinkPacketAddress,// address for forwarding packet if cell controller
	output [11:0] LinkPacketLength,	// legth of the data packet being processed
	output [31:0] LinkData,	 			// data from the packet being processed
	output LinkDataValid,				// data valid to user application
	output LinkStartOfPacket,
	output LinkEndOfPacket,
	//
	output	CWRxUsrClock,
	 output [31:0] CWRxDataOut,
	 output [31:0] CCWRxDataOut,
	 output CWRxCharIsKOut,
	 output	CCWRxCharIsKOut,	
		
	output		[11:0]   dbg
    );
    
	// Input Packet Parsing and flow control for the RocketIO input stream
	parameter CommaChar 	= 8'h3C; //8'b 00111100;		// k28.1 character for wors alignment
	parameter MpsFaultChar  = 8'h1C; //8'b 00011100; 		// k28.0 character for clock correction
	 
	wire	tile0_plllkdet_i;
	wire	tile0_txoutclk0_i;
	wire	tile0_txoutclk0_to_dcm_i;
	wire	TxClock, TxClock2x;
	//wire	CWRxUsrClock;
	wire	CCWRxUsrClock;
	
	BUFG txoutclk_dcm0_bufg_i
    (
        .I                              (tile0_txoutclk0_i),
        .O                              (tile0_txoutclk0_to_dcm_i)
    );
	
    assign  txoutclk_mmcm0_reset_i               =  !tile0_plllkdet_i;
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
    txoutclk_mmcm0_i
    (
        .CLK0_OUT                       (TxClock),
        .CLK1_OUT                       (TxClock2x),
        .CLK2_OUT                       (),
        .CLK3_OUT                       (),
        .CLK_IN                         (tile0_txoutclk0_to_dcm_i),
        .MMCM_LOCKED_OUT                (txoutclk_dcm0_locked_i),
        .MMCM_RESET_IN                  (txoutclk_mmcm0_reset_i)
    );
        
	assign	TxClock125 = TxClock;	
        
	wire	[7:0]	LinkStatus;	
	wire 	[3:0] 	LocalCWRxCharIsKin;
	wire 	[3:0] 	LocalCCWRxCharIsKin;
	wire 	[31:0] 	CWRIOdataIn;
	wire 	[31:0] 	CCWRIOdataIn;
	
	reg	[31:0] RIOdataOut;
	reg	CharIsKOut;
	reg MpsFaultReg;
	wire	MpsFaultIn;
	wire	ResetOut;
	wire [33:0] IdleSeqOut;
	
	
	assign MpsFaultIn = 1'b0;
	
	
	wire [33:0] LocalBufferDataOut;
	wire LocalPacketDataValid, LocalPacketReady, LocalBufferEmpty;
	wire LocalBufferFlush;
	reg LocalPacketsInQue;
	
	 // This subdesign implements the interface to local data sources
	 LocalPacketProcessor myLocalPacketProcessor (
		 .FillClock(clk), 				//117 MHz same as ADC Fs
		 .SourcePacketSizeIn( SourcePacketSizeIn ), 	//
		 .SourcePacketDestIn( SourcePacketSAddrIn ), 	//
		 .DataIn(LocalData), 
		 .DataInValid(LocalDataValid), 
		 .Reset(Reset), 					//Fifo output for local packet from x,y data
		 .FlushClock(TxClock), 		//Fifo read clock
		 .FlushEnable(LocalBufferFlush), 
		 .DataOut(LocalBufferDataOut), 		//local data out. [33:0] for tx
		 .PacketReady(LocalPacketReady), 
		 .BufferEmpty(LocalBufferEmpty), 
		 .LocalPacketDataValid(LocalPacketDataValid),
		 .full() 
		 );
		 
		 
	 // Local packets in Que
	 always @ (posedge TxClock)
	 begin
		if (Reset == 1'b1) 
			LocalPacketsInQue <= 1'b0;
		
		else if (LocalPacketReady == 1'b1)
			LocalPacketsInQue <= 1'b1;
			
		else if (LocalBufferEmpty == 1'b1)
			LocalPacketsInQue <= 1'b0;
			
		else
			LocalPacketsInQue <= LocalPacketsInQue;
	 end
	 assign LocalBufferFlush = LocalPacketsInQue;
	 
	 // RocketIO data stream to the stream processor and other nodes on the link: If there is a machine
	 // protection fault the output data is a k28.0 character only in this way other nodes on the link
	 // will get the fault as top priority. When the fault is cleared normal data transmition will resume
	 // In the Idle state a k28.5 comma character is transmitted at ~3kHz for link data alignment. When 
	 // in Local mode the data from this node is transmitted and in remote the data from the adjacent 
	 // node is transmitted (forwarded). Finally if the link is broken the the loss of synch state machine 
	 // goes to state 3. In that case zeroes are broadcast to avoid sending out garbage.
    always @ (posedge TxClock)
	 begin	 
		//Data
	 	RIOdataOut <= (LocalPacketDataValid == 1'b0 ) ? IdleSeqOut[33:2] : LocalBufferDataOut[33:2];
		//Start Char
		CharIsKOut <= (LocalPacketDataValid == 1'b0 ) ? IdleSeqOut[1]    : LocalBufferDataOut[1];
	 end
	 
	 // create a series of 1024 zeroes with two embedded characters k28.0 for RIOtxUserClk2correction
	 // and k28.1 for data alignment
	 
	 reg [15:0] IdleSeqCounter;
	 always @ (posedge TxClock)
	 begin
		if (Reset == 1'b1) IdleSeqCounter <= 10'b0;
		else IdleSeqCounter <= IdleSeqCounter + 1;
	 end
	 
	 assign IdleSeqOut = (IdleSeqCounter[4:0] == 5'b10000) ? ({24'b0,CommaChar,2'b10}) : 33'b0;
	 
	/* 
	 wire [31:0] CWRxDataOut, CCWRxDataOut;
	 wire CWRxCharIsKOut, CCWRxCharIsKOut;
*/
	 
	 
	 //Dual GTX serial Interface
	 SerialInterface GtxSerialInterface
	 (
		 .GtxRefClock(GtxRefClock),		 
		 .tile0_txoutclk0_i(tile0_txoutclk0_i), 	// output to top level
		 .tile0_txusrclk0_i(TxClock2x),   			// input from top level
		 .tile0_txusrclk20_i(TxClock), 				// input from top level
		 .txoutclk_dcm0_locked_i(txoutclk_dcm0_locked_i), 
		 .tile0_plllkdet_i(tile0_plllkdet_i), 
		 .CWRxUsrClock(CWRxUsrClock),   // output to remote packet processor input FIFO
		 .CCWRxUsrClock(CCWRxUsrClock), // output to remote packet processor input FIFO
		 .trigger(trigger),
		 .CWTxDataIn(RIOdataOut), 	  // data in to the rocket io is data out from SDI
		 .CWTxCharIsKin({3'b000,CharIsKOut}), 
		 .CCWTxDataIn(RIOdataOut), 
		 .CCWTxCharIsKin({3'b000,CharIsKOut}), 
		 //receice from gtx
		 .CWRxDataOut( CWRxDataOut ), 
		 .CCWRxDataOut( CCWRxDataOut ), 
		 .CWRxCharIsKOut(CWRxCharIsKOut), 
		 .CCWRxCharIsKOut(CCWRxCharIsKOut), 
		 .GTXRESET_IN(Reset), 
		 .AllGTXReset(ResetOut), 
		 .LinkStatus(LinkStatus),
		 .RXN_IN(RXN_IN), 
		 .RXP_IN(RXP_IN), 
		 .TXN_OUT(TXN_OUT), 
		 .TXP_OUT(TXP_OUT)	 
		 ); 
		
	wire CWmyReceivedLocalPacketHead, CCWmyReceivedLocalPacketHead;	
	wire CWmyReceivedLength, CCWmyReceivedLength;
	/*
	*	Local Packet timeout check.
	*/		 
	assign CWmyReceivedLocalPacketHead  = ((CWRxCharIsKOut)  && ( CWRxDataOut[7:0] == 8'h5C) &&  (CWRxDataOut[18:8] == 12'd0));
	assign CCWmyReceivedLocalPacketHead = ((CCWRxCharIsKOut) && (CCWRxDataOut[7:0] == 8'h5C) && (CCWRxDataOut[18:8] == 12'd0));
	assign CWmyReceivedLength  = ((CWRxCharIsKOut)  && ( CWRxDataOut[7:0] == 8'h5C) &&  (CWRxDataOut[31:20] == 12'd1024));
	assign CCWmyReceivedLength = ((CCWRxCharIsKOut) && (CCWRxDataOut[7:0] == 8'h5C) && (CCWRxDataOut[31:20] == 12'd1024));
	
	 reg [10:0] cw_addr_cnt, RxLength;	
	 reg rx_data_valid;
	 always @ (posedge CWRxUsrClock)
	 begin
		if (Reset == 1'b1 || CWmyReceivedLocalPacketHead == 1'b1) begin
			cw_addr_cnt <= 11'd0;
			rx_data_valid <= 1'b1;
			RxLength <= CWRxDataOut[31:20];
		end	
		else if (cw_addr_cnt <= RxLength ) begin
			cw_addr_cnt <= cw_addr_cnt + 1;
			rx_data_valid <= 1'b1;
		end	
		else begin
			cw_addr_cnt <= cw_addr_cnt;
			rx_data_valid <= 1'b0;
		end	
	 end


	 
	// Remote Packet received
	//wire IdleBit;
	wire [15:0] MyStopAddress;
	wire [33:0] CWRemoteBufferDataOut;
	wire [31:0] CWCRCErrorCount;
	wire RemoteBufferFlush;
	wire RemotePacketReady, RemoteBufferEmpty;
	reg RemotePacketsInQue;

	
`ifdef	BBBBBBBBBBBBBBBB	
	 // This subdesign implements the interface to remote data sources
	 RemotePacketProcessor CWRemotePacketProcessor (
		 .Reset(Reset),		 
		 .CRC_reset( 1'b0 ),
		 .Trigger(trig), 
		 .Idle( 1'b0 ),
		 .RioRxClock(CWRxUsrClock), 
		 .RxData(CWRxDataOut), 
		 .RxCharIsK(CWRxCharIsKOut),
		 .MyStopAddress(16'd0),		 
		 .RioTxClock(TxClock), 		//Fifo read clock
		 .TxBufferFlush(RemoteBufferFlush), 
		 .TxData(CWRemoteBufferDataOut), 		//remote data out. [33:0] for tx
`ifdef	RXFIFO_MONITORING		 
		 .RemoteRxFifoData(),
		 .RemoteRxFifoDataValid(),		 
`endif		 
		 .PacketReady(RemotePacketReady), 
		 .TxBufferEmpty(RemoteBufferEmpty), 
		 .PacketDropped(),
		 .CRCErrorCount(CWCRCErrorCount),
		 .LocalPacketRcvdOnRemote(),	//kha
		 .RemoteModeDone(),
		 .LocalRxPacketDataValid(),
		 .RxFifoEmpty_o(),
`ifdef	EDK_IPCORE_MODE	
		 .myReceivedLocalPacketHead()
`else		 
		 .myReceivedLocalPacketHead(),		 
		 .CONTROL0(CONTROL1)
`endif		 
		 );
	 
	 
	 // Remote Packets in Q
	 always @ (posedge TxClock)
	 begin
		if (Reset == 1'b1) 
			RemotePacketsInQue <= 1'b0;
		
		else if (RemotePacketReady == 1'b1)
			RemotePacketsInQue <= 1'b1;
			
		else if (RemoteBufferEmpty == 1'b1)
			RemotePacketsInQue <= 1'b0;
			
		else
			RemotePacketsInQue <= RemotePacketsInQue;
	 end
	 assign RemoteBufferFlush = RemotePacketsInQue;
	 
	
	 	
		
	 // This subdesign pulls the data from the outgoing stream 
	 // and writes it to the local user interface
	 StreamDataInterface myStreamDataInterface (
		 .Clock(TxClock), 
		 .Reset(Reset), 
		 .DataIn(CWRemoteBufferDataOut[33:2]), 
		 .CharIsK(CWRemoteBufferDataOut[1]), 
		 .MemoryAddress(LinkDataAddress),
		 .PacketAddress(LinkPacketAddress),
		 .PacketLength(LinkPacketLength), 
		 .DataOut(LinkData), 
		 .DataValid(LinkDataValid),
		 .LinkStartOfPacket(LinkStartOfPacket),
		 .LinkEndOfPacket(LinkEndOfPacket)		 
		 );
`endif
		
		
/* 09/06/16 : error message when enabled chip scope 
[Pack 1105] A collection of symbols which have restrictive placement or routing requirements exceeds the size of the range associated with the area group.  This will cause placement problems.  The collection is:
	The RPM "glb_gtx2_top_/myIla/U0/I_NO_D.U_ILA/U_TRIG/U_TM/G_NMU[0].U_M/U_MU/I_MUT_GANDX.U_match/I_CS_GANDX.U_CS_GANDX_SRL/I_V6.U_CS_GANDX_SRL_V6/U_CS_GAND_SRL_V6/I_USE_RPM_EQ0.U_GAND_SRL_SET/MSET".  Relationally Placed Macros (RPMs) have a predetermined physical shape.
The collection is 33 slices tall by 1 slices wide.  The area group is:
	"pblock_glb_gtx2_top_" (SLICE_X146Y170:SLICE_X161Y189)
*/
	
//`define	AAAAAAAAAAAA	
`ifdef	AAAAAAAAAAAA
		wire [35:0] CONTROL0;
		myicon myIcon (
			.CONTROL0(CONTROL0) // INOUT BUS [35:0]
		); 
	
		wire [255:0] TRIG0;
		
		assign TRIG0[31:0]  = RIOdataOut;
		assign TRIG0[63:32] = IdleSeqOut[33:2];
		assign TRIG0[65]    = CharIsKOut;
		assign TRIG0[66]    = IdleSeqOut[1];
		assign TRIG0[67]    = LocalBufferFlush;
		assign TRIG0[68]    = LocalPacketReady;
		assign TRIG0[69]    = LocalBufferEmpty;
		assign TRIG0[70]    = LocalPacketDataValid;
		
		assign TRIG0[102:71]  = LocalData;
		assign TRIG0[103]     = LocalDataValid;
		assign TRIG0[135:104] = LocalBufferDataOut[33:2];
		assign TRIG0[136]     = LocalBufferDataOut[1];
		assign TRIG0[137]     = LocalBufferDataOut[0];
		
		assign TRIG0[169:138] = CWRxDataOut;
		assign TRIG0[170]     = CWRxCharIsKOut;
		assign TRIG0[202:171] = CCWRxDataOut;
		assign TRIG0[203]     = CCWRxCharIsKOut;
		assign TRIG0[214:204] = cw_addr_cnt;
		assign TRIG0[215]     = rx_data_valid;		
		
		assign TRIG0[249:216] = CWRemoteBufferDataOut;
		assign TRIG0[250]     = RemoteBufferFlush;
		assign TRIG0[251]     = RemotePacketReady;
		assign TRIG0[252]     = RemoteBufferEmpty;
		
		assign TRIG0[255]   = trig;
				 
		myila myIla (
			 .CONTROL(CONTROL0), // INOUT BUS [35:0]
			 .CLK(TxClock), // IN
			 .TRIG0(TRIG0) // IN BUS [99:0]
		);   
`endif


    
//`define	AAAAA	 
`ifdef	AAAAA
		wire [35:0] CONTROL0;
		icon myIcon (
			.CONTROL0(CONTROL0) // INOUT BUS [35:0]
		); 
	
		wire [255:0] TRIG0;
		
		assign TRIG0[31:0]  = LinkData;
		assign TRIG0[47:32] = LinkDataAddress;
		assign TRIG0[63:48] = LinkPacketAddress;
		assign TRIG0[65]    = LinkDataValid;
		assign TRIG0[85:70] = 0;
		assign TRIG0[255]   = trig;
				 
		ila myIla (
			 .CONTROL(CONTROL0), // INOUT BUS [35:0]
			 .CLK(TxClock), // IN
			 .TRIG0(TRIG0) // IN BUS [99:0]
		);  
`endif		
		
	
		assign	dbg[0] = LocalBufferDataOut[1];
		assign	dbg[1] = IdleSeqOut[1];
		assign	dbg[2] = LocalBufferFlush;
		assign	dbg[3] = LocalPacketReady;
		assign	dbg[4] = LocalBufferEmpty;
		assign	dbg[5] = CWRxCharIsKOut;
		assign	dbg[6] = CCWRxCharIsKOut;
		assign	dbg[7] = LocalPacketDataValid;
		assign	dbg[8] = CWmyReceivedLocalPacketHead;
		assign	dbg[9] = CCWmyReceivedLocalPacketHead;
		assign	dbg[10] =  CWmyReceivedLength;
		assign	dbg[11] =  CWmyReceivedLength;
		

endmodule
