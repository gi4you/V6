`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 			Brookhaven National Lab
// Engineer: 			Joe DeLong
// 
// Create Date:    	12:49:40 12/22/2009 
// Design Name: 		Single Side SDI
// Module Name:    	RemotePacketProcessor 
// Project Name:		SDI 
// Target Devices: 	FX70T
// Tool versions: 	ISE 11.4
// Description: 		This subdesign takes the input from the RocketIO receiver
//							And buffers the data while a local device is using the RIO
//							transmitter. As packets are recieved the CRC value is checked
//							and if they match the data is moved to an output FIFO. When
//							a packet is ready for transmission the packet ready flag is
//							set and if there is no data left in the buffer the done flag
//							is set (buffer empty). If the packet transmitted from the 
//							local port is detected on the remote port the 
//							LocalPacketRcvdOnRemote signal is driven.
//
// Dependencies: PacketBuffFIFO, RemotePacketBuffer
//
// Revision: 1.1
// Revision 0.01 - File Created
// Additional Comments: 
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////

module RemotePacketProcessor(
		// RocketIO receiver interface
		input Reset,
		input CRC_reset,		//kha
		input Trigger,
		input Idle,
		input RioRxClock,
		input [31:0] RxData,
		input RxCharIsK,
		input [15:0] MyStopAddress,
		
		// RocketIO transmitter interface
		input RioTxClock,
		input TxBufferFlush,
		output [33:0] TxData, 				// bit 1 = start of packet (CharIsK) and bit 0 = end of packet
`ifdef	RXFIFO_MONITORING		
		output [31:0] RemoteRxFifoData,		//kha for monitoring remote packets
		output RemoteRxFifoDataValid,
`endif		
		// flow control
		output PacketReady,
		output TxBufferEmpty,
		output PacketDropped,
		output reg [31:0] CRCErrorCount,
		output reg LocalPacketRcvdOnRemote,
		output reg RemoteModeDone,
		/* Local loopback data */
		output  LocalRxPacketDataValid, 
		output	RxFifoEmpty_o,
		//output  [31:0] crc_wd_timeout_cnt,
		//output	crc_watchdog_rst,
`ifdef	EDK_IPCORE_MODE
		output  myReceivedLocalPacketHead
		//output	[31:0] LocalRxData
`else		
		output  myReceivedLocalPacketHead,
		//output	[31:0] LocalRxData,	//Output						
		inout [35 : 0] CONTROL0
`endif		
    );

	parameter K_START = 8'h5C;	//8'b 01011100; 	  		// k28.2 character marks packet start
	parameter CRC_INIT = 32'hFFFFFFFF;
	parameter CONST_REMOTE_TIMEOUT = 16'd3000; 	// 24 us 
	
`ifdef EDK_IPCORE_MODE   	
	parameter IDLE    = 4'b001;
	parameter SIGNAL  = 4'b010;
	parameter FLUSH   = 4'b011;
	parameter CLEAR   = 4'b100;	 
	parameter WAIT0   = 4'b101;
	parameter WAIT1   = 4'b110;
	
`else
	parameter IDLE    		= 4'b001;
	parameter WAIT_SIGNAL  	= 4'b010;
	parameter FLUSH   		= 4'b011;
	parameter CLEAR   		= 4'b100;	
	parameter TIMEOUT		= 4'b101;
	parameter GET_LOCAL_HEAD= 4'b110;		
	
