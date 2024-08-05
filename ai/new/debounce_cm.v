

//http://web.mit.edu/6.111/www/f2005/code/jtag2mem_6111/debounce.v.html

`timescale 1ns / 1ps
// clk = 10 kHz
//
module debounceIndex_cm (reset, clk, noisy, clean);
   input reset, clk, noisy;
   output clean;

   parameter NDELAY = 4;	// 4 : 10 1 ms delay
   parameter NBITS  = 5;

   reg [NBITS-1:0] count;
   reg xnew, clean;

   always @(posedge clk)
     if (reset) begin 
		xnew <= noisy; 
		clean <= noisy; 
		count <= 0; 
	end
     else if (noisy != xnew) begin 
		xnew <= noisy; 
		count <= 0; 
	end
     else if (count == NDELAY) clean <= xnew;
     else count <= count+1;

endmodule