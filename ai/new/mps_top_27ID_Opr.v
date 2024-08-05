`timescale 1ns / 1ps

/*
	
********************************************************************
Machine protection System which called Active Interlock System

        Designed and Implemented by NSLS-II Controls group
        Kiman Ha      kha@bnl.gov
              ext) 8058
********************************************************************
		
    8/27/13
            wire [15:0] k_addr = plb_WR_Reg7[15:0];
            fifo xy_pos_mon_fifo added for position x, y monitoring


    7 NSLS-II Project Beamlines (DOE), Operation to begin in 2015
            ?Inelastic X-ray Scattering (IXS)
            ?Hard X-ray Nanoprobe (HXN)
            ?Coherent Hard X-ray Scattering (CHX)
            ?Coherent Soft X-ray Scat & Pol (CSX1, CSX2)
            ?Sub-micron Res X-ray Spec (SRX)
            ?X-ray Powder Diffraction (XPD)

    //////
    8/9/13
            .aie_addr_gen() added after simulation
            User enable/disable ID number
                    assign aie_USET_MASK_SETTING = {plb_w_reg[5], plb_w_reg[4]};
    9/3/13
            plb_w_reg[13][27]   -- DAC lookup write
            plb_w_reg[13][28]   -- Output latch clear
            plb_w_reg[13][29]   -- NPI 1 write enable/disable
            plb_w_reg[13][30]	-- PM user reset
            plb_w_reg[13][31]	-- Fault trigger test
            plb_w_Reg12[23:8]	-- npi reset periode


    9/4/13	Create project 1.2  10:00 AM
            Added npi_burst_0 in the EDK


    plb_r_reg21  Reference DPRAM out A read


    BEAM STATUS:
            xy_pos_Reg[76][16]  - Beam current 2 mA status from Cell #3
            xy_pos_Reg[76][17]  - Beam current 50 mA status from Cell #3
            xy_pos_Reg[76][18]  - 25 Hz clock for PLC heart beat


    ID Gap status:
		
    ///////////////////////////// Rev 5 Table //////////////////////
    12/18/13
            H1,H2,H3/V1,V2,V3	offset setting changed by Petr's Rev 5

    change log:
            1) ram_cs1[10] -> ram_cs1[12]  iL_enable[ram_addr1] <= ram_Data1		12/18/13 changed cs
            2) new signal added for Rev 5 calculation
                    input	 [31:0]	AIOL_H1,
                    input	 [31:0]	AIOL_V1,
                    input	 [31:0]	AIOL_H2,
                    input	 [31:0]	AIOL_V2,
                    input	 [31:0]	AIOL_H3,
                    input	 [31:0]	AIOL_V3,
                    input	 [31:0]	AIAL_H,
                    input	 [31:0]	AIAL_V,

    1/13/14
        Local packet trigger used Trig1Start
	
02/01/14	
    plb_Reg10[31:28]
    plb_Reg10[27:24]
    plb_Reg10[14]	ID dump enable control
    plb_Reg10[15]	BM dump enable control
    plb_Reg10[23:16]


    ram_cs2[0] - sr_bpm_ref_offset_x
    ram_cs2[1] - xy_Mask Ram control
    ram_cs2[2] -
    ram_cs2[3] - wfm_fifo_we
    ram_cs2[4] - sr_bpm_ref_offset_y
    ram_cs2[5] - sr-bpm origin offset xy
    ram_cs2[6] - u_fifo_rd                     10/07/14
    ram_cs2[7] - uBlaze x,y position ram write
    ram_cs2[8] - u_fifo_rd  ---NOT USED
    ram_cs2[9] - xy_err_fifo_rd
    ram_cs2[15:10] - FPGA Calc result read
	
02/02/2014
    Npi_0_Data1[7:0]   = xy_pos_read  -- npi port assign that single position data
	
	
02/04/14
 external input bit address
    .beam current

    plb_w_reg[15][9:0]	- position address
    plb_w_reg[15][31:16]
	
///////////////////////	
02/16/2014
    1) timing correction for GTX2 tx packet
	   assign SdiDataRdValid = tmpSdiDataRdValid[1] && bpmPosDataValid;
	2) Digital output added signal inverting       
	   assign ExtDout_pin = ~ext_dout_buf;		Digital Output inverting
	3) gtx2 port test for external zynq interface
	   glb_gtx2_top  :  cw/ccw received head test.
	4) external herterbit watchdog added   
	   io_watchdog_out   added for external heartbit test
	
03/06/2014
    1) make pblock, softReboot module it dosent worked.
	
	** 03/10/14 Installed CELL31 and worked well
	
   
03/14/2014
	1) evr ts latched timestamp changed
	2) pm_npi_enable signal added to reg22 for postmortem status monitorin.
	3) added ext_dout_buf[9]   = FINAL_DUMP_OUT;   for Cell 32 interface
	4) LEMO2 Output added evr_TRIGGER_0

03/19/2014
	1) cw/ccw local packet delay
	2) Remote head packet checking routine
	
03/23/2014
	1) NPI_BURST_0  data aind logic changed
	2) idbpm_lookup.v  Cell23-2 bpm address added
	3) plb_w_Reg12[28] select npi_0 source
	4) gap_status_info added for enable gap status
	
 03/26/2014
	FIRMWARE INSTALLED 03/25/2014
	1) Added ~FINALOUT  
		Real operation:
		1 : OK, 0 : DUMP
		
  //////////////
  03/27/2014
    1) added sr-bpm reference is sr_bpm_ref_offset_xy_set


    //////////////
    03/28/2014
        1) tested sr-bpm ofsset BRAM with chip scope
                .Individual reference setting for 360 x,y positions
        2) added PhotonShutters_Status_info status reading and assign bit address

        3) x,y chamber offset ram added for ID interlock
                .New requirement
        4) npi burst address generator added
                .control width and shift address
        5) ID bpm Interlock Run/Stop register added
                .Automatically control Run/Stop
        6) BR-bpm & ID interlock condition logic added

    03/31/2014
        1) added sr-bpm orignin offset BRAM added
                sr_bpm_origin_offset_xy_set
                .x( bpmPos_Xy_Data - sr_bpm_origin_offset_xy_set ),

    04/11/2014
        for enable/disable selected BPM
        1) assign 	bpmPos_Xy_Data = (xy_Mask_DataValid == 1'b1) ? bpmPos_Xy_Data_raw : 32'd0;

    05/20/2014
        1) added slow dump and fast dump fore requirement
        2) aie_USET_MASK_SETTING_REG added for
        3) Fast value control logic added
	
    05/22/2014
            CellPLC_BmBeamDump && plb_Reg10[12] and plb_Reg10[13] for Enable/Disable Fast DUMP
    05/29/2014
            1) bm_iL_info[BM_IL_ADDRESS[9:0]] <= 1'b1;   corrected address 9:0, it was 8:0
                            - cell 30 y status not changed.
            2) .EXT_TRIG_B( plb_Reg10[11] ),  watchdog reset
    05/30/2014
            1) SysMonData[CELL30]  address corrected
            2) OUTPUT polarity changed
            3) position_interlock_nm added logic that enable
    05/31/14
            1) assign BM_DUMP =   BM_LATCHED_OUT ;	//05/31/14  Keep latch until your reset
    06/04/14
            1) assign FINAL_DUMP_OUT = SLOW_DUMP || FAST_DUMP ;

    08/25/14
            1) BM_PhotonShutters_Status added for new requirement

    08/26/14
            1) PM waveform changed

            2) NPI-0  POST MORTERM
               NPI-1  BPM POSITION


    09/02/14
            1) added XyPosition_26bitFormatData
            2) Gapstatus  0 : closed,   1: opened

    09/03/14
            1) LEMO OUT 4  (ext_out[3])
                    LEMO IN 1   open    ->  0
                    LEMO IN 1   0       ->  0
                    LEMO IN 1   1       ->  1

    09/22/14
            SysMonData[65] <= CellPLCWdStatus;
            SysMonData[66] <= CellPLC_IdBeamDump_Status;
            SysMonData[67] <= CellPLC_BmBeamDump_Status;

    09/23/14
            1) AI-ID logic correction
            2) PLC WD test
    09/25/14
            1) ID_BpmPosValueStatus register added for monitoring bpm error or sdi error detection.

    10/03/14
            1) changed shutter status, gap status ... address order is same as "aie_addr"
            2) plb_w_reg[5][aie_addr]  for AI-ID enable
            3) Added ID_BpmFailStatusLatch status

    10/04/14
            1) (ID_BpmFailStatusLatch && plb_Reg10[12] )
            2) plb_Reg10[12]  is Canting and ID_BpmFailStatusLatch enable
    10/07/14
            1) if( (y_mm_il || x_mm_il || x_mrad_il || y_mrad_il) == 1'b1 )  AIE-A mode

        2) u_fifo_out added for MUX FIFO Read
            3) ID_CalMux_mon_fifo
    10/08/14
            1) ENABLE Logic changed
            2) PhotonShutters_Status_info    = (plb_w_reg[13][2]

            3) .clk_enable( xy_Mask_DataValid && (dcct_50mA && !BM_LATCHED_OUT)  ),  //position interlock if BM is Safe ?

    10/10/14
            bm bpm fault detection need test.
            1) bm_bpm_data_fail_stats  added for lemo output
            2) bpm_id_12_rx_status


    10/11/14
            1) ( ID_BpmFailStatusLatch && plb_w_reg[13][5] )    -- ID bm fault enable
            2) reg  [31:0]  SysMonData[82:0];    70 -> 82
            3) bm_err_count <= 8'd0;  bm error count error   - plb_r_reg24
            4) assign FAST_DUMP = (ID_DUMP || plb_w_reg[13][6]) && plb_Reg10[14] ;
            5) if( (ID_BpmPosValueStatus[31:0] & plb_w_reg[5] ) != 32'd0) begin

    10/13/14
            1) NPI 1 is 8 bpm
            assign npi_fifo_wr_en = pm_npi_wr_valid & plb_w_reg[13][29] & UsrTrig_NipStrobe_Synced;

    10/14/14
            1) NPI wr one clock delay from fifo ~empty

    10/15/14
            1) .evg_soft_event( evr_TRIGGER_7 || evr_TRIGGER_6 ),     trig 7 and 6  both
            2) t_CNT   npi user trigger count added


    11/05/14
       Added 0.2mA status from PLC and PV
       1) ( (plb_w_Reg12[3] || dcct_2mA || dcct_0_2mA) && PhotonShutters_Status_info[aie_addr] )

	   
	   
    /////////////////////////////////////////////
    11/12/14
            1) firmware update logic work OK with BEAM.
            2) logic Ok.

    11/13/14
            AI-ID added data limit +/- 20 mm limit
            CM_Status 5  -> [1][2]

            added ID bpm out of range error and assigned plb_r_reg20 = id_out_of_limit_cnt


    11/20/14
    Tested at #23 with real machine
            1) dcct_plc_hb added to reg24[19] for monitoring

    plb_w_Reg12[3]  - 0.2 mA status from uBlaze  --- not used
    plb_w_reg[13][6]  is monitoring by SysStatus reg


    11/22/2014
            added :
                    SysMonData[72] <= Cell_PLC_WD_Status;		//11/22/14
                    SysMonData[73] <= cell_plc_wd_timeout;
                    Cell PLC watch Dog timer added
                    ublaze_watchdog arme when reset


    11/30/2014
            1) plb_Reg10[13]   CellPLC dump enable/disable   REMOVED

    12/02/14
            1) active_intlock_process : .BeamCurrentStatus( (dcct_2mA || dcct_0_2mA) && IDAI_RunStopStatusReg[aie_addr] ),


    12/15/14
            1) C8 and C18 address corrected for Gap, PS, PLC

    12/19/14
            Added Cell 8 and Cell 18 PLC WatchDog status
            assign	  Cell_PLC_WD_Status[6] = CellPLCWdStatus[8];	//#8
            assign	  Cell_PLC_WD_Status[7] = CellPLCWdStatus[9];	//#18

    02/08/2015
            .clk_enable( ~FINAL_DUMP_OUT && xy_Mask_DataValid && (dcct_50mA && !BM_LATCHED_OUT)  ),
            .BeamCurrentStatus( ~FINAL_DUMP_OUT && (dcct_2mA || dcct_0_2mA) && IDAI_RunStopStatusReg[aie_addr] ),


    02/18/2015
            assign ext_dout_buf[5]   = fp_lemo_in[1];	//PM Reset to EVG
            .usr_rst(plb_w_reg[13][30], evr_TRIGGER_5 ),


    02/24/2015
            re mapping external output

    03/03/15
            .usr_rst(plb_w_reg[13][30], evr_TRIGGER_6 ),	trigger 5 not respond

    03/19/2015
            added BPM BPM fault detection logic
            assign BM_DUMP =   BM_LATCHED_OUT || (BM_ERR_DUMP && plb_Reg10[8]);

            plb_w_reg[13][19:10]  is number of bpm errors set

            assign ext_dout_buf[11]  = bm_bpm_err_status;

    ///////////////////////////////////////////////////////////////
    03/22/15
            assign plb_r_reg28 = bm_bpm_err_counts;

    Beam test is OK, 03/24/15
    Known issue:
       NO bpm ID infomation when bpm fault received.

    ////////////////////////////////////////////////////////////////
	
    03/25/15
    position_interlock_nm
            .y( bpmPos_Xy_Data ),			// 03/25/15 for zero detection


    03/26/15
            RF trip readback timestamp added
            rf_trip_st_Trig
                    SysMonData[77] <= TS_time[63:32];	//sec
                    SysMonData[78] <= TS_time[31:0];	//nsec

    03/30/15
            bm_bpm_fault_detected   added
            position_interlock_nm.v
                    bm_bpm_fault_detected signal just monitoring.
    03/31/15
            1)
            position_interlock_nm timing correction
            added position_interlock_nm
            2)
            else if ( IL_SUM == 1'b1 | bm_bpm_fault_detected == 1'b1 ) begin   for LED display.


            2 weeks working well


     * 04/13/15
     * Rf trip signal detected ? Stop update
     * Read by PM client software
     *

    assign FINAL_FREE_DUMP_OUT = ~(BM_DUMP || ID_DUMP) ;
    assign COD_Ram_WR_ENABLE   = FINAL_FREE_DUMP_OUT && xy_Mask_DataValid
    assign SDI_Reg_WR_ENABLE   = FINAL_FREE_DUMP_OUT && SdiDataRdValid;
		
		
		
		
    05/05/2015
            1) #3 re assign digital inputs
		
#	CC input bits	Description	Action  (Sergei/ Kiman ??please confirm	CC input signal level
1	10	I> SR stored beam current threshold for ID shutter  (present value 0.2 mA)	Activate ID beam envelope	1: < 0.2mA           0: > 0.2mA
2	11	I> SR stored beam current for ID (present value 2 mA)	Activate ID beam envelope	1: < 2mA             0:  > 2mA
3	12	I>SR stored beam current threshold for BM SR protection (present 50 mA)	Activate BM beam envelope
 	1: < 50mA             0:  > 50mA
4	13	I> Maximum SR stored beam current 	Dump SR stored beam	1: < 500mA           0: > 500mA
5	14	HB fault	Dump SR stored beam	1: Normal           0: Fault
6	15	System fault	Dump SR stored beam	1: Normal           0: Fault
7	16	Large beam loss - > 50% in < 1 msec	Capture beam data history	1: Normal           0: Loss




        assign plb_r_reg24 = {  dcct_beam_loss
        .falt_trig( plb_w_reg[13][31] || FINAL_DUMP_OUT || dcct_beam_loss ),


05/15/2015
    1) Cell 16 and 17 ABIX beamline added and lookup table changed		
	2) DCCT Dump timestamp added
	
	
06/03/2015
	1) All logic tested
	2) DCCT beam loss timestamp added
	
	3) changed address beam loss timestamp sec and nsec	
			SysMonData[71] <= TS_time[31:0];	//nsec
			SysMonData[70] <= TS_time[63:32];	//sec	
			
	4) #16 and #17 PLC signal routed.
	5) Canting magnet status also added
	
	6) Cell PLC dump added individual register  'CellPLC_IdBeamDumpOutReg'
		assign FINAL_DUMP_OUT = SLOW_DUMP || FAST_DUMP || CellPLC_IdBeamDumpOutReg;
	
06/08/15	
	SysMonData[74] <= bm_spike_cnt;	
	pm triger generate slow/fast 
	.falt_trig( plb_w_reg[13][31] || SLOW_DUMP || FAST_DUMP || dcct_beam_loss ),

	
06/10/2015	
	.bm_pos_limit( plb_w_reg[8] )
	if(	(!CellPLC_IdBeamDump_Status[aie_addr] && plb_w_reg[5][aie_addr] &&  dcct_0_2mA ) == 1'b1 ) begin	
	
	
06/11/15
		1) added debounceIndex
	assign	AI_BM_RfTripCmd    = (plb_w_reg[15][12] == 1'b1) ? AI_BM_RfTripDebounce5CycleCmdReg : BM_LATCHED_OUT;
	assign  BM_LATCHED_OUT_RST = (plb_w_reg[15][12] == 1'b1) ? Trig1Start : 1'b0;	// every 10 kHz clear latched status


06/14/15
		Tested at BPM Lab
		assign plb_r_reg22 = {t_CNT[7:0], 16'd0, 1'b0, AI_BM_Fifo2CycleCompErrorCntReg, AI_BM_Fifo2CycleCompDblErrorReg, ai_active, pm_npi_enable, 1'b0, 1'b0, 1'b0};	
	
	
06/16/2015
		*** BEAM TEST
	- 5 cycle debaunce tested
		* debaounce module count number is not matched I gaiv 1 for delay value, but real measurement result is 5 cycle.
		
		
	Changed:	
	comp2points module active signal change logic	
	1) 	.activate( ~FINAL_DUMP_OUT && (dcct_50mA && !AI_BM_RfTripCmd)

	
06/18/2015
	1) AI-ID debaounce added
	2) plb_Reg10[13]  for PLC Fast dump enable/disable
	3) AI-ID logic added glitch detection logic (output delay)

	
//////////////////////////////////////////////////////////////////
	Found one clock glitch with BM and ID bpms.
	-16.7 mm, -8. mm, 200 ~ 500, -1.72 mm...
	
06/20/2015
	1) gletch_ignore _ID_trip timing test OK, 2 cycle delay
	2) gletch_ignore _BM_trip
	
	
07/05/2015
	1) orbit rms added
	
07/09/15
	PM fault trigger, 
	DCCT beam loss disabled because PLC reset issue.
		.falt_trig( plb_w_reg[13][31] || AI_ID_RfTripCmd || BM_DUMP || CellPLC_IdBeamDumpOutReg ),
		
		
	PM generator update		

07/14/15	
	SysMonData[76] <= y_orbit_rms;
	
07/24/15
	plb_w_reg[15][14]	- select ID normal/2 cycle mode 
	
	
09/09/2015	
	Added Cell3 Canting Magnet status (Signals added to Cell 4 Cell Controller Inputs )	

10/12/2015
	added "bpm_id_12_rx_status[1:0]" for monitoring error status
	assign plb_r_reg22 = {t_CNT[7:0], 15'd0, -> assign plb_r_reg22 = {t_CNT[7:0], bpm_id_12_rx_status, 13'd0,	
	
10/26/2015
	FAST DUMP added CellPLC_IdBeamDumpOutReg

	
10/27/2015
	IDBPM_LOOKUP  #4 for NEXT beamline and  PLC status

11/03/2015
	1) bm_position_interlock_nm.v added error count
	2) aie_limit.v defaulr pos 1000 nm
	3) PM falt stop count changed .Ref_CNT(32'd1000),	
	
	
11/06/2015	
	Added Beam status for RF cavity
	assign ext_dout_buf[10]  =  (dcct_2mA & dcct_0_2mA);	//ext_heartbit_2kHz;	//(ext_heartbit_2kHz && ~io_watchdog_out);		//2 kHz free running
	assign ext_dout_buf[11]  = ~(dcct_2mA & dcct_0_2mA);
	
	
11/17/2015
	1) Lemo output added AI-ID status	

11/19/2015
	assign ext_dout_buf[11]  = ~(dcct_2mA & dcct_0_2mA & FINAL_DUMP_OUT)
	
12/03/2015 
	Machine test version
	1) IDAI_BeamDumpControlReg not corredtly working so reduced signal 
        IDAI_BeamDumpControlReg[aie_addr] <= 	(
                        //IDAI_RunStopStatusReg[aie_addr] &&
                        aie_DumpStatus[aie_addr]
                        );
	2) ID_DUMP signal wason't reset
	3) epics 48 1 , 
	assign  AI_ID_LatchedOutReset = (plb_w_Reg12[1] == 1'b1) ? AI_ID_ResetTrig : 1'b0;	
	4)plb_w_reg[4] 	User reset trigger delay test
	
12/04/2015
	1) .falt_trig( plb_w_reg[13][31] || AI_ID_RfTripCmd || BM_DUMP || ID_DUMP || bm_il_sum ),
	
	C23 test with #23-bpm8 with glitch function.
	Need changd debounce function.
	2)
	bm_debounce 	AI_ID_trip0 modified clk 10 kHz -> sysClk, variable counts
	
12/06/2015
	BM fault count logic same as AI-ID
	bm_position_interlock_nm
	
	
12/09/2015
	removed unused logics
	aiPM_Trigger   added
	
12/11/2015
	Added ID 12 and ID 21 logics
	SysMonData[74] <= {20'd0, ext_dout_buf};


################# 2016 #####################	
01/09/16/2016
	ID12-2 offset cal values are not corret.
	(addr == 7'd16)  
	
	
1/29/16
	NOF_CELL_PLC	18
	CantedMagnets_Status[13], CantedMagnets_Status[14]	for ID 4 CANTING
	((!CantedMagnets_Status[13]) && UserMaskControl_0[13] && UserMaskControl_0[14]) ||    for ID-4	
	
	
2/21/2016 for test Cell 12 
    (addr == 7'd18)  ? {10'd310, 10'd301, 10'd300, 10'd299, 10'd298} :	// CELL 12-1  BPM 7 & 8
    (addr == 7'd19)  ? {10'd310, 10'd305, 10'd304, 10'd303, 10'd302} :	// CELL 12-2  BPM 9 & 10
		
		
////////////////////////////////////////////////////////////////////////////////////////////////////
June/1 2016  restart operation after long shutdown.
Caused several BPM fault at ID8-bpm8
		
06/06/2016
    BPM fault signal also implemented a single glitch ignore function.
    
    AI_ID_ILOutStatus added ID bpm status
    ID bpm fault status added for 2 cycle continuce fault detect
    (AI_ID_ILOutStatus[7]&&plb_w_reg[13][5]) || (AI_ID_ILOutStatus[6]&&plb_w_reg[13][5])


		
////////////		
07/07/2016
    7/06, 7/07 caused 3 times beam dump by AI system.

    1) uBlaze dunp by cod diff zero
    reg		[15:0] AI_ID_STATUS_OUTreg_cnt;		//07/07/16 added
    reg 	BM_PS_closed_status;

    CellPLC_IdBeamDumpOutReg move to
    if(  IDAI_RunStopStatusReg[aie_addr] &&	( AI_ID_STATUS_OUT || (!BM_PhotonShutters_Status_info[aie_addr]) ||
                                                                                    ( CM_Status && plb_Reg10[12]) || CellPLC_IdBeamDumpOutReg )   )


    Canting status reading
    if(	CM_Status == 1'b1 ) begin
            CM_Status_reg <= 1'b1;
    end
    else begin
            CM_Status_reg <= CM_Status_reg;
    end

    assign sysStatus[25]    = CellPLC_IdBeamDumpOutReg;
	

	
////////	
08/05/16
    *** Several times beam dump by Canting Magnet status fault, needed to latch end of status.

    1) CantedMagnets_Status_Reg added for Latch Canting fault status.
    2) SysMon register size incres 82 to 90

    3) Conditions :
            ( CM_Status_Reg && plb_Reg10[12])  -> ( CM_Status && plb_Reg10[12])
            'CM_Status_Reg' is status latching,

    added Canting status reading
    4) SysMonData[84] <= CM_SdiRawBitStatus_Reg[31:0];
	
	
09/22/16
    1) Added lookup table for Cell 19 and Cell 2 input
    2) Added Cell 19 and 2 front end status
	
09/28/16
    1) Found one of missing
    ((!CantedMagnets_Status[18]) && UserMaskControl_0[18] && UserMaskControl_0[19])
	
09/29/16
    Gap_Status0   - gap status sdi data
    Gap_Status01  - debounce data

    PhotonShutters_Status0 - IP photon shutter
    PhotonShutters_Status01
	
	
10/03/16
    ID-19 Canting signals to Cell 18 Cell controller : 9, 10, 11


10/20/16 
    // Cell 17 - bit 9 caused failure several time

    reg 	[`MAX_IDEVICE-1:0] CM_SdiRawBitStatus_InReg;
    wire 	[`MAX_IDEVICE-1:0] CM_SdiRawBitStatus;


    Added CM input debaunce function
    genvar cm_ref_i;

10/21/16
    debounceIndex_cm 	debounce_CM_Test  added for debounce test



    ID02, ID19 commissinig completed and system looks very stable.
	
11/16/2016
    Added CellPLC_IdBeamDump_Status latch any of PLC dump detected.

    // 11/18/16 for latch fast dump status
    if(CellPLC_IdBeamDump_Status != UserMaskControl_0)
            CellPLC_IdBeamDump_Status <= CellPLC_IdBeamDump_StatusDebounceOut;
    else
            CellPLC_IdBeamDump_Status <= CellPLC_IdBeamDump_Status;
    //}

	

//
12/01/16
    Installed a new V6 Tri-Temac license (Create license from Joe's Xilinx account)


    PM trigger used only BM/ID dump
    assign aiPM_Trigger = plb_w_reg[13][31] || AI_ID_RfTripCmd || BM_DUMP;

    debouncer module used and 200 us delay.

    .Added Cell 07 up/down stream 4 bpms at lookup table

	
	
12/21/16
    Canting debounce - 1 ms
	
	
01/04/17
    Indivisual enable/disable function added
    aie_addr_gen.v  mask2 signal added for Indivisual ID enable/disable

///////////////////////////////////////////////////////////////////////////
01/17/2017	
    - 01/16 1:10 PM, 3:40PM caused cell 23 glitch 5 arc bpms and ID, BM-PS, ID-PS - Dumped BEAM
    - 01/17 8:40 AM  caused glitch 4 bpms - NO beam dump
	
	
01/17/2017
    ID photon shutter and BM photon shuter debounce changed to 1 ms
    assign sysStatus[6] = AI_BM_RfTripCmd;	//before only out of envelope
    assign sysStatus[6] = BM_DUMP;			// Changed to

01/20/17
    added buffer for fault diagnostics
    // 01/20/17 added for fault detection
    if( BM_ERR_DUMP==1'b0 || ID_DUMP == 1'b1 ) begin
            SdiLinkData_Reg[ SdiRamAddr ] <= SdiOrSimGlobalBpmData;
    end
	
	
01/23/17
    AI-BM zero error debounce 2 ms added
            Reg_bm_bpm_err_status_filt_out
            reg  	[`MAX_IDEVICE-1:0] CellPLC_IdBeamDump_Status_Latch_Reg;
    SysMonData[66] <= (CellPLC_IdBeamDumpOutReg == 1'b1) ? CellPLC_IdBeamDump_Status_Latch_Reg : CellPLC_IdBeamDump_Status;


01/24/17
    CellPLC_IdBeamDump_Status <= 32'hFFFFFFFF;	// PLC 1: is normal 0: is fault


	
10/10/17
	NOTE:
    lemo pin 0 input from BPM lemo output and send PM trigger to timing system.
    uBlaze dump indicate only LED status, the last run caused twice uBlaze DUMP.



    fp_lemo_in[0] added for PM global trigger
    uBlazeDumpCmd added wire
    uBlaze Dump indicate only status without the FINAL Dump
 	
10/24/17
    fp_lemo_in[0] monitoring from LO output
    fp_lemo_in[0] only trigger PM event not Internal PM trigger
	
	

	
/////////////////////////////////////////////	
10/18/17
    1) Cell 7 Cell controller I/O test with PLC.
    2) ID-7 adding start

11/08/17	
    assign sysStatus[20]    = ~sdi2_cc_sfp_link[1] | sdi2_cc_sfp_link[0];
    assign sysStatus[21]    = ~sdi2_cc_sfp_link[3] | sdi2_cc_sfp_link[2];
    assign sysStatus[22]    = fp_lemo_in[0];
    assign sysStatus[23]    = fp_lemo_in[1];

    assign ext_dout_buf[0]   = ~(FINAL_DUMP_OUT | aiPM_Trigger | fp_lemo_in[0] );  	//PM Trigger
	

04/09/19 EVR PM reset
04/10/19
    User reset changed single clock reset.


6/13/20 : Ray requested change beam dump logic.
    Slow dump first ? then disable the fast dump
    ID_FaultTimestampOut
    BM_FaultTimestampOut
	
	
9/3/22 :
    Added C27 and C20
    
9/7/22 : sec/nsec register map order change
    ID_FaultTimestampOut
    BM_FaultTimestampOut
	
    ID_FaultTimestampOut[31:0] <= 32'd0; //9/7/22: nsec clear
    BM_FaultTimestampOut[31:0] <= 32'd0; //9/7/22: nsec clear
    // 9/7/22 change sec/nsec register
    SysMonData[86] <= ID_FaultTimestampOut[63:32];	//sec
    SysMonData[87] <= ID_FaultTimestampOut[31:0];	//nsec			
    SysMonData[88] <= BM_FaultTimestampOut[63:32];	//sec
    SysMonData[89] <= BM_FaultTimestampOut[31:0];	//nsec

       

    Rear LEMO IN3 connected with AI2 Zynq LEMO out
10/14/22 :
    Added PLC WD for C27
    Clear sync edge with 10 kHz trigger
    AIE_USR_FULL_MASK_SETTING
    added:
        beam_plc_trip_TS_status
        ext_trip_TS_status
        din_buf[2] - IN3
        
        

11/01/22
    C23 update and test
    
    ad_rst added
    id_glitch_detected    
	id_glitch_count
    _BUILD_TIME_
    
    
    
    
###################################################################################################	
NOT USED:

    plb_w_Reg12[3]
    plb_w_reg[15][15]
    
	
 */


//2016 
//`define	FIRMWARE_VER        32'h080916
//`define	FIRMWARE_VER        32'h100316
//`define	FIRMWARE_VER        32'h102016
//`define	FIRMWARE_VER        32'h111816
//`define	FIRMWARE_VER        32'h122216
// 2017
//`define	FIRMWARE_VER        32'h012317
//`define	FIRMWARE_VER        32'h101017

//`define	FIRMWARE_VER        32'h110817
//`define	FIRMWARE_VER        32'h4102019	//04/09/19
//`define	FIRMWARE_VER        32'h5062019
//`define	FIRMWARE_VER        32'h6172020


//`define	FIRMWARE_VER        32'h9032022

//`define	FIRMWARE_VER        32'h9072022

//`define	FIRMWARE_VER        32'h10172022


