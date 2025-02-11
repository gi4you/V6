/*
 *	4 channel DAC address generator
 *
 */
 
`timescale 1 ns / 1 ns
`define	MAX_CLK_COUNT	30	//40
`define	MAX_ID_NUM		60

module dac_addr_gen_v2
          (
		clk,
		reset,
		trig,
		clk_enable,
		address_move,	// alway's 0
		AddressStart,	//alway's 0
		AddressEnd,		//how many clock's need for calculation ~ 50
		aie_mask,		//masking bit
		Data16b,
		aie_addr,
		wr_one,
		wr,
		mask
          );


	input   clk;
	input   reset;
	input   trig;	
	input   clk_enable;
	input   [11:0] address_move;  // ufix12
	input   [11:0] AddressStart;  // ufix12
	input   [11:0] AddressEnd;  // ufix12
	input   [63:0] aie_mask;
	output  signed [15:0] Data16b;  // int16
	output  reg [6:0]  aie_addr;
	output  wr_one;
	output  wr;
	output mask;
	
	
	wire mask_bit;
	wire enb;
	wire enb_1_1_1;
	reg [11:0] Counter_Free_Running_out1;  // ufix12
	wire Relational_Operator2_out1;
	wire Relational_Operator1_out1;
	wire Relational_Operator3_out1;
	wire Logical_Operator2_out1;


  assign enb_1_1_1 = clk_enable;

  always @ (posedge clk or posedge reset)
    begin: Counter_Free_Running_process
      if (reset == 1'b1 | trig == 1'b1) begin
        Counter_Free_Running_out1 <= 12'b000000000000;
		aie_addr <= 10'd0;
      end
      else begin
        if (enb == 1'b1) begin
          if (Counter_Free_Running_out1 == 12'd`MAX_CLK_COUNT ) begin
				Counter_Free_Running_out1 <= 12'b000000000000;
				aie_addr <= aie_addr+1;				
          end
          else if (aie_addr >= 10'd`MAX_ID_NUM) begin
				Counter_Free_Running_out1 <= Counter_Free_Running_out1;  
          end          
          else begin
            Counter_Free_Running_out1 <= Counter_Free_Running_out1 + 1;
          end
        end
      end
    end // Counter_Free_Running_process


	assign Data16b = Counter_Free_Running_out1;
	assign Relational_Operator2_out1 = (Counter_Free_Running_out1 == address_move) ? 1'b1 : 1'b0;
	assign Relational_Operator1_out1 = (Counter_Free_Running_out1 >= AddressStart) ? 1'b1 : 1'b0;
	assign Relational_Operator3_out1 = (Counter_Free_Running_out1 <= AddressEnd) ? 1'b1 : 1'b0;
	assign Logical_Operator2_out1 =  Relational_Operator1_out1 & Relational_Operator3_out1 & ~reset;
	
	
	assign mask_bit = aie_mask[aie_addr] && (aie_addr < `MAX_ID_NUM);
	assign wr_one = Relational_Operator2_out1 && (aie_addr < `MAX_ID_NUM); //&& mask_bit ;	
	assign wr = Logical_Operator2_out1 && (aie_addr < `MAX_ID_NUM) && mask_bit ;
	assign enb = clk_enable;
	
	assign mask = mask_bit;
endmodule  

