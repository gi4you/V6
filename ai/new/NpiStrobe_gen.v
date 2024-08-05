`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    20:52:00 05/23/2012 
// Design Name: 
// Module Name:    strb_gen 
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

module NpiStrobe_gen(
		input				Clock,
		input 				Reset,
		input 			 	myEvent,
		input 	[31:0]	myDelay,
		input	[31:0]	myWidth,
		output 				trigger,
		output 				npiRst
    );

	 reg [31:0] delayCounter;
	 reg [31:0] widthCounter;
	 reg startDelay;
	 reg startWidth;

	 wire trigVal;

	  wire trig;
	  
	 pos_edge start
			  (
			   .clk(Clock),
			   .reset(Reset),
			   .enb(1'b1),
			   .trig_in(myEvent),
			   .trig_out(trig)
			  );
			  
			  
	 always @ (posedge Clock)
	 begin
		if (Reset) delayCounter <= 0;
		else if (startDelay) delayCounter <= delayCounter + 1;
		else if (delayCounter >= (myDelay)) delayCounter <= 0;
		else delayCounter <= delayCounter;
	 end
	 
	 always @ (posedge Clock)
	 begin
		if (Reset) widthCounter <= 0;
		else if (startWidth) widthCounter <= widthCounter + 1;
		else if (widthCounter >= myWidth) widthCounter <= 0;
		else widthCounter <= widthCounter;
	 end
	 
	 always @ (posedge Clock)
	 begin
		if (Reset) startDelay <= 0;
		else if (trig == 1'b1) startDelay <= 1;
		else if (delayCounter == (myDelay - 1)) startDelay <= 0;
		else startDelay <= startDelay;
	 end
	 
	 always @ (posedge Clock)
	 begin
		if (Reset) startWidth <= 0;
		else if (delayCounter == (myDelay)) startWidth <= 1;
		else if (widthCounter == (myWidth - 1)) startWidth <= 0;
		else startWidth <= startWidth;
	 end
	 
	  
	 //assign triggVal = (myPolarity) ? !startWidth : startWidth;
	 assign triggVal = startWidth;
	 assign trigger = ((myDelay != 32'd0) && (myWidth != 32'd0)) ? triggVal : 1'b0;
	 assign npiRst = trig;
	 

	
endmodule