`endif
	(* FSM_ENCODING="ONE-HOT", SAFE_IMPLEMENTATION="NO" *) reg [2:0] state = IDLE;	 
	
	reg RxBufferFlush;
	reg [31:0] InputData_reg;
	reg [11:0] LinkPacketLength;
	reg [15:0] LinkPacketAddress;
	reg [11:0] PacketLengthCounter;
	reg RxFIFOfillEna;
	reg CheckCRC, TxBufferFill;
	reg [11:0] PacketFlushCounter;				 
	reg PacketStartBit;
	reg PacketStopBit;
	reg [31:0]RxCalcCRCvalue_reg;
	
	wire [31:0] RxTxData;
	wire [11:0] myPacketLength;
	wire StartOfPacket; 
	wire [31:0] RxCalcCRCvalue;
	wire PacketReadyFIFOwrEna;
	wire NoPacketsInQue;
	wire [33:0] TxPacketBufferData;
	wire RxBufferEmpty;
	wire [31:0] CRCout;
	reg	PacketDropped_i;		
	reg	[31:0] LoopCnt; 	 
	reg RxFifoValid;
	reg LocalRxBufferFill;
	wire   RemoteRxFifoDataValid; 
	
	 // add a variable for the packet length coming in on the RocketIO link
	 assign myPacketLength = RxData[31:20];
	 assign PacketReady = ~NoPacketsInQue;
	 
	 assign PacketReadyFIFOwrEna = (PacketStopBit) && (state!=FLUSH) && (!PacketDropped_i);
	 // detect an incoming packet
	 assign StartOfPacket = ((RxCharIsK) && (RxData[7:0] == K_START));	 
	 
	 assign RemoteRxFifoDataValid = RxBufferFlush && RxFifoValid; //July 19, remove tail two 32-bit
	 assign RemoteRxFifoData      = RxTxData;	 
	 //assign LocalRxData           = RxTxData;		/* Copy Rx Data */
	 assign RxFifoEmpty_o         = RxBufferEmpty;	/* July 17 for timeout check */

	 wire   ReceivedLocalPacketHead;
	 assign ReceivedLocalPacketHead =  ( (RxTxData[7:0] == K_START) && RxTxData[19:8] == MyStopAddress );	 
	 assign myReceivedLocalPacketHead = ((RxCharIsK) && (RxData[7:0] == K_START) && (RxData[19:8] == MyStopAddress));
	 /*
	  *  Local Loopback remote Data Valid
	  *        --[HEAD][X][Y][CRC][00][00]-------
	  *           --------------- 
	  *        __|               |_______           RemoteRxFifoDataValid
	  *
	  *                 ------------------          LocalRxBufferFill
	  *        ________|                  |_______  
      *	  
	  *            ----    
	  *        ___|    |___________________________
	  *            --------------
	  *		   ___|              |________________  LocalRxPacketDataValid
	  */
	 
	 
	 assign LocalRxPacketDataValid = (LocalRxBufferFill || ReceivedLocalPacketHead) && RemoteRxFifoDataValid;

	 
	 always @ (posedge RioRxClock)
	 begin
		if (Reset==1'b1) RxFifoValid <= 1'b1;
		else if (PacketFlushCounter <= 12'd2) RxFifoValid <= 1'b0;
		else RxFifoValid <= RxFifoValid <= 1'b1;
	 end	 
	 
	 
	 // delay incoming data by one clock cycle
	 always @ (posedge RioRxClock)
	 begin
		InputData_reg <= RxData;
		RxCalcCRCvalue_reg <= RxCalcCRCvalue;
	 end
	 	 
	 // if the start of packet is detected store the packet length and address
	 always @ (posedge RioRxClock)
	 begin
		if (StartOfPacket)
			begin
				LinkPacketLength  <= myPacketLength;	// RxData[31:24] Packet length in words
				//LinkPacketAddress <= RxData[23:8];
				LinkPacketAddress <= RxData[19:8];
			end	 
		else if (state == CLEAR)
			begin
				LinkPacketAddress <= 16'b0;	// Packet length in words
			end
		else
			begin
				LinkPacketLength <= LinkPacketLength;	// Packet length in words
				LinkPacketAddress <= LinkPacketAddress;
			end
	 end
	 
	 // packet length counter is used to enable the input data going to
	 // the packet FIFO "myPacketBuffFIFO" for the lenght of the current packet 
	 always @ (posedge RioRxClock)
	 begin
		if (Reset==1'b1) 
			PacketLengthCounter <= 12'b0;
		else if (StartOfPacket) 
			PacketLengthCounter <= ({1'b0,((myPacketLength)+1)}); //add one for crc check
		else if (PacketLengthCounter > 12'd0) 
			PacketLengthCounter <= PacketLengthCounter - 1;
		else 
			PacketLengthCounter <= PacketLengthCounter;
	 end
	 
	 // packet FIFO input enable signal active for the length of the packet
	 always @ (posedge RioRxClock)
	 begin
		if ((Reset==1'b1) || ((RxFIFOfillEna == 1'b1) && (PacketLengthCounter == 12'd0) && (StartOfPacket == 1'b0))) 
			RxFIFOfillEna <= 1'b0;
		else if (StartOfPacket == 1'b1) 
			RxFIFOfillEna <= 1'b1;
		else 
			RxFIFOfillEna <= RxFIFOfillEna;
	 end

	 ////////////////////////////////////////////////////////////////////////////////
	 // CRC checking section
	 // Uses the CRC hardware built into the Virtex 5. The incoming CRC value is stored
	 // in the packet. The clock tick after the packet FIFO is filled the CRC block has 
	 // the correct CRC value to be compared against the packet CRC value.	

	 // signal to initiate a CRC check
	 always @ (posedge RioRxClock)
	 begin
		if (Reset == 1'b1 )	
			CheckCRC <= 1'b0;
		else if (PacketLengthCounter == 12'd3)				
			CheckCRC <= 1'b1;
		else if (PacketLengthCounter == 12'd2)
			CheckCRC <= 1'b0;
		else CheckCRC <= CheckCRC;
	 end
	 
	 
	 ////////////////////////////////////////////////////////////////////////////////
	 // After the CRC value is verified the input packet buffer needs to be flushed.
	 // If the CRC value is correct the buffer is flushed into the transmit buffer
	 // so it can be forwarded. If not the data is dumped. This packet length counter
	 // is used to time the buffer flush wherever the data goes.
	 
	 // packet FIFO Packet flush counter  
	 always @ (posedge RioRxClock)
	 begin
		if (Reset==1'b1) 
			PacketFlushCounter <= 12'b0;
		else if (CheckCRC == 1'b1) 
			//PacketFlushCounter <= ({1'b0,(LinkPacketLength + 1)}); // initialize the counter
			PacketFlushCounter <= ((LinkPacketLength + 1));
		else if (RxBufferFlush == 1'b1) 
			PacketFlushCounter <= PacketFlushCounter - 1;
		else 
			PacketFlushCounter <= PacketFlushCounter;
	 end
	 
	 // packet FIFO flush enable signal active for the length of the packet	 
	 always @ (posedge RioRxClock)
	 begin
		if ((Reset==1'b1) || ((PacketFlushCounter == 12'd0) && !CheckCRC) || (RxBufferEmpty))
			RxBufferFlush <= 1'b0;
		//else if (CheckCRC || (state == FLUSH) || (Idle == 1'b1)) 
		else if (CheckCRC || (state == FLUSH) )
			RxBufferFlush <= 1'b1;
		else 
			RxBufferFlush <= RxBufferFlush;
	 end	 

	 // If the CRC check is good and this is not the local data 
	 // then fill the transmit buffer otherwise dump the data
	 always @ (posedge RioRxClock)
	 begin
		if ((Reset==1'b1) || ((PacketFlushCounter == 12'd0) && !CheckCRC) || (RxBufferEmpty))
			TxBufferFill <= 1'b0;
		//else if ((CheckCRC) && (CRCout == InputData_reg) && !(state == FLUSH) && (RxTxData[19:8] != MyStopAddress) && !Idle)
		else if ((CheckCRC) && (CRCout == InputData_reg) && !(state == FLUSH)  && !Idle)
			TxBufferFill <= 1'b1;
		else 
			TxBufferFill <= TxBufferFill;
	 end	 

	/*
		Kha for Local loopback
		Local loopback data push to FIFO memory		
	*/
	 // packet FIFO input enable signal active for the length of the packet	 	 
	 always @ (posedge RioRxClock)
	 begin
		if ((Reset==1'b1) || ((PacketFlushCounter == 12'd0) && !CheckCRC) || (RxBufferEmpty))
			LocalRxBufferFill <= 1'b0;
		//else if ( RxBufferFlush && (RxTxData[23:8] == MyStopAddress) )
		else if ( RxBufferFlush && (RxTxData[19:8] == MyStopAddress) )
			LocalRxBufferFill <= 1'b1;		//write to Local received FIFO
		else 
			LocalRxBufferFill <= LocalRxBufferFill;
	 end
	 
	 	 
	 // if the CRC check fails flag the error	 
	 always @ (posedge RioRxClock)
	 begin
		if ((Reset==1'b1) || (PacketFlushCounter == 12'd2))
			PacketDropped_i <= 1'b0;
		else if ((CheckCRC) && (CRCout != InputData_reg) && !Idle)
			PacketDropped_i <= 1'b1;
		else 
			PacketDropped_i <= PacketDropped_i;
	 end	 
	 
	 always @ (posedge RioRxClock)
	 begin
		if ( Reset==1'b1  || CRC_reset == 1'b1 ) 	//crc clear
			begin
				CRCErrorCount <= 32'b0;
			end	
		else if ((CheckCRC) && (CRCout != InputData_reg) && !Idle)
			begin
				CRCErrorCount <= (CRCErrorCount + 1);
			end
		else
			begin 
				CRCErrorCount <= CRCErrorCount;
			end
	 end	 	 
	 
	 ////////////////////////////////////////////////////////////////////////////
	 // Flow control section. This section adds markers for the beginning and end
	 // of an incoming packet. The begining of a packet is also used to signal
	 // a "k" character to the Tx Rio port. It's placed in bit '1' of the Tx FIFO
	 // The last word of the packet is marked by a 1 in bit '0' of the Tx FIFO.
	 always @ (posedge RioRxClock)
	 begin
		if (Reset==1'b1) PacketStartBit <= 1'b0;
		else if (CheckCRC) PacketStartBit <= 1'b1; //&& (CRCout == InputData_reg) && !(state == FLUSH) && !(LinkPacketAddress == MyStopAddress)) PacketStartBit <= 1'b1;
		else PacketStartBit <= 1'b0;
	 end
	 
	 always @ (posedge RioRxClock)
	 begin
		if (Reset==1'b1) PacketStopBit <= 1'b0;
		else if (PacketFlushCounter == 12'd3) PacketStopBit <= 1'b1;
		else PacketStopBit <= 1'b0;
	 end	  
	 
	 // need to know if all data has been circulated. To detect this look for a 
	 // local packet that comes in on the remote data stream. If this occurs don't
	 // signal packet ready and flush the packet from the input buffer. This only
	 // works if the link is complete and all nodes in the loop are connected

`ifdef EDK_IPCORE_MODE      
    always@(posedge RioRxClock)
      if (Reset) begin
         state <= IDLE;
      end
      else
         (* FULL_CASE, PARALLEL_CASE *)case (state)
            IDLE : begin
				if  ((LinkPacketAddress == MyStopAddress) && (CheckCRC))	
					state <= SIGNAL;
				else
					state <= IDLE;
				end				
            SIGNAL : state <= FLUSH;				
            FLUSH : begin
				if (RxBufferEmpty)
					state <= CLEAR;
				else 
					state <= FLUSH;
				end				
			//CLEAR: state <= IDLE;	
			CLEAR: state <= WAIT0;		// 2 clock extention for sync with TX clock
			WAIT0: state <= WAIT1;	
			WAIT1: state <= IDLE;
         endcase
   
   
	//LocalPacketRcvdOnRemote -- sometimes missing then timeout		
    always@(posedge RioRxClock)
	 begin
      if (state == IDLE) LocalPacketRcvdOnRemote <= 1'b0;
		else if (state == SIGNAL) LocalPacketRcvdOnRemote <= 1'b1;		
		//else if (state == CLEAR) LocalPacketRcvdOnRemote <= 1'b0;
		//else if (state == FLUSH) LocalPacketRcvdOnRemote <= 1'b0;		//July 17		
		else if (state == CLEAR || state == WAIT0  || state == WAIT1) LocalPacketRcvdOnRemote <= 1'b0;	//2 clock extention
		else LocalPacketRcvdOnRemote <= LocalPacketRcvdOnRemote;
	 end
	 
	 //  RemoteModeDone -- always good
	 always@(posedge RioRxClock)
	 begin
      if (Reset) RemoteModeDone <= 1'b0;
		if (((state == FLUSH) && RxBufferEmpty) || (state == CLEAR)) RemoteModeDone <= 1'b1;
		//if ( state == CLEAR) RemoteModeDone <= 1'b1;	//July 17
		else RemoteModeDone <= 1'b0;
	 end
	 
