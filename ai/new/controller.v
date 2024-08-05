//controller
module controller (
    input 	clk, reset,
    input 	fofbCalStart, 
	input	[8:0] CalcLanth,	//for variable 
    output reg [8:0] addRamR,
	output reg [8:0] addRamRReg2,	        
    output reg oneModeStart,
	output wire addRamR_Valid
	);
	
	reg oneModeStartReg1,oneModeStartReg2,calRunning;
	
	//parameter CAL_LENGTH = 9'h167;   //359
	//parameter CAL_LENGTH = 9'h1DF;   //479
	
	always @(negedge clk) begin
		oneModeStart <= fofbCalStart;
		oneModeStartReg1 <= oneModeStart;
		oneModeStartReg2 <= oneModeStartReg1; 
	end

	always @(negedge clk) begin
		if(reset==1'b1)
			calRunning <= 1'h0;
		else if((oneModeStartReg2==1'b0)&&(oneModeStartReg1==1'b1)) 
			calRunning <= 1'h1;
		//else if (addRamR == CAL_LENGTH)
		else if (addRamR == CalcLanth )	//06/08/14
			calRunning <= 1'h0;
		else 
			calRunning <= calRunning;
	end

	reg add_Valid;
	always @(negedge clk) begin	
		if(calRunning==1'b0) begin
			addRamR <= 9'h0;
		end	
		else begin
			addRamR <= addRamR + 9'h1;
		end	
	end
  
	//need one daley for BPM RAM
	reg [8:0] addRamR2dly;
	always @(posedge clk) begin		
		addRamR2dly <= addRamR;
		addRamRReg2 <= addRamR2dly;	
	end	

	assign addRamR_Valid = calRunning;
endmodule