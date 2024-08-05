////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: P.68d
//  \   \         Application: netgen
//  /   /         Filename: addsb_11_0_61bea11e4a7792d3.v
// /___/   /\     Timestamp: Thu Dec 05 08:06:49 2013
// \   \  /  \ 
//  \___\/\___\
//             
// Command	: -w -sim -ofmt verilog C:/Users/kha/AppData/Local/Temp/1/sysgentmp-kha/cg_wk/c1cfdeebce5d23323/tmp/_cg/addsb_11_0_61bea11e4a7792d3.ngc C:/Users/kha/AppData/Local/Temp/1/sysgentmp-kha/cg_wk/c1cfdeebce5d23323/tmp/_cg/addsb_11_0_61bea11e4a7792d3.v 
// Device	: 6vlx240tff1156-1
// Input file	: C:/Users/kha/AppData/Local/Temp/1/sysgentmp-kha/cg_wk/c1cfdeebce5d23323/tmp/_cg/addsb_11_0_61bea11e4a7792d3.ngc
// Output file	: C:/Users/kha/AppData/Local/Temp/1/sysgentmp-kha/cg_wk/c1cfdeebce5d23323/tmp/_cg/addsb_11_0_61bea11e4a7792d3.v
// # of Modules	: 1
// Design Name	: addsb_11_0_61bea11e4a7792d3
// Xilinx        : c:\xilinx\14.6\ise_ds\ise\
//             
// Purpose:    
//     This verilog netlist is a verification model and uses simulation 
//     primitives which may not represent the true implementation of the 
//     device, however the netlist is functionally correct and should not 
//     be modified. This file cannot be synthesized and should only be used 
//     with supported simulation tools.
//             
// Reference:  
//     Command Line Tools User Guide, Chapter 23 and Synthesis and Simulation Design Guide, Chapter 6
//             
////////////////////////////////////////////////////////////////////////////////

`timescale 1 ns/1 ps

module addsb_11_0_61bea11e4a7792d3 (
  clk, ce, a, b, s
)/* synthesis syn_black_box syn_noprune=1 */;
  input clk;
  input ce;
  input [32 : 0] a;
  input [32 : 0] b;
  output [32 : 0] s;
  
  // synthesis translate_off
  
  wire \blk00000001/sig000000c8 ;
  wire \blk00000001/sig000000c7 ;
  wire \blk00000001/sig000000c6 ;
  wire \blk00000001/sig000000c5 ;
  wire \blk00000001/sig000000c4 ;
  wire \blk00000001/sig000000c3 ;
  wire \blk00000001/sig000000c2 ;
  wire \blk00000001/sig000000c1 ;
  wire \blk00000001/sig000000c0 ;
  wire \blk00000001/sig000000bf ;
  wire \blk00000001/sig000000be ;
  wire \blk00000001/sig000000bd ;
  wire \blk00000001/sig000000bc ;
  wire \blk00000001/sig000000bb ;
  wire \blk00000001/sig000000ba ;
  wire \blk00000001/sig000000b9 ;
  wire \blk00000001/sig000000b8 ;
  wire \blk00000001/sig000000b7 ;
  wire \blk00000001/sig000000b6 ;
  wire \blk00000001/sig000000b5 ;
  wire \blk00000001/sig000000b4 ;
  wire \blk00000001/sig000000b3 ;
  wire \blk00000001/sig000000b2 ;
  wire \blk00000001/sig000000b1 ;
  wire \blk00000001/sig000000b0 ;
  wire \blk00000001/sig000000af ;
  wire \blk00000001/sig000000ae ;
  wire \blk00000001/sig000000ad ;
  wire \blk00000001/sig000000ac ;
  wire \blk00000001/sig000000ab ;
  wire \blk00000001/sig000000aa ;
  wire \blk00000001/sig000000a9 ;
  wire \blk00000001/sig000000a8 ;
  wire \blk00000001/sig000000a7 ;
  wire \blk00000001/sig000000a6 ;
  wire \blk00000001/sig000000a5 ;
  wire \blk00000001/sig000000a4 ;
  wire \blk00000001/sig000000a3 ;
  wire \blk00000001/sig000000a2 ;
  wire \blk00000001/sig000000a1 ;
  wire \blk00000001/sig000000a0 ;
  wire \blk00000001/sig0000009f ;
  wire \blk00000001/sig0000009e ;
  wire \blk00000001/sig0000009d ;
  wire \blk00000001/sig0000009c ;
  wire \blk00000001/sig0000009b ;
  wire \blk00000001/sig0000009a ;
  wire \blk00000001/sig00000099 ;
  wire \blk00000001/sig00000098 ;
  wire \blk00000001/sig00000097 ;
  wire \blk00000001/sig00000096 ;
  wire \blk00000001/sig00000095 ;
  wire \blk00000001/sig00000094 ;
  wire \blk00000001/sig00000093 ;
  wire \blk00000001/sig00000092 ;
  wire \blk00000001/sig00000091 ;
  wire \blk00000001/sig00000090 ;
  wire \blk00000001/sig0000008f ;
  wire \blk00000001/sig0000008e ;
  wire \blk00000001/sig0000008d ;
  wire \blk00000001/sig0000008c ;
  wire \blk00000001/sig0000008b ;
  wire \blk00000001/sig0000008a ;
  wire \blk00000001/sig00000089 ;
  wire \blk00000001/sig00000088 ;
  wire \blk00000001/sig00000087 ;
  wire \blk00000001/sig00000086 ;
  wire \blk00000001/sig00000085 ;
  wire \blk00000001/sig00000084 ;
  wire \blk00000001/sig00000083 ;
  wire \blk00000001/sig00000082 ;
  wire \blk00000001/sig00000081 ;
  wire \blk00000001/sig00000080 ;
  wire \blk00000001/sig0000007f ;
  wire \blk00000001/sig0000007e ;
  wire \blk00000001/sig0000007d ;
  wire \blk00000001/sig0000007c ;
  wire \blk00000001/sig0000007b ;
  wire \blk00000001/sig0000007a ;
  wire \blk00000001/sig00000079 ;
  wire \blk00000001/sig00000078 ;
  wire \blk00000001/sig00000077 ;
  wire \blk00000001/sig00000076 ;
  wire \blk00000001/sig00000075 ;
  wire \blk00000001/sig00000074 ;
  wire \blk00000001/sig00000073 ;
  wire \blk00000001/sig00000072 ;
  wire \blk00000001/sig00000071 ;
  wire \blk00000001/sig00000070 ;
  wire \blk00000001/sig0000006f ;
  wire \blk00000001/sig0000006e ;
  wire \blk00000001/sig0000006d ;
  wire \blk00000001/sig0000006c ;
  wire \blk00000001/sig0000006b ;
  wire \blk00000001/sig0000006a ;
  wire \blk00000001/sig00000069 ;
  wire \blk00000001/sig00000068 ;
  wire \blk00000001/sig00000067 ;
  wire \blk00000001/sig00000066 ;
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000085  (
    .I0(a[32]),
    .I1(b[32]),
    .O(\blk00000001/sig000000a7 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000084  (
    .I0(a[31]),
    .I1(b[31]),
    .O(\blk00000001/sig00000088 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000083  (
    .I0(a[30]),
    .I1(b[30]),
    .O(\blk00000001/sig00000089 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000082  (
    .I0(a[29]),
    .I1(b[29]),
    .O(\blk00000001/sig0000008a )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000081  (
    .I0(a[28]),
    .I1(b[28]),
    .O(\blk00000001/sig0000008b )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000080  (
    .I0(a[27]),
    .I1(b[27]),
    .O(\blk00000001/sig0000008c )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk0000007f  (
    .I0(a[26]),
    .I1(b[26]),
    .O(\blk00000001/sig0000008d )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk0000007e  (
    .I0(a[25]),
    .I1(b[25]),
    .O(\blk00000001/sig0000008e )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk0000007d  (
    .I0(a[24]),
    .I1(b[24]),
    .O(\blk00000001/sig0000008f )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk0000007c  (
    .I0(a[23]),
    .I1(b[23]),
    .O(\blk00000001/sig00000090 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk0000007b  (
    .I0(a[22]),
    .I1(b[22]),
    .O(\blk00000001/sig00000091 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk0000007a  (
    .I0(a[21]),
    .I1(b[21]),
    .O(\blk00000001/sig00000092 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000079  (
    .I0(a[20]),
    .I1(b[20]),
    .O(\blk00000001/sig00000093 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000078  (
    .I0(a[19]),
    .I1(b[19]),
    .O(\blk00000001/sig00000094 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000077  (
    .I0(a[18]),
    .I1(b[18]),
    .O(\blk00000001/sig00000095 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000076  (
    .I0(a[17]),
    .I1(b[17]),
    .O(\blk00000001/sig00000096 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000075  (
    .I0(a[16]),
    .I1(b[16]),
    .O(\blk00000001/sig00000097 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000074  (
    .I0(a[15]),
    .I1(b[15]),
    .O(\blk00000001/sig00000098 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000073  (
    .I0(a[14]),
    .I1(b[14]),
    .O(\blk00000001/sig00000099 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000072  (
    .I0(a[13]),
    .I1(b[13]),
    .O(\blk00000001/sig0000009a )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000071  (
    .I0(a[12]),
    .I1(b[12]),
    .O(\blk00000001/sig0000009b )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000070  (
    .I0(a[11]),
    .I1(b[11]),
    .O(\blk00000001/sig0000009c )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk0000006f  (
    .I0(a[10]),
    .I1(b[10]),
    .O(\blk00000001/sig0000009d )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk0000006e  (
    .I0(a[9]),
    .I1(b[9]),
    .O(\blk00000001/sig0000009e )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk0000006d  (
    .I0(a[8]),
    .I1(b[8]),
    .O(\blk00000001/sig0000009f )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk0000006c  (
    .I0(a[7]),
    .I1(b[7]),
    .O(\blk00000001/sig000000a0 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk0000006b  (
    .I0(a[6]),
    .I1(b[6]),
    .O(\blk00000001/sig000000a1 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk0000006a  (
    .I0(a[5]),
    .I1(b[5]),
    .O(\blk00000001/sig000000a2 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000069  (
    .I0(a[4]),
    .I1(b[4]),
    .O(\blk00000001/sig000000a3 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000068  (
    .I0(a[3]),
    .I1(b[3]),
    .O(\blk00000001/sig000000a4 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000067  (
    .I0(a[2]),
    .I1(b[2]),
    .O(\blk00000001/sig000000a5 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000066  (
    .I0(a[1]),
    .I1(b[1]),
    .O(\blk00000001/sig000000a6 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \blk00000001/blk00000065  (
    .I0(a[0]),
    .I1(b[0]),
    .O(\blk00000001/sig000000a8 )
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000064  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000067 ),
    .Q(s[0])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000063  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000087 ),
    .Q(s[1])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000062  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000086 ),
    .Q(s[2])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000061  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000085 ),
    .Q(s[3])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000060  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000084 ),
    .Q(s[4])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000005f  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000083 ),
    .Q(s[5])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000005e  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000082 ),
    .Q(s[6])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000005d  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000081 ),
    .Q(s[7])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000005c  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000080 ),
    .Q(s[8])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000005b  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig0000007f ),
    .Q(s[9])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000005a  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig0000007e ),
    .Q(s[10])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000059  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig0000007d ),
    .Q(s[11])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000058  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig0000007c ),
    .Q(s[12])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000057  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig0000007b ),
    .Q(s[13])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000056  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig0000007a ),
    .Q(s[14])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000055  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000079 ),
    .Q(s[15])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000054  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000078 ),
    .Q(s[16])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000053  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000077 ),
    .Q(s[17])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000052  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000076 ),
    .Q(s[18])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000051  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000075 ),
    .Q(s[19])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000050  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000074 ),
    .Q(s[20])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000004f  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000073 ),
    .Q(s[21])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000004e  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000072 ),
    .Q(s[22])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000004d  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000071 ),
    .Q(s[23])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000004c  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000070 ),
    .Q(s[24])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000004b  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig0000006f ),
    .Q(s[25])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000004a  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig0000006e ),
    .Q(s[26])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000049  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig0000006d ),
    .Q(s[27])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000048  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig0000006c ),
    .Q(s[28])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000047  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig0000006b ),
    .Q(s[29])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000046  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig0000006a ),
    .Q(s[30])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000045  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000069 ),
    .Q(s[31])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000044  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000068 ),
    .Q(s[32])
  );
  MUXCY   \blk00000001/blk00000043  (
    .CI(\blk00000001/sig00000066 ),
    .DI(a[0]),
    .S(\blk00000001/sig000000a8 ),
    .O(\blk00000001/sig000000c8 )
  );
  MUXCY   \blk00000001/blk00000042  (
    .CI(\blk00000001/sig000000c8 ),
    .DI(a[1]),
    .S(\blk00000001/sig000000a6 ),
    .O(\blk00000001/sig000000c7 )
  );
  MUXCY   \blk00000001/blk00000041  (
    .CI(\blk00000001/sig000000c7 ),
    .DI(a[2]),
    .S(\blk00000001/sig000000a5 ),
    .O(\blk00000001/sig000000c6 )
  );
  MUXCY   \blk00000001/blk00000040  (
    .CI(\blk00000001/sig000000c6 ),
    .DI(a[3]),
    .S(\blk00000001/sig000000a4 ),
    .O(\blk00000001/sig000000c5 )
  );
  MUXCY   \blk00000001/blk0000003f  (
    .CI(\blk00000001/sig000000c5 ),
    .DI(a[4]),
    .S(\blk00000001/sig000000a3 ),
    .O(\blk00000001/sig000000c4 )
  );
  MUXCY   \blk00000001/blk0000003e  (
    .CI(\blk00000001/sig000000c4 ),
    .DI(a[5]),
    .S(\blk00000001/sig000000a2 ),
    .O(\blk00000001/sig000000c3 )
  );
  MUXCY   \blk00000001/blk0000003d  (
    .CI(\blk00000001/sig000000c3 ),
    .DI(a[6]),
    .S(\blk00000001/sig000000a1 ),
    .O(\blk00000001/sig000000c2 )
  );
  MUXCY   \blk00000001/blk0000003c  (
    .CI(\blk00000001/sig000000c2 ),
    .DI(a[7]),
    .S(\blk00000001/sig000000a0 ),
    .O(\blk00000001/sig000000c1 )
  );
  MUXCY   \blk00000001/blk0000003b  (
    .CI(\blk00000001/sig000000c1 ),
    .DI(a[8]),
    .S(\blk00000001/sig0000009f ),
    .O(\blk00000001/sig000000c0 )
  );
  MUXCY   \blk00000001/blk0000003a  (
    .CI(\blk00000001/sig000000c0 ),
    .DI(a[9]),
    .S(\blk00000001/sig0000009e ),
    .O(\blk00000001/sig000000bf )
  );
  MUXCY   \blk00000001/blk00000039  (
    .CI(\blk00000001/sig000000bf ),
    .DI(a[10]),
    .S(\blk00000001/sig0000009d ),
    .O(\blk00000001/sig000000be )
  );
  MUXCY   \blk00000001/blk00000038  (
    .CI(\blk00000001/sig000000be ),
    .DI(a[11]),
    .S(\blk00000001/sig0000009c ),
    .O(\blk00000001/sig000000bd )
  );
  MUXCY   \blk00000001/blk00000037  (
    .CI(\blk00000001/sig000000bd ),
    .DI(a[12]),
    .S(\blk00000001/sig0000009b ),
    .O(\blk00000001/sig000000bc )
  );
  MUXCY   \blk00000001/blk00000036  (
    .CI(\blk00000001/sig000000bc ),
    .DI(a[13]),
    .S(\blk00000001/sig0000009a ),
    .O(\blk00000001/sig000000bb )
  );
  MUXCY   \blk00000001/blk00000035  (
    .CI(\blk00000001/sig000000bb ),
    .DI(a[14]),
    .S(\blk00000001/sig00000099 ),
    .O(\blk00000001/sig000000ba )
  );
  MUXCY   \blk00000001/blk00000034  (
    .CI(\blk00000001/sig000000ba ),
    .DI(a[15]),
    .S(\blk00000001/sig00000098 ),
    .O(\blk00000001/sig000000b9 )
  );
  MUXCY   \blk00000001/blk00000033  (
    .CI(\blk00000001/sig000000b9 ),
    .DI(a[16]),
    .S(\blk00000001/sig00000097 ),
    .O(\blk00000001/sig000000b8 )
  );
  MUXCY   \blk00000001/blk00000032  (
    .CI(\blk00000001/sig000000b8 ),
    .DI(a[17]),
    .S(\blk00000001/sig00000096 ),
    .O(\blk00000001/sig000000b7 )
  );
  MUXCY   \blk00000001/blk00000031  (
    .CI(\blk00000001/sig000000b7 ),
    .DI(a[18]),
    .S(\blk00000001/sig00000095 ),
    .O(\blk00000001/sig000000b6 )
  );
  MUXCY   \blk00000001/blk00000030  (
    .CI(\blk00000001/sig000000b6 ),
    .DI(a[19]),
    .S(\blk00000001/sig00000094 ),
    .O(\blk00000001/sig000000b5 )
  );
  MUXCY   \blk00000001/blk0000002f  (
    .CI(\blk00000001/sig000000b5 ),
    .DI(a[20]),
    .S(\blk00000001/sig00000093 ),
    .O(\blk00000001/sig000000b4 )
  );
  MUXCY   \blk00000001/blk0000002e  (
    .CI(\blk00000001/sig000000b4 ),
    .DI(a[21]),
    .S(\blk00000001/sig00000092 ),
    .O(\blk00000001/sig000000b3 )
  );
  MUXCY   \blk00000001/blk0000002d  (
    .CI(\blk00000001/sig000000b3 ),
    .DI(a[22]),
    .S(\blk00000001/sig00000091 ),
    .O(\blk00000001/sig000000b2 )
  );
  MUXCY   \blk00000001/blk0000002c  (
    .CI(\blk00000001/sig000000b2 ),
    .DI(a[23]),
    .S(\blk00000001/sig00000090 ),
    .O(\blk00000001/sig000000b1 )
  );
  MUXCY   \blk00000001/blk0000002b  (
    .CI(\blk00000001/sig000000b1 ),
    .DI(a[24]),
    .S(\blk00000001/sig0000008f ),
    .O(\blk00000001/sig000000b0 )
  );
  MUXCY   \blk00000001/blk0000002a  (
    .CI(\blk00000001/sig000000b0 ),
    .DI(a[25]),
    .S(\blk00000001/sig0000008e ),
    .O(\blk00000001/sig000000af )
  );
  MUXCY   \blk00000001/blk00000029  (
    .CI(\blk00000001/sig000000af ),
    .DI(a[26]),
    .S(\blk00000001/sig0000008d ),
    .O(\blk00000001/sig000000ae )
  );
  MUXCY   \blk00000001/blk00000028  (
    .CI(\blk00000001/sig000000ae ),
    .DI(a[27]),
    .S(\blk00000001/sig0000008c ),
    .O(\blk00000001/sig000000ad )
  );
  MUXCY   \blk00000001/blk00000027  (
    .CI(\blk00000001/sig000000ad ),
    .DI(a[28]),
    .S(\blk00000001/sig0000008b ),
    .O(\blk00000001/sig000000ac )
  );
  MUXCY   \blk00000001/blk00000026  (
    .CI(\blk00000001/sig000000ac ),
    .DI(a[29]),
    .S(\blk00000001/sig0000008a ),
    .O(\blk00000001/sig000000ab )
  );
  MUXCY   \blk00000001/blk00000025  (
    .CI(\blk00000001/sig000000ab ),
    .DI(a[30]),
    .S(\blk00000001/sig00000089 ),
    .O(\blk00000001/sig000000aa )
  );
  MUXCY   \blk00000001/blk00000024  (
    .CI(\blk00000001/sig000000aa ),
    .DI(a[31]),
    .S(\blk00000001/sig00000088 ),
    .O(\blk00000001/sig000000a9 )
  );
  XORCY   \blk00000001/blk00000023  (
    .CI(\blk00000001/sig000000c8 ),
    .LI(\blk00000001/sig000000a6 ),
    .O(\blk00000001/sig00000087 )
  );
  XORCY   \blk00000001/blk00000022  (
    .CI(\blk00000001/sig000000c7 ),
    .LI(\blk00000001/sig000000a5 ),
    .O(\blk00000001/sig00000086 )
  );
  XORCY   \blk00000001/blk00000021  (
    .CI(\blk00000001/sig000000c6 ),
    .LI(\blk00000001/sig000000a4 ),
    .O(\blk00000001/sig00000085 )
  );
  XORCY   \blk00000001/blk00000020  (
    .CI(\blk00000001/sig000000c5 ),
    .LI(\blk00000001/sig000000a3 ),
    .O(\blk00000001/sig00000084 )
  );
  XORCY   \blk00000001/blk0000001f  (
    .CI(\blk00000001/sig000000c4 ),
    .LI(\blk00000001/sig000000a2 ),
    .O(\blk00000001/sig00000083 )
  );
  XORCY   \blk00000001/blk0000001e  (
    .CI(\blk00000001/sig000000c3 ),
    .LI(\blk00000001/sig000000a1 ),
    .O(\blk00000001/sig00000082 )
  );
  XORCY   \blk00000001/blk0000001d  (
    .CI(\blk00000001/sig000000c2 ),
    .LI(\blk00000001/sig000000a0 ),
    .O(\blk00000001/sig00000081 )
  );
  XORCY   \blk00000001/blk0000001c  (
    .CI(\blk00000001/sig000000c1 ),
    .LI(\blk00000001/sig0000009f ),
    .O(\blk00000001/sig00000080 )
  );
  XORCY   \blk00000001/blk0000001b  (
    .CI(\blk00000001/sig000000c0 ),
    .LI(\blk00000001/sig0000009e ),
    .O(\blk00000001/sig0000007f )
  );
  XORCY   \blk00000001/blk0000001a  (
    .CI(\blk00000001/sig000000bf ),
    .LI(\blk00000001/sig0000009d ),
    .O(\blk00000001/sig0000007e )
  );
  XORCY   \blk00000001/blk00000019  (
    .CI(\blk00000001/sig000000be ),
    .LI(\blk00000001/sig0000009c ),
    .O(\blk00000001/sig0000007d )
  );
  XORCY   \blk00000001/blk00000018  (
    .CI(\blk00000001/sig000000bd ),
    .LI(\blk00000001/sig0000009b ),
    .O(\blk00000001/sig0000007c )
  );
  XORCY   \blk00000001/blk00000017  (
    .CI(\blk00000001/sig000000bc ),
    .LI(\blk00000001/sig0000009a ),
    .O(\blk00000001/sig0000007b )
  );
  XORCY   \blk00000001/blk00000016  (
    .CI(\blk00000001/sig000000bb ),
    .LI(\blk00000001/sig00000099 ),
    .O(\blk00000001/sig0000007a )
  );
  XORCY   \blk00000001/blk00000015  (
    .CI(\blk00000001/sig000000ba ),
    .LI(\blk00000001/sig00000098 ),
    .O(\blk00000001/sig00000079 )
  );
  XORCY   \blk00000001/blk00000014  (
    .CI(\blk00000001/sig000000b9 ),
    .LI(\blk00000001/sig00000097 ),
    .O(\blk00000001/sig00000078 )
  );
  XORCY   \blk00000001/blk00000013  (
    .CI(\blk00000001/sig000000b8 ),
    .LI(\blk00000001/sig00000096 ),
    .O(\blk00000001/sig00000077 )
  );
  XORCY   \blk00000001/blk00000012  (
    .CI(\blk00000001/sig000000b7 ),
    .LI(\blk00000001/sig00000095 ),
    .O(\blk00000001/sig00000076 )
  );
  XORCY   \blk00000001/blk00000011  (
    .CI(\blk00000001/sig000000b6 ),
    .LI(\blk00000001/sig00000094 ),
    .O(\blk00000001/sig00000075 )
  );
  XORCY   \blk00000001/blk00000010  (
    .CI(\blk00000001/sig000000b5 ),
    .LI(\blk00000001/sig00000093 ),
    .O(\blk00000001/sig00000074 )
  );
  XORCY   \blk00000001/blk0000000f  (
    .CI(\blk00000001/sig000000b4 ),
    .LI(\blk00000001/sig00000092 ),
    .O(\blk00000001/sig00000073 )
  );
  XORCY   \blk00000001/blk0000000e  (
    .CI(\blk00000001/sig000000b3 ),
    .LI(\blk00000001/sig00000091 ),
    .O(\blk00000001/sig00000072 )
  );
  XORCY   \blk00000001/blk0000000d  (
    .CI(\blk00000001/sig000000b2 ),
    .LI(\blk00000001/sig00000090 ),
    .O(\blk00000001/sig00000071 )
  );
  XORCY   \blk00000001/blk0000000c  (
    .CI(\blk00000001/sig000000b1 ),
    .LI(\blk00000001/sig0000008f ),
    .O(\blk00000001/sig00000070 )
  );
  XORCY   \blk00000001/blk0000000b  (
    .CI(\blk00000001/sig000000b0 ),
    .LI(\blk00000001/sig0000008e ),
    .O(\blk00000001/sig0000006f )
  );
  XORCY   \blk00000001/blk0000000a  (
    .CI(\blk00000001/sig000000af ),
    .LI(\blk00000001/sig0000008d ),
    .O(\blk00000001/sig0000006e )
  );
  XORCY   \blk00000001/blk00000009  (
    .CI(\blk00000001/sig000000ae ),
    .LI(\blk00000001/sig0000008c ),
    .O(\blk00000001/sig0000006d )
  );
  XORCY   \blk00000001/blk00000008  (
    .CI(\blk00000001/sig000000ad ),
    .LI(\blk00000001/sig0000008b ),
    .O(\blk00000001/sig0000006c )
  );
  XORCY   \blk00000001/blk00000007  (
    .CI(\blk00000001/sig000000ac ),
    .LI(\blk00000001/sig0000008a ),
    .O(\blk00000001/sig0000006b )
  );
  XORCY   \blk00000001/blk00000006  (
    .CI(\blk00000001/sig000000ab ),
    .LI(\blk00000001/sig00000089 ),
    .O(\blk00000001/sig0000006a )
  );
  XORCY   \blk00000001/blk00000005  (
    .CI(\blk00000001/sig000000aa ),
    .LI(\blk00000001/sig00000088 ),
    .O(\blk00000001/sig00000069 )
  );
  XORCY   \blk00000001/blk00000004  (
    .CI(\blk00000001/sig000000a9 ),
    .LI(\blk00000001/sig000000a7 ),
    .O(\blk00000001/sig00000068 )
  );
  XORCY   \blk00000001/blk00000003  (
    .CI(\blk00000001/sig00000066 ),
    .LI(\blk00000001/sig000000a8 ),
    .O(\blk00000001/sig00000067 )
  );
  GND   \blk00000001/blk00000002  (
    .G(\blk00000001/sig00000066 )
  );

// synthesis translate_on

endmodule

// synthesis translate_off

`ifndef GLBL
`define GLBL

`timescale  1 ps / 1 ps

