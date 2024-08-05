`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    10:12:50 11/14/2012 
// Design Name: 
// Module Name:    dac_trig_gen 
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
module Dac_trig_gen(
	input clk,
	input trig,
	output dac_multi_trig,
	output trig_delay
    );

   //////// double trigger generator ////////////
   reg [9:0]  counter2=0;
   reg trig_dly=1'b0;
   
   
   always @(posedge clk) begin
		if(trig == 1'b1) begin
			counter2 <= 0;
			trig_dly <= 1'b1;
		end
		else begin	
			counter2 <= counter2 + 9'd1;
			if(counter2 > 10'd200) trig_dly <= 0;
		end	
	end	
	
	//first edge
	reg trigReg, trigReg_reg, TrigEdge; 
	always @ (posedge clk) 
	begin 
		trigReg <= trig_dly;
		trigReg_reg <= trigReg;
		TrigEdge <= (trigReg & !trigReg_reg);
	end
	// 2nd edge
	reg trigReg2, trigReg_reg2, TrigEdge2; 
	always @ (posedge clk) 
	begin 
		trigReg2 <= ~trig_dly;
		trigReg_reg2 <= trigReg2;
		TrigEdge2 <= (trigReg2 & !trigReg_reg2);
	end
	
	assign dac_multi_trig = 	TrigEdge | TrigEdge2;	
	assign trig_delay = trig_dly;
    ////////
	
endmodule
