// -------------------------------------------------------------
//
// File Name: hdlsrc\fa_um.v
// Created: 2014-04-21 09:54:43
//
//
// -------------------------------------------------------------



`timescale 1 ns / 1 ns

module fa_um
          (
           clk,
           reset,
           clk_enable,
           x_nm_i,
           i_gain,
           factor,
           ce_out,
           x_um_filt_o,
           x_um_o
          );


  input   clk;
  input   reset;
  input   clk_enable;
  input   signed [31:0] x_nm_i;  // int32
  input   [31:0] i_gain;  // uint32
  input   signed [15:0] factor;  // sfix16_En15
  output  ce_out;
  output  signed [31:0] x_um_filt_o;  // int32
  output  signed [31:0] x_um_o;  // int32

  parameter signed [31:0] dt_Int_1_InitialCondition = 0;  // int32
  parameter signed [31:0] dt_Int_1_gainval = 439804651;  // int32

  wire enb;
  wire enb_1_1_1;
  wire signed [31:0] mult0_out1;  // int32
  wire signed [31:0] dt_Int_1_out1;  // int32
  wire signed [31:0] Sum13_out1;  // int32
  wire signed [31:0] mult1_out1;  // int32
  reg signed [31:0] dt_Int_1_state0;  // int32
  wire signed [31:0] dt_Int_1_state0_next;  // int32
  wire signed [63:0] t_2;  // sfix64
  wire signed [31:0] t_3;  // int32
  wire signed [47:0] mul_temp;  // sfix48_En15
  wire signed [32:0] Sum13_out1_tmp;  // sfix33
  wire signed [32:0] sub_cast;  // sfix33
  wire signed [32:0] sub_cast_1;  // sfix33
  wire signed [33:0] sub_temp;  // sfix34
  wire signed [64:0] mul_temp_1;  // sfix65

  assign enb_1_1_1 = clk_enable;
  assign ce_out = enb_1_1_1;
  assign mul_temp = x_nm_i * factor;
  assign mult0_out1 = $signed({mul_temp[46:15]} + (mul_temp[47] & |mul_temp[14:0]));

  assign sub_cast = $signed({{1{mult0_out1[31]}}, mult0_out1});
  assign sub_cast_1 = $signed({{1{dt_Int_1_out1[31]}}, dt_Int_1_out1});
  assign sub_temp = sub_cast - sub_cast_1;
  assign Sum13_out1_tmp = (sub_temp[33] == 1'b0 & sub_temp[32] != 1'b0) ? 33'b011111111111111111111111111111111 : 
      (sub_temp[33] == 1'b1 && sub_temp[32] != 1'b1) ? 33'b100000000000000000000000000000000 : sub_temp[32:0];

  assign Sum13_out1 = (Sum13_out1_tmp[32] == 1'b0 & Sum13_out1_tmp[31] != 1'b0) ? 32'b01111111111111111111111111111111 : 
      (Sum13_out1_tmp[32] == 1'b1 && Sum13_out1_tmp[31] != 1'b1) ? 32'b10000000000000000000000000000000 : Sum13_out1_tmp[31:0];

  assign mul_temp_1 = $signed({1'b0, i_gain}) * Sum13_out1;
  assign mult1_out1 = mul_temp_1[31:0];

  always @(posedge(reset), posedge(clk))
    begin
      if (reset)
        dt_Int_1_state0 <= dt_Int_1_InitialCondition;
      else 
        if (enb)
          dt_Int_1_state0 <= dt_Int_1_state0_next;

    end

  assign dt_Int_1_out1 = dt_Int_1_state0;
  assign t_2 = dt_Int_1_gainval * mult1_out1;
  assign t_3 = {{10{t_2[63]}}, t_2[63:42]};
  assign dt_Int_1_state0_next = dt_Int_1_state0 + t_3;


  assign x_um_filt_o = dt_Int_1_out1;
  assign x_um_o = mult0_out1;
  assign enb = clk_enable;

endmodule  // fa_um

