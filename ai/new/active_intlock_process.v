/*
 *	Active interlock processor.
 *
 *
 *	Inputs:
 *		x1 - bpm 1 x position
 *		y1 - bpm 1 y position
 *		x2 - bpm 2 x position
 *		y2 - bpm 2 y position
 *
 *		x12_diamond offset - bpm 1 x and y interlock limit value
 *		y12_diamond_offset - bpm 2 x and y interlock limit value
 *
 *
 *		AI Settings Logic:
 *		AI EPS Logic for e-beam within the allowed phase space of AIE-ID (if faulse -> Dump e-beam))
 *		
 *      AIE NAME      Shape                 e-bmp1       e-bmp2        e-beam offset at AIE              e-beam angle at AIE
 *                                                                     location                          location
 *   --------------------------------------------------------------------------------------------------------------------------------
 *		AIE-ID-A	rectangular										  |(x2-x1)*(s3-s1)/(s2-s1)|< AIOL & |(x2-x1)/(s2-s1)|< AIAL
 *		AIE-ID-B	diamond					|x1|< AIOL & |x2|< AIOL
 *		AIE-ID-C	rect+optimal offset		|x1|< AIOL & |x2|< AIOL & |(x2-x1)*(s3-s1)/(s2-s1)|< AIOL & |(x2-x1)/(s2-s1)|< AIAL
 *		AIE-ID-D	rect+small offset		|x1|< AIOL & |x2|< AIOL &                                 & |(x2-x1)/(s2-s1)|< AIAL
 *
 *
 *	LOGIC:
 *		rectangular (for AIE-ID-A)				: 20
 * 		diamond (for AIE-ID-B & AIE-BM)			: 10 		
 * 		rectangular + optimal offset (AIE-ID-C)	: 21
 * 		rectangular + small offset (AIE-ID-D)	: 22
 *

	>> Each unit has the following pvs with current consideration:
	>> 
	>> 'name': [], # str, active interlock device name
	>> 'units': [], # str, units for each columns. Empty string if it does not have one.
	>> 'code': [], # int, active interlock logic code
	>> 'offset': [], # double, offset relative to the center of a straight section
	>> 'safecurent': [], # double, allowed beam current for safe operation.
	>> 'aihol': [], # double, allowed horizontal offset limit
	>> 'aivol': [], # double, allowed vertical offset limit
	>> 'aihal': [], # double, allowed horizontal angle limit
	>> 'aival': [], # double, allowed vertical angle limit
	>> 'up_name': [], # str, upstream BPM name involved in this active interlock unit
	>> 'up_offset': [], # double, offset of upstream BPM relative to the center of a straight section
	>> 'up_aihol': [], # double, allowed horizontal offset limit of upstream BPM
	>> 'up_aivol': [], # double, allowed vertical offset limit of upstream BPM
	>> 'down_name': [], # str, downstream BPM name involved in this active interlock unit
	>> 'down_offset': [], # double, offset of downstream BPM relative to the center of a straight section
	>> 'down_aihol': [], # double, allowed horizontal offset limit of downstream BPM
	>> 'down_aivol': [], # double, allowed vertical offset limit of downstream BPM

	
	
0: H1 offset dp-ram		5000000		5 mm
1: V1 offset dp-ram		5000000
2: H2
3: V2
4: H3
5: V3
6: mrad X				2500000		2.5 mm
7: mrad Y
8: S for mrad(angle)
9: S for mm
10: Logic type
	
8: S for mrad(angle)

addr= 0 : 0x1882FD80	0.191
addr= 1 : 0x1882FD80
addr= 2 : 0x1882FD80
addr= 3 : 0x1882FD80
addr= 4 : 0x1882FD80
addr= 5 : 0x1882FD80
addr= 6 : 0x1882FD80
addr= 7 : 0x11704FF4
addr= 8 : 0x11704FF4
addr= 9 : 0x11704FF4

9: S for mm	
addr= 0 : 0x3E554D00	0.487
addr= 1 : 0x3E554D00
addr= 2 : 0x3E554D00
addr= 3 : 0x3E554D00
addr= 4 : 0x3E554D00
addr= 5 : 0x3E554D00
addr= 6 : 0x3E554D00
addr= 7 : 0x3FDC486A
addr= 8 : 0x3FDC486A
addr= 9 : 0x3FDC486A
	
	
  09/26/14
	RESET -> reset
	bpm_pos_status  testing but not corectly working
	bpm_pos_status[aie_addr[5:0]]    changed
	
	10/08/14
		cnt_rst    added
		muxValid   timing fixed
		
	10/10/14
		bpm_12_rx_status
	11/12/14
		BeamCurrentStatus added for bpm fault chaecking
	
	11/13/14	
		Position limit +/- 20 mm 

	10/12/2015	for detect which bpm caused fault
		bpm_12_rx_status[1:0]

	06/06/2016
		output	[7:0] debug from [5:0]
		
	
 */

