/*
 *	
 *
 *	8/29/13
 *	kha@bnl.gov
 *
 *	03/15/2014
 *		1) Added BM_address to 'Integer_Delay_out1' 
 *		BM_address <= { Integer_Delay_out1
 *
 *		2) IL_SUM timing correction with 'BM_IL_ADDRESS'
 *		3) err_cnt_o signal added for error count
 *
 *	05/30/2014
 *		assign Logical_xyPosOutOfLimit =  enb &&  
 *
 *	03/25/15 added for zero detection
 *	assign bm_bpm_fault_detected = (AvgCal_y_out1 == 32'd0) ? 1'b1 : 1'b0;
 *	assign y_h_out = bm_bpm_fault_detected;  
 *	assign y_l_out = bm_bpm_fault_detected;
 *
 *	03/30/15
 *		bm_bpm_fault_detected   logic changed
 *
 *	06/10/2015
 *		bm_pos_limit  set point
 *	6/11/15
 *		IL_GlitchLatchedOut
 *
 *	11/01/2015 out of envelop error position count
 *		   xx_pos_out_cnt,
 *		   yy_pos_out_cnt
 *		   
 */


`timescale 1 ns / 1 ns


`define	NOF_BPM_POS_OUT		4	//x,y all count

module bm_position_interlock_nm
          (
           clk,
           reset,
		   //ClearLatch,
		   trig,
           clk_enable,
		   bm_xy_Mask_DataValid,	//added 03/31/15
           x,
           y,
		   posAddress,
           x_h_limit,
           x_low_limit,
           y_h_limit,
           y_low_limit,
           avg_num,		//2/(N+1)
           ce_out,
           x_avg_out,
           y_avg_out,
           x_h_out,
           x_l_out,
           y_h_out,
           y_l_out,
		   IL_SUM,
           IL_LATCHED_OUT,	//latched output
		   IL_GlitchLatchedOut,
		   err_cnt_o,	//interlock count
		   BM_IL_ADDRESS,		   
		   bm_pos_limit,
		   err_flag,
		   
		   xx_pos_out_cnt,
		   yy_pos_out_cnt
          );


  input   clk;
  input   reset;
  //input		ClearLatch;		//06/11/15
  input		trig;
  input   clk_enable;
  input		bm_xy_Mask_DataValid;
  input   signed [31:0] x;  // int32
  input   signed [31:0] y;  // int32
  input	  [9:0] posAddress;
  input   signed [31:0] x_h_limit;  // int32
  input   signed [31:0] x_low_limit;  // int32
  input   signed [31:0] y_h_limit;  // int32
  input   signed [31:0] y_low_limit;  // int32
  input   [15:0] avg_num;  // ufix16_En15
  output  ce_out;
  output  signed [31:0] x_avg_out;  // int32
  output  signed [31:0] y_avg_out;  // int32
  output  x_h_out;
  output  x_l_out;
  output  y_h_out;
  output  y_l_out;
  output  IL_SUM;	
  output  IL_LATCHED_OUT;
  output [9:0] err_cnt_o;
  output  [14:0] BM_IL_ADDRESS;
  output	IL_GlitchLatchedOut;
  input	[31:0] bm_pos_limit;
  output	err_flag;
  output	[15:0] xx_pos_out_cnt;
  output	[15:0] yy_pos_out_cnt;

  wire enb;
  wire enb_1_1_1;
  wire signed [31:0] AvgCal_x_out1;  // int32
  wire signed [31:0] AvgCal_y_out1;  // int32
  wire Relational_Operator1_out1;
  wire Relational_Operator_out1;
  wire bm_bpm_fault_detected;
  wire Relational_Operator2_out1;
  wire Logical_xyPosOutOfLimit;
  reg  Integer_Delay_out1;
  
  reg	[13:0] BM_address;
  reg	latched_data;
  reg 	trig0, trig1, trig2;
	
  //11/01/15
  reg	[15:0] xx_pos_out_cntReg, yy_pos_out_cntReg;
  wire	xx_out_flag, yy_out_flag;
  
  assign enb_1_1_1 = clk_enable;
  assign ce_out = enb_1_1_1;
  assign enb = clk_enable;
  
 ////////////////// 
