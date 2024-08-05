// -------------------------------------------------------------
//
// File Name: hdlsrc\dac_offset_calc.v
// Created: 2012-12-15 23:33:15
// Generated by MATLAB 7.6 and Simulink HDL Coder 1.3
//
//
// -------------------------------------------------------------
//	Input signal is 32 bit, but Data value is recommend 0 ~ 32767
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module dac_offset_calc
          (
           InS32b,
           OutU16b
          );


  input   signed [31:0] InS32b;  // int32
  output  [15:0] OutU16b;  // uint16


  wire signed [15:0] ToFixPt_out1;  // int16
  wire signed [15:0] OFFSET_out1;  // int16
  wire [15:0] Sum2_out1;  // uint16
  wire signed [16:0] Sum2_out1_tmp;  // sfix17
  wire signed [16:0] add_cast;  // sfix17
  wire signed [16:0] add_cast_1;  // sfix17
  wire signed [17:0] add_temp;  // sfix18

  assign ToFixPt_out1 = ((InS32b[31] == 1'b0 & InS32b[30:15] != 16'b0000000000000000) || (InS32b[31] == 1'b0 && InS32b[15:0] == 16'b0111111111111111) // special case0
) ? 16'b0111111111111111 : 
      (InS32b[31] == 1'b1 && InS32b[30:15] != 16'b1111111111111111) ? 16'b1000000000000000 : InS32b[15:0];

  assign OFFSET_out1 = 16'b0111111111111111;

  assign add_cast = $signed({{1{ToFixPt_out1[15]}}, ToFixPt_out1});
  assign add_cast_1 = $signed({{1{OFFSET_out1[15]}}, OFFSET_out1});
  assign add_temp = add_cast + add_cast_1;
  assign Sum2_out1_tmp = (add_temp[17] == 1'b0 & add_temp[16] != 1'b0) ? 17'b01111111111111111 : 
      (add_temp[17] == 1'b1 && add_temp[16] != 1'b1) ? 17'b10000000000000000 : add_temp[16:0];

  assign Sum2_out1 = (Sum2_out1_tmp[16] == 1'b1 ) ? 16'b0000000000000000 : $unsigned(Sum2_out1_tmp[15:0]);

  assign OutU16b = Sum2_out1;

endmodule  // dac_offset_calc

