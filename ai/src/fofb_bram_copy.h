/*
 * fofb_bram_copy.h
 *
 *  Created on: Jun 6, 2013
 *      Author: kha
 */

#ifndef FOFB_BRAM_COPY_H_
#define FOFB_BRAM_COPY_H_

#define	V_LEN	540

void	Ref_Init(int nm);
void	VRAM_Init();
void	URAM_Init(int offset);
void	V_P_I_RAM_Init(int RamN);

void	fofb_DDR2URAM_Copy(int a, int trig);
void	fofb_DDR2VRAM_Copy(int a, int trig);

#endif /* FOFB_BRAM_COPY_H_ */
