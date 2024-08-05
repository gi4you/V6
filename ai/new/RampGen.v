`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    18:34:22 07/21/2012 
// Design Name: 
// Module Name:    RampGen 
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
`timescale 1 ns / 1 ns

module RampGen
          (
           clk,
           reset,
           trig,
           Ref_CNT,
           DataFreeRunOut,
           RstOut
          );


  input   clk;
  input   reset;
  input   trig;
  input   [31:0] Ref_CNT;  // uint32
  output  [31:0] DataFreeRunOut;  // uint32
  output  RstOut;


  wire enb;
  reg [31:0] Counter_Free_Running_out1=0;  // uint32
  wire Relational_Operator_out1;


  always @ (posedge clk or posedge reset)
    begin: Counter_Free_Running_process
      if (reset == 1'b1 | Relational_Operator_out1) begin
        Counter_Free_Running_out1 <= 32'h00000000;
      end
      else begin
        if (enb == 1'b1) begin
          if (Counter_Free_Running_out1 == 32'hFFFFFFFF) begin
            Counter_Free_Running_out1 <= 32'h00000000;
          end
          else begin
            Counter_Free_Running_out1 <= Counter_Free_Running_out1 + 1;
          end
        end
      end
    end // Counter_Free_Running_process

  assign DataFreeRunOut = Counter_Free_Running_out1;
  assign Relational_Operator_out1 = (Counter_Free_Running_out1 == Ref_CNT) ? 1'b1 : 1'b0;

  assign RstOut = Relational_Operator_out1;
  assign enb = trig;

endmodule  // PacketGen


