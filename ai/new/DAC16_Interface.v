/*
 *	DAC Interface module.
 *
 */

`timescale 1ns / 1ps


module DAC_Interface 
(
	input 	clk,						//100MHz - 125 MHz
	input 	Reset,
	input	[9:0]	SdiAddr_i,			//SDI address
	input	[9:0]	plb_SdiAddrSel_i,	//User selected address
	input			SdiDataValid_i,		//SDI data valid
	input	[31:0]  SdiData_i,			//SDI data
	input	[15:0]	CH_A_TestData,		//User Test Data
	//DAC interface
	output 	dacSync,        	// /dacSync
	output 	dacSclk,       		// dacSclk
	output 	dacSdin,			// SERIAL DATA OUT
	output 	dacClr,
	output 	dacLoad,
	output 	[8:0] dbg,
	output reg [19:0]	Dac20bitIn //Jan 19/2013
	);

	wire fifo_read =  ~empty;
	
	// 100 MHz / 8 = 12 MHz	
	wire SCLK;		
	//BUFR #(	.BUFR_DIVIDE("8") 	// "BYPASS", "1", "2", "3", "4", "5", "6", "7", "8"
	BUFR #(	.BUFR_DIVIDE("4")		//50 MHz clock
	) BUFR_inst (
		.O( SCLK ), 			// Clock buffer output
		.CE(1'b1), 				// Clock enable input
		.CLR(1'b0), 			// Clock buffer reset input
		.I( clk ) 				// Clock buffer input
	);
	
	wire   DAC_10kHz;	//sync clock generator
	DacClkGen DacClkGen_
        (
    	.clk(SCLK),    //125MHz/8
    	.reset(Reset),
    	.trig(fifo_read),
    	.start_dly(11'd1),
    	.clk_out(DAC_10kHz)    // 10 kHz output
        );
        	
	//DAC Clk positive edge
	reg dac_trigReg, dac_trigReg_reg, dac_trig; 
	always @ (posedge SCLK) 
	begin 
		dac_trigReg <= DAC_10kHz;
		dac_trigReg_reg <= dac_trigReg;
		dac_trig <= (dac_trigReg & !dac_trigReg_reg);
	end
		
	reg dac_n_trigReg, dac_n_trigReg_reg, dac_n_trig; 
	always @ (posedge SCLK) 
	begin 
		dac_n_trigReg <= ~DAC_10kHz;
		dac_n_trigReg_reg <= dac_n_trigReg;
		dac_n_trig <= (dac_n_trigReg & !dac_n_trigReg_reg);
	end
	
	//Dual trigger
	wire dual_dac_trig = dac_n_trig | dac_trig;
	wire dac_multi_trig, trig_delay;
		Dac_trig_gen Dac_trig_gen(
		.clk(SCLK),
		.trig( dual_dac_trig ),
		.dac_multi_trig(dac_multi_trig),
		.trig_delay(trig_delay)
	);
	
     		
	////////////////////////////////////////
    // BPM LOCAL DATA DAC OUTPUT TEST
    wire    [15:0] dac16_reg;
    dac_offset_calc    dac_offset_calc
    (
        .InS32b (SdiData_i),    
        .OutU16b(dac16_reg)                  
    );
       
    //Jan 19/2013 for Corrector PS DAC output
    wire    [19:0] dac20_reg; 
	dac20_offset_calc	Dac20bit
          (
           .InS32b(SdiData_i),
           .OutU20b(dac20_reg)
          );
          
	// x,y position data to DAC  FIFO OUTPUT
	// plb_Control_0[7:0] alue is BPM number for DAC output test.
	reg	[15:0] bpm_x_dac;
	
	wire DAC_ENABLE;
	assign DAC_ENABLE = (SdiAddr_i[9:0] == plb_SdiAddrSel_i[9:0] ) ? 1'b1 : 1'b0;
    reg  DAC_Trigger;
	always @(posedge clk ) begin
		if( DAC_ENABLE == 1'b1 & SdiDataValid_i == 1'b1 ) begin		
			bpm_x_dac  <= dac16_reg; 
			Dac20bitIn <= dac20_reg;	//for Corrector PS Output 20bit DAC
			end
		else begin	
			bpm_x_dac  <= bpm_x_dac;
			Dac20bitIn <= Dac20bitIn;
		end	
	end
		
	// 0ne clock dly
	always @(posedge clk ) begin
		DAC_Trigger <= DAC_ENABLE;  
    end
    		
	//Use FIFO memory	
	wire    [15:0] DataBuffCH2;	
	DAC_FIFO DAC_CH2 (
	      .rst(Reset), 			// input rst
	      .wr_clk( clk), 		// input wr_clk
	      .rd_clk( SCLK), 		// input rd_clk
	      .din( bpm_x_dac), 		// input [15 : 0] din
	      .wr_en( DAC_Trigger), // input wr_en
	      .rd_en( fifo_read), 	// input rd_en
	      .dout( DataBuffCH2), 	// output [15 : 0] dout
	      .full(), 				// output full
	      .empty(empty) 		// output empty
	);
	
	
	wire [15:0] OutU16b_A, OutU16b_B, OutU16b_C;	
    assign OutU16b_A = CH_A_TestData;	
    assign OutU16b_B = bpm_x_dac;
    assign OutU16b_C = DataBuffCH2;		      	
	wire [7:0] dbgDcnt;
	wire dbgStart, dbgDone;
	
	DacAD5754 DAC_AD5754 (
	    .reset(Reset), 
	    .trig( { DAC_10kHz, trig_delay, dac_multi_trig } ), 	//10 kha
	    .clk(SCLK), 		//10MHZ serial clock
	    .dacDataI_CHA(OutU16b_A), 		  	// A
		.dacDataI_CHB(OutU16b_B ),       	// B
		.dacDataI_CHC(OutU16b_C),
	    .dacSync(dacSync),		//DAC 
	    .dacSclk(dacSclk), 		//DAC
	    .dacSdin(dacSdin), 		//DAC
	    .dacClr(dacClr), 		//DAC
	    .dacLoad(dacLoad), 
	    .dbgDcnt(dbgDcnt), 
	    .dbgDone(dbgDone), 
	    .dbgStart(dbgStart)
	    );

	
	assign dbg[8:0] = { DAC_10kHz, fifo_read, DAC_ENABLE, dbgDone, dbgStart, trig_delay, dac_multi_trig, dac_trig, SCLK };

	
endmodule
