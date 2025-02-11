// -------------------------------------------------------------
//
// File Name: hdlsrc\aie_limit.v
// Created: 2014-03-15 20:16:45
// Generated by MATLAB 7.6 and Simulink HDL Coder 1.3
//
//
// -------------------------------------------------------------


`timescale 1 ns / 1 ns

module aie_limit
          (
		   CLK,
		   LIMIT,
           pos_i,
           valid,
           pos_o,
           valid_n
          );

  input CLK;		  
  input   signed [31:0] LIMIT;
  input   signed [31:0] pos_i;  // int32
  output  valid;
  output  signed [31:0] pos_o;  // int32
  output  valid_n;


  //wire signed [31:0] const1_out1;  // int32
  wire Relational_Operator1_out1;
  //wire signed [31:0] const2_out1;  // int32
  wire Relational_Operator3_out1;
  wire Logical_Operator2_out1;
  wire signed [31:0] um_const_out1;  // int32
  //wire signed [31:0] Enable1_out1;  // int32
  //wire Logical_Operator1_out1;

  
  //assign const1_out1 = 32'h01312D00;
  //assign const2_out1 = 32'hFECED300;
  //assign Relational_Operator1_out1 = (pos_i <= const1_out1) ? 1'b1 : 1'b0;
  //assign Relational_Operator3_out1 = (pos_i >= const2_out1) ? 1'b1 : 1'b0;

  
  reg signed [31:0]  limit_pos, limit_neg;
  	always @ (posedge CLK) begin
		limit_pos <= LIMIT;
		limit_neg <= LIMIT * -1;		
	end
		
  assign Relational_Operator1_out1 = (pos_i <= limit_pos) ? 1'b1 : 1'b0;
  assign Relational_Operator3_out1 = (pos_i >= limit_neg) ? 1'b1 : 1'b0;
  
  

  assign Logical_Operator2_out1 =  Relational_Operator1_out1 & Relational_Operator3_out1;

  assign valid = Logical_Operator2_out1;
  assign um_const_out1 = 32'h000003e8;		//1000  = 1 um

  //assign Enable1_out1 = (Logical_Operator2_out1 == 1'b1) ? pos_i : um_const_out1;
  assign pos_o = (Logical_Operator2_out1 == 1'b1) ? pos_i : um_const_out1;
  //assign Logical_Operator1_out1 = !Logical_Operator2_out1;

  assign valid_n = !Logical_Operator2_out1;


endmodule  // aie_limit

