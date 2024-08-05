`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    09:56:40 01/12/2010 
// Design Name: 
// Module Name:    StreamDataInterface 
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
//	Aug/28 AddressCounter length 8 -> 16
//		
//
//////////////////////////////////////////////////////////////////////////////////
module StreamDataInterface(
		input Clock,
		input Reset,
		input [31:0] DataIn,
		input CharIsK,
		output reg [15:0] MemoryAddress,
		output reg [11:0] PacketAddress,
		output reg [11:0] PacketLength,
		output reg [31:0] DataOut,
		output reg DataValid,
		output reg LinkStartOfPacket,
		output reg LinkEndOfPacket
    );

	//HEAD
	//34 bit    12+12+8+1+1
	//{SourcePacketSize, SourcePacketDest, KStart, 1'b1, 1'b0}
	
	 parameter KStart = 8'h5C;		//b 01011100; 	// k28.2 character marks packet start

	 reg [31:0] InputData_reg;		
	 reg [11:0] PacketLengthCounter;
	 reg DataValid_reg;
	 reg LinkStartOfPacket_reg, LinkEndOfPacket_reg;
	 //reg [7:0] AddressCounter;
	 reg [15:0] AddressCounter;		// need extension
	 
	 wire StartOfPacket;
	 wire [11:0] RIOpacketLength;
	 
	 
	 assign StartOfPacket = ((CharIsK) && (DataIn[7:0] == KStart));
	 assign RIOpacketLength = DataIn[31:20];
	 
	 
	 
	 always @ (posedge Clock)
	 begin
		InputData_reg <= DataIn;
		DataOut <= InputData_reg;
		DataValid <= DataValid_reg;
		LinkEndOfPacket_reg <= (PacketLengthCounter == 1);
		LinkStartOfPacket_reg <= StartOfPacket;
		LinkEndOfPacket <= LinkEndOfPacket_reg;
		LinkStartOfPacket <= LinkStartOfPacket_reg;
	 end
	 
	 // detect the begining of a packet and store address and length
	 always @ (posedge Clock)
	 if (StartOfPacket)
	 begin
		PacketAddress <= DataIn[19:8];	// address from packet sent to user
		PacketLength <= RIOpacketLength;	// Packet length in words
	 end
	 
	 // packet length counter is used to enable the input data going to
	 // the packet FIFO "myPacketBuffFIFO" for the length of the current packet 
	 always @ (posedge Clock)
	 begin
		if (Reset==1'b1) 
			PacketLengthCounter <= 9'b0;
		else if (StartOfPacket) 
			PacketLengthCounter <= (RIOpacketLength - 1); //remove one for header
		else if (PacketLengthCounter > 9'b0) 
			PacketLengthCounter <= PacketLengthCounter - 1;
		else 
			PacketLengthCounter <= PacketLengthCounter;
	 end
	 
	 
	 always @ (posedge Clock)
	 begin
		if (Reset) DataValid_reg <= 1'b0;
		
		else if (PacketLengthCounter >= 9'd2)
			DataValid_reg <= 1'b1;			
		else DataValid_reg <= 1'b0;
	 end

	 // Counter for generating an address for each word in the packet
	 always @ (posedge Clock)
	 begin
		//if (Reset) AddressCounter <= 8'd0;		
		//else if (StartOfPacket) AddressCounter <= DataIn[23:8];		
		if (Reset) AddressCounter <= 16'd0;
		else if (StartOfPacket) AddressCounter <= DataIn[19:8];	
		else if (DataValid_reg) AddressCounter <= (AddressCounter + 1);		
		else AddressCounter <= AddressCounter;
	 end
	 
  	 always @ (posedge Clock) MemoryAddress <= AddressCounter;
	 
	 
//`define	AA	 	
`ifdef	AA
		wire [35:0] CONTROL0;
		icon myIcon (
			.CONTROL0(CONTROL0) // INOUT BUS [35:0]
		); 
	
		wire [255:0] TRIG0;
		
		assign TRIG0[31:0]  = DataIn;
		assign TRIG0[63:32] = DataOut;
		assign TRIG0[75:64] = PacketLength;
		assign TRIG0[76]    = DataValid;
		assign TRIG0[92:77] = MemoryAddress;
		assign TRIG0[93]    = LinkStartOfPacket;
		assign TRIG0[94]    = LinkEndOfPacket;
		assign TRIG0[106:95] = PacketLengthCounter;
		assign TRIG0[107] = StartOfPacket;
		assign TRIG0[255]   = CharIsK;

				 
		ila myIla (
			 .CONTROL(CONTROL0), // INOUT BUS [35:0]
			 .CLK(Clock), // IN
			 .TRIG0(TRIG0) // IN BUS [99:0]
		); 	 
`endif
	 
endmodule
