`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    13:41:25 12/01/2012 
// Design Name: 
// Module Name:    bpm_xy_position_sim 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//	BPM x, y position data for simulation.
//		Block data is count data 0 .. 360
//		Pos X : 0,2,4,6,8....
//		Pos Y : 1,3,5,7,9....
//
//	01/05/2013
//	1) Added five singanls for microblaze interface Dualport memory port A
//		input	SimDataMicroblazeMode,
//		input	CS_a,       	//cs   1 
//		input	WR_a,          	//wr   1
//		input	[9:0] Ram_Addr_a,        //addr 12:0
//		input   [31:0] Ram_Data_a, 
//
//
//////////////////////////////////////////////////////////////////////////////////

module bpm_xy_position_sim(
	input 	clk,
	input 	reset,
	input 	trig,
	//01/05/2013
	input	SimDataMicroblazeMode,
	input	CS_a,       	//cs   1 
	input	WR_a,          	//wr   1
	input	[9:0] Ram_Addr_a,        //addr 12:0
	input   [31:0] Ram_Data_a,        //data 31:0		
	//
	input	[11:0] StartAddr,	
	input	[11:0] EndAddr,
	input	[11:0] sel_addr,	//address select 0.. 360
	input	[19:0] Kx,
	input	[19:0] Kxoff,
	input	mode,
	input	DataTypeIsRamp,		//1: Ramp Data generator, 0: Cos wave generator
	// DpRam Port-B
	input	DpRamB_Read,
	input	[11:0] DpRamB_Address,
	output	[31:0] PosXyData,
	output	PosXyDataValid,
	output	PosSingleDataValid,
	output  [15:0] DAC_DataOut,
	output  WrDone
	//output	[31:0] dbgData,
	//output dbgWr	
    );

    
wire	wr, wr_one; 
wire 	WRE;  
wire [15:0] DataCount16b;
wire [11:0] addr_ram_a;
wire [31:0] DataIna; 
wire [19:0] cos_x;
wire [31:0] RampData;


fofb_addr_gen 	BpmPosXY_DataGen (
    .clk(clk), 
    .reset(reset), 
    .trig(trig), 
    .clk_enable(1'b1), 
    .Gain(16'd1), 
    .address_move(sel_addr), 
    .AddressStart(StartAddr), 	//0
    .AddressEnd(EndAddr), 		//359
    .ce_out(), 
    .Data16b(DataCount16b), 
    .Data32b(), 
    .addr(addr_ram_a), 
    .wr_one(wr_one), 
    .wr(wr)
    );
        
//10/05/13 added    
//Ramp data generator
RampGen	RampGen_
	(
	.clk(clk),
	.reset(reset),
	.trig(wr_one),
	.Ref_CNT( {12'd0, Kx} ),
	.DataFreeRunOut(RampData),
	.RstOut()
	);
              
// Cos waveform generator
nco_cos20b COS_Gen (
    .clk(clk), 
    .reset(reset), 
    .clk_enable(wr_one), 
    .Kx(Kx), 
    .Ky(Kx), 
    .Kxoff(Kxoff), 
    .Kyoff(Kxoff), 
    .freq(32'h028F5D70),   // 100 Hz
    .ce_out(), 
    .cos_x(cos_x), 
    .cos_y()
    );
        
    //01/05/2013 Dual input is possible for simulation Data        
	wire	DpRamCS_a;       		//cs   1 
	wire	DpRamWR_a;          	//wr   1
	wire	[9:0] DpRam_Addr_a;     //addr 12:0
	wire   [31:0] DpRam_Data_a;     //data 31:0		
	assign 	DpRamCS_a      = (SimDataMicroblazeMode == 1'b1) ? CS_a       : 1'b1;    
	assign 	DpRamWR_a      = (SimDataMicroblazeMode == 1'b1) ? WR_a       : WRE;
	assign 	DpRam_Addr_a   = (SimDataMicroblazeMode == 1'b1) ? Ram_Addr_a : addr_ram_a[9:0];	//9:0
	assign 	DpRam_Data_a   = (SimDataMicroblazeMode == 1'b1) ? Ram_Data_a : DataIna; 
		
	/////////// X,Y BPM DATA RAM, 2 clock latanc
`ifdef SIM_COUNT_MODE
	BPM_XY_Buffer BPM_XY_Buffer_SIM (
		.clka(clk),
		/* A */
		.ena(1'b1),
		.wea(WRE),
		.addra(addr_ram_a[9:0]),
		.dina( DataIna ),
		/* B */
		.clkb(clk),
		.enb(DpRamB_Read),
		.addrb(DpRamB_Address[9:0]),
		.doutb(PosXyData)
	);
`else
	BPM_XY_Buffer BPM_XY_Buffer_SIM (
		.clka(clk),
		/* A */
		.ena(DpRamCS_a),
		.wea(DpRamWR_a),
		.addra(DpRam_Addr_a),
		.dina( DpRam_Data_a ),
		/* B */
		.clkb(clk),
		.enb(DpRamB_Read),
		.addrb(DpRamB_Address[9:0]),
		.doutb(PosXyData)
	);
`endif
	
	// 2 clk delay
	reg Delay1_out1, Delay2_out1;
  	always @ (posedge clk) begin        		
  		Delay1_out1 <= DpRamB_Read;        		
  		Delay2_out1 <= Delay1_out1;      		
    end 
     
	reg Delay3_out1, Delay4_out1;
  	always @ (posedge clk) begin        		
  		Delay3_out1 <= achk;        		
  		Delay4_out1 <= Delay3_out1;      		
    end 
    
        
  reg [15:0] DAC_Data;  
  always @ (posedge clk)
    begin
      if (reset == 1'b1) begin
        DAC_Data <= 16'd0;
      end
      else begin
        if (PosSingleDataValid == 1'b1) begin
          DAC_Data <= PosXyData;
        end
      end
    end // Delay2_process
    
	wire dly5clk;
	pid_en_5clk_dly WrDly
          (
           .clk(clk),
           .reset(reset),
           .enb(1'b1),
           .t_in(WRE),
           .t_out(dly5clk)
          );
              
	pos_edge	trig_gen
          (
           .clk(clk),
           .reset(reset),
           .enb(1'b1),
           .trig_in(!dly5clk),	//not
           .trig_out(WrDone)
          );	
		  
        
    // mode:0 -- block x,y count data write 0.. 360
    // mode:1 -- write cos data to selected address
    wire [31:0] DataOut;
    assign DataOut = (DataTypeIsRamp == 1'b1) ? RampData : {12'd0, cos_x};
    
    assign DataIna =  (mode == 1'b0) ? ({16'd0, DataCount16b}) : DataOut;    
	assign WRE     =  (mode == 1'b0) ?  wr : wr_one;
	assign PosXyDataValid = Delay2_out1;
	
	// for DAC output test
	wire achk = (DpRamB_Address == sel_addr);	
	assign PosSingleDataValid = Delay4_out1;	// 2 clk latancy
	assign DAC_DataOut = DAC_Data;
	
	//assign dbgData = {16'd0, DataCount16b} ; //Data;
	//assign dbgWr   = wr;		//WR
		
endmodule
