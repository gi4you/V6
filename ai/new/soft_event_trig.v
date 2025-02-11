
`timescale 1 ns / 1 ns

module soft_event_trig
          (
           clk,
           reset,
           clk_enable,
           evg_trig,
           evg_soft_event,
           trig_out
          );


  input   clk;
  input   reset;
  input   clk_enable;
  input	  evg_trig;			// 1Hz booster or SR trigger
  input   evg_soft_event;	// soft event
  output  trig_out;


  wire enb;
  reg  cell_out1;
  reg  Delay_out1;
  wire evg_soft_event_edge;
  
            
	reg T0Reg, T0Reg_reg1, T0Reg_reg2, TRIG0; 
	always @ (posedge clk) 
	begin 
		T0Reg <= evg_soft_event;
		T0Reg_reg1 <= T0Reg;
		T0Reg_reg2 <= T0Reg_reg1;
		TRIG0 <= (T0Reg_reg1 & !T0Reg_reg2);
	end
	assign evg_soft_event_edge = TRIG0;
	
  
  always @ (posedge clk)
    begin: cell_process
      if (reset == 1'b1 || trig_out == 1'b1) begin
        	cell_out1 <= 1'b0;		//clear output
      end
      else begin
        if (enb == 1'b1 && evg_soft_event_edge == 1'b1) begin
          	cell_out1 <= 1'b1;
        end
        else begin
        	cell_out1 <= cell_out1;
        end
      end
    end // cell_process

    
  always @ (posedge clk)
    begin: Delay_process
      if (reset == 1'b1) begin
        Delay_out1 <= 0;
      end
      else begin
        if (enb == 1'b1) begin
          Delay_out1 <= cell_out1;
        end
      end
    end // Delay_process

  assign trig_out =  evg_trig && Delay_out1;
  assign enb = clk_enable;

endmodule  // soft_event_trig