module glbl ();

    parameter ROC_WIDTH = 100000;
    parameter TOC_WIDTH = 0;

//--------   STARTUP Globals --------------
    wire GSR;
    wire GTS;
    wire GWE;
    wire PRLD;
    tri1 p_up_tmp;
    tri (weak1, strong0) PLL_LOCKG = p_up_tmp;

    wire PROGB_GLBL;
    wire CCLKO_GLBL;

    reg GSR_int;
    reg GTS_int;
    reg PRLD_int;

//--------   JTAG Globals --------------
    wire JTAG_TDO_GLBL;
    wire JTAG_TCK_GLBL;
    wire JTAG_TDI_GLBL;
    wire JTAG_TMS_GLBL;
    wire JTAG_TRST_GLBL;

    reg JTAG_CAPTURE_GLBL;
    reg JTAG_RESET_GLBL;
    reg JTAG_SHIFT_GLBL;
    reg JTAG_UPDATE_GLBL;
    reg JTAG_RUNTEST_GLBL;

    reg JTAG_SEL1_GLBL = 0;
    reg JTAG_SEL2_GLBL = 0 ;
    reg JTAG_SEL3_GLBL = 0;
    reg JTAG_SEL4_GLBL = 0;

    reg JTAG_USER_TDO1_GLBL = 1'bz;
    reg JTAG_USER_TDO2_GLBL = 1'bz;
    reg JTAG_USER_TDO3_GLBL = 1'bz;
    reg JTAG_USER_TDO4_GLBL = 1'bz;

    assign (weak1, weak0) GSR = GSR_int;
    assign (weak1, weak0) GTS = GTS_int;
    assign (weak1, weak0) PRLD = PRLD_int;

    initial begin
	GSR_int = 1'b1;
	PRLD_int = 1'b1;
	#(ROC_WIDTH)
	GSR_int = 1'b0;
	PRLD_int = 1'b0;
    end

    initial begin
	GTS_int = 1'b1;
	#(TOC_WIDTH)
	GTS_int = 1'b0;
    end

endmodule

`endif

// synthesis translate_on
////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: P.68d
//  \   \         Application: netgen
//  /   /         Filename: addsb_11_0_c5cb2be34e3bf19d.v
// /___/   /\     Timestamp: Thu Dec 05 08:07:27 2013
// \   \  /  \ 
//  \___\/\___\
//             
// Command	: -w -sim -ofmt verilog C:/Users/kha/AppData/Local/Temp/1/sysgentmp-kha/cg_wk/c1cfdeebce5d23323/tmp/_cg/addsb_11_0_c5cb2be34e3bf19d.ngc C:/Users/kha/AppData/Local/Temp/1/sysgentmp-kha/cg_wk/c1cfdeebce5d23323/tmp/_cg/addsb_11_0_c5cb2be34e3bf19d.v 
// Device	: 6vlx240tff1156-1
// Input file	: C:/Users/kha/AppData/Local/Temp/1/sysgentmp-kha/cg_wk/c1cfdeebce5d23323/tmp/_cg/addsb_11_0_c5cb2be34e3bf19d.ngc
// Output file	: C:/Users/kha/AppData/Local/Temp/1/sysgentmp-kha/cg_wk/c1cfdeebce5d23323/tmp/_cg/addsb_11_0_c5cb2be34e3bf19d.v
// # of Modules	: 1
// Design Name	: addsb_11_0_c5cb2be34e3bf19d
// Xilinx        : c:\xilinx\14.6\ise_ds\ise\
//             
// Purpose:    
//     This verilog netlist is a verification model and uses simulation 
//     primitives which may not represent the true implementation of the 
//     device, however the netlist is functionally correct and should not 
//     be modified. This file cannot be synthesized and should only be used 
//     with supported simulation tools.
//             
// Reference:  
//     Command Line Tools User Guide, Chapter 23 and Synthesis and Simulation Design Guide, Chapter 6
//             
////////////////////////////////////////////////////////////////////////////////

`timescale 1 ns/1 ps