`timescale 1 ns / 1 ns


`define	__DATA_32BIT__

module active_intlock_process (

    input    	clk,
    input    	reset,
	input		cnt_rst,
	input		trig,
	input		EN,				//IL latch enable
	input		Clear,			//IL latch clear
	input		BeamCurrentStatus,
	input	 [7:0] aie_addr,	//address
	input		AIeCalValidIn,
	input      [1:0] mode,
	input	mux_dir,
	input	 [7:0] logic,		//xy1 & xy2 & mm & mrad
`ifdef	__DATA_32BIT__	
	input	 [31:0] x1_nm_i,	//x1 position
	input	 [31:0] x2_nm_i,	//x2 position
	input	 [31:0] y1_nm_i,	//y1 position
	input	 [31:0] y2_nm_i,	//y2 position
`else
	input	 [25:0] x1_nm_i,	//x1 position
	input	 [25:0] x2_nm_i,	//x2 position
	input	 [25:0] y1_nm_i,	//y1 position
	input	 [25:0] y2_nm_i,	//y2 position
`endif	
	input	 [31:0] Cell_status,
	
	//12/18/13
	input	 [31:0]	AIOL_H1,
	input	 [31:0]	AIOL_V1,
	input	 [31:0]	AIOL_H2,
	input	 [31:0]	AIOL_V2,
	input	 [31:0]	AIOL_H3,
	input	 [31:0]	AIOL_V3,		
	input	 [31:0]	AIAL_H,
	input	 [31:0]	AIAL_V,
	//Offset
	input		[31:0] OFFSET_H1,
	input		[31:0] OFFSET_V1,
	input		[31:0] OFFSET_H2,
	input		[31:0] OFFSET_V2,
	//
	input	 [31:0] S_mm,
	input	 [31:0] S_angle,
	input      [3:0] mux_addr,
	output	[31:0] mux_Data_o,
	output	[31:0] x_mm_cal,
	output	[31:0] y_mm_cal,
	output	[31:0] x_mrad_cal,
	output	[31:0] y_mrad_cal,
	output	[31:0] x_mrad_err,
	output	[31:0] y_mrad_err,	
	output	[31:0] x_mm_err,
	output	[31:0] y_mm_err,
	output	muxValid,
	output	[7:0] debug,
	output	[5:0] Il_latched_s,	
	output	[63:0] AI_ID_RfDumpOut,
	output	[63:0] Aie_bm_o,
	output	[63:0] Aie_A_o,
	output	[63:0] Aie_C_o,
	output	[63:0] Aie_D_o,	
	output	datavalid,
	output	il_LatchValid_o,
	input	dump_info_ram_en,
	input	[8:0] dump_info_ram_addr,
	output	[31:0] dump_info_ram_data,
	output	Il_activated,
	output reg [63:0] bpm_pos_status,
	output 	[1:0] bpm_12_rx_status,
	output	[31:0] out_of_limit_cnt,
	
	input	[31:0] bm_pos_limit
);

    // AIE-ID		
    parameter  AIE_ID_A = 20 ;
    parameter  AIE_ID_B = 21 ;
    parameter  AIE_ID_C = 22 ;
    parameter  AIE_ID_D = 23 ;	
    parameter  AIE_ID_ANGLE = 24 ;


	wire	[5:0] result;
	wire	xy1, xy2;
	wire	xy1_sum, xy2_sum;
	wire	il_OffsetCalOutLatchValid;
	wire	[14:0] rt_info_x1y1, rt_info_x2y2;
	
	assign	il_LatchValid_o = il_OffsetCalOutLatchValid;
	
	
`ifdef	__DATA_32BIT__	
	
	reg	 [31:0] x1_nm_i2;	//x1 position
	reg	 [31:0] x2_nm_i2;	//x2 position
	reg	 [31:0] y1_nm_i2;	//y1 position
	reg	 [31:0] y2_nm_i2;	//y2 position
    	
	/*
	 *	Added +/- 20 mm limit
	 */
	
	wire signed [31:0] x1_nm_limit_i, x2_nm_limit_i;  // int32
    wire signed [31:0] y1_nm_limit_i, y2_nm_limit_i;  // int32
  
	wire	[3:0] out_of_limit;
	// +/- 20 mm limit values, if bigger or less then +/- 20 mm, output will 0
	aie_limit	_x1_limit
          (
		   .CLK(clk),
		   .LIMIT( bm_pos_limit ),
		   
           .pos_i(x1_nm_i),
           .valid(),
           .pos_o(x1_nm_limit_i),
           .valid_n(out_of_limit[0])
          );
		  
	aie_limit	_x2_limit
          (
		   .CLK(clk),
		   .LIMIT( bm_pos_limit ),	
		   
           .pos_i(x2_nm_i),
           .valid(),
           .pos_o(x2_nm_limit_i),
           .valid_n(out_of_limit[1])
          );
	
	aie_limit	_y1_limit
          (
		   .CLK(clk),
		   .LIMIT( bm_pos_limit ),
		   
           .pos_i(y1_nm_i),
           .valid(),
           .pos_o(y1_nm_limit_i),
           .valid_n(out_of_limit[2])
          );
		  
	aie_limit	_y2_limit
          (
		   .CLK(clk),
		   .LIMIT( bm_pos_limit ),
		   
           .pos_i(y2_nm_i),
           .valid(),
           .pos_o(y2_nm_limit_i),
           .valid_n(out_of_limit[3])
          );
		  
		  
	always @( posedge clk ) begin
	 	x1_nm_i2 <= (x1_nm_limit_i - OFFSET_H1);
	 	x2_nm_i2 <= (x2_nm_limit_i - OFFSET_H2);
	 	y1_nm_i2 <= (y1_nm_limit_i - OFFSET_V1);
	 	y2_nm_i2 <= (y2_nm_limit_i - OFFSET_V2);
	end		  

	
	// BPM Fault Detection Logic
	// 11/14/2014
	// range error count
	reg	[31:0] out_of_limit_cnt_reg;
	reg x1_pos_status, x2_pos_status;
	
	always @( posedge clk ) begin
		//if (reset | Clear ) begin	
		if ( Clear ) begin	  
			out_of_limit_cnt_reg <= 32'd0;
		end
		else if (EN == 1'b1) begin	
			if(out_of_limit != 4'b0000)
				out_of_limit_cnt_reg <= out_of_limit_cnt_reg + 1;
			else
				out_of_limit_cnt_reg <= out_of_limit_cnt_reg;
		end	
	end	
	assign	out_of_limit_cnt = out_of_limit_cnt_reg;	
	
	always @( posedge clk ) begin
		if ( Clear ) begin		//ONLY user clear
			x1_pos_status <= 1'b0;
			x2_pos_status <= 1'b0;
		end
		else if( EN && BeamCurrentStatus ) begin
			//bpm 1
			if( (x1_nm_i == 32'd0 ) && ( y1_nm_i == 32'd0) )	//both 0 (ZERO)
				x1_pos_status <= 1'b1;
			else
				x1_pos_status <= 1'b0;
			//bpm 2	
			if( (x2_nm_i == 32'd0 ) && ( y2_nm_i == 32'd0) )	
				x2_pos_status <= 1'b1;			
			else
				x2_pos_status <= 1'b0;
				
		end
	end	
	
	
	// BPM Fault status Output
	assign bpm_12_rx_status = {x2_pos_status, x1_pos_status};
		
	// 1 clock delay
	reg EN2;
	always @( posedge clk ) begin
		EN2 <= EN;
	end
	
	always @( posedge clk ) begin
		//if (reset | Clear ) begin
		if ( Clear ) begin
			bpm_pos_status <= 64'd0;
		end
		else if( EN2 && BeamCurrentStatus) begin	
			//result: any position were bad it will beam dump.	
			if( (x1_pos_status || x2_pos_status) == 1'b1) begin
				bpm_pos_status[aie_addr[5:0]]  <= 1'b1;				
			end	
			else begin//latch
				bpm_pos_status[aie_addr[5:0]] <= bpm_pos_status[aie_addr[5:0]];
			end	
		end		
	end
	
`else	
	// register mode and 26bit - 26-bit
	//
	reg	 [31:0] x1_nm_i2;	//x1 position
	reg	 [31:0] x2_nm_i2;	//x2 position
	reg	 [31:0] y1_nm_i2;	//y1 position
	reg	 [31:0] y2_nm_i2;	//y2 position
		
	always @( posedge clk ) begin
	 	x1_nm_i2 <= (x1_nm_i - OFFSET_H1[25:0]);
	 	x2_nm_i2 <= (x2_nm_i - OFFSET_H2[25:0]);
	 	y1_nm_i2 <= (y1_nm_i - OFFSET_V1[25:0]);
	 	y2_nm_i2 <= (y2_nm_i - OFFSET_V2[25:0]);
	end

`endif
	
	
	//assign LatchEnabled = EN & AIeCalValidIn;	//Calculation output latch 
	/*
	 *	ID BPM number
	 *	x, y limit value
	 *	
	 */
	//
	//	AIE-ID-B : Diamond type 
	//
	position_interlock_nm _xy1
	(
		.clk(clk),
		.reset(reset | Clear ),
		.clk_enable(EN),			//
		.x( x1_nm_i2 ),				//x1 position
		.y( y1_nm_i2 ),				//x2 position
		.posAddress( {2'b00, aie_addr} ),	
		.x_h_limit( AIOL_H1 ),		//1.5mm
		.x_low_limit( AIOL_H1 * -1 ),
		.y_h_limit( AIOL_V1 ),
		.y_low_limit( AIOL_V1 * -1 ),
		.avg_num( 16'h7fff ), 	//2/(N+1)*2^15 	0x7fff:1
		.ce_out(),
		.x_avg_out(), 
		.y_avg_out(),
		.x_h_out(),
		.x_l_out(),
		.y_h_out(),
		.y_l_out(),
		.IL_SUM(xy1_sum),
		.IL_LATCHED_OUT( xy1 ),		//latched output
		.BM_IL_ADDRESS( rt_info_x1y1 )
	);
	
	position_interlock_nm _xy2
	(
		.clk(clk),
		.reset(reset | Clear),
		.clk_enable( EN), 			//LatchEnabled),	//LatchEnabled ),
		.x( x2_nm_i2 ),				//x1 position
		.y( y2_nm_i2 ),				//x2 position
		.posAddress({2'b00, aie_addr}),
		.x_h_limit( AIOL_H2 ),		//1.5mm
		.x_low_limit( AIOL_H2 * -1 ),
		.y_h_limit( AIOL_V2 ),
		.y_low_limit( AIOL_V2 * -1 ),
		.avg_num( 16'h7fff ), 		//2/(N+1)*2^15 	0x7fff:1
		.ce_out(),
		.x_avg_out(), 
		.y_avg_out(),
		.x_h_out(),
		.x_l_out(),
		.y_h_out(),
		.y_l_out(),
		.IL_SUM(xy2_sum),
		.IL_LATCHED_OUT( xy2 ),	//latched output
		.BM_IL_ADDRESS( rt_info_x2y2 )
	);
//else

//end
//endgenerate
///////////////////////////////////////////////
	//wire [31:0] x_mm_cal, y_mm_cal, x_mrad_cal, y_mrad_cal;
	
	//
	// |(x2-x1)* (s3-s1)/(s2-s1) |<
	//	* (s3-s1)/(s2-s1)  Calculate by Microblaze
	// |(x2-x1)/(s2-s1)| = |(x2-x1)/(4-(-4))|=|(x2-x1)/8|
	//	* (s2-s1) Calculate by Microblaze
	wire x_mm_il_latched_out;
	wire y_mm_il_latched_out;
	wire x_mm_il, y_mm_il;
	
	wire   [31:0] mm_s_i;
	wire   [31:0] mrad_s_i;	
    assign 	mm_s_i   = (mode[0] == 1'b1) ? S_angle :  S_mm; 
	assign 	mrad_s_i = (mode[1] == 1'b1) ? S_angle :  S_mm; 
	
		/*
		 * Offset Calculation
		 */
	mps_aie_cal	mm		//mm
	(
		.clk(clk),
		.CalEnable(AIeCalValidIn),	//Calc
		.EN( EN ),	//LatchEnabled),			//Output Latched only Masked IDs
		.RESET(reset | Clear),	
		.mode(mode[0]),  		          //0,  1 for angle, 0 for mm
		.x1_nm_i(x1_nm_i2),
		.x2_nm_i(x2_nm_i2),
		.y1_nm_i(y1_nm_i2),
		.y2_nm_i(y2_nm_i2),	
		//
		//.s_i( mm_s_i ),		//150+3
		.s_i( S_mm ),	
		.x_set( AIOL_H3 ),	
		.y_set( AIOL_V3 ),
		.x_err(x_mm_err),
		.y_err(y_mm_err),
		.x_cal_out( x_mm_cal ),
		.y_cal_out( y_mm_cal ),
		.x_pre_il( x_mm_il ),
		.y_pre_il( y_mm_il ),		
		.x_il( x_mm_il_latched_out ),
		.y_il( y_mm_il_latched_out ),
		.datavalid(datavalid),
		.il_valid(il_OffsetCalOutLatchValid)
	);	
	

	//Angle calculations
	wire x_mrad_il_latched_out;
	wire y_mrad_il_latched_out;
	wire	x_mrad_il, y_mrad_il;
	wire	il_AngleCalOutLatchValid;
	mps_aie_cal	mrad_angle	//angle
	(
		.clk(clk),
		.CalEnable( AIeCalValidIn ),	//
		.EN( EN ),	
		.RESET(reset | Clear),	
		.mode(mode[1]),      //  1,   1 for angle, 0 for mm		
		.x1_nm_i(x1_nm_i2),
		.x2_nm_i(x2_nm_i2),
		.y1_nm_i(y1_nm_i2),
		.y2_nm_i(y2_nm_i2),	
		//		
		//.s_i( mrad_s_i ),
		.s_i( S_angle ),	
		.x_set( AIAL_H ),	//63
		.y_set( AIAL_V ),
		.x_err(x_mrad_err),
		.y_err(y_mrad_err),
		.x_cal_out( x_mrad_cal ),
		.y_cal_out( y_mrad_cal ),
		.x_pre_il( x_mrad_il ),
		.y_pre_il( y_mrad_il ),
		.x_il( x_mrad_il_latched_out ),
		.y_il( y_mrad_il_latched_out ),
		.datavalid(),
		.il_valid(il_AngleCalOutLatchValid)	
	);	
	
	
	assign result[0] = xy1_sum;	//xy1;							
	assign result[1] = xy2_sum;	//xy2;							
	assign result[2] = x_mm_il;	//x_mm_il_latched_out;
	assign result[3] = y_mm_il;	//y_mm_il_latched_out;			//mm	
	assign result[4] = x_mrad_il;	//x_mrad_il_latched_out;
	assign result[5] = y_mrad_il;	//y_mrad_il_latched_out;		//angle
	
	assign debug = { x2_pos_status, x1_pos_status, y_mrad_il, x_mrad_il, y_mm_il, x_mm_il, xy2_sum, xy1_sum };
	//latched status
	assign Il_latched_s = { y_mrad_il_latched_out, x_mrad_il_latched_out, y_mm_il_latched_out, x_mm_il_latched_out, xy2, xy1 };	
/*	
 * 		diamond (for AIE-ID-B & AIE-BM)			: 10
 * 		rectangular (for AIE-ID-A)				: 20
 * 		rectangular + optimal offset (AIE-ID-C)	: 21
 * 		rectangular + small offset (AIE-ID-D)	: 22
 */
	reg	[15:0] dump_out[64:0];
	reg [63:0] RegAie_bm, RegAie_A, RegAie_C, RegAie_D;
	reg [63:0] d_out;
	reg		il_we;
	reg		IL_mon;
	always @( posedge clk ) begin
		if(Clear == 1'b1 | reset == 1'b1) begin
			// Clean all registers by USER
			IL_mon    <= 1'b0;
			RegAie_bm <= 64'h0;
			RegAie_A  <= 64'h0;
			RegAie_C  <= 64'h0;
			RegAie_D  <= 64'h0;
			d_out <= 64'h0;;
			dump_out[0] <= 10'd0;
			dump_out[1] <= 10'd0;
			dump_out[2] <= 10'd0;
			dump_out[3] <= 10'd0;
			dump_out[4] <= 10'd0;
			dump_out[5] <= 10'd0;
			dump_out[6] <= 10'd0;
			dump_out[7] <= 10'd0;
			dump_out[8] <= 10'd0;
			dump_out[9] <= 10'd0;
			dump_out[10] <= 10'd0;
			dump_out[11] <= 10'd0;
			dump_out[12] <= 10'd0;
			dump_out[13] <= 10'd0;
			dump_out[14] <= 10'd0;
			dump_out[15] <= 10'd0;
			dump_out[16] <= 10'd0;
			dump_out[17] <= 10'd0;
			dump_out[18] <= 10'd0;
			dump_out[19] <= 10'd0;
			dump_out[20] <= 10'd0;
			dump_out[21] <= 10'd0;
			dump_out[22] <= 10'd0;
			dump_out[23] <= 10'd0;
			dump_out[24] <= 10'd0;
			dump_out[25] <= 10'd0;
			dump_out[26] <= 10'd0;
			dump_out[27] <= 10'd0;
			dump_out[28] <= 10'd0;
			dump_out[29] <= 10'd0;
			dump_out[30] <= 10'd0;	
			dump_out[31] <= 10'd0;			
			dump_out[32] <= 10'd0;
		end	

		/* Beam is over 2 mA */
		else if ( il_OffsetCalOutLatchValid && BeamCurrentStatus )
		begin 	
		
			// Diagnostics
			//if( IL_mon && (result[0] || result[1] || result[2] || result[3] || result[4] || result[5]) ) begin	//first bpm information only
			if( (result[0] || result[1] || result[2] || result[3] || result[4] || result[5]) ) begin
				dump_out[aie_addr]       <= {4'b0000, debug, aie_addr[5:0]};
				RegAie_bm[aie_addr[5:0]] <= (xy2_sum | xy1_sum);
				RegAie_A[aie_addr[5:0]]  <= (y_mm_il | x_mm_il | x_mrad_il | y_mrad_il);
				RegAie_C[aie_addr[5:0]]  <= (y_mrad_il | x_mrad_il);
				RegAie_D[aie_addr[5:0]]  <= (y_mrad_il | x_mrad_il | y_mm_il | x_mm_il | xy2_sum | xy1_sum);
				IL_mon <= 1'b1;	// NO MORE UNITIL USER RESET
			end	
			else begin	//Latched until USER RESET
				dump_out[aie_addr]       <=	 dump_out[aie_addr];
				RegAie_bm[aie_addr[5:0]] <=  RegAie_bm[aie_addr[5:0]];
				RegAie_A[aie_addr[5:0]]  <=  RegAie_A[aie_addr[5:0]];
				RegAie_C[aie_addr[5:0]]  <=  RegAie_C[aie_addr[5:0]];
				RegAie_D[aie_addr[5:0]]  <=  RegAie_D[aie_addr[5:0]];
				IL_mon <= IL_mon;	//10/07/14
			end
				
			

			///////////////////////				
			// AIE-A RECTANGULAR
			// 
			///////////////////////	
			if( logic[7:0] == AIE_ID_A ) begin				
				if( (y_mm_il || x_mm_il || x_mrad_il || y_mrad_il) == 1'b1 )
					d_out[aie_addr[5:0]]     <= 1'b1;
				else
					d_out[aie_addr[5:0]]     <= d_out[aie_addr[5:0]] ;
			end	
			
			//////////		
			// AIE-B DIAMOND
			//////////	
			if( logic[7:0] == AIE_ID_B ) begin
				if( xy2_sum | xy1_sum )
					d_out[aie_addr[5:0]]     <= 1'b1;
				else	
					d_out[aie_addr[5:0]]     <= d_out[aie_addr[5:0]] ;
			end

			
			//////////			
			// AIE-C
			//////////	
			if( logic[7:0] == AIE_ID_C  ) begin
				if( xy2_sum | xy1_sum | y_mrad_il | x_mrad_il )
					d_out[aie_addr[5:0]]     <= 1'b1;
				else
					d_out[aie_addr[5:0]]     <= d_out[aie_addr[5:0]] ;					
			end	

			//////////
			// AIE-D
			//////////
			if( logic[7:0] == AIE_ID_D ) begin
				if(result[0] | result[1] | result[2] | result[3] | result[4] | result[5] )
					d_out[aie_addr[5:0]]     <= 1'b1;
				else
					d_out[aie_addr[5:0]]     <= d_out[aie_addr[5:0]] ;					
			end		
			
			///////////////
			//TEST 1 mm
			/*
			if( logic[7:0] == 8'h30 ) begin	
				if( x_mm_il | y_mm_il ) 
					d_out[aie_addr[5:0]]     <= 1'b1;
				else
					d_out[aie_addr[5:0]]     <= d_out[aie_addr[5:0]] ;						
			end
			*/
			/////////////////
			//TEST 2 ANGLE			
			if( logic[7:0] == AIE_ID_ANGLE ) begin	//ANGLE TEST
				if( x_mrad_il | y_mrad_il )
					d_out[aie_addr[5:0]]     <= 1'b1;
				else
					d_out[aie_addr[5:0]]     <= d_out[aie_addr[5:0]] ;			
			end			
			
			
		end			
	end
	
	//assign  debug = { y_mrad_il_latched_out, x_mrad_il_latched_out, y_mm_il_latched_out, x_mm_il_latched_out, xy2, xy1};	
	//
	assign AI_ID_RfDumpOut  = d_out;
	assign Aie_bm_o = RegAie_bm;
	assign Aie_A_o  = RegAie_A;
	assign Aie_C_o  = RegAie_C;
	assign Aie_D_o  = RegAie_D;		
	assign Il_activated = IL_mon;
	
	//one clock delay
	always @( posedge clk ) begin
		il_we <= il_OffsetCalOutLatchValid;
	end
	
	posDpram_32x512 	dump_info (	
		.clka(clk), // input clka
		.ena(1'b1), // input ena
		.wea( il_we ), //Must need WR
		.addra({3'd0, aie_addr} ), // input [8 : 0] addra
		.dina( { 16'd0, dump_out[aie_addr] } ), 
		//read by uBlaze
		.rstb( Clear ),
		.clkb(clk), 	// input clkb
		.enb(dump_info_ram_en), 	// input enb
		.addrb(dump_info_ram_addr), 		// input [8 : 0] addrb
		.doutb(dump_info_ram_data) 		
	);	
	

	reg	[31:0] CNT;
	always @( posedge clk ) begin
		if ( cnt_rst == 1'b1 )	//count reset
			CNT <= 32'd0;
		else if (EN == 1'b1) CNT <= CNT + 1;
		else
			CNT <= CNT;
	end
	wire   [3:0] addr_out; 
	mux16 mux_SerialData (
		.clk(clk), 
		.reset(reset), 
		.trig( EN ), 		// 0 - 60 full 
		.clk_enable(1'b1), 
		.a1({8'h5C, 16'd0, aie_addr} ), 
		.a2({ logic, 16'd0,  1'b0,1'b0, debug } ), 
		.a3(x1_nm_i2), 		//x1 - offset
		.a4(y1_nm_i2), 
		.a5(x2_nm_i2), 
		.a6(y2_nm_i2), 
		.a7(x_mm_cal), 
		.a8(y_mm_cal), 
		.a9(x_mrad_cal), 
		.a10(y_mrad_cal), 
		.a11( d_out[31:0] ), 
		.a12( {17'd0, rt_info_x1y1}), 
		.a13( {17'd0, rt_info_x2y2}), 
		.a14( 32'd0 ), 
		.a15( 32'd0 ), 
		.a16( CNT ), 			//count
		.len( 4'd16 ), 		//length
		.addr(mux_addr), 	//manual address when dir = 1
		.dir( mux_dir ), 	//1:external address, 0: free running
		.ce_out(), 
		.Data_o(mux_Data_o), 
		.we(muxValid),
		.addr_out(addr_out)
	);	
	

		
	//Cell_status[31:0]
	// Check, ID-AI ON or OFF
	/*
	Active Interlock (AI) On/Off Status depends on SR Operation Mode
	??SR Safe Current
	??ID gap Status (open/closed)
	??Front End BM & ID Photon Shutters (open/closed)
	??AI On/Off Status will be defined automatically
	
	OFF [Gap=Fully Open]
	OFF [I?? & Gap=Closed]
	ON [I>2 & Gap=Closed]
	ON [IDPhSh=Open]
	*/

	

//`define	__CHIPSCOPE_ENABLE	
`ifdef	__CHIPSCOPE_ENABLE
	
	wire [35:0] CONTROL0;
	wire [255:0] trigger0;

		
	ICON_CH1 usrIcon1(
        CONTROL0
        ) /* synthesis syn_black_box syn_noprune=1 */;
        
	///////////////////////////	
	Ila usrIla1(
		.CONTROL(CONTROL0),
		.CLK(clk),
		.TRIG0(trigger0)
	);
	
	assign trigger0[31:0]       = x1_nm_i2; 
	assign trigger0[63:32]      = x2_nm_i2; 
	assign trigger0[95:64]      = y1_nm_i2; 
	assign trigger0[127:96]     = y2_nm_i2;
	
	assign trigger0[134:128]    = aie_addr[6:0];
	assign trigger0[135]        = AIeCalValidIn;
	assign trigger0[136]        = EN;	
	assign trigger0[137]        = il_OffsetCalOutLatchValid;
	assign trigger0[138]        = x1_pos_status;
	assign trigger0[139]        = x2_pos_status;
	assign trigger0[171:140]    = x_mm_cal;	//x_mrad_cal;	//OFFSET_H1;	//bpm_pos_status[31:0];
	assign trigger0[203:172]    = y_mm_cal;	//y_mrad_cal;	//OFFSET_V1;	//mux_Data_o;
	assign trigger0[204]        = muxValid;
	assign trigger0[210:205]    = { y_mrad_il, x_mrad_il, y_mm_il, x_mm_il, xy2_sum, xy1_sum };
	assign trigger0[218:211]    = d_out[7:0];
	assign trigger0[226:219]    = logic[7:0];
	assign trigger0[230:227]    = addr_out;
	assign trigger0[234:231]    = out_of_limit;
	assign trigger0[250:235]    = out_of_limit_cnt_reg[15:0];	
	
	assign trigger0[253]        = IL_mon;
	assign trigger0[254]        = il_AngleCalOutLatchValid;
	//
	assign trigger0[255]        = trig;
		
`endif
//////////////	

endmodule 

