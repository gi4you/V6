#ifndef __DATA_CONVERTER_H_	/* prevent circular inclusions */
#define __DATA_CONVERTER_H_	/* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

#include "xbasic_types.h"

float   Long2Float( unsigned int *pawSrc, float *padDest, int iDestSize);
float   Hex32_to_Float ( unsigned int pawSrc, float *padDest);
void	WriteFloat2DWordReg(unsigned int RegAddr, float val);

void 	uInt16ToBytes( Xuint16* padSrc, int iSrcSize, Xuint8* pawDest);
void 	uInt32ToBytes( Xuint32* padSrc, int iSrcSize, Xuint8* pawDest);
void 	Float2Bytes( Xfloat32* padSrc, int iSrcSize, Xuint8* pawDest);
void 	Byte2Float( Xuint8* pawSrc, Xfloat32* padDest, int iDestSize);
void 	Float2Word( float* padSrc, int iSrcSize, Xuint16* pawDest, Xuint16 wRealType);
void 	Word2Float( Xuint16* pawSrc, float* padDest, int iDestSize, Xuint16 wRealType);


#ifdef __cplusplus
}
#endif


#endif /* end of protection macro */