`ifdef	AVG_ENABLED  
  AvgCal_x   u_AvgCal_x   (.clk(clk),
                           .reset(reset),
                           .enb(enb),
                           .tbt_in(x),  // int32
                           .avg_num(avg_num),  // ufix16_En15
                           .avg_out(AvgCal_x_out1)  // int32
                           );

  assign x_avg_out = AvgCal_x_out1;
  AvgCal_x   u_AvgCal_y   (.clk(clk),
                           .reset(reset),
                           .enb(enb),
                           .tbt_in(y),  // int32
                           .avg_num(avg_num),  // ufix16_En15
                           .avg_out(AvgCal_y_out1)  // int32
                           );

`else

	// +/- 20 mm limit values, if bigger or less then +/- 20 mm, output will 0
	aie_limit	_x_limit
          (
		   .CLK(clk),
		   .LIMIT( bm_pos_limit ),			  
           .pos_i(x),
           .valid(),
           .pos_o(AvgCal_x_out1),
           .valid_n(xx_out_flag)
          );
		  
	aie_limit	_y_limit
          (
		   .CLK(clk),
		   .LIMIT( bm_pos_limit ),			  
           .pos_i(y),
           .valid(),
           .pos_o(AvgCal_y_out1),
           .valid_n(yy_out_flag)
          );	

		     
	// 03/15/2014 added register 
	/*
	reg	[9:0]  posAddress_reg;
	reg signed [31:0] AvgCal_x_out1;  
	reg signed [31:0] AvgCal_y_out1;  
 
	always @ (posedge clk) begin
		AvgCal_x_out1 <= x;
		AvgCal_y_out1 <= y;	
		posAddress_reg <= posAddress;		
	end
	*/
	reg signed [31:0] x_reg;  
	reg signed [31:0] y_reg; 	
	always @ (posedge clk) begin
		x_reg <= AvgCal_x_out1;
		y_reg <= AvgCal_y_out1;		
	end
	
	assign x_avg_out = x_reg;
	assign y_avg_out = y_reg;
	
