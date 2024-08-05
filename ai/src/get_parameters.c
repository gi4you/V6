/*
 * 
 *	Kiman Ha	kha@bnl.gov
 *	Module : get_parameters.c
 *
 *
 */
#include "xmk.h"
#ifdef __XMK__
  #include <sys/timer.h>
  #include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include "types.h"
#include "xparameters.h"
#include "get_paramters.h"
#include "ssp_eeprom.h"


extern SSP_Config_t SSP_Config;		/* EEPROM */



/**/  
size_t __strlen(char *string)
{
   	char *r1 = string - 1;
   	do {} while (*++r1);
   	return r1 - string;
}

/**/
void *__memcpy(void *s, void *ct, size_t n)
{
   char *from, *to;

   if ( n > 0 )
   {
      from = (char *)ct;  to = (char *)s;
      do *to++ = *from++; while (--n != 0);
   }

   return s;
}


#define BUFLEN 		20
/**/
int __ltoa(long val, char *buffer)
{
    char  tempc[BUFLEN];
    char *bufptr;
    int   neg = val < 0;
    long  uval = neg ? -val : val;
    int   len;

    *(bufptr = &tempc[BUFLEN - 1]) = 0;

    do { *--bufptr = (uval % 10) + '0'; }  while(uval /= 10);
    if (neg) *--bufptr = '-';

    __memcpy(buffer, bufptr, len = (tempc + BUFLEN) - bufptr);
    return len - 1;    /* DON'T COUNT NULL TERMINATION */
}

/**/
/* double to string convertion */
void    __dtos(double num, char *buf, int prec)
{
    int    ival;
    int    pow = 0;
    double max = 0x7FFFFFFF;

    if (num < 0)
    {
        *buf++ = '-';
        num    = -num;
    }

    while (num > max) { num /= 10; pow++; }

    buf += __ltoa(ival = num, buf);

    if (prec > 0) do
    {
       num -= ival;
       if (pow-- == 0) *buf++ = '.';
       *buf++ = (ival = num *= 10) + '0';
    }
    while (--prec);

    *buf = 0;
}


 
 
/*
 *
$EEPROM DATA: 
Version: 12
Serial: 1234
MACAddress: 11-22-33-44-55-66
NetMask: 255.255.255.0
Gateway: 192.168.70.254
IPAddress: 192.168.70.1
Date1: 2010/02/16
Date2: 2010/02/16
Notes: NSLS-2 Digital BPM
#END of FILE
 *
 *
 */
