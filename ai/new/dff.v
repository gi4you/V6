

//http://learn.digilentinc.com/Documents/259

// assign D <= ~Q;
`timescale 1ns / 1ps
module dff(
    input D,
    input clk,
    input rst,
    output reg Q
    );
 
always @ (posedge(clk), posedge(rst))
begin
    if (rst == 1)
        Q <= 1'b0;
    else
        Q <= D;
end
     
endmodule
 
 /*
 
 // test bench
 
 `timescale 1ns / 1ps
 
module tb;
 
    // Inputs
    reg clk;
    reg rst;
 
    // Outputs
    wire led;
 
    // Instantiate the Unit Under Test (UUT)
    clk_divider uut (
        .clk(clk), 
        .rst(rst), 
        .led(led)
    );
 
    always
        #5 clk = ~clk;
 
    initial begin
        // Initialize Inputs
        clk = 0;
        rst = 1;
         
        #10 rst = 0;
 
        // Wait 100 ns for global reset to finish
        #100;
         
    end
       
endmodule

*/