`define	FIRMWARE_VER            32'h11012022



//AI-ID number of MAX IDs for operation
//`define	AIE_USR_FULL_MASK_SETTING	64'h00000000001FFFFF
// 11/07/17 added ID7U, D
`define	AIE_USR_FULL_MASK_SETTING    64'h0000000000FFFFFF



`define	CC_SDI_IN_CLK_50_MHZ         1
`define	__DATA_32BIT__               1

// Variable for test
`define	MAX_LEN_SDI_DATA             801     //26*31, Cell 30 + MPS 1
`define	SYS_CTRL_LEN                 1024
`define	MAX_IDEVICE                  64
`define	AIE_CAL_NCLK                 30      //each AIE allow 30 cycles for calculations

//
`define	OFFSET_BASE                  50
`define	OFFSET_M_BASE                60
`define	OFFSET_MRAD_BASE             80
`define	S_M_BASE                     150
`define	S_MRAD_BASE                  200

`define	NPI_BURST_0_DDR3_START       32'hC2000000
`define	NPI_DDR3_START               32'hD0000000
`define	NPI_DDR3_END                 32'hFFFF0000

// Position address information
`define	POS_ADDR_BEAM_CURRENT        32'd76

`define	NOF_LIMIT_SET                15

// address table
`define	ADDR_H1                      0	//AIO_H1
`define	ADDR_V1                      1	//AIO_V1
`define	ADDR_H2                      2
`define	ADDR_V2                      3
`define	ADDR_H3                      4
`define	ADDR_V3	                     5
`define	ADDR_ANGLE_H                 6	//AIAL_H
`define	ADDR_ANGLE_V                 7	//AIAL_V
`define	ADDR_MRAD_S                  8
`define	ADDR_MM_S                    9
`define	ADDR_LOGIC                   10
`define	H1_CHAMBER_OFFSET            11	//added 03/29/14
`define	V1_CHAMBER_OFFSET            12
`define	H2_CHAMBER_OFFSET            13	//added 03/31/14
`define	V2_CHAMBER_OFFSET            14


//CELL PLC
//`define	NOF_CELL_PLC	     22		//[0..21]	Not used
`define	N_OF_PLC_VALID               17  //16  //10/14/22 +1

// was 2, 1 
`define	BM_GLITCH_CYCLE              4'd3
`define	ID_GLITCH_CYCLE	             4'd2



`define	_BUILD_TIME_  32'd1667339292;


//////////////////////
/// MPS TOP  /////////
//////////////////////
module Mps_top (
    input 			sys_clk_in_p,		//200 MHz
    input 			sys_clk_in_n,
    /* RS232 */
    input 			fpga_0_RS232_Uart_1_sin_pin,
    output 			fpga_0_RS232_Uart_1_sout_pin,
    /* RESET */
    input 			sys_rst_pin,
    /* EEPROM */
    inout 			fpga_0_IIC_EEPROM_Scl_pin,
    inout 			fpga_0_IIC_EEPROM_Sda_pin,
    /* FLASH */
    output [5:30]   fpga_0_FLASH_Mem_A_pin,
    inout  [0:15]   fpga_0_FLASH_Mem_DQ_pin,
    output          fpga_0_FLASH_Mem_WEN_pin,
    output          fpga_0_FLASH_Mem_OEN_pin,
    output          fpga_0_FLASH_Mem_CEN_pin,
    /* HARD EMAC */
    output          fpga_0_Hard_Ethernet_MAC_TemacPhy_RST_n_pin,
    output  [7:0]   fpga_0_Hard_Ethernet_MAC_GMII_TXD_0_pin,
    output          fpga_0_Hard_Ethernet_MAC_GMII_TX_EN_0_pin,
    output          fpga_0_Hard_Ethernet_MAC_GMII_TX_CLK_0_pin,
    output          fpga_0_Hard_Ethernet_MAC_GMII_TX_ER_0_pin,
    input           fpga_0_Hard_Ethernet_MAC_GMII_RX_ER_0_pin,
    input           fpga_0_Hard_Ethernet_MAC_GMII_RX_CLK_0_pin,
    input           fpga_0_Hard_Ethernet_MAC_GMII_RX_DV_0_pin,
    input   [7:0]   fpga_0_Hard_Ethernet_MAC_GMII_RXD_0_pin,
    input           fpga_0_Hard_Ethernet_MAC_MII_TX_CLK_0_pin,
    output          fpga_0_Hard_Ethernet_MAC_MDC_0_pin,
    inout           fpga_0_Hard_Ethernet_MAC_MDIO_0_pin,
    /* DDR3 */
    output  [0:0]   fpga_0_DDR3_SDRAM_DDR3_Clk_pin,
    output  [0:0]   fpga_0_DDR3_SDRAM_DDR3_Clk_n_pin,
    output  [0:0]   fpga_0_DDR3_SDRAM_DDR3_CE_pin,
    output  [0:0]   fpga_0_DDR3_SDRAM_DDR3_CS_n_pin,
    output  [0:0]   fpga_0_DDR3_SDRAM_DDR3_ODT_pin,
    output          fpga_0_DDR3_SDRAM_DDR3_RAS_n_pin,
    output          fpga_0_DDR3_SDRAM_DDR3_CAS_n_pin,
    output          fpga_0_DDR3_SDRAM_DDR3_WE_n_pin,
    output  [2:0]   fpga_0_DDR3_SDRAM_DDR3_BankAddr_pin,
    output  [14:0]  fpga_0_DDR3_SDRAM_DDR3_Addr_pin,
    inout   [31:0]  fpga_0_DDR3_SDRAM_DDR3_DQ_pin,
    output  [3:0]   fpga_0_DDR3_SDRAM_DDR3_DM_pin,
    inout   [3:0]   fpga_0_DDR3_SDRAM_DDR3_DQS_pin,
    inout   [3:0]   fpga_0_DDR3_SDRAM_DDR3_DQS_n_pin,
    output          fpga_0_DDR3_SDRAM_DDR3_Reset_n_pin,
    output          fpga_0_clk_25MHz_88E1111_CLKOUT5_pin,
    /* EVR CLK */
    input           evr_0_evr_TILE2_REFCLK_PAD_N_IN_pin,
    input           evr_0_evr_TILE2_REFCLK_PAD_P_IN_pin,

    /* SFP IIC */
    output	[5:0] 	sfp_sclk,
    inout	[5:0] 	sfp_sdata,
    /* Ehernet LED */
    input			eth_phy_led_link100,
    input			eth_phy_led_link1000,
    input			eth_phy_led_duplex,
    input			eth_phy_led_rx,
    input			eth_phy_led_tx,
    output			eth_phy_led1,
    output			eth_phy_led2p,
    output			eth_phy_led2n,

    /* I/O board DAC output signal */
    output 			dac_load_p, dac_load_n,
    output 			dac_clr_p,  dac_clr_n,
    output 			dac_sync_p, dac_sync_n,
    output 			dac_sdin_p, dac_sdin_n,
    output 			dac_sclk_p, dac_sclk_n,
    input 			dac_sdo_p,  dac_sdo_n,
    /* I/O board Digital I/O */
    input  [15:0] ExtDin_pin,
    output [11:0] ExtDout_pin,

    /* Debug */
    output  [1:0]  fp_out_p,
    output  [1:0]  fp_out_n,

    input   [1:0]  fp_in_p,
    input   [1:0]  fp_in_n,

    input   [1:0]  RXN_IN,
    input   [1:0]  RXP_IN,
    output  [1:0]  TXN_OUT,
    output  [1:0]  TXP_OUT,

    output  [7:0]  dbg_led,
    output  [3:0]  fp_led,
    output  [11:0] sfp_led,
    output  [19:0] DebugOUT
    );
	
	
    ///////////  microblaze variable ///////////
    //Inputs
    wire sdi2_CW_LocalPktDataValid, sdi2_CCW_LocalPktDataValid;
    wire [31:0] sdi2_CW_LocalPktData;
    wire [31:0] sdi2_CCW_LocalPktData;

    //Outputs
    wire clk, evrClkFa10kHz;				//fofb and dsp clock 100 MHz

    wire evr_TRIGGER_0;		// 1H or 10 Hz trigger
    wire evr_TRIGGER_1;		// 10 kHz trigger by event system
    wire evr_TRIGGER_2;
    wire evr_TRIGGER_3;
    wire evr_TRIGGER_4;
    wire evr_TRIGGER_5;
    wire evr_TRIGGER_6;
    wire evr_TRIGGER_7;
    wire TRIGGER_1;
    wire intTrig;
    //wire evr_0_IP2INTC_Irpt0;
    wire evr_0_IP2INTC_Irpt1_LOL;
    wire evr_0_evr125RfLockedClk;
    wire [63:0] evr_0_TsLatched;
    wire evr_0_TsValid;
    wire evr_DIST_BUS_2_pin;

