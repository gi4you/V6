#ifndef CONSOLE_CMDS_H_
#define CONSOLE_CMDS_H_

/* debug commands */

int		cmd_EEPROM_IP_Lookup_Setup(int argc, char *argv[]);
int  	cmd_Get_EEPROM_ConfigureFromFile(int argc, char *argv[]);
int		cmd_ConsolReadEEPROM_IP_Info(int argc, char *argv[]);


void	cmd_FlashParmSize();

void	cmd_EpicsCmds(int argc, char *argv[]);

#endif /*CONSOLE_CMDS_H_*/
