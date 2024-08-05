// -------------------------------------------------------------
//
//		125 MHz/8 = 15 MHz
//		66 ns * 800 = 52 us
//		trigger input make a 50 % clock generator
// -------------------------------------------------------------

`timescale 1 ns / 1 ns

module DacClkGen
    (
	input   clk,
	input   reset,
	input   trig,
	input	[10:0] start_dly,
	output  clk_out
    );


  wire enb;
  reg [10:0] Counter_Free_Running_out1;  
  wire Relational_Operator2_out1;
  wire Relational_Operator1_out1;
  wire Relational_Operator3_out1;
  wire Logical_Operator2_out1;

  
  always @ (posedge clk or posedge reset)
    begin: Counter_Free_Running_process
      if (reset == 1'b1 | trig == 1'b1 ) begin
        Counter_Free_Running_out1 <= 11'd0;
      end
      else begin
        //if (enb == 1'b1) begin
          if (Counter_Free_Running_out1 == 11'd1000 ) begin	//1000
            Counter_Free_Running_out1 <= Counter_Free_Running_out1;
          end          
          else begin
            Counter_Free_Running_out1 <= Counter_Free_Running_out1 + 1;
          end
        //end
      end
    end // Counter_Free_Running_process

    /*
    reg start_bit, end_bit;
    always @ (posedge clk ) begin
    	if(Counter_Free_Running_out1 >= start_dly) start_bit <= 1'b1;
    	else start_bit <= 1'b0;
    end
    always @ (posedge clk ) begin
    	if(Counter_Free_Running_out1 <= start_dly+11'd700) end_bit <= 1'b1;
    	else end_bit <= 1'b0;
    end    
    */
    
	assign Relational_Operator2_out1 = (Counter_Free_Running_out1 == start_dly) ? 1'b1 : 1'b0;
	
	assign Relational_Operator1_out1 = (Counter_Free_Running_out1 >= start_dly ) ? 1'b1 : 1'b0;
	
	assign Relational_Operator3_out1 = (Counter_Free_Running_out1 <= start_dly+11'd710 ) ? 1'b1 : 1'b0; //700
	
	assign Logical_Operator2_out1 =  Relational_Operator1_out1 & Relational_Operator3_out1;
	
	//assign clk_out = start_bit & end_bit; 
	assign clk_out = Logical_Operator2_out1;	//50% duty


	//assign cnt = Counter_Free_Running_out1;

endmodule  // addr_gen