`ifdef  LOCAL_SDI_ENABLE		
    wire sdi2_0_CCW_LinkDataValid;
    wire sdi2_0_StartCalc;
    wire sdi2_0_CWLinkDataValid;
    wire sdi2_0_CWLocalModeState;
    wire sdi2_0_CWRemoteModeState;
    wire [3:0] sdi2_0_sfp_link;
    wire sdi2_0_CCWRemoteModeState;
    wire sdi2_0_CCW_LedRxFifoValid;
    wire sdi2_0_CW_DualPortRAMWrite;
    wire sdi2_0_LinkDataValid;
    wire sdi2_0_DPRAM_ReadStart;
    wire sdi2_0_DPRAM_Valid;
    wire sdi2_0_CellSdi_Strb;
    wire [31:0] sdi2_0_CellSdi_DataOut;
    wire [9:0] sdi2_0_CellSdi_Addr;
    wire sdi2_0_AllLinkValidStatus;
    wire sdi2_0_DirLED;
`endif			
    wire sdi2_cc_MpsStartTrigger;
    wire sdi2_cc_BiDirLinkDataValid;
    wire sdi2_cc_DPRAM_Valid;
    wire sdi2_cc_DPRAM_ReadStart;
    wire [3:0] sdi2_cc_sfp_link;
    wire sdi2_cc_CCWRemoteModeState;
    wire sdi2_cc_CWRemoteModeState;
    wire sdi2_cc_StartCalc;
    wire sdi2_cc_LinkDataValid;
    wire sdi2_cc_CCW_LinkDataValid;
    wire sdi2_cc_AllLinkValidStatus;


    wire  sdi2_cc_NipStrobe;
    // PLB Interface register
    wire [31:0] plb_w_reg[0:15];
    wire [31:0] plb_r_reg31, plb_r_reg30, plb_r_reg29, plb_r_reg28, plb_r_reg27, plb_r_reg26, plb_r_reg25; /* Reg31-Reg25 */
    wire [31:0] plb_r_reg24, plb_r_reg23, plb_r_reg22, plb_r_reg21,plb_r_reg20;
	
    wire [31:0] plb_Data0				= plb_w_reg[0];
    wire [31:0] plb_Addr0 				= plb_w_reg[1];
    wire [31:0] plb_Data1				= plb_w_reg[2];
    wire [31:0] plb_Addr1 				= plb_w_reg[3];
    wire [31:0] plb_WR_Reg6 			= plb_w_reg[6];		/* BPM select for DAC output */
    wire [31:0] plb_WR_Reg7  			= plb_w_reg[7];   	/* pid filter gain */
    //wire [31:0] plb_Reg9	   			= plb_w_reg[9];		//default 10000
    wire [31:0] plb_Reg10			   	= plb_w_reg[10];	//default 10000
    wire [31:0] plb_w_Reg12	    		= plb_w_reg[12];

    reg [31:0] UserMaskControl_0, UserMaskControl_1;

    ///////////  FOFB variable ///////////
    reg Trig1Start;
    reg TRIG1;
    wire	GTX_RefClock;
    wire DspMatrixCalStartTrig;				//It's time to Matrix Vector calculation Ut [] * PosError []
    wire sync_trig;
    wire npi_sync_trig;
    wire [31:0] sr_bpm_ref_offset_xy_set;
    wire [31:0] sr_bpm_origin_offset_xy_set;
    wire [0:0]  UserBramOutXyMaskOut;
    wire [31:0] bpmPos_Xy_Data;
    wire [31:0] Ut_DataIn;
    //
    wire [15:0] DAC_DataOut;
    wire PosXyDataValid, PosSingleDataValid;
    wire [31:0] SdiOrSimGlobalBpmData, BpmXy_SimData, CC_Sdi_RamData;
    //wire [31:0] XyPosition_26bitFormatData;
    wire [9:0] 	dpram_addr;
    wire [8:0] 	bram_x_addr, bram_y_addr;	// bram_addr;
    wire dpram_rd, bram_x_wr, bram_y_wr;
    wire [9:0] 	bpmPos_xy_Addr;
    wire [9:0] 	xy_pos_addr;	//1024

    // memory selection
    wire [12:0] ram_addr1, ram_addr2;
    wire [31:0] ram_Data1, ram_Data2;
    wire [20:0] ram_cs1, ram_cs2;
    wire 		ram_WR1, ram_WR2;
    wire mySim_Trig;
    wire [31:0] PsSet_muxDatOut;
    wire fifo_we;
    wire fifo_rd;
    wire fifo_we_envelop, fifo_rst;
    wire full, empty;
    wire start_bit;
    wire xy_Mask_DataValid;
    wire [31:0] xy_pos_fifo_out;
	
    /////////////////////////
    /* Register location   */
    /////////////////////////
    wire 	[1:0]  beam_current;		// beam current range status ?
    //

    //reg  [31:0]  SysMonData[82:0];		// System monitoring registers
    //reg  	[31:0]  SysMonData[90:0];
    reg     [31:0]  SysMonData[132:0];		// 6/16/20


    //
    reg     [`MAX_IDEVICE-1:0] Gap_Status0;
    wire    [`MAX_IDEVICE-1:0] Gap_Status01;
    reg     [`MAX_IDEVICE-1:0] PhotonShutters_Status0;
    wire    [`MAX_IDEVICE-1:0] PhotonShutters_Status01;
    reg     [`MAX_IDEVICE-1:0] BM_PhotonShutters_Status0;
    wire    [`MAX_IDEVICE-1:0] BM_PhotonShutters_Status01;
    wire    [63:0] ID_BpmPosValueStatus;		//09/25/14
    reg     [31:0] CellPLC_IdBeamDump_Status;
    reg     [31:0] CellPLC_BmBeamDump_Status;
    reg     [31:0] CellPLC_IdBeamDump_StatusReg;
    wire    [31:0] CellPLC_IdBeamDump_StatusDebounceOut;
    reg     [31:0] CellPLCWdStatus;
    reg     [31:0] CellPLC_IdBeamDump_Status_Latch_Reg;		//01/23/17 added
    reg     [`MAX_IDEVICE-1:0] IDAI_BeamDumpControlReg;
    reg     [`MAX_IDEVICE-1:0] IDAI_RunStopStatusReg;
    wire    [`MAX_IDEVICE-1:0] gap_status_info;
    wire    [`MAX_IDEVICE-1:0] PhotonShutters_Status_info;
    wire    [`MAX_IDEVICE-1:0] BM_PhotonShutters_Status_info;
    reg     [`MAX_IDEVICE:0] ID_BpmPosValueStatusReg2;
    //
    reg     [`MAX_IDEVICE-1:0] CantedMagnets_Status;
    reg     [`MAX_IDEVICE-1:0] CantedMagnets_Status_Reg;
    //reg   [`MAX_IDEVICE-1:0] CM_SdiRawBitStatus;
    reg     [`MAX_IDEVICE-1:0] CM_SdiRawBitStatus_InReg;
    wire    [`MAX_IDEVICE-1:0] CM_SdiRawBitStatus;

    reg     [`MAX_IDEVICE-1:0] CM_SdiRawBitStatus2;
    reg     [`MAX_IDEVICE-1:0] CM_SdiRawBitStatus_Reg;		
	
    reg     [780:0] bm_iL_info;
    wire    npi_we;
    //fifo
    wire    u_fifo_we, u_fifo_full, u_fifo_empty;
    wire    [31:0] u_fifo_out;

    wire    bpmRamCellMark;

    wire    [9:0] sdi_max_signal;
    wire    [9:0] BpmAllValidNumber;
    wire    bpmPosDataValid, bpmPosFoFbDataValid;
    wire    [9:0] SdiRamAddr;
    wire    SdiDataRdValid;

    wire    [31:0] npi_DDR_start_addr;
    wire    Npi0_PM_RstOut;
    wire    [31:0] FaPmTrigCnt_out;
    wire    Npi_1_WrEn;
    wire    NpiWrEn_0;
    wire    [31:0] NpiData1, NpiData0;
    wire    [31:0] Npi_0_Data1, Npi_0_Data0;

    wire    muxValid;
    wire    [31:0] mux_Data_o;
    wire    pm_npi_enable;
    wire    pm_npi_wr;
    wire    npi_0_pm_reset;		//pm
    wire    npi_1_strb_reset;
    wire    npi_burst_0_reset;
    wire    npi_stop;
    wire    [63:0] TS_time;

    wire    [6:0] aie_addr;
    wire    [49:0] IdBpmPos_base;
    wire    AIeCalValid;
    wire    AIeCalOutLatch;
    wire    [63:0] aie_USET_MASK_SETTING;
    reg     [63:0] aie_USET_MASK_SETTING_REG;
    wire    aie_USER_MASK_OUT;
    wire    [9:0] add_x1, add_y1, add_x2, add_y2, add_cells;
    wire    [31:0] SetRamReadDoutA [`NOF_LIMIT_SET:0];
    wire    [31:0] SetRamDoutB [`NOF_LIMIT_SET:0];
    wire    [31:0] RefDpramOut;
    wire    [31:0] bram_odate[5:0];

    wire    BM_DUMP;
    wire    FINAL_DUMP_OUT;
    wire    dcct_0_2mA, dcct_2mA, dcct_50mA, dcct_500mA, dcct_plc_hb, dcct_sys_fault, dcct_beam_loss;
    wire    GlobalLatchClearingReset;
    wire    [9:0]	BmPosAddress;
    wire    SLOW_DUMP, FAST_DUMP;
    reg     ID_DUMP;
    wire    io_watchdog_out;
    wire    [3:0] sdi2_cc_MyLocalLoopbackDataValid_o;
    wire    [1:0] sdi2_cc_RemotePacketDroppedStatus;
    wire    sdi2_cc_DirLED;
    reg     ID_BpmFailStatus, ID_BpmFailStatusLatch;
    reg     CM_Status, CM_Status_reg, CM_Status2, CM_StatusTmpReg;
    //bpm fault
    reg     [31:0] y1_pos_buff[`MAX_IDEVICE:0];
    reg     [31:0] y2_pos_buff[`MAX_IDEVICE:0];
    wire    [31:0] Cell_PLC_WD_Status;		//Cell 1 - 30
    wire    [31:0] cell_plc_wd_timeout;

    // 10/08/14 for bp bpm position error detect
    wire    Qo_5kHz;
    wire    Din_trig;
    wire    bm_pos_err_valid;
    wire signed [31:0] bm_pos_err;

    wire    [8:0] bpmPosRdAddr;
    wire    bpmPosAddrRdValid;
    wire    bm_pos_wr_n0, bm_pos_wr_n1, bm_err_cal_start;
    wire    [1:0] bpm_id_12_rx_status;
    wire    [31:0] id_out_of_limit_cnt;
    reg     [7:0] t_CNT;
    wire    [1:0] fp_lemo_in;
    reg     [15:0] bm_bpm_err;
    reg     BM_ERR_DUMP;
    reg     t5khz_0_Reg, t5khz_0_reg1, t5khz_0_reg2;
    reg     bm_bpm_err_status;
    wire    bm_bpm_err_status_filt_out;
    reg     Reg_bm_bpm_err_status_filt_out;
    reg     [31:0] bm_bpm_err_counts;

    reg     rf_trip_st_Trig;
    reg     rf_trip_st_T1Reg, rf_trip_st_T1Reg_reg1, rf_trip_st_T1Reg_reg2;
    wire    bm_bpm_fault_detected;
    wire    FINAL_FREE_DUMP_OUT;
    wire    SDI_Reg_WR_ENABLE;
    wire    CalBram_En;
    reg     CellPLC_IdBeamDumpOutReg;
	
    wire 	[31:0] XyPosDiff;
    reg 	[1:0] bm_pos_diff_status;
    reg		bm_pos_glitch_status_1, bm_pos_glitch_status_2;
    wire 	AI_BM_RfTripCmd;
    //wire BM_LATCHED_OUT_RST;
    reg 	AI_BM_RfTripDebounce5CycleCmdReg;
    reg 	[3:0] Counter_Free_Running_out1;
    wire 	IL_BM_GlitchLatchedOut;
    //AI-ID
    wire  	id_debounce_trip_out, id_debounce_trip_out2;
    reg		id_pos_glitch_status_1;
    reg		[2:0] Trig1DlyReg;

    // two data compare
    wire 	ai_active;
    wire 	ai_sngl_err;
    wire 	ai_dbl_err;
    wire 	[15:0] data_addr;
    wire 	[15:0] fifo_cnt;
	
	
    wire	gClk10kHz;
    wire	bm_err_count_flag;
    wire 	AI_BPM_RfTripModeSel;
    reg		AI_ID_RfTripDebounceCmdReg;
    wire 	Npi0_PM_RstOut_str;
    wire 	x_orbit_valid, y_orbit_valid;
    wire	[31:0] 	x_orbit_rms, y_orbit_rms;
    wire 	npi_0_pm_reset_str;
    wire	[15:0] bm_xx_pos_out_cnt, bm_yy_pos_out_cnt;
    wire	[7:0] AI_ID_ILOutStatus;
    wire	[5:0] Il_latched_s;
    wire 	IDAI_Status;

    wire    [31:0] x_mm_cal, y_mm_cal, x_mrad_cal, y_mrad_cal, x_mrad_err, y_mrad_err, x_mm_err, y_mm_err;
    wire 	[31:0] testw;
    wire	[63:0] aie_DumpStatus;
    wire	aieOutDataValid;
    wire	aieIl_LatchValid_o;


    wire	[63:0] Aie_bm_o;
    wire	[63:0] Aie_A_o;
    wire	[63:0] Aie_C_o;
    wire	[63:0] Aie_D_o;
    wire	Il_activated;

    /*ID2*/
    wire	[63:0] Aie_bm_o2;
    wire	[63:0] Aie_A_o2;
    wire	[63:0] Aie_C_o2;
    wire	[63:0] Aie_D_o2;

    // 12/03/15
    reg		DspMatrixCalStartTrig_5kHz;
    wire 	ai_id_trig1, ai_id_trig2;
    reg 	[3:0] aieIl_Latch_dly;
    reg 	[3:0] aieIl_Latch_dly2;
    wire 	ai_id1_rst_acq_trig;			// 5kHz
    wire 	ai_id2_rst_acq_trig;

    wire 	AI_ID_STATUS_OUT;
    reg		[6:0]	ai_id_err_cnt;
    reg 	ai_id_multiple_err_status;
    reg 	ai_id_dump2;
    wire	ai_id_dump_status;
    wire 	ai_id_dump_rst;
    reg		[1:0] ai_id_dump2_rstReg;

    wire	ai_id_trip_src;
    wire 	ai_bm_trip_src;

    wire	ai_id_glitch_rise;
    reg	 	glitch_width;
    reg 	[15:0] counter_out;
    wire	trig_width;
    reg 	ai_rst_trig;
	
    wire	aiPM_Trigger;
    wire 	ai_pm_trigger_event;
    wire	[11:0] ext_dout_buf;
    reg		[15:0] AI_ID_STATUS_OUTreg_cnt;		//07/07/16 added
    reg 	BM_PS_closed_status;
    reg 	[6:0] aie_addrBmPsReg;
    reg 	[15:0] Canting_sum;

    wire	uBlazeDumpCmd;		//10/10/17
    wire 	AIID_process_RST;
    reg 	evr_t6_T0Reg, evr_t6_T0Reg_reg1, evr_t6_T0Reg_reg2;
    wire 	EVR_PM_Reset_Sts;
    reg		EVR_PM_Reset;
    wire    EVR_Sync_Reset; //10/14/22

    reg [63:0] ID_FaultTimestampOut;
    reg [63:0] BM_FaultTimestampOut;
    reg id_glitch_detected;
    reg [15:0] id_glitch_count;
    
    
    ///////////////////////
    ///// MICRO BLAZE /////
    ///////////////////////
    (* BOX_TYPE = "user_black_box" *)
    system
    system_i (
        .sys_clk_in_p ( sys_clk_in_p ),
        .sys_clk_in_n ( sys_clk_in_n ),
        .fpga_0_RS232_Uart_1_sin_pin ( fpga_0_RS232_Uart_1_sin_pin ),
        .fpga_0_RS232_Uart_1_sout_pin ( fpga_0_RS232_Uart_1_sout_pin ),
        .sys_rst_pin ( sys_rst_pin ),
        .fpga_0_IIC_EEPROM_Scl_pin ( fpga_0_IIC_EEPROM_Scl_pin ),
        .fpga_0_IIC_EEPROM_Sda_pin ( fpga_0_IIC_EEPROM_Sda_pin ),
        .fpga_0_FLASH_Mem_A_pin ( fpga_0_FLASH_Mem_A_pin ),
        .fpga_0_FLASH_Mem_DQ_pin ( fpga_0_FLASH_Mem_DQ_pin ),
        .fpga_0_FLASH_Mem_WEN_pin ( fpga_0_FLASH_Mem_WEN_pin ),
        .fpga_0_FLASH_Mem_OEN_pin ( fpga_0_FLASH_Mem_OEN_pin ),
        .fpga_0_FLASH_Mem_CEN_pin ( fpga_0_FLASH_Mem_CEN_pin ),
        .fpga_0_Hard_Ethernet_MAC_TemacPhy_RST_n_pin ( fpga_0_Hard_Ethernet_MAC_TemacPhy_RST_n_pin ),
        .fpga_0_Hard_Ethernet_MAC_GMII_TXD_0_pin ( fpga_0_Hard_Ethernet_MAC_GMII_TXD_0_pin ),
        .fpga_0_Hard_Ethernet_MAC_GMII_TX_EN_0_pin ( fpga_0_Hard_Ethernet_MAC_GMII_TX_EN_0_pin ),
        .fpga_0_Hard_Ethernet_MAC_GMII_TX_CLK_0_pin ( fpga_0_Hard_Ethernet_MAC_GMII_TX_CLK_0_pin ),
        .fpga_0_Hard_Ethernet_MAC_GMII_TX_ER_0_pin ( fpga_0_Hard_Ethernet_MAC_GMII_TX_ER_0_pin ),
        .fpga_0_Hard_Ethernet_MAC_GMII_RX_ER_0_pin ( fpga_0_Hard_Ethernet_MAC_GMII_RX_ER_0_pin ),
        .fpga_0_Hard_Ethernet_MAC_GMII_RX_CLK_0_pin ( fpga_0_Hard_Ethernet_MAC_GMII_RX_CLK_0_pin ),
        .fpga_0_Hard_Ethernet_MAC_GMII_RX_DV_0_pin ( fpga_0_Hard_Ethernet_MAC_GMII_RX_DV_0_pin ),
        .fpga_0_Hard_Ethernet_MAC_GMII_RXD_0_pin ( fpga_0_Hard_Ethernet_MAC_GMII_RXD_0_pin ),
        .fpga_0_Hard_Ethernet_MAC_MII_TX_CLK_0_pin ( fpga_0_Hard_Ethernet_MAC_MII_TX_CLK_0_pin ),
        .fpga_0_Hard_Ethernet_MAC_MDC_0_pin ( fpga_0_Hard_Ethernet_MAC_MDC_0_pin ),
        .fpga_0_Hard_Ethernet_MAC_MDIO_0_pin ( fpga_0_Hard_Ethernet_MAC_MDIO_0_pin ),
        .fpga_0_DDR3_SDRAM_DDR3_Clk_pin ( fpga_0_DDR3_SDRAM_DDR3_Clk_pin[0:0] ),
        .fpga_0_DDR3_SDRAM_DDR3_Clk_n_pin ( fpga_0_DDR3_SDRAM_DDR3_Clk_n_pin[0:0] ),
        .fpga_0_DDR3_SDRAM_DDR3_CE_pin ( fpga_0_DDR3_SDRAM_DDR3_CE_pin[0:0] ),
        .fpga_0_DDR3_SDRAM_DDR3_CS_n_pin ( fpga_0_DDR3_SDRAM_DDR3_CS_n_pin[0:0] ),
        .fpga_0_DDR3_SDRAM_DDR3_ODT_pin ( fpga_0_DDR3_SDRAM_DDR3_ODT_pin[0:0] ),
        .fpga_0_DDR3_SDRAM_DDR3_RAS_n_pin ( fpga_0_DDR3_SDRAM_DDR3_RAS_n_pin ),
        .fpga_0_DDR3_SDRAM_DDR3_CAS_n_pin ( fpga_0_DDR3_SDRAM_DDR3_CAS_n_pin ),
        .fpga_0_DDR3_SDRAM_DDR3_WE_n_pin ( fpga_0_DDR3_SDRAM_DDR3_WE_n_pin ),
        .fpga_0_DDR3_SDRAM_DDR3_BankAddr_pin ( fpga_0_DDR3_SDRAM_DDR3_BankAddr_pin ),
        .fpga_0_DDR3_SDRAM_DDR3_Addr_pin ( fpga_0_DDR3_SDRAM_DDR3_Addr_pin ),
        .fpga_0_DDR3_SDRAM_DDR3_DQ_pin ( fpga_0_DDR3_SDRAM_DDR3_DQ_pin ),
        .fpga_0_DDR3_SDRAM_DDR3_DM_pin ( fpga_0_DDR3_SDRAM_DDR3_DM_pin ),
        .fpga_0_DDR3_SDRAM_DDR3_DQS_pin ( fpga_0_DDR3_SDRAM_DDR3_DQS_pin ),
        .fpga_0_DDR3_SDRAM_DDR3_DQS_n_pin ( fpga_0_DDR3_SDRAM_DDR3_DQS_n_pin ),
        .fpga_0_DDR3_SDRAM_DDR3_Reset_n_pin ( fpga_0_DDR3_SDRAM_DDR3_Reset_n_pin ),
        .fpga_0_clk_25MHz_88E1111_CLKOUT5_pin ( fpga_0_clk_25MHz_88E1111_CLKOUT5_pin ),
        .evr_0_evr_TILE2_REFCLK_PAD_N_IN_pin ( evr_0_evr_TILE2_REFCLK_PAD_N_IN_pin ),
        .evr_0_evr_TILE2_REFCLK_PAD_P_IN_pin ( evr_0_evr_TILE2_REFCLK_PAD_P_IN_pin ),
        //---------------------
        // EVENT
        //---------------------
        .Clk_FOFB_100M ( clk ),	//OUTCLK 7, disable using EVR 62.5 MHz
        .evr_0_TRIGGER_0 ( evr_TRIGGER_0 ),
        .evr_0_TRIGGER_1 ( evr_TRIGGER_1 ),	//
        .evr_0_TRIGGER_2 ( evr_TRIGGER_2),	//RUN/STOP
        .evr_0_TRIGGER_3 ( evr_TRIGGER_3),	//UT RAM Active
        .evr_0_TRIGGER_4 ( evr_TRIGGER_4),	//V RAM Active
        .evr_0_TRIGGER_5 ( evr_TRIGGER_5),	//U PID Reset
        .evr_0_TRIGGER_6 ( evr_TRIGGER_6),	//V PID Reset PM Reset
        .evr_0_TRIGGER_7 ( evr_TRIGGER_7),
        .evr_0_TimeStamp( {TS_time} ),		//[63:0]
        .evr_0_IP2INTC_Irpt0 (),
        .evr_0_IP2INTC_Irpt1_LOL ( evr_0_IP2INTC_Irpt1_LOL ),
        .evr_0_evr125RfLockedClk ( evr_0_evr125RfLockedClk ),	//125 MHz clock
        .evr_0_TsLatched ( evr_0_TsLatched ),					//[63:0]
        .evr_0_TsValid ( evr_0_TsValid ),
        .evr_0_TsRequest ( Npi0_PM_RstOut ),	//requerst latch
        .evr_0_evr_DIST_BUS_2_pin ( evr_DIST_BUS_2_pin), 			//10 kHz
        .evr_0_evr_GtxRefClock_OUT( GTX_RefClock ),
        ///////////////
        // BPM SID NPI
        .npi_burst_0_din_clk ( clk ),
        .npi_burst_0_npi_fifo_Rst ( npi_burst_0_reset  ),
        .npi_burst_0_fifo_wr_en ( NpiWrEn_0 ), 	//pm_npi_wr ),
        .npi_burst_0_fifo_din ( {Npi_0_Data1, Npi_0_Data0} ),
        .npi_burst_0_wr_buf_start_addr ( `NPI_BURST_0_DDR3_START ),
        //---------------------
        // CELL SDI
        //---------------------
        .sdi2_cc_SdiTrigger_IN( TRIGGER_1 ),
`ifdef 	CC_SDI_IN_CLK_50_MHZ
        .sdi2_cc_CWLocalDataClock ( clk ), 		//cc sdi input data clock is 100 MHz
`else
        .sdi2_cc_CWLocalDataClock ( evr_0_evr125RfLockedClk ),	//cc sdi input data clock is 125 MHz event RF Locked clock
`endif
        //SDI Local Packet
        //CW Packet
        .sdi2_cc_CWLocalDataValid ( sdi2_CW_LocalPktDataValid ),
        .sdi2_cc_CWLocalData ( sdi2_CW_LocalPktData ),
        //CCW Packet
        .sdi2_cc_bpmTestDataValid ( sdi2_CCW_LocalPktDataValid ),
        .sdi2_cc_bpmTestData ( sdi2_CCW_LocalPktData ),
        //
        .sdi2_cc_DPRAM_Clk_pin ( clk ),			// 50 MHz clk for FOFB interface
        .sdi2_cc_fofb_en ( dpram_rd ),			// read signal
        .sdi2_cc_fofb_addr ( dpram_addr ),		// address
        .sdi2_cc_fofb_data ( CC_Sdi_RamData ),	// data
        .sdi2_cc_MpsStartTrigger ( sdi2_cc_MpsStartTrigger ),
        .sdi2_cc_BiDirLinkDataValid ( sdi2_cc_BiDirLinkDataValid ),
        .sdi2_cc_DPRAM_Valid ( sdi2_cc_DPRAM_Valid ),
        .sdi2_cc_DPRAM_ReadStart ( sdi2_cc_DPRAM_ReadStart ),
        .sdi2_cc_sfp_link ( sdi2_cc_sfp_link ),
        .sdi2_cc_CCWRemoteModeState ( sdi2_cc_CCWRemoteModeState ),
        .sdi2_cc_CWRemoteModeState ( sdi2_cc_CWRemoteModeState ),
        .sdi2_cc_StartCalc ( sdi2_cc_StartCalc ),
        .sdi2_cc_LinkDataValid ( sdi2_cc_LinkDataValid ),
        //8/15/13 added
        .sdi2_cc_LinkHead(),
        .sdi2_cc_LinkData(),
        .sdi2_cc_LinkStartOfPacket(),
        .sdi2_cc_TxClock125MHz(),
        //
        .sdi2_cc_CCW_LinkDataValid ( sdi2_cc_CCW_LinkDataValid ),
        .sdi2_cc_AllLinkValidStatus ( sdi2_cc_AllLinkValidStatus ),
        .sdi2_cc_NipStrobe(sdi2_cc_NipStrobe),
        .sdi2_cc_DirLED( sdi2_cc_DirLED ),
        .sdi2_cc_SdiUsrNpiTrigger( npi_sync_trig ),
        //
        //NPI interface BPM Position
        .npi_burst_cc_DDR_start_addr( npi_DDR_start_addr ),	//[31:0]
        .npi_burst_cc_fifo_din( { NpiData1, NpiData0 } ),
        .npi_burst_cc_fifo_wr_en( Npi_1_WrEn ),			// POSITION
        .npi_burst_cc_fifo_RstIn( npi_1_strb_reset ),	//reset | User Reset
        .npi_burst_cc_din_Clk( clk ),
        .sdi2_cc_MyLocalLoopbackDataValid_o(sdi2_cc_MyLocalLoopbackDataValid_o),
        .sdi2_cc_RemotePacketDroppedStatus(sdi2_cc_RemotePacketDroppedStatus),
        .Sysmon_ALARM(),
        // COEFF SET
        .w_reg0 ( plb_w_reg[0] ),	//data
        .w_reg1 ( plb_w_reg[1] ),	//control
        .w_reg2 ( plb_w_reg[2] ),
        .w_reg3 ( plb_w_reg[3] ),
        .w_reg4 ( plb_w_reg[4] ),
        .w_reg5 ( plb_w_reg[5] ),
        .w_reg6 ( plb_w_reg[6] ),
        .w_reg7 ( plb_w_reg[7] ),
        .w_reg8 ( plb_w_reg[8] ),
        .w_reg9 ( plb_w_reg[9] ),
        .w_reg10 ( plb_w_reg[10] ),
        .w_reg11 ( plb_w_reg[11] ),
        .w_reg12 ( plb_w_reg[12] ),
        .w_reg13 ( plb_w_reg[13] ),
        .w_reg14 ( plb_w_reg[14] ),
        .w_reg15 ( plb_w_reg[15] ),
        //read registers
        .rw_reg20 ( plb_r_reg20 ),
        .rw_reg21 ( plb_r_reg21 ),
        .rw_reg22 ( plb_r_reg22 ),
        .rw_reg23 ( plb_r_reg23 ),
        .rw_reg24 ( plb_r_reg24 ),
        .rw_reg25 ( plb_r_reg25 ),
        .rw_reg26 ( plb_r_reg26 ),
        .rw_reg27 ( plb_r_reg27 ),
        .rw_reg28 ( plb_r_reg28 ),
        .rw_reg29 ( plb_r_reg29 ),
        .rw_reg30 ( plb_r_reg30 ),
        .rw_reg31 ( plb_r_reg31 )
    );
		
    //SYSTEM RESET
    //assign 	reset = sys_rst_pin;
    // 11/01/22 added
    ad_rst ad_rst_i (
        .rst_async(sys_rst_pin),
        .clk (clk),
        .rstn(),
        .rst (reset)
    );
    
    
    // 04/09/19 EVR PM reset
    //always @ (posedge clk)
    //begin
    //    evr_t6_T0Reg      <= plb_w_reg[13][28]; 	//Global Clearing
    //    evr_t6_T0Reg_reg1 <= evr_t6_T0Reg;
    //    evr_t6_T0Reg_reg2 <= evr_t6_T0Reg_reg1;
    //    EVR_PM_Reset      <= (evr_t6_T0Reg_reg1 & !evr_t6_T0Reg_reg2);
    //end

    //10/14/22
    ResetTrigSync(
        .sysClk     (clk),
        .reset      (reset),
        .ResetTrigIn(plb_w_reg[13][28]),
        .trig       (Trig1Start),   //10 kHz trigger
        .trigSyncOut(EVR_Sync_Reset)
    );
        
    stretch	evr_trig6_st
    (
        .clk(clk),
        .reset(reset),
        //.sig_in(EVR_PM_Reset),
        .sig_in(EVR_Sync_Reset),
        .len(32'd25000000),		// 500 ms @ 20 ns
        .sig_out(EVR_PM_Reset_Sts)
    );
	
    //assign	GlobalLatchClearingReset  = EVR_PM_Reset;	//04/09/19
    assign	GlobalLatchClearingReset  = EVR_Sync_Reset;	//10/14/22
    assign  uBlazeDumpCmd = plb_w_reg[13][6];		// dump command from uBlaze
        //04/09/19
    assign  AIID_process_RST = plb_w_reg[13][8];	//NOT USED

	
    //fixed timing
    reg Dly1_bpmPosDataValid, Dly2_bpmPosDataValid;
    always @ (posedge clk) begin
        Dly1_bpmPosDataValid <= bpmPosDataValid;
        Dly2_bpmPosDataValid <= Dly1_bpmPosDataValid;
    end
	
    //mask control register
    always @ (posedge clk) begin
        UserMaskControl_0 <= plb_w_reg[5];
        UserMaskControl_1 <= plb_w_reg[4];
    end

    assign  aie_USET_MASK_SETTING = { 32'h00000000, UserMaskControl_0 };

    //NPI addr calculations
    assign npi_DDR_start_addr = `NPI_DDR3_START;	//base address for NPI hD0000000

    //select internal or external trigger
    assign 	TRIGGER_1 = (plb_WR_Reg6[28] == 1'b1) ? intTrig :  evr_TRIGGER_1;
	
    //internal clock generator
    blinker #(
        .GEN_CLK_FREQUENCY(50000000),
        .GEN_BLINK_PERIOD(0.0001)		//10 kHz
    )
    led__sysclk2
    (
        .P_I_CLK(clk),
        .P_O_LED(intTrig)
    );
    wire	ext_heartbit_2kHz;
    blinker #(
        .GEN_CLK_FREQUENCY(10000),
        .GEN_BLINK_PERIOD(0.0005)		//2 kHz
    )
    led__ext_heartbit_2kHz
    (
        .P_I_CLK( TRIGGER_1 ),
        .P_O_LED(ext_heartbit_2kHz)
    );
	

    stretch	_npi_0_reset_m2
    (
        .clk(clk),
        .reset(reset),
        .sig_in(npi_0_pm_reset),
        .len(32'd1000000),		// d1000000 = 20 ms
        .sig_out(npi_0_pm_reset_str)
    );

    // Register Read by microblaze
    wire [15:0] k_addr = plb_WR_Reg7[15:0];	//for access 1024   8/27/13

    reg  [31:0] rd_reg25, rd_reg20;
    assign plb_r_reg25 = rd_reg25;

    always @(*) case( plb_Addr0[22:18] ) //b11111 = 31
        5'd0:  rd_reg25 <= xy_pos_Reg[k_addr];
        /*
        4'd0:  begin
                if(xy_pos_Reg[k_addr] > 25'h1FFFFFF)
                        rd_reg25 <= { 6'h3F, xy_pos_Reg[k_addr][25:0] };
                else
                        rd_reg25 <= { 6'h00, xy_pos_Reg[k_addr][25:0] };
        end
        */
        5'd1:  rd_reg25 <= bm_iL_info[k_addr];
        5'd2:  rd_reg25 <= Gap_Status01[k_addr];
        5'd3:  rd_reg25 <= PhotonShutters_Status01[k_addr];
        5'd4:  rd_reg25 <= IDAI_BeamDumpControlReg[k_addr];
        5'd5:  rd_reg25 <= IDAI_RunStopStatusReg[k_addr];
        5'd6:  rd_reg25 <= aie_USET_MASK_SETTING[k_addr];
        5'd7:  rd_reg25 <= SysMonData[k_addr];
        //05/20/2014 new added
        5'd8:  rd_reg25 <= aie_USET_MASK_SETTING_REG[k_addr];
        5'd9:  rd_reg25 <= CellPLC_IdBeamDump_Status[k_addr];
        5'd10: rd_reg25 <= CellPLC_BmBeamDump_Status[k_addr];
        5'd11: rd_reg25 <= CellPLCWdStatus[k_addr];
        5'd12: rd_reg25 <= BM_PhotonShutters_Status01[k_addr];
        5'd13: rd_reg25 <= y1_pos_buff[k_addr];
        5'd14: rd_reg25 <= y2_pos_buff[k_addr];

        //01/20/17 SDI fault detection after beam dump
        5'd15:  rd_reg25 <= SdiLinkData_Reg[k_addr];
        
        // 10/18/22 Available space for readout
        
        5'd29:  rd_reg25 <= 32'd29;
        5'd30:  rd_reg25 <= 32'd30;
        5'd31:  rd_reg25 <= 32'd31;                
    endcase

    // 5/30/13 replace from pos_edge
    // Different clock domain
    // input 125 MHz, output 62.5 MHz
    reg T0Reg, T0Reg_reg1, T0Reg_reg2;
    always @ (posedge clk)
    begin
        T0Reg <= TRIGGER_1;
        T0Reg_reg1 <= T0Reg;
        T0Reg_reg2 <= T0Reg_reg1;
        Trig1Start <= (T0Reg_reg1 & !T0Reg_reg2);
    end


    reg T1Reg, T1Reg_reg1, T1Reg_reg2;
    always @ (posedge clk)
    begin
        T1Reg <= sdi2_cc_StartCalc;	//40 ns (8 ns * 5) at user_logic.v
        T1Reg_reg1 <= T1Reg;
        T1Reg_reg2 <= T1Reg_reg1;
        TRIG1 <= (T1Reg_reg1 & !T1Reg_reg2);
    end

    // NPI global trigger by EVG soft event code
    soft_event_trig  npi_trig_sync
    (
        .clk(clk),
        .reset(reset),
        .clk_enable(1'b1),
        .evg_trig(Trig1Start),             //10 kHz trigger
        .evg_soft_event( evr_TRIGGER_7 ),  //soft trigger
        .trig_out(npi_sync_trig)
    );
	
    ///////////////////////////////////
    // SDI DPRAM Simulator DPRAM
    ///////////////////////////////////
    bpm_xy_position_sim sdi_xy_sim (
        .clk(clk),
        .reset(reset),
        .trig( Trig1Start ), 					//Cal output
        /* *microblaze interface* */
        .SimDataMicroblazeMode( plb_WR_Reg7[28] ),	//sim data mode 1: Count, 0: Microblaze control
        .CS_a( ram_cs2[7] ),       			//cs   1
        .WR_a( ram_WR2 ),          			//wr   1
        .Ram_Addr_a( ram_addr2[9:0] ),   	//addr 9:0
        .Ram_Data_a( ram_Data2 ),        	//data 31:0
        /* ----- */
        .StartAddr(12'd0),
        .EndAddr( {2'b00, sdi_max_signal} ),
        .sel_addr( plb_WR_Reg6[21:10] ), 		//12'd5),
        .Kx(20'd20000), 						//max 32767
        .Kxoff(20'd0),
        .mode( plb_WR_Reg6[31] ), 				//0: all memory write(0~359), 1: only selected one memory address write and DAC output test
        .DataTypeIsRamp(plb_WR_Reg7[29]),		//1: Ramp, 0:sin
        .DpRamB_Read( dpram_rd ), 				//in for DPRAM read
        .DpRamB_Address({2'd0, dpram_addr} ), 	//in for DPRAM read
        .PosXyData( BpmXy_SimData ),				//x,y position data output
        .PosXyDataValid( PosXyDataValid ),
        .PosSingleDataValid( PosSingleDataValid ),
        .DAC_DataOut( DAC_DataOut ),		//write to DAC port
        .WrDone(mySim_Trig)			//T1
    );
	
    //
    // Mux for Bpm Simulation or Sdi real Data ?
    wire 	PosSimulation = plb_WR_Reg6[29];

    assign 	SdiOrSimGlobalBpmData = (PosSimulation == 1'b1) ? BpmXy_SimData :  CC_Sdi_RamData;

    wire	DspTrig;	//T2
    assign 	DspTrig = (PosSimulation == 1'b1) ? mySim_Trig : TRIG1;

    assign  sdi_max_signal    = plb_w_reg[14][19:10];		//max 780 26*30
    assign  BpmAllValidNumber = plb_w_reg[14][29:20];		//180 for RF bpm, if ID bpm installed ? add more
	
    //05/13/2013 New module
    /////// SDI Data Read only Bpm Data ///////////////
    bpm_pos_read 	SDI_PositionRead (
        .clk(clk),
        .trig( DspTrig ),
        //.cell_max_number(plb_w_reg[14][4:0]),	//5bit  max=26 : 7/03/13
        .cell_max_number(5'd26),                //It's fixed number
        .cell_bmp_number(plb_w_reg[14][9:5]),   //5bit  used=12 for ONLY RF BPM
        .address_move(10'd0),
        .AddressStart(10'd0),
        .AddressEnd(sdi_max_signal),            //max address 360 .. 780
        .even_bit(),
        .addr(),
        .wr_one(),
        .bpmRam_RD(dpram_rd),                   //SDI DPRAM Read
        .bpmRam_ADDR(dpram_addr),               //SDI DPRAM address OUTPUT
        .bpmRamCellMark(bpmRamCellMark),
        .cell_cnt(),
        .pos_start_addr(),
        .bram_x_wr(bram_x_wr),                  //Write SDI DATA to Next DPRAM
        .bram_y_wr(bram_y_wr),
        .bram_x_addr( bram_x_addr ),            //Address to Next DPRAM
        .bram_y_addr( bram_y_addr ),
        .bpmPosDataValid( bpmPosDataValid),
        .bpmPosFoFbDataValid( bpmPosFoFbDataValid),
        .DspMatrixCalStartTrig( DspMatrixCalStartTrig)
    );
	 
     	    
    ///////////////////////////////////
    // CC Link Packet Generator for Pattan test
    ///////////////////////////////////
    wire	cw_pkt_start_trig, ccw_pkt_start_trig;
    wire	cw_pkt_trig, ccw_pkt_trig;

    //CW packet delay
    NpiStrobe_gen 	CC_LocalPktDelay(
        .Clock(evr_0_evr125RfLockedClk),
        .Reset(reset),
        .myEvent( TRIGGER_1 ),
        .myDelay(32'd1),	//delay min 1
        .myWidth( {16'd0, plb_w_reg[9][15:0]} ),
        .trigger(cw_pkt_trig),
        .npiRst()
    );
    //ccw packet delay
    NpiStrobe_gen 	CCW_LocalPktDelay(
        .Clock(evr_0_evr125RfLockedClk),
        .Reset(reset),
        .myEvent( TRIGGER_1 ),
        .myDelay(32'd1),	//delay
        .myWidth( {16'd0, plb_w_reg[9][31:16]} ),
        .trigger(ccw_pkt_trig),
        .npiRst()
    );
	
    //CW local packet generator
    cc_countdatagen 	CW_LinkDataPacketGen(
        .Reset(reset),
`ifdef 	CC_SDI_IN_CLK_50_MHZ
        .sdi_clk( clk ),
`else
        .sdi_clk( evr_0_evr125RfLockedClk),
`endif
        .Trigger ( cw_pkt_trig ),                         //Trig1 width negative edge for packet start
        .maxCount( {plb_Reg10[23:16], 8'd0, 16'd10000} ), //plb_Reg10[23:16]=26
        .fa_pos_x(32'd0),
        .fa_pos_y(32'd1),
        .DataSel(plb_WR_Reg6[30]),                       // 1:tmpLocalData, 0:cos
        .wfmKx(32'd1000),                                //{16'd0, plb_Reg9[15:0]}),
        .wfmKy(32'd0),                                   // Ky, Not used
        .wfmPhase_inc(32'h028F5D70),                     // Frequency control
        .LocalDataValid(sdi2_CW_LocalPktDataValid),
        .LocalCountData( sdi2_CW_LocalPktData ),         //max range + 65536
        .LocalBpmPosData(),
        .RampRst(),
        .fa_evr_trig( cw_pkt_start_trig )                //event synced output
    );
    
    //CCW local packet generator
    cc_countdatagen 	CCW_LinkDataPacketGen(
        .Reset(reset),
`ifdef 	CC_SDI_IN_CLK_50_MHZ
        .sdi_clk( clk ),
`else
        .sdi_clk( evr_0_evr125RfLockedClk),
`endif
        .Trigger ( ccw_pkt_trig ), //TRIGGER_1 ),		//Trig1 width negative edge for packet start
        //.Trigger( sdi2_cc_DPRAM_ReadStart),        //20 ns stp
        //.maxCount( {8'd14, 8'd0, 16'd10000} ), 		// Msb 8-bit is length, default is 14
        .maxCount( {plb_Reg10[23:16], 8'd0, 16'd10000} ),		//plb_Reg10[23:16]=26
        .fa_pos_x(32'd0),
        .fa_pos_y(32'd1),
        .DataSel(plb_WR_Reg6[30]), 				// 1:tmpLocalData, 0:cos
        .wfmKx(32'd1000),	//({16'd0, plb_Reg9[15:0]}),
        .wfmKy(32'd0),								// Ky, Not used
        .wfmPhase_inc(32'h028F5D70),				// Frequency control
        .LocalDataValid(sdi2_CCW_LocalPktDataValid),
        .LocalCountData( sdi2_CCW_LocalPktData ),    	//max range + 65536
        .LocalBpmPosData(),
        .RampRst(),
        .fa_evr_trig( ccw_pkt_start_trig )            //event synced output
    );
         
		 
    //////////////////////////////////////////////////////////////////
    ////// Global RX, Y eference and Position buffer
    //////////////////////////////////////////////////////////////////
    wire	[31:0] plb_r_reg31_00, plb_r_reg31_01;
    assign plb_r_reg31 = ( plb_w_reg[15][11:10] == 3'b000) ? plb_r_reg31_00 :
                          (plb_w_reg[15][11:10] == 3'b001) ? plb_r_reg31_01 :
                          (plb_w_reg[15][11:10] == 3'b010) ? u_fifo_out     :		//10/07/14
                          32'd0;
    // sr-bpm offset
    refDpram_32x1024 sr_bpm_ref_offset_xy (
        //uBlaze PLB interface
        .clka(clk),             // input clka
        .ena(ram_cs2[0]),       //cs
        .wea(ram_WR2),          //wr
        .addra(ram_addr2),      //addr
        .dina(ram_Data2),       //data
        .douta(plb_r_reg31_00),     // output [31 : 0] douta
        // read from fofb controller
        .clkb(clk), // input clkb
        .enb(1'b1), // input enb
        .web(1'b0), // input [0 : 0] web
        .addrb( bpmPos_xy_Addr), //ref_rd_addr), 	// input [8 : 0] addrb
        .dinb(32'd0), 			// input [31 : 0] dinb
        .doutb(sr_bpm_ref_offset_xy_set) 	// output [31 : 0] doutb
    );

    // 03/31/2014 added for sr-bpm origin offset
    refDpram_32x1024 sr_bpm_org_offset_xy (
        //uBlaze PLB interface
        .clka(clk),             // input clka
        .ena(ram_cs2[5]),       //cs
        .wea(ram_WR2),          //wr
        .addra(ram_addr2),      //addr
        .dina(ram_Data2),       //data
        .douta(plb_r_reg31_01),     // output [31 : 0] douta
        // read from fofb controller
        .clkb(clk), // input clkb
        .enb(1'b1), // input enb
        .web(1'b0), // input [0 : 0] web
        .addrb( bpmPos_xy_Addr), //ref_rd_addr), 	// input [8 : 0] addrb
        .dinb(32'd0), 			// input [31 : 0] dinb
        .doutb(sr_bpm_origin_offset_xy_set) 	// nm
    );


    ////////////////////////////////////
    // X,y position data are same memory
    // y offset address is 180
    // BpmAllValidNumber = 180 for ONLY RF BPM
    // BpmAllValidNumber = 240 for RF BPM + ID BPM
    wire	[31:0] bpmPos_Xy_Data_raw;
    assign xy_pos_addr = (bram_x_wr == 1'b1) ? {1'b0,bram_x_addr} : bram_y_addr + BpmAllValidNumber;
    posDpram_32x512 x_y_pos (	//1024
        .clka(clk), // input clka
        .ena(1'b1), // input ena
        .wea(bpmPosDataValid), //Must need WR
        .addra(xy_pos_addr), // input [8 : 0] addra
        .dina( SdiOrSimGlobalBpmData ), // from SDI DPRAM or Sim DPRAM
        //read by fofb
        .rstb(reset),
        .clkb(clk), // input clkb
        .enb(1'b1), // input enb
        .addrb(bpmPos_xy_Addr), // input [8 : 0] addrb
        //.doutb(bpmPos_Xy_Data) 	// to Eigen Mode calculation
        .doutb(bpmPos_Xy_Data_raw)
    );
	
    //Added 04/11/2014 if Mask is set 0, User Position Data will be ZERO
    assign bpmPos_Xy_Data = (xy_Mask_DataValid == 1'b1) ? bpmPos_Xy_Data_raw : 32'd0;

    wire	[31:0] mask_ram_data_a;

    //1024 fifo memory for Masking
    xyMaskRam_1x1024 xy_Mask_Ctrl (
        .clka  (clk),
        .ena   (ram_cs2[1]),
        .wea   (ram_WR2),
        .addra (ram_addr2),
        .dina  (ram_Data2),
        .douta (mask_ram_data_a), 	//read from uBlaze
        .clkb  (clk),
        .enb   (1'b1),
        .web   (1'b0),
        .addrb (bpmPos_xy_Addr),
        .dinb  (32'd0),
        .doutb (UserBramOutXyMaskOut)
    );


    /*
    * this address generator read position RAM and reference RAM
    */
    wire bpmPos_xy_Addr_Valid;
    fofb_addr_gen 	addr_gen (
        .clk(clk),
        .reset(reset),
        .trig(DspMatrixCalStartTrig),
        .clk_enable(1'b1),
        .Gain(16'd1),
        .address_move(12'd0),
        .AddressStart(12'd0),
        .AddressEnd({2'b00, sdi_max_signal} ),		//0 - 780
        .ce_out(),
        .Data16b(),
        .Data32b(),
        .addr(bpmPos_xy_Addr), 		//10bit
        .wr_one(),
        .wr(bpmPos_xy_Addr_Valid)
    );
        	
	
    /* ************************************************************************
    *	Read global position data for interlock calculation
    *	SdiOrSimGlobalBpmData [779:0]
    * ************************************************************************
    */
    //wire [31:0] xy_pos_read;
    reg [31:0] xy_pos_Reg [`MAX_LEN_SDI_DATA-1:0];
    reg [31:0] SdiLinkData_Reg [`MAX_LEN_SDI_DATA-1:0];		// 01/20/17

    integer pos_i;
    initial begin
        for (pos_i=0; pos_i<`MAX_LEN_SDI_DATA; pos_i=pos_i+1) begin
            xy_pos_Reg[pos_i] = 32'd0;
        end
    end
  	
    /* Copy SDI DPRAM to Register */
    always @ (posedge clk)
    begin
        if (SDI_Reg_WR_ENABLE == 1'b1) begin	//04/13/15 if out of envelop, frozon waveform
            xy_pos_Reg[ SdiRamAddr ] <= SdiOrSimGlobalBpmData;
            // 01/20/17 added for fault detection
            if( BM_ERR_DUMP==1'b0 && ID_DUMP == 1'b0 ) begin
                SdiLinkData_Reg[ SdiRamAddr ] <= SdiOrSimGlobalBpmData;
            end
        end
    end
    //assign xy_pos_read = xy_pos_Reg[ plb_w_reg[15][9:0] ];
  

    // 03/23/14
    wire   	SdiRamSingleValid;
    reg		SdiRamSingleValid_d1;
    //plb_WR_Reg6[9:0]
    assign SdiRamSingleValid = (plb_w_reg[15][9:0] ==  SdiRamAddr) ? (1'b1 && SdiDataRdValid) : 1'b0;	//time correction
    always @ (posedge clk) begin
        SdiRamSingleValid_d1 <= SdiRamSingleValid;
    end

    //V123*2 = 6, reference orbit (2), mask bpm (2)
    multiple_ram_cs DpRamControl (
        .clk     (clk),
        .reset   (reset),
        //uBlaze PLB interface
        .RAM_sel (plb_Addr1[16:13]),    //plb reg3 [16:13]
        .Data_in ( plb_Data1 ),         //plb reg2 [31:0]
        .Write_in(plb_Addr1[17]),       //plb reg3 [17]
        .Addr_in (plb_Addr1[12:0]),     //plb reg3 [12:0]
        //
        .Ram_Addr(ram_addr2),
        .ram_cs  (ram_cs2),
        .Data    (ram_Data2),
        .wr      (ram_WR2)
    );
		
		

    //plb_Reg10[13] - PLC ID enable/disable
    //                     8
    // epics DB reg22-I 240
    assign plb_r_reg22 = {
        aie_DumpStatus[7:0],
        IDAI_BeamDumpControlReg[7:0], 	//
        plb_w_Reg12[2],					//[15]
        bpm_id_12_rx_status[1],			//[14]
        bpm_id_12_rx_status[0],			//[13]
        ai_id_multiple_err_status,    	//[12]
        ID_DUMP, 			//[11]
        plb_w_Reg12[0],         //AI_ID_MODE, 		//[10]
        plb_w_reg[15][15], 	//[9]  
        plb_w_reg[15][14], 	//[8]
        npi_0_pm_reset_str, 1'b0, 1'b0, ai_active, 	//[7:4]
        plb_w_Reg12[1], plb_w_reg[15][13], plb_w_reg[15][12], plb_Reg10[13]};	//[3:0]

		
    ///////////////////////////
    ///// GLobal Register	///
    ///////////////////////////
    /* Address decode for controls
    *
    *	cs1[0] - cs1[11]
    */
    multiple_ram_cs RdRegCsControl (
        .clk     (clk),
        .reset   (reset),
        .RAM_sel (plb_Addr0[16:13]), 	//plb reg1 [16:13]
        .Data_in ( plb_Data0 ),			//plb reg0 [31:0]
        .Write_in(plb_Addr0[17]), 		//plb reg1 [17]
        .Addr_in (plb_Addr0[12:0]),		//plb reg1 [12:0]
        //
        .Ram_Addr(ram_addr1),
        .ram_cs  (ram_cs1),
        .Data    (ram_Data1),
        .wr      (ram_WR1)
    );
		
/*
    wire	[9:0] base_addr[10:0];
    assign	base_addr[0] = set_base[9:0];
    assign	base_addr[1] = set_base[19:10];
    assign	base_addr[2] = set_base[29:20];
    assign	base_addr[3] = set_base[39:30];
    assign	base_addr[4] = set_base[49:40];
    assign	base_addr[5] = set_base[59:50];
    assign	base_addr[6] = set_base[69:60];
    assign	base_addr[7] = set_base[79:70];
    assign	base_addr[8] = set_base[89:80];
    assign	base_addr[9] = set_base[99:90];
*/	

    // Create BRAM block for uBlaze interface 0 - 12
    genvar ref_i;
    generate	//15
        for (ref_i=0; ref_i < `NOF_LIMIT_SET+1; ref_i=ref_i+1) begin  : Reference_Set
        setpoint_mem 	ram_SetOffset(
          .clka  (clk),
          .ena   (ram_cs1[ref_i]),
          .wea   (ram_WR1),
          .addra (ram_addr1),
          .dina  (ram_Data1),
          .douta ( SetRamReadDoutA[ref_i] ),
          //
          .clkb  (clk),
          .enb   (AIeCalValid),
          .web   (1'b0),
          .addrb ( aie_addr ),
          .dinb  (32'd0),
          .doutb ( SetRamDoutB[ref_i] )
        );
        end
    endgenerate
	
	
    //BRAM reading
    mux24 	Ref_reading
    (
        .sel( {1'b0, plb_w_Reg12[27:24]} ),		//  input   [4:0] sel;  // ufix5
        .a1(SetRamReadDoutA[0]),
        .a2(SetRamReadDoutA[1]),
        .a3(SetRamReadDoutA[2]),
        .a4(SetRamReadDoutA[3]),
        .a5(SetRamReadDoutA[4]),
        .a6(SetRamReadDoutA[5]),
        .a7(SetRamReadDoutA[6]),
        .a8(SetRamReadDoutA[7]),
        .a9(SetRamReadDoutA[8]),
        .a10(SetRamReadDoutA[9]),
        .a11(SetRamReadDoutA[10]),
        .a12(SetRamReadDoutA[11]),
        .a13(SetRamReadDoutA[12]),
        .a14(SetRamReadDoutA[13]),
        .a15(SetRamReadDoutA[14]),
        .a16(32'd0),
        .a17(32'd0),
        .a18(32'd0),
        .a19(32'd0),
        .a20(32'd0),
        .a21(32'd0),
        .a22(32'd0),
        .a23(32'd0),
        .a24(32'd0),
        .Mux_Data_o( RefDpramOut )
    );
		  
				
    ///////////////////////////////////
    ////// DAC OUTPUT MAPPING /////////
    ///////////////////////////////////
    wire dac_sdo, dac_sync, dac_clr, dac_load, dac_sdin, dac_sclk;
    wire [7:0] dac_dbg;
    wire Dac10kHzDataClk;
    //DAC wire connection
    OBUFDS #( .IOSTANDARD("LVDS_25") ) OBUFDS_inst1 (.O(dac_load_p), .OB(dac_load_n), .I(dac_load) );
    OBUFDS #( .IOSTANDARD("LVDS_25") ) OBUFDS_inst2 (.O(dac_clr_p),  .OB(dac_clr_n),  .I(dac_clr) );
    OBUFDS #( .IOSTANDARD("LVDS_25") ) OBUFDS_inst3 (.O(dac_sync_p), .OB(dac_sync_n), .I(dac_sync) );
    OBUFDS #( .IOSTANDARD("LVDS_25") ) OBUFDS_inst4 (.O(dac_sdin_p), .OB(dac_sdin_n), .I(dac_sdin) );
    OBUFDS #( .IOSTANDARD("LVDS_25") ) OBUFDS_inst5 (.O(dac_sclk_p), .OB(dac_sclk_n), .I(dac_sclk) );
    IBUFDS #(
    .CAPACITANCE("DONT_CARE"),.DIFF_TERM("TRUE"),.IBUF_DELAY_VALUE("0"),.IFD_DELAY_VALUE("AUTO"),.IOSTANDARD("LVDS_25")
    ) IBUFDS_inst41 ( .O(dac_sdo), .I(dac_sdo_p), .IB(dac_sdo_n));
			
    	
    // 2 clock delay, because dpram Latency is 2 clock
    reg [9:0] tmp_dpaddr0, tmp_dpaddr1;
    always @ (posedge clk) begin
        tmp_dpaddr0 <= dpram_addr;
        tmp_dpaddr1 <= tmp_dpaddr0;
    end
    assign	SdiRamAddr = tmp_dpaddr1;


    reg [1:0] tmpSdiDataRdValid;
    always @ (posedge clk) begin
        tmpSdiDataRdValid[0] <= bpmPosDataValid;
        tmpSdiDataRdValid[1] <= tmpSdiDataRdValid[0];
    end
    assign SdiDataRdValid = tmpSdiDataRdValid[1] && bpmPosDataValid;		//Feb/16/2014 timing correction


    // 1: Local SDI output to DAC, 0: Global SDI output to
    wire DacInIsLocalSDI = plb_WR_Reg7[27];

	
    /*
     Important : bpm SDI DPRAM clk = 100 MHz, CC SDI input clk = 50 MHz
                 Local SDI output and Remote SDI output CLK must be same.
    */

    // BPM_SDI_DPRAM_CLK is 125 MHz, CC_SDI DPRAM Output clock 50 MHz, so DAC_Interface clock is 50 MHz
    // Only acceptable CC_SDI_DPRAM
    //assign 	DacStrobe = (PosSimulation == 1'b1) ? PosXyDataValid : SdiDataRdValid ;
`ifdef	__NO_REG__	
    wire DacStrobe;
    wire [9:0]  DacAddr;
    wire [31:0] DacData;

    assign 	DacStrobe = PosXyDataValid;
    assign 	DacAddr   = SdiRamAddr;				//[9:0]
    assign 	DacData   = SdiOrSimGlobalBpmData;
`else
    reg DacStrobe;
    reg [9:0]  DacAddr;
    reg [31:0] DacData;

    always @ (posedge clk) begin
        DacStrobe <= PosXyDataValid;
        DacAddr   <= SdiRamAddr;				//[9:0]
        DacData   <= SdiOrSimGlobalBpmData;
    end
`endif	
	
    wire [15:0] dac_ch_a;
    //assign 	dac_ch_a   = ( plb_w_reg[15][15] == 1'b1) ? plb_w_reg[15][31:16] :  wfm_fifo_out[15:0];
    assign 	dac_ch_a   = plb_w_reg[15][31:16];

    wire	[15:0] DACOUT[3:0];
    reg		[31:0] bpm_x_dac;
    wire   DAC_ENABLE;
    assign DAC_ENABLE = (DacAddr[9:0] == plb_WR_Reg6[9:0] ) ? 1'b1 : 1'b0;


    always @(posedge clk ) begin
        if( DAC_ENABLE == 1'b1 & DacStrobe == 1'b1 ) begin
            bpm_x_dac <= DacData;
        end
        else begin
            bpm_x_dac  <= bpm_x_dac;
        end
    end
	
		
    //DAC clock delay adjustable
    // 100 ns - 100
    NpiStrobe_gen DAC_CLK_GEN(
        .Clock(clk),
        .Reset(reset),
        .myEvent( Trig1Start ),
        .myDelay( 32'd6000),	// 60 us  SysCtrlRegs[27] + 10),	//delay 100 ns 80 us
        .myWidth( 32'd5000 ), 	// 50 us width
        .trigger( Dac10kHzDataClk ),
        .npiRst()
    );
	
    BUFG _clkout1_bufg_i
    (
        .O (gClk10kHz),
        .I (Dac10kHzDataClk)
    );

	
    //03/13/2015 added
    /*
      SysCtrlRegs[18]
       gain 1     = 32'h7fffffff;
       gain 0.1   = 32'h0ccccccd;
       gain 0.01  = 32'h0147ae14;
       gain 0.001 = 32'h0020c49c;
    */
    wire [15:0] pos_xy_dac16_out1;
    pos_xy_dac16   u_pos_xy_dac16
    (
        .clk(clk),
        .reset(1'b0),
        .enb(DAC_ENABLE),
        .pos_nm( bpm_x_dac ),
        .scale( 32'h0ccccccd ),  // 0.1
        .OutU20b(pos_xy_dac16_out1)
    );
			
			
    assign	DACOUT[0] = bpm_x_dac + 15'd32767;
    assign	DACOUT[1] = dac_ch_a;
    assign	DACOUT[2] = (plb_WR_Reg6[9:0] * 30);
    assign	DACOUT[3] = pos_xy_dac16_out1;

    DAC_Interface_v2 DAC2
    (
        .sysClk(clk),				//100 MHz
        .Reset( reset ),
        .dacClk( gClk10kHz ),		//10 kHz
        .OutU16b_A( DACOUT[0] ),
        .OutU16b_B( DACOUT[1] ),
        .OutU16b_C( DACOUT[2] ),
        .OutU16b_D( DACOUT[3] ),
        //DAC interface
        .dacSync(dac_sync),        	// dacSync
        .dacSclk(dac_sclk),       	// dacSclk
        .dacSdin(dac_sdin),			// SERIAL DATA OUT
        .dacLoad(dac_load),
        .dacClr(dac_clr),
        .dbg(dac_dbg)
        );
		
	    
    wire led_trigger0;
    stretch	evr_trig
    (
    `ifdef 	CC_SDI_IN_CLK_50_MHZ
        .clk(clk),
    `else
        .clk(evr_0_evr125RfLockedClk),
    `endif
        .reset(reset),
        .sig_in(evr_TRIGGER_0),
        .len(32'd5000000),		// 8ns * 6250 = 50.0 us
        .sig_out(led_trigger0)
    );
	  
	
    //SFP EEPROM read
    wire [15:0] sfp_eeprom[5:0];
    sfp_i2c_readout sFp_Read (
        .clk(clk),
        .reset(reset),
        .strobe( plb_WR_Reg7[25] ),	//trig for read
        .addr(plb_WR_Reg7[24:16] ), 	//address eeprom [8:0] 0 - 511
        .sfp_sclk(sfp_sclk),
        .sfp_sdata(sfp_sdata),
        .sfp0_i2c_data( sfp_eeprom[0]),
        .sfp1_i2c_data( sfp_eeprom[1]),
        .sfp2_i2c_data( sfp_eeprom[2]),
        .sfp3_i2c_data( sfp_eeprom[3]),
        .sfp4_i2c_data( sfp_eeprom[4]),
        .sfp5_i2c_data( sfp_eeprom[5])
    );
	
    //uMlaze soft reboot
    multiboot SoftReBOOT (
        .clk( clk ),
        .trig( plb_WR_Reg7[26] ), //Soft reboot
        .CLK_OUT(),
        .icap_error()
    );

    //Corrector Output monitoring
    assign fifo_we = (~full) && fifo_we_envelop;
    assign fifo_rd = (ram_cs2[2] && ram_WR2); // && ~empty;
		
    soft_event_trig  fifo_trig_sync
    (
        .clk(clk),
        .reset(reset),
        .clk_enable(1'b1),
        .evg_trig(Trig1Start),					//10 kHz trigger
        .evg_soft_event( plb_Addr1[20] ),	//soft trigger
        .trig_out(sync_trig)
    );

    NpiStrobe_gen fifo_trig(
        .Clock(clk),
        .Reset(reset),
        .myEvent( sync_trig ),
        .myDelay(32'd1),
        .myWidth( {plb_w_reg[15][31:16]*10}),  //32'd50000),
        .trigger(fifo_we_envelop),
        .npiRst(fifo_rst)
    );
	
    ///////// ID MUX read /////////
    assign u_fifo_we = (~u_fifo_full) && fifo_we_envelop && muxValid;	//10/07/14
    assign u_fifo_rd = (ram_cs2[6] && ram_WR2); // && ~u_fifo_empty;
    fifo_32x2K 	ID_CalMux_mon_fifo (
        .clk(clk),
        .rst(fifo_rst),
        .din(mux_Data_o), 	// input [31 : 0] din
        .wr_en(u_fifo_we), 		// input wr_en
        .rd_en(u_fifo_rd), 		// input rd_en
        .dout(u_fifo_out), 		// output [31 : 0] dout
        .full(u_fifo_full), 	// output full
        .empty(u_fifo_empty) 	// output empty
    );
    /////////////////////////
	
	
    wire xy_err_fifo_we, xy_err_fifo_rd, xy_err_fifo_full;
    //assign xy_err_fifo_we = (~xy_err_fifo_full &&   xy_err_fifo_mask_bit) && fifo_we_envelop;

    assign xy_err_fifo_we = (~xy_err_fifo_full &&   xy_Mask_DataValid) && fifo_we_envelop ;
    assign xy_err_fifo_rd = (ram_cs2[9] && ram_WR2); // && ~xy_err_fifo_empty;


    //RF BPM Closed Orbit
    posDpram_32x512 	xy_pos_mon_bram (
        .clka (clk),
        .ena  (1'b1),
        //.wea(xy_Mask_DataValid), 	//Valid, only RF BPM X,y position
        .wea  (FINAL_FREE_DUMP_OUT && xy_Mask_DataValid),	// 04/13/15 added, if out of envelop ? frozon waveform
        .addra(BmPosAddress), 	// input [8 : 0] addra
        .dina (bpmPos_Xy_Data), 		//x,y closed orbit
        //read by uBlaze
        .rstb (reset),
        .clkb (clk),
        .enb  (ram_cs2[8]),
        .addrb(ram_addr2[8:0]), 		// input [8 : 0] addrb
        .doutb(xy_pos_fifo_out)
    );

	
    ///////////////////////////////////////////////////////////////
    ///// Active Interlock Logic
    ///// max 60 beamlines
    ///////////////////////////////////////////////////////////////
    //parameter ADDR_BM_LOW_SP 	= 1020;
    //parameter ADDR_BM_HI_SP 	= 1021;

    wire	BM_LATCHED_OUT, bm_high, bm_low;

    wire 	IL_SUM;
    wire	[13:0] BM_IL_ADDRESS;
    wire	bm_il_sum;
    wire [13:0] bm_il_address_index;


    //bpmPos_xy_Addr is 2 clock delayed
    reg [9:0] tmp_xy_Addr0, tmp_xy_Addr1;
    always @ (posedge clk) begin
        tmp_xy_Addr0 <= bpmPos_xy_Addr;
        tmp_xy_Addr1 <= tmp_xy_Addr0;
    end
    assign BmPosAddress = tmp_xy_Addr1;
	
    /*
    ************************************************
    *	BENDING MAGNET INTERLOCK
    *
    ************************************************
    */
    wire signed  [31:0] bpmPosDataWithOffset;
    wire signed [31:0] bm_limit_hi_set, bm_limit_low_set;
    wire [9:0] BM_ilCount;
    wire bm_bpm_enable;

    assign bm_limit_hi_set  = sr_bpm_ref_offset_xy_set;
    assign bm_limit_low_set = sr_bpm_ref_offset_xy_set * -1;

    assign bpmPosDataWithOffset = bpmPos_Xy_Data - sr_bpm_origin_offset_xy_set;

    assign bm_bpm_enable = ~FINAL_DUMP_OUT && xy_Mask_DataValid && (dcct_50mA && !AI_BM_RfTripCmd);
	
	
    /*
    *	bpmPos_Xy_Data              : position data (x[0:179],y[180:359])
    *	sr_bpm_origin_offset_xy_set : Origin Offset from database (x[0:179],y[180:359]
    *
    */
    bm_position_interlock_nm 	__BENDING_MAGNET_
    (
        .clk(clk),
        .reset( reset || GlobalLatchClearingReset ),	//Latch Clear by User Command
        //.ClearLatch( BM_LATCHED_OUT_RST ),
        .trig( Trig1Start ),	//for error reset every 10 kHz
        .clk_enable( bm_bpm_enable ),
        .bm_xy_Mask_DataValid(xy_Mask_DataValid),	//added 03/31/15
        .x( bpmPosDataWithOffset ),		//x,y closed orbit with offset
        //.y( 32'd100 ),
        .y( bpmPos_Xy_Data ),			// 03/25/15 for zero detection
        .posAddress(BmPosAddress),
        .x_h_limit(   bm_limit_hi_set ),			//5.0 mm default
        .x_low_limit( bm_limit_low_set ),			//3.0 mm default
        .y_h_limit( 32'd200 ),	//Not used
        .y_low_limit( 32'd0 ),	//Not used
        .avg_num( 16'h7fff ), 	//2/(N+1)*2^15 	0x7fff:1
        .ce_out(),
        .x_avg_out(),
        .y_avg_out(),
        .x_h_out(bm_high),	//high
        .x_l_out(bm_low),	//
        .y_h_out( bm_bpm_fault_detected ),
        .y_l_out(),
        .IL_SUM( bm_il_sum ),	//Un latched output
        .IL_LATCHED_OUT(BM_LATCHED_OUT),
        .IL_GlitchLatchedOut(IL_BM_GlitchLatchedOut),
        .err_cnt_o(BM_ilCount),
        .BM_IL_ADDRESS(bm_il_address_index),	//IL position address
        .bm_pos_limit( plb_w_reg[8] ),
        .err_flag( AI_BM_ErrorCountTripCmd ),
        //
        .xx_pos_out_cnt(bm_xx_pos_out_cnt),
        .yy_pos_out_cnt(bm_yy_pos_out_cnt)
    );

	
    //01/20/17
    //assign BM_IL_ADDRESS = ( plb_w_reg[15][13] == 1'b1 ) ?  data_addr[13:0] : bm_il_address_index;
    //assign IL_SUM        = ( plb_w_reg[15][13] == 1'b1 ) ? 	ai_dbl_err      : bm_il_sum;
    assign BM_IL_ADDRESS =  bm_il_address_index;
    assign IL_SUM        =  bm_il_sum;

	
    /*
    * Read RF-BPM interlock history data
    */
    //one clock delay
    reg il_tmp, il_we;
    always @( posedge clk ) begin
        if( GlobalLatchClearingReset == 1'b1 | reset == 1'b1) begin
            bm_iL_info <= 780'd0;
        end
        //else if ( IL_SUM == 1'b1) begin
        else if ( IL_SUM == 1'b1 | bm_bpm_fault_detected == 1'b1 ) begin	//03/31/15
            bm_iL_info[BM_IL_ADDRESS[9:0]] <= 1'b1;
        end
        else  bm_iL_info <= bm_iL_info;
    end
 
    posDpram_32x512  BM_dump_info (
        .clka (clk), 		// input clka
        .ena  (1'b1), 		// input ena
        .wea  (IL_SUM),		//if caused beam dump than stop update BRAM
        .addra(BM_IL_ADDRESS[8:0]), // input [8 : 0] addra
        .dina ({16'd0, 1'b0, BM_LATCHED_OUT, BM_IL_ADDRESS[13:0]}),
        //AI_BM_RfTripCmd
        //read by uBlaze
        .rstb (GlobalLatchClearingReset),
        .clkb (clk), 			// input clkb
        .enb  (ram_cs2[15]), 		// input enb
        .addrb(ram_addr2[8:0]), 	// input [8 : 0] addrb
        .doutb(bram_odate[5])
    );

    reg	dumpTrig;
    reg dumpT1Reg, dumpT1Reg_reg1, dumpT1Reg_reg2;
    always @ (posedge clk)
    begin
        dumpT1Reg <= FINAL_DUMP_OUT;
        dumpT1Reg_reg1 <= dumpT1Reg;
        dumpT1Reg_reg2 <= dumpT1Reg_reg1;
        dumpTrig <= (dumpT1Reg_reg1 & !dumpT1Reg_reg2);
    end
		
    //register map corrected at uBlaze    
    always @ (posedge clk ) begin
        if(GlobalLatchClearingReset == 1'b1) begin
            SysMonData[14] <= 32'd0;           //nsec
            SysMonData[15] <=  TS_time[63:32]; //sec
        end
        else if(dumpTrig == 1'b1) begin		//copy timestamp
            SysMonData[14] <= TS_time[31:0];	//nsec
            SysMonData[15] <= TS_time[63:32];	//sec
        end
    end
	
	
	
    ///////////////////////////////////////////////////////////////
    /*
        ?Inelastic X-ray Scattering (IXS)
        ?Hard X-ray Nanoprobe (HXN)
        ?Coherent Hard X-ray Scattering (CHX)
        ?Coherent Soft X-ray Scat & Pol (CSX1, CSX2)
        ?Sub-micron Res X-ray Spec (SRX)
        ?X-ray Powder Diffraction (XPD)

    *	LOGIC:
    *		rectangular (for AIE-ID-A)				: 20
    * 		diamond (for AIE-ID-B & AIE-BM)			: 10
    * 		rectangular + optimal offset (AIE-ID-C)	: 21
    * 		rectangular + small offset (AIE-ID-D)	: 22
    */

    // AIE enable/Disable control
    /*
    always @ (posedge clk ) begin
        if(GlobalLatchClearingReset == 1'b1) begin
                aie_USET_MASK_SETTING_REG <= 120'd0;
        end
        else if( Trig1Start == 1'b1 ) begin
                aie_USET_MASK_SETTING_REG[plb_w_reg[4][7:0]] <= plb_w_reg[4][8];
        end
    end
    */
	
    ////////////////////////////////////////////////
    ///// 5 kHz DspMatrixCalStartTrig //////////////

    always @ (posedge clk ) begin	//25 MHz clock
        if(reset)
            DspMatrixCalStartTrig_5kHz <= 1'b0;
        else if(DspMatrixCalStartTrig == 1'b1) begin
            DspMatrixCalStartTrig_5kHz <= ~DspMatrixCalStartTrig_5kHz ; 	// 5 kHz
        end
    end
		
    edge_detect (
        .async_sig(DspMatrixCalStartTrig_5kHz),
        .clk (clk),
        .rise(ai_id_trig1),
        .fall(ai_id_trig2)
    );
	
	
    aie_addr_gen AIE_ADDR_GEN (
        .clk(clk),
        .reset(reset),
        .trig( DspMatrixCalStartTrig ),
        .clk_enable(1'b1),
        .address_move(12'd`AIE_CAL_NCLK-5),
        .AddressStart(12'd0),
        .AddressEnd(12'd`AIE_CAL_NCLK), 		//30 clock
        .aie_mask (`AIE_USR_FULL_MASK_SETTING),	//MAX IDs
        .aie_mask2(aie_USET_MASK_SETTING),		//Input Masking ID number's
        .Data16b(),
        .aie_addr(aie_addr),
        .wr_one( AIeCalOutLatch ),
        .wr( AIeCalValid ),			//max 60 pulse, I asume max IDs are 60
        .mask(),
        .mask2(aie_USER_MASK_OUT)
    );

    IDBPM_LOOKUP _IDBPM_LOOKUP(
        .addr(aie_addr),
        .add_x1(IdBpmPos_base[9:0]),		//X1
        .add_y1(IdBpmPos_base[19:10]),		//Y1
        .add_x2(IdBpmPos_base[29:20]),		//X2
        .add_y2(IdBpmPos_base[39:30]),		//Y2
        .add_cells(IdBpmPos_base[49:40])
    );
	


    /*
    * AI_ID
    */
    active_intlock_process  IXS		//Inelastic X-ray Scattering
    (
        .clk(clk),
        .reset(reset ),
        .cnt_rst(fifo_rst),
        .trig(DspTrig),		//10/06/14 added
        .EN( AIeCalOutLatch ),
        .Clear( GlobalLatchClearingReset ),	//Reset all Latched status, every cycle start new calculation

        // if FINAL_DUMP_OUT generate immediately disable AI-ID processing
        //.BeamCurrentStatus( ~FINAL_DUMP_OUT && (dcct_2mA || dcct_0_2mA) && IDAI_RunStopStatusReg[aie_addr] ),	//02/08/2015
        .BeamCurrentStatus( ~FINAL_DUMP_OUT && IDAI_RunStopStatusReg[aie_addr] ),	//04/09/2019
        .aie_addr(aie_addr),
        .AIeCalValidIn(AIeCalValid),
        .logic( SetRamDoutB[`ADDR_LOGIC] ),				//Logic table for xy1 & xy2 & mm & mrad
        .mode(plb_w_reg[13][21:20]),
        .mux_dir( plb_w_reg[13][0] ),
        //ID bpm Position input, use [25:0]  26bit
`ifdef	__DATA_32BIT__
        .x1_nm_i( xy_pos_Reg[ IdBpmPos_base[9:0] ] ),		//SDI x1 position
        .y1_nm_i( xy_pos_Reg[ IdBpmPos_base[19:10] ] ),		//y1 position
        .x2_nm_i( xy_pos_Reg[ IdBpmPos_base[29:20] ] ),		//SDI x2 position
        .y2_nm_i( xy_pos_Reg[ IdBpmPos_base[39:30] ] ),		//y2 position
`else
        .x1_nm_i( xy_pos_Reg[ IdBpmPos_base[9:0] ][25:0] ),		//SDI x1 position
        .y1_nm_i( xy_pos_Reg[ IdBpmPos_base[19:10] ][25:0] ),		//y1 position
        .x2_nm_i( xy_pos_Reg[ IdBpmPos_base[29:20] ][25:0] ),		//SDI x2 position
        .y2_nm_i( xy_pos_Reg[ IdBpmPos_base[39:30] ][25:0] ),		//y2 position
`endif
        .Cell_status( xy_pos_Reg[ IdBpmPos_base[49:40] ] ),	//Cell status for decision Interlock Mode
        .AIOL_H1( SetRamDoutB[`ADDR_H1] ),
        .AIOL_V1( SetRamDoutB[`ADDR_V1] ),
        .AIOL_H2( SetRamDoutB[`ADDR_H2] ),
        .AIOL_V2( SetRamDoutB[`ADDR_V2]),
        .AIOL_H3( SetRamDoutB[`ADDR_H3]),
        .AIOL_V3( SetRamDoutB[`ADDR_V3]),
        .AIAL_H(  SetRamDoutB[`ADDR_ANGLE_H]),
        .AIAL_V(  SetRamDoutB[`ADDR_ANGLE_V]),
        // 3/31/2014 new added offset
        .OFFSET_H1( SetRamDoutB[`H1_CHAMBER_OFFSET]),
        .OFFSET_V1( SetRamDoutB[`V1_CHAMBER_OFFSET]),
        .OFFSET_H2( SetRamDoutB[`H2_CHAMBER_OFFSET]),
        .OFFSET_V2( SetRamDoutB[`V2_CHAMBER_OFFSET]),
        //
        .S_angle(SetRamDoutB[`ADDR_MRAD_S]),     	// S =  (S3-S1)/(S2-S1) FIX32_31
        .S_mm(SetRamDoutB[`ADDR_MM_S]),        		// S =  1/(S2-S1)       FIX32_31
        .mux_addr(  plb_w_reg[13][25:22] ),
        //result
        .mux_Data_o(mux_Data_o),		/* FIFO or NPI for data monitoring */
        .x_mm_cal(x_mm_cal),
        .y_mm_cal(y_mm_cal),
        .x_mrad_cal(x_mrad_cal),
        .y_mrad_cal(y_mrad_cal),
        .x_mrad_err(x_mrad_err),
        .y_mrad_err(y_mrad_err),
        .x_mm_err(x_mm_err),
        .y_mm_err(y_mm_err),
        .muxValid(muxValid),					/* Enable */
        .debug(AI_ID_ILOutStatus),
        .Il_latched_s(Il_latched_s),
        .AI_ID_RfDumpOut(aie_DumpStatus),		//output result
        .Aie_bm_o(Aie_bm_o),
        .Aie_A_o(Aie_A_o),
        .Aie_C_o(Aie_C_o),
        .Aie_D_o(Aie_D_o),
        .datavalid(aieOutDataValid),
        .il_LatchValid_o(aieIl_LatchValid_o),
        .dump_info_ram_en( ram_cs2[14] ),
        .dump_info_ram_addr( ram_addr2[8:0] ),
        .dump_info_ram_data( bram_odate[4] ),
        .Il_activated(Il_activated),
        .bpm_pos_status  (ID_BpmPosValueStatus),    //ID bpm x, y zero status ?
        .bpm_12_rx_status(bpm_id_12_rx_status),
        .out_of_limit_cnt(id_out_of_limit_cnt),

        .bm_pos_limit( plb_w_reg[8] )
    );
    assign plb_r_reg20 = id_out_of_limit_cnt;
	
	

	
    wire [31:0] pm_cnt_periode;
    assign pm_cnt_periode = {8'd0, plb_w_Reg12[23:8]};
    //assign aiPM_Trigger = plb_w_reg[13][31] || AI_ID_RfTripCmd || BM_DUMP || ID_DUMP || bm_il_sum || dcct_beam_loss;

    // 12/01/16
    //assign aiPM_Trigger = plb_w_reg[13][31] || AI_ID_RfTripCmd || BM_DUMP;

    //10/10/2017 : added fp_lemo_in[0]
    //assign aiPM_Trigger = plb_w_reg[13][31] || AI_ID_RfTripCmd || BM_DUMP || ID_DUMP || fp_lemo_in[0];
    
    // 01/20/17                 User                AI-ID           AI-BM      AI-ID single
    assign aiPM_Trigger = plb_w_reg[13][31] || AI_ID_RfTripCmd || BM_DUMP || ID_DUMP ;
    

    //Post-mortem timing
    pm_timing_gen PM_TIMING (
        .clk        (clk),
        .reset      (reset),
        .trig       (Trig1Start ), 	    //10 kHz trig
        .falt_trig  (aiPM_Trigger),
        .periode    (pm_cnt_periode ),       // max size every 2 sec, ddr start address is zero
        .usr_rst    (plb_w_reg[13][30] | evr_TRIGGER_6 ), 	//user reset
        //.npi_trig(npi_0_pm_reset),
        .npi_stop   (npi_stop),
        .npi_enable (pm_npi_enable),
        // 061915 added
        .RingCntRstOut         (npi_0_pm_reset ),
        .RingCntFaPmTrigCnt_out(FaPmTrigCnt_out )
    );
    assign Npi0_PM_RstOut = npi_0_pm_reset;
	
	
    assign evr_0_TsValid = npi_0_pm_reset | npi_stop;		//take timestamp data
    // timestamp read
    always @ (posedge clk ) begin
        if(npi_0_pm_reset == 1'b1) begin		//start timestamp
            SysMonData[10] <= evr_0_TsLatched[31:0];
            SysMonData[11] <= evr_0_TsLatched[63:32];
        end
        else if(npi_stop == 1'b1) begin	//stop timestamp
            SysMonData[12] <= evr_0_TsLatched[31:0];
            SysMonData[13] <= evr_0_TsLatched[63:32];	//sec
        end
    end
	
    //Current simulation
    //dcct_2mA, dcct_50mA;
    //xy_pos_Reg[100][0] and xy_pos_Reg[100][1] - Cell 3 Sdi Data for DCCT status reading
    /*
    Cell controller SDI:
        .fa_pos_x( { ExtDin_pin, plb_CCLinkData[15:0]} ), 			//Cell Controller Data 0
    .fa_pos_y( {16'd0, plb_CCLinkData[31:16]} ), 				//Cell Controller Data 1	 *
    */
    // plb_w_Reg12[31]  beam simulation mode
    // 1: from PV
    // 0: from PLC

    // NEW DCCT CONTROLLER PIN MAP [26.. ]
    assign dcct_0_2mA     = (plb_w_Reg12[31] == 1'b1) ?  plb_w_Reg12[29] : ~xy_pos_Reg[`POS_ADDR_BEAM_CURRENT][25];	// 0.2mA from PLC
    assign dcct_2mA       = (plb_w_Reg12[31] == 1'b1) ?  plb_w_Reg12[30] : ~xy_pos_Reg[`POS_ADDR_BEAM_CURRENT][26];
    assign dcct_50mA      = (plb_w_Reg12[31] == 1'b1) ?  plb_w_Reg12[29] : ~xy_pos_Reg[`POS_ADDR_BEAM_CURRENT][27];
    assign dcct_500mA     =	~xy_pos_Reg[`POS_ADDR_BEAM_CURRENT][28];
    assign dcct_plc_hb    = ~xy_pos_Reg[`POS_ADDR_BEAM_CURRENT][29];	//Beam Current PLC HB
    assign dcct_sys_fault = ~xy_pos_Reg[`POS_ADDR_BEAM_CURRENT][30];
    assign dcct_beam_loss = ~xy_pos_Reg[`POS_ADDR_BEAM_CURRENT][31];


        //AI-ID dual reset trigger delay
    always @ (posedge clk)
    begin
        aieIl_Latch_dly[0] <= aieIl_LatchValid_o;
        aieIl_Latch_dly[1] <= aieIl_Latch_dly[0];
        aieIl_Latch_dly[2] <= aieIl_Latch_dly[1];
    end
    assign ai_id1_rst_acq_trig = aieIl_Latch_dly[2];
	
	
    /*
    NPI Control
        RESET: npi_0_pm_reset
        WR:    pm_npi_wr
        DATA:  mux_Data_o
    */


    /*                    _
    *	TRIG           __| |_____________________________________________________________________
    *                              __________
    *	AIeCalValid    ____________|          |__________________________________________________
    *
    *                                       _
    *  AIeCalOutLatch _____________________| |__________________________________________________
    *
    *	Gap status 0 is open
    *
    */

    // Gap and Photon source
    // gap_status_info                0 : GAP CLOSED,          1 : OPEN
    // PhotonShutters_Status_info	  0 : CLOSED               1 : PHOTON SHTTTER OPEN,
    // BM_PhotonShutters_Status  	  0 : CLOSED               1 : PHOTON SHTTTER OPEN,


    assign  gap_status_info               = (plb_w_reg[13][1] == 1'b0 ) ?  Gap_Status01               : 64'h0000000000000000;	//CLOSE
    // ID PS OPEN = '1'
    assign  PhotonShutters_Status_info    = (plb_w_reg[13][2] == 1'b0 ) ?  PhotonShutters_Status01    : 64'h00000000FFFFFFFF;	//OPEN
    assign  BM_PhotonShutters_Status_info = (plb_w_reg[13][3] == 1'b0 ) ?  BM_PhotonShutters_Status01 : 64'h00000000FFFFFFFF;	//OPEN


    /////////////////////////
    // AI-ID processing
    /////////////////////////
    always @ (posedge clk ) begin
        if(reset == 1'b1 || GlobalLatchClearingReset == 1'b1) begin	//clearing all latched Data
            ID_DUMP                  <= 1'b0;
            IDAI_BeamDumpControlReg  <= `MAX_IDEVICE'd0;
            IDAI_RunStopStatusReg    <= `MAX_IDEVICE'd0;
            ID_BpmFailStatus         <= 1'b0;
            ID_BpmFailStatusLatch    <= 1'b0;
            AI_ID_STATUS_OUTreg_cnt  <= 16'd0;
            //
            CantedMagnets_Status_Reg <= `MAX_IDEVICE'hffffffffffffffff;
            CM_SdiRawBitStatus_Reg   <= `MAX_IDEVICE'hffffffffffffffff;
            CM_Status2      <= 1'b0;
            CM_StatusTmpReg <= 1'b0;
            CM_Status_reg   <= 1'b0;
            CM_Status       <= 1'b0;
            CellPLC_IdBeamDumpOutReg <= 1'b0;
            CellPLC_IdBeamDump_Status_Latch_Reg <= 	31'd0;
            id_glitch_count <= 16'd0;
        end
        else if(aieIl_LatchValid_o == 1'b1) begin	// FOR AI-ID LOGIC ID Gap status   : COLSED = '1' , OPEN '0'
            /* 10/08/14
                    if (([ID Gap=Closed] AND [I_SR>I_SCID])	OR	[IDPS=Open] AND [I_SR>I_SCID])
                    then (AI-ID=ON)
                    else (AI-ID=OFF)
            */

            // 11/06/14
            //                                       2 mA        GAP CLOSED                        ID_PS OPEN                                USER ENABLE/DISABLE
            //IDAI_RunStopStatusReg[aie_addr]   <=  UserMaskControl_0[aie_addr] && ( (dcct_2mA && !gap_status_info[aie_addr]) || ( (dcct_2mA || dcct_0_2mA) && PhotonShutters_Status_info[aie_addr] ) ) ;
            // 04/09/19
            IDAI_RunStopStatusReg[aie_addr]   <=  UserMaskControl_0[aie_addr] && ( (dcct_2mA && !gap_status_info[aie_addr]) || (dcct_0_2mA && PhotonShutters_Status_info[aie_addr]) );

            IDAI_BeamDumpControlReg[aie_addr] <= 	//aie_USER_MASK_OUT &&                  //Manual Enable/Disable
                                            IDAI_RunStopStatusReg[aie_addr] &&	   			//BP_PS CLOSED?
                                            ( aie_DumpStatus[aie_addr] || 					//ID sngle/offset cal result
                                            ( !BM_PhotonShutters_Status_info[aie_addr]) ||
                                            ( CM_Status2 && plb_Reg10[12]) ||				//Canting Magnet
                                            ( ID_BpmFailStatusLatch && plb_w_reg[13][5] ) 	//BPM Status
                                            );

            //Single cycle error detection mode
            if( IDAI_BeamDumpControlReg[aie_addr] == 1'b1 && IDAI_RunStopStatusReg[aie_addr] == 1'b1) begin	//04/09/19 added IDAI_RunStopStatusReg
                ID_DUMP <= 1'b1;
                id_glitch_detected <= 1'b1;
                id_glitch_count <= id_glitch_count+1;
            end
            else begin
                ID_DUMP <= ID_DUMP;
                id_glitch_detected <= 1'b0;
            end


            //11/22/2015
            if(	(!CellPLC_IdBeamDump_Status[aie_addr] && UserMaskControl_0[aie_addr]  ) == 1'b1 ) begin
                CellPLC_IdBeamDumpOutReg <= 1'b1 & plb_Reg10[13];	//Dump condition
                CellPLC_IdBeamDump_Status_Latch_Reg <= CellPLC_IdBeamDump_Status;
            end
            else begin
                CellPLC_IdBeamDumpOutReg <= CellPLC_IdBeamDumpOutReg;
                CellPLC_IdBeamDump_Status_Latch_Reg	<= CellPLC_IdBeamDump_Status_Latch_Reg;
            end

            // ID bpm fault status monitoring
            if( (ID_BpmPosValueStatus[31:0] & UserMaskControl_0 ) != 32'd0) begin
                ID_BpmFailStatus      <= 1'b1;
                ID_BpmFailStatusLatch <= 1'b1;
            end
            else begin
                ID_BpmFailStatus      <= 1'b0;
                ID_BpmFailStatusLatch <= ID_BpmFailStatusLatch;
            end

            // count AI-ID single errors
            if(	AI_ID_STATUS_OUT == 1'b1 ) begin
                AI_ID_STATUS_OUTreg_cnt <= AI_ID_STATUS_OUTreg_cnt + 1;
            end
            else begin
                AI_ID_STATUS_OUTreg_cnt <= AI_ID_STATUS_OUTreg_cnt;
            end


            //07/07/16
            // Canting status reading
            // ID-19 : CantedMagnets_Status[18] and CantedMagnets_Status[19]
            CM_Status <= ((!CantedMagnets_Status[1]) && UserMaskControl_0[1]  && UserMaskControl_0[2]) ||
                                ((!CantedMagnets_Status[11]) && UserMaskControl_0[11] && UserMaskControl_0[12]) ||
                                ((!CantedMagnets_Status[13]) && UserMaskControl_0[13] && UserMaskControl_0[14]) ||
                                ((!CantedMagnets_Status[15]) && UserMaskControl_0[15] && UserMaskControl_0[16]) ||
                                ((!CantedMagnets_Status[18]) && UserMaskControl_0[18] && UserMaskControl_0[19]) ;



            //Reset only canting magnet
            if(plb_w_reg[13][27] == 1'b1) begin
                CantedMagnets_Status_Reg <= 64'hffffffffffffffff;
                CM_SdiRawBitStatus_Reg   <= 64'hffffffffffffffff;
                CM_Status_reg            <= 1'b0;
            end
            else if(CM_Status == 1'b1 && FINAL_DUMP_OUT == 1'b0 ) begin
                if( CM_Status_reg == 1'b0) begin	//if CM_Status_reg is ZERO ?
                    CantedMagnets_Status_Reg <= CantedMagnets_Status;	// keep latching
                    CM_SdiRawBitStatus_Reg   <= CM_SdiRawBitStatus;
                    CM_Status_reg <= 1'b1;	//08/05/16 for status monitoring
                end

                //delay
                CM_StatusTmpReg <= 1'b1;
                CM_Status2    	<= CM_StatusTmpReg;
            end
            else begin
                CantedMagnets_Status_Reg <= CantedMagnets_Status_Reg;
                CM_SdiRawBitStatus_Reg   <= CM_SdiRawBitStatus_Reg;
                CM_Status_reg <= CM_Status_reg;		//status indicate to sysStatus[28]
                //
                CM_StatusTmpReg <= 1'b0;
                CM_Status2    	<= CM_StatusTmpReg;
            end
            ///

        end

    end

	
    /*
     *	12/04/2015 new added for continuce error detection.
     */
    always @ (posedge clk ) begin
        ai_id_dump2_rstReg[0] <= Trig1Start;
        ai_id_dump2_rstReg[1] <= ai_id_dump2_rstReg[0];
    end;

    assign ai_id_dump_rst = ai_id_dump2_rstReg[1];

    always @ (posedge clk ) begin
        if(GlobalLatchClearingReset == 1'b1) begin	//clear
            ai_id_err_cnt             <= 7'd0;
            ai_id_multiple_err_status <= 1'b0;
            ai_id_dump2               <= 1'b0;
            BM_PS_closed_status       <= 0;
            aie_addrBmPsReg           <= 7'd0;
        end
        else if(AIeCalOutLatch == 1'b1) begin
            // 07/07/16 added "CellPLC_IdBeamDumpOutReg" signal
            if(  IDAI_RunStopStatusReg[aie_addr] &&	( AI_ID_STATUS_OUT || (!BM_PhotonShutters_Status_info[aie_addr]) || ( CM_Status2 && plb_Reg10[12] ) || CellPLC_IdBeamDumpOutReg )   )
                begin
                    /// 07/07/06 added
                    if(BM_PhotonShutters_Status_info[aie_addr] == 1'b0) begin
                        BM_PS_closed_status <= 1'b1;
                        aie_addrBmPsReg <= aie_addr;
                    end
                    else begin
                        BM_PS_closed_status <= BM_PS_closed_status;
                    end
                    ///
                    ai_id_err_cnt <= ai_id_err_cnt + 1;
                    ai_id_dump2   <= 1'b1;	//Glitch
                end
            else begin
                ai_id_err_cnt <= ai_id_err_cnt;
                //
                ai_id_dump2 <= ai_id_dump2;
            end
        end
        else if (ai_id_dump_rst == 1'b1) begin	//clear every trigger
            if(ai_id_err_cnt > 7'd0) begin
                ai_id_multiple_err_status <= 1'b1;
            end
            else begin
                ai_id_multiple_err_status <= ai_id_multiple_err_status;
            end
            //clear every cycle
            ai_id_err_cnt <= 7'd0;
            ai_id_dump2   <= 1'b0;
        end
    end
	
	
	
	
/*	
const struct PosId_Cc tPosId_Cc[31] =
{
	{  0,  0,  0,   0,  0,   0,  0,   0,  0 },
	{  1,  25,  24,   17,  16,   15,  14,   13,  12 },
	{  2,  51,  50,   43,  42,   41,  40,   39,  38 },
	{  3,  77,  76,   69,  68,   67,  66,   65,  64 },
	{  4, 103, 102,   95,  94,   93,  92,   91,  90 },
	{  5, 129, 128,  121, 120,  119, 118,  117, 116 },
	{  6, 155, 154,  147, 146,  145, 144,  143, 142 },
	{  7, 181, 180,  173, 172,  171, 170,  169, 168 },
	{  8, 207, 206,  199, 198,  197, 196,  195, 194 },
	{  9, 233, 232,  225, 224,  223, 222,  221, 220 },
	{ 10, 259, 258,  251, 250,  249, 248,  247, 246 },
	{ 11, 285, 284,  277, 276,  275, 274,  273, 272 },
	{ 12, 311, 310,  303, 302,  301, 300,  299, 298 },
	{ 13, 337, 336,  329, 328,  327, 326,  325, 324 },
	{ 14, 363, 362,  355, 354,  353, 352,  351, 350 },
	{ 15, 389, 388,  381, 380,  379, 378,  377, 376 },
	{ 16, 415, 414,  407, 406,  405, 404,  403, 402 },
	{ 17, 441, 440,  433, 432,  431, 430,  429, 428 },
	{ 18, 467, 466,  459, 458,  457, 456,  455, 454 },
	{ 19, 493, 492,  485, 484,  483, 482,  481, 480 },
	{ 20, 519, 518,  511, 510,  509, 508,  507, 506 },
	{ 21, 545, 544,  537, 536,  535, 534,  533, 532 },
	{ 22, 571, 570,  563, 562,  561, 560,  559, 558 },
	{ 23, 597, 596,  589, 588,  587, 586,  585, 584 },
	{ 24, 623, 622,  615, 614,  613, 612,  611, 610 },
	{ 25, 649, 648,  641, 640,  639, 638,  637, 636 },
	{ 26, 675, 674,  667, 666,  665, 664,  663, 662 },
	{ 27, 701, 700,  693, 692,  691, 690,  689, 688 },
	{ 28, 727, 726,  719, 718,  717, 716,  715, 714 },
	{ 29, 753, 752,  745, 744,  743, 742,  741, 740 },
	{ 30, 779, 778,  771, 770,  769, 768,  767, 766 },
};
*/	


//10/03/14   added
`define	C1_CDATA_BASE			24
`define	C2_CDATA_BASE			50
`define	C3_CDATA_BASE			76
`define	C4_CDATA_BASE			102
`define	C5_CDATA_BASE			128
`define	C6_CDATA_BASE			154
`define	C7_CDATA_BASE			180
`define	C8_CDATA_BASE			206
`define	C9_CDATA_BASE			232
`define	C10_CDATA_BASE			258
`define	C11_CDATA_BASE			284
`define	C12_CDATA_BASE			310
`define	C13_CDATA_BASE			336
`define	C14_CDATA_BASE			362
`define	C15_CDATA_BASE			388
`define	C16_CDATA_BASE			414
`define	C17_CDATA_BASE			440
`define	C18_CDATA_BASE			466
`define	C19_CDATA_BASE			492
`define	C20_CDATA_BASE			518
`define	C21_CDATA_BASE			544
`define	C22_CDATA_BASE			570
`define	C23_CDATA_BASE			596
`define	C24_CDATA_BASE			622
`define	C25_CDATA_BASE			648
`define	C26_CDATA_BASE			674
`define	C27_CDATA_BASE			700
`define	C28_CDATA_BASE			726
`define	C29_CDATA_BASE			752
`define	C30_CDATA_BASE			778




    //Gap status, sdi cc address 0
`define	IN_OFF		16
    //Gap status, sdi cc address 0
    always @ (posedge clk ) begin
        if(reset == 1'b1 || GlobalLatchClearingReset == 1'b1) begin	//clear
            Gap_Status0               <= `MAX_IDEVICE'hFFFFFFFFFFFFFFFF;
            PhotonShutters_Status0    <= `MAX_IDEVICE'hFFFFFFFFFFFFFFFF;
            BM_PhotonShutters_Status0 <= `MAX_IDEVICE'hFFFFFFFFFFFFFFFF;
            CantedMagnets_Status      <= `MAX_IDEVICE'hFFFFFFFFFFFFFFFF;
            //CM_SdiRawBitStatus       <= `MAX_IDEVICE'hFFFFFFFFFFFFFFFF;
            CM_SdiRawBitStatus_InReg  <= `MAX_IDEVICE'hFFFFFFFFFFFFFFFF;
            //CellPLC_IdBeamDump_Status <= 32'h00000000;
            CellPLC_IdBeamDump_Status <= 32'hFFFFFFFF;	// PLC 1: is normal 0: is fault
        end
        else if(AIeCalOutLatch == 1'b1) begin
            //
            // GAP STATUS
            //		DIN bit [1:0]  for ID Gap status
            //		Cell 3  DIN bit [5:4]  for ID Gap status
            //			Cell 3 Bit [3:0] assigned for BEAM CURRENT STATUS
            //
            //	    CC digital In SDI DATA         USER DATA
            //     ______________________________________________
            //     [ 31............... 16 | 15................0 |
            //     ---------------------------------------------
            //
            //		1 : OPEN, 0: CLOSED    from SDI Link
            //
            Gap_Status0[0]  <= xy_pos_Reg[`C3_CDATA_BASE][`IN_OFF+0];
            Gap_Status0[1]  <= xy_pos_Reg[`C5_CDATA_BASE][`IN_OFF+0];	//#5 UPSTREAM
            Gap_Status0[2]  <= xy_pos_Reg[`C5_CDATA_BASE][`IN_OFF+0];	//#5 DOWNSTREAM
            Gap_Status0[3]  <= xy_pos_Reg[`C10_CDATA_BASE][`IN_OFF+0];
            Gap_Status0[4]  <= xy_pos_Reg[`C11_CDATA_BASE][`IN_OFF+0];
            Gap_Status0[5]  <= xy_pos_Reg[`C23_CDATA_BASE][`IN_OFF+0];
            Gap_Status0[6]  <= xy_pos_Reg[`C23_CDATA_BASE][`IN_OFF+0];	//#23 DOWNSTREAM
            Gap_Status0[7]  <= xy_pos_Reg[`C28_CDATA_BASE][`IN_OFF+0];
            Gap_Status0[8]  <= xy_pos_Reg[`C8_CDATA_BASE][`IN_OFF+0];
            Gap_Status0[9]  <= xy_pos_Reg[`C18_CDATA_BASE][`IN_OFF+0];
            //
            Gap_Status0[10] <= xy_pos_Reg[`C16_CDATA_BASE][`IN_OFF+0];
            Gap_Status0[11] <= xy_pos_Reg[`C17_CDATA_BASE][`IN_OFF+0];
            Gap_Status0[12] <= xy_pos_Reg[`C17_CDATA_BASE][`IN_OFF+0];
            //Next beamline
            Gap_Status0[13] <= xy_pos_Reg[`C4_CDATA_BASE][`IN_OFF+0];
            Gap_Status0[14] <= xy_pos_Reg[`C4_CDATA_BASE][`IN_OFF+0];
            // ID12
            Gap_Status0[15] <= xy_pos_Reg[`C12_CDATA_BASE][`IN_OFF+0];
            Gap_Status0[16] <= xy_pos_Reg[`C12_CDATA_BASE][`IN_OFF+0];
            Gap_Status0[17] <= xy_pos_Reg[`C21_CDATA_BASE][`IN_OFF+0];
            // ID19
            Gap_Status0[18] <= xy_pos_Reg[`C19_CDATA_BASE][`IN_OFF+0];
            Gap_Status0[19] <= xy_pos_Reg[`C19_CDATA_BASE][`IN_OFF+0];
            // ID 2
            Gap_Status0[20] <= xy_pos_Reg[`C2_CDATA_BASE][`IN_OFF+0];
            // ID 7 up/down
            Gap_Status0[21] <= xy_pos_Reg[`C7_CDATA_BASE][`IN_OFF+0];
            Gap_Status0[22] <= xy_pos_Reg[`C7_CDATA_BASE][`IN_OFF+0];
            //
            Gap_Status0[23] <= xy_pos_Reg[`C27_CDATA_BASE][`IN_OFF+0];  //09/03/22
            Gap_Status0[24] <= xy_pos_Reg[`C20_CDATA_BASE][`IN_OFF+0];  //09/03/22
            //
            Gap_Status0[25] <= 1'b1;        //674][`IN_OFF+0];   //1 :Open, 0: Close
            Gap_Status0[26] <= 1'b1;        //700][`IN_OFF+0];
            Gap_Status0[27] <= 1'b1;        //726][`IN_OFF+0];
            Gap_Status0[28] <= 1'b1;        //752][`IN_OFF+0];
            Gap_Status0[29] <= 1'b1;        //778][`IN_OFF+0];	//#30
            Gap_Status0[30] <= 1'b1;        //778][`IN_OFF+0];	//#31
            Gap_Status0[31] <= 1'b1;
		 
		 		 
            //
            //ID Front End First Photon Shutter
            //		DIN bit [3:2]  for Photon Shutter status
            //	Cell 3  DIN bit [7:6]  for Photon Shutter status
            PhotonShutters_Status0[0]  <= xy_pos_Reg[`C3_CDATA_BASE][`IN_OFF+2];
            PhotonShutters_Status0[1]  <= xy_pos_Reg[`C5_CDATA_BASE][`IN_OFF+2];		//#5 UPSTREAM
            PhotonShutters_Status0[2]  <= xy_pos_Reg[`C5_CDATA_BASE][`IN_OFF+2];
            PhotonShutters_Status0[3]  <= xy_pos_Reg[`C10_CDATA_BASE][`IN_OFF+2];
            PhotonShutters_Status0[4]  <= xy_pos_Reg[`C11_CDATA_BASE][`IN_OFF+2];
            PhotonShutters_Status0[5]  <= xy_pos_Reg[`C23_CDATA_BASE][`IN_OFF+2];
            PhotonShutters_Status0[6]  <= xy_pos_Reg[`C23_CDATA_BASE][`IN_OFF+2];		//#23 DOWNSTREAM
            PhotonShutters_Status0[7]  <= xy_pos_Reg[`C28_CDATA_BASE][`IN_OFF+2];
            PhotonShutters_Status0[8]  <= xy_pos_Reg[`C8_CDATA_BASE][`IN_OFF+2];
            PhotonShutters_Status0[9]  <= xy_pos_Reg[`C18_CDATA_BASE][`IN_OFF+2];
            //
            PhotonShutters_Status0[10] <= xy_pos_Reg[`C16_CDATA_BASE][`IN_OFF+2];
            PhotonShutters_Status0[11] <= xy_pos_Reg[`C17_CDATA_BASE][`IN_OFF+2];
            PhotonShutters_Status0[12] <= xy_pos_Reg[`C17_CDATA_BASE][`IN_OFF+2];
            // NEXT beamline
            PhotonShutters_Status0[13] <= xy_pos_Reg[`C4_CDATA_BASE][`IN_OFF+2];
            PhotonShutters_Status0[14] <= xy_pos_Reg[`C4_CDATA_BASE][`IN_OFF+2];
            //
            PhotonShutters_Status0[15] <= xy_pos_Reg[`C12_CDATA_BASE][`IN_OFF+2];
            PhotonShutters_Status0[16] <= xy_pos_Reg[`C12_CDATA_BASE][`IN_OFF+2];
            PhotonShutters_Status0[17] <= xy_pos_Reg[`C21_CDATA_BASE][`IN_OFF+2];
            //
            PhotonShutters_Status0[18] <= xy_pos_Reg[`C19_CDATA_BASE][`IN_OFF+2];
            PhotonShutters_Status0[19] <= xy_pos_Reg[`C19_CDATA_BASE][`IN_OFF+2];
            //
            PhotonShutters_Status0[20] <= xy_pos_Reg[`C2_CDATA_BASE][`IN_OFF+2];
            // ID 7
            PhotonShutters_Status0[21] <= xy_pos_Reg[`C7_CDATA_BASE][`IN_OFF+2];
            PhotonShutters_Status0[22] <= xy_pos_Reg[`C7_CDATA_BASE][`IN_OFF+2];
            //9/3/2022
            PhotonShutters_Status0[23] <= xy_pos_Reg[`C27_CDATA_BASE][`IN_OFF+2];
            PhotonShutters_Status0[24] <= xy_pos_Reg[`C20_CDATA_BASE][`IN_OFF+2];
            //
            PhotonShutters_Status0[25] <= 1'b0;        //674][`IN_OFF+2];
            PhotonShutters_Status0[26] <= 1'b0;        //700][`IN_OFF+2];
            PhotonShutters_Status0[27] <= 1'b0;        //726][`IN_OFF+2];
            PhotonShutters_Status0[28] <= 1'b0;        //752][`IN_OFF+2];
            PhotonShutters_Status0[29] <= 1'b0;        //xy_pos_Reg[778][`IN_OFF+2];	//#30
            PhotonShutters_Status0[30] <= 1'b0;        //
            PhotonShutters_Status0[31] <= 1'b0;        //
		 
            //
            // Bending Photon SHUTTER   08/25/14 added
            BM_PhotonShutters_Status0[0]  <= xy_pos_Reg[`C3_CDATA_BASE][`IN_OFF+1];
            BM_PhotonShutters_Status0[1]  <= xy_pos_Reg[`C5_CDATA_BASE][`IN_OFF+1];		//#5 UPSTREAM
            BM_PhotonShutters_Status0[2]  <= xy_pos_Reg[`C5_CDATA_BASE][`IN_OFF+1];
            BM_PhotonShutters_Status0[3]  <= xy_pos_Reg[`C10_CDATA_BASE][`IN_OFF+1];
            BM_PhotonShutters_Status0[4]  <= xy_pos_Reg[`C11_CDATA_BASE][`IN_OFF+1];
            BM_PhotonShutters_Status0[5]  <= xy_pos_Reg[`C23_CDATA_BASE][`IN_OFF+1];
            BM_PhotonShutters_Status0[6]  <= xy_pos_Reg[`C23_CDATA_BASE][`IN_OFF+1];		//#23 DOWNSTREAM
            BM_PhotonShutters_Status0[7]  <= xy_pos_Reg[`C28_CDATA_BASE][`IN_OFF+1];
            BM_PhotonShutters_Status0[8]  <= xy_pos_Reg[`C8_CDATA_BASE][`IN_OFF+1];
            BM_PhotonShutters_Status0[9]  <= xy_pos_Reg[`C18_CDATA_BASE][`IN_OFF+1];
            //
            BM_PhotonShutters_Status0[10] <= xy_pos_Reg[`C16_CDATA_BASE][`IN_OFF+1];
            BM_PhotonShutters_Status0[11] <= xy_pos_Reg[`C17_CDATA_BASE][`IN_OFF+1];
            BM_PhotonShutters_Status0[12] <= xy_pos_Reg[`C17_CDATA_BASE][`IN_OFF+1];
            // NEXT beamline
            BM_PhotonShutters_Status0[13] <= xy_pos_Reg[`C4_CDATA_BASE][`IN_OFF+1];
            BM_PhotonShutters_Status0[14] <= xy_pos_Reg[`C4_CDATA_BASE][`IN_OFF+1];
            // ID 12
            BM_PhotonShutters_Status0[15] <= xy_pos_Reg[`C12_CDATA_BASE][`IN_OFF+1];
            BM_PhotonShutters_Status0[16] <= xy_pos_Reg[`C12_CDATA_BASE][`IN_OFF+1];
            BM_PhotonShutters_Status0[17] <= xy_pos_Reg[`C21_CDATA_BASE][`IN_OFF+1];
            //
            BM_PhotonShutters_Status0[18] <= xy_pos_Reg[`C19_CDATA_BASE][`IN_OFF+1];
            BM_PhotonShutters_Status0[19] <= xy_pos_Reg[`C19_CDATA_BASE][`IN_OFF+1];
            BM_PhotonShutters_Status0[20] <= xy_pos_Reg[`C2_CDATA_BASE][`IN_OFF+1];
            //
            BM_PhotonShutters_Status0[21] <= xy_pos_Reg[`C7_CDATA_BASE][`IN_OFF+1];
            BM_PhotonShutters_Status0[22] <= xy_pos_Reg[`C7_CDATA_BASE][`IN_OFF+1];
            //9/3/2022
            BM_PhotonShutters_Status0[23] <= xy_pos_Reg[`C27_CDATA_BASE][`IN_OFF+1];
            BM_PhotonShutters_Status0[24] <= xy_pos_Reg[`C20_CDATA_BASE][`IN_OFF+1];
            //
            BM_PhotonShutters_Status0[25] <= 1'b0;        //[674][`IN_OFF+1];
            BM_PhotonShutters_Status0[26] <= 1'b0;        //[700][`IN_OFF+1];
            BM_PhotonShutters_Status0[27] <= 1'b0;        //[726][`IN_OFF+1];
            BM_PhotonShutters_Status0[28] <= 1'b0;        //[752][`IN_OFF+1];
            BM_PhotonShutters_Status0[29] <= 1'b0;        //[778][`IN_OFF+1];	//#30
            BM_PhotonShutters_Status0[30] <= 1'b0;
            BM_PhotonShutters_Status0[31] <= 1'b0;
		 
            //
            // FAST Valve Beam Dump added 5/20/2014
            //
            CellPLC_IdBeamDump_StatusReg[0]  <= xy_pos_Reg[`C3_CDATA_BASE][`IN_OFF+5]  && xy_pos_Reg[`C3_CDATA_BASE][`IN_OFF+4];		//#03
            CellPLC_IdBeamDump_StatusReg[1]  <= xy_pos_Reg[`C5_CDATA_BASE][`IN_OFF+5]  && xy_pos_Reg[`C5_CDATA_BASE][`IN_OFF+4];		//#05
            CellPLC_IdBeamDump_StatusReg[2]  <= xy_pos_Reg[`C5_CDATA_BASE][`IN_OFF+5]  && xy_pos_Reg[`C5_CDATA_BASE][`IN_OFF+4];		//#05
            CellPLC_IdBeamDump_StatusReg[3]  <= xy_pos_Reg[`C10_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C10_CDATA_BASE][`IN_OFF+4];		//#10
            CellPLC_IdBeamDump_StatusReg[4]  <= xy_pos_Reg[`C11_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C11_CDATA_BASE][`IN_OFF+4];
            CellPLC_IdBeamDump_StatusReg[5]  <= xy_pos_Reg[`C23_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C23_CDATA_BASE][`IN_OFF+4];		//#23 UP-STREAM
            CellPLC_IdBeamDump_StatusReg[6]  <= xy_pos_Reg[`C23_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C23_CDATA_BASE][`IN_OFF+4];		//#23 DOWNSTREAM
            CellPLC_IdBeamDump_StatusReg[7]  <= xy_pos_Reg[`C28_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C28_CDATA_BASE][`IN_OFF+4];
            CellPLC_IdBeamDump_StatusReg[8]  <= xy_pos_Reg[`C8_CDATA_BASE][`IN_OFF+5]  && xy_pos_Reg[`C8_CDATA_BASE][`IN_OFF+4];
            CellPLC_IdBeamDump_StatusReg[9]  <= xy_pos_Reg[`C18_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C18_CDATA_BASE][`IN_OFF+4];
            // ABIX beamline C16 and C17
            CellPLC_IdBeamDump_StatusReg[10] <= xy_pos_Reg[`C16_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C16_CDATA_BASE][`IN_OFF+4];
            CellPLC_IdBeamDump_StatusReg[11] <= xy_pos_Reg[`C17_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C17_CDATA_BASE][`IN_OFF+4];
            CellPLC_IdBeamDump_StatusReg[12] <= xy_pos_Reg[`C17_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C17_CDATA_BASE][`IN_OFF+4];
            // NEXT beamline
            CellPLC_IdBeamDump_StatusReg[13] <= xy_pos_Reg[`C4_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C4_CDATA_BASE][`IN_OFF+4];
            CellPLC_IdBeamDump_StatusReg[14] <= xy_pos_Reg[`C4_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C4_CDATA_BASE][`IN_OFF+4];
            // ID 12
            CellPLC_IdBeamDump_StatusReg[15] <= xy_pos_Reg[`C12_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C12_CDATA_BASE][`IN_OFF+4];
            CellPLC_IdBeamDump_StatusReg[16] <= xy_pos_Reg[`C12_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C12_CDATA_BASE][`IN_OFF+4];
            CellPLC_IdBeamDump_StatusReg[17] <= xy_pos_Reg[`C21_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C21_CDATA_BASE][`IN_OFF+4];
            //
            CellPLC_IdBeamDump_StatusReg[18] <= xy_pos_Reg[`C19_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C19_CDATA_BASE][`IN_OFF+4];
            CellPLC_IdBeamDump_StatusReg[19] <= xy_pos_Reg[`C19_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C19_CDATA_BASE][`IN_OFF+4];
            //
            CellPLC_IdBeamDump_StatusReg[20] <= xy_pos_Reg[`C2_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C2_CDATA_BASE][`IN_OFF+4];
            // ID07
            CellPLC_IdBeamDump_StatusReg[21] <= xy_pos_Reg[`C7_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C2_CDATA_BASE][`IN_OFF+4];
            CellPLC_IdBeamDump_StatusReg[22] <= xy_pos_Reg[`C7_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C2_CDATA_BASE][`IN_OFF+4];
            //9/3/2022
            CellPLC_IdBeamDump_StatusReg[23] <= xy_pos_Reg[`C27_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C2_CDATA_BASE][`IN_OFF+4];
            CellPLC_IdBeamDump_StatusReg[24] <= xy_pos_Reg[`C20_CDATA_BASE][`IN_OFF+5] && xy_pos_Reg[`C2_CDATA_BASE][`IN_OFF+4];
            //
            CellPLC_IdBeamDump_StatusReg[25] <= 1'b0;        //xy_pos_Reg[674][`IN_OFF+5] && xy_pos_Reg[674][`IN_OFF+4];
            CellPLC_IdBeamDump_StatusReg[26] <= 1'b0;        //xy_pos_Reg[700][`IN_OFF+5] && xy_pos_Reg[700][`IN_OFF+4];
            CellPLC_IdBeamDump_StatusReg[27] <= 1'b0;        //xy_pos_Reg[726][`IN_OFF+5] && xy_pos_Reg[726][`IN_OFF+4];
            CellPLC_IdBeamDump_StatusReg[28] <= 1'b0;        //xy_pos_Reg[752][`IN_OFF+5] && xy_pos_Reg[752][`IN_OFF+4];
            CellPLC_IdBeamDump_StatusReg[29] <= 1'b0;        //xy_pos_Reg[778][`IN_OFF+5] && xy_pos_Reg[778][`IN_OFF+4];	//#30
            CellPLC_IdBeamDump_StatusReg[30] <= 1'b0;
            CellPLC_IdBeamDump_StatusReg[31] <= 1'b0;
		 
		 
		 
            //
            // NOT SUPPORTED by PLC
            // Cell 9 : IN 7, 8  for BM DUMP (added 8/9/16)
            //
            CellPLC_BmBeamDump_Status[0]  <= 1'b1;	//xy_pos_Reg[24][`IN_OFF+7] && xy_pos_Reg[24][`IN_OFF+6];	//#1
            CellPLC_BmBeamDump_Status[1]  <= 1'b1;	//xy_pos_Reg[50][`IN_OFF+7] && xy_pos_Reg[50][`IN_OFF+6];	//#2
            CellPLC_BmBeamDump_Status[2]  <= 1'b1;	//xy_pos_Reg[76][`IN_OFF+11] && xy_pos_Reg[76][`IN_OFF+10];	//CELL 3,  [16] 2mA beam, [17] 50 mA beam status
            CellPLC_BmBeamDump_Status[3]  <= 1'b1;	//xy_pos_Reg[102][`IN_OFF+7] && xy_pos_Reg[102][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[4]  <= 1'b1;	//xy_pos_Reg[128][`IN_OFF+7] && xy_pos_Reg[128][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[5]  <= 1'b1;	//xy_pos_Reg[154][`IN_OFF+7] && xy_pos_Reg[154][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[6]  <= 1'b1;	//xy_pos_Reg[180][`IN_OFF+7] && xy_pos_Reg[180][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[7]  <= 1'b1;	//xy_pos_Reg[206][`IN_OFF+7] && xy_pos_Reg[206][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[8]  <= 1'b1;	//xy_pos_Reg[232][`IN_OFF+7] && xy_pos_Reg[232][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[9]  <= 1'b1;	//xy_pos_Reg[258][`IN_OFF+7] && xy_pos_Reg[258][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[10] <= 1'b1;	//xy_pos_Reg[284][`IN_OFF+7] && xy_pos_Reg[284][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[11] <= 1'b1;	//xy_pos_Reg[310][`IN_OFF+7] && xy_pos_Reg[310][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[12] <= 1'b1;	//xy_pos_Reg[336][`IN_OFF+7] && xy_pos_Reg[336][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[13] <= 1'b1;	//xy_pos_Reg[362][`IN_OFF+7] && xy_pos_Reg[362][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[14] <= 1'b1;	//xy_pos_Reg[388][`IN_OFF+7] && xy_pos_Reg[388][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[15] <= 1'b1;	//xy_pos_Reg[414][`IN_OFF+7] && xy_pos_Reg[414][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[16] <= 1'b1;	//xy_pos_Reg[440][`IN_OFF+7] && xy_pos_Reg[440][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[17] <= 1'b1;	//xy_pos_Reg[466][`IN_OFF+7] && xy_pos_Reg[466][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[18] <= 1'b1;	//xy_pos_Reg[492][`IN_OFF+7] && xy_pos_Reg[492][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[19] <= 1'b1;	//xy_pos_Reg[518][`IN_OFF+7] && xy_pos_Reg[518][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[20] <= 1'b1;	//xy_pos_Reg[544][`IN_OFF+7] && xy_pos_Reg[544][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[21] <= 1'b1;	//xy_pos_Reg[570][`IN_OFF+7] && xy_pos_Reg[570][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[22] <= 1'b1;	//xy_pos_Reg[596][`IN_OFF+7] && xy_pos_Reg[596][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[23] <= 1'b1;	//xy_pos_Reg[622][`IN_OFF+7] && xy_pos_Reg[622][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[24] <= 1'b1;	//xy_pos_Reg[648][`IN_OFF+7] && xy_pos_Reg[648][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[25] <= 1'b1;	//xy_pos_Reg[674][`IN_OFF+7] && xy_pos_Reg[674][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[26] <= 1'b1;	//xy_pos_Reg[700][`IN_OFF+7] && xy_pos_Reg[700][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[27] <= 1'b1;	//xy_pos_Reg[726][`IN_OFF+7] && xy_pos_Reg[726][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[28] <= 1'b1;	//xy_pos_Reg[752][`IN_OFF+7] && xy_pos_Reg[752][`IN_OFF+6];
            CellPLC_BmBeamDump_Status[29] <= 1'b1;	//xy_pos_Reg[778][`IN_OFF+7] && xy_pos_Reg[778][`IN_OFF+6];	//#30
            CellPLC_BmBeamDump_Status[30] <= 1'b1;
            CellPLC_BmBeamDump_Status[31] <= 1'b1;
            //
            //
            // PLC WatchDog status
            //CellPLCWdStatus[0]  <= xy_pos_Reg[`C3_CDATA_BASE][`IN_OFF+12];		//CELL 3,  [16] 2mA beam, [17] 50 mA beam status
            CellPLCWdStatus[0]  <= xy_pos_Reg[`C3_CDATA_BASE][`IN_OFF+8];
            CellPLCWdStatus[1]  <= xy_pos_Reg[`C5_CDATA_BASE][`IN_OFF+8];		//#5 UPSTREAM
            CellPLCWdStatus[2]  <= xy_pos_Reg[`C5_CDATA_BASE][`IN_OFF+8];
            CellPLCWdStatus[3]  <= xy_pos_Reg[`C10_CDATA_BASE][`IN_OFF+8];
            CellPLCWdStatus[4]  <= xy_pos_Reg[`C11_CDATA_BASE][`IN_OFF+8];
            CellPLCWdStatus[5]  <= xy_pos_Reg[`C23_CDATA_BASE][`IN_OFF+8];
            CellPLCWdStatus[6]  <= xy_pos_Reg[`C23_CDATA_BASE][`IN_OFF+8];
            CellPLCWdStatus[7]  <= xy_pos_Reg[`C28_CDATA_BASE][`IN_OFF+8];
            CellPLCWdStatus[8]  <= xy_pos_Reg[`C8_CDATA_BASE][`IN_OFF+8];
            CellPLCWdStatus[9]  <= xy_pos_Reg[`C18_CDATA_BASE][`IN_OFF+8];
            //
            CellPLCWdStatus[10] <= xy_pos_Reg[`C16_CDATA_BASE][`IN_OFF+8];
            CellPLCWdStatus[11] <= xy_pos_Reg[`C17_CDATA_BASE][`IN_OFF+8];
            CellPLCWdStatus[12] <= xy_pos_Reg[`C17_CDATA_BASE][`IN_OFF+8];
            //NEXT beamline
            CellPLCWdStatus[13] <= xy_pos_Reg[`C4_CDATA_BASE][`IN_OFF+8];
            CellPLCWdStatus[14] <= xy_pos_Reg[`C4_CDATA_BASE][`IN_OFF+8];
            //ID 12
            CellPLCWdStatus[15] <= xy_pos_Reg[`C12_CDATA_BASE][`IN_OFF+8];
            CellPLCWdStatus[16] <= xy_pos_Reg[`C12_CDATA_BASE][`IN_OFF+8];
            CellPLCWdStatus[17] <= xy_pos_Reg[`C21_CDATA_BASE][`IN_OFF+8];
            //ID 19
            CellPLCWdStatus[18] <= xy_pos_Reg[`C19_CDATA_BASE][`IN_OFF+8];
            CellPLCWdStatus[19] <= xy_pos_Reg[`C19_CDATA_BASE][`IN_OFF+8];
            //ID 02
            CellPLCWdStatus[20] <= xy_pos_Reg[`C2_CDATA_BASE][`IN_OFF+8];
            //ID 07
            CellPLCWdStatus[21] <= xy_pos_Reg[`C7_CDATA_BASE][`IN_OFF+8];
            CellPLCWdStatus[22] <= xy_pos_Reg[`C7_CDATA_BASE][`IN_OFF+8];
            //ID 27 HEX 9/3/2022
            CellPLCWdStatus[23] <= xy_pos_Reg[`C27_CDATA_BASE][`IN_OFF+8];
            //ID 20
            CellPLCWdStatus[24] <= xy_pos_Reg[`C20_CDATA_BASE][`IN_OFF+8];
            //
            CellPLCWdStatus[25] <= 1'b0;	//[674][`IN_OFF+8];
            CellPLCWdStatus[26] <= 1'b0;	//[700][`IN_OFF+8];
            CellPLCWdStatus[27] <= 1'b0;	//[726][`IN_OFF+8];
            CellPLCWdStatus[28] <= 1'b0;	//[752][`IN_OFF+8];
            CellPLCWdStatus[29] <= 1'b0;	//[778][`IN_OFF+8];	//#30
            CellPLCWdStatus[30] <= 1'b0;	//[778][`IN_OFF+8];	//#31
            CellPLCWdStatus[31] <= 1'b0;
		 
	
	
/* CANTING STATUS

	Cell 4
	IN-10 :   SR:C31-{AI}C04:D0_S16-I.B9        SR:C04-MG{PS:BC1A-ID05}CtctON-Sts    in operation
	IN-11 :   SR:C31-{AI}C04:D0_S16-I.BA       SR:C04-MG{PS:BC2A-ID05}CtctON-Sts    in operation

	IN-12 :   SR:C31-{AI}C04:D0_S16-I.BB       SR:C03-MG{PS:BC1A-ID04}CtctON-Sts    not yet
	IN-13 :   SR:C31-{AI}C04:D0_S16-I.BC       SR:C03-MG{PS:BC2A-ID04}CtctON-Sts    not yet
	IN-14 :   SR:C31-{AI}C04:D0_S16-I.BD       SR:C04-MG{PS:BC3A-ID04}CtctON-Sts    not yet

	Cell 5
	IN-10 :   SR:C31-{AI}C05:D0_S16-I.B9        SR:C05-MG{PS:BC3A-ID05}CtctON-Sts    in operation

	Cell 16
	IN-10 :   SR:C31-{AI}C16:D0_S16-I.B9        SR:C16-MG{PS:BC1A-ID17}CtctON-Sts    in operation
	IN-11 :   SR:C31-{AI}C16:D0_S16-I.BA       SR:C16-MG{PS:BC2A-ID17}CtctON-Sts    in operation

	Cell 17
	IN-10 :   SR:C31-{AI}C17:D0_S16-I.B9        SR:C17-MG{PS:BC3A-ID17}CtctON-Sts    in operation

	Cell 11
	IN-10 :   SR:C31-{AI}C11:D0_S16-I.B9        SR:C11-MG{PS:BC1A-ID12}CtctON-Sts    not yet
	IN-11 :   SR:C31-{AI}C11:D0_S16-I.BA       SR:C11-MG{PS:BC2A-ID12}CtctON-Sts    not yet

	Cell 12
	IN-10 :   SR:C31-{AI}C12:D0_S16-I.B9        SR:C12-MG{PS:BC3A-ID12}CtctON-Sts    not yet

*/
		 
            // READ CANTING STATUS from SDI REGISTER
            CM_SdiRawBitStatus_InReg[0] <= xy_pos_Reg[`C4_CDATA_BASE][`IN_OFF+9];
            CM_SdiRawBitStatus_InReg[1] <= xy_pos_Reg[`C4_CDATA_BASE][`IN_OFF+10];
            CM_SdiRawBitStatus_InReg[2] <= xy_pos_Reg[`C5_CDATA_BASE][`IN_OFF+9];
            //
            CM_SdiRawBitStatus_InReg[3] <= xy_pos_Reg[`C16_CDATA_BASE][`IN_OFF+9];
            CM_SdiRawBitStatus_InReg[4] <= xy_pos_Reg[`C16_CDATA_BASE][`IN_OFF+10];
            CM_SdiRawBitStatus_InReg[5] <= xy_pos_Reg[`C17_CDATA_BASE][`IN_OFF+9];
            //
            CM_SdiRawBitStatus_InReg[6] <= xy_pos_Reg[`C4_CDATA_BASE][`IN_OFF+11];
            CM_SdiRawBitStatus_InReg[7] <= xy_pos_Reg[`C4_CDATA_BASE][`IN_OFF+12];
            CM_SdiRawBitStatus_InReg[8] <= xy_pos_Reg[`C4_CDATA_BASE][`IN_OFF+13];
            //
            CM_SdiRawBitStatus_InReg[9] <= xy_pos_Reg[`C11_CDATA_BASE][`IN_OFF+9];
            CM_SdiRawBitStatus_InReg[10]<= xy_pos_Reg[`C11_CDATA_BASE][`IN_OFF+10];
            CM_SdiRawBitStatus_InReg[11]<= xy_pos_Reg[`C12_CDATA_BASE][`IN_OFF+9];

            // 09/22/16  : ID 19
            CM_SdiRawBitStatus_InReg[12] <= xy_pos_Reg[`C18_CDATA_BASE][`IN_OFF+9];
            CM_SdiRawBitStatus_InReg[13] <= xy_pos_Reg[`C18_CDATA_BASE][`IN_OFF+10];
            CM_SdiRawBitStatus_InReg[14] <= xy_pos_Reg[`C18_CDATA_BASE][`IN_OFF+11];
            //
            //
            // ID 7 Canting ??? check pin number. NO Canting
            //CM_SdiRawBitStatus_InReg[15] <= xy_pos_Reg[`C17_CDATA_BASE][`IN_OFF+9];
            //CM_SdiRawBitStatus_InReg[16] <= xy_pos_Reg[`C17_CDATA_BASE][`IN_OFF+10];
            //CM_SdiRawBitStatus_InReg[17] <= xy_pos_Reg[`C17_CDATA_BASE][`IN_OFF+11];

		
            //copy for latch
            //CM_SdiRawBitStatus2 <= CM_SdiRawBitStatus;


            // Canting Debounce output
            // #5
            Canting_sum[0]   <= CM_SdiRawBitStatus[0] && CM_SdiRawBitStatus[1] && CM_SdiRawBitStatus[2];
            CantedMagnets_Status[1]   <= Canting_sum[0];
            CantedMagnets_Status[2]   <= Canting_sum[0];

            // Cell 17 Up/Down
            Canting_sum[1]  <= CM_SdiRawBitStatus[3] && CM_SdiRawBitStatus[4] && CM_SdiRawBitStatus[5];
            CantedMagnets_Status[11]  <= Canting_sum[1];
            CantedMagnets_Status[12]  <= Canting_sum[1];
            //ID-4
            Canting_sum[2]  <= CM_SdiRawBitStatus[6] && CM_SdiRawBitStatus[7] && CM_SdiRawBitStatus[8];
            CantedMagnets_Status[13]  <= Canting_sum[2];
            CantedMagnets_Status[14]  <= Canting_sum[2];
            //ID12
            Canting_sum[3]  <= CM_SdiRawBitStatus[9] && CM_SdiRawBitStatus[10] && CM_SdiRawBitStatus[11];
            CantedMagnets_Status[15]  <= Canting_sum[3];
            CantedMagnets_Status[16]  <= Canting_sum[3];

            //ID19
            Canting_sum[4]  <= CM_SdiRawBitStatus[12] && CM_SdiRawBitStatus[13] && CM_SdiRawBitStatus[14];
            CantedMagnets_Status[18]  <= Canting_sum[4];
            CantedMagnets_Status[19]  <= Canting_sum[4];

            //ID07
            //Canting_sum[5]  <= CM_SdiRawBitStatus[15] && CM_SdiRawBitStatus[16] && CM_SdiRawBitStatus[17];
            CantedMagnets_Status[21]  <= 1'b1;	//Canting_sum[5];
            CantedMagnets_Status[22]  <= 1'b1;	//Canting_sum[5];


            //Make normal status
            CantedMagnets_Status[0]      <= 1'b1;
            CantedMagnets_Status[10:3]   <= 8'b11111111;
            CantedMagnets_Status[17]     <= 1'b1;
            CantedMagnets_Status[20]     <= 1'b1;

            CantedMagnets_Status[31:23]  <= 10'b1111111111;

		
            //06/11/15
            //01/24/17
            CellPLC_IdBeamDump_Status <= CellPLC_IdBeamDump_StatusDebounceOut;


            // 11/18/16 for latch fast dump status (01/24/17 found issue it's not updating )
            /*
            if(CellPLC_IdBeamDump_Status != UserMaskControl_0)
                CellPLC_IdBeamDump_Status <= CellPLC_IdBeamDump_StatusDebounceOut;
            else
                CellPLC_IdBeamDump_Status <= CellPLC_IdBeamDump_Status;
                */
            //}
		
        end
    end
	
	
    // Cell 17 - bit 9 caused several time
    // 10/20/16 Added debounce
    // 11/29/16
    // Canting magnet signal
    debouncer #
    (
        .DEBNC_CLOCKS (50000),			 // 20 ns * 50000 1 ms
        .PORT_WIDTH   (32)
    ) CM_glitch(
        .SIGNAL_I(CM_SdiRawBitStatus_InReg[31:0]),
        .CLK_I(clk), 			// 50 MHz
        .SIGNAL_O(CM_SdiRawBitStatus[31:0])
    );


    //
    // Fast dump denouncing about 5 cycles ( 500 us)
    // 11/29/16
    // debouncer
    debouncer #
    (   .DEBNC_CLOCKS (10000), 		 // 20 ns * 10000 200 us
        .PORT_WIDTH   (32)
    ) fast_dump_glitch(
        .SIGNAL_I (CellPLC_IdBeamDump_StatusReg),
        .CLK_I    (clk), 				// 50 MHz
        .SIGNAL_O (CellPLC_IdBeamDump_StatusDebounceOut)
    );

		
    // 09/29/16 added
    debouncer #
    (   .DEBNC_CLOCKS (20000), 		 // 20 ns * 20000 400 us
        .PORT_WIDTH   (32)
    ) gap_st_glitch(
        .SIGNAL_I(Gap_Status0),
        .CLK_I   (clk), 				// 50 MHz
        .SIGNAL_O(Gap_Status01)
    );

    // ID PS
    /*
    genvar IdPs_st_ref_i;
    generate
        for (IdPs_st_ref_i=0; IdPs_st_ref_i < `NOF_CELL_PLC; IdPs_st_ref_i=IdPs_st_ref_i+1) begin  : IDPSStatus_Debounce
                debounceIndex 	debounce_IO2(
                .reset(reset),
                .clk(gClk10kHz),
                .noisy(PhotonShutters_Status0[IdPs_st_ref_i]),
                .clean(PhotonShutters_Status01[IdPs_st_ref_i] )
                );
        end
    endgenerate
    */
    //01/17/17 changed to 1 ms
    debouncer #
    (
        .DEBNC_CLOCKS (50000),			 // 20 ns * 50000 1 ms
        .PORT_WIDTH   (32)
    ) id_ps_glitch(
        .SIGNAL_I(PhotonShutters_Status0[31:0]),
        .CLK_I   (clk), 			// 50 MHz
        .SIGNAL_O(PhotonShutters_Status01[31:0])
    );

	
    // BM PS
    /*
    genvar BmPs_st_ref_i;
    generate
        for (BmPs_st_ref_i=0; BmPs_st_ref_i < `NOF_CELL_PLC; BmPs_st_ref_i=BmPs_st_ref_i+1) begin  : BMPSStatus_Debounce
            debounceIndex 	debounce_IO2(
            .reset(reset),
            .clk(gClk10kHz),
            .noisy(BM_PhotonShutters_Status0[BmPs_st_ref_i]),
            .clean(BM_PhotonShutters_Status01[BmPs_st_ref_i] )
            );
        end
    endgenerate
    */
	
    //01/17/17 changed to 1 ms
    debouncer #
    (
        .DEBNC_CLOCKS (50000),			 // 20 ns * 50000 1 ms
        .PORT_WIDTH   (32)
    ) bm_ps_glitch(
        .SIGNAL_I(BM_PhotonShutters_Status0[31:0]),
        .CLK_I   (clk), 			// 50 MHz
        .SIGNAL_O(BM_PhotonShutters_Status01[31:0])
    );

	
	
    /*
    BM-AI Status
        OFF [I=50]
        ON [I>50]
    */

    //assign BM_DUMP =   BM_LATCHED_OUT || (BM_ERR_DUMP && plb_Reg10[8]);	//05/31/14  Keep latch until your reset
    //assign BM_DUMP =   AI_BM_RfTripCmd || (BM_ERR_DUMP && plb_Reg10[8]);	// OK but no debounce
    // 01/20/17 debounce 1 ms
    //assign BM_DUMP =   AI_BM_RfTripCmd || (Reg_bm_bpm_err_status_filt_out && plb_w_reg[15][13]) || (BM_ERR_DUMP && plb_Reg10[8]) ;

    //                    Pos envelope              debounce
    assign BM_DUMP =   AI_BM_RfTripCmd || (Reg_bm_bpm_err_status_filt_out && plb_Reg10[8]) ;

    //assign SLOW_DUMP = (BM_DUMP && plb_Reg10[15]);

    //assign FAST_DUMP = (ID_DUMP || plb_w_reg[13][6]) && plb_Reg10[14] ;
    // plb_w_reg[13][6] : uBlaze protection output
    // plb_Reg10[14]    : ID RF trip enable/disable
    //assign FAST_DUMP = (AI_ID_RfTripCmd || plb_w_reg[13][6]) && plb_Reg10[14];
    /*
    10/26/15, fast dump added CellPLC_IdBeamDumpOutReg,
    */
    //assign FAST_DUMP = (AI_ID_RfTripCmd || CellPLC_IdBeamDumpOutReg || plb_w_reg[13][6]) && plb_Reg10[14];
    //assign FINAL_DUMP_OUT = SLOW_DUMP || FAST_DUMP || CellPLC_IdBeamDumpOutReg; 	//(CellPLC_IdBeamDumpOutReg && plb_Reg10[13]);

    ////////////////////////////////////////////////////////////////////
    // 07/07/16
    // plb_Reg10[14]    - Fase enable/disable
    // plb_w_reg[13][6] - uBlaze dump

    //assign FAST_DUMP = AI_ID_RfTripCmd && plb_Reg10[14];
    //assign SLOW_DUMP = (BM_DUMP && plb_Reg10[15]);

    //assign FINAL_DUMP_OUT = SLOW_DUMP || FAST_DUMP || uBlazeDumpCmd;	//plb_w_reg[13][6];
    //assign FINAL_DUMP_OUT = SLOW_DUMP || FAST_DUMP;	// remove uBlazeDumpCmd status from the FINAL_DUMP_OUT



    /* ***************************************************/
    /*	6/13/20                                          */
    /*		Slow dump first ? then disable the fast dump */
    /*****************************************************/
    wire fastDUMP;
    wire slowDUMP;
    reg  SlowDumpOutReg;
    wire FastDelayedDumpOut;
    reg  fastDUMP_DelayCtrl;
    wire FastDelayedEnable;

    assign fastDUMP = AI_ID_RfTripCmd && plb_Reg10[14];
    assign slowDUMP = BM_DUMP && plb_Reg10[15];

    assign FastDelayedEnable = plb_w_reg[13][9];	//enable/disable

    always @ (posedge clk)
    begin
        if(GlobalLatchClearingReset == 1'b1) begin
            fastDUMP_DelayCtrl <= 1'b0;
        end
        else if(FastDelayedEnable == 1'b1) begin			//mps_plb_Reg13.fastDumpDelay = 1; mps_Reg_Set(13, *((Xuint32 *)&mps_plb_Reg13 ));
            if(fastDUMP == 1'b1 && slowDUMP == 1'b1) begin	//Fast dump and Slow dump ?
                fastDUMP_DelayCtrl <= 1'b1;
            end
            else if(fastDUMP == 1'b1 && slowDUMP == 1'b0) begin	//Fast Only
                fastDUMP_DelayCtrl <= 1'b0;
            end
        end
        else begin
            fastDUMP_DelayCtrl <= 1'b0;
        end
    end
	
    //delay control
    gateDelayFast Fast_delay (
        .Clk  (clk),
        .Inp  (fastDUMP),
        .Delay(plb_w_reg[11][17:10]*5000), 	//5000=100us	// 20 ns * 5000 = 100 (us)
        .Width(32'h7FFFFFFF),				// 42.949672940 sec
        .Q    (FastDelayedDumpOut)
    );
    ////////////////////////////////

    assign FAST_DUMP = (fastDUMP_DelayCtrl == 1'b1) ? (FastDelayedDumpOut & dcct_2mA) : fastDUMP;
    assign SLOW_DUMP = slowDUMP;
    assign FINAL_DUMP_OUT = SLOW_DUMP || FAST_DUMP;

    reg id_TS_status;
    //ID fast fault timestamp
    always @ (posedge clk ) begin
        if(reset) begin
            ID_FaultTimestampOut[63:32] <= `_BUILD_TIME_; 
        end
        else if(GlobalLatchClearingReset == 1'b1) begin
            ID_FaultTimestampOut[31:0] <= 32'd0; //9/7/22: nsec clear
            id_TS_status               <= 1'b0;
        end
        else if( (FAST_DUMP == 1'b1) && (id_TS_status == 1'b0) ) begin
            ID_FaultTimestampOut <= TS_time;
            id_TS_status         <= 1'b1;
        end
        else begin
            ID_FaultTimestampOut <= ID_FaultTimestampOut;
            id_TS_status         <= id_TS_status;
        end
    end
	
    //BM slow fault timestamp
    reg bm_TS_status;
    always @ (posedge clk ) begin
        if(reset) begin
            BM_FaultTimestampOut[63:32] <= `_BUILD_TIME_;
        end    
        else if(GlobalLatchClearingReset == 1'b1) begin
            BM_FaultTimestampOut[31:0] <= 32'd0; //9/7/22: nsec clear
            bm_TS_status               <= 1'b0;
        end
        else if( (SLOW_DUMP == 1'b1) && (bm_TS_status == 1'b0) ) begin
            BM_FaultTimestampOut <= TS_time;
            bm_TS_status         <= 1'b1;
        end
        else begin
            BM_FaultTimestampOut <= BM_FaultTimestampOut;
            bm_TS_status         <= bm_TS_status;
        end
    end

	

    /////////////////////////////////////////////



    /*
     * 	plb_Reg10[12]  - PLC BM Dump control
     *	plb_Reg10[13]  - PLC ID Dump control
     */
    /*
     * 04/13/15
     * Rf trip signal detected ? Stop update
     * Read by PM client software
     *
    */
    assign FINAL_FREE_DUMP_OUT = ~(BM_DUMP || AI_ID_RfTripCmd) ;
    assign SDI_Reg_WR_ENABLE   = FINAL_FREE_DUMP_OUT && SdiDataRdValid;


    // if Beam is dump, than stop BRAM Update
    assign CalBram_En = AIeCalOutLatch && FINAL_FREE_DUMP_OUT;	// 04/13/2015

    posDpram_32x512 	mm_x_cal_result (
        .clka  (clk), // input clka
        .ena   (1'b1), // input ena
        .wea   (CalBram_En), //Must need WR
        .addra ({3'd0, aie_addr} ), // input [8 : 0] addra
        .dina  (x_mm_cal),
        //read by uBlaze
        .rstb  (GlobalLatchClearingReset),
        .clkb  (clk), 	// input clkb
        .enb   (ram_cs2[10]), 	// input enb
        .addrb (ram_addr2[8:0]), 		// input [8 : 0] addrb
        .doutb (bram_odate[0])
    );

    posDpram_32x512 	mm_y_cal_result (
        .clka  (clk), // input clka
        .ena   (1'b1), // input ena
        .wea   (CalBram_En), //Must need WR
        .addra ({3'd0, aie_addr} ), // input [8 : 0] addra
        .dina  (y_mm_cal),
        //read by uBlaze
        .rstb  (GlobalLatchClearingReset),
        .clkb  (clk), 	// input clkb
        .enb   (ram_cs2[11]), 	// input enb
        .addrb (ram_addr2[8:0]), 		// input [8 : 0] addrb
        .doutb (bram_odate[1])
    );
	
    posDpram_32x512 	mrad_x_cal_result (
        .clka  (clk), // input clka
        .ena   (1'b1), // input ena
        .wea   (CalBram_En), //Must need WR
        .addra ({3'd0, aie_addr} ), // input [8 : 0] addra
        .dina  (x_mrad_cal),
        //read by uBlaze
        .rstb  (GlobalLatchClearingReset),
        .clkb  (clk), 	// input clkb
        .enb   (ram_cs2[12]), 	// input enb
        .addrb (ram_addr2[8:0]), 		// input [8 : 0] addrb
        .doutb (bram_odate[2])
    );

    posDpram_32x512 	mrad_y_cal_result (
        .clka  (clk), // input clka
        .ena   (1'b1), // input ena
        .wea   (CalBram_En), //Must need WR
        .addra ({3'd0, aie_addr} ), // input [8 : 0] addra
        .dina  (y_mrad_cal),
        //read by uBlaze
        .rstb  (GlobalLatchClearingReset),
        .clkb  (clk),            // input clkb
        .enb   (ram_cs2[13]),    // input enb
        .addrb (ram_addr2[8:0]), // input [8 : 0] addrb
        .doutb (bram_odate[3])
    );
    //calculation result
    assign plb_r_reg30 = (   plb_w_Reg12[6:4] == 3'b000) ? bram_odate[0] :
                            (plb_w_Reg12[6:4] == 3'b001) ? bram_odate[1] :
                            (plb_w_Reg12[6:4] == 3'b010) ? bram_odate[2] :
                            (plb_w_Reg12[6:4] == 3'b011) ? bram_odate[3] :
                            (plb_w_Reg12[6:4] == 3'b100) ? bram_odate[4] : 	//ID IL dump info
                            (plb_w_Reg12[6:4] == 3'b101) ? bram_odate[5] : 	//BM dump info
                                                                    32'd0;
                             
			
    /*
    *	74F3037
    *	Quad 2-input NAND 30 Ohm driver
    *
    *     INPUT    OUTPUT
    *  ---------------
    *	Dna Dnb 	/Qn
            L 	L 		H
            L 	H 		H
            H 	L 		H
            H 	H 		L
    */

    assign ai_pm_trigger_event = (plb_w_Reg12[1] == 1'b1) ? aiPM_Trigger : 1'b0;

    // Debounce Test 10/21/16
    wire debounce_out;
	
    // 11/29/16
    // debouncer test signal
    debouncer #
    (   .DEBNC_CLOCKS (50000),     // 20 ns * 50000 is 1 ms
        .PORT_WIDTH   (1)
    ) CM_test_glitch(
        .SIGNAL_I (din_buf[0]),
        .CLK_I    (clk),           // 50 MHz
        .SIGNAL_O (debounce_out)
    );



    /* 6/08 reassign Digital Out */
    // [0] 100 Hz ~ 2 kHz for Micloblaze or SDI link for Hertbert
    //assign ext_dout_buf[0]   = ~(FINAL_DUMP_OUT | aiPM_Trigger );	//PM Trigger
    assign ext_dout_buf[0]   = ~(FINAL_DUMP_OUT | aiPM_Trigger | fp_lemo_in[0] );  	//PM Trigger
    assign ext_dout_buf[1]   = ~fp_lemo_in[1];										//PM Reset to EVG	//~plb_w_Reg12[1];
    assign ext_dout_buf[2]   = debounce_out;	//(plb_w_Reg12[2] == 1'b1) ? din_buf[0] : din_buf[6] ;
    // those pin for debugging
    assign ext_dout_buf[3]   = (FINAL_DUMP_OUT | fp_lemo_in[0]);	//ExtDin_pin[0];	//RF DUMP STATUS to CELL 23 INPUT
    assign ext_dout_buf[4]   = FINAL_DUMP_OUT;	//xy_pos_Reg[`POS_ADDR_BEAM_CURRENT][16];	//2 mA status from #3 SDI
    assign ext_dout_buf[5]   = ~FINAL_DUMP_OUT;	//dcct_plc_wd_timeout;	//DCCT PLC timeout 1: Normal, 0: TO
    //
    // RF TRIP
    //          0 : OK         ---> LEMO OUT  1
    //			1 : TRIP       ---> LEMO OUT  0
    assign ext_dout_buf[6]   = SLOW_DUMP;	//LEMO 7 xy_pos_Reg[`POS_ADDR_BEAM_CURRENT][18];	//Hertbeat status from #3 SDI
    assign ext_dout_buf[7]   = SLOW_DUMP;	//LEMO 8
    assign ext_dout_buf[8]   = FAST_DUMP;	//LEMO 9
    assign ext_dout_buf[9]   = FAST_DUMP;	//LEMO 10
    //
    assign ext_dout_buf[10]  = ~(dcct_2mA & dcct_0_2mA & ~FINAL_DUMP_OUT);	//ext_heartbit_2kHz;	//(ext_heartbit_2kHz && ~io_watchdog_out);		//2 kHz free running
    /*
    *	0 : NO Dump
    *	1 : DUMP
    */
    assign ext_dout_buf[11]  = ~(dcct_2mA & dcct_0_2mA & ~FINAL_DUMP_OUT);  // EVG DBUS.5 INPUT

    // LEMO OUT HIGH : NORMAL
    //          LOW  : FAIL
    // Logic low is Ok, High is timeout
    // Lemo out is inverted logic so LOW -> inverted HIGH
    //assign ext_dout_buf[11]  = io_watchdog_out;		//0:Normal, 1:TO I/O hertbert status
    //assign ext_dout_buf[11]  = bm_bpm_err_status;

    //Inverting	for 74F3037 INPUT
    assign ExtDout_pin = ext_dout_buf;
	
	
	

    wire   lemo1_buf, lemo2_buf;
    /*
    *	06/06/2016 : ID bpm fault status added for 2 cycle continuce fault detect
    *	(AI_ID_ILOutStatus[7]&&plb_w_reg[13][5]) || (AI_ID_ILOutStatus[6]&&plb_w_reg[13][5])
    */
    //Debug output debug = { x2_pos_status, x1_pos_status, y_mrad_il, x_mrad_il, y_mm_il, x_mm_il, xy2_sum, xy1_sum };
    assign AI_ID_STATUS_OUT = ( (AI_ID_ILOutStatus[7]&&plb_w_reg[13][5]) || (AI_ID_ILOutStatus[6]&&plb_w_reg[13][5]) ||  AI_ID_ILOutStatus[5] || AI_ID_ILOutStatus[4] || AI_ID_ILOutStatus[3] || AI_ID_ILOutStatus[2] || AI_ID_ILOutStatus[1] || AI_ID_ILOutStatus[0]);

    assign lemo1_buf =  (plb_Reg10[31:28] == 4'd0)  ? sdi2_cc_LinkDataValid :
                    (plb_Reg10[31:28] == 4'd1)  ? sdi2_cc_RemotePacketDroppedStatus[0] :
                    (plb_Reg10[31:28] == 4'd2)  ? sdi2_cc_CWRemoteModeState :
                    (plb_Reg10[31:28] == 4'd3)  ? aiPM_Trigger :
                    (plb_Reg10[31:28] == 4'd4)  ? TRIG1 : 				//fp_lemo_in[0] : //aieIl_LatchValid_o :
                    (plb_Reg10[31:28] == 4'd5)  ? aieOutDataValid :
                    (plb_Reg10[31:28] == 4'd6)  ? AIeCalOutLatch :
                    (plb_Reg10[31:28] == 4'd7)  ? SdiDataRdValid : 		//bm_bpm_err_status_filt_out :	//ai_id_dump_rst :
                    (plb_Reg10[31:28] == 4'd8)  ? aie_USER_MASK_OUT :
                    (plb_Reg10[31:28] == 4'd9)  ? bpmPosFoFbDataValid : //ai_id_trip_src :
                    (plb_Reg10[31:28] == 4'd10) ? IL_BM_GlitchLatchedOut :
                    (plb_Reg10[31:28] == 4'd11) ? AI_ID_RfTripDebounceCmdReg :
                    (plb_Reg10[31:28] == 4'd12) ? xy_Mask_DataValid : //id_debounce_trip_out :
                    (plb_Reg10[31:28] == 4'd13) ? id_pos_glitch_status_1 :
                    (plb_Reg10[31:28] == 4'd14) ? fastDUMP : //ai_bm_trip_src : //ai_rst_trig :
                    (plb_Reg10[31:28] == 4'd15) ? fastDUMP_DelayCtrl : //CM_SdiRawBitStatus_InReg[2] : //DspMatrixCalStartTrig_5kHz :
                                                  1'b0;

    assign lemo2_buf = ( plb_Reg10[27:24] == 4'd0)  ?  sdi2_cc_CCW_LinkDataValid  :
                    (plb_Reg10[27:24] == 4'd1)  ?  sdi2_cc_RemotePacketDroppedStatus[1] :
                    (plb_Reg10[27:24] == 4'd2)  ?  bm_bpm_err_status :
                    (plb_Reg10[27:24] == 4'd3)  ?  AI_BM_ErrorCountTripCmd :
                    (plb_Reg10[27:24] == 4'd4)  ?  ai_id_multiple_err_status :
                    (plb_Reg10[27:24] == 4'd5)  ?  DspTrig :
                    (plb_Reg10[27:24] == 4'd6)  ?  AI_ID_RfTripCmd :
                    (plb_Reg10[27:24] == 4'd7)  ?  Reg_bm_bpm_err_status_filt_out :
                    (plb_Reg10[27:24] == 4'd8)  ?  ai_id_dump2 :
                    (plb_Reg10[27:24] == 4'd9)  ?  ai_id_glitch_rise :
                    (plb_Reg10[27:24] == 4'd10) ?  AI_ID_STATUS_OUT :
                    (plb_Reg10[27:24] == 4'd11) ?  ID_DUMP :
                    (plb_Reg10[27:24] == 4'd12) ?  AIeCalValid :
                    (plb_Reg10[27:24] == 4'd13) ?  CM_SdiRawBitStatus[0] : //CM_Status2 :
                    (plb_Reg10[27:24] == 4'd14) ?  FastDelayedDumpOut : //CM_SdiRawBitStatus[1] : //CM_Status : //Trig1Start :
                    (plb_Reg10[27:24] == 4'd15) ?  FAST_DUMP : //CM_SdiRawBitStatus[2] : //DspMatrixCalStartTrig:
                                                   1'b0;

    OBUFDS fp_lemo_out0  (.O(fp_out_p[0]), .OB(fp_out_n[0]), .I(lemo1_buf) );
    OBUFDS fp_lemo_out1  (.O(fp_out_p[1]), .OB(fp_out_n[1]), .I(lemo2_buf) );


    /*
     *  GTX 17, 18  for external Interface
     */	

    //Rx Output
    parameter 	PKT_LEN = 12'd780; //All SDI RAM data
    //parameter PKT_LEN = 12'd360; //SR-BPM closed orbit only

    wire	TxClock;
    wire [15:0] LinkDataAddress;   // address from data packet being processed
    wire [11:0] LinkPacketAddress; // address for forwarding packet if cell controller
    wire [11:0] LinkPacketLength;  // legth of the data packet being processed
    wire [31:0] LinkData;          // data from the packet being processed
    wire LinkDataValid;            // data valid to user application
    wire LinkStartOfPacket;
    wire LinkEndOfPacket;
    wire [11:0]   dbg;

    wire	CWRxUsrClock;
    wire [31:0] CWRxDataOut, CCWRxDataOut;
    wire CWRxCharIsKOut, CCWRxCharIsKOut;

    glb_gtx2_top	glb_gtx2_top_ (
        .Reset (reset),
        .trig  (Trig1Start),
        .clk   (clk),
        // TX packets
        .GtxRefClock         (GTX_RefClock),
        .SourcePacketSizeIn  (PKT_LEN+2),    //0x30E Head + CRC (780+2)
        .SourcePacketSAddrIn (12'd0),
        // SEND All the SDI received block
        .LocalData           (SdiOrSimGlobalBpmData),
        .LocalDataValid      (SdiDataRdValid),

        //RocketIO serial input
        .RXN_IN  (RXN_IN),
        .RXP_IN  (RXP_IN),
        .TXN_OUT (TXN_OUT),
        .TXP_OUT (TXP_OUT),
        //received Link data
        .TxClock125       (TxClock),
        .LinkDataAddress  (LinkDataAddress),
        .LinkPacketAddress(LinkPacketAddress),
        .LinkPacketLength (LinkPacketLength),
        .LinkData         (LinkData),
        .LinkDataValid    (LinkDataValid),
        .LinkStartOfPacket(LinkStartOfPacket),
        .LinkEndOfPacket  (LinkEndOfPacket),
        //received GTX
        .CWRxUsrClock     (CWRxUsrClock),
        .CWRxDataOut      (CWRxDataOut),
        .CCWRxDataOut     (CCWRxDataOut),
        .CWRxCharIsKOut   (CWRxCharIsKOut),
        .CCWRxCharIsKOut  (CCWRxCharIsKOut),
        .dbg(dbg)
    );
	
	
    assign xy_Mask_DataValid = UserBramOutXyMaskOut & bpmPos_xy_Addr_Valid & (bpmPos_xy_Addr > 10'd1);
		
    ////////////////////////////////////////////////////////////////
    assign eth_phy_led1		= eth_phy_led_link1000;  //--active low signal
    assign eth_phy_led2p 	= (eth_phy_led_rx == 1'b0)? 1'b1 : eth_phy_led_tx;
    assign eth_phy_led2n	= eth_phy_led_rx;



    ////////////////////////////////////
    // Front pannel Output
    assign fp_led[0] = sdi2_cc_NipStrobe || fifo_we_envelop;
    assign fp_led[1] = PosSimulation;            //Simulation 1:
    assign fp_led[2] = sdi2_cc_AllLinkValidStatus;
    assign fp_led[3] = FINAL_DUMP_OUT || led_trigger0;
    //Dump
    //SFP LEDs
    assign sfp_led[0]  = led_trigger0;
    assign sfp_led[1]  = 1'b0;				//evr_0_IP2INTC_Irpt1_LOL;		//Yello 1 - Loss
    assign sfp_led[2]  = plb_w_reg[13][26];		//NPI enable
    assign sfp_led[3]  = plb_w_reg[13][30];		//PM User reset
    assign sfp_led[4]  = plb_Addr1[20];	       //soft trigger
    assign sfp_led[5]  = 0;	
    assign sfp_led[6]  = pm_npi_enable; 		    //fp_lemo_in[0] | fp_lemo_in[1];
    assign sfp_led[7]  = npi_0_pm_reset_str;	//evr_TRIGGER_7;
    assign sfp_led[8]  = ~sdi2_cc_sfp_link[1];
    assign sfp_led[9]  = sdi2_cc_sfp_link[0];
    assign sfp_led[10] = ~sdi2_cc_sfp_link[3];
    assign sfp_led[11] = sdi2_cc_sfp_link[2];


    //Debug LED
    assign dbg_led[0] = bm_pos_glitch_status_1;	//Il_activated;		//IL Latch clear
    assign dbg_led[1] = cell_plc_wd_timeout[0];	//Il_latched_s[0];     	//Diamond xy1
    assign dbg_led[2] = cell_plc_wd_timeout[1];	//Il_latched_s[1];  		//Diamond xy2
    assign dbg_led[3] = cell_plc_wd_timeout[2];	//Il_latched_s[2];
    assign dbg_led[4] = cell_plc_wd_timeout[3];	//Il_latched_s[3];
    assign dbg_led[5] = plb_w_reg[15][12];	//evr_TRIGGER_5;	//cell_plc_wd_timeout[4];	//Il_latched_s[4];
    assign dbg_led[6] = AI_BM_RfTripCmd;	//evr_TRIGGER_6;	//dcct_plc_wd_timeout;	//Il_latched_s[5];             //mrad_iL_x_out[3]; //mrad_iL_out[0];
    assign dbg_led[7] = AI_BM_RfTripDebounce5CycleCmdReg;	//io_watchdog_out;	//BM_LATCHED_OUT;     //mrad_iL_y_out[3]; //mrad_iL_out[1];


    // Debug Output  bpmPosDataValid, bpmPosFoFbDataValid;
    assign DebugOUT[0]  = TRIGGER_1;
    assign DebugOUT[1]  = SdiDataRdValid;	//aie_USER_MASK_OUT;			//npi_0_pm_reset;	//bpmPosDataValid;	//dac_dbg[6];					//sdi2_0_LinkDataValid;
    assign DebugOUT[2]  = sdi2_cc_StartCalc;	//AIeCalOutLatch;				//npi_stop;	//bpmPosFoFbDataValid;     //;
    assign DebugOUT[3]  = BM_LATCHED_OUT;				//pm_npi_enable;	//DspMatrixCalStartTrig; //wfm_fifo_we;
    assign DebugOUT[4]  = IL_BM_GlitchLatchedOut;		//npi_burst_0_wr;	//bpmPos_xy_Addr_Valid; 		//sdi2_0_DPRAM_Valid;
    assign DebugOUT[5]  = AI_BM_RfTripDebounce5CycleCmdReg;			//Npi0_PM_RstOut;	//sdi2_CW_LocalPktDataValid;	//wfm_fifo_full;
    assign DebugOUT[6]  = AI_ID_RfTripDebounceCmdReg;	//plb_w_reg[13][31];	//AIeCalValid;	//Npi0_PM_RstOut;
    assign DebugOUT[7]  = FINAL_DUMP_OUT;	//;	//cw_pkt_start_trig;	//npi_stop;	//npi_sync_trig;
    assign DebugOUT[8]  = AIeCalValid;	//gClk10kHz;	//SdiDataRdValid;	//UsrTrig_NipStrobe_Synced;	//ccw_pkt_start_trig;	//IL_SUM;	//npi_0_pm_reset;	//fifo_we_envelop;
    assign DebugOUT[9]  = pm_npi_wr_valid;	//xy_Mask_DataValid;	//UserBramOutXyMaskOut;	//evr_TRIGGER_7;				//0;
    assign DebugOUT[10] = bm_pos_glitch_status_1;	//npi_burst_0_reset;	//sdi2_cc_LinkDataValid;
    assign DebugOUT[11] = BM_ERR_DUMP;	//sdi2_cc_CCW_LinkDataValid;
    assign DebugOUT[12] = aieIl_LatchValid_o;	//t5khz_0_reg2;	//npi_burst_0_reset;	//sdi2_cc_CWRemoteModeState;
    assign DebugOUT[13] = Reg_bm_bpm_err_status_filt_out;		//bm_pos_wr_n0;	//npi_1_strb_reset;	//sdi2_cc_CCWRemoteModeState;
    assign DebugOUT[14] = Qo_5kHz;		//ram_WR2;	//Npi0_PM_RstOut;	//sdi2_cc_StartCalc;
    assign DebugOUT[15] = id_pos_glitch_status_1;		//bpmPosAddrRdValid;	//Npi_1_WrEn;			//Dac10kHzDataClk;	//npi_fifo_wr_en;	//pm_npi_wr;
    assign DebugOUT[16] = id_debounce_trip_out;			//bm_pos_err_valid;	//u_fifo_we;	//aie_USER_MASK_O;	//Npi_1_WrEn;         //;
    assign DebugOUT[17] = bm_pos_wr_n1;	//sync_trig;	//NpiWrEn_0;		//cw_pkt_trig;	//sync_trig;				//sdi2_0_NipEnable;
    assign DebugOUT[18] = bm_err_cal_start;		//npi_burst_0_wr;	//ccw_pkt_trig;	//cw_pkt_start_trig;	//sdi2_cc_DPRAM_ReadStart;		//ram_WR2;
    assign DebugOUT[19] = Trig1Start;			//sdi2_cc_DPRAM_Valid;
		
    //evr_DIST_BUS_2_pin

    // Registers for uBlaze reading
    wire  [15:0] din_buf;
    assign din_buf = ~ExtDin_pin;

    //BM_ilCount added 03/17/2014
    assign plb_r_reg29 = (ram_cs2[1] == 1'b1) ? mask_ram_data_a : BM_ilCount;
    assign plb_r_reg28 = bm_bpm_err_counts;
    assign plb_r_reg27 = `FIRMWARE_VER;
    assign plb_r_reg26 = xy_pos_fifo_out;
    assign plb_r_reg21 = RefDpramOut;

    //
    // trigger increment every trigger
    always @( posedge clk ) begin
        if ( GlobalLatchClearingReset == 1'b1 )	//count reset
            t_CNT <= 8'd0;
        else if (npi_1_strb_reset == 1'b1) t_CNT <= t_CNT + 1;
        else
            t_CNT <= t_CNT;
    end


    //                        31             30                       29             [28:20]         19          18                 [17]          [16]           [15:0]
    //assign plb_r_reg24 = {  dcct_beam_loss, CellPLC_IdBeamDumpOutReg, sdi2_cc_DirLED, 9'd0, dcct_plc_hb, dcct_sys_fault, dcct_plc_wd_timeout, io_watchdog_out, din_buf};
    // 07/07/16 added AI_ID_STATUS_OUTreg_cnt
    //assign plb_r_reg24 = {  dcct_beam_loss, CellPLC_IdBeamDumpOutReg, sdi2_cc_DirLED, AI_ID_STATUS_OUTreg_cnt[8:0], dcct_plc_hb, dcct_sys_fault, dcct_plc_wd_timeout, io_watchdog_out, din_buf};

    // 04/09/19 Added status plb_w_reg[15][14], AI_ID_RfTripDebounceCmdReg
    assign plb_r_reg24 = {  dcct_beam_loss, CellPLC_IdBeamDumpOutReg, sdi2_cc_DirLED, AI_ID_STATUS_OUTreg_cnt[8:3], plb_w_reg[15][14], ai_id_dump_status, AI_ID_RfTripDebounceCmdReg, dcct_plc_hb, dcct_sys_fault, dcct_plc_wd_timeout, io_watchdog_out, din_buf};


    //5/30/13 added for system monitoring
    /*
    id_TS_status - FAST_DUMP
    bm_TS_status - SLOW_DUMP

    */
    wire [31:0] sysStatus;
    assign sysStatus[0]     = PosSimulation;
    assign sysStatus[1]     = plb_w_Reg12[31];			//beam sim ?
    assign sysStatus[2]     = dcct_2mA;
    assign sysStatus[3]     = dcct_50mA;
    assign sysStatus[4]     = sdi2_cc_NipStrobe || fifo_we_envelop;
    assign sysStatus[5]     = FINAL_DUMP_OUT;	//FINAL DUMP
    assign sysStatus[6]     = BM_DUMP;			//AI_BM_RfTripCmd;
    assign sysStatus[7]     = ID_DUMP;			//ID Glitch
    assign sysStatus[8]     = Il_activated;				//plb_Reg9[27];
    assign sysStatus[9]     = plb_w_reg[13][26];		//PM NPI Run/Stop
    assign sysStatus[10]    = plb_w_reg[13][29];		//POS NPI Run/Stop
    assign sysStatus[11]    = pm_npi_enable;			// pm enable status read plb_w_reg[13][30];
    assign sysStatus[12]    = plb_Reg10[14];			//ID Fast dump enable
    assign sysStatus[13]    = plb_Reg10[15];
    assign sysStatus[14]    = sdi2_cc_AllLinkValidStatus;
    //assign sysStatus[19:15] = Il_latched_s[5:0];
    //assign sysStatus[16:15] = {dcct_500mA, dcct_sys_fault};		//Il_latched_s[2:0];
    assign sysStatus[15]    = AI_BM_RfTripDebounce5CycleCmdReg;	//BM_ERR_DUMP;
    assign sysStatus[16]    = AI_BM_ErrorCountTripCmd;
    assign sysStatus[17]    = FINAL_FREE_DUMP_OUT;
    assign sysStatus[18]    = bm_bpm_enable;
    assign sysStatus[19]    = Reg_bm_bpm_err_status_filt_out;	// 2 ms bpm error status
    /*
    assign sysStatus[20]    = ~sdi2_cc_sfp_link[1];
    assign sysStatus[21]    = sdi2_cc_sfp_link[0];
    assign sysStatus[22]    = ~sdi2_cc_sfp_link[3];
    assign sysStatus[23]    = sdi2_cc_sfp_link[2];
    */
    assign sysStatus[20]    = ~sdi2_cc_sfp_link[1] | sdi2_cc_sfp_link[0] | ~sdi2_cc_sfp_link[3] | sdi2_cc_sfp_link[2];
    assign sysStatus[21]    = fastDUMP_DelayCtrl;	//~sdi2_cc_sfp_link[3] | sdi2_cc_sfp_link[2];
    assign sysStatus[22]    = fp_lemo_in[0];
    assign sysStatus[23]    = EVR_PM_Reset_Sts;	//fp_lemo_in[1];
    //
    assign sysStatus[24]    = uBlazeDumpCmd;	//plb_w_reg[13][6];			//uBlaze user rf dump     u_fifo_full;
    assign sysStatus[25]    = CellPLC_IdBeamDumpOutReg;	//07/07/16 frentend PLC dump status
    assign sysStatus[26]    = aiPM_Trigger;				//bm_bpm_data_fail;
    assign sysStatus[27]    = dcct_0_2mA;
    assign sysStatus[28]    = CM_Status_reg;
    assign sysStatus[29]    = BM_PS_closed_status;		//07/07/16
    assign sysStatus[30]    = ID_BpmFailStatusLatch;
    assign sysStatus[31]    = led_trigger0;

    assign plb_r_reg23      = sysStatus;				//monitoring system status


`define	BMIL_BASE	16
    always @ (posedge clk)
    begin
        if(DspTrig == 1'b1) begin
            SysMonData[0] <= {9'd0, aie_addrBmPsReg, sfp_eeprom[0]};
            SysMonData[1] <= {16'd0, sfp_eeprom[1]};
            SysMonData[2] <= {16'd0, sfp_eeprom[2]};
            SysMonData[3] <= {16'd0, sfp_eeprom[3]};
            SysMonData[4] <= {16'd0, sfp_eeprom[4]};
            SysMonData[5] <= {16'd0, sfp_eeprom[5]};
            //
            SysMonData[6]  <= IDAI_BeamDumpControlReg[31:0];
            SysMonData[7]  <= IDAI_BeamDumpControlReg[63:32];
            SysMonData[8]  <= gap_status_info[31:0];	//Gap_Status[31:0];
            SysMonData[9]  <= gap_status_info[63:32];	//Gap_Status[63:31];
            //
            SysMonData[47] <= Aie_bm_o[31:0];
            SysMonData[48] <= Aie_bm_o[63:32];
            SysMonData[49] <= Aie_A_o[31:0];
            SysMonData[50] <= Aie_A_o[63:32];
            SysMonData[51] <= Aie_C_o[31:0];
            SysMonData[52] <= Aie_C_o[63:32];
            SysMonData[53] <= Aie_D_o[31:0];
            SysMonData[54] <= Aie_D_o[63:32];
            SysMonData[55] <= aie_DumpStatus[31:0];
            SysMonData[56] <= aie_DumpStatus[63:32];
            //
            SysMonData[57] <= PhotonShutters_Status_info[31:0];
            SysMonData[58] <= PhotonShutters_Status_info[63:32];
            SysMonData[59] <= IDAI_RunStopStatusReg[31:0];
            SysMonData[60] <= IDAI_RunStopStatusReg[63:32];
            SysMonData[61] <= BM_PhotonShutters_Status_info[31:0];
            SysMonData[62] <= BM_PhotonShutters_Status_info[63:32];
            //
            SysMonData[63] <= FaPmTrigCnt_out;
            SysMonData[64] <= CantedMagnets_Status_Reg[31:0];	//CantedMagnets_Status[31:0];
            SysMonData[65] <= CellPLCWdStatus;			//09/22/14
            //SysMonData[66] <= CellPLC_IdBeamDump_Status;
            // 01/23/17 added for Latched data display
            SysMonData[66] <= (CellPLC_IdBeamDumpOutReg == 1'b1) ? CellPLC_IdBeamDump_Status_Latch_Reg : CellPLC_IdBeamDump_Status;
            SysMonData[67] <= CellPLC_BmBeamDump_Status;
            SysMonData[68] <= ID_BpmPosValueStatus[31:0];		//09/25/14
            SysMonData[69] <= ID_BpmPosValueStatusReg2[31:0];
            //SysMonData[70] 	- timestamp
            //SysMonData[71] 	- timestamp

            SysMonData[72] <= Cell_PLC_WD_Status;		//11/22/14
            SysMonData[73] <= cell_plc_wd_timeout;
            SysMonData[74] <= {20'd0, ext_dout_buf};	//bm_spike_cnt;				//06/08/15
            SysMonData[75] <= x_orbit_rms;
            SysMonData[76] <= y_orbit_rms;
            //SysMonData[77]		- reserved
            //SysMonData[78]		- reserved
            SysMonData[79] <= {16'd0, bm_xx_pos_out_cnt};
            SysMonData[80] <= {16'd0, bm_yy_pos_out_cnt};
            SysMonData[81] <= `FIRMWARE_VER;
            SysMonData[82] <= CantedMagnets_Status[31:0];		//08/05/2016 added
            SysMonData[83] <= 32'h00000083;
            SysMonData[84] <= CM_SdiRawBitStatus_Reg[31:0];
            SysMonData[85] <= 32'h00000083;
            //SysMonData[85] <= 32'h00000085;
            //SysMonData[86] <= 32'h00000086;
            //SysMonData[87] <= 32'h00000087;
            //SysMonData[88] <= 32'h00000088;
            //SysMonData[89] <= 32'h00000089;
            // 6/16/20
            // 9/7/22 change sec/nsec register
            SysMonData[86] <= ID_FaultTimestampOut[63:32];	//sec
            SysMonData[87] <= ID_FaultTimestampOut[31:0];	//nsec
            SysMonData[88] <= BM_FaultTimestampOut[63:32];	//sec
            SysMonData[89] <= BM_FaultTimestampOut[31:0];	//nsec

            //
            SysMonData[90] <= {16'd0, id_glitch_count}; //32'd90;
            SysMonData[91] <= 32'd91;
            SysMonData[92] <= 32'd92;
            SysMonData[93] <= 32'd93;
            SysMonData[94] <= 32'd94;
            SysMonData[95] <= 32'd95;
            SysMonData[96] <= 32'd96;



            //10, 11,12,13,14, 15 -- for timestamp
            /*
             *  AI-BM Interlock status monitoring
             *	for RF BPM ONLY, 0 - 359
             */
`define	BPM_Y_OFF	180
            // start address 16                           Y                                      X
            SysMonData[`BMIL_BASE+0]  <= {16'd0, bm_iL_info[`BPM_Y_OFF+5:`BPM_Y_OFF+0],  4'b0000,  bm_iL_info[5:0]};	//#1
            SysMonData[`BMIL_BASE+1]  <= {16'd0, bm_iL_info[`BPM_Y_OFF+11:`BPM_Y_OFF+6], 4'b0000,  bm_iL_info[11:6]};	//#2
            SysMonData[`BMIL_BASE+2]  <= {16'd0, bm_iL_info[`BPM_Y_OFF+17:`BPM_Y_OFF+12], 4'b0000, bm_iL_info[17:12]};	//#3
            SysMonData[`BMIL_BASE+3]  <= {16'd0, bm_iL_info[`BPM_Y_OFF+23:`BPM_Y_OFF+18], 4'b0000, bm_iL_info[23:18]};	//#4
            SysMonData[`BMIL_BASE+4]  <= {16'd0, bm_iL_info[`BPM_Y_OFF+29:`BPM_Y_OFF+24], 4'b0000, bm_iL_info[29:24]};	//#5
            SysMonData[`BMIL_BASE+5]  <= {16'd0, bm_iL_info[`BPM_Y_OFF+35:`BPM_Y_OFF+30], 4'b0000, bm_iL_info[35:30]};	//#6
            SysMonData[`BMIL_BASE+6]  <= {16'd0, bm_iL_info[`BPM_Y_OFF+41:`BPM_Y_OFF+36], 4'b0000, bm_iL_info[41:36]};	//#7
            SysMonData[`BMIL_BASE+7]  <= {16'd0, bm_iL_info[`BPM_Y_OFF+48:`BPM_Y_OFF+42], 4'b0000, bm_iL_info[47:42]};	//#8
            SysMonData[`BMIL_BASE+8]  <= {16'd0, bm_iL_info[`BPM_Y_OFF+53:`BPM_Y_OFF+48], 4'b0000, bm_iL_info[53:48]};	//#9
            SysMonData[`BMIL_BASE+9]  <= {16'd0, bm_iL_info[`BPM_Y_OFF+59:`BPM_Y_OFF+54], 4'b0000, bm_iL_info[59:54]};	//#10
            //
            SysMonData[`BMIL_BASE+10] <= {16'd0, bm_iL_info[`BPM_Y_OFF+65:`BPM_Y_OFF+60], 4'b0000, bm_iL_info[65:60]};
            SysMonData[`BMIL_BASE+11] <= {16'd0, bm_iL_info[`BPM_Y_OFF+71:`BPM_Y_OFF+66], 4'b0000, bm_iL_info[71:66]};
            SysMonData[`BMIL_BASE+12] <= {16'd0, bm_iL_info[`BPM_Y_OFF+77:`BPM_Y_OFF+72], 4'b0000, bm_iL_info[77:72]};
            SysMonData[`BMIL_BASE+13] <= {16'd0, bm_iL_info[`BPM_Y_OFF+83:`BPM_Y_OFF+78], 4'b0000, bm_iL_info[83:78]};
            SysMonData[`BMIL_BASE+14] <= {16'd0, bm_iL_info[`BPM_Y_OFF+89:`BPM_Y_OFF+84], 4'b0000, bm_iL_info[89:84]};
            SysMonData[`BMIL_BASE+15] <= {16'd0, bm_iL_info[`BPM_Y_OFF+95:`BPM_Y_OFF+90], 4'b0000, bm_iL_info[95:90]};
            SysMonData[`BMIL_BASE+16] <= {16'd0, bm_iL_info[`BPM_Y_OFF+101:`BPM_Y_OFF+96], 4'b0000, bm_iL_info[101:96]};
            SysMonData[`BMIL_BASE+17] <= {16'd0, bm_iL_info[`BPM_Y_OFF+107:`BPM_Y_OFF+102], 4'b0000, bm_iL_info[107:102]};
            SysMonData[`BMIL_BASE+18] <= {16'd0, bm_iL_info[`BPM_Y_OFF+113:`BPM_Y_OFF+108], 4'b0000, bm_iL_info[113:108]};
            SysMonData[`BMIL_BASE+19] <= {16'd0, bm_iL_info[`BPM_Y_OFF+119:`BPM_Y_OFF+114], 4'b0000, bm_iL_info[119:114]};
            SysMonData[`BMIL_BASE+20] <= {16'd0, bm_iL_info[`BPM_Y_OFF+125:`BPM_Y_OFF+120], 4'b0000, bm_iL_info[125:120]};
            SysMonData[`BMIL_BASE+21] <= {16'd0, bm_iL_info[`BPM_Y_OFF+131:`BPM_Y_OFF+126], 4'b0000, bm_iL_info[131:126]};
            SysMonData[`BMIL_BASE+22] <= {16'd0, bm_iL_info[`BPM_Y_OFF+137:`BPM_Y_OFF+132], 4'b0000, bm_iL_info[137:132]};
            SysMonData[`BMIL_BASE+23] <= {16'd0, bm_iL_info[`BPM_Y_OFF+143:`BPM_Y_OFF+138], 4'b0000, bm_iL_info[143:138]};
            SysMonData[`BMIL_BASE+24] <= {16'd0, bm_iL_info[`BPM_Y_OFF+149:`BPM_Y_OFF+144], 4'b0000, bm_iL_info[149:144]};
            SysMonData[`BMIL_BASE+25] <= {16'd0, bm_iL_info[`BPM_Y_OFF+155:`BPM_Y_OFF+150], 4'b0000, bm_iL_info[155:150]};
            SysMonData[`BMIL_BASE+26] <= {16'd0, bm_iL_info[`BPM_Y_OFF+161:`BPM_Y_OFF+156], 4'b0000, bm_iL_info[161:156]};
            SysMonData[`BMIL_BASE+27] <= {16'd0, bm_iL_info[`BPM_Y_OFF+167:`BPM_Y_OFF+162], 4'b0000, bm_iL_info[167:162]};
            SysMonData[`BMIL_BASE+28] <= {16'd0, bm_iL_info[`BPM_Y_OFF+173:`BPM_Y_OFF+168], 4'b0000, bm_iL_info[173:168]};
            SysMonData[`BMIL_BASE+29] <= {16'd0, bm_iL_info[`BPM_Y_OFF+179:`BPM_Y_OFF+174], 4'b0000, bm_iL_info[179:174]};

        end
    end
	
    wire	[63:0] fifo_out64;
    wire	npi_fifo_empty;
    wire	npi_fifo_wr_en;
    //
    //CC
    // npi-1
    // epics npi enable and npi strobe for npi write
    //
    reg	[2:0] sdi2_cc_NipStrobe_dly;
    reg	UsrTrig_NipStrobe_Synced;
    always @ (posedge clk)
    begin
        if (Trig1Start == 1'b1) begin
            sdi2_cc_NipStrobe_dly[0] <= sdi2_cc_NipStrobe;
            //sdi2_cc_NipStrobe_dly[1] <= sdi2_cc_NipStrobe_dly[0];
            //sdi2_cc_NipStrobe_dly[2] <= sdi2_cc_NipStrobe_dly[1];
            //UsrTrig_NipStrobe_Synced <= sdi2_cc_NipStrobe_dly[2];
            UsrTrig_NipStrobe_Synced <= sdi2_cc_NipStrobe_dly[0];
        end
        else
            UsrTrig_NipStrobe_Synced <= UsrTrig_NipStrobe_Synced;
    end


    assign npi_fifo_wr_en = pm_npi_wr_valid & plb_w_reg[13][29] & UsrTrig_NipStrobe_Synced;	//10/13/14  8 bpms

    assign Npi_1_WrEn = ~npi_fifo_empty;
	
    fifo_npi fifo_npi_burst_1 (
        .rst(reset), 		// input rst
        .wr_clk(clk), 	// input wr_clk
        .rd_clk(clk), 	// input rd_clk
        .din( DacData ),  //SdiOrSimGlobalBpmData ), // input [31 : 0] din
        .wr_en( npi_fifo_wr_en ), // input wr_en
        .rd_en( Npi_1_WrEn ), 	// input rd_en
        .dout(fifo_out64), 		// output [63 : 0] dout
        .full(), 					// output full
        .empty(npi_fifo_empty) 	// output empty
    );
	
    //NPI Data Endian Conversion
    //npi_burst_cc_fifo_din( { NpiData1, NpiData0 } ),
    assign NpiData1[7:0]   = fifo_out64[31:24];
    assign NpiData1[15:8]  = fifo_out64[23:16];
    assign NpiData1[23:16] = fifo_out64[15:8];
    assign NpiData1[31:24] = fifo_out64[7:0];

    assign NpiData0[7:0]   = fifo_out64[63:56];
    assign NpiData0[15:8]  = fifo_out64[55:48];
    assign NpiData0[23:16] = fifo_out64[47:40];
    assign NpiData0[31:24] = fifo_out64[39:32];
	
    //
    // NPI_BURST_0
    //
    wire npi_burst_0_wr;
    wire [31:0] npi_burst_0_data;
    //wire	[31:0] x_um_filt_o, x_um_o, x_dt_nm_o;

    wire [9:0] Counter_Free_Running;
    wire  npi_burst_wr_enable;
    fofb_addr_gen 	addr_gen_npi (
        .clk(clk),
        .reset(reset),
        .trig( SdiRamSingleValid ),
        .clk_enable( PosXyDataValid ), //1'b1),
        .Gain(16'd1),
        .address_move(12'd0),
        .AddressStart(12'd0),
        .AddressEnd({2'b00, plb_w_reg[11][9:0]} ),	//width control, single pulse = 0 , 10 = 9
        .ce_out(),
        .Data16b(),
        .Data32b(),
        .addr(Counter_Free_Running), 		//10bit
        .wr_one(),
        .wr(npi_burst_wr_enable)
    );

    ////////////////////
    wire	[63:0] fifo_out64_0;
    wire	npi_fifo_empty_0;
    assign	NpiWrEn_0 = ~npi_fifo_empty_0;


    //NPI reset logic
    reg npi_0_Reg, npi_0_reg1, npi_0_reg2;
    always @ (posedge clk)
    begin
        npi_0_Reg  <= UsrTrig_NipStrobe_Synced;	//10/14/14
        npi_0_reg1 <= npi_0_Reg;
        npi_0_reg2 <= (npi_0_Reg & !npi_0_reg1);
    end
    assign npi_1_strb_reset = npi_0_reg2;

    fifo_npi fifo_npi_burst_0 (
        .rst(reset), 		// input rst
        .wr_clk(clk), 	// input wr_clk
        .rd_clk(clk), 	// input rd_clk
        .din( npi_burst_0_data ), // input [31 : 0] din
        .wr_en( npi_burst_0_wr ), // input wr_en
        .rd_en( NpiWrEn_0 ), // input rd_en
        .dout(fifo_out64_0), // output [63 : 0] dout
        .full(), 				// output full
        .empty(npi_fifo_empty_0) // output empty
    );
	
	
    //Post Morterm 08-26/14
    wire   pm_npi_wr_valid;
    assign pm_npi_wr         = pm_npi_wr_valid && SdiDataRdValid && pm_npi_enable & plb_w_reg[13][26];
    assign npi_burst_0_wr    = pm_npi_wr;
    assign npi_burst_0_data  = DacData;	// 1 clock delay from  SdiOrSimGlobalBpmData;
    assign npi_burst_0_reset = npi_0_pm_reset;

    wire   cell_start_addr;
    assign cell_start_addr = (SdiRamAddr == 10'd0) | ((SdiRamAddr % 26) == 0) ;

    reg [5:0] count;
    always @ (posedge clk)
    begin
        if (cell_start_addr == 1'b1 && SdiDataRdValid == 1'b1)
            count <= 6'd0;
        else if (SdiDataRdValid == 1'b1)
            count <= count+1'b1;
        else count <= count;
    end
    assign pm_npi_wr_valid = (count > 6'd15) ? 1'b0 : 1'b1;
    ///// end /////
	
	
	
    //NPI Data Endian Conversion
    //npi_burst_0_fifo_din ( {Npi_0_Data1, Npi_0_Data0} ),
    assign Npi_0_Data1[7:0]   = fifo_out64_0[31:24];
    assign Npi_0_Data1[15:8]  = fifo_out64_0[23:16];
    assign Npi_0_Data1[23:16] = fifo_out64_0[15:8];
    assign Npi_0_Data1[31:24] = fifo_out64_0[7:0];
    //
    assign Npi_0_Data0[7:0]   = fifo_out64_0[63:56];
    assign Npi_0_Data0[15:8]  = fifo_out64_0[55:48];
    assign Npi_0_Data0[23:16] = fifo_out64_0[47:40];
    assign Npi_0_Data0[31:24] = fifo_out64_0[39:32];

	
    // Feb/16/2014 added
    ublaze_watchdog		ext_board_watchdog
    (
       .sys_clk_i(clk),
       .reset(reset),
       .UsrClear(plb_w_Reg12[7]),	//user reset
       .enable(1'b1),
       .EXT_TRIG_B( plb_Reg10[11] ),
       .trig_watchdog_rst( io_watchdog_out ),	// 1: timeout, 0 : normal
       .timeout_cnt(),
       .trig_cnt()
    );

    wire	dcct_plc_wd_timeout;
    ublaze_watchdog		dcct_plc_watchdog
    (
       .sys_clk_i(clk),
       .reset(reset),
       .UsrClear(plb_w_Reg12[7]),	//user reset
       .enable(1'b1),
       .EXT_TRIG_B( dcct_plc_hb ),
       .trig_watchdog_rst( dcct_plc_wd_timeout ),	// 1: timeout, 0 : normal
       .timeout_cnt(),
       .trig_cnt()
    );
	
		 
		 
    /////////////////////////////////////////
    ///	11/22/2014, PLC watch Dog
    /////////////////////////////////////////
    assign  Cell_PLC_WD_Status[0] = CellPLCWdStatus[0];	//#3
    assign  Cell_PLC_WD_Status[1] = CellPLCWdStatus[1];	//#5
    assign  Cell_PLC_WD_Status[2] = CellPLCWdStatus[3];	//#10
    assign  Cell_PLC_WD_Status[3] = CellPLCWdStatus[4];	//#11
    assign  Cell_PLC_WD_Status[4] = CellPLCWdStatus[5];	//#23
    assign  Cell_PLC_WD_Status[5] = CellPLCWdStatus[7];	//#28
    //
    assign  Cell_PLC_WD_Status[6] = CellPLCWdStatus[8];	//#8
    assign  Cell_PLC_WD_Status[7] = CellPLCWdStatus[9];	//#18

    assign  Cell_PLC_WD_Status[8] = CellPLCWdStatus[10];	//#16
    assign  Cell_PLC_WD_Status[9] = CellPLCWdStatus[11];	//#17
    assign  Cell_PLC_WD_Status[10] = CellPLCWdStatus[13];	//#4
    assign  Cell_PLC_WD_Status[11] = CellPLCWdStatus[15];	//#12

    assign  Cell_PLC_WD_Status[12] = CellPLCWdStatus[17];	//#21
    assign  Cell_PLC_WD_Status[13] = CellPLCWdStatus[18];	//#19
    assign  Cell_PLC_WD_Status[14] = CellPLCWdStatus[20];	//#2
    assign  Cell_PLC_WD_Status[15] = CellPLCWdStatus[21];	//#7
    assign  Cell_PLC_WD_Status[16] = CellPLCWdStatus[22];   //#27 10/14/22
	
	
		 
    genvar plc_ref_i;
    generate
        for (plc_ref_i=0; plc_ref_i < `N_OF_PLC_VALID; plc_ref_i=plc_ref_i+1) begin  : PLC_WD_Create

            ublaze_watchdog		CellPlcWatchdog
            (
                .sys_clk_i(clk),
                .reset(1'b0),
                .UsrClear( plb_w_Reg12[7] ),	//user reset
                .enable(1'b1),
                .EXT_TRIG_B( Cell_PLC_WD_Status[plc_ref_i] ),
                .trig_watchdog_rst( cell_plc_wd_timeout[plc_ref_i] ),	// 1: timeout, 0 : normal
                .timeout_cnt(),
                .trig_cnt()
            );
        end
    endgenerate
		
		
		
    //////////////////////////////////////////
    // 10/08/14
    // BM Position error calculation
    //////////////////////////////////////////
    // 10 kHz divider 5 kHz

    assign	Din_trig = !Qo_5kHz;
    dff __diff(
        .D(Din_trig),
        .clk(Trig1Start),
        .rst(1'b0),
        .Q(Qo_5kHz)
    );


//////////////////////////////////////////////////////////////////////////	

	

    /*******************************************************
     *	BM Glitch detection logic
     *******************************************************
     */
    reg bm_glitch_width;
    reg [15:0] bm_counter_out;

    edge_detect edge_fault_detect (
        .async_sig(bm_pos_glitch_status_1),
        .clk      (clk),
        .rise     (bm_glitch_rise),
        .fall     ()
    );

    always @ (posedge clk)
    begin
        if( GlobalLatchClearingReset == 1'b1 || reset == 1'b1) begin
            bm_pos_glitch_status_1 <= 1'b0;
        end
        else if (Trig1Start == 1'b1) begin	//check status
            if( IL_BM_GlitchLatchedOut == 1'b1 ) begin
                    bm_pos_glitch_status_1 <= 1'b1;
            end
            else begin
                bm_pos_glitch_status_1 <= 1'b0;
            end
        end
    end
	
    always @ (posedge clk)
    begin
        if (bm_glitch_rise || GlobalLatchClearingReset) begin
            bm_counter_out <= #1 16'd0;
            bm_glitch_width <= #1 1'b0;
        end
        else if (bm_pos_glitch_status_1 == 1'b1) begin
            bm_counter_out <= #1 bm_counter_out + 1;
            if( bm_counter_out > (plb_w_reg[15][31:16]+15020) )  begin	// 15020*20ns=300.4 us, cycle + offset cycle(2)
                bm_glitch_width <= #1 1'b1;
            end
        end
        else begin
            bm_counter_out <= bm_counter_out;
        end
     end

    assign ai_bm_trip_src = bm_glitch_width;
	
		
    always @ (posedge clk)
    begin
        if (reset == 1'b1 || GlobalLatchClearingReset == 1'b1 ) begin
            AI_BM_RfTripDebounce5CycleCmdReg <= 1'b0;
        end
        else begin
            if ( ai_bm_trip_src == 1'b1) begin
                AI_BM_RfTripDebounce5CycleCmdReg  <= 1'b1;
            end
            else
                AI_BM_RfTripDebounce5CycleCmdReg  <= AI_BM_RfTripDebounce5CycleCmdReg;
        end
    end
	
			
    /*
     *
     */
    edge_detect ai_id_fault_edge_detect(
        .async_sig(id_pos_glitch_status_1),
        .clk      (clk),
        .rise     (ai_id_glitch_rise),
        .fall     ()
    );

    always @ (posedge clk)
    begin
        if (ai_id_glitch_rise || GlobalLatchClearingReset) begin
            counter_out  <= #1 16'd0;
            glitch_width <= #1 1'b0;
        end
        else if (id_pos_glitch_status_1 == 1'b1) begin
            counter_out <= #1 counter_out + 1;
            if( counter_out > plb_w_reg[15][31:16] ) begin
                glitch_width <= #1 1'b1;
            end
        end
        else begin
            counter_out <= counter_out;
        end
    end

    assign ai_id_trip_src = glitch_width;
		
    always @ (posedge clk)
    begin
        if (reset == 1'b1 || GlobalLatchClearingReset == 1'b1 ) begin
            AI_ID_RfTripDebounceCmdReg <= 1'b0;
        end
        else begin
            if ( ai_id_trip_src == 1'b1) begin
                AI_ID_RfTripDebounceCmdReg  <= 1'b1;
            end
            else
                AI_ID_RfTripDebounceCmdReg  <= AI_ID_RfTripDebounceCmdReg;
        end
    end
	
		
    assign ai_id_dump_status   = ai_id_dump2;
    always @ (posedge clk)
        begin
            if( GlobalLatchClearingReset == 1'b1 || reset == 1'b1) begin
                id_pos_glitch_status_1 <= 1'b0;
            end
        else if ( Trig1Start == 1'b1 ) begin	//check status every 10 kHz
            if( ai_id_dump_status == 1'b1 ) begin
                id_pos_glitch_status_1 <= 1'b1;
            end
            else begin
                id_pos_glitch_status_1 <= 1'b0;
            end
        end
    end
	
	
// NOT USED
//	/* programable delay */
//	stretch	_ai_id_rst_delay
//	(
//		.clk(clk),	
//		.reset(reset),
//		.sig_in(Trig1Start),
//		.len( plb_w_reg[4] ),		// d1000000 = 20 ms
//		.sig_out(trig_width)
//	);
	
//	reg T0RegRst, T0RegRst_reg1, T0RegRst_reg2; 
//	always @ (posedge clk) 
//	begin 
//		T0RegRst <= ~trig_width; 
//		T0RegRst_reg1 <= T0Reg;
//		T0RegRst_reg2 <= T0RegRst_reg1;
//		ai_rst_trigai_rst_trig <= (T0RegRst_reg1 & !T0RegRst_reg2);
//	end
	

    // AI-BM Trip mode selection
    // SR:C31-{AI}BM:CompCalModeSel-SP : default is 01
    assign AI_BPM_RfTripModeSel = (plb_Reg10[10:9] == 2'b00)  ? BM_LATCHED_OUT :           // single point detection
                                  (plb_Reg10[10:9] == 2'b01)  ? AI_BM_ErrorCountTripCmd :  // 3 bpm's out of envelop, OPERATION Mode
                                  (plb_Reg10[10:9] == 2'b10)  ? 0 :	// 2 point Compare
                                                                0;	// 2 point compare and 3 error count

    //assign	AI_BM_RfTripCmd       = (plb_w_reg[15][12] == 1'b1) ? AI_BM_RfTripDebounce5CycleCmdReg : AI_BPM_RfTripModeSel ;
    //01/23/17
    assign AI_BM_RfTripCmd = (plb_w_reg[15][12] == 1'b1) ? (AI_BM_RfTripDebounce5CycleCmdReg && AI_BM_ErrorCountTripCmd) : AI_BPM_RfTripModeSel ;

    /* AI-ID :                                                  more than 2 cycle,      single cycle */
    assign AI_ID_RfTripCmd = (plb_w_reg[15][14] == 1'b1) ? AI_ID_RfTripDebounceCmdReg : ID_DUMP ;

	
	
	
	
	
    /**********************************************************************
    *	count AI-BM BPMs error (BPM ADC saturation, PLL unlock)
    *
    *       _
    *		| |_________________________________	Trig1Start
    *
    *
    *	Logic Test OK with BEAM.  03/23/2015
    *
    **********************************************************************
    */

    always @ (posedge clk)
    begin
        t5khz_0_Reg  <= Qo_5kHz;
        t5khz_0_reg1 <= t5khz_0_Reg;
        t5khz_0_reg2 <= (t5khz_0_Reg & !t5khz_0_reg1);
    end

	
    always @ (posedge clk)
    begin
        if( GlobalLatchClearingReset == 1'b1 ) begin
            bm_bpm_err <= 16'd0;
            bm_bpm_err_status <= 1'b0;
            bm_bpm_err_counts <= 32'd0;
        end
        else if( !BM_ERR_DUMP && t5khz_0_reg2 ) begin 	//every 5 kHz
            bm_bpm_err <= 16'd0;						// clear error value
            //bm_bpm_err_status <= 1'b0;
        end
        else if (xy_Mask_DataValid == 1'b1 ) begin
            if( bpmPos_Xy_Data == 32'd0 ) begin
                bm_bpm_err <= bm_bpm_err + 1;	//if received zero position increment value ?
                bm_bpm_err_counts <= bm_bpm_err_counts + 1;
                bm_bpm_err_status <= 1'b1;
            end
            else begin
                bm_bpm_err <= bm_bpm_err;
                bm_bpm_err_counts <= bm_bpm_err_counts;
                if(bm_bpm_err < plb_w_reg[13][19:10] )
                    bm_bpm_err_status <= 1'b0;
                else
                    bm_bpm_err_status <= bm_bpm_err_status;
            end
        end
    end
	
    always @ (posedge clk)
    begin
        if( GlobalLatchClearingReset == 1'b1 ) begin
            BM_ERR_DUMP <= 0;
        end
        else if( Trig1Start == 1'b1 ) begin	// 10 kHz
            if( (bm_bpm_err > plb_w_reg[13][19:10]) && (dcct_50mA == 1'b1) ) BM_ERR_DUMP <= 1'b1;
            else begin
                BM_ERR_DUMP <= BM_ERR_DUMP;
            end
        end
    end
	
    // 01/20/2017 added
    debouncer #
    (   .DEBNC_CLOCKS (50000), 		 // 20 ns * 50000 is 1 ms
        .PORT_WIDTH   (1)
    ) bm_error_signal(
        .SIGNAL_I(bm_bpm_err_status),
        .CLK_I   (clk),
        .SIGNAL_O(bm_bpm_err_status_filt_out)
    );

    // 2nd debounce
    wire bm_bpm_err_status_filt_out2;
    debouncer #
    (   .DEBNC_CLOCKS ( 50000 ), 		 // 20 ns * 50000 is 1 ms
        .PORT_WIDTH   (1)
    ) bm_error_signal2 (
        .SIGNAL_I(bm_bpm_err_status_filt_out),
        .CLK_I(clk),
        .SIGNAL_O(bm_bpm_err_status_filt_out2)
    );
	
    always @ (posedge clk)
    begin
        if( GlobalLatchClearingReset == 1'b1 ) begin
            Reg_bm_bpm_err_status_filt_out <= 0;
        end
        else if( Trig1Start == 1'b1 ) begin
            if( (bm_bpm_err_status_filt_out2 == 1'b1) && (dcct_50mA == 1'b1) ) Reg_bm_bpm_err_status_filt_out <= 1'b1;
            else begin
                Reg_bm_bpm_err_status_filt_out <= Reg_bm_bpm_err_status_filt_out;
            end
        end
    end
	
	
	
    /*	03/26/15
    *	Read RF trip status for capturing timestamp
    *   NOT USED IN7 externl port
    */
    always @ (posedge clk)
    begin
        //rf_trip_st_T1Reg      <= din_buf[6];	//rf trip readback
        rf_trip_st_T1Reg      <= din_buf[2];	//IN3 : AI2 fast/slow faults 10/16/22
        rf_trip_st_T1Reg_reg1 <= rf_trip_st_T1Reg;
        rf_trip_st_T1Reg_reg2 <= rf_trip_st_T1Reg_reg1;
        rf_trip_st_Trig       <= (rf_trip_st_T1Reg_reg1 & !rf_trip_st_T1Reg_reg2);
    end

    reg ext_trip_TS_status;  //10/16/22 added status register
    always @ (posedge clk ) begin
        if(GlobalLatchClearingReset == 1'b1) begin
            SysMonData[77] <=  TS_time[63:32];
            SysMonData[78] <= 32'd0;
            ext_trip_TS_status <= 1'b0;
        end
        else if(rf_trip_st_Trig == 1'b1 && ext_trip_TS_status == 1'b0) begin		//copy timestamp
            SysMonData[77] <= TS_time[63:32];	//sec
            SysMonData[78] <= TS_time[31:0];	//nsec
            ext_trip_TS_status <= 1'b1;
        end
        else begin
            ext_trip_TS_status   <= ext_trip_TS_status;
        end        
    end

    	
    // DCCT Dump timestamp, 05/15/2015
    reg	dcct_dumpTrig;
    reg dcct_dumpT1Reg, dcct_dumpT1Reg_reg1, dcct_dumpT1Reg_reg2;
    always @ (posedge clk)
    begin
        dcct_dumpT1Reg 		<= dcct_beam_loss || CellPLC_IdBeamDumpOutReg; //10/16/22 added CellPLC_IdBeamDumpOutReg
        dcct_dumpT1Reg_reg1 <= dcct_dumpT1Reg;
        dcct_dumpT1Reg_reg2 <= dcct_dumpT1Reg_reg1;
        dcct_dumpTrig 		<= (dcct_dumpT1Reg_reg1 & !dcct_dumpT1Reg_reg2);
    end

    reg beam_plc_trip_TS_status; //10/16/22
    always @ (posedge clk ) begin
        if(GlobalLatchClearingReset == 1'b1) begin
            SysMonData[70] <= TS_time[63:32];   //sec
            SysMonData[71] <= 32'd0;            //nsec
            beam_plc_trip_TS_status <= 1'b0;
        end
        else if(dcct_dumpTrig == 1'b1 && beam_plc_trip_TS_status == 1'b0) begin	//copy timestamp
            SysMonData[70] <= TS_time[63:32];	//sec
            SysMonData[71] <= TS_time[31:0];	//nsec
            beam_plc_trip_TS_status <= 1'b1;
        end
        else begin
            beam_plc_trip_TS_status   <= beam_plc_trip_TS_status;
        end         
    end
	//////////////////////////////////////////////
	 
//    //11/01/22 added 
//    always @ (posedge clk ) begin 
//        if(reset | GlobalLatchClearingReset == 1'b1) id_glitch_count <= 16'd0;
//        else if(Trig1Start == 1'b1) begin
//            //if(id_glitch_detected == 1'b1) begin
//            //    id_glitch_count <= id_glitch_count+1;
//            //end
//        end            
//    end    


    
	
    // RMS calculation
    //		BmPosAddress     0...179 = x,  180 .. 359 = y
    // NOT USED
    //assign x_orbit_valid = (BmPosAddress >= 8'd0 && BmPosAddress < 8'd180) & xy_Mask_DataValid ? 1'b1 : 1'b0;
    //assign y_orbit_valid = (BmPosAddress >= 8'd180 && BmPosAddress < 8'd360) & xy_Mask_DataValid ? 1'b1 : 1'b0;

	
    //////////////////////////////////////////////////////
		
    IBUFDS fp_in0 (.O( fp_lemo_in[0] ),.I(fp_in_p[0]),.IB(fp_in_n[0]));
    IBUFDS fp_in1 (.O( fp_lemo_in[1] ),.I(fp_in_p[1]),.IB(fp_in_n[1]));
	

	
    ////////////////////////////////////////
    ///////// Chip Scope ///////////////////
    ///////////////////////////////////////

//`define	__AI__CHIPSCOPE_1__
`ifdef	__AI__CHIPSCOPE_1__
    wire [35:0] CONTROL0;
    wire [255:0] trigger0;

    myIcon01 myIcon01_i (
        .CONTROL0(CONTROL0) // INOUT BUS [35:0]
    );
    myIla01 myIla01_i (
        .CONTROL(CONTROL0), // INOUT BUS [35:0]
        .CLK(clk), 			// IN
        .TRIG0(trigger0) // IN BUS [255:0]
    );
        
    assign trigger0[31:0]       = bpmPos_Xy_Data; 
    assign trigger0[63:32]      = SdiOrSimGlobalBpmData;
    assign trigger0[95:64]      = xy_pos_fifo_out; 
    assign trigger0[127:96]     = aie_USET_MASK_SETTING[31:0];
    assign trigger0[135:128]    = bpmPosRdAddr[7:0];
    assign trigger0[136]        = BM_ERR_DUMP;
    assign trigger0[137]        = ID_DUMP;
    assign trigger0[138]        = FINAL_FREE_DUMP_OUT;

    assign trigger0[150:142]    = BmPosAddress[8:0];
    assign trigger0[157:151]    = aie_addr[6:0];
        
    assign trigger0[245]    	= aie_USER_MASK_OUT;
    assign trigger0[246]    	= AIeCalValid;
    assign trigger0[247]    	= AIeCalOutLatch;
    assign trigger0[248]    	= DspMatrixCalStartTrig;
    assign trigger0[249]    	= bpmPosAddrRdValid;
    assign trigger0[250]    	= xy_Mask_DataValid;
    assign trigger0[251]    	= SDI_Reg_WR_ENABLE;
    assign trigger0[252]        = cell_start_addr;
    assign trigger0[253]        = SdiDataRdValid;
    assign trigger0[254]        = aieIl_LatchValid_o;
    //
    assign trigger0[255]        = Trig1Start;	//trigger
`endif	


endmodule

/*
 * 	E N D  O F  T O P
 *	T H A N K  Y O U
 *
 */
