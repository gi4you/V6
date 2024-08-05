//
//	AD5754
//	Only support 0 ~ 5V, AVSS=GND
//
module DacAD5754_v2
  (	
	input reset,
	input [2:0] trig,
	input clk,            // 12.5 MHz , for before fpga clock is active
	input [15:0] dacDataI_CHA,
	input [15:0] dacDataI_CHB,
	input [15:0] dacDataI_CHC,
	input [15:0] dacDataI_CHD,
	output dacSync,        		// /dacSync
	output dacSclk,       		// dacSclk
	output dacSdin,				// SERIAL DATA OUT
	output reg dacClr,
	output reg dacLoad,
	output [7:0] dbgDcnt,
	output dbgDone,
	output dbgStart
   );
   
   wire   read = 1'b0;    // Always write for now
   wire   wz = 1'b0;  	// Always send 1 byte at a time
   
   
   reg [21:0] addr_data;

   reg [5:0]  entry;
   reg 	      start;
   reg [7:0]  counter=0;
   reg [23:0] command;
      
	  
	always @(posedge clk)
	begin
		dacClr <= 1'b1;
		dacLoad <= 1'b0;  //use dacSync to load and set		
	end
	  
	reg [5:0] aie_addr;	  
  //address count		
  always @ (posedge clk or posedge reset)
    begin
      if (reset == 1'b1 | trig[1] == 1'b1) begin
			aie_addr <= 6'd0;
      end
      else begin
          if (trig[0] == 1'b1 ) begin
				aie_addr <= aie_addr+1;				
          end
			 else
				aie_addr <= aie_addr;
      end
    end 

	 
   always @*
     case(entry)			    //REG   //ADDR  //DATA
       6'd00 : addr_data = {3'b010, 3'b000, 16'h001f};    	//POWER CONTROL REGISTER  power up
       //6'd01 : addr_data = {3'b001, 3'b000, 16'h0004}; 		//OUTPUT RANGE A +/-10 V
       //6'd02 : addr_data = {3'b001, 3'b001, 16'h0004}; 		//
       //6'd03 : addr_data = {3'b001, 3'b010, 16'h0004};
       //6'd04 : addr_data = {3'b001, 3'b011, 16'h0004};		//D +-10V
       6'd01 : addr_data = {3'b001, 3'b000, 16'h0000}; 		//OUTPUT RANGE A 5V
		 6'd02 : addr_data = {3'b001, 3'b001, 16'h0000}; 		//
       6'd03 : addr_data = {3'b001, 3'b010, 16'h0000};
       6'd04 : addr_data = {3'b001, 3'b011, 16'h0000};		//D 5V
		 
       6'd05 : addr_data = {3'b000, 3'b000, 16'h7fff};   	//  dumy data    
	   6'd06 : addr_data = {3'b000, 3'b000, dacDataI_CHA };   	//  CH1
       default : addr_data = {3'b000, 3'b000, 16'hf0ff}; 	//
     endcase // case(entry)

   	wire [5:0]  lastentry = 6'd6; 
	   
   	wire done = (counter >= 8'd49);		// length of data
    	
		
	//trig = DAC_10kHz, trig_delay, dac_multi_trig	
   always @(posedge clk)
     if(reset)
       begin
	  	entry <= #1 6'd0;
	  	start <= #1 1'b1;
       end
     else if(start)
       start <= #1 1'b0;
     else if(done && (entry<lastentry))
	     begin		 			 	
		 		entry <= #1 entry + 6'd1;
		 		start <= #1 1'b1;
	     end

   	else if( done && (entry >= lastentry))	//trigger by external pulse
   		start <= trig[0];   
   		       
	always @(posedge clk)
		if(reset)
			begin
			counter <= #1 8'd0;
			command <= #1 23'd0;
			end
		else if(start)
			begin				
				counter <= #1 8'd1;	
				
			  case(aie_addr)			    //REG   //ADDR  //DATA
				 6'd01 : command <= #1 {read, wz, {3'b000, 3'b000, dacDataI_CHA} };	//CHA    	//POWER CONTROL REGISTER  power up
				 6'd02 : command <= #1 {read, wz, {3'b000, 3'b001, dacDataI_CHB} }; 		//OUTPUT RANGE A +/-10 V
				 6'd03 : command <= #1 {read, wz, {3'b000, 3'b010, dacDataI_CHC} };	//CH3		//
				 6'd04 : command <= #1 {read, wz, {3'b000, 3'b011, dacDataI_CHD} };	//CH4
				 6'd05 : command <= #1 {read,wz, 3'b010, 3'b000, 16'h001f };		//power up	
				 /*  +/-10V
				 6'd06 : command <= #1 {read,wz, 3'b001, 3'b000, 16'h0004 };   
				 6'd07 : command <= #1 {read,wz, 3'b001, 3'b001, 16'h0004 };
				 6'd08 : command <= #1 {read,wz, 3'b001, 3'b010, 16'h0004 };
				 6'd09 : command <= #1 {read,wz, 3'b001, 3'b011, 16'h0004 };
				 */
				 //+5V
				 6'd06 : command <= #1 {read,wz, 3'b001, 3'b000, 16'h0000 };   
				 6'd07 : command <= #1 {read,wz, 3'b001, 3'b001, 16'h0000 };
				 6'd08 : command <= #1 {read,wz, 3'b001, 3'b010, 16'h0000 };
				 6'd09 : command <= #1 {read,wz, 3'b001, 3'b011, 16'h0000 };				 
				 default : command <= #1 {read,wz, 3'b010, 3'b000, 16'h001f };		//power up
			  endcase // case(entry)
					
			end
		//else if( |counter && ~done )
		else if( ~done )
			begin			
			counter <= #1 counter + 7'd1;
			
			if(~counter[0])
				command <= {command[22:0],1'b0};
			end
   
	
	assign dacSync = (done | counter == 8'd0);  
	assign dacSclk = ~(~counter[0]);	// div/2	
	assign dacSdin = command[23];
	
	assign dbgDcnt = counter;
	assign dbgDone = done; 
	assign dbgStart = start;  

	
endmodule 
