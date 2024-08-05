

`timescale 1 ns / 1 ns
//average f1_amp(filter1_amp, filter1_out, 8, AUD_DACLRCK);
/////////////////////////////////////////////////////
//// Time weighted average amplitude (2'comp) ///////
/////////////////////////////////////////////////////
// dk_const    e-folding time of average			         
// 3			~8 samples
// 4			15 
// 5			32
// 6			64
// 7			128 -- 2.7 mSec at 48kHz (2.083333333333333e-5)
// 8			256 -- 5.3 mSec (useful for music/voice)
// 9			512 -- 10.5 mSec (useful for music/voice)
// 10			1024 -- 21 mSec (useful for music/voice)
// 11			2048 -- 42 mSec
module average (out, in, dk_const, clk);

	output reg signed [31:0] out ;
	input wire signed [31:0] in ;
	input wire [3:0] dk_const ;
	input wire clk;
	
	wire signed  [31:0] new_out ;
	//first order lowpass of absolute value of input
	assign new_out = out - (out>>>dk_const) + ((in[31]?-in:in)>>>dk_const) ;
	always @(posedge clk)
	begin
		 out <= new_out ;
	end
endmodule

