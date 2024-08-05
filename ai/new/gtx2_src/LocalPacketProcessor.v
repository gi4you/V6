`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 			Brookhaven National Lab
// Engineer: 			Joe DeLong
// 
// Create Date:   	14:59:13 12/15/2009 
// Design Name: 		SDI
// Module Name:   	LocalPacketProcessor 
// Project Name: 		Cell Controller
// Target Devices: 	V5FX70T, V6?
// Tool versions: 	11.1
// Description: 		This module prepares a packet of data for transmition over 
//							the SDI link if in source mode or forwards a packet if not
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//
//	8/2/ added PAD1 stat for PacketReady signal tooks 2 clock
//
//////////////////////////////////////////////////////////////////////////////////

`define EDK_IPCORE_MODE

module LocalPacketProcessor(
		// user interface
		input FillClock,						// User clock unkown speed
		input [11:0] SourcePacketSizeIn,		// if the device is a source define packet size
		input [11:0] SourcePacketDestIn,		// cell controller address for packet, Kha think Local Start Address
		input [31:0] DataIn,				
		input DataInValid,

		// interface to single side SDI
		input Reset,
		input FlushClock,				// 125 MHz txUserClock2
		input FlushEnable,				// remove packet(s) from FIFO
		output [33:0] DataOut,			// data plus charIsK and end of packet marker
		output reg PacketReady,			// valid whenever a packet ir ready for transmit
		output BufferEmpty,				// no more data to process
		output LocalPacketDataValid,
		output full
    );
	 
	wire [11:0] SourcePacketSize;
	wire [11:0] SourcePacketDest;
	
	//assign SourcePacketSize = SourcePacketSizeIn[25:31];		//assigned 7-bit, but I want 8-bit "24..31"?
	assign SourcePacketSize = SourcePacketSizeIn[11:0];			//12-bit
	assign SourcePacketDest = SourcePacketDestIn [11:0];
	
	//parameter DEBUG = 0;
`ifdef	EDK_IPCORE_MODE		 
  	parameter 	DEBUG = 0;	
`else	
		parameter 	DEBUG = 1;
