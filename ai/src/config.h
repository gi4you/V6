#ifndef CONFIG_H
#define CONFIG_H


/* Include xmk.h if running under xilkernel */
#include "xmk.h"
#include "types.h"


#define EMAC_BASEADDR 	XPAR_LLTEMAC_0_BASEADDR

#define	__XMK__		1

/* 
 *
 */
typedef struct {
  unsigned char mac_address[6];
  unsigned char ip_address[4];
  unsigned char ip_netmask[4];
  unsigned char ip_gateway[4];  
} __attribute__((packed)) net0_config_t;

typedef struct {
	unsigned short 	year;
	unsigned char 	month;
	unsigned char 	day;
} __attribute__((packed)) DBPM_date_t;

typedef struct {
	unsigned short 		version;
	unsigned short 		serialNumber;
	unsigned short		Location;		/* Linac, Booster, SR */
  	net0_config_t 	net_cfg;  
  	DBPM_date_t 		fab_date;
  	DBPM_date_t 		cfg_date;
  	char 			notes[120];
} __attribute__((packed)) DBPM_SysConfig_t;


extern DBPM_SysConfig_t DBPM_SysConfig;


#endif

