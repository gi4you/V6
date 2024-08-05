/*
 * ddr.h
 *
 *  Created on: Jan 6, 2011
 *      Author: kha
 */

#ifndef DDR_H_
#define DDR_H_

int 	cmd_DDR_CoeffDataRead(int argc, char *argv[]);
void	cmd_Clear_DDR2(int argc, char *argv[]);
int 	cmd_DDR_CellController_DATAs(int argc, char *argv[]);
int 	cmd_DDR_FA_WFM_DATAs(int argc, char *argv[]);
int 	cmd_DDR_CC_WFM_DATAs(int argc, char *argv[]);

#endif /* DDR_H_ */
