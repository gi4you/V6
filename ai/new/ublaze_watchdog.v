`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    09:17:33 07/25/2012 
// Design Name: 
// Module Name:    trig_watchdog 
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
// Ref: https://code.ros.org/trac/ros-pkg/browser/stacks/camera_drivers/trunk/wge100_camera_firmware/src/hardware
//				/verilog/trig_watchdog.v?rev=33087
// 8 ns * 12500 = 100 us
// 14000 = 112 us
//
module ublaze_watchdog(/*AUTOARG*/
	   // Outputs
	   trig_watchdog_rst,
	   timeout_cnt,
	   trig_cnt,
	   // Inputs
	   sys_clk_i, 
	   reset,
	   UsrClear,
	   enable,
	   EXT_TRIG_B
	   );
	   
	// 10 ns clock, 
	//localparam WATCHDOG_COUNT = 32'd1000000000; 		// 10 sec @ 10 ns clock (100 MHz)
	// 20 ns clock
	localparam WATCHDOG_COUNT = 32'd5000000000;			// 10 sec @ 20 ns clock (50 MHz)
	

	input sys_clk_i;
	input reset;
	input UsrClear;
	input	enable;
	input EXT_TRIG_B;
	output trig_watchdog_rst;
	output reg [31:0] timeout_cnt;
	output reg [31:0] trig_cnt;


	reg [31:0] count = WATCHDOG_COUNT;
	//reg reg_trig_watchdog_rst = 0;
	reg watchdog_rst = 0;
	reg armed = 0;
	  	
	reg trigReg, trigReg_reg, TrigEdge; 
	always @ (posedge sys_clk_i) 
	begin 
		trigReg <= EXT_TRIG_B;
		trigReg_reg <= trigReg;
		TrigEdge <= (trigReg & !trigReg_reg);
	end
	
	always @(posedge sys_clk_i) begin
		if (reset == 1'b1 | UsrClear == 1'b1) begin
			watchdog_rst <= 0;
			count <= WATCHDOG_COUNT;
			timeout_cnt <= 32'd0;
			trig_cnt <= 32'd0;
			//
			armed <= 1;		// 11/22/2014  added if reset condition ?  ARMED enabled
		end	
		else if (TrigEdge == 1'b1) begin
		  armed <= 1;
		  count <= WATCHDOG_COUNT;
		  trig_cnt <= trig_cnt +1;  //increment
		end 
		else if(count > 32'd500)
			count <= count - 1;	
		else count <= count;
		

		if (armed && count < 32'd1000 ) begin
			watchdog_rst <= 1'b1;	//latch
			//watchdog_rst <= ~watchdog_rst;	//toggle
			timeout_cnt <= timeout_cnt + 1;
		end  
			
	end

	//assign trig_watchdog_rst = watchdog_rst;
	assign trig_watchdog_rst = (enable == 1'b1) ? watchdog_rst : 1'b0;
	  
	  
	endmodule
