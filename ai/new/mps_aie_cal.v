/*
 * Rectaangler
 *
	%  mm  :  
		s1 = -4 m, s2 = 4 m, s3 = 0
		S = ((s3-s1)/(s2-s1) ) * 10000   % for integer conversion
		
		x_nm = |(x2-x1)/ S | <
			|(x2-x1)/(4-(-4))|=|(x2-x1)/8| 
		x_nm = x_nm * 0.0001		%RESULT	
 */
`timescale 1 ns / 1 ns

module mps_aie_cal
(
	input	clk,
	input	CalEnable,
	input	EN,
	input	RESET,
	input	mode,
	input	 [31:0] x1_nm_i,
	input	 [31:0] x2_nm_i,
	input	 [31:0] y1_nm_i,
	input	 [31:0] y2_nm_i,	
	input	 [31:0] s_i,		// uBlaze setting value  X 10000
	input	 [31:0] x_set,
	input	 [31:0] y_set,
	//
	output	 [31:0] x_err,
	output	 [31:0] y_err,
	output	 [31:0] x_cal_out,	//cal output
	output	 [31:0] y_cal_out,	//cal output
	output	x_pre_il,
	output	y_pre_il,
	output	x_il,
	output	y_il,
	output  datavalid,
	output	il_valid

);

	wire	 [31:0] nm_err_x, nm_err_y;
	wire	 [31:0] nm_pos_x, nm_pos_y;

	wire	x_il_valid, y_il_valid;
	wire	x_datavalid, y_datavalid;
	assign  il_valid = x_il_valid;		// | y_il_valid;		//05/18/15 added x and y
	assign	datavalid = x_datavalid;	// | y_datavalid;
	
	
	//XXXXX
	sysgen_mm_cal 	x_mm_ang_cal (
		.cal_enable( CalEnable ),	
		.ce_1(1'b1), 
		.clk_1(clk), 
		.enable(EN),
		.datavalid(x_datavalid),
		.h_out(), 
		.hi_limit(x_set), 
		.il(x_pre_il), 
		.il_latched_o(x_il),
		.l_out(), 
		.low_limit(x_set * -1 ), 
		.nm_err_s32_o(nm_err_x), 
		.nm_o(nm_pos_x), 
		.rst(RESET),
		.s_i(s_i),
		.sel_angle_mode(mode), 
		.s_o(),
		.x1_nm_i(x1_nm_i),
		.x1_nm_i1(x1_nm_i),  
		.x2_nm_i(x2_nm_i),
		.il_valid(x_il_valid)
	);
	
	//YYYYYY
	sysgen_mm_cal 	y_mm_ang_cal (
		.cal_enable( CalEnable ),	
		.ce_1(1'b1), 
		.clk_1(clk), 
		.enable(EN),
		.datavalid( y_datavalid ),
		.h_out(), 
		.hi_limit(y_set), 
		.il(y_pre_il), 
		.il_latched_o(y_il),
		.low_limit(y_set * -1 ), 
		.nm_err_s32_o(nm_err_y), 
		.nm_o(nm_pos_y), 
		.rst(RESET),
		.s_i(s_i), 
		.sel_angle_mode(mode),    //mm
		.s_o(),
		.x1_nm_i(y1_nm_i),
		.x1_nm_i1(y1_nm_i),  
		.x2_nm_i(y2_nm_i),
		.il_valid( y_il_valid )
	);
	
assign	x_err = nm_err_x;
assign	y_err = nm_err_y;
assign	x_cal_out = nm_pos_x ;	
assign	y_cal_out = nm_pos_y ; 
	

endmodule	