`endif
	
	parameter CRC_INIT = 32'hFFFFFFFF;
	parameter KStart = 8'b 01011100; 	// k28.2 character marks packet start 0x5C

	reg [15:0] LengthCount;
	reg FillEnable;
	reg LengthCounterEnable;
	reg [31:0] DataIn_reg, DataIn_reg_reg;
	reg [31:0] LocalCalcCRCvalue_reg;
	reg [31:0] CRCout_reg;
	
	wire [33:0] BufferInputData;
	wire [31:0] LocalCalcCRCvalue;
	wire [31:0] CrcDataIn;
	wire [31:0] CRCout;
	//////////////////////////////////////////////////////////////////////////////
	// If the device is a data source a packet header and CRC word are added to
	// the data before transmitting. The buffer empty signal is deasserted and 
	// when the packet is assembled the Packet ready signal is asserted
	
	parameter IDLE = 7'b0000001; 	// wait for data valid
	parameter HEAD = 7'b0000010; 	// insert the header into the fifo
	parameter DATA = 7'b0000100;	// insert the data into the fifo
	parameter CRC  = 7'b0001000;	// insert the CRC value into the fifo
	parameter RDY  = 7'b0010000;	// set flag for data ready to be sent
	parameter PAD  = 7'b0100000;	// force a zero at end of packet
	parameter PAD2 = 7'b1000000;
	parameter PAD1 = 7'b1000001;

   //(* FSM_ENCODING="ONE-HOT", SAFE_IMPLEMENTATION="NO" *) reg [6:0] state = IDLE;
   reg [6:0] state = IDLE;

   always@(posedge FillClock)
      if (Reset) begin
         state <= IDLE;
      end
      else
         (* FULL_CASE, PARALLEL_CASE *)case (state)
            IDLE : begin
               if (DataInValid)
                  state <= HEAD;						
               else
                  state <= IDLE;
            end
            HEAD : begin
                  state <= DATA;
            end
            DATA : begin
               if (LengthCount == 16'd2)
                  state <= CRC;
               else
                  state <= DATA;
            end
            CRC : begin
                  state <= RDY;
            end
            RDY : begin			
                  state <= PAD;
            end
            PAD : begin			
                  state <= PAD1;	//add one more clock
            end
            PAD1 : begin			
                  state <= PAD2;
            end			
			PAD2 : begin
				state <= IDLE;
				end
         endcase
			
	// data going into the FIFO: bits 33:2 data to be transmitted, bit 1 is packet start 
	// and bit 0 is end of packet. The data needs to be delayed one clock tick for header
	// insertion
	always @ (posedge FillClock) DataIn_reg <= DataIn;
	always @ (posedge FillClock) DataIn_reg_reg <= DataIn_reg;
	always @ (posedge FillClock) LocalCalcCRCvalue_reg <= LocalCalcCRCvalue;
	always @ (posedge FillClock) CRCout_reg <= CRCout;
	
	//34 bit    12+12+8+1+1
    assign BufferInputData = (state == HEAD) ? {SourcePacketSize, SourcePacketDest, KStart, 1'b1, 1'b0} :
	((state == RDY) ? {CRCout,1'b0,1'b1} : ((state == PAD) ? 34'd0 : ((state == PAD2) ? 34'd0 : {DataIn_reg_reg,1'b0,1'b0})));	// LocalCalcCRCvalue_reg	
	
	// packet length counter enable
	always @ (posedge FillClock)
	begin
		if (Reset) LengthCounterEnable <= 1'b0;
		
		else if ((state == HEAD) || (state == DATA) || (state == CRC)) LengthCounterEnable <= 1'b1;
		
		else LengthCounterEnable <= 1'b0;
	end
	
	// packet length counter for insertion of data, CRC value and end of packet flag	
	always @ (posedge FillClock)
	begin
		if (Reset) LengthCount <= 16'd0;
		
		else if ((state == IDLE) && (DataInValid == 1'b1)) LengthCount <= (SourcePacketSize - 2);
		
		else if (LengthCounterEnable) LengthCount <= (LengthCount -1);
		
		else if ((state == IDLE) && (DataInValid == 1'b0)) LengthCount <= 16'd0;
		
		else LengthCount <= LengthCount;
	end
	
	// packet fifo write enable
	always @ (posedge FillClock)
	begin
		if (Reset) FillEnable <= 1'b0;
		
		else if ((state == IDLE) && (DataInValid == 1'b1)) FillEnable <= 1'b1;
		
		else if (state == HEAD) FillEnable <= 1'b1;

		else if (state == DATA) FillEnable <= 1'b1;
		
		else if (state == CRC) FillEnable <= 1'b1;
		
		else if (state == RDY) FillEnable <= 1'b1;
		
		else if (state == PAD) FillEnable <= 1'b1;

		else FillEnable <= 1'b0;
		
	end
	
	// after the packet is assembled drive packet ready
	always @ (posedge FillClock)
	begin
		if (Reset) PacketReady <= 1'b0;
		
		else if (state == PAD1 || state == PAD2) PacketReady <= 1'b1;
		
		else PacketReady <= 1'b0;
	end


	//LocalPacketBuffer myLocalPacketBuffer (
	LocalPacketBuffer myLocalPacketBuffer (
		.rst(Reset),
		.wr_clk(FillClock),
		.rd_clk(FlushClock),
		.din(BufferInputData), // Bus [33 : 0] 
		.wr_en(FillEnable),
		.rd_en(FlushEnable),						
		.dout(DataOut), // Bus [33 : 0] 		
		.full(full),
		.empty(BufferEmpty)
		);

 /*
	  *  Local Loopback remote Data Valid
	  *        --[HEAD][X][Y][CRC][00][00]-------
	  *           --------------- 
	  *        __|               |___          LocalPacketDataValid
	  *
	  */
	wire LocalPacketDataValid;
    reg	aa, bb;
	
	assign LocalPacketDataValid = bb && ~BufferEmpty; 
	
	always @ (posedge FlushClock)
	begin
		if (Reset) aa <= 1'b0;		
		else if (FlushEnable == 1'b1) begin
			aa <= 1'b1;
		end	
		else aa <= 1'b0;
	end
	always @ (posedge FlushClock)
	begin
		bb <= aa;
		//cc <= bb;
	end	
	
	// CRC generator for the local packet data i.e. BPM data packet
	reg CRCreset;
	
	always @ (posedge FillClock)
	begin
		if (Reset) CRCreset <= 1'b0;
		
		else if ((state == IDLE) && (DataInValid == 1'b1)) CRCreset <= 1'b1;
		
		else CRCreset <= 1'b0;
	end
	
	assign CrcDataIn = (state==HEAD) ? {(SourcePacketSize),SourcePacketDest,KStart} : DataIn_reg_reg;
	
    //CRC Calculation
	LocalDataCRC LOCAL_CRC(
		.clk(FillClock),
		.rst(state == PAD2),		
		.data_in(CrcDataIn),
		.crc_en(FillEnable),
		.CRCout(CRCout)
	);    
	
	
`ifdef AAAAAAAAAAAAAA	
		
		wire [35:0] CONTROL0;
		icon myIcon (
			.CONTROL0(CONTROL0) // INOUT BUS [35:0]
		); 
		
		wire [255:0] TRIG0;
		
		assign TRIG0[11:0]  = SourcePacketDest;
		assign TRIG0[23:12] = SourcePacketSize;
		assign TRIG0[55:24] = DataIn;
		assign TRIG0[56] = DataInValid;
		assign TRIG0[57] = FlushEnable;
		assign TRIG0[89:58] = DataOut[33:2];
		assign TRIG0[90] = DataOut[1];
		assign TRIG0[91] = DataOut[0];
		assign TRIG0[92] = PacketReady;
		assign TRIG0[93] = BufferEmpty;
		assign TRIG0[94] = full;
		assign TRIG0[101:95] = state;
		assign TRIG0[102] = FillEnable;
		assign TRIG0[136:103] = {2'b00,CrcDataIn};
		assign TRIG0[168:137] = CRCout;
		assign TRIG0[169]     = CRCreset;
		assign TRIG0[170]     = LocalPacketDataValid;
		assign TRIG0[254:171] = 0;
		
		assign TRIG0[255] = DataInValid;


		ila myIla (
			 .CONTROL(CONTROL0), // INOUT BUS [35:0]
			 .CLK(FlushClock), // IN
			 .TRIG0(TRIG0) // IN BUS [99:0]
		);

`endif
	
		
endmodule
