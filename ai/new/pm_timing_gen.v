`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    15:06:41 09/03/2013 
// Design Name: 
// Module Name:    pm_timing_gen 
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
module pm_timing_gen(

	input	clk,
	input 	reset,
	input	trig,
	input	falt_trig,
	input	[31:0] periode,
	input	usr_rst,	//reset by user
	//output	npi_trig,
	output	npi_stop,
	output	npi_enable,	
	output	RingCntRstOut,
	output	[31:0] RingCntFaPmTrigCnt_out
    );


	wire RstOut;
	
	RampGen __npi_start (
		.clk(clk), 
		.reset(reset), 
		.trig(trig & npi_enable), 	
		.Ref_CNT(periode), 
		.DataFreeRunOut( RingCntFaPmTrigCnt_out ), 	//address count
		.RstOut(RstOut)		
	);
	assign	RingCntRstOut = RstOut;
	//assign npi_trig = npi_enable & RstOut;	// NPI reset every period only npi enabled
	
	wire npi_stop;
	wire [31:0] FaultFreeRunOut;
	wire falt_trig;
	FaultRampGen __npi_stop (
		.clk(clk), 
		.reset(reset), 
		.usr_rst(usr_rst),
		.trig( trig ),
		.fault_trig( falt_trig ),		
		//.Ref_CNT(32'd2000),  		//stop size npi_stop after fault trigger, 0.2 sec
		.Ref_CNT(32'd1000),
		.DataFreeRunOut(), 
		.RstOut(npi_stop),
		.npi_enable(npi_enable)
	);
	
	
endmodule
