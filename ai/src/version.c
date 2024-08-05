/*
 *  
 * Module : Version information.
 * 
 */
 
#include <stdio.h>
#include "sys/time.h"

#include "fpga_system.h"
#include "dfe_comm.h"
#include "cmd_modbus.h"
#include "version.h"
#include "fofb.h"


//#define SW_VERSION		0.02212016		//8 digit
//#define SW_VERSION		0.01242017
//
//#define SW_VERSION		0.11082017

//#define SW_VERSION		0.02152019
#define SW_VERSION		0.10212022


/*
 * 
 *  linux 
 *		$date +%s
 */
void NSLS2FOFB_StartUpMessage()
{
	//time_t timestamp;
	
	printf("\n\n***Firmware Compilation Date and Time : %s %s\n",__DATE__, __TIME__) ;
	
	/* time stamp values from http://www.currenttimestamp.com/ */
	//timestamp = (time_t)(1323962975);
	
	/* FPGA Version read */
	ReadFPGA_Ver();

    printf(
	"\r\n\r\n"
	"***************************************************************************\r\n"
	"** NSLS2 Machine Protection System, uBlaze firmware Version %.8f\r\n"
	"**\r\n"	
	"** Built : %s, %s \r\n"
	"**\r\n"	
	"** National Synchrotron Light Source-II (NSLS-2)\r\n"
	"** Brookhaven National Laboratory\r\n"
	"** Upton, New York 11973-5000, USA\r\n"
	"**\r\n"
	"** Copyright (C) 2009-2015 NSLS-2\r\n"
	"**\r\n"
	"***************************************************************************\r\n"
	, SW_VERSION, __DATE__, __TIME__ );

}
