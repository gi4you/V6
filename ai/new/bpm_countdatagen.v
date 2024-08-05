/*
 *  sdi data interface
 *	Sync with after 3 ticks fa trigger.
 *
 */
`timescale 1ns / 1ns


module cc_countdatagen(
		input 			Reset,	//system reset | NPI trigger	
		input 			sdi_clk,
		input 			Trigger,
		input [31:0] 	maxCount,
		input [31:0] 	fa_pos_x,
		input [31:0] 	fa_pos_y,
		input			DataSel,			// Mux- data type
		input [31:0] 	wfmKx,				// Kx
		input [31:0] 	wfmKy,				// Ky
		input [31:0] 	wfmPhase_inc,		// frequency control
		output  		LocalDataValid,
		output [31:0] 	LocalCountData,
		output [31:0] 	LocalBpmPosData,
		output 			RampRst,
		output 			fa_evr_trig            //event synced output		
    );

	//localparam COUNT = 8'd14; 	//bpm and CC
	
	reg	counterEna;
	reg [7:0] counter;
	reg [31:0] tmpLocalData;
	reg [31:0] tmpBpmPosData;
	reg [31:0] tmpSinCosData;
	
`ifdef	BPM_SDI	
	//------------------------	
	// Negative edge trigger
	reg trigReg, trigReg_reg, trig; 
	always @ (posedge sdi_clk) 
	begin 
		trigReg <= ~Trigger;  //for delay from 10 kHz trigger
		trigReg_reg <= trigReg;
		trig <= (trigReg & !trigReg_reg);
	end
		
	//-----------------------	
	// Positive edge trigger
	reg PtrigReg, PtrigReg_reg, PosTrigEdge; 
	always @ (posedge sdi_clk) 
	begin 
		PtrigReg <= Trigger;  //for delay from 10 kHz trigger
		PtrigReg_reg <= PtrigReg;
		PosTrigEdge <= (PtrigReg & !PtrigReg_reg);
	end		
	assign fa_evr_trig = PosTrigEdge;
`endif

	assign trig = Trigger;
	assign fa_evr_trig = Trigger;
	//----------------------
	//wire RampRst;
	wire [31:0] RampData;	 
	RampGen ramp
          (
           .clk(sdi_clk),
           .reset(Reset),
           .trig(trig),
           .Ref_CNT( {8'd0, maxCount[23:0]} ),
           .DataFreeRunOut(RampData),
           .RstOut(RampRst)
          );
          
         

	//-----------------------
    wire [31:0] sin_out, cos_out;	       
	nco_sincos32	nco_sincos32_100Hz
          (
           .clk(sdi_clk),
           .reset(Reset),
           .clk_enable(trig),
           .phase_inc(wfmPhase_inc), 	//32'h028F5D70),	//100 Hz
           .Kx(wfmKx), 			//32'd10000000),
           .Ky(wfmKy),				//32'd10000000),
           .ce_out(),
           .sin(sin_out),
           .cos(cos_out)
          );
        

`ifdef SIN_COS
wire [15:0] 	myCosOut;
nco_16bit DacOutput
          (
           .clk(sdi_clk),				//10 kHz from EVR
           .reset(Reset),
           .clk_enable(trig),
           .phase_inc(wfmPhase_inc), //32'h028F5D70),		//28F5D70 - 100 Hz
           .gain(wfmKx[15:0]),  //16'd30000),
           .ce_out(),
           .cos_int16(myCosOut),
           .cos_sfix()
          );            
`endif
		            
	
	always @ (posedge sdi_clk)
	begin
		if (Reset) counterEna <= 1'b0;
		else if (trig) begin
			counterEna <= 1'b1;		// enabel when en_valid, if count 0 is skip
		end	
		else if ( counter == maxCount[31:24] ) counterEna <= 1'b0;	
		else counterEna <= counterEna;
	end
	
	//-------------------------------------
	// Ramp data is increment every trigger
	always @ (posedge sdi_clk)
	begin
	 if (trig) counter <= 8'd0;
	 else if (counterEna) begin
	 	counter <= (counter + 1);
		tmpLocalData <= RampData + counter;
		//bpm position data
		if(counter == 8'd0) begin
			tmpBpmPosData <= fa_pos_x;
			tmpSinCosData <= sin_out;
			//tmpSinCosData <= {16'd0, myCosOut };
		end	
		else if(counter == 8'd1) begin
			tmpBpmPosData <= fa_pos_y;
			tmpSinCosData <= cos_out;
			//tmpSinCosData <= {16'd0, myCosOut };
		end	
	 end
	 else counter <= counter;
	end
  
    reg tmp;
	always @ (posedge sdi_clk) begin
		tmp <= counterEna;
	end
	
	assign LocalDataValid  = counterEna & tmp;	
	assign LocalBpmPosData = tmpBpmPosData;		//10 kHz bpm position data (x,y)	
	
	//assign LocalCountData  = tmpLocalData;		//internal count value
	assign LocalCountData = ( DataSel == 1'b1) ? tmpLocalData : tmpSinCosData;

endmodule  