`else
	//It worked wel at ISE
	reg [11:0] count;
    always@(posedge RioRxClock)
      if (Reset) begin
         state <= IDLE;
      end
      else
         (* FULL_CASE, PARALLEL_CASE *)case (state)
         	     
         	IDLE : begin
         		if(Trigger == 1'b1) begin
         			state <= GET_LOCAL_HEAD;
         			count <= 12'd0;
         			RemoteModeDone <= 1'b0;
         			end
         		else
         			state <= IDLE;	
				end	
         	GET_LOCAL_HEAD : begin         		
				if  ( myReceivedLocalPacketHead ) begin		/* Local Packet received ? goto check CRC status*/				
					state <= WAIT_SIGNAL;					
					end
				else begin
					state <= GET_LOCAL_HEAD;
					count <= count + 1;            		
            		if( count > CONST_REMOTE_TIMEOUT) state <= TIMEOUT;					
					end
			end		
            WAIT_SIGNAL : begin            	
            	if( CheckCRC == 1'b1) begin
            		LocalPacketRcvdOnRemote <= 1'b1;
            		state <= FLUSH;
            	end	
            	else  begin	
            		state <= WAIT_SIGNAL;
            		end	
			end				
            FLUSH : begin
				if (RxBufferEmpty)
					state <= CLEAR;
				else 
					state <= FLUSH;				
			end
				
			CLEAR  : begin
				RemoteModeDone <= 1'b1;
				LocalPacketRcvdOnRemote <= 1'b0;
				state <= IDLE;			
			end	
			TIMEOUT: state <= IDLE;
				
         endcase		
`endif
//////////////////////////////////////////////////////////////	

			 
	 assign TxPacketBufferData = {RxTxData,PacketStartBit,PacketStopBit};	 
	 
	 //June 10, FIFO reset
	 wire	FIFO_Rst;
	 assign FIFO_Rst = ( Reset==1'b1  || CRC_reset == 1'b1 || Trigger == 1'b1 );
	 
	///////////////////////////////////	  
	 
	 // FIFO memory for storing incoming RIO packets while CRC is verified
	 ReceivePacketFIFO myReceivePacketFIFO (
		.clk(RioRxClock),
		//.rst(FIFO_Rst),
		.rst(Reset),
		.din(InputData_reg),  
		.wr_en(RxFIFOfillEna),
		.rd_en(RxBufferFlush),
		.dout(RxTxData), 	//[31:0]
		.full(),
		.empty(RxBufferEmpty)
		);
	  			 
	 // FIFO memory for storing incoming packets after CRC check and before
	 // transmitting. This is a holding place while loacal data is transmitted
	 // This FIFO crosses clock domains
	 TxPacketFIFO myTxPacketFIFO (
		//.rst(FIFO_Rst),
		.rst(Reset),
		.wr_clk(RioRxClock),
		.rd_clk(RioTxClock),
		.din(TxPacketBufferData),  	//[33:0] {RxTxData,PacketStartBit,PacketStopBit}
		.wr_en(TxBufferFill),		//
		//.rd_en((TxBufferFlush || Idle)),
		.rd_en(~TxBufferEmpty),
		.dout(TxData),  			// 33:0]
		.full(),
		.empty(TxBufferEmpty)
		);

		
	 // this FIFO is used for flow control if a complete validated packet 
	 // is ready the output goes high. If a local packet is detected the
	 // output remains low
	 PacketQue myPacketQue (
		.rst(FIFO_Rst),
		.wr_clk(RioRxClock),
		.rd_clk(RioTxClock),
		.din(1'b1), 
		.wr_en(PacketReadyFIFOwrEna),
		//.rd_en((TxData[0] || Idle)),
		.rd_en(TxData[0] ),
		.dout(),  
		.full(),
		.empty(NoPacketsInQue)
		);
   
    //CRC Calculation
	LocalDataCRC CRC(
		.clk(RioRxClock),
		.rst(StartOfPacket),		
		.data_in(InputData_reg),
		.crc_en(RxFIFOfillEna),
		.CRCout(CRCout)
	);	

	//Stretching drop signal to 200 clock
	/*
	stretch 	PacketDropSignal (
        .clk(RioRxClock),
		.reset(StartOfPacket),
		.sig_in(PacketDropped_i),
		.len(12'd2000),
		.sig_out(PacketDropped)
	);
	*/
	assign PacketDropped = PacketDropped_i;
	
`ifdef	WATCHDOG	
	 // WD 150 us if over 150 us, timeout_cnt increment
	 //wire [31:0] crc_wd_timeout_cnt;
	 //
	 //  CheckCRC  __|___|___|___|___|___|___|_______~~  __|___|___|___|___|___|___|____
	 //              1   2   3   4   5   6  LOCAL_ID 
	 trig_watchdog crc_wd (
	 		.trig_watchdog_rst(crc_watchdog_rst),
	   		.timeout_cnt(crc_wd_timeout_cnt),
			.trig_cnt(),
	   		.sys_clk_i(RioRxClock), 
	   		.reset(Reset),
			.UsrClear(CRC_reset)
	   		.EXT_TRIG_B(CheckCRC)	//myReceivedLocalPacketHead
	   	);	
`endif

		
///// CHIPSCOPE ///////////////////////	
`ifdef	AAAAAA		 
 
		wire [35:0] CONTROL0;
		icon myIcon (
			.CONTROL0(CONTROL0) // INOUT BUS [35:0]
		); 

		
		wire [255:0] TRIG0;
		
		assign TRIG0[0]     = RxFIFOfillEna;
		assign TRIG0[32:1]  = InputData_reg;
		assign TRIG0[33]    = RxBufferFlush;
		assign TRIG0[65:34] = RxTxData;
		assign TRIG0[66]    = RxFifoValid; 
		assign TRIG0[67]    = TxBufferFill;
		assign TRIG0[101:68] = TxPacketBufferData;
		assign TRIG0[102] = TxBufferFlush;
		assign TRIG0[136:103] = TxData;
		assign TRIG0[137] = NoPacketsInQue;
		assign TRIG0[138] = LocalPacketRcvdOnRemote;
		assign TRIG0[139] = RemoteRxFifoDataValid;
		assign TRIG0[143:140] = state;
		assign TRIG0[175:144] = RxCalcCRCvalue_reg;
		assign TRIG0[207:176] = CRCout;
		assign TRIG0[208] = StartOfPacket;
		assign TRIG0[220:209] = PacketLengthCounter;
		assign TRIG0[221] = PacketStopBit;
		assign TRIG0[227] = CheckCRC;
		assign TRIG0[228] = Idle;
		assign TRIG0[229] = RemoteModeDone;
		assign TRIG0[230] = LocalRxBufferFill;
		assign TRIG0[231] = LocalRxPacketDataValid;
		assign TRIG0[232] = myReceivedLocalPacketHead;
		assign TRIG0[233] = RxBufferEmpty;
		assign TRIG0[234] = TxBufferEmpty;
		assign TRIG0[235] = RxCharIsK;
		assign TRIG0[236] = PacketReadyFIFOwrEna;
		assign TRIG0[249:237] = PacketFlushCounter;
		
		assign TRIG0[254] = PacketDropped;
		assign TRIG0[255] = Trigger;

		ila myIla (
			 .CONTROL(CONTROL0), // INOUT BUS [35:0]
			 .CLK(RioRxClock), // IN
			 .TRIG0(TRIG0) // IN BUS [99:0]
		);	 
//	end
//	endgenerate
	
`endif

	
 endmodule
