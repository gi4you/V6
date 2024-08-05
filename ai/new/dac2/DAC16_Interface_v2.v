/*
 *	DAC Interface module.
 *
 */

`timescale 1ns / 1ps


module DAC_Interface_v2
(
	input	sysClk,
	input 	Reset,
	input	dacClk,		//10kHz
	input	[15:0]	OutU16b_A,
	input	[15:0]	OutU16b_B,
	input	[15:0]	OutU16b_C,
	input	[15:0]	OutU16b_D,
	//DAC interface
	output 	dacSync,        	// /dacSync
	output 	dacSclk,       		// dacSclk
	output 	dacSdin,			// SERIAL DATA OUT
	output 	dacClr,
	output 	dacLoad,
	output 	[7:0] dbg

	);
	// 100 MHz / 5 = 10 MHz	
	//  2 : 25 MHz 
	wire SCLK;		
	//BUFR #(	.BUFR_DIVIDE("8") 	// "BYPASS", "1", "2", "3", "4", "5", "6", "7", "8"
	BUFR #(	.BUFR_DIVIDE("2")		//100M/6 MHz clock
	) BUFR_inst (
		.O( SCLK ), 			// Clock buffer output
		.CE(1'b1), 				// Clock enable input
		.CLR(1'b0), 			// Clock buffer reset input
		.I( sysClk ) 			// Clock buffer input
	);
	
    reg [15:0] CHA_Data, CHB_Data, CHC_Data, CHD_Data;
    always @ (posedge SCLK) 
    begin
        CHA_Data <= OutU16b_A;
        CHB_Data <= OutU16b_B;
        CHC_Data <= OutU16b_C;
        CHD_Data <= OutU16b_D;
    end
    		
	//DAC Clk positive edge
	reg dac_trigReg, dac_trigReg_reg, dac_trig; 
	always @ (posedge SCLK) 
	begin 
		dac_trigReg <= dacClk;
		dac_trigReg_reg <= dac_trigReg;
		dac_trig <= (dac_trigReg & !dac_trigReg_reg);
	end
		
		
	wire	AIeCalValid ;	    	
	wire [7:0] dbgDcnt;
	wire dbgStart, dbgDone;
	reg a_trigReg, a_trigReg_reg, a_TrigEdge; 
	
	DacAD5754_v2 DAC_AD5754 (
		.reset(Reset), 
		.trig( { dacClk, dac_trig, a_TrigEdge } ), 	//10 kha
		.clk(SCLK), 		//16MHZ serial clock
		.dacDataI_CHA(CHA_Data), 		  	// A
		.dacDataI_CHB(CHB_Data ),       	// B
		.dacDataI_CHC(CHC_Data),
		.dacDataI_CHD(CHD_Data),
		//
		.dacSync(dacSync),		//DAC 
		.dacSclk(dacSclk), 		//DAC
		.dacSdin(dacSdin), 		//DAC
		.dacClr(dacClr), 		//DAC
		.dacLoad(dacLoad), 
		.dbgDcnt(dbgDcnt), 
		.dbgDone(dbgDone), 
		.dbgStart(dbgStart)
	    );

`define	AIE_CAL_NCLK				30		//each AIE allow 30 cycles for calculations		
	dac_addr_gen_v2 AIE_ADDR_GEN (
		.clk( SCLK ), 
		.reset(Reset), 
		.trig(dac_trig), 
		.clk_enable(1'b1), 
		.address_move(12'd`AIE_CAL_NCLK-2),     
		.AddressStart(12'd0), 
		.AddressEnd(12'd`AIE_CAL_NCLK), 	//30 clock 
		.aie_mask(64'h0000001111111111),	//Input Masking ID number's
		.Data16b(), 
		.aie_addr(), 
		.wr_one(  ), 
		.wr( AIeCalValid ),			//max 60 pulse, I assume max IDs are 60
		.mask()
	);	
	

	always @ (posedge SCLK) 
	begin 
		a_trigReg <= AIeCalValid;
		a_trigReg_reg <= a_trigReg;
		a_TrigEdge <= (a_trigReg & !a_trigReg_reg);
	end
	
	
	assign dbg[7:0] = { 1'b0, 1'b0, dbgDone, dbgStart, AIeCalValid, a_TrigEdge, dac_trig, SCLK };
	
	
endmodule
