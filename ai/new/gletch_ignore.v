`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    18:50:40 06/18/2015 
// Design Name: 
// Module Name:    gletch_ignore 
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
module gletch_ignore(
	input clk,
	input trig,	
	input	[3:0] NofCycle,
	input reset,
	input	sigIn,
	output	trip_out
    );

	// Tested at ISE
	//  2 : single pulse ignore
	//	1 : single 100 us pulse accepted
	//
	//parameter CNT  = 1;
	
	reg trip_status;
	reg [3:0] trip_count;
	
	always @ (posedge clk)
    begin
		if (reset == 1'b1  ) begin
			trip_count <= 0;		
			trip_status <= 1'b0;
		end
		else begin
			if(trig == 1'b1) begin
				if ( trip_count >= NofCycle ) begin
					trip_count  <= trip_count;
					trip_status <= 1'b1;
				end	
				else if ( sigIn == 1'b1) begin
					trip_count  <= trip_count + 1;		
					trip_status <= 1'b0;
				end	
				else begin
					trip_status <= 1'b0;
					trip_status <= 0;
				end	
			end	
		end		
    end 	
	 
	 
	 assign trip_out = trip_status;
endmodule
