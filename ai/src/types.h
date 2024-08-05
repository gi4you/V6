/*
 *  
 * 
 */
#ifndef __TYPES_H__
#define __TYPES_H__

#ifndef TRUE
# define TRUE                          (1U)
#endif

#ifndef FALSE
# define FALSE                         (0U)
#endif

#include "xbasic_types.h"		/* contains basic types for Xilinx software IP */
//#include "rtwtypes.h"


#if(MATLAB)
/*=======================================================================*
 * Fixed width word size data types:                                     *
 *   int8_T, int16_T, int32_T     - signed 8, 16, or 32 bit integers     *
 *   uint8_T, uint16_T, uint32_T  - unsigned 8, 16, or 32 bit integers   *
 *   real32_T, real64_T           - 32 and 64 bit floating point numbers *
 *                                                                       *
 *                                                                       *
 *   Note:  Because the specified hardware does not have native support  *
 *          for all data sizes, some data types are actually typedef'ed  *
 *          from larger native data sizes.  The following data types are *
 *          not in the ideal native data types:                          *
 *                                                                       *
 *          int8_T, uint8_T                                              *
 *=======================================================================*/
typedef char int8_T;
typedef unsigned char uint8_T;
typedef short int16_T;
typedef unsigned short uint16_T;
typedef long int32_T;
typedef unsigned long uint32_T;
typedef float real32_T;
typedef double real64_T;


#if(XILINX_TYPE)
/** @name Primitive types
 * These primitive types are created for transportability.
 * They are dependent upon the target architecture.
 * @{
 */
typedef unsigned char   Xuint8;     /**< unsigned 8-bit */
typedef char            Xint8;      /**< signed 8-bit */
typedef unsigned short  Xuint16;    /**< unsigned 16-bit */
typedef short           Xint16;     /**< signed 16-bit */
typedef unsigned long   Xuint32;    /**< unsigned 32-bit */
typedef long            Xint32;     /**< signed 32-bit */
typedef float           Xfloat32;   /**< 32-bit floating point */
typedef double          Xfloat64;   /**< 64-bit double precision floating point */
typedef unsigned long   Xboolean;   /**< boolean (XTRUE or XFALSE) */

typedef struct
{
    Xuint32 Upper;
    Xuint32 Lower;
} Xuint64;

#endif	//(XILINX_TYPE)



/*===========================================================================*
 * Generic type definitions: real_T, time_T, boolean_T, char_T, int_T,       *
 *                           uint_T and byte_T.                              *
 *===========================================================================*/
typedef double real_T;
typedef double time_T;
typedef unsigned int boolean_T;
typedef int int_T;
typedef unsigned int uint_T;
typedef char char_T;
typedef char_T byte_T;

#endif	//(AAA)


typedef unsigned int		WORD;
typedef unsigned char    	BYTE;
//typedef int					BOOL;

#define HIBYTE(w)   ((BYTE) (((WORD) (w) >> 8) & 0xFF)) 
#define LOBYTE(w)   ((BYTE) (w)) 
#define HIWORD(l)   ((WORD) (((DWORD) (l) >> 16) & 0xFFFF)) 
#define LOWORD(l)   ((WORD) (l)) 

// Hardware memory access
typedef volatile Xfloat32 	DDR2_FLOAT32_REG;		
typedef volatile Xint32 	DDR2_I32BIT_REG;		// Hardware register definition
typedef volatile Xuint32 	DDR2_UI32BIT_REG;		// Hardware register definition
typedef volatile Xint16 	DDR2_I16BIT_REG;		
typedef volatile Xuint16 	DDR2_UI16BIT_REG;		

//
typedef int             int16;
typedef long            int32;
typedef unsigned int    Uint16;
typedef unsigned long   Uint32;
typedef float           float32;
typedef long double     float64;

#define DDR2_MEM_CAST(a) (a)


#endif
