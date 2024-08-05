`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/07/2022 01:38:30 PM
// Design Name: 
// Module Name: ResetTrigSync
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module ResetTrigSync(
    input sysClk,
    input reset,
    input ResetTrigIn,
    input trig,
    output reg trigSyncOut
);
	
	
    localparam IDLE_   = 2'b01;
    localparam READY_  = 2'b10;
    localparam SYNC_   = 2'b11;

    reg [1:0] state = IDLE_;
    reg [3:0] count;
	
    always@(posedge sysClk) begin
        if(reset == 1'b1) begin
            trigSyncOut <= 1'b0;
            state       <= IDLE_;
        end
        else begin
             case (state)
                    IDLE_  : begin
                        if (ResetTrigIn == 1'b1) begin	// user reset detected?
                            count <= 4'd0;
                            state <= READY_;
                        end
                        else state <= IDLE_;
                    end
                    READY_ : begin
                        if (trig) begin
                            count <= count + 1;
                            if(count > 2) begin
                                trigSyncOut <= 1'b1;
                                state  <= SYNC_;
                            end
                        end
                    end
                    SYNC_: begin
                        trigSyncOut <= 1'b0;
                        state       <= IDLE_;
                   end
                endcase
        end
    end
	
endmodule	