`endif
////////////////////////////
  
  
  assign Relational_Operator1_out1 = (AvgCal_x_out1 >= x_h_limit) ? 1'b1 : 1'b0;
  assign x_h_out = Relational_Operator1_out1;
  
  assign Relational_Operator_out1 = (AvgCal_x_out1 <= x_low_limit) ? 1'b1 : 1'b0;
  assign x_l_out = Relational_Operator_out1;
  
  /*
  assign bm_bpm_fault_detected = (AvgCal_y_out1 >= y_h_limit) ? 1'b1 : 1'b0;
  assign y_h_out = bm_bpm_fault_detected;  
  assign Relational_Operator2_out1 = (AvgCal_y_out1 <= y_low_limit) ? 1'b1 : 1'b0;
  assign y_l_out = Relational_Operator2_out1;
  */
  
	// 03/25/15 added for zero detection
	assign bm_bpm_fault_detected = ( (AvgCal_y_out1 == 32'd0) && (bm_xy_Mask_DataValid == 1'b1)) ? 1'b1 : 1'b0;
	assign y_h_out = bm_bpm_fault_detected;  
	assign y_l_out = bm_bpm_fault_detected;
  
  
  assign Relational_Operator2_out1 = 1'b0;
  
  //05/30/2014
  //if enb is 0, output is 0, only valid >50 mA and Mask valid
   //assign Logical_xyPosOutOfLimit =  enb && (Relational_Operator1_out1 | Relational_Operator_out1 | bm_bpm_fault_detected | Relational_Operator2_out1);
  
   assign Logical_xyPosOutOfLimit =  enb && (Relational_Operator1_out1 | Relational_Operator_out1 );

  always @ (posedge clk or posedge reset)
    begin: Integer_Delay_process
      if (reset == 1'b1 ) begin
        Integer_Delay_out1 <= 0;
		BM_address <= 14'b00001111111111;
		latched_data <= 1'b0; //clear
      end
      else begin
		if (trig2 == 1'b1)	 begin
			latched_data <= 1'b0;	//clear every 10 kHz
		end
		else if (enb == 1'b1 && ( Logical_xyPosOutOfLimit == 1'b1 | bm_bpm_fault_detected == 1'b1) ) begin					
			latched_data <= 1'b1;
			
			Integer_Delay_out1 <= Logical_xyPosOutOfLimit;
			BM_address <= { Integer_Delay_out1, bm_bpm_fault_detected, Relational_Operator2_out1, Relational_Operator1_out1, Relational_Operator_out1, posAddress};			
        end
		else begin
			Integer_Delay_out1 <= Integer_Delay_out1;
			BM_address <= BM_address;
		end	
      end
    end // Integer_Delay_process

	reg il_sum_reg;
	always @ (posedge clk)
		il_sum_reg <= Logical_xyPosOutOfLimit;	//one clock delay
		
		
	always @ (posedge clk) begin
		trig0 <= trig;
		trig1 <= trig0;
		trig2 <= trig1;
	end
	
	// Every cycle, count how many bpms caused interlock
	reg [9:0] err_cnt;

	always @ (posedge clk) begin
      if (trig1 == 1'b1) begin		// clearing 2 cycle later
        err_cnt <= 10'd0;
      end
      else begin
        if (enb == 1'b1 & Logical_xyPosOutOfLimit == 1'b1) begin			
			err_cnt <= err_cnt+1;
        end
		else begin
			err_cnt <= err_cnt;
		end	
      end	
	end
	assign	err_cnt_o = err_cnt;
	
	
	/*
	  -------------          |----------------------
	               |_________|
	                           _ 
	  ________________________| |____________________
	 */
	reg trig_eoe; 
	reg T0Reg, T0Reg_reg1, T0Reg_reg2; 
	always @ (posedge clk) 
	begin 
		T0Reg <= ~enb; 		//BM data valid during 360 cycle
		T0Reg_reg1 <= T0Reg;
		T0Reg_reg2 <= T0Reg_reg1;
		trig_eoe   <= (T0Reg_reg1 & !T0Reg_reg2);
	end
	
	reg	beam_dump_check;
	always @ (posedge clk) begin
      if (reset == 1'b1) begin
        beam_dump_check <= 1'b0;
      end
      else begin
		if (trig_eoe == 1'b1) begin		// 11/01/15 every cycle error count is > 3 bpms 
			if( err_cnt >= `NOF_BPM_POS_OUT ) begin
				beam_dump_check <= 1'b1;
			end
        end
		else begin
			beam_dump_check <= beam_dump_check;
		end	
      end	
	end

	
	/////////////////////////////////////////////////////////////////
	//////////// 11/01/2015		x, y out of envelop count 
	always @ (posedge clk) begin
      if (reset == 1'b1) begin		
        xx_pos_out_cntReg <= 15'd0;
      end
      else begin
        if (bm_xy_Mask_DataValid == 1'b1 & xx_out_flag == 1'b1) begin			
			xx_pos_out_cntReg <= xx_pos_out_cntReg+1;
        end
		else begin
			xx_pos_out_cntReg <= xx_pos_out_cntReg;
		end	
      end	
	end
	
	always @ (posedge clk) begin
      if (reset == 1'b1) begin		
        yy_pos_out_cntReg <= 15'd0;
      end
      else begin
        if (bm_xy_Mask_DataValid == 1'b1 & xx_out_flag == 1'b1) begin			
			yy_pos_out_cntReg <= yy_pos_out_cntReg+1;
        end
		else begin
			yy_pos_out_cntReg <= yy_pos_out_cntReg;
		end	
      end	
	end
	
	assign xx_pos_out_cnt = xx_pos_out_cntReg;
	assign yy_pos_out_cnt = yy_pos_out_cntReg;
	
	
	assign IL_SUM = il_sum_reg;	//03/15/2014 one clock delay for sync with 'BM_address'
	assign IL_LATCHED_OUT = Integer_Delay_out1 ;		//BM_LATCHED_OUT
	assign IL_GlitchLatchedOut = latched_data;
		
	//Which BPM position is Faulted???
	assign BM_IL_ADDRESS = BM_address;
	assign err_flag = beam_dump_check;
	
endmodule  // position_interlock_nm