module addsb_11_0_c5cb2be34e3bf19d (
a, b, s
)/* synthesis syn_black_box syn_noprune=1 */;
  input [32 : 0] a;
  input [32 : 0] b;
  output [32 : 0] s;
  
  // synthesis translate_off
  
  wire \blk00000001/sig000000a5 ;
  wire \blk00000001/sig000000a4 ;
  wire \blk00000001/sig000000a3 ;
  wire \blk00000001/sig000000a2 ;
  wire \blk00000001/sig000000a1 ;
  wire \blk00000001/sig000000a0 ;
  wire \blk00000001/sig0000009f ;
  wire \blk00000001/sig0000009e ;
  wire \blk00000001/sig0000009d ;
  wire \blk00000001/sig0000009c ;
  wire \blk00000001/sig0000009b ;
  wire \blk00000001/sig0000009a ;
  wire \blk00000001/sig00000099 ;
  wire \blk00000001/sig00000098 ;
  wire \blk00000001/sig00000097 ;
  wire \blk00000001/sig00000096 ;
  wire \blk00000001/sig00000095 ;
  wire \blk00000001/sig00000094 ;
  wire \blk00000001/sig00000093 ;
  wire \blk00000001/sig00000092 ;
  wire \blk00000001/sig00000091 ;
  wire \blk00000001/sig00000090 ;
  wire \blk00000001/sig0000008f ;
  wire \blk00000001/sig0000008e ;
  wire \blk00000001/sig0000008d ;
  wire \blk00000001/sig0000008c ;
  wire \blk00000001/sig0000008b ;
  wire \blk00000001/sig0000008a ;
  wire \blk00000001/sig00000089 ;
  wire \blk00000001/sig00000088 ;
  wire \blk00000001/sig00000087 ;
  wire \blk00000001/sig00000086 ;
  wire \blk00000001/sig00000085 ;
  wire \blk00000001/sig00000084 ;
  wire \blk00000001/sig00000083 ;
  wire \blk00000001/sig00000082 ;
  wire \blk00000001/sig00000081 ;
  wire \blk00000001/sig00000080 ;
  wire \blk00000001/sig0000007f ;
  wire \blk00000001/sig0000007e ;
  wire \blk00000001/sig0000007d ;
  wire \blk00000001/sig0000007c ;
  wire \blk00000001/sig0000007b ;
  wire \blk00000001/sig0000007a ;
  wire \blk00000001/sig00000079 ;
  wire \blk00000001/sig00000078 ;
  wire \blk00000001/sig00000077 ;
  wire \blk00000001/sig00000076 ;
  wire \blk00000001/sig00000075 ;
  wire \blk00000001/sig00000074 ;
  wire \blk00000001/sig00000073 ;
  wire \blk00000001/sig00000072 ;
  wire \blk00000001/sig00000071 ;
  wire \blk00000001/sig00000070 ;
  wire \blk00000001/sig0000006f ;
  wire \blk00000001/sig0000006e ;
  wire \blk00000001/sig0000006d ;
  wire \blk00000001/sig0000006c ;
  wire \blk00000001/sig0000006b ;
  wire \blk00000001/sig0000006a ;
  wire \blk00000001/sig00000069 ;
  wire \blk00000001/sig00000068 ;
  wire \blk00000001/sig00000067 ;
  wire \blk00000001/sig00000066 ;
  wire \blk00000001/sig00000065 ;
  wire \blk00000001/sig00000064 ;
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000064  (
    .I0(a[32]),
    .I1(b[32]),
    .O(\blk00000001/sig00000084 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000063  (
    .I0(a[31]),
    .I1(b[31]),
    .O(\blk00000001/sig00000065 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000062  (
    .I0(a[30]),
    .I1(b[30]),
    .O(\blk00000001/sig00000066 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000061  (
    .I0(a[29]),
    .I1(b[29]),
    .O(\blk00000001/sig00000067 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000060  (
    .I0(a[28]),
    .I1(b[28]),
    .O(\blk00000001/sig00000068 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk0000005f  (
    .I0(a[27]),
    .I1(b[27]),
    .O(\blk00000001/sig00000069 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk0000005e  (
    .I0(a[26]),
    .I1(b[26]),
    .O(\blk00000001/sig0000006a )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk0000005d  (
    .I0(a[25]),
    .I1(b[25]),
    .O(\blk00000001/sig0000006b )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk0000005c  (
    .I0(a[24]),
    .I1(b[24]),
    .O(\blk00000001/sig0000006c )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk0000005b  (
    .I0(a[23]),
    .I1(b[23]),
    .O(\blk00000001/sig0000006d )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk0000005a  (
    .I0(a[22]),
    .I1(b[22]),
    .O(\blk00000001/sig0000006e )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000059  (
    .I0(a[21]),
    .I1(b[21]),
    .O(\blk00000001/sig0000006f )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000058  (
    .I0(a[20]),
    .I1(b[20]),
    .O(\blk00000001/sig00000070 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000057  (
    .I0(a[19]),
    .I1(b[19]),
    .O(\blk00000001/sig00000071 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000056  (
    .I0(a[18]),
    .I1(b[18]),
    .O(\blk00000001/sig00000072 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000055  (
    .I0(a[17]),
    .I1(b[17]),
    .O(\blk00000001/sig00000073 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000054  (
    .I0(a[16]),
    .I1(b[16]),
    .O(\blk00000001/sig00000074 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000053  (
    .I0(a[15]),
    .I1(b[15]),
    .O(\blk00000001/sig00000075 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000052  (
    .I0(a[14]),
    .I1(b[14]),
    .O(\blk00000001/sig00000076 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000051  (
    .I0(a[13]),
    .I1(b[13]),
    .O(\blk00000001/sig00000077 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000050  (
    .I0(a[12]),
    .I1(b[12]),
    .O(\blk00000001/sig00000078 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk0000004f  (
    .I0(a[11]),
    .I1(b[11]),
    .O(\blk00000001/sig00000079 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk0000004e  (
    .I0(a[10]),
    .I1(b[10]),
    .O(\blk00000001/sig0000007a )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk0000004d  (
    .I0(a[9]),
    .I1(b[9]),
    .O(\blk00000001/sig0000007b )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk0000004c  (
    .I0(a[8]),
    .I1(b[8]),
    .O(\blk00000001/sig0000007c )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk0000004b  (
    .I0(a[7]),
    .I1(b[7]),
    .O(\blk00000001/sig0000007d )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk0000004a  (
    .I0(a[6]),
    .I1(b[6]),
    .O(\blk00000001/sig0000007e )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000049  (
    .I0(a[5]),
    .I1(b[5]),
    .O(\blk00000001/sig0000007f )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000048  (
    .I0(a[4]),
    .I1(b[4]),
    .O(\blk00000001/sig00000080 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000047  (
    .I0(a[3]),
    .I1(b[3]),
    .O(\blk00000001/sig00000081 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000046  (
    .I0(a[2]),
    .I1(b[2]),
    .O(\blk00000001/sig00000082 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000045  (
    .I0(a[1]),
    .I1(b[1]),
    .O(\blk00000001/sig00000083 )
  );
  LUT2 #(
    .INIT ( 4'h9 ))
  \blk00000001/blk00000044  (
    .I0(a[0]),
    .I1(b[0]),
    .O(\blk00000001/sig00000085 )
  );
  MUXCY   \blk00000001/blk00000043  (
    .CI(\blk00000001/sig00000064 ),
    .DI(a[0]),
    .S(\blk00000001/sig00000085 ),
    .O(\blk00000001/sig000000a5 )
  );
  MUXCY   \blk00000001/blk00000042  (
    .CI(\blk00000001/sig000000a5 ),
    .DI(a[1]),
    .S(\blk00000001/sig00000083 ),
    .O(\blk00000001/sig000000a4 )
  );
  MUXCY   \blk00000001/blk00000041  (
    .CI(\blk00000001/sig000000a4 ),
    .DI(a[2]),
    .S(\blk00000001/sig00000082 ),
    .O(\blk00000001/sig000000a3 )
  );
  MUXCY   \blk00000001/blk00000040  (
    .CI(\blk00000001/sig000000a3 ),
    .DI(a[3]),
    .S(\blk00000001/sig00000081 ),
    .O(\blk00000001/sig000000a2 )
  );
  MUXCY   \blk00000001/blk0000003f  (
    .CI(\blk00000001/sig000000a2 ),
    .DI(a[4]),
    .S(\blk00000001/sig00000080 ),
    .O(\blk00000001/sig000000a1 )
  );
  MUXCY   \blk00000001/blk0000003e  (
    .CI(\blk00000001/sig000000a1 ),
    .DI(a[5]),
    .S(\blk00000001/sig0000007f ),
    .O(\blk00000001/sig000000a0 )
  );
  MUXCY   \blk00000001/blk0000003d  (
    .CI(\blk00000001/sig000000a0 ),
    .DI(a[6]),
    .S(\blk00000001/sig0000007e ),
    .O(\blk00000001/sig0000009f )
  );
  MUXCY   \blk00000001/blk0000003c  (
    .CI(\blk00000001/sig0000009f ),
    .DI(a[7]),
    .S(\blk00000001/sig0000007d ),
    .O(\blk00000001/sig0000009e )
  );
  MUXCY   \blk00000001/blk0000003b  (
    .CI(\blk00000001/sig0000009e ),
    .DI(a[8]),
    .S(\blk00000001/sig0000007c ),
    .O(\blk00000001/sig0000009d )
  );
  MUXCY   \blk00000001/blk0000003a  (
    .CI(\blk00000001/sig0000009d ),
    .DI(a[9]),
    .S(\blk00000001/sig0000007b ),
    .O(\blk00000001/sig0000009c )
  );
  MUXCY   \blk00000001/blk00000039  (
    .CI(\blk00000001/sig0000009c ),
    .DI(a[10]),
    .S(\blk00000001/sig0000007a ),
    .O(\blk00000001/sig0000009b )
  );
  MUXCY   \blk00000001/blk00000038  (
    .CI(\blk00000001/sig0000009b ),
    .DI(a[11]),
    .S(\blk00000001/sig00000079 ),
    .O(\blk00000001/sig0000009a )
  );
  MUXCY   \blk00000001/blk00000037  (
    .CI(\blk00000001/sig0000009a ),
    .DI(a[12]),
    .S(\blk00000001/sig00000078 ),
    .O(\blk00000001/sig00000099 )
  );
  MUXCY   \blk00000001/blk00000036  (
    .CI(\blk00000001/sig00000099 ),
    .DI(a[13]),
    .S(\blk00000001/sig00000077 ),
    .O(\blk00000001/sig00000098 )
  );
  MUXCY   \blk00000001/blk00000035  (
    .CI(\blk00000001/sig00000098 ),
    .DI(a[14]),
    .S(\blk00000001/sig00000076 ),
    .O(\blk00000001/sig00000097 )
  );
  MUXCY   \blk00000001/blk00000034  (
    .CI(\blk00000001/sig00000097 ),
    .DI(a[15]),
    .S(\blk00000001/sig00000075 ),
    .O(\blk00000001/sig00000096 )
  );
  MUXCY   \blk00000001/blk00000033  (
    .CI(\blk00000001/sig00000096 ),
    .DI(a[16]),
    .S(\blk00000001/sig00000074 ),
    .O(\blk00000001/sig00000095 )
  );
  MUXCY   \blk00000001/blk00000032  (
    .CI(\blk00000001/sig00000095 ),
    .DI(a[17]),
    .S(\blk00000001/sig00000073 ),
    .O(\blk00000001/sig00000094 )
  );
  MUXCY   \blk00000001/blk00000031  (
    .CI(\blk00000001/sig00000094 ),
    .DI(a[18]),
    .S(\blk00000001/sig00000072 ),
    .O(\blk00000001/sig00000093 )
  );
  MUXCY   \blk00000001/blk00000030  (
    .CI(\blk00000001/sig00000093 ),
    .DI(a[19]),
    .S(\blk00000001/sig00000071 ),
    .O(\blk00000001/sig00000092 )
  );
  MUXCY   \blk00000001/blk0000002f  (
    .CI(\blk00000001/sig00000092 ),
    .DI(a[20]),
    .S(\blk00000001/sig00000070 ),
    .O(\blk00000001/sig00000091 )
  );
  MUXCY   \blk00000001/blk0000002e  (
    .CI(\blk00000001/sig00000091 ),
    .DI(a[21]),
    .S(\blk00000001/sig0000006f ),
    .O(\blk00000001/sig00000090 )
  );
  MUXCY   \blk00000001/blk0000002d  (
    .CI(\blk00000001/sig00000090 ),
    .DI(a[22]),
    .S(\blk00000001/sig0000006e ),
    .O(\blk00000001/sig0000008f )
  );
  MUXCY   \blk00000001/blk0000002c  (
    .CI(\blk00000001/sig0000008f ),
    .DI(a[23]),
    .S(\blk00000001/sig0000006d ),
    .O(\blk00000001/sig0000008e )
  );
  MUXCY   \blk00000001/blk0000002b  (
    .CI(\blk00000001/sig0000008e ),
    .DI(a[24]),
    .S(\blk00000001/sig0000006c ),
    .O(\blk00000001/sig0000008d )
  );
  MUXCY   \blk00000001/blk0000002a  (
    .CI(\blk00000001/sig0000008d ),
    .DI(a[25]),
    .S(\blk00000001/sig0000006b ),
    .O(\blk00000001/sig0000008c )
  );
  MUXCY   \blk00000001/blk00000029  (
    .CI(\blk00000001/sig0000008c ),
    .DI(a[26]),
    .S(\blk00000001/sig0000006a ),
    .O(\blk00000001/sig0000008b )
  );
  MUXCY   \blk00000001/blk00000028  (
    .CI(\blk00000001/sig0000008b ),
    .DI(a[27]),
    .S(\blk00000001/sig00000069 ),
    .O(\blk00000001/sig0000008a )
  );
  MUXCY   \blk00000001/blk00000027  (
    .CI(\blk00000001/sig0000008a ),
    .DI(a[28]),
    .S(\blk00000001/sig00000068 ),
    .O(\blk00000001/sig00000089 )
  );
  MUXCY   \blk00000001/blk00000026  (
    .CI(\blk00000001/sig00000089 ),
    .DI(a[29]),
    .S(\blk00000001/sig00000067 ),
    .O(\blk00000001/sig00000088 )
  );
  MUXCY   \blk00000001/blk00000025  (
    .CI(\blk00000001/sig00000088 ),
    .DI(a[30]),
    .S(\blk00000001/sig00000066 ),
    .O(\blk00000001/sig00000087 )
  );
  MUXCY   \blk00000001/blk00000024  (
    .CI(\blk00000001/sig00000087 ),
    .DI(a[31]),
    .S(\blk00000001/sig00000065 ),
    .O(\blk00000001/sig00000086 )
  );
  XORCY   \blk00000001/blk00000023  (
    .CI(\blk00000001/sig000000a5 ),
    .LI(\blk00000001/sig00000083 ),
    .O(s[1])
  );
  XORCY   \blk00000001/blk00000022  (
    .CI(\blk00000001/sig000000a4 ),
    .LI(\blk00000001/sig00000082 ),
    .O(s[2])
  );
  XORCY   \blk00000001/blk00000021  (
    .CI(\blk00000001/sig000000a3 ),
    .LI(\blk00000001/sig00000081 ),
    .O(s[3])
  );
  XORCY   \blk00000001/blk00000020  (
    .CI(\blk00000001/sig000000a2 ),
    .LI(\blk00000001/sig00000080 ),
    .O(s[4])
  );
  XORCY   \blk00000001/blk0000001f  (
    .CI(\blk00000001/sig000000a1 ),
    .LI(\blk00000001/sig0000007f ),
    .O(s[5])
  );
  XORCY   \blk00000001/blk0000001e  (
    .CI(\blk00000001/sig000000a0 ),
    .LI(\blk00000001/sig0000007e ),
    .O(s[6])
  );
  XORCY   \blk00000001/blk0000001d  (
    .CI(\blk00000001/sig0000009f ),
    .LI(\blk00000001/sig0000007d ),
    .O(s[7])
  );
  XORCY   \blk00000001/blk0000001c  (
    .CI(\blk00000001/sig0000009e ),
    .LI(\blk00000001/sig0000007c ),
    .O(s[8])
  );
  XORCY   \blk00000001/blk0000001b  (
    .CI(\blk00000001/sig0000009d ),
    .LI(\blk00000001/sig0000007b ),
    .O(s[9])
  );
  XORCY   \blk00000001/blk0000001a  (
    .CI(\blk00000001/sig0000009c ),
    .LI(\blk00000001/sig0000007a ),
    .O(s[10])
  );
  XORCY   \blk00000001/blk00000019  (
    .CI(\blk00000001/sig0000009b ),
    .LI(\blk00000001/sig00000079 ),
    .O(s[11])
  );
  XORCY   \blk00000001/blk00000018  (
    .CI(\blk00000001/sig0000009a ),
    .LI(\blk00000001/sig00000078 ),
    .O(s[12])
  );
  XORCY   \blk00000001/blk00000017  (
    .CI(\blk00000001/sig00000099 ),
    .LI(\blk00000001/sig00000077 ),
    .O(s[13])
  );
  XORCY   \blk00000001/blk00000016  (
    .CI(\blk00000001/sig00000098 ),
    .LI(\blk00000001/sig00000076 ),
    .O(s[14])
  );
  XORCY   \blk00000001/blk00000015  (
    .CI(\blk00000001/sig00000097 ),
    .LI(\blk00000001/sig00000075 ),
    .O(s[15])
  );
  XORCY   \blk00000001/blk00000014  (
    .CI(\blk00000001/sig00000096 ),
    .LI(\blk00000001/sig00000074 ),
    .O(s[16])
  );
  XORCY   \blk00000001/blk00000013  (
    .CI(\blk00000001/sig00000095 ),
    .LI(\blk00000001/sig00000073 ),
    .O(s[17])
  );
  XORCY   \blk00000001/blk00000012  (
    .CI(\blk00000001/sig00000094 ),
    .LI(\blk00000001/sig00000072 ),
    .O(s[18])
  );
  XORCY   \blk00000001/blk00000011  (
    .CI(\blk00000001/sig00000093 ),
    .LI(\blk00000001/sig00000071 ),
    .O(s[19])
  );
  XORCY   \blk00000001/blk00000010  (
    .CI(\blk00000001/sig00000092 ),
    .LI(\blk00000001/sig00000070 ),
    .O(s[20])
  );
  XORCY   \blk00000001/blk0000000f  (
    .CI(\blk00000001/sig00000091 ),
    .LI(\blk00000001/sig0000006f ),
    .O(s[21])
  );
  XORCY   \blk00000001/blk0000000e  (
    .CI(\blk00000001/sig00000090 ),
    .LI(\blk00000001/sig0000006e ),
    .O(s[22])
  );
  XORCY   \blk00000001/blk0000000d  (
    .CI(\blk00000001/sig0000008f ),
    .LI(\blk00000001/sig0000006d ),
    .O(s[23])
  );
  XORCY   \blk00000001/blk0000000c  (
    .CI(\blk00000001/sig0000008e ),
    .LI(\blk00000001/sig0000006c ),
    .O(s[24])
  );
  XORCY   \blk00000001/blk0000000b  (
    .CI(\blk00000001/sig0000008d ),
    .LI(\blk00000001/sig0000006b ),
    .O(s[25])
  );
  XORCY   \blk00000001/blk0000000a  (
    .CI(\blk00000001/sig0000008c ),
    .LI(\blk00000001/sig0000006a ),
    .O(s[26])
  );
  XORCY   \blk00000001/blk00000009  (
    .CI(\blk00000001/sig0000008b ),
    .LI(\blk00000001/sig00000069 ),
    .O(s[27])
  );
  XORCY   \blk00000001/blk00000008  (
    .CI(\blk00000001/sig0000008a ),
    .LI(\blk00000001/sig00000068 ),
    .O(s[28])
  );
  XORCY   \blk00000001/blk00000007  (
    .CI(\blk00000001/sig00000089 ),
    .LI(\blk00000001/sig00000067 ),
    .O(s[29])
  );
  XORCY   \blk00000001/blk00000006  (
    .CI(\blk00000001/sig00000088 ),
    .LI(\blk00000001/sig00000066 ),
    .O(s[30])
  );
  XORCY   \blk00000001/blk00000005  (
    .CI(\blk00000001/sig00000087 ),
    .LI(\blk00000001/sig00000065 ),
    .O(s[31])
  );
  XORCY   \blk00000001/blk00000004  (
    .CI(\blk00000001/sig00000086 ),
    .LI(\blk00000001/sig00000084 ),
    .O(s[32])
  );
  XORCY   \blk00000001/blk00000003  (
    .CI(\blk00000001/sig00000064 ),
    .LI(\blk00000001/sig00000085 ),
    .O(s[0])
  );
  VCC   \blk00000001/blk00000002  (
    .P(\blk00000001/sig00000064 )
  );

// synthesis translate_on

endmodule

// synthesis translate_off

`ifndef GLBL
`define GLBL

`timescale  1 ps / 1 ps

module glbl ();

    parameter ROC_WIDTH = 100000;
    parameter TOC_WIDTH = 0;

//--------   STARTUP Globals --------------
    wire GSR;
    wire GTS;
    wire GWE;
    wire PRLD;
    tri1 p_up_tmp;
    tri (weak1, strong0) PLL_LOCKG = p_up_tmp;

    wire PROGB_GLBL;
    wire CCLKO_GLBL;

    reg GSR_int;
    reg GTS_int;
    reg PRLD_int;

//--------   JTAG Globals --------------
    wire JTAG_TDO_GLBL;
    wire JTAG_TCK_GLBL;
    wire JTAG_TDI_GLBL;
    wire JTAG_TMS_GLBL;
    wire JTAG_TRST_GLBL;

    reg JTAG_CAPTURE_GLBL;
    reg JTAG_RESET_GLBL;
    reg JTAG_SHIFT_GLBL;
    reg JTAG_UPDATE_GLBL;
    reg JTAG_RUNTEST_GLBL;

    reg JTAG_SEL1_GLBL = 0;
    reg JTAG_SEL2_GLBL = 0 ;
    reg JTAG_SEL3_GLBL = 0;
    reg JTAG_SEL4_GLBL = 0;

    reg JTAG_USER_TDO1_GLBL = 1'bz;
    reg JTAG_USER_TDO2_GLBL = 1'bz;
    reg JTAG_USER_TDO3_GLBL = 1'bz;
    reg JTAG_USER_TDO4_GLBL = 1'bz;

    assign (weak1, weak0) GSR = GSR_int;
    assign (weak1, weak0) GTS = GTS_int;
    assign (weak1, weak0) PRLD = PRLD_int;

    initial begin
	GSR_int = 1'b1;
	PRLD_int = 1'b1;
	#(ROC_WIDTH)
	GSR_int = 1'b0;
	PRLD_int = 1'b0;
    end

    initial begin
	GTS_int = 1'b1;
	#(TOC_WIDTH)
	GTS_int = 1'b0;
    end

endmodule

`endif

// synthesis translate_on
////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: P.68d
//  \   \         Application: netgen
//  /   /         Filename: mult_11_2_5808000c9562bf6b.v
// /___/   /\     Timestamp: Thu Dec 05 08:08:08 2013
// \   \  /  \ 
//  \___\/\___\
//             
// Command	: -w -sim -ofmt verilog C:/Users/kha/AppData/Local/Temp/1/sysgentmp-kha/cg_wk/c1cfdeebce5d23323/tmp/_cg/mult_11_2_5808000c9562bf6b.ngc C:/Users/kha/AppData/Local/Temp/1/sysgentmp-kha/cg_wk/c1cfdeebce5d23323/tmp/_cg/mult_11_2_5808000c9562bf6b.v 
// Device	: 6vlx240tff1156-1
// Input file	: C:/Users/kha/AppData/Local/Temp/1/sysgentmp-kha/cg_wk/c1cfdeebce5d23323/tmp/_cg/mult_11_2_5808000c9562bf6b.ngc
// Output file	: C:/Users/kha/AppData/Local/Temp/1/sysgentmp-kha/cg_wk/c1cfdeebce5d23323/tmp/_cg/mult_11_2_5808000c9562bf6b.v
// # of Modules	: 1
// Design Name	: mult_11_2_5808000c9562bf6b
// Xilinx        : c:\xilinx\14.6\ise_ds\ise\
//             
// Purpose:    
//     This verilog netlist is a verification model and uses simulation 
//     primitives which may not represent the true implementation of the 
//     device, however the netlist is functionally correct and should not 
//     be modified. This file cannot be synthesized and should only be used 
//     with supported simulation tools.
//             
// Reference:  
//     Command Line Tools User Guide, Chapter 23 and Synthesis and Simulation Design Guide, Chapter 6
//             
////////////////////////////////////////////////////////////////////////////////

`timescale 1 ns/1 ps

module mult_11_2_5808000c9562bf6b (
  clk, ce, sclr, a, b, p
)/* synthesis syn_black_box syn_noprune=1 */;
  input clk;
  input ce;
  input sclr;
  input [32 : 0] a;
  input [31 : 0] b;
  output [64 : 0] p;
  
  // synthesis translate_off
  
  wire \blk00000001/sig000001a6 ;
  wire \blk00000001/sig000001a5 ;
  wire \blk00000001/sig000001a4 ;
  wire \blk00000001/sig000001a3 ;
  wire \blk00000001/sig000001a2 ;
  wire \blk00000001/sig000001a1 ;
  wire \blk00000001/sig000001a0 ;
  wire \blk00000001/sig0000019f ;
  wire \blk00000001/sig0000019e ;
  wire \blk00000001/sig0000019d ;
  wire \blk00000001/sig0000019c ;
  wire \blk00000001/sig0000019b ;
  wire \blk00000001/sig0000019a ;
  wire \blk00000001/sig00000199 ;
  wire \blk00000001/sig00000198 ;
  wire \blk00000001/sig00000197 ;
  wire \blk00000001/sig00000196 ;
  wire \blk00000001/sig00000195 ;
  wire \blk00000001/sig00000194 ;
  wire \blk00000001/sig00000193 ;
  wire \blk00000001/sig00000192 ;
  wire \blk00000001/sig00000191 ;
  wire \blk00000001/sig00000190 ;
  wire \blk00000001/sig0000018f ;
  wire \blk00000001/sig0000018e ;
  wire \blk00000001/sig0000018d ;
  wire \blk00000001/sig0000018c ;
  wire \blk00000001/sig0000018b ;
  wire \blk00000001/sig0000018a ;
  wire \blk00000001/sig00000189 ;
  wire \blk00000001/sig00000188 ;
  wire \blk00000001/sig00000187 ;
  wire \blk00000001/sig00000186 ;
  wire \blk00000001/sig00000185 ;
  wire \blk00000001/sig00000184 ;
  wire \blk00000001/sig00000183 ;
  wire \blk00000001/sig00000182 ;
  wire \blk00000001/sig00000181 ;
  wire \blk00000001/sig00000180 ;
  wire \blk00000001/sig0000017f ;
  wire \blk00000001/sig0000017e ;
  wire \blk00000001/sig0000017d ;
  wire \blk00000001/sig0000017c ;
  wire \blk00000001/sig0000017b ;
  wire \blk00000001/sig0000017a ;
  wire \blk00000001/sig00000179 ;
  wire \blk00000001/sig00000178 ;
  wire \blk00000001/sig00000177 ;
  wire \blk00000001/sig00000176 ;
  wire \blk00000001/sig00000175 ;
  wire \blk00000001/sig00000174 ;
  wire \blk00000001/sig00000173 ;
  wire \blk00000001/sig00000172 ;
  wire \blk00000001/sig00000171 ;
  wire \blk00000001/sig00000170 ;
  wire \blk00000001/sig0000016f ;
  wire \blk00000001/sig0000016e ;
  wire \blk00000001/sig0000016d ;
  wire \blk00000001/sig0000016c ;
  wire \blk00000001/sig0000016b ;
  wire \blk00000001/sig0000016a ;
  wire \blk00000001/sig00000169 ;
  wire \blk00000001/sig00000168 ;
  wire \blk00000001/sig00000167 ;
  wire \blk00000001/sig00000166 ;
  wire \blk00000001/sig00000165 ;
  wire \blk00000001/sig00000164 ;
  wire \blk00000001/sig00000163 ;
  wire \blk00000001/sig00000162 ;
  wire \blk00000001/sig00000161 ;
  wire \blk00000001/sig00000160 ;
  wire \blk00000001/sig0000015f ;
  wire \blk00000001/sig0000015e ;
  wire \blk00000001/sig0000015d ;
  wire \blk00000001/sig0000015c ;
  wire \blk00000001/sig0000015b ;
  wire \blk00000001/sig0000015a ;
  wire \blk00000001/sig00000159 ;
  wire \blk00000001/sig00000158 ;
  wire \blk00000001/sig00000157 ;
  wire \blk00000001/sig00000156 ;
  wire \blk00000001/sig00000155 ;
  wire \blk00000001/sig00000154 ;
  wire \blk00000001/sig00000153 ;
  wire \blk00000001/sig00000152 ;
  wire \blk00000001/sig00000151 ;
  wire \blk00000001/sig00000150 ;
  wire \blk00000001/sig0000014f ;
  wire \blk00000001/sig0000014e ;
  wire \blk00000001/sig0000014d ;
  wire \blk00000001/sig0000014c ;
  wire \blk00000001/sig0000014b ;
  wire \blk00000001/sig0000014a ;
  wire \blk00000001/sig00000149 ;
  wire \blk00000001/sig00000148 ;
  wire \blk00000001/sig00000147 ;
  wire \blk00000001/sig00000146 ;
  wire \blk00000001/sig00000145 ;
  wire \blk00000001/sig00000144 ;
  wire \blk00000001/sig00000143 ;
  wire \blk00000001/sig00000142 ;
  wire \blk00000001/sig00000141 ;
  wire \blk00000001/sig00000140 ;
  wire \blk00000001/sig0000013f ;
  wire \blk00000001/sig0000013e ;
  wire \blk00000001/sig0000013d ;
  wire \blk00000001/sig0000013c ;
  wire \blk00000001/sig0000013b ;
  wire \blk00000001/sig0000013a ;
  wire \blk00000001/sig00000139 ;
  wire \blk00000001/sig00000138 ;
  wire \blk00000001/sig00000137 ;
  wire \blk00000001/sig00000136 ;
  wire \blk00000001/sig00000135 ;
  wire \blk00000001/sig00000134 ;
  wire \blk00000001/sig00000133 ;
  wire \blk00000001/sig00000132 ;
  wire \blk00000001/sig00000131 ;
  wire \blk00000001/sig00000130 ;
  wire \blk00000001/sig0000012f ;
  wire \blk00000001/sig0000012e ;
  wire \blk00000001/sig0000012d ;
  wire \blk00000001/sig0000012c ;
  wire \blk00000001/sig0000012b ;
  wire \blk00000001/sig0000012a ;
  wire \blk00000001/sig00000129 ;
  wire \blk00000001/sig00000128 ;
  wire \blk00000001/sig00000127 ;
  wire \blk00000001/sig00000126 ;
  wire \blk00000001/sig00000125 ;
  wire \blk00000001/sig00000124 ;
  wire \blk00000001/sig00000123 ;
  wire \blk00000001/sig00000122 ;
  wire \blk00000001/sig00000121 ;
  wire \blk00000001/sig00000120 ;
  wire \blk00000001/sig0000011f ;
  wire \blk00000001/sig0000011e ;
  wire \blk00000001/sig0000011d ;
  wire \blk00000001/sig0000011c ;
  wire \blk00000001/sig0000011b ;
  wire \blk00000001/sig0000011a ;
  wire \blk00000001/sig00000119 ;
  wire \blk00000001/sig00000118 ;
  wire \blk00000001/sig00000117 ;
  wire \blk00000001/sig00000116 ;
  wire \blk00000001/sig00000115 ;
  wire \blk00000001/sig00000114 ;
  wire \blk00000001/sig00000113 ;
  wire \blk00000001/sig00000112 ;
  wire \blk00000001/sig00000111 ;
  wire \blk00000001/sig00000110 ;
  wire \blk00000001/sig0000010f ;
  wire \blk00000001/sig0000010e ;
  wire \blk00000001/sig0000010d ;
  wire \blk00000001/sig0000010c ;
  wire \blk00000001/sig0000010b ;
  wire \blk00000001/sig0000010a ;
  wire \blk00000001/sig00000109 ;
  wire \blk00000001/sig00000108 ;
  wire \blk00000001/sig00000107 ;
  wire \blk00000001/sig00000106 ;
  wire \blk00000001/sig00000105 ;
  wire \blk00000001/sig00000104 ;
  wire \blk00000001/sig00000103 ;
  wire \blk00000001/sig00000102 ;
  wire \blk00000001/sig00000101 ;
  wire \blk00000001/sig00000100 ;
  wire \blk00000001/sig000000ff ;
  wire \blk00000001/sig000000fe ;
  wire \blk00000001/sig000000fd ;
  wire \blk00000001/sig000000fc ;
  wire \blk00000001/sig000000fb ;
  wire \blk00000001/sig000000fa ;
  wire \blk00000001/sig000000f9 ;
  wire \blk00000001/sig000000f8 ;
  wire \blk00000001/sig000000f7 ;
  wire \blk00000001/sig000000f6 ;
  wire \blk00000001/sig000000f5 ;
  wire \blk00000001/sig000000f4 ;
  wire \blk00000001/sig000000f3 ;
  wire \blk00000001/sig000000f2 ;
  wire \blk00000001/sig000000f1 ;
  wire \blk00000001/sig000000f0 ;
  wire \blk00000001/sig000000ef ;
  wire \blk00000001/sig000000ee ;
  wire \blk00000001/sig000000ed ;
  wire \blk00000001/sig000000ec ;
  wire \blk00000001/sig000000eb ;
  wire \blk00000001/sig000000ea ;
  wire \blk00000001/sig000000e9 ;
  wire \blk00000001/sig000000e8 ;
  wire \blk00000001/sig000000e7 ;
  wire \blk00000001/sig000000e6 ;
  wire \blk00000001/sig000000e5 ;
  wire \blk00000001/sig000000e4 ;
  wire \blk00000001/sig000000e3 ;
  wire \blk00000001/sig000000e2 ;
  wire \blk00000001/sig000000e1 ;
  wire \blk00000001/sig000000e0 ;
  wire \blk00000001/sig000000df ;
  wire \blk00000001/sig000000de ;
  wire \blk00000001/sig000000dd ;
  wire \blk00000001/sig000000dc ;
  wire \blk00000001/sig000000db ;
  wire \blk00000001/sig000000da ;
  wire \blk00000001/sig000000d9 ;
  wire \blk00000001/sig000000d8 ;
  wire \blk00000001/sig000000d7 ;
  wire \blk00000001/sig000000d6 ;
  wire \blk00000001/sig000000d5 ;
  wire \blk00000001/sig000000d4 ;
  wire \blk00000001/sig000000d3 ;
  wire \blk00000001/sig000000d2 ;
  wire \blk00000001/sig000000d1 ;
  wire \blk00000001/sig000000d0 ;
  wire \blk00000001/sig000000cf ;
  wire \blk00000001/sig000000ce ;
  wire \blk00000001/sig000000cd ;
  wire \blk00000001/sig000000cc ;
  wire \blk00000001/sig000000cb ;
  wire \blk00000001/sig000000ca ;
  wire \blk00000001/sig000000c9 ;
  wire \blk00000001/sig000000c8 ;
  wire \blk00000001/sig000000c7 ;
  wire \blk00000001/sig000000c6 ;
  wire \blk00000001/sig000000c5 ;
  wire \blk00000001/sig000000c4 ;
  wire \blk00000001/sig000000c3 ;
  wire \blk00000001/sig000000c2 ;
  wire \blk00000001/sig000000c1 ;
  wire \blk00000001/sig000000c0 ;
  wire \blk00000001/sig000000bf ;
  wire \blk00000001/sig000000be ;
  wire \blk00000001/sig000000bd ;
  wire \blk00000001/sig000000bc ;
  wire \blk00000001/sig000000bb ;
  wire \blk00000001/sig000000ba ;
  wire \blk00000001/sig000000b9 ;
  wire \blk00000001/sig000000b8 ;
  wire \blk00000001/sig000000b7 ;
  wire \blk00000001/sig000000b6 ;
  wire \blk00000001/sig000000b5 ;
  wire \blk00000001/sig000000b4 ;
  wire \blk00000001/sig000000b3 ;
  wire \blk00000001/sig000000b2 ;
  wire \blk00000001/sig000000b1 ;
  wire \blk00000001/sig000000b0 ;
  wire \blk00000001/sig000000af ;
  wire \blk00000001/sig000000ae ;
  wire \blk00000001/sig000000ad ;
  wire \blk00000001/sig000000ac ;
  wire \blk00000001/sig000000ab ;
  wire \blk00000001/sig000000aa ;
  wire \blk00000001/sig000000a9 ;
  wire \blk00000001/sig000000a8 ;
  wire \blk00000001/sig000000a7 ;
  wire \blk00000001/sig000000a6 ;
  wire \blk00000001/sig000000a5 ;
  wire \blk00000001/sig000000a4 ;
  wire \blk00000001/sig000000a3 ;
  wire \blk00000001/sig000000a2 ;
  wire \blk00000001/sig000000a1 ;
  wire \blk00000001/sig000000a0 ;
  wire \blk00000001/sig0000009f ;
  wire \blk00000001/sig0000009e ;
  wire \blk00000001/sig0000009d ;
  wire \blk00000001/sig0000009c ;
  wire \blk00000001/sig0000009b ;
  wire \blk00000001/sig0000009a ;
  wire \blk00000001/sig00000099 ;
  wire \blk00000001/sig00000098 ;
  wire \blk00000001/sig00000097 ;
  wire \blk00000001/sig00000096 ;
  wire \blk00000001/sig00000095 ;
  wire \blk00000001/sig00000094 ;
  wire \blk00000001/sig00000093 ;
  wire \blk00000001/sig00000092 ;
  wire \blk00000001/sig00000091 ;
  wire \blk00000001/sig00000090 ;
  wire \blk00000001/sig0000008f ;
  wire \blk00000001/sig0000008e ;
  wire \blk00000001/sig0000008d ;
  wire \blk00000001/sig0000008c ;
  wire \blk00000001/sig0000008b ;
  wire \blk00000001/sig0000008a ;
  wire \blk00000001/sig00000089 ;
  wire \blk00000001/sig00000088 ;
  wire \blk00000001/sig00000087 ;
  wire \blk00000001/sig00000086 ;
  wire \NLW_blk00000001/blk00000007_PATTERNBDETECT_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_MULTSIGNOUT_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_CARRYCASCOUT_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_UNDERFLOW_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_PATTERNDETECT_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_OVERFLOW_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_CARRYOUT(3)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_CARRYOUT(2)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_CARRYOUT(1)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_CARRYOUT(0)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_BCOUT(17)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_BCOUT(16)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_BCOUT(15)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_BCOUT(14)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_BCOUT(13)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_BCOUT(12)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_BCOUT(11)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_BCOUT(10)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_BCOUT(9)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_BCOUT(8)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_BCOUT(7)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_BCOUT(6)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_BCOUT(5)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_BCOUT(4)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_BCOUT(3)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_BCOUT(2)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_BCOUT(1)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_BCOUT(0)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(47)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(46)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(45)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(44)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(43)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(42)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(41)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(40)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(39)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(38)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(37)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(36)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(35)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(34)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(33)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(32)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(31)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(30)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(29)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(28)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(27)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(26)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(25)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(24)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(23)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(22)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(21)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(20)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(19)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(18)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000007_P(17)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_PATTERNBDETECT_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_MULTSIGNOUT_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_CARRYCASCOUT_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_UNDERFLOW_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_PATTERNDETECT_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_OVERFLOW_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(29)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(28)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(27)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(26)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(25)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(24)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(23)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(22)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(21)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(20)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(19)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(18)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(17)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(16)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(15)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(14)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(13)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(12)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(11)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(10)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(9)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(8)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(7)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(6)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(5)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(4)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(3)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(2)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(1)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_ACOUT(0)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_CARRYOUT(3)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_CARRYOUT(2)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_CARRYOUT(1)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_CARRYOUT(0)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_BCOUT(17)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_BCOUT(16)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_BCOUT(15)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_BCOUT(14)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_BCOUT(13)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_BCOUT(12)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_BCOUT(11)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_BCOUT(10)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_BCOUT(9)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_BCOUT(8)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_BCOUT(7)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_BCOUT(6)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_BCOUT(5)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_BCOUT(4)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_BCOUT(3)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_BCOUT(2)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_BCOUT(1)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_BCOUT(0)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(47)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(46)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(45)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(44)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(43)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(42)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(41)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(40)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(39)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(38)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(37)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(36)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(35)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(34)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(33)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(32)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(31)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(30)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(29)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(28)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(27)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(26)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(25)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(24)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(23)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(22)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(21)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(20)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(19)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(18)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(17)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(16)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(15)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(14)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(13)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(12)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(11)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(10)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(9)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(8)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(7)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(6)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(5)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(4)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(3)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(2)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(1)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000006_P(0)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_PATTERNBDETECT_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_MULTSIGNOUT_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_CARRYCASCOUT_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_UNDERFLOW_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_PATTERNDETECT_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_OVERFLOW_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_CARRYOUT(3)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_CARRYOUT(2)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_CARRYOUT(1)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_CARRYOUT(0)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_BCOUT(17)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_BCOUT(16)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_BCOUT(15)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_BCOUT(14)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_BCOUT(13)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_BCOUT(12)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_BCOUT(11)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_BCOUT(10)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_BCOUT(9)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_BCOUT(8)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_BCOUT(7)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_BCOUT(6)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_BCOUT(5)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_BCOUT(4)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_BCOUT(3)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_BCOUT(2)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_BCOUT(1)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_BCOUT(0)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(47)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(46)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(45)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(44)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(43)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(42)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(41)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(40)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(39)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(38)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(37)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(36)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(35)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(34)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(33)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(32)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(31)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(30)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(29)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(28)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(27)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(26)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(25)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(24)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(23)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(22)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(21)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(20)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(19)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(18)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000005_P(17)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_PATTERNBDETECT_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_MULTSIGNOUT_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_CARRYCASCOUT_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_UNDERFLOW_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_PATTERNDETECT_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_OVERFLOW_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(29)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(28)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(27)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(26)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(25)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(24)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(23)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(22)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(21)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(20)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(19)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(18)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(17)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(16)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(15)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(14)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(13)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(12)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(11)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(10)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(9)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(8)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(7)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(6)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(5)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(4)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(3)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(2)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(1)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_ACOUT(0)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_CARRYOUT(3)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_CARRYOUT(2)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_CARRYOUT(1)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_CARRYOUT(0)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_BCOUT(17)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_BCOUT(16)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_BCOUT(15)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_BCOUT(14)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_BCOUT(13)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_BCOUT(12)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_BCOUT(11)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_BCOUT(10)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_BCOUT(9)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_BCOUT(8)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_BCOUT(7)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_BCOUT(6)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_BCOUT(5)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_BCOUT(4)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_BCOUT(3)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_BCOUT(2)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_BCOUT(1)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_BCOUT(0)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_P(47)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_P(46)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_P(45)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_P(44)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_P(43)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_P(42)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_P(41)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_P(40)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_P(39)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_P(38)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_P(37)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_P(36)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_P(35)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_P(34)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_P(33)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_P(32)_UNCONNECTED ;
  wire \NLW_blk00000001/blk00000004_P(31)_UNCONNECTED ;
  LUT2 #(
    .INIT ( 4'h8 ))
  \blk00000001/blk0000002a  (
    .I0(ce),
    .I1(sclr),
    .O(\blk00000001/sig000000b6 )
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000029  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000178 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[0])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000028  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000179 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[1])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000027  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig0000017a ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[2])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000026  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig0000017b ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[3])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000025  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig0000017c ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[4])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000024  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig0000017d ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[5])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000023  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig0000017e ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[6])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000022  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig0000017f ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[7])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000021  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000180 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[8])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000020  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000181 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[9])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000001f  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000182 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[10])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000001e  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000183 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[11])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000001d  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000184 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[12])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000001c  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000185 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[13])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000001b  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000186 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[14])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000001a  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000187 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[15])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000019  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig00000188 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[16])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000018  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig000000e8 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[17])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000017  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig000000e9 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[18])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000016  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig000000ea ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[19])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000015  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig000000eb ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[20])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000014  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig000000ec ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[21])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000013  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig000000ed ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[22])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000012  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig000000ee ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[23])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000011  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig000000ef ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[24])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000010  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig000000f0 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[25])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000000f  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig000000f1 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[26])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000000e  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig000000f2 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[27])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000000d  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig000000f3 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[28])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000000c  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig000000f4 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[29])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000000b  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig000000f5 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[30])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk0000000a  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig000000f6 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[31])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000009  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig000000f7 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[32])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \blk00000001/blk00000008  (
    .C(clk),
    .CE(ce),
    .D(\blk00000001/sig000000f8 ),
    .R(\blk00000001/sig000000b6 ),
    .Q(p[33])
  );
  DSP48E1 #(
    .ACASCREG ( 1 ),
    .ADREG ( 0 ),
    .ALUMODEREG ( 0 ),
    .AREG ( 1 ),
    .AUTORESET_PATDET ( "NO_RESET" ),
    .A_INPUT ( "DIRECT" ),
    .BCASCREG ( 1 ),
    .BREG ( 1 ),
    .B_INPUT ( "DIRECT" ),
    .CARRYINREG ( 0 ),
    .CARRYINSELREG ( 0 ),
    .CREG ( 0 ),
    .DREG ( 0 ),
    .INMODEREG ( 0 ),
    .MASK ( 48'h000000000000 ),
    .MREG ( 0 ),
    .OPMODEREG ( 0 ),
    .PATTERN ( 48'h000000000000 ),
    .PREG ( 1 ),
    .SEL_MASK ( "MASK" ),
    .SEL_PATTERN ( "PATTERN" ),
    .USE_DPORT ( "FALSE" ),
    .USE_MULT ( "MULTIPLY" ),
    .USE_PATTERN_DETECT ( "NO_PATDET" ),
    .USE_SIMD ( "ONE48" ))
  \blk00000001/blk00000007  (
    .PATTERNBDETECT(\NLW_blk00000001/blk00000007_PATTERNBDETECT_UNCONNECTED ),
    .RSTC(\blk00000001/sig00000147 ),
    .CEB1(\blk00000001/sig00000147 ),
    .CEAD(\blk00000001/sig00000147 ),
    .MULTSIGNOUT(\NLW_blk00000001/blk00000007_MULTSIGNOUT_UNCONNECTED ),
    .CEC(\blk00000001/sig00000147 ),
    .RSTM(\blk00000001/sig00000147 ),
    .MULTSIGNIN(\blk00000001/sig00000147 ),
    .CEB2(ce),
    .RSTCTRL(\blk00000001/sig00000147 ),
    .CEP(ce),
    .CARRYCASCOUT(\NLW_blk00000001/blk00000007_CARRYCASCOUT_UNCONNECTED ),
    .RSTA(\blk00000001/sig000000b6 ),
    .CECARRYIN(\blk00000001/sig00000147 ),
    .UNDERFLOW(\NLW_blk00000001/blk00000007_UNDERFLOW_UNCONNECTED ),
    .PATTERNDETECT(\NLW_blk00000001/blk00000007_PATTERNDETECT_UNCONNECTED ),
    .RSTALUMODE(\blk00000001/sig00000147 ),
    .RSTALLCARRYIN(\blk00000001/sig00000147 ),
    .CED(\blk00000001/sig00000147 ),
    .RSTD(\blk00000001/sig00000147 ),
    .CEALUMODE(\blk00000001/sig00000147 ),
    .CEA2(ce),
    .CLK(clk),
    .CEA1(\blk00000001/sig00000147 ),
    .RSTB(\blk00000001/sig000000b6 ),
    .OVERFLOW(\NLW_blk00000001/blk00000007_OVERFLOW_UNCONNECTED ),
    .CECTRL(\blk00000001/sig00000147 ),
    .CEM(\blk00000001/sig00000147 ),
    .CARRYIN(\blk00000001/sig00000147 ),
    .CARRYCASCIN(\blk00000001/sig00000147 ),
    .RSTINMODE(\blk00000001/sig00000147 ),
    .CEINMODE(\blk00000001/sig00000147 ),
    .RSTP(\blk00000001/sig000000b6 ),
    .ACOUT({\blk00000001/sig000001a6 , \blk00000001/sig000001a5 , \blk00000001/sig000001a4 , \blk00000001/sig000001a3 , \blk00000001/sig000001a2 , 
\blk00000001/sig000001a1 , \blk00000001/sig000001a0 , \blk00000001/sig0000019f , \blk00000001/sig0000019e , \blk00000001/sig0000019d , 
\blk00000001/sig0000019c , \blk00000001/sig0000019b , \blk00000001/sig0000019a , \blk00000001/sig00000199 , \blk00000001/sig00000198 , 
\blk00000001/sig00000197 , \blk00000001/sig00000196 , \blk00000001/sig00000195 , \blk00000001/sig00000194 , \blk00000001/sig00000193 , 
\blk00000001/sig00000192 , \blk00000001/sig00000191 , \blk00000001/sig00000190 , \blk00000001/sig0000018f , \blk00000001/sig0000018e , 
\blk00000001/sig0000018d , \blk00000001/sig0000018c , \blk00000001/sig0000018b , \blk00000001/sig0000018a , \blk00000001/sig00000189 }),
    .OPMODE({\blk00000001/sig00000147 , \blk00000001/sig000000b7 , \blk00000001/sig000000b7 , \blk00000001/sig00000147 , \blk00000001/sig000000b7 , 
\blk00000001/sig00000147 , \blk00000001/sig000000b7 }),
    .PCIN({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .ALUMODE({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .C({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .CARRYOUT({\NLW_blk00000001/blk00000007_CARRYOUT(3)_UNCONNECTED , \NLW_blk00000001/blk00000007_CARRYOUT(2)_UNCONNECTED , 
\NLW_blk00000001/blk00000007_CARRYOUT(1)_UNCONNECTED , \NLW_blk00000001/blk00000007_CARRYOUT(0)_UNCONNECTED }),
    .INMODE({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .BCIN({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .B({\blk00000001/sig00000147 , b[16], b[15], b[14], b[13], b[12], b[11], b[10], b[9], b[8], b[7], b[6], b[5], b[4], b[3], b[2], b[1], b[0]}),
    .BCOUT({\NLW_blk00000001/blk00000007_BCOUT(17)_UNCONNECTED , \NLW_blk00000001/blk00000007_BCOUT(16)_UNCONNECTED , 
\NLW_blk00000001/blk00000007_BCOUT(15)_UNCONNECTED , \NLW_blk00000001/blk00000007_BCOUT(14)_UNCONNECTED , 
\NLW_blk00000001/blk00000007_BCOUT(13)_UNCONNECTED , \NLW_blk00000001/blk00000007_BCOUT(12)_UNCONNECTED , 
\NLW_blk00000001/blk00000007_BCOUT(11)_UNCONNECTED , \NLW_blk00000001/blk00000007_BCOUT(10)_UNCONNECTED , 
\NLW_blk00000001/blk00000007_BCOUT(9)_UNCONNECTED , \NLW_blk00000001/blk00000007_BCOUT(8)_UNCONNECTED , 
\NLW_blk00000001/blk00000007_BCOUT(7)_UNCONNECTED , \NLW_blk00000001/blk00000007_BCOUT(6)_UNCONNECTED , 
\NLW_blk00000001/blk00000007_BCOUT(5)_UNCONNECTED , \NLW_blk00000001/blk00000007_BCOUT(4)_UNCONNECTED , 
\NLW_blk00000001/blk00000007_BCOUT(3)_UNCONNECTED , \NLW_blk00000001/blk00000007_BCOUT(2)_UNCONNECTED , 
\NLW_blk00000001/blk00000007_BCOUT(1)_UNCONNECTED , \NLW_blk00000001/blk00000007_BCOUT(0)_UNCONNECTED }),
    .D({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .P({\NLW_blk00000001/blk00000007_P(47)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(46)_UNCONNECTED , 
\NLW_blk00000001/blk00000007_P(45)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(44)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(43)_UNCONNECTED , 
\NLW_blk00000001/blk00000007_P(42)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(41)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(40)_UNCONNECTED , 
\NLW_blk00000001/blk00000007_P(39)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(38)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(37)_UNCONNECTED , 
\NLW_blk00000001/blk00000007_P(36)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(35)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(34)_UNCONNECTED , 
\NLW_blk00000001/blk00000007_P(33)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(32)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(31)_UNCONNECTED , 
\NLW_blk00000001/blk00000007_P(30)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(29)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(28)_UNCONNECTED , 
\NLW_blk00000001/blk00000007_P(27)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(26)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(25)_UNCONNECTED , 
\NLW_blk00000001/blk00000007_P(24)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(23)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(22)_UNCONNECTED , 
\NLW_blk00000001/blk00000007_P(21)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(20)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(19)_UNCONNECTED , 
\NLW_blk00000001/blk00000007_P(18)_UNCONNECTED , \NLW_blk00000001/blk00000007_P(17)_UNCONNECTED , \blk00000001/sig00000188 , \blk00000001/sig00000187 
, \blk00000001/sig00000186 , \blk00000001/sig00000185 , \blk00000001/sig00000184 , \blk00000001/sig00000183 , \blk00000001/sig00000182 , 
\blk00000001/sig00000181 , \blk00000001/sig00000180 , \blk00000001/sig0000017f , \blk00000001/sig0000017e , \blk00000001/sig0000017d , 
\blk00000001/sig0000017c , \blk00000001/sig0000017b , \blk00000001/sig0000017a , \blk00000001/sig00000179 , \blk00000001/sig00000178 }),
    .A({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , a[16], a[15], a[14], a[13], a[12], a[11], a[10], a[9], a[8], a[7], 
a[6], a[5], a[4], a[3], a[2], a[1], a[0]}),
    .PCOUT({\blk00000001/sig00000177 , \blk00000001/sig00000176 , \blk00000001/sig00000175 , \blk00000001/sig00000174 , \blk00000001/sig00000173 , 
\blk00000001/sig00000172 , \blk00000001/sig00000171 , \blk00000001/sig00000170 , \blk00000001/sig0000016f , \blk00000001/sig0000016e , 
\blk00000001/sig0000016d , \blk00000001/sig0000016c , \blk00000001/sig0000016b , \blk00000001/sig0000016a , \blk00000001/sig00000169 , 
\blk00000001/sig00000168 , \blk00000001/sig00000167 , \blk00000001/sig00000166 , \blk00000001/sig00000165 , \blk00000001/sig00000164 , 
\blk00000001/sig00000163 , \blk00000001/sig00000162 , \blk00000001/sig00000161 , \blk00000001/sig00000160 , \blk00000001/sig0000015f , 
\blk00000001/sig0000015e , \blk00000001/sig0000015d , \blk00000001/sig0000015c , \blk00000001/sig0000015b , \blk00000001/sig0000015a , 
\blk00000001/sig00000159 , \blk00000001/sig00000158 , \blk00000001/sig00000157 , \blk00000001/sig00000156 , \blk00000001/sig00000155 , 
\blk00000001/sig00000154 , \blk00000001/sig00000153 , \blk00000001/sig00000152 , \blk00000001/sig00000151 , \blk00000001/sig00000150 , 
\blk00000001/sig0000014f , \blk00000001/sig0000014e , \blk00000001/sig0000014d , \blk00000001/sig0000014c , \blk00000001/sig0000014b , 
\blk00000001/sig0000014a , \blk00000001/sig00000149 , \blk00000001/sig00000148 }),
    .ACIN({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .CARRYINSEL({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 })
  );
  DSP48E1 #(
    .ACASCREG ( 0 ),
    .ADREG ( 0 ),
    .ALUMODEREG ( 1 ),
    .AREG ( 0 ),
    .AUTORESET_PATDET ( "NO_RESET" ),
    .A_INPUT ( "CASCADE" ),
    .BCASCREG ( 1 ),
    .BREG ( 1 ),
    .B_INPUT ( "DIRECT" ),
    .CARRYINREG ( 0 ),
    .CARRYINSELREG ( 0 ),
    .CREG ( 0 ),
    .DREG ( 0 ),
    .INMODEREG ( 0 ),
    .MASK ( 48'h000000000000 ),
    .MREG ( 1 ),
    .OPMODEREG ( 0 ),
    .PATTERN ( 48'h000000000000 ),
    .PREG ( 0 ),
    .SEL_MASK ( "MASK" ),
    .SEL_PATTERN ( "PATTERN" ),
    .USE_DPORT ( "FALSE" ),
    .USE_MULT ( "MULTIPLY" ),
    .USE_PATTERN_DETECT ( "NO_PATDET" ),
    .USE_SIMD ( "ONE48" ))
  \blk00000001/blk00000006  (
    .PATTERNBDETECT(\NLW_blk00000001/blk00000006_PATTERNBDETECT_UNCONNECTED ),
    .RSTC(\blk00000001/sig00000147 ),
    .CEB1(\blk00000001/sig00000147 ),
    .CEAD(\blk00000001/sig00000147 ),
    .MULTSIGNOUT(\NLW_blk00000001/blk00000006_MULTSIGNOUT_UNCONNECTED ),
    .CEC(\blk00000001/sig00000147 ),
    .RSTM(\blk00000001/sig000000b6 ),
    .MULTSIGNIN(\blk00000001/sig00000147 ),
    .CEB2(ce),
    .RSTCTRL(\blk00000001/sig00000147 ),
    .CEP(\blk00000001/sig00000147 ),
    .CARRYCASCOUT(\NLW_blk00000001/blk00000006_CARRYCASCOUT_UNCONNECTED ),
    .RSTA(\blk00000001/sig00000147 ),
    .CECARRYIN(\blk00000001/sig00000147 ),
    .UNDERFLOW(\NLW_blk00000001/blk00000006_UNDERFLOW_UNCONNECTED ),
    .PATTERNDETECT(\NLW_blk00000001/blk00000006_PATTERNDETECT_UNCONNECTED ),
    .RSTALUMODE(\blk00000001/sig000000b6 ),
    .RSTALLCARRYIN(\blk00000001/sig00000147 ),
    .CED(\blk00000001/sig00000147 ),
    .RSTD(\blk00000001/sig00000147 ),
    .CEALUMODE(ce),
    .CEA2(\blk00000001/sig00000147 ),
    .CLK(clk),
    .CEA1(\blk00000001/sig00000147 ),
    .RSTB(\blk00000001/sig000000b6 ),
    .OVERFLOW(\NLW_blk00000001/blk00000006_OVERFLOW_UNCONNECTED ),
    .CECTRL(\blk00000001/sig00000147 ),
    .CEM(ce),
    .CARRYIN(\blk00000001/sig00000147 ),
    .CARRYCASCIN(\blk00000001/sig00000147 ),
    .RSTINMODE(\blk00000001/sig00000147 ),
    .CEINMODE(\blk00000001/sig00000147 ),
    .RSTP(\blk00000001/sig00000147 ),
    .ACOUT({\NLW_blk00000001/blk00000006_ACOUT(29)_UNCONNECTED , \NLW_blk00000001/blk00000006_ACOUT(28)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_ACOUT(27)_UNCONNECTED , \NLW_blk00000001/blk00000006_ACOUT(26)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_ACOUT(25)_UNCONNECTED , \NLW_blk00000001/blk00000006_ACOUT(24)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_ACOUT(23)_UNCONNECTED , \NLW_blk00000001/blk00000006_ACOUT(22)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_ACOUT(21)_UNCONNECTED , \NLW_blk00000001/blk00000006_ACOUT(20)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_ACOUT(19)_UNCONNECTED , \NLW_blk00000001/blk00000006_ACOUT(18)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_ACOUT(17)_UNCONNECTED , \NLW_blk00000001/blk00000006_ACOUT(16)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_ACOUT(15)_UNCONNECTED , \NLW_blk00000001/blk00000006_ACOUT(14)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_ACOUT(13)_UNCONNECTED , \NLW_blk00000001/blk00000006_ACOUT(12)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_ACOUT(11)_UNCONNECTED , \NLW_blk00000001/blk00000006_ACOUT(10)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_ACOUT(9)_UNCONNECTED , \NLW_blk00000001/blk00000006_ACOUT(8)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_ACOUT(7)_UNCONNECTED , \NLW_blk00000001/blk00000006_ACOUT(6)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_ACOUT(5)_UNCONNECTED , \NLW_blk00000001/blk00000006_ACOUT(4)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_ACOUT(3)_UNCONNECTED , \NLW_blk00000001/blk00000006_ACOUT(2)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_ACOUT(1)_UNCONNECTED , \NLW_blk00000001/blk00000006_ACOUT(0)_UNCONNECTED }),
    .OPMODE({\blk00000001/sig000000b7 , \blk00000001/sig00000147 , \blk00000001/sig000000b7 , \blk00000001/sig00000147 , \blk00000001/sig000000b7 , 
\blk00000001/sig00000147 , \blk00000001/sig000000b7 }),
    .PCIN({\blk00000001/sig00000177 , \blk00000001/sig00000176 , \blk00000001/sig00000175 , \blk00000001/sig00000174 , \blk00000001/sig00000173 , 
\blk00000001/sig00000172 , \blk00000001/sig00000171 , \blk00000001/sig00000170 , \blk00000001/sig0000016f , \blk00000001/sig0000016e , 
\blk00000001/sig0000016d , \blk00000001/sig0000016c , \blk00000001/sig0000016b , \blk00000001/sig0000016a , \blk00000001/sig00000169 , 
\blk00000001/sig00000168 , \blk00000001/sig00000167 , \blk00000001/sig00000166 , \blk00000001/sig00000165 , \blk00000001/sig00000164 , 
\blk00000001/sig00000163 , \blk00000001/sig00000162 , \blk00000001/sig00000161 , \blk00000001/sig00000160 , \blk00000001/sig0000015f , 
\blk00000001/sig0000015e , \blk00000001/sig0000015d , \blk00000001/sig0000015c , \blk00000001/sig0000015b , \blk00000001/sig0000015a , 
\blk00000001/sig00000159 , \blk00000001/sig00000158 , \blk00000001/sig00000157 , \blk00000001/sig00000156 , \blk00000001/sig00000155 , 
\blk00000001/sig00000154 , \blk00000001/sig00000153 , \blk00000001/sig00000152 , \blk00000001/sig00000151 , \blk00000001/sig00000150 , 
\blk00000001/sig0000014f , \blk00000001/sig0000014e , \blk00000001/sig0000014d , \blk00000001/sig0000014c , \blk00000001/sig0000014b , 
\blk00000001/sig0000014a , \blk00000001/sig00000149 , \blk00000001/sig00000148 }),
    .ALUMODE({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .C({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .CARRYOUT({\NLW_blk00000001/blk00000006_CARRYOUT(3)_UNCONNECTED , \NLW_blk00000001/blk00000006_CARRYOUT(2)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_CARRYOUT(1)_UNCONNECTED , \NLW_blk00000001/blk00000006_CARRYOUT(0)_UNCONNECTED }),
    .INMODE({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .BCIN({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .B({b[31], b[31], b[31], b[31], b[30], b[29], b[28], b[27], b[26], b[25], b[24], b[23], b[22], b[21], b[20], b[19], b[18], b[17]}),
    .BCOUT({\NLW_blk00000001/blk00000006_BCOUT(17)_UNCONNECTED , \NLW_blk00000001/blk00000006_BCOUT(16)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_BCOUT(15)_UNCONNECTED , \NLW_blk00000001/blk00000006_BCOUT(14)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_BCOUT(13)_UNCONNECTED , \NLW_blk00000001/blk00000006_BCOUT(12)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_BCOUT(11)_UNCONNECTED , \NLW_blk00000001/blk00000006_BCOUT(10)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_BCOUT(9)_UNCONNECTED , \NLW_blk00000001/blk00000006_BCOUT(8)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_BCOUT(7)_UNCONNECTED , \NLW_blk00000001/blk00000006_BCOUT(6)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_BCOUT(5)_UNCONNECTED , \NLW_blk00000001/blk00000006_BCOUT(4)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_BCOUT(3)_UNCONNECTED , \NLW_blk00000001/blk00000006_BCOUT(2)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_BCOUT(1)_UNCONNECTED , \NLW_blk00000001/blk00000006_BCOUT(0)_UNCONNECTED }),
    .D({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .P({\NLW_blk00000001/blk00000006_P(47)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(46)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_P(45)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(44)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(43)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_P(42)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(41)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(40)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_P(39)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(38)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(37)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_P(36)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(35)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(34)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_P(33)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(32)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(31)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_P(30)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(29)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(28)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_P(27)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(26)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(25)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_P(24)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(23)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(22)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_P(21)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(20)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(19)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_P(18)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(17)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(16)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_P(15)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(14)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(13)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_P(12)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(11)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(10)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_P(9)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(8)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(7)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_P(6)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(5)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(4)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_P(3)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(2)_UNCONNECTED , \NLW_blk00000001/blk00000006_P(1)_UNCONNECTED , 
\NLW_blk00000001/blk00000006_P(0)_UNCONNECTED }),
    .A({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .PCOUT({\blk00000001/sig00000146 , \blk00000001/sig00000145 , \blk00000001/sig00000144 , \blk00000001/sig00000143 , \blk00000001/sig00000142 , 
\blk00000001/sig00000141 , \blk00000001/sig00000140 , \blk00000001/sig0000013f , \blk00000001/sig0000013e , \blk00000001/sig0000013d , 
\blk00000001/sig0000013c , \blk00000001/sig0000013b , \blk00000001/sig0000013a , \blk00000001/sig00000139 , \blk00000001/sig00000138 , 
\blk00000001/sig00000137 , \blk00000001/sig00000136 , \blk00000001/sig00000135 , \blk00000001/sig00000134 , \blk00000001/sig00000133 , 
\blk00000001/sig00000132 , \blk00000001/sig00000131 , \blk00000001/sig00000130 , \blk00000001/sig0000012f , \blk00000001/sig0000012e , 
\blk00000001/sig0000012d , \blk00000001/sig0000012c , \blk00000001/sig0000012b , \blk00000001/sig0000012a , \blk00000001/sig00000129 , 
\blk00000001/sig00000128 , \blk00000001/sig00000127 , \blk00000001/sig00000126 , \blk00000001/sig00000125 , \blk00000001/sig00000124 , 
\blk00000001/sig00000123 , \blk00000001/sig00000122 , \blk00000001/sig00000121 , \blk00000001/sig00000120 , \blk00000001/sig0000011f , 
\blk00000001/sig0000011e , \blk00000001/sig0000011d , \blk00000001/sig0000011c , \blk00000001/sig0000011b , \blk00000001/sig0000011a , 
\blk00000001/sig00000119 , \blk00000001/sig00000118 , \blk00000001/sig00000117 }),
    .ACIN({\blk00000001/sig000001a6 , \blk00000001/sig000001a5 , \blk00000001/sig000001a4 , \blk00000001/sig000001a3 , \blk00000001/sig000001a2 , 
\blk00000001/sig000001a1 , \blk00000001/sig000001a0 , \blk00000001/sig0000019f , \blk00000001/sig0000019e , \blk00000001/sig0000019d , 
\blk00000001/sig0000019c , \blk00000001/sig0000019b , \blk00000001/sig0000019a , \blk00000001/sig00000199 , \blk00000001/sig00000198 , 
\blk00000001/sig00000197 , \blk00000001/sig00000196 , \blk00000001/sig00000195 , \blk00000001/sig00000194 , \blk00000001/sig00000193 , 
\blk00000001/sig00000192 , \blk00000001/sig00000191 , \blk00000001/sig00000190 , \blk00000001/sig0000018f , \blk00000001/sig0000018e , 
\blk00000001/sig0000018d , \blk00000001/sig0000018c , \blk00000001/sig0000018b , \blk00000001/sig0000018a , \blk00000001/sig00000189 }),
    .CARRYINSEL({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 })
  );
  DSP48E1 #(
    .ACASCREG ( 2 ),
    .ADREG ( 0 ),
    .ALUMODEREG ( 0 ),
    .AREG ( 2 ),
    .AUTORESET_PATDET ( "NO_RESET" ),
    .A_INPUT ( "DIRECT" ),
    .BCASCREG ( 2 ),
    .BREG ( 2 ),
    .B_INPUT ( "DIRECT" ),
    .CARRYINREG ( 0 ),
    .CARRYINSELREG ( 0 ),
    .CREG ( 0 ),
    .DREG ( 0 ),
    .INMODEREG ( 0 ),
    .MASK ( 48'h000000000000 ),
    .MREG ( 0 ),
    .OPMODEREG ( 0 ),
    .PATTERN ( 48'h000000000000 ),
    .PREG ( 0 ),
    .SEL_MASK ( "MASK" ),
    .SEL_PATTERN ( "PATTERN" ),
    .USE_DPORT ( "FALSE" ),
    .USE_MULT ( "MULTIPLY" ),
    .USE_PATTERN_DETECT ( "NO_PATDET" ),
    .USE_SIMD ( "ONE48" ))
  \blk00000001/blk00000005  (
    .PATTERNBDETECT(\NLW_blk00000001/blk00000005_PATTERNBDETECT_UNCONNECTED ),
    .RSTC(\blk00000001/sig00000147 ),
    .CEB1(ce),
    .CEAD(\blk00000001/sig00000147 ),
    .MULTSIGNOUT(\NLW_blk00000001/blk00000005_MULTSIGNOUT_UNCONNECTED ),
    .CEC(\blk00000001/sig00000147 ),
    .RSTM(\blk00000001/sig00000147 ),
    .MULTSIGNIN(\blk00000001/sig00000147 ),
    .CEB2(ce),
    .RSTCTRL(\blk00000001/sig00000147 ),
    .CEP(\blk00000001/sig00000147 ),
    .CARRYCASCOUT(\NLW_blk00000001/blk00000005_CARRYCASCOUT_UNCONNECTED ),
    .RSTA(\blk00000001/sig000000b6 ),
    .CECARRYIN(\blk00000001/sig00000147 ),
    .UNDERFLOW(\NLW_blk00000001/blk00000005_UNDERFLOW_UNCONNECTED ),
    .PATTERNDETECT(\NLW_blk00000001/blk00000005_PATTERNDETECT_UNCONNECTED ),
    .RSTALUMODE(\blk00000001/sig00000147 ),
    .RSTALLCARRYIN(\blk00000001/sig00000147 ),
    .CED(\blk00000001/sig00000147 ),
    .RSTD(\blk00000001/sig00000147 ),
    .CEALUMODE(\blk00000001/sig00000147 ),
    .CEA2(ce),
    .CLK(clk),
    .CEA1(ce),
    .RSTB(\blk00000001/sig000000b6 ),
    .OVERFLOW(\NLW_blk00000001/blk00000005_OVERFLOW_UNCONNECTED ),
    .CECTRL(\blk00000001/sig00000147 ),
    .CEM(\blk00000001/sig00000147 ),
    .CARRYIN(\blk00000001/sig00000147 ),
    .CARRYCASCIN(\blk00000001/sig00000147 ),
    .RSTINMODE(\blk00000001/sig00000147 ),
    .CEINMODE(\blk00000001/sig00000147 ),
    .RSTP(\blk00000001/sig00000147 ),
    .ACOUT({\blk00000001/sig00000116 , \blk00000001/sig00000115 , \blk00000001/sig00000114 , \blk00000001/sig00000113 , \blk00000001/sig00000112 , 
\blk00000001/sig00000111 , \blk00000001/sig00000110 , \blk00000001/sig0000010f , \blk00000001/sig0000010e , \blk00000001/sig0000010d , 
\blk00000001/sig0000010c , \blk00000001/sig0000010b , \blk00000001/sig0000010a , \blk00000001/sig00000109 , \blk00000001/sig00000108 , 
\blk00000001/sig00000107 , \blk00000001/sig00000106 , \blk00000001/sig00000105 , \blk00000001/sig00000104 , \blk00000001/sig00000103 , 
\blk00000001/sig00000102 , \blk00000001/sig00000101 , \blk00000001/sig00000100 , \blk00000001/sig000000ff , \blk00000001/sig000000fe , 
\blk00000001/sig000000fd , \blk00000001/sig000000fc , \blk00000001/sig000000fb , \blk00000001/sig000000fa , \blk00000001/sig000000f9 }),
    .OPMODE({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig000000b7 , \blk00000001/sig00000147 , \blk00000001/sig000000b7 , 
\blk00000001/sig00000147 , \blk00000001/sig000000b7 }),
    .PCIN({\blk00000001/sig00000146 , \blk00000001/sig00000145 , \blk00000001/sig00000144 , \blk00000001/sig00000143 , \blk00000001/sig00000142 , 
\blk00000001/sig00000141 , \blk00000001/sig00000140 , \blk00000001/sig0000013f , \blk00000001/sig0000013e , \blk00000001/sig0000013d , 
\blk00000001/sig0000013c , \blk00000001/sig0000013b , \blk00000001/sig0000013a , \blk00000001/sig00000139 , \blk00000001/sig00000138 , 
\blk00000001/sig00000137 , \blk00000001/sig00000136 , \blk00000001/sig00000135 , \blk00000001/sig00000134 , \blk00000001/sig00000133 , 
\blk00000001/sig00000132 , \blk00000001/sig00000131 , \blk00000001/sig00000130 , \blk00000001/sig0000012f , \blk00000001/sig0000012e , 
\blk00000001/sig0000012d , \blk00000001/sig0000012c , \blk00000001/sig0000012b , \blk00000001/sig0000012a , \blk00000001/sig00000129 , 
\blk00000001/sig00000128 , \blk00000001/sig00000127 , \blk00000001/sig00000126 , \blk00000001/sig00000125 , \blk00000001/sig00000124 , 
\blk00000001/sig00000123 , \blk00000001/sig00000122 , \blk00000001/sig00000121 , \blk00000001/sig00000120 , \blk00000001/sig0000011f , 
\blk00000001/sig0000011e , \blk00000001/sig0000011d , \blk00000001/sig0000011c , \blk00000001/sig0000011b , \blk00000001/sig0000011a , 
\blk00000001/sig00000119 , \blk00000001/sig00000118 , \blk00000001/sig00000117 }),
    .ALUMODE({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .C({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .CARRYOUT({\NLW_blk00000001/blk00000005_CARRYOUT(3)_UNCONNECTED , \NLW_blk00000001/blk00000005_CARRYOUT(2)_UNCONNECTED , 
\NLW_blk00000001/blk00000005_CARRYOUT(1)_UNCONNECTED , \NLW_blk00000001/blk00000005_CARRYOUT(0)_UNCONNECTED }),
    .INMODE({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .BCIN({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .B({\blk00000001/sig00000147 , b[16], b[15], b[14], b[13], b[12], b[11], b[10], b[9], b[8], b[7], b[6], b[5], b[4], b[3], b[2], b[1], b[0]}),
    .BCOUT({\NLW_blk00000001/blk00000005_BCOUT(17)_UNCONNECTED , \NLW_blk00000001/blk00000005_BCOUT(16)_UNCONNECTED , 
\NLW_blk00000001/blk00000005_BCOUT(15)_UNCONNECTED , \NLW_blk00000001/blk00000005_BCOUT(14)_UNCONNECTED , 
\NLW_blk00000001/blk00000005_BCOUT(13)_UNCONNECTED , \NLW_blk00000001/blk00000005_BCOUT(12)_UNCONNECTED , 
\NLW_blk00000001/blk00000005_BCOUT(11)_UNCONNECTED , \NLW_blk00000001/blk00000005_BCOUT(10)_UNCONNECTED , 
\NLW_blk00000001/blk00000005_BCOUT(9)_UNCONNECTED , \NLW_blk00000001/blk00000005_BCOUT(8)_UNCONNECTED , 
\NLW_blk00000001/blk00000005_BCOUT(7)_UNCONNECTED , \NLW_blk00000001/blk00000005_BCOUT(6)_UNCONNECTED , 
\NLW_blk00000001/blk00000005_BCOUT(5)_UNCONNECTED , \NLW_blk00000001/blk00000005_BCOUT(4)_UNCONNECTED , 
\NLW_blk00000001/blk00000005_BCOUT(3)_UNCONNECTED , \NLW_blk00000001/blk00000005_BCOUT(2)_UNCONNECTED , 
\NLW_blk00000001/blk00000005_BCOUT(1)_UNCONNECTED , \NLW_blk00000001/blk00000005_BCOUT(0)_UNCONNECTED }),
    .D({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .P({\NLW_blk00000001/blk00000005_P(47)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(46)_UNCONNECTED , 
\NLW_blk00000001/blk00000005_P(45)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(44)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(43)_UNCONNECTED , 
\NLW_blk00000001/blk00000005_P(42)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(41)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(40)_UNCONNECTED , 
\NLW_blk00000001/blk00000005_P(39)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(38)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(37)_UNCONNECTED , 
\NLW_blk00000001/blk00000005_P(36)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(35)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(34)_UNCONNECTED , 
\NLW_blk00000001/blk00000005_P(33)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(32)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(31)_UNCONNECTED , 
\NLW_blk00000001/blk00000005_P(30)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(29)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(28)_UNCONNECTED , 
\NLW_blk00000001/blk00000005_P(27)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(26)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(25)_UNCONNECTED , 
\NLW_blk00000001/blk00000005_P(24)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(23)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(22)_UNCONNECTED , 
\NLW_blk00000001/blk00000005_P(21)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(20)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(19)_UNCONNECTED , 
\NLW_blk00000001/blk00000005_P(18)_UNCONNECTED , \NLW_blk00000001/blk00000005_P(17)_UNCONNECTED , \blk00000001/sig000000f8 , \blk00000001/sig000000f7 
, \blk00000001/sig000000f6 , \blk00000001/sig000000f5 , \blk00000001/sig000000f4 , \blk00000001/sig000000f3 , \blk00000001/sig000000f2 , 
\blk00000001/sig000000f1 , \blk00000001/sig000000f0 , \blk00000001/sig000000ef , \blk00000001/sig000000ee , \blk00000001/sig000000ed , 
\blk00000001/sig000000ec , \blk00000001/sig000000eb , \blk00000001/sig000000ea , \blk00000001/sig000000e9 , \blk00000001/sig000000e8 }),
    .A({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , a[32], 
a[32], a[32], a[32], a[32], a[32], a[32], a[32], a[32], a[32], a[31], a[30], a[29], a[28], a[27], a[26], a[25], a[24], a[23], a[22], a[21], a[20], 
a[19], a[18], a[17]}),
    .PCOUT({\blk00000001/sig000000e7 , \blk00000001/sig000000e6 , \blk00000001/sig000000e5 , \blk00000001/sig000000e4 , \blk00000001/sig000000e3 , 
\blk00000001/sig000000e2 , \blk00000001/sig000000e1 , \blk00000001/sig000000e0 , \blk00000001/sig000000df , \blk00000001/sig000000de , 
\blk00000001/sig000000dd , \blk00000001/sig000000dc , \blk00000001/sig000000db , \blk00000001/sig000000da , \blk00000001/sig000000d9 , 
\blk00000001/sig000000d8 , \blk00000001/sig000000d7 , \blk00000001/sig000000d6 , \blk00000001/sig000000d5 , \blk00000001/sig000000d4 , 
\blk00000001/sig000000d3 , \blk00000001/sig000000d2 , \blk00000001/sig000000d1 , \blk00000001/sig000000d0 , \blk00000001/sig000000cf , 
\blk00000001/sig000000ce , \blk00000001/sig000000cd , \blk00000001/sig000000cc , \blk00000001/sig000000cb , \blk00000001/sig000000ca , 
\blk00000001/sig000000c9 , \blk00000001/sig000000c8 , \blk00000001/sig000000c7 , \blk00000001/sig000000c6 , \blk00000001/sig000000c5 , 
\blk00000001/sig000000c4 , \blk00000001/sig000000c3 , \blk00000001/sig000000c2 , \blk00000001/sig000000c1 , \blk00000001/sig000000c0 , 
\blk00000001/sig000000bf , \blk00000001/sig000000be , \blk00000001/sig000000bd , \blk00000001/sig000000bc , \blk00000001/sig000000bb , 
\blk00000001/sig000000ba , \blk00000001/sig000000b9 , \blk00000001/sig000000b8 }),
    .ACIN({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .CARRYINSEL({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 })
  );
  DSP48E1 #(
    .ACASCREG ( 0 ),
    .ADREG ( 0 ),
    .ALUMODEREG ( 0 ),
    .AREG ( 0 ),
    .AUTORESET_PATDET ( "NO_RESET" ),
    .A_INPUT ( "CASCADE" ),
    .BCASCREG ( 2 ),
    .BREG ( 2 ),
    .B_INPUT ( "DIRECT" ),
    .CARRYINREG ( 0 ),
    .CARRYINSELREG ( 0 ),
    .CREG ( 0 ),
    .DREG ( 0 ),
    .INMODEREG ( 0 ),
    .MASK ( 48'h000000000000 ),
    .MREG ( 0 ),
    .OPMODEREG ( 0 ),
    .PATTERN ( 48'h000000000000 ),
    .PREG ( 1 ),
    .SEL_MASK ( "MASK" ),
    .SEL_PATTERN ( "PATTERN" ),
    .USE_DPORT ( "FALSE" ),
    .USE_MULT ( "MULTIPLY" ),
    .USE_PATTERN_DETECT ( "NO_PATDET" ),
    .USE_SIMD ( "ONE48" ))
  \blk00000001/blk00000004  (
    .PATTERNBDETECT(\NLW_blk00000001/blk00000004_PATTERNBDETECT_UNCONNECTED ),
    .RSTC(\blk00000001/sig00000147 ),
    .CEB1(ce),
    .CEAD(\blk00000001/sig00000147 ),
    .MULTSIGNOUT(\NLW_blk00000001/blk00000004_MULTSIGNOUT_UNCONNECTED ),
    .CEC(\blk00000001/sig00000147 ),
    .RSTM(\blk00000001/sig00000147 ),
    .MULTSIGNIN(\blk00000001/sig00000147 ),
    .CEB2(ce),
    .RSTCTRL(\blk00000001/sig00000147 ),
    .CEP(ce),
    .CARRYCASCOUT(\NLW_blk00000001/blk00000004_CARRYCASCOUT_UNCONNECTED ),
    .RSTA(\blk00000001/sig00000147 ),
    .CECARRYIN(\blk00000001/sig00000147 ),
    .UNDERFLOW(\NLW_blk00000001/blk00000004_UNDERFLOW_UNCONNECTED ),
    .PATTERNDETECT(\NLW_blk00000001/blk00000004_PATTERNDETECT_UNCONNECTED ),
    .RSTALUMODE(\blk00000001/sig00000147 ),
    .RSTALLCARRYIN(\blk00000001/sig00000147 ),
    .CED(\blk00000001/sig00000147 ),
    .RSTD(\blk00000001/sig00000147 ),
    .CEALUMODE(\blk00000001/sig00000147 ),
    .CEA2(\blk00000001/sig00000147 ),
    .CLK(clk),
    .CEA1(\blk00000001/sig00000147 ),
    .RSTB(\blk00000001/sig000000b6 ),
    .OVERFLOW(\NLW_blk00000001/blk00000004_OVERFLOW_UNCONNECTED ),
    .CECTRL(\blk00000001/sig00000147 ),
    .CEM(\blk00000001/sig00000147 ),
    .CARRYIN(\blk00000001/sig00000147 ),
    .CARRYCASCIN(\blk00000001/sig00000147 ),
    .RSTINMODE(\blk00000001/sig00000147 ),
    .CEINMODE(\blk00000001/sig00000147 ),
    .RSTP(\blk00000001/sig000000b6 ),
    .ACOUT({\NLW_blk00000001/blk00000004_ACOUT(29)_UNCONNECTED , \NLW_blk00000001/blk00000004_ACOUT(28)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_ACOUT(27)_UNCONNECTED , \NLW_blk00000001/blk00000004_ACOUT(26)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_ACOUT(25)_UNCONNECTED , \NLW_blk00000001/blk00000004_ACOUT(24)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_ACOUT(23)_UNCONNECTED , \NLW_blk00000001/blk00000004_ACOUT(22)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_ACOUT(21)_UNCONNECTED , \NLW_blk00000001/blk00000004_ACOUT(20)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_ACOUT(19)_UNCONNECTED , \NLW_blk00000001/blk00000004_ACOUT(18)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_ACOUT(17)_UNCONNECTED , \NLW_blk00000001/blk00000004_ACOUT(16)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_ACOUT(15)_UNCONNECTED , \NLW_blk00000001/blk00000004_ACOUT(14)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_ACOUT(13)_UNCONNECTED , \NLW_blk00000001/blk00000004_ACOUT(12)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_ACOUT(11)_UNCONNECTED , \NLW_blk00000001/blk00000004_ACOUT(10)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_ACOUT(9)_UNCONNECTED , \NLW_blk00000001/blk00000004_ACOUT(8)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_ACOUT(7)_UNCONNECTED , \NLW_blk00000001/blk00000004_ACOUT(6)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_ACOUT(5)_UNCONNECTED , \NLW_blk00000001/blk00000004_ACOUT(4)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_ACOUT(3)_UNCONNECTED , \NLW_blk00000001/blk00000004_ACOUT(2)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_ACOUT(1)_UNCONNECTED , \NLW_blk00000001/blk00000004_ACOUT(0)_UNCONNECTED }),
    .OPMODE({\blk00000001/sig000000b7 , \blk00000001/sig00000147 , \blk00000001/sig000000b7 , \blk00000001/sig00000147 , \blk00000001/sig000000b7 , 
\blk00000001/sig00000147 , \blk00000001/sig000000b7 }),
    .PCIN({\blk00000001/sig000000e7 , \blk00000001/sig000000e6 , \blk00000001/sig000000e5 , \blk00000001/sig000000e4 , \blk00000001/sig000000e3 , 
\blk00000001/sig000000e2 , \blk00000001/sig000000e1 , \blk00000001/sig000000e0 , \blk00000001/sig000000df , \blk00000001/sig000000de , 
\blk00000001/sig000000dd , \blk00000001/sig000000dc , \blk00000001/sig000000db , \blk00000001/sig000000da , \blk00000001/sig000000d9 , 
\blk00000001/sig000000d8 , \blk00000001/sig000000d7 , \blk00000001/sig000000d6 , \blk00000001/sig000000d5 , \blk00000001/sig000000d4 , 
\blk00000001/sig000000d3 , \blk00000001/sig000000d2 , \blk00000001/sig000000d1 , \blk00000001/sig000000d0 , \blk00000001/sig000000cf , 
\blk00000001/sig000000ce , \blk00000001/sig000000cd , \blk00000001/sig000000cc , \blk00000001/sig000000cb , \blk00000001/sig000000ca , 
\blk00000001/sig000000c9 , \blk00000001/sig000000c8 , \blk00000001/sig000000c7 , \blk00000001/sig000000c6 , \blk00000001/sig000000c5 , 
\blk00000001/sig000000c4 , \blk00000001/sig000000c3 , \blk00000001/sig000000c2 , \blk00000001/sig000000c1 , \blk00000001/sig000000c0 , 
\blk00000001/sig000000bf , \blk00000001/sig000000be , \blk00000001/sig000000bd , \blk00000001/sig000000bc , \blk00000001/sig000000bb , 
\blk00000001/sig000000ba , \blk00000001/sig000000b9 , \blk00000001/sig000000b8 }),
    .ALUMODE({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .C({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .CARRYOUT({\NLW_blk00000001/blk00000004_CARRYOUT(3)_UNCONNECTED , \NLW_blk00000001/blk00000004_CARRYOUT(2)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_CARRYOUT(1)_UNCONNECTED , \NLW_blk00000001/blk00000004_CARRYOUT(0)_UNCONNECTED }),
    .INMODE({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .BCIN({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .B({b[31], b[31], b[31], b[31], b[30], b[29], b[28], b[27], b[26], b[25], b[24], b[23], b[22], b[21], b[20], b[19], b[18], b[17]}),
    .BCOUT({\NLW_blk00000001/blk00000004_BCOUT(17)_UNCONNECTED , \NLW_blk00000001/blk00000004_BCOUT(16)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_BCOUT(15)_UNCONNECTED , \NLW_blk00000001/blk00000004_BCOUT(14)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_BCOUT(13)_UNCONNECTED , \NLW_blk00000001/blk00000004_BCOUT(12)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_BCOUT(11)_UNCONNECTED , \NLW_blk00000001/blk00000004_BCOUT(10)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_BCOUT(9)_UNCONNECTED , \NLW_blk00000001/blk00000004_BCOUT(8)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_BCOUT(7)_UNCONNECTED , \NLW_blk00000001/blk00000004_BCOUT(6)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_BCOUT(5)_UNCONNECTED , \NLW_blk00000001/blk00000004_BCOUT(4)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_BCOUT(3)_UNCONNECTED , \NLW_blk00000001/blk00000004_BCOUT(2)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_BCOUT(1)_UNCONNECTED , \NLW_blk00000001/blk00000004_BCOUT(0)_UNCONNECTED }),
    .D({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .P({\NLW_blk00000001/blk00000004_P(47)_UNCONNECTED , \NLW_blk00000001/blk00000004_P(46)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_P(45)_UNCONNECTED , \NLW_blk00000001/blk00000004_P(44)_UNCONNECTED , \NLW_blk00000001/blk00000004_P(43)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_P(42)_UNCONNECTED , \NLW_blk00000001/blk00000004_P(41)_UNCONNECTED , \NLW_blk00000001/blk00000004_P(40)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_P(39)_UNCONNECTED , \NLW_blk00000001/blk00000004_P(38)_UNCONNECTED , \NLW_blk00000001/blk00000004_P(37)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_P(36)_UNCONNECTED , \NLW_blk00000001/blk00000004_P(35)_UNCONNECTED , \NLW_blk00000001/blk00000004_P(34)_UNCONNECTED , 
\NLW_blk00000001/blk00000004_P(33)_UNCONNECTED , \NLW_blk00000001/blk00000004_P(32)_UNCONNECTED , \NLW_blk00000001/blk00000004_P(31)_UNCONNECTED , 
p[64], p[63], p[62], p[61], p[60], p[59], p[58], p[57], p[56], p[55], p[54], p[53], p[52], p[51], p[50], p[49], p[48], p[47], p[46], p[45], p[44], 
p[43], p[42], p[41], p[40], p[39], p[38], p[37], p[36], p[35], p[34]}),
    .A({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , 
\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 }),
    .PCOUT({\blk00000001/sig00000086 , \blk00000001/sig00000087 , \blk00000001/sig00000088 , \blk00000001/sig00000089 , \blk00000001/sig0000008a , 
\blk00000001/sig0000008b , \blk00000001/sig0000008c , \blk00000001/sig0000008d , \blk00000001/sig0000008e , \blk00000001/sig0000008f , 
\blk00000001/sig00000090 , \blk00000001/sig00000091 , \blk00000001/sig00000092 , \blk00000001/sig00000093 , \blk00000001/sig00000094 , 
\blk00000001/sig00000095 , \blk00000001/sig00000096 , \blk00000001/sig00000097 , \blk00000001/sig00000098 , \blk00000001/sig00000099 , 
\blk00000001/sig0000009a , \blk00000001/sig0000009b , \blk00000001/sig0000009c , \blk00000001/sig0000009d , \blk00000001/sig0000009e , 
\blk00000001/sig0000009f , \blk00000001/sig000000a0 , \blk00000001/sig000000a1 , \blk00000001/sig000000a2 , \blk00000001/sig000000a3 , 
\blk00000001/sig000000a4 , \blk00000001/sig000000a5 , \blk00000001/sig000000a6 , \blk00000001/sig000000a7 , \blk00000001/sig000000a8 , 
\blk00000001/sig000000a9 , \blk00000001/sig000000aa , \blk00000001/sig000000ab , \blk00000001/sig000000ac , \blk00000001/sig000000ad , 
\blk00000001/sig000000ae , \blk00000001/sig000000af , \blk00000001/sig000000b0 , \blk00000001/sig000000b1 , \blk00000001/sig000000b2 , 
\blk00000001/sig000000b3 , \blk00000001/sig000000b4 , \blk00000001/sig000000b5 }),
    .ACIN({\blk00000001/sig00000116 , \blk00000001/sig00000115 , \blk00000001/sig00000114 , \blk00000001/sig00000113 , \blk00000001/sig00000112 , 
\blk00000001/sig00000111 , \blk00000001/sig00000110 , \blk00000001/sig0000010f , \blk00000001/sig0000010e , \blk00000001/sig0000010d , 
\blk00000001/sig0000010c , \blk00000001/sig0000010b , \blk00000001/sig0000010a , \blk00000001/sig00000109 , \blk00000001/sig00000108 , 
\blk00000001/sig00000107 , \blk00000001/sig00000106 , \blk00000001/sig00000105 , \blk00000001/sig00000104 , \blk00000001/sig00000103 , 
\blk00000001/sig00000102 , \blk00000001/sig00000101 , \blk00000001/sig00000100 , \blk00000001/sig000000ff , \blk00000001/sig000000fe , 
\blk00000001/sig000000fd , \blk00000001/sig000000fc , \blk00000001/sig000000fb , \blk00000001/sig000000fa , \blk00000001/sig000000f9 }),
    .CARRYINSEL({\blk00000001/sig00000147 , \blk00000001/sig00000147 , \blk00000001/sig00000147 })
  );
  GND   \blk00000001/blk00000003  (
    .G(\blk00000001/sig00000147 )
  );
  VCC   \blk00000001/blk00000002  (
    .P(\blk00000001/sig000000b7 )
  );

// synthesis translate_on

endmodule

// synthesis translate_off

`ifndef GLBL
`define GLBL

`timescale  1 ps / 1 ps

module glbl ();

    parameter ROC_WIDTH = 100000;
    parameter TOC_WIDTH = 0;

//--------   STARTUP Globals --------------
    wire GSR;
    wire GTS;
    wire GWE;
    wire PRLD;
    tri1 p_up_tmp;
    tri (weak1, strong0) PLL_LOCKG = p_up_tmp;

    wire PROGB_GLBL;
    wire CCLKO_GLBL;

    reg GSR_int;
    reg GTS_int;
    reg PRLD_int;

//--------   JTAG Globals --------------
    wire JTAG_TDO_GLBL;
    wire JTAG_TCK_GLBL;
    wire JTAG_TDI_GLBL;
    wire JTAG_TMS_GLBL;
    wire JTAG_TRST_GLBL;

    reg JTAG_CAPTURE_GLBL;
    reg JTAG_RESET_GLBL;
    reg JTAG_SHIFT_GLBL;
    reg JTAG_UPDATE_GLBL;
    reg JTAG_RUNTEST_GLBL;

    reg JTAG_SEL1_GLBL = 0;
    reg JTAG_SEL2_GLBL = 0 ;
    reg JTAG_SEL3_GLBL = 0;
    reg JTAG_SEL4_GLBL = 0;

    reg JTAG_USER_TDO1_GLBL = 1'bz;
    reg JTAG_USER_TDO2_GLBL = 1'bz;
    reg JTAG_USER_TDO3_GLBL = 1'bz;
    reg JTAG_USER_TDO4_GLBL = 1'bz;

    assign (weak1, weak0) GSR = GSR_int;
    assign (weak1, weak0) GTS = GTS_int;
    assign (weak1, weak0) PRLD = PRLD_int;

    initial begin
	GSR_int = 1'b1;
	PRLD_int = 1'b1;
	#(ROC_WIDTH)
	GSR_int = 1'b0;
	PRLD_int = 1'b0;
    end

    initial begin
	GTS_int = 1'b1;
	#(TOC_WIDTH)
	GTS_int = 1'b0;
    end

endmodule

`endif

// synthesis translate_on

//-----------------------------------------------------------------
// System Generator version 14.6 VERILOG source file.
//
// Copyright(C) 2013 by Xilinx, Inc.  All rights reserved.  This
// text/file contains proprietary, confidential information of Xilinx,
// Inc., is distributed under license from Xilinx, Inc., and may be used,
// copied and/or disclosed only pursuant to the terms of a valid license
// agreement with Xilinx, Inc.  Xilinx hereby grants you a license to use
// this text/file solely for design, simulation, implementation and
// creation of design files limited to Xilinx devices or technologies.
// Use with non-Xilinx devices or technologies is expressly prohibited
// and immediately terminates your license unless covered by a separate
// agreement.
//
// Xilinx is providing this design, code, or information "as is" solely
// for use in developing programs and solutions for Xilinx devices.  By
// providing this design, code, or information as one possible
// implementation of this feature, application or standard, Xilinx is
// making no representation that this implementation is free from any
// claims of infringement.  You are responsible for obtaining any rights
// you may require for your implementation.  Xilinx expressly disclaims
// any warranty whatsoever with respect to the adequacy of the
// implementation, including but not limited to warranties of
// merchantability or fitness for a particular purpose.
//
// Xilinx products are not intended for use in life support appliances,
// devices, or systems.  Use in such applications is expressly prohibited.
//
// Any modifications that are made to the source code are done at the user's
// sole risk and will be unsupported.
//
// This copyright and support notice must be retained as part of this
// text at all times.  (c) Copyright 1995-2013 Xilinx, Inc.  All rights
// reserved.
//-----------------------------------------------------------------
// synopsys translate_off
`ifndef simulating
  `define simulating 1
`endif
// synopsys translate_on
`ifndef simulating
  `define simulating 0
`endif
`ifndef xlUnsigned
 `define xlUnsigned 1
`endif
`ifndef xlSigned
 `define xlSigned 2
`endif
`ifndef xlFloat
 `define xlFloat 3
`endif
`ifndef xlWrap
 `define xlWrap 1
`endif
`ifndef xlSaturate
 `define xlSaturate 2
`endif
`ifndef xlTruncate
 `define xlTruncate 1
`endif
`ifndef xlRound
 `define xlRound 2
`endif
`ifndef xlRoundBanker
 `define xlRoundBanker 3
`endif
`ifndef xlAddMode
 `define xlAddMode 1
`endif
`ifndef xlSubMode
 `define xlSubMode 2
`endif
`ifndef xlConvPkgIncluded
 `define xlConvPkgIncluded 1
`endif

//-----------------------------------------------------------------
// System Generator version 14.6 VERILOG source file.
//
// Copyright(C) 2013 by Xilinx, Inc.  All rights reserved.  This
// text/file contains proprietary, confidential information of Xilinx,
// Inc., is distributed under license from Xilinx, Inc., and may be used,
// copied and/or disclosed only pursuant to the terms of a valid license
// agreement with Xilinx, Inc.  Xilinx hereby grants you a license to use
// this text/file solely for design, simulation, implementation and
// creation of design files limited to Xilinx devices or technologies.
// Use with non-Xilinx devices or technologies is expressly prohibited
// and immediately terminates your license unless covered by a separate
// agreement.
//
// Xilinx is providing this design, code, or information "as is" solely
// for use in developing programs and solutions for Xilinx devices.  By
// providing this design, code, or information as one possible
// implementation of this feature, application or standard, Xilinx is
// making no representation that this implementation is free from any
// claims of infringement.  You are responsible for obtaining any rights
// you may require for your implementation.  Xilinx expressly disclaims
// any warranty whatsoever with respect to the adequacy of the
// implementation, including but not limited to warranties of
// merchantability or fitness for a particular purpose.
//
// Xilinx products are not intended for use in life support appliances,
// devices, or systems.  Use in such applications is expressly prohibited.
//
// Any modifications that are made to the source code are done at the user's
// sole risk and will be unsupported.
//
// This copyright and support notice must be retained as part of this
// text at all times.  (c) Copyright 1995-2013 Xilinx, Inc.  All rights
// reserved.
//-----------------------------------------------------------------
`timescale 1 ns / 10 ps
module srl17e (clk, ce, d, q);
    parameter width = 16;
    parameter latency = 8;
    input clk, ce;
    input [width-1:0] d;
    output [width-1:0] q;
    parameter signed [5:0] a = latency - 2;
    wire[width - 1:0] #0.2 d_delayed;
    wire[width - 1:0] srl16_out;
    genvar i;
    assign d_delayed = d ;
    generate
      for(i=0; i<width; i=i+1)
      begin:reg_array
            if (latency > 1)
                begin: has_2_latency
                 SRL16E u1 (.CLK(clk), .D(d_delayed[i]), .Q(srl16_out[i]), .CE(ce), .A0(a[0]), .A1(a[1]), .A2(a[2]), .A3(a[3]));
                end
            if (latency <= 1)
                begin: has_1_latency
                 assign srl16_out[i] = d_delayed[i];
                end
            if (latency != 0)
                begin: has_latency
                 FDE u2 (.C(clk), .D(srl16_out[i]), .Q(q[i]), .CE(ce));
                end
            if (latency == 0)
                begin:has_0_latency
                 assign q[i] = srl16_out[i];
                end
        end
    endgenerate
endmodule
module synth_reg (i, ce, clr, clk, o);
   parameter width  = 8;
   parameter latency  = 1;
   input[width - 1:0] i;
   input ce,clr,clk;
   output[width - 1:0] o;
   parameter complete_num_srl17es = latency/17;
   parameter remaining_latency = latency%17;
   parameter temp_num_srl17es = (latency/17) + ((latency%17)?1:0);
   parameter num_srl17es = temp_num_srl17es ? temp_num_srl17es : 1;
   wire [width - 1:0] z [0:num_srl17es-1];
   genvar t;
    generate
      if (latency <= 17)
          begin:has_only_1
              srl17e #(width, latency) srl17e_array0(.clk(clk), .ce(ce), .d(i), .q(o));
          end
     endgenerate
    generate
     if (latency > 17)
        begin:has_1
             assign o = z[num_srl17es-1];
             srl17e #(width, 17) srl17e_array0(.clk(clk), .ce(ce), .d(i), .q(z[0]));
        end
   endgenerate
   generate
      if (latency > 17)
          begin:more_than_1
              for (t=1; t < complete_num_srl17es; t=t+1)
                begin:left_complete_ones
                   srl17e #(width, 17) srl17e_array(.clk(clk), .ce(ce), .d(z[t-1]), .q(z[t]));
                end
          end
   endgenerate
   generate
     if ((remaining_latency > 0) && (latency>17))
          begin:remaining_ones
             srl17e #(width, (latency%17)) last_srl17e (.clk(clk), .ce(ce), .d(z[num_srl17es-2]), .q(z[num_srl17es-1]));
          end
   endgenerate
endmodule
module synth_reg_reg (i, ce, clr, clk, o);
   parameter width  = 8;
   parameter latency  = 1;
   input[width - 1:0] i;
   input ce, clr, clk;
   output[width - 1:0] o;
   wire[width - 1:0] o;
   genvar idx;
   reg[width - 1:0] reg_bank [latency:0];
   integer j;
   initial
     begin
        for (j=0; j < latency+1; j=j+1)
          begin
             reg_bank[j] = {width{1'b0}};
          end
     end

   generate
     if (latency == 0)
        begin:has_0_latency
         assign o = i;
       end
   endgenerate

   always @(i)
        begin
             reg_bank[0] = i;
         end
   generate
     if (latency > 0)
        begin:more_than_1
            assign o = reg_bank[latency];
         for (idx=0; idx < latency; idx=idx+1) begin:sync_loop
            always @(posedge clk)
                begin
                 if(clr)
                    begin
                      for (j=0; j < latency+1; j=j+1)
                        begin
                          reg_bank[j] = {width{1'b0}};
                       end
                    end
                 else if (ce)
                    begin
                        reg_bank[idx+1] <= reg_bank[idx] ;
                    end
               end
        end
      end
   endgenerate
endmodule

//-----------------------------------------------------------------
// System Generator version 14.6 VERILOG source file.
//
// Copyright(C) 2013 by Xilinx, Inc.  All rights reserved.  This
// text/file contains proprietary, confidential information of Xilinx,
// Inc., is distributed under license from Xilinx, Inc., and may be used,
// copied and/or disclosed only pursuant to the terms of a valid license
// agreement with Xilinx, Inc.  Xilinx hereby grants you a license to use
// this text/file solely for design, simulation, implementation and
// creation of design files limited to Xilinx devices or technologies.
// Use with non-Xilinx devices or technologies is expressly prohibited
// and immediately terminates your license unless covered by a separate
// agreement.
//
// Xilinx is providing this design, code, or information "as is" solely
// for use in developing programs and solutions for Xilinx devices.  By
// providing this design, code, or information as one possible
// implementation of this feature, application or standard, Xilinx is
// making no representation that this implementation is free from any
// claims of infringement.  You are responsible for obtaining any rights
// you may require for your implementation.  Xilinx expressly disclaims
// any warranty whatsoever with respect to the adequacy of the
// implementation, including but not limited to warranties of
// merchantability or fitness for a particular purpose.
//
// Xilinx products are not intended for use in life support appliances,
// devices, or systems.  Use in such applications is expressly prohibited.
//
// Any modifications that are made to the source code are done at the user's
// sole risk and will be unsupported.
//
// This copyright and support notice must be retained as part of this
// text at all times.  (c) Copyright 1995-2013 Xilinx, Inc.  All rights
// reserved.
//-----------------------------------------------------------------
`timescale 1 ns / 10 ps
module synth_reg_w_init (i, ce, clr, clk, o);
   parameter width  = 8;
   parameter init_index  = 0;
   parameter [width-1 : 0] init_value  = 'b0000;
   parameter latency  = 1;

   input[width - 1:0] i;
   input ce, clr, clk;
   output[width - 1:0] o;
   wire[(latency + 1) * width - 1:0] dly_i;
   wire #0.2 dly_clr;
   genvar index;

   generate
     if (latency == 0)
        begin:has_0_latency
         assign o = i;
        end
     else
        begin:has_latency
         assign dly_i[(latency + 1) * width - 1:latency * width] = i ;
         assign dly_clr = clr ;
         for (index=1; index<=latency; index=index+1)
           begin:fd_array
// synopsys translate_off
             defparam reg_comp_1.width = width;
             defparam reg_comp_1.init_index = init_index;
             defparam reg_comp_1.init_value = init_value;
// synopsys translate_on
             single_reg_w_init #(width, init_index, init_value)
               reg_comp_1(.clk(clk),
                          .i(dly_i[(index + 1)*width-1:index*width]),
                          .o(dly_i[index * width - 1:(index - 1) * width]),
                          .ce(ce),
                          .clr(dly_clr));
             end
        assign o = dly_i[width-1:0];
       end
   endgenerate
endmodule
module single_reg_w_init (i, ce, clr, clk, o);
   parameter width  = 8;
   parameter init_index  = 0;
   parameter [width-1 : 0] init_value  = 8'b00000000;
   input[width - 1:0] i;
   input ce;
   input clr;
   input clk;
   output[width - 1:0] o;
   parameter [0:0] init_index_val = (init_index ==  1) ? 1'b1 : 1'b0;
   parameter [width-1:0] result = (width > 1) ? { {(width-1){1'b0}}, init_index_val } : init_index_val;
   parameter [width-1:0] init_const = (init_index > 1) ? init_value : result;
   wire[width - 1:0] o;
   genvar index;

   generate
     for (index=0;index < width; index=index+1) begin:fd_prim_array
          if (init_const[index] == 0)
            begin:rst_comp
              FDRE fdre_comp(.C(clk),
                             .D(i[index]),
                             .Q(o[index]),
                             .CE(ce),
                             .R(clr));
            end
          else
            begin:set_comp
             FDSE fdse_comp(.C(clk),
                            .D(i[index]),
                            .Q(o[index]),
                            .CE(ce),
                            .S(clr));
            end
    end
   endgenerate
endmodule

//-----------------------------------------------------------------
// System Generator version 14.6 VERILOG source file.
//
// Copyright(C) 2013 by Xilinx, Inc.  All rights reserved.  This
// text/file contains proprietary, confidential information of Xilinx,
// Inc., is distributed under license from Xilinx, Inc., and may be used,
// copied and/or disclosed only pursuant to the terms of a valid license
// agreement with Xilinx, Inc.  Xilinx hereby grants you a license to use
// this text/file solely for design, simulation, implementation and
// creation of design files limited to Xilinx devices or technologies.
// Use with non-Xilinx devices or technologies is expressly prohibited
// and immediately terminates your license unless covered by a separate
// agreement.
//
// Xilinx is providing this design, code, or information "as is" solely
// for use in developing programs and solutions for Xilinx devices.  By
// providing this design, code, or information as one possible
// implementation of this feature, application or standard, Xilinx is
// making no representation that this implementation is free from any
// claims of infringement.  You are responsible for obtaining any rights
// you may require for your implementation.  Xilinx expressly disclaims
// any warranty whatsoever with respect to the adequacy of the
// implementation, including but not limited to warranties of
// merchantability or fitness for a particular purpose.
//
// Xilinx products are not intended for use in life support appliances,
// devices, or systems.  Use in such applications is expressly prohibited.
//
// Any modifications that are made to the source code are done at the user's
// sole risk and will be unsupported.
//
// This copyright and support notice must be retained as part of this
// text at all times.  (c) Copyright 1995-2013 Xilinx, Inc.  All rights
// reserved.
//-----------------------------------------------------------------
`ifndef xlConvPkgIncluded
`include "conv_pkg.v"
`endif
`ifndef xlConvertType
 `define xlConvertType
 `timescale 1 ns / 10 ps
module cast (inp, res);
    parameter signed [31:0] old_width = 4;
    parameter signed [31:0] old_bin_pt = 1;
    parameter signed [31:0] new_width = 4;
    parameter signed [31:0] new_bin_pt = 1;
    parameter signed [31:0] new_arith = `xlSigned;
    input [old_width - 1 : 0] inp;
    output [new_width - 1 : 0] res;

   parameter signed [31:0] right_of_dp = new_bin_pt - old_bin_pt;
    wire [new_width-1:0] result;
   genvar i;
   assign res = result;
   generate
      for (i = 0; i<new_width;  i = i+1)
        begin:cast_loop
           if ((i-right_of_dp) > old_width - 1)
             begin:u0
                if (new_arith == `xlUnsigned)
                  begin:u1
                     assign result[i] = 1'b0;
                  end
                if (new_arith == `xlSigned)
                  begin:u2
                     assign result[i] = inp[old_width-1];
                  end
             end
           else if ((i-right_of_dp) >= 0)
             begin:u3
                assign result[i] = inp[i-right_of_dp];
             end
           else
             begin:u4
                assign result[i] = 1'b0;
             end
        end
      endgenerate

endmodule
module shift_division_result (quotient, fraction, res);
    parameter signed [31:0] q_width = 16;
    parameter signed [31:0] f_width = 16;
    parameter signed [31:0] fraction_width = 8;
    parameter signed [31:0] shift_value = 8;
    parameter signed [31:0] shift_dir = 0;
    parameter signed [31:0] vec_MSB = q_width + f_width - 1;
    parameter signed [31:0] result_MSB = q_width + fraction_width - 1;
    parameter signed [31:0] result_LSB = vec_MSB - result_MSB;
    input [q_width - 1 : 0] quotient;
    input [f_width - 1 : 0] fraction;
    output [result_MSB : 0] res;

    wire [vec_MSB:0] vec;
    wire [vec_MSB:0] temp;
   genvar i;
   assign res = vec[vec_MSB:result_LSB];
   assign temp = { quotient, fraction };
   generate
      if (shift_dir == 1)
      begin:left_shift_loop
         for (i = vec_MSB; i>=0;  i = i-1)
         begin:u0
            if (i < shift_value)
            begin:u1
               assign vec[i] = 1'b0;
            end
            else
            begin:u2
               assign vec[i] = temp[i-shift_value];
             end
         end
      end
      else
      begin:right_shift_loop
         for (i = 0; i <= vec_MSB; i = i+1)
         begin:u3
            if (i > vec_MSB - shift_value)
            begin:u4
               assign vec[i] = temp[vec_MSB];
            end
            else
            begin:u5
               assign vec[i] = temp[i+shift_value];
            end
         end
      end
   endgenerate

endmodule
module shift_op (inp, res);
    parameter signed [31:0] inp_width = 16;
    parameter signed [31:0] result_width = 16;
    parameter signed [31:0] shift_value = 8;
    parameter signed [31:0] shift_dir = 0;
    parameter signed [31:0] vec_MSB = inp_width - 1;
    parameter signed [31:0] result_MSB = result_width - 1;
    parameter signed [31:0] result_LSB = vec_MSB - result_MSB;
    input [vec_MSB : 0] inp;
    output [result_MSB : 0] res;

    wire [vec_MSB:0] vec;
   genvar i;
   assign res = vec[vec_MSB:result_LSB];
   generate
      if (shift_dir == 1)
      begin:left_shift_loop
         for (i = vec_MSB; i>=0;  i = i-1)
         begin:u0
            if (i < shift_value)
            begin:u1
               assign vec[i] = 1'b0;
            end
            else
            begin:u2
               assign vec[i] = inp[i-shift_value];
             end
         end
      end
      else
      begin:right_shift_loop
         for (i = 0; i <= vec_MSB; i = i+1)
         begin:u3
            if (i > vec_MSB - shift_value)
            begin:u4
               assign vec[i] = inp[vec_MSB];
            end
            else
            begin:u5
               assign vec[i] = inp[i+shift_value];
            end
         end
      end
   endgenerate

endmodule
module pad_lsb (inp, res);
   parameter signed [31:0] orig_width = 4;
   parameter signed [31:0] new_width = 2;
   input [orig_width - 1 : 0] inp;
   output [new_width - 1 : 0] res;
   parameter signed [31:0] pad_pos = new_width - orig_width -1;
   wire [new_width-1:0] result;
   genvar i;
   assign  res = result;
   generate
      if (new_width >= orig_width)
        begin:u0
           assign result[new_width-1:new_width-orig_width] = inp[orig_width-1:0];
        end
   endgenerate

   generate
      if (pad_pos >= 0)
        begin:u1
           assign result[pad_pos:0] = {pad_pos{1'b0}};
        end
   endgenerate
endmodule
module zero_ext (inp, res);
   parameter signed [31:0]  old_width = 4;
   parameter signed [31:0]  new_width = 2;
   input [old_width - 1 : 0] inp;
   output [new_width - 1 : 0] res;
   wire [new_width-1:0] result;
   genvar i;
   assign res = result;
   generate
     if (new_width > old_width)
       begin:u0
          assign result = { {(new_width-old_width){1'b0}}, inp};
       end
     else
       begin:u1
          assign result[new_width-1:0] = inp[new_width-1:0];
       end
    endgenerate
endmodule
module sign_ext (inp, res);
   parameter signed [31:0]  old_width = 4;
   parameter signed [31:0]  new_width = 2;
   input [old_width - 1 : 0] inp;
   output [new_width - 1 : 0] res;
   wire [new_width-1:0] result;
   assign res = result;
   generate
     if (new_width > old_width)
       begin:u0
          assign result = { {(new_width-old_width){inp[old_width-1]}}, inp};
       end
     else
       begin:u1
          assign result[new_width-1:0] = inp[new_width-1:0];
       end
   endgenerate

endmodule
module extend_msb (inp, res);
    parameter signed [31:0]  old_width = 4;
    parameter signed [31:0]  new_width = 4;
    parameter signed [31:0]  new_arith = `xlSigned;
    input [old_width - 1 : 0] inp;
    output [new_width - 1 : 0] res;
    wire [new_width-1:0] result;
   assign res = result;
   generate
      if (new_arith ==`xlUnsigned)
        begin:u0
           zero_ext # (old_width, new_width)
             em_zero_ext (.inp(inp), .res(result));
        end
      else
        begin:u1
           sign_ext # (old_width, new_width)
             em_sign_ext (.inp(inp), .res(result));
        end
    endgenerate
endmodule
module align_input (inp, res);
    parameter signed [31:0]  old_width = 4;
    parameter signed [31:0]  delta = 1;
    parameter signed [31:0]  new_arith = `xlSigned;
    parameter signed [31:0]  new_width = 4;
    input [old_width - 1 : 0] inp;
    output [new_width - 1 : 0] res;
    parameter signed [31:0]  abs_delta = (delta >= 0) ? (delta) : (-delta);
    wire [new_width-1:0] result;
    wire [(old_width+abs_delta)-1:0] padded_inp;
   assign res = result;
   generate
      if (delta > 0)
        begin:u0
           pad_lsb # (old_width, old_width+delta)
             ai_pad_lsb (.inp(inp), .res(padded_inp));
           extend_msb # (old_width+delta, new_width, new_arith)
             ai_extend_msb (.inp(padded_inp), .res(result));
        end
      else
        begin:u1
           extend_msb # (old_width, new_width, new_arith)
             ai_extend_msb (.inp(inp), .res(result));
        end
   endgenerate
endmodule
module round_towards_inf (inp, res);
    parameter signed [31:0]  old_width = 4;
    parameter signed [31:0]  old_bin_pt = 2;
    parameter signed [31:0]  old_arith = `xlSigned;
    parameter signed [31:0]  new_width = 4;
    parameter signed [31:0]  new_bin_pt = 1;
    parameter signed [31:0]  new_arith = `xlSigned;
    input [old_width - 1 : 0] inp;
    output [new_width - 1 : 0] res;

   parameter signed [31:0]  right_of_dp = old_bin_pt - new_bin_pt;
   parameter signed [31:0]  abs_right_of_dp = (new_bin_pt > old_bin_pt) ? (new_bin_pt-old_bin_pt) : (old_bin_pt - new_bin_pt);
   parameter signed [31:0]  right_of_dp_2 = (right_of_dp >=2) ? right_of_dp-2 : 0;
   parameter signed [31:0]  right_of_dp_1 = (right_of_dp >=1) ? right_of_dp-1 : 0;
   reg [new_width-1:0] one_or_zero;
   wire [new_width-1:0] truncated_val;
   wire signed [new_width-1:0] result_signed;
   wire [abs_right_of_dp+old_width-1 : 0] padded_val;
   initial
     begin
        one_or_zero = {new_width{1'b0}};
     end
   generate
      if (right_of_dp >= 0)
        begin:u0
           if (new_arith ==`xlUnsigned)
             begin:u1
                zero_ext # (old_width-right_of_dp, new_width)
                  rti_zero_ext (.inp(inp[old_width-1:right_of_dp]), .res(truncated_val));
             end
           else
             begin:u2
                sign_ext # (old_width-right_of_dp, new_width)
                  rti_sign_ext (.inp(inp[old_width-1:right_of_dp]), .res(truncated_val));
             end
        end
      else
        begin:u3
           pad_lsb # (old_width, abs_right_of_dp+old_width)
             rti_pad_lsb (.inp(inp), .res(padded_val));
           if (new_arith ==`xlUnsigned)
             begin:u4
                zero_ext # (abs_right_of_dp+old_width, new_width)
                  rti_zero_ext1 (.inp(padded_val), .res(truncated_val));
             end
           else
             begin:u5
                sign_ext # (abs_right_of_dp+old_width, new_width)
                  rti_sign_ext1 (.inp(padded_val), .res(truncated_val));
             end
        end
   endgenerate
   generate
      if (new_arith == `xlSigned)
        begin:u6
           always @(inp)
             begin
                one_or_zero = {new_width{1'b0}};
               if (inp[old_width-1] == 1'b0)
                 begin
                    one_or_zero[0] = 1'b1;
                 end
               if ((right_of_dp >=2) && (right_of_dp <= old_width))
                 begin
                    if(|inp[right_of_dp_2:0] == 1'b1)
                       begin
                          one_or_zero[0] = 1'b1;
                       end
                 end
               if ((right_of_dp >=1) && (right_of_dp <= old_width))
                 begin
                    if(inp[right_of_dp_1] == 1'b0)
                      begin
                         one_or_zero[0] = 1'b0;
                      end
                 end
               else
                 begin
                    one_or_zero[0] = 1'b0;
                 end
             end
             assign result_signed = truncated_val + one_or_zero;
             assign res = result_signed;
        end

      else
        begin:u7
           always @(inp)
             begin
                one_or_zero = {new_width{1'b0}};
               if ((right_of_dp >=1) && (right_of_dp <= old_width))
                 begin
                    one_or_zero[0] = inp[right_of_dp_1];
                 end
             end
             assign res = truncated_val + one_or_zero;
        end
   endgenerate

endmodule
module round_towards_even (inp, res);
    parameter signed [31:0]  old_width = 4;
    parameter signed [31:0]  old_bin_pt = 2;
    parameter signed [31:0]  old_arith = `xlSigned;
    parameter signed [31:0]  new_width = 4;
    parameter signed [31:0]  new_bin_pt = 1;
    parameter signed [31:0]  new_arith = `xlSigned;
    input [old_width - 1 : 0] inp;
    output [new_width - 1 : 0] res;
   parameter signed [31:0]  right_of_dp = old_bin_pt - new_bin_pt;
   parameter signed [31:0]  abs_right_of_dp = (new_bin_pt > old_bin_pt) ? (new_bin_pt-old_bin_pt) : (old_bin_pt - new_bin_pt);
   parameter signed [31:0]  expected_new_width = old_width - right_of_dp + 1;
   reg [new_width-1:0] one_or_zero;
   wire signed [new_width-1:0] result_signed;
   wire [new_width-1:0] truncated_val;
   wire [abs_right_of_dp+old_width-1 : 0] padded_val;
   initial
     begin
      one_or_zero = { new_width{1'b0}};
     end

   generate
      if (right_of_dp >= 0)
        begin:u0
           if (new_arith == `xlUnsigned)
             begin:u1
                zero_ext # (old_width-right_of_dp, new_width)
                            rte_zero_ext (.inp(inp[old_width-1:right_of_dp]), .res(truncated_val));
             end
           else
             begin:u2
                sign_ext # (old_width-right_of_dp, new_width)
                            rte_sign_ext (.inp(inp[old_width-1:right_of_dp]), .res(truncated_val));
             end
        end

      else
        begin:u3
           pad_lsb # (old_width, abs_right_of_dp+old_width)
                            rte_pad_lsb (.inp(inp), .res(padded_val));
           if (new_arith == `xlUnsigned)
             begin:u4
                zero_ext # (abs_right_of_dp+old_width, new_width)
                            rte_zero_ext1 (.inp(padded_val), .res(truncated_val));
             end
           else
             begin:u5
                sign_ext # (abs_right_of_dp+old_width, new_width)
                            rte_sign_ext1 (.inp(padded_val), .res(truncated_val));
             end
        end
   endgenerate

   generate
      if ((right_of_dp ==1) && (right_of_dp <= old_width))
        begin:u6a
           always @(inp)
             begin
                one_or_zero = { new_width{1'b0}};
                if(inp[right_of_dp-1] == 1'b1)
                  begin
                     one_or_zero[0] = inp[right_of_dp];
                  end
                else
                  begin
                     one_or_zero[0] = inp[right_of_dp-1];
                  end
             end
       end
      else if ((right_of_dp >=2) && (right_of_dp <= old_width))
        begin:u6b
           always @(inp)
             begin
                one_or_zero = { new_width{1'b0}};
                if( (inp[right_of_dp-1] == 'b1) && !(|inp[right_of_dp-2:0]) )
                  begin
                     one_or_zero[0] = inp[right_of_dp];
                  end
                else
                  begin
                     one_or_zero[0] = inp[right_of_dp-1];
                  end
             end
       end
      else
        begin:u7
            always @(inp)
             begin
                one_or_zero = { new_width{1'b0}};
             end
        end
   endgenerate

   generate
      if (new_arith == `xlSigned)
        begin:u8
           assign result_signed = truncated_val + one_or_zero;
           assign res = result_signed;
        end
      else
        begin:u9
           assign res = truncated_val + one_or_zero;
        end
   endgenerate

endmodule
module trunc (inp, res);
    parameter signed [31:0]  old_width = 4;
    parameter signed [31:0]  old_bin_pt = 2;
    parameter signed [31:0]  old_arith = `xlSigned;
    parameter signed [31:0]  new_width = 4;
    parameter signed [31:0]  new_bin_pt = 1;
    parameter signed [31:0]  new_arith = `xlSigned;
    input [old_width - 1 : 0] inp;
    output [new_width - 1 : 0] res;

   parameter signed [31:0]  right_of_dp = old_bin_pt - new_bin_pt;
   parameter signed [31:0]  abs_right_of_dp = (new_bin_pt > old_bin_pt) ? (new_bin_pt-old_bin_pt) : (old_bin_pt - new_bin_pt);
   wire [new_width-1:0] result;
   wire [abs_right_of_dp+old_width-1 : 0] padded_val;
   assign res = result;
   generate
      if (new_bin_pt > old_bin_pt)
        begin:tr_u2
           pad_lsb # (old_width, abs_right_of_dp+old_width)
             tr_pad_lsb (.inp(inp), .res(padded_val));
           extend_msb # (old_width+abs_right_of_dp, new_width, new_arith)
             tr_extend_msb (.inp(padded_val), .res(result));
        end
      else
        begin:tr_u1
           extend_msb # (old_width-right_of_dp, new_width, new_arith)
             tr_extend_msb (.inp(inp[old_width-1:right_of_dp]), .res(result));
        end
   endgenerate

endmodule
module saturation_arith (inp, res);
    parameter signed [31:0]  old_width = 4;
    parameter signed [31:0]  old_bin_pt = 2;
    parameter signed [31:0]  old_arith = `xlSigned;
    parameter signed [31:0]  new_width = 4;
    parameter signed [31:0]  new_bin_pt = 1;
    parameter signed [31:0]  new_arith = `xlSigned;
    input [old_width - 1 : 0] inp;
    output [new_width - 1 : 0] res;
   parameter signed [31:0]  abs_right_of_dp = (new_bin_pt > old_bin_pt) ? (new_bin_pt-old_bin_pt) : (old_bin_pt - new_bin_pt);
   parameter signed [31:0]  abs_width = (new_width > old_width) ? (new_width-old_width) : 1;
   parameter signed [31:0]  abs_new_width = (old_width > new_width) ? new_width : 1;
   reg overflow;
   reg [old_width-1:0] vec;
   reg [new_width-1:0] result;
   assign res = result;
   generate
      if (old_width > new_width)
        begin:sa_u0
           always @ (inp)
             begin
                vec = inp;
                overflow = 1;
                if ( (old_arith == `xlSigned) && (new_arith == `xlSigned) )
                  begin
                    if (~|inp[old_width-1:abs_new_width-1] || &inp[old_width-1:abs_new_width-1])
                     begin
                       overflow = 0;
                     end
                 end

                if ( (old_arith == `xlSigned) && (new_arith == `xlUnsigned))
                   begin
                    if (~|inp[old_width-1 : abs_new_width])
                    begin
                       overflow = 0;
                    end
                end

                if ((old_arith == `xlUnsigned) &&  (new_arith == `xlUnsigned))
                  begin
                    if (~|inp[old_width-1 : abs_new_width])
                     begin
                       overflow = 0;
                     end
                  end

               if ( (old_arith == `xlUnsigned) && (new_arith == `xlSigned))
                 begin
                  if (~|inp[old_width-1:abs_new_width-1] || &inp[old_width-1:abs_new_width-1])
                    begin
                      overflow = 0;
                    end
                 end
               if (overflow == 1)
                 begin
                   if (new_arith == `xlSigned)
                     begin
                       if (inp[old_width-1] == 'b0)
                         begin
                           result = (new_width ==1) ? 1'b0 : {1'b0, {(new_width-1){1'b1}} };
                         end
                      else
                        begin
                          result = (new_width ==1) ? 1'b1 : {1'b1, {(new_width-1){1'b0}} };
                       end
                     end
                   else
                     begin
                       if ((old_arith == `xlSigned) && (inp[old_width-1] == 'b1))
                         begin
                           result = {new_width{1'b0}};
                         end
                       else
                         begin
                           result = {new_width{1'b1}};
                         end
                     end
                 end
               else
                 begin
                    if ( (old_arith == `xlSigned) && (new_arith == `xlUnsigned) && (inp[old_width-1] == 'b1) )
                    begin
                      vec = {old_width{1'b0}};
                    end
                    result = vec[new_width-1:0];
                 end
             end
        end
   endgenerate
   generate
      if (new_width > old_width)
        begin:sa_u1
         always @ (inp)
           begin
            vec = inp;
            if ( (old_arith == `xlSigned) && (new_arith == `xlUnsigned) && (inp[old_width-1] == 1'b1) )
              begin
                vec = {old_width{1'b0}};
              end
              if (new_arith == `xlUnsigned)
                begin
                  result = { {abs_width{1'b0}}, vec};
                end
              else
                begin
                  result = { {abs_width{inp[old_width-1]}}, vec};
                end
           end
       end
   endgenerate

   generate
      if (new_width == old_width)
        begin:sa_u2
         always @ (inp)
           begin
             if ( (old_arith == `xlSigned) && (new_arith == `xlUnsigned) && (inp[old_width-1] == 'b1) )
               begin
                 result = {old_width{1'b0}};
               end
             else
               begin
                 result = inp;
               end
           end
        end
   endgenerate

endmodule
module wrap_arith (inp, res);
    parameter signed [31:0]  old_width = 4;
    parameter signed [31:0]  old_bin_pt = 2;
    parameter signed [31:0]  old_arith = `xlSigned;
    parameter signed [31:0]  new_width = 4;
    parameter signed [31:0]  new_bin_pt = 1;
    parameter signed [31:0]  new_arith = `xlSigned;
    parameter signed [31:0]  result_arith = ((old_arith==`xlSigned)&&(new_arith==`xlUnsigned))? `xlSigned : new_arith;
    input [old_width - 1 : 0] inp;
    output [new_width - 1 : 0] res;
   wire [new_width-1:0] result;
   cast # (old_width, old_bin_pt, new_width, new_bin_pt, result_arith)
     wrap_cast (.inp(inp), .res(result));
   assign res = result;

endmodule
module convert_type (inp, res);
    parameter signed [31:0]  old_width = 4;
    parameter signed [31:0]  old_bin_pt = 2;
    parameter signed [31:0]  old_arith = `xlSigned;
    parameter signed [31:0]  new_width = 4;
    parameter signed [31:0]  new_bin_pt = 1;
    parameter signed [31:0]  new_arith = `xlSigned;
    parameter signed [31:0]  quantization = `xlTruncate;
    parameter signed [31:0]  overflow = `xlWrap;
    input [old_width - 1 : 0] inp;
    output [new_width - 1 : 0] res;

   parameter signed [31:0]  fp_width = old_width + 2;
   parameter signed [31:0]  fp_bin_pt = old_bin_pt;
   parameter signed [31:0]  fp_arith = old_arith;
   parameter signed [31:0]  q_width = fp_width + new_bin_pt - old_bin_pt;
   parameter signed [31:0]  q_bin_pt = new_bin_pt;
   parameter signed [31:0]  q_arith = old_arith;
   wire [fp_width-1:0] full_precision_result;
   wire [new_width-1:0] result;
   wire [q_width-1:0] quantized_result;
   assign res = result;
   cast # (old_width, old_bin_pt, fp_width, fp_bin_pt, fp_arith)
     fp_cast (.inp(inp), .res(full_precision_result));
   generate
      if (quantization == `xlRound)
        begin:ct_u0
           round_towards_inf # (fp_width, fp_bin_pt, fp_arith, q_width, q_bin_pt, q_arith)
             quant_rtf (.inp(full_precision_result), .res(quantized_result));
        end
   endgenerate

   generate
      if (quantization == `xlRoundBanker)
        begin:ct_u1
           round_towards_even # (fp_width, fp_bin_pt, fp_arith, q_width, q_bin_pt, q_arith)
             quant_rte (.inp(full_precision_result), .res(quantized_result));
        end
   endgenerate

   generate
      if (quantization == `xlTruncate)
        begin:ct_u2
           trunc # (fp_width, fp_bin_pt, fp_arith, q_width, q_bin_pt, q_arith)
             quant_tr (.inp(full_precision_result), .res(quantized_result));
        end
   endgenerate

   generate
      if (overflow == `xlSaturate)
        begin:ct_u3
           saturation_arith # (q_width, q_bin_pt, q_arith, new_width, new_bin_pt, new_arith)
            ovflo_sat (.inp(quantized_result), .res(result));
        end
   endgenerate

   generate
      if ((overflow == `xlWrap) || (overflow == 3))
        begin:ct_u4
           wrap_arith # (q_width, q_bin_pt, q_arith, new_width, new_bin_pt, new_arith)
             ovflo_wrap (.inp(quantized_result), .res(result));
        end
   endgenerate

endmodule
`endif

//-----------------------------------------------------------------
// System Generator version 14.6 VERILOG source file.
//
// Copyright(C) 2013 by Xilinx, Inc.  All rights reserved.  This
// text/file contains proprietary, confidential information of Xilinx,
// Inc., is distributed under license from Xilinx, Inc., and may be used,
// copied and/or disclosed only pursuant to the terms of a valid license
// agreement with Xilinx, Inc.  Xilinx hereby grants you a license to use
// this text/file solely for design, simulation, implementation and
// creation of design files limited to Xilinx devices or technologies.
// Use with non-Xilinx devices or technologies is expressly prohibited
// and immediately terminates your license unless covered by a separate
// agreement.
//
// Xilinx is providing this design, code, or information "as is" solely
// for use in developing programs and solutions for Xilinx devices.  By
// providing this design, code, or information as one possible
// implementation of this feature, application or standard, Xilinx is
// making no representation that this implementation is free from any
// claims of infringement.  You are responsible for obtaining any rights
// you may require for your implementation.  Xilinx expressly disclaims
// any warranty whatsoever with respect to the adequacy of the
// implementation, including but not limited to warranties of
// merchantability or fitness for a particular purpose.
//
// Xilinx products are not intended for use in life support appliances,
// devices, or systems.  Use in such applications is expressly prohibited.
//
// Any modifications that are made to the source code are done at the user's
// sole risk and will be unsupported.
//
// This copyright and support notice must be retained as part of this
// text at all times.  (c) Copyright 1995-2013 Xilinx, Inc.  All rights
// reserved.
//-----------------------------------------------------------------
module xlconvert(din, clk, ce, clr, en, dout);
   parameter din_width= 16;
   parameter din_bin_pt= 4;
   parameter din_arith= `xlUnsigned;
   parameter dout_width= 8;
   parameter dout_bin_pt= 2;
   parameter dout_arith= `xlUnsigned;
   parameter en_width = 1;
   parameter en_bin_pt = 0;
   parameter en_arith = `xlUnsigned;
   parameter bool_conversion = 0;
   parameter latency = 0;
   parameter quantization= `xlTruncate;
   parameter overflow= `xlWrap;
   input [din_width-1:0] din;
   input clk, ce, clr;
   input [en_width-1:0] en;
   output [dout_width-1:0] dout;
   wire [dout_width-1:0]   result;
   wire internal_ce;
   assign internal_ce = ce & en[0];

generate
 if (bool_conversion == 1)
    begin:bool_converion_generate
       assign result = din;
    end
 else
    begin:std_conversion
       convert_type #(din_width,
                      din_bin_pt,
                      din_arith,
                              dout_width,
                      dout_bin_pt,
                      dout_arith,
                      quantization,
                      overflow)
        conv_udp (.inp(din), .res(result));
    end
endgenerate
generate
if (latency > 0)
     begin:latency_test
        synth_reg # (dout_width, latency)
          reg1 (
               .i(result),
               .ce(internal_ce),
               .clr(clr),
               .clk(clk),
               .o(dout));
     end
else
     begin:latency0
        assign dout = result;
     end
endgenerate
endmodule

//-----------------------------------------------------------------
// System Generator version 14.6 VERILOG source file.
//
// Copyright(C) 2013 by Xilinx, Inc.  All rights reserved.  This
// text/file contains proprietary, confidential information of Xilinx,
// Inc., is distributed under license from Xilinx, Inc., and may be used,
// copied and/or disclosed only pursuant to the terms of a valid license
// agreement with Xilinx, Inc.  Xilinx hereby grants you a license to use
// this text/file solely for design, simulation, implementation and
// creation of design files limited to Xilinx devices or technologies.
// Use with non-Xilinx devices or technologies is expressly prohibited
// and immediately terminates your license unless covered by a separate
// agreement.
//
// Xilinx is providing this design, code, or information "as is" solely
// for use in developing programs and solutions for Xilinx devices.  By
// providing this design, code, or information as one possible
// implementation of this feature, application or standard, Xilinx is
// making no representation that this implementation is free from any
// claims of infringement.  You are responsible for obtaining any rights
// you may require for your implementation.  Xilinx expressly disclaims
// any warranty whatsoever with respect to the adequacy of the
// implementation, including but not limited to warranties of
// merchantability or fitness for a particular purpose.
//
// Xilinx products are not intended for use in life support appliances,
// devices, or systems.  Use in such applications is expressly prohibited.
//
// Any modifications that are made to the source code are done at the user's
// sole risk and will be unsupported.
//
// This copyright and support notice must be retained as part of this
// text at all times.  (c) Copyright 1995-2013 Xilinx, Inc.  All rights
// reserved.
//-----------------------------------------------------------------
module xldelay #(parameter width = -1, latency = -1, reg_retiming = 0, reset = 0)
  (input [width-1:0] d,
   input ce, clk, en, rst,
   output [width-1:0] q);
generate
  if ((latency == 0) || ((reg_retiming == 0) && (reset == 0)))
  begin:srl_delay
    synth_reg # (width, latency)
      reg1 (
        .i(d),
        .ce(ce & en),
        .clr(1'b0),
        .clk(clk),
        .o(q));
  end

  if ((latency>=1) && ((reg_retiming) || (reset)))
  begin:reg_delay
    synth_reg_reg # (width, latency)
      reg2 (
        .i(d),
        .ce(ce & en),
        .clr(rst),
        .clk(clk),
        .o(q));
  end
endgenerate
endmodule


module inverter_33a63b558a (
  input [(1 - 1):0] ip,
  output [(1 - 1):0] op,
  input clk,
  input ce,
  input clr);
  wire ip_1_26;
  reg op_mem_22_20[0:(1 - 1)];
  initial
    begin
      op_mem_22_20[0] = 1'b0;
    end
  wire op_mem_22_20_front_din;
  wire op_mem_22_20_back;
  wire op_mem_22_20_push_front_pop_back_en;
  localparam [(1 - 1):0] const_value = 1'b1;
  wire internal_ip_12_1_bitnot;
  assign ip_1_26 = ip;
  assign op_mem_22_20_back = op_mem_22_20[0];
  always @(posedge clk)
    begin:proc_op_mem_22_20
      integer i;
      if (((ce == 1'b1) && (op_mem_22_20_push_front_pop_back_en == 1'b1)))
        begin
          op_mem_22_20[0] <= op_mem_22_20_front_din;
        end
    end
  assign internal_ip_12_1_bitnot = ~ip_1_26;
  assign op_mem_22_20_push_front_pop_back_en = 1'b0;
  assign op = internal_ip_12_1_bitnot;
endmodule
 



module logical_4340034b97 (
  input [(1 - 1):0] d0,
  input [(1 - 1):0] d1,
  output [(1 - 1):0] y,
  input clk,
  input ce,
  input clr);
  wire d0_1_24;
  wire d1_1_27;
  reg latency_pipe_5_26[0:(1 - 1)];
  initial
    begin
      latency_pipe_5_26[0] = 1'b0;
    end
  wire latency_pipe_5_26_front_din;
  wire latency_pipe_5_26_back;
  wire latency_pipe_5_26_push_front_pop_back_en;
  wire fully_2_1_bit;
  assign d0_1_24 = d0;
  assign d1_1_27 = d1;
  assign latency_pipe_5_26_back = latency_pipe_5_26[0];
  always @(posedge clk)
    begin:proc_latency_pipe_5_26
      integer i;
      if (((ce == 1'b1) && (latency_pipe_5_26_push_front_pop_back_en == 1'b1)))
        begin
          latency_pipe_5_26[0] <= latency_pipe_5_26_front_din;
        end
    end
  assign fully_2_1_bit = d0_1_24 | d1_1_27;
  assign latency_pipe_5_26_front_din = fully_2_1_bit;
  assign latency_pipe_5_26_push_front_pop_back_en = 1'b1;
  assign y = latency_pipe_5_26_back;
endmodule
 



module logical_f48f811944 (
  input [(1 - 1):0] d0,
  input [(1 - 1):0] d1,
  output [(1 - 1):0] y,
  input clk,
  input ce,
  input clr);
  wire d0_1_24;
  wire d1_1_27;
  reg latency_pipe_5_26[0:(1 - 1)];
  initial
    begin
      latency_pipe_5_26[0] = 1'b0;
    end
  wire latency_pipe_5_26_front_din;
  wire latency_pipe_5_26_back;
  wire latency_pipe_5_26_push_front_pop_back_en;
  wire fully_2_1_bit;
  assign d0_1_24 = d0;
  assign d1_1_27 = d1;
  assign latency_pipe_5_26_back = latency_pipe_5_26[0];
  always @(posedge clk)
    begin:proc_latency_pipe_5_26
      integer i;
      if (((ce == 1'b1) && (latency_pipe_5_26_push_front_pop_back_en == 1'b1)))
        begin
          latency_pipe_5_26[0] <= latency_pipe_5_26_front_din;
        end
    end
  assign fully_2_1_bit = d0_1_24 & d1_1_27;
  assign latency_pipe_5_26_front_din = fully_2_1_bit;
  assign latency_pipe_5_26_push_front_pop_back_en = 1'b1;
  assign y = latency_pipe_5_26_back;
endmodule
 


//-----------------------------------------------------------------
// System Generator version 14.6 VERILOG source file.
//
// Copyright(C) 2013 by Xilinx, Inc.  All rights reserved.  This
// text/file contains proprietary, confidential information of Xilinx,
// Inc., is distributed under license from Xilinx, Inc., and may be used,
// copied and/or disclosed only pursuant to the terms of a valid license
// agreement with Xilinx, Inc.  Xilinx hereby grants you a license to use
// this text/file solely for design, simulation, implementation and
// creation of design files limited to Xilinx devices or technologies.
// Use with non-Xilinx devices or technologies is expressly prohibited
// and immediately terminates your license unless covered by a separate
// agreement.
//
// Xilinx is providing this design, code, or information "as is" solely
// for use in developing programs and solutions for Xilinx devices.  By
// providing this design, code, or information as one possible
// implementation of this feature, application or standard, Xilinx is
// making no representation that this implementation is free from any
// claims of infringement.  You are responsible for obtaining any rights
// you may require for your implementation.  Xilinx expressly disclaims
// any warranty whatsoever with respect to the adequacy of the
// implementation, including but not limited to warranties of
// merchantability or fitness for a particular purpose.
//
// Xilinx products are not intended for use in life support appliances,
// devices, or systems.  Use in such applications is expressly prohibited.
//
// Any modifications that are made to the source code are done at the user's
// sole risk and will be unsupported.
//
// This copyright and support notice must be retained as part of this
// text at all times.  (c) Copyright 1995-2013 Xilinx, Inc.  All rights
// reserved.
//-----------------------------------------------------------------
module xlmult_sysgen_mm_cal (a, b, ce, clr, clk, core_ce, core_clr,core_clk, rst, en,p);
    parameter core_name0 = "";
    parameter a_width = 4;
    parameter a_bin_pt = 2;
    parameter a_arith = `xlSigned;
    parameter b_width = 4;
    parameter b_bin_pt = 1;
    parameter b_arith = `xlSigned;
    parameter p_width = 8;
    parameter p_bin_pt = 2;
    parameter p_arith = `xlSigned;
    parameter rst_width = 1;
    parameter rst_bin_pt = 0;
    parameter rst_arith = `xlUnsigned;
    parameter en_width = 1;
    parameter en_bin_pt = 0;
    parameter en_arith = `xlUnsigned;
    parameter quantization = `xlTruncate;
    parameter overflow = `xlWrap;
    parameter extra_registers = 0;
    parameter c_a_width = 7;
    parameter c_b_width = 7;
    parameter c_type = 0;
    parameter c_a_type = 0;
    parameter c_b_type = 0;
    parameter c_baat = 4;
    parameter oversample = 1;
    parameter multsign = `xlSigned;
    parameter c_output_width = 16;
    input [a_width - 1 : 0] a;
    input [b_width - 1 : 0] b;
    input ce, clr, clk;
    input core_ce, core_clr, core_clk;
    input en, rst;
    output [p_width - 1 : 0] p;
    wire [c_a_width - 1 : 0]    tmp_a, conv_a;
    wire [c_b_width - 1 : 0]    tmp_b, conv_b;
   wire [c_output_width - 1 : 0] tmp_p;
   wire [p_width - 1 : 0] conv_p;
   wire internal_ce, internal_clr, internal_core_ce;
   wire rfd, rdy, nd;

   assign internal_ce = ce & en;
   assign internal_core_ce = core_ce & en;
   assign internal_clr = (clr | rst) & en;
   assign nd = ce & en;
   zero_ext # (a_width, c_a_width) zero_ext_a (.inp(a), .res(tmp_a));
   zero_ext # (b_width, c_b_width) zero_ext_b (.inp(b), .res(tmp_b));
   convert_type # (c_output_width, a_bin_pt+b_bin_pt, multsign,
                   p_width, p_bin_pt, p_arith, quantization, overflow)
     conv_udp (.inp(tmp_p), .res(conv_p));

generate
  if (core_name0 == "mult_11_2_5808000c9562bf6b")
    begin:comp0
    mult_11_2_5808000c9562bf6b core_instance0 (
        .a(tmp_a),
        .b(tmp_b),
        .clk(clk),
        .ce(internal_ce),
        .sclr(internal_clr),
        .p(tmp_p)
      );
  end
   if (extra_registers > 0)
     begin:latency_gt_0
        synth_reg # (p_width, extra_registers)
          reg1 (
               .i(conv_p),
               .ce(internal_ce),
               .clr(internal_clr),
               .clk(clk),
               .o(p));
     end

   if (extra_registers == 0)
     begin:latency_eq_0
        assign p = conv_p;
     end
endgenerate
endmodule

//-----------------------------------------------------------------
// System Generator version 14.6 VERILOG source file.
//
// Copyright(C) 2013 by Xilinx, Inc.  All rights reserved.  This
// text/file contains proprietary, confidential information of Xilinx,
// Inc., is distributed under license from Xilinx, Inc., and may be used,
// copied and/or disclosed only pursuant to the terms of a valid license
// agreement with Xilinx, Inc.  Xilinx hereby grants you a license to use
// this text/file solely for design, simulation, implementation and
// creation of design files limited to Xilinx devices or technologies.
// Use with non-Xilinx devices or technologies is expressly prohibited
// and immediately terminates your license unless covered by a separate
// agreement.
//
// Xilinx is providing this design, code, or information "as is" solely
// for use in developing programs and solutions for Xilinx devices.  By
// providing this design, code, or information as one possible
// implementation of this feature, application or standard, Xilinx is
// making no representation that this implementation is free from any
// claims of infringement.  You are responsible for obtaining any rights
// you may require for your implementation.  Xilinx expressly disclaims
// any warranty whatsoever with respect to the adequacy of the
// implementation, including but not limited to warranties of
// merchantability or fitness for a particular purpose.
//
// Xilinx products are not intended for use in life support appliances,
// devices, or systems.  Use in such applications is expressly prohibited.
//
// Any modifications that are made to the source code are done at the user's
// sole risk and will be unsupported.
//
// This copyright and support notice must be retained as part of this
// text at all times.  (c) Copyright 1995-2013 Xilinx, Inc.  All rights
// reserved.
//-----------------------------------------------------------------
`timescale 1 ns / 10 ps
module xlregister (d, rst, en, ce, clk, q);
   parameter d_width = 5;
   parameter init_value = 'b0;

   input [d_width-1:0] d;
   input rst, en, ce, clk;
   output [d_width-1:0] q;
   wire internal_clr, internal_ce;
   assign internal_clr = rst & ce;
   assign internal_ce  = ce & en;

   synth_reg_w_init #(.width(d_width),
                      .init_index(2),
                      .init_value(init_value),
                      .latency(1))
   synth_reg_inst(.i(d),
                  .ce(internal_ce),
                  .clr(internal_clr),
                  .clk(clk),
                  .o(q));
endmodule


module relational_cd44469155 (
  input [(33 - 1):0] a,
  input [(32 - 1):0] b,
  output [(1 - 1):0] op,
  input clk,
  input ce,
  input clr);
  wire signed [(33 - 1):0] a_1_31;
  wire signed [(32 - 1):0] b_1_34;
  reg op_mem_32_22[0:(1 - 1)];
  initial
    begin
      op_mem_32_22[0] = 1'b0;
    end
  wire op_mem_32_22_front_din;
  wire op_mem_32_22_back;
  wire op_mem_32_22_push_front_pop_back_en;
  localparam [(1 - 1):0] const_value = 1'b1;
  wire signed [(33 - 1):0] cast_18_16;
  wire result_18_3_rel;
  assign a_1_31 = a;
  assign b_1_34 = b;
  assign op_mem_32_22_back = op_mem_32_22[0];
  always @(posedge clk)
    begin:proc_op_mem_32_22
      integer i;
      if (((ce == 1'b1) && (op_mem_32_22_push_front_pop_back_en == 1'b1)))
        begin
          op_mem_32_22[0] <= op_mem_32_22_front_din;
        end
    end
  assign cast_18_16 = {{1{b_1_34[31]}}, b_1_34[31:0]};
  assign result_18_3_rel = a_1_31 > cast_18_16;
  assign op_mem_32_22_front_din = result_18_3_rel;
  assign op_mem_32_22_push_front_pop_back_en = 1'b1;
  assign op = op_mem_32_22_back;
endmodule
 



module relational_6f290671ba (
  input [(33 - 1):0] a,
  input [(32 - 1):0] b,
  output [(1 - 1):0] op,
  input clk,
  input ce,
  input clr);
  wire signed [(33 - 1):0] a_1_31;
  wire signed [(32 - 1):0] b_1_34;
  reg op_mem_32_22[0:(1 - 1)];
  initial
    begin
      op_mem_32_22[0] = 1'b0;
    end
  wire op_mem_32_22_front_din;
  wire op_mem_32_22_back;
  wire op_mem_32_22_push_front_pop_back_en;
  localparam [(1 - 1):0] const_value = 1'b1;
  wire signed [(33 - 1):0] cast_16_16;
  wire result_16_3_rel;
  assign a_1_31 = a;
  assign b_1_34 = b;
  assign op_mem_32_22_back = op_mem_32_22[0];
  always @(posedge clk)
    begin:proc_op_mem_32_22
      integer i;
      if (((ce == 1'b1) && (op_mem_32_22_push_front_pop_back_en == 1'b1)))
        begin
          op_mem_32_22[0] <= op_mem_32_22_front_din;
        end
    end
  assign cast_16_16 = {{1{b_1_34[31]}}, b_1_34[31:0]};
  assign result_16_3_rel = a_1_31 < cast_16_16;
  assign op_mem_32_22_front_din = result_16_3_rel;
  assign op_mem_32_22_push_front_pop_back_en = 1'b1;
  assign op = op_mem_32_22_back;
endmodule
 



module constant_6e9c75157d (
  output [(32 - 1):0] op,
  input clk,
  input ce,
  input clr);
  assign op = 32'b01000000000000000000000000000000;
endmodule
 


//-----------------------------------------------------------------
// System Generator version 14.6 VERILOG source file.
//
// Copyright(C) 2013 by Xilinx, Inc.  All rights reserved.  This
// text/file contains proprietary, confidential information of Xilinx,
// Inc., is distributed under license from Xilinx, Inc., and may be used,
// copied and/or disclosed only pursuant to the terms of a valid license
// agreement with Xilinx, Inc.  Xilinx hereby grants you a license to use
// this text/file solely for design, simulation, implementation and
// creation of design files limited to Xilinx devices or technologies.
// Use with non-Xilinx devices or technologies is expressly prohibited
// and immediately terminates your license unless covered by a separate
// agreement.
//
// Xilinx is providing this design, code, or information "as is" solely
// for use in developing programs and solutions for Xilinx devices.  By
// providing this design, code, or information as one possible
// implementation of this feature, application or standard, Xilinx is
// making no representation that this implementation is free from any
// claims of infringement.  You are responsible for obtaining any rights
// you may require for your implementation.  Xilinx expressly disclaims
// any warranty whatsoever with respect to the adequacy of the
// implementation, including but not limited to warranties of
// merchantability or fitness for a particular purpose.
//
// Xilinx products are not intended for use in life support appliances,
// devices, or systems.  Use in such applications is expressly prohibited.
//
// Any modifications that are made to the source code are done at the user's
// sole risk and will be unsupported.
//
// This copyright and support notice must be retained as part of this
// text at all times.  (c) Copyright 1995-2013 Xilinx, Inc.  All rights
// reserved.
//-----------------------------------------------------------------
 module xladdsub_sysgen_mm_cal (a, b, c_in, ce, clr, clk, rst, en, c_out, s);
parameter core_name0= "";
parameter a_width= 16;
parameter signed a_bin_pt= 4;
parameter a_arith= `xlUnsigned;
parameter c_in_width= 16;
parameter c_in_bin_pt= 4;
parameter c_in_arith= `xlUnsigned;
parameter c_out_width= 16;
parameter c_out_bin_pt= 4;
parameter c_out_arith= `xlUnsigned;
parameter b_width= 8;
parameter signed b_bin_pt= 2;
parameter b_arith= `xlUnsigned;
parameter s_width= 17;
parameter s_bin_pt= 4;
parameter s_arith= `xlUnsigned;
parameter rst_width= 1;
parameter rst_bin_pt= 0;
parameter rst_arith= `xlUnsigned;
parameter en_width= 1;
parameter en_bin_pt= 0;
parameter en_arith= `xlUnsigned;
parameter full_s_width= 17;
parameter full_s_arith= `xlUnsigned;
parameter mode= `xlAddMode;
parameter extra_registers= 0;
parameter latency= 0;
parameter quantization= `xlTruncate;
parameter overflow= `xlWrap;
parameter c_a_width= 16;
parameter c_b_width= 8;
parameter c_a_type= 1;
parameter c_b_type= 1;
parameter c_has_sclr= 0;
parameter c_has_ce= 0;
parameter c_latency= 0;
parameter c_output_width= 17;
parameter c_enable_rlocs= 1;
parameter c_has_c_in= 0;
parameter c_has_c_out= 0;
input [a_width-1:0] a;
input [b_width-1:0] b;
input c_in, ce, clr, clk, rst, en;
output c_out;
output [s_width-1:0] s;
parameter full_a_width = full_s_width;
parameter full_b_width = full_s_width;
parameter full_s_bin_pt = (a_bin_pt > b_bin_pt) ? a_bin_pt : b_bin_pt;
wire [full_a_width-1:0] full_a;
wire [full_b_width-1:0] full_b;
wire [full_s_width-1:0] full_s;
wire [full_s_width-1:0] core_s;
wire [s_width-1:0] conv_s;
wire  temp_cout;
wire  real_a,real_b,real_s;
wire  internal_clr;
wire  internal_ce;
wire  extra_reg_ce;
wire  override;
wire  logic1;
wire  temp_cin;
assign internal_clr = (clr | rst) & ce;
assign internal_ce = ce & en;
assign logic1 = 1'b1;
assign temp_cin = (c_has_c_in) ? c_in : 1'b0;
align_input # (a_width, b_bin_pt - a_bin_pt, a_arith, full_a_width)
align_inp_a(.inp(a),.res(full_a));
align_input # (b_width, a_bin_pt - b_bin_pt, b_arith, full_b_width)
align_inp_b(.inp(b),.res(full_b));
convert_type # (full_s_width, full_s_bin_pt, full_s_arith, s_width,
                s_bin_pt, s_arith, quantization, overflow)
conv_typ_s(.inp(core_s),.res(conv_s));
generate
  if (core_name0 == "addsb_11_0_c5cb2be34e3bf19d")
    begin:comp0
    addsb_11_0_c5cb2be34e3bf19d core_instance0 (
         .a(full_a),
         .s(core_s),
         .b(full_b)
      );
  end

  if (core_name0 == "addsb_11_0_61bea11e4a7792d3")
    begin:comp1
    addsb_11_0_61bea11e4a7792d3 core_instance1 (
         .a(full_a),
         .clk(clk),
         .ce(internal_ce),
         .s(core_s),
         .b(full_b)
      );
  end

endgenerate
generate
  if (extra_registers > 0)
  begin:latency_test

    if (c_latency > 1)
    begin:override_test
      synth_reg # (1, c_latency)
        override_pipe (
          .i(logic1),
          .ce(internal_ce),
          .clr(internal_clr),
          .clk(clk),
          .o(override));
      assign extra_reg_ce = ce & en & override;
    end
    if ((c_latency == 0) || (c_latency == 1))
    begin:no_override
      assign extra_reg_ce = ce & en;
    end
    synth_reg # (s_width, extra_registers)
      extra_reg (
        .i(conv_s),
        .ce(extra_reg_ce),
        .clr(internal_clr),
        .clk(clk),
        .o(s));
    if (c_has_c_out == 1)
    begin:cout_test
      synth_reg # (1, extra_registers)
        c_out_extra_reg (
          .i(temp_cout),
          .ce(extra_reg_ce),
          .clr(internal_clr),
          .clk(clk),
          .o(c_out));
    end

  end
endgenerate
generate
  if ((latency == 0) || (extra_registers == 0))
  begin:latency_s
    assign s = conv_s;
  end
endgenerate
generate
  if (((latency == 0) || (extra_registers == 0)) &&
      (c_has_c_out == 1))
  begin:latency0
    assign c_out = temp_cout;
  end
endgenerate
generate
  if (c_has_c_out == 0)
  begin:tie_dangling_cout
    assign c_out = 0;
  end
endgenerate
endmodule


module mux_40689d2172 (
  input [(1 - 1):0] sel,
  input [(33 - 1):0] d0,
  input [(32 - 1):0] d1,
  output [(33 - 1):0] y,
  input clk,
  input ce,
  input clr);
  wire sel_1_20;
  wire [(33 - 1):0] d0_1_24;
  wire [(32 - 1):0] d1_1_27;
  wire [(1 - 1):0] sel_internal_2_1_convert;
  reg [(33 - 1):0] unregy_join_6_1;
  assign sel_1_20 = sel;
  assign d0_1_24 = d0;
  assign d1_1_27 = d1;
  assign sel_internal_2_1_convert = {sel_1_20};
  always @(d0_1_24 or d1_1_27 or sel_internal_2_1_convert)
    begin:proc_switch_6_1
      case (sel_internal_2_1_convert)
        1'b0 :
          begin
            unregy_join_6_1 = d0_1_24;
          end
        default:
          begin
            unregy_join_6_1 = {{1{d1_1_27[31]}}, d1_1_27[31:0]};
          end
      endcase
    end
  assign y = unregy_join_6_1;
endmodule
 

// Generated from Simulink block "sysgen_mm_cal"

(* core_generation_info = "sysgen_mm_cal,sysgen_core,{clock_period=10.00000000,clocking=Clock_Enables,compilation=NGC_Netlist,sample_periods=1.00000000000,testbench=0,total_blocks=66,xilinx_adder_subtracter_block=2,xilinx_arithmetic_relational_operator_block=2,xilinx_bus_multiplexer_block=1,xilinx_constant_block_block=1,xilinx_delay_block=4,xilinx_gateway_in_block=10,xilinx_gateway_out_block=9,xilinx_inverter_block=1,xilinx_logical_block_block=2,xilinx_multiplier_block=1,xilinx_register_block=10,xilinx_resource_estimator_block=1,xilinx_system_generator_block=1,xilinx_type_converter_block=1,}" *)
module sysgen_mm_cal (
  cal_enable,
  ce_1,
  clk_1,
  datavalid,
  enable,
  h_out,
  hi_limit,
  il,
  il_latched_o,
  il_valid,
  l_out,
  low_limit,
  nm_err_s32_o,
  nm_o,
  rst,
  s_i,
  s_o,
  sel_angle_mode,
  x1_nm_i,
  x1_nm_i1,
  x2_nm_i
);

  input [0:0] cal_enable;
  input [0:0] ce_1;
  input [0:0] clk_1;
  input [0:0] enable;
  input [31:0] hi_limit;
  input [31:0] low_limit;
  input [0:0] rst;
  input [31:0] s_i;
  input [0:0] sel_angle_mode;
  input [31:0] x1_nm_i;
  input [31:0] x1_nm_i1;
  input [31:0] x2_nm_i;
  output [0:0] datavalid;
  output [0:0] h_out;
  output [0:0] il;
  output [0:0] il_latched_o;
  output [0:0] il_valid;
  output [0:0] l_out;
  output [31:0] nm_err_s32_o;
  output [32:0] nm_o;
  output [31:0] s_o;

  wire [32:0] add_s_net;
  wire [0:0] cal_enable_net;
  wire [0:0] ce_1_sg_x0;
  wire [0:0] clk_1_sg_x0;
  wire [31:0] convert_dout_net;
  wire [0:0] datavalid_net;
  wire [0:0] delay1_q_net;
  wire [0:0] delay2_q_net;
  wire [0:0] enable_net;
  wire [0:0] h_out_net;
  wire [31:0] hi_limit_net;
  wire [0:0] il_latched_o_net;
  wire [0:0] il_net;
  wire [0:0] il_valid_net;
  wire [0:0] inverter_op_net;
  wire [0:0] l_out_net;
  wire [0:0] logical1_y_net;
  wire [31:0] low_limit_net;
  wire [31:0] mult1_p_net;
  wire [32:0] mux_y_net;
  wire [31:0] nm_err_s32_o_net;
  wire [32:0] nm_o_net;
  wire [31:0] register1_q_net;
  wire [31:0] register2_q_net;
  wire [31:0] register3_q_net;
  wire [31:0] register4_q_net;
  wire [0:0] register5_q_net;
  wire [31:0] register9_q_net;
  wire [31:0] register_q_net;
  wire [0:0] rst_net;
  wire [31:0] s_i_net;
  wire [31:0] s_o_net;
  wire [0:0] sel_angle_mode_net;
  wire [32:0] sub_s_net;
  wire [31:0] x1_nm_i1_net;
  wire [31:0] x1_nm_i_net;
  wire [31:0] x2_nm_i_net;

  assign cal_enable_net = cal_enable;
  assign ce_1_sg_x0 = ce_1;
  assign clk_1_sg_x0 = clk_1;
  assign datavalid = datavalid_net;
  assign enable_net = enable;
  assign h_out = h_out_net;
  assign hi_limit_net = hi_limit;
  assign il = il_net;
  assign il_latched_o = il_latched_o_net;
  assign il_valid = il_valid_net;
  assign l_out = l_out_net;
  assign low_limit_net = low_limit;
  assign nm_err_s32_o = nm_err_s32_o_net;
  assign nm_o = nm_o_net;
  assign rst_net = rst;
  assign s_i_net = s_i;
  assign s_o = s_o_net;
  assign sel_angle_mode_net = sel_angle_mode;
  assign x1_nm_i_net = x1_nm_i;
  assign x1_nm_i1_net = x1_nm_i1;
  assign x2_nm_i_net = x2_nm_i;


  xladdsub_sysgen_mm_cal #(

    .a_arith(`xlSigned),
    .a_bin_pt(0),
    .a_width(32),
    .b_arith(`xlSigned),
    .b_bin_pt(0),
    .b_width(32),
    .c_has_c_out(0),
    .c_latency(1),
    .c_output_width(33),
    .core_name0("addsb_11_0_61bea11e4a7792d3"),
    .extra_registers(0),
    .full_s_arith(2),
    .full_s_width(33),
    .latency(1),
    .overflow(1),
    .quantization(1),
    .s_arith(`xlSigned),
    .s_bin_pt(0),
    .s_width(33))
  add (
    .a(mult1_p_net),
    .b(register9_q_net),
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .clr(1'b0),
    .en(1'b1),
    .s(add_s_net)
  );

  xlconvert #(

    .bool_conversion(0),
    .din_arith(2),
    .din_bin_pt(0),
    .din_width(32),
    .dout_arith(2),
    .dout_bin_pt(0),
    .dout_width(32),
    .latency(0),
    .overflow(`xlWrap),
    .quantization(`xlTruncate))
  convert (
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .clr(1'b0),
    .din(mult1_p_net),
    .en(1'b1),
    .dout(convert_dout_net)
  );

  xldelay #(

    .latency(1),
    .reg_retiming(0),
    .reset(0),
    .width(1))
  delay1 (
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .d(register5_q_net),
    .en(1'b1),
    .rst(1'b1),
    .q(delay1_q_net)
  );

  xldelay #(

    .latency(3),
    .reg_retiming(0),
    .reset(0),
    .width(1))
  delay2 (
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .d(cal_enable_net),
    .en(1'b1),
    .rst(1'b1),
    .q(delay2_q_net)
  );

  xldelay #(

    .latency(2),
    .reg_retiming(0),
    .reset(0),
    .width(1))
  delay3 (
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .d(delay2_q_net),
    .en(1'b1),
    .rst(1'b1),
    .q(datavalid_net)
  );

  xldelay #(

    .latency(3),
    .reg_retiming(0),
    .reset(0),
    .width(1))
  delay4 (
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .d(logical1_y_net),
    .en(1'b1),
    .rst(1'b1),
    .q(il_valid_net)
  );

  inverter_33a63b558a  inverter (
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .clr(1'b0),
    .ip(delay1_q_net),
    .op(inverter_op_net)
  );

  logical_4340034b97  logical (
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .clr(1'b0),
    .d0(h_out_net),
    .d1(l_out_net),
    .y(il_net)
  );

  logical_f48f811944  logical1 (
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .clr(1'b0),
    .d0(inverter_op_net),
    .d1(register5_q_net),
    .y(logical1_y_net)
  );

  xlmult_sysgen_mm_cal #(

    .a_arith(`xlSigned),
    .a_bin_pt(0),
    .a_width(33),
    .b_arith(`xlSigned),
    .b_bin_pt(31),
    .b_width(32),
    .c_a_type(0),
    .c_a_width(33),
    .c_b_type(0),
    .c_b_width(32),
    .c_baat(33),
    .c_output_width(65),
    .c_type(0),
    .core_name0("mult_11_2_5808000c9562bf6b"),
    .extra_registers(0),
    .multsign(2),
    .overflow(1),
    .p_arith(`xlSigned),
    .p_bin_pt(0),
    .p_width(32),
    .quantization(1))
  mult1 (
    .a(sub_s_net),
    .b(register2_q_net),
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .clr(1'b0),
    .core_ce(ce_1_sg_x0),
    .core_clk(clk_1_sg_x0),
    .core_clr(1'b1),
    .en(1'b1),
    .rst(1'b0),
    .p(mult1_p_net)
  );

  mux_40689d2172  mux (
    .ce(1'b0),
    .clk(1'b0),
    .clr(1'b0),
    .d0(add_s_net),
    .d1(mult1_p_net),
    .sel(sel_angle_mode_net),
    .y(mux_y_net)
  );

  xlregister #(

    .d_width(32),
    .init_value(32'b00000000000000000000000000000000))
  register1 (
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .d(x1_nm_i_net),
    .en(1'b1),
    .rst(1'b0),
    .q(register1_q_net)
  );

  xlregister #(

    .d_width(32),
    .init_value(32'b00000000000000000000000000000000))
  register2 (
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .d(s_i_net),
    .en(1'b1),
    .rst(1'b0),
    .q(register2_q_net)
  );

  xlregister #(

    .d_width(32),
    .init_value(32'b00000000000000000000000000000000))
  register3 (
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .d(hi_limit_net),
    .en(1'b1),
    .rst(1'b0),
    .q(register3_q_net)
  );

  xlregister #(

    .d_width(32),
    .init_value(32'b00000000000000000000000000000000))
  register4 (
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .d(low_limit_net),
    .en(1'b1),
    .rst(1'b0),
    .q(register4_q_net)
  );

  xlregister #(

    .d_width(1),
    .init_value(1'b0))
  register5 (
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .d(enable_net),
    .en(1'b1),
    .rst(1'b0),
    .q(register5_q_net)
  );

  xlregister #(

    .d_width(1),
    .init_value(1'b0))
  register6 (
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .d(il_net),
    .en(logical1_y_net),
    .rst(rst_net),
    .q(il_latched_o_net)
  );

  xlregister #(

    .d_width(32),
    .init_value(32'b00000000000000000000000000000000))
  register7 (
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .d(convert_dout_net),
    .en(1'b1),
    .rst(1'b0),
    .q(nm_err_s32_o_net)
  );

  xlregister #(

    .d_width(33),
    .init_value(33'b000000000000000000000000000000000))
  register8 (
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .d(mux_y_net),
    .en(delay2_q_net),
    .rst(1'b0),
    .q(nm_o_net)
  );

  xlregister #(

    .d_width(32),
    .init_value(32'b00000000000000000000000000000000))
  register9 (
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .d(x1_nm_i1_net),
    .en(1'b1),
    .rst(1'b0),
    .q(register9_q_net)
  );

  xlregister #(

    .d_width(32),
    .init_value(32'b00000000000000000000000000000000))
  register_x0 (
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .d(x2_nm_i_net),
    .en(1'b1),
    .rst(1'b0),
    .q(register_q_net)
  );

  relational_cd44469155  relational (
    .a(nm_o_net),
    .b(register3_q_net),
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .clr(1'b0),
    .op(h_out_net)
  );

  relational_6f290671ba  relational1 (
    .a(nm_o_net),
    .b(register4_q_net),
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .clr(1'b0),
    .op(l_out_net)
  );

  constant_6e9c75157d  s (
    .ce(1'b0),
    .clk(1'b0),
    .clr(1'b0),
    .op(s_o_net)
  );

  xladdsub_sysgen_mm_cal #(

    .a_arith(`xlSigned),
    .a_bin_pt(0),
    .a_width(32),
    .b_arith(`xlSigned),
    .b_bin_pt(0),
    .b_width(32),
    .c_has_c_out(0),
    .c_latency(0),
    .c_output_width(33),
    .core_name0("addsb_11_0_c5cb2be34e3bf19d"),
    .extra_registers(0),
    .full_s_arith(2),
    .full_s_width(33),
    .latency(0),
    .overflow(1),
    .quantization(1),
    .s_arith(`xlSigned),
    .s_bin_pt(0),
    .s_width(33))
  sub (
    .a(register_q_net),
    .b(register1_q_net),
    .ce(ce_1_sg_x0),
    .clk(clk_1_sg_x0),
    .clr(1'b0),
    .en(1'b1),
    .s(sub_s_net)
  );
endmodule
