/*
 *	
 *	Module : EEPROM control head file
 *
 */
#ifndef SSP_EEPROM_H_
#define SSP_EEPROM_H_

//#include "xcope.h"
#include "types.h"


extern int EE_Init(void);
extern int EE_Read( unsigned addr, unsigned char *rbuf, int n_bytes );
extern int EE_Write( unsigned addr, unsigned char *rbuf, int n_bytes );


typedef struct {
	unsigned short n_bytes;
	unsigned short checksum;
} __attribute__((packed)) SSP_Config_hdr_t;

typedef struct {
  	unsigned char mac_address[6];
  	unsigned char ip_address[4];
  	unsigned char ip_netmask[4];
  	unsigned char ip_gateway[4];  
} __attribute__((packed)) net_config_t;

typedef struct {
	unsigned short year;
	unsigned char month;
	unsigned char day;
} __attribute__((packed)) SSP_date_t;

/* System data */
typedef struct {
	unsigned int		BpmIDs;
	unsigned char		Location;		/* LINAC, BOOSTER, SR */
	unsigned char		CellNo;			/* 1 ~ 32 */
	unsigned long		bootFlashStartAddress;
} __attribute__((packed)) System_date_t;


#define N_CFG_OPTS 				25
typedef struct {
  	SSP_Config_hdr_t 	hdr;		/* head information */
  	unsigned short 		version;
  	net_config_t 		net_cfg;
  	unsigned short 		serial;
  	SSP_date_t 			fab_date;
  	SSP_date_t 			cfg_date;  	
  	System_date_t		SystData;
  	char 				notes[35];  	
} __attribute__((packed)) SSP_Config_t;

extern int EE_ReadConfig( int addr, SSP_Config_t *cfg );
extern int EE_WriteConfig( int addr, SSP_Config_t *cfg );
extern void EE_print_config( SSP_Config_t *cfg, char *heading );
/**/
unsigned short fletcher16( unsigned char *data, size_t len );


extern SSP_Config_t SSP_Config;

#endif

