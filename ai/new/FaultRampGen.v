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

module FaultRampGen
          (
           clk,
           reset,
		   usr_rst,
           trig,
		   fault_trig,
           Ref_CNT,
           DataFreeRunOut,
           RstOut,
		   npi_enable
          );


  input   clk;
  input   reset;
  input		usr_rst;
  input   trig;
  input		fault_trig;
  input   [31:0] Ref_CNT;  // uint32
  output  [31:0] DataFreeRunOut;  // uint32
  output  RstOut;
  output  reg npi_enable;


	reg fault_cnt_en;
  wire enb;
  reg [31:0] Counter_Free_Running_out1;  // uint32
  wire Relational_Operator_out1;

	always @ (posedge clk) begin
		if (reset == 1'b1 | RstOut == 1'b1 | usr_rst == 1'b1) fault_cnt_en <= 1'b0;
		else if(fault_trig == 1'b1)
			fault_cnt_en <= 1'b1;		//start count
	end
	
  always @ (posedge clk or posedge reset)
    begin: Counter_Free_Running_process
      //if (reset == 1'b1 | Relational_Operator_out1) begin
	  if (reset == 1'b1 | usr_rst == 1'b1) begin
        Counter_Free_Running_out1 <= 32'h00000000;
      end
      else begin
        if (enb == 1'b1 && fault_cnt_en == 1'b1) begin
          //if (Counter_Free_Running_out1 == 32'hFFFFFFFF) begin
          //  Counter_Free_Running_out1 <= 32'h00000000;
		  if(Relational_Operator_out1 == 1'b1) begin
				Counter_Free_Running_out1 <=  Counter_Free_Running_out1;
          end
          else begin
            Counter_Free_Running_out1 <= Counter_Free_Running_out1 + 1;
          end
        end
      end
    end // Counter_Free_Running_process


	always @ (posedge clk) begin
		if (reset == 1'b1 | usr_rst == 1'b1)
			npi_enable <= 1'b1;
		else if(RstOut) 
			npi_enable <= 1'b0;
	end	



	
  assign DataFreeRunOut = Counter_Free_Running_out1;
  assign Relational_Operator_out1 = (Counter_Free_Running_out1 == Ref_CNT) ? 1'b1 : 1'b0;


	reg T1Reg, T1Reg_reg1, T1Reg_reg2, TRIG0; 
	always @ (posedge clk) 
	begin 
		T1Reg <= Relational_Operator_out1;	//40 ns (8 ns * 5) at user_logic.v
		T1Reg_reg1 <= T1Reg;
		T1Reg_reg2 <= T1Reg_reg1;
		TRIG0 <= (T1Reg_reg1 & !T1Reg_reg2);
	end

	
  assign RstOut = TRIG0; //Relational_Operator_out1;
  assign enb = trig;

endmodule  // PacketGen