int	SystemInformation(char *data)
{
char *tokenPtr;
char c;
int	index, row, col;
unsigned char	info[13][50];
char	s[20];  
int		total; 	
unsigned int	ver, year, month, day, aa,bb,cc,dd,ee,ff;
char	name[20];


	safe_print("\r\n--------- CONFIG INFO ------------\r\n");
	index = 0;		
   	while(data[index++] != '\r') {}		//find head
   	//xil_printf("found = %d\n", index);
   	col=row=0;                                   
   	tokenPtr = strtok( &data[index], "\r" );
   	while ( tokenPtr != NULL ) 
   	{ 	  	   	
 		strcpy(&info[row][0], tokenPtr);
      	tokenPtr = strtok( NULL, "\r" );
      	if(tokenPtr == NULL) break;
  		
  		row++;
  		if(row > 13) break;
   	}
   	
   	total = row;
  	   	
   	for(index=0; index<row; index++) {
	   	switch (index) {
		   	case 0:		//Version
   				sscanf( &info[index][0], "%s %d", &name, &ver);
   				SSP_Config.version = ver;
				xil_printf("%s  %d\r\n", name, ver );
				break;		   	
		   	case 1:		//Serial
   				sscanf( &info[index][0], "%s %d", &name, &ver);
   				SSP_Config.serial = ver;
				xil_printf("%s  %d\r\n", name, ver );
				break;
		   	case 2:		//MAC
   				sscanf( &info[index][0], "%s %d-%d-%d-%d-%d-%d", &name, &aa, &bb, &cc, &dd, &ee, &ff);
				xil_printf("%s  %d-%d-%d-%d-%d-%d\r\n", name, aa, bb, cc, dd, ee, ff );
				SSP_Config.net_cfg.mac_address[0] = (unsigned char) aa;
				SSP_Config.net_cfg.mac_address[1] = (unsigned char) bb;
				SSP_Config.net_cfg.mac_address[2] = (unsigned char) cc;
				SSP_Config.net_cfg.mac_address[3] = (unsigned char) dd;
				SSP_Config.net_cfg.mac_address[4] = (unsigned char) ee;
				SSP_Config.net_cfg.mac_address[5] = (unsigned char) ff;				
				break;
														
		   	case 3:	//IP
   				sscanf( &info[index][0], "%s %d.%d.%d.%d", &name, &aa, &bb, &cc, &dd);
				xil_printf("%s  %d.%d.%d.%d\r\n", name, aa, bb, cc, dd );
				SSP_Config.net_cfg.ip_address[0] = (unsigned char) aa;
				SSP_Config.net_cfg.ip_address[1] = (unsigned char) bb;
				SSP_Config.net_cfg.ip_address[2] = (unsigned char) cc;
				SSP_Config.net_cfg.ip_address[3] = (unsigned char) dd;
				break;			   	
		   	case 4:	//GW
   				sscanf( &info[index][0], "%s %d.%d.%d.%d", &name, &aa, &bb, &cc, &dd);
				xil_printf("%s  %d.%d.%d.%d\r\n", name, aa, bb, cc, dd );
				SSP_Config.net_cfg.ip_gateway[0] = (unsigned char) aa;
				SSP_Config.net_cfg.ip_gateway[1] = (unsigned char) bb;
				SSP_Config.net_cfg.ip_gateway[2] = (unsigned char) cc;
				SSP_Config.net_cfg.ip_gateway[3] = (unsigned char) dd;
				break;		   	
		   	case 5:	//MASK
   				sscanf( &info[index][0], "%s %d.%d.%d.%d", &name, &aa, &bb, &cc, &dd);
				xil_printf("%s  %d.%d.%d.%d\r\n", name, aa, bb, cc, dd );
				SSP_Config.net_cfg.ip_netmask[0] = (unsigned char) aa;
				SSP_Config.net_cfg.ip_netmask[1] = (unsigned char) bb;
				SSP_Config.net_cfg.ip_netmask[2] = (unsigned char) cc;
				SSP_Config.net_cfg.ip_netmask[3] = (unsigned char) dd;
				break;	
													
		   	case 6:	//Date 1
   				sscanf( &info[index][0], "%s %d/%d/%d", &name, &year, &month, &day);
				xil_printf("%s  %d-%d-%d\r\n", name, year, month, day );
  				SSP_Config.fab_date.year = year;
  				SSP_Config.fab_date.month = month;
  				SSP_Config.fab_date.day = day;				
				break;			   	
		   	case 7:	//Date 2
   				sscanf( &info[index][0], "%s %d/%d/%d", &name, &year, &month, &day);
				xil_printf("%s  %d-%d-%d\r\n", name, year, month, day );
  				SSP_Config.cfg_date.year = year;
  				SSP_Config.cfg_date.month = month;
  				SSP_Config.cfg_date.day = day;				
				break;				
			case 8:	//BPM ID
   				sscanf( &info[index][0], "%s %d", &name, &ver);
   				SSP_Config.SystData.BpmIDs = ver;
				xil_printf("%s  %d\r\n", name, ver );			
				break;
			case 9:	// LOC  
   				sscanf( &info[index][0], "%s %d", &name, &ver);   				
   				SSP_Config.SystData.Location = ver;
				xil_printf("%s  %d\r\n", name, ver );			
				break;
			case 10:	//Cell
   				sscanf( &info[index][0], "%s %d", &name, &ver);
   				SSP_Config.SystData.CellNo = ver;
				xil_printf("%s  %d\r\n", name, ver );			
				break;
			case 11:	//Start Address
				//xil_printf("addr= %s\r\n", &info[index][0]);
   				sscanf( &info[index][0], "%s %x", &name, &ver);
   				SSP_Config.SystData.bootFlashStartAddress = ver;
				xil_printf("%s  0x%x\r\n", name, ver );			
				break;				
			case 12:  	//Notes 				
				xil_printf("%s\r\n", &info[index][0] );
				strcpy(SSP_Config.notes, &info[index][6]);
				break;					
			default:
				break;	
		}
	}	
	if ( EE_WriteConfig( 0, &SSP_Config ) != 0)  {
		xil_printf(("\r\nEE Write ERROR ...\r\n"));
	}	

   	xil_printf("\r\EEPROM Updated...= %d bytes\r\n", total);
   	return (total);
}
