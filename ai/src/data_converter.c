/*
 *
 *	Module : data_converter.c
 *
 *
 */
#include "xbasic_types.h"
#include "data_converter.h"
#include "pscEpics.h"

#include "cmd_modbus.h"

#define	REAL_NORMAL		1
/*
 * 32-bit Hex data to floating point conversion.
 *	EPICS (float) -> Long   -----> Microblaze (Long to Float)
 */
float    Long2Float( unsigned int *pawSrc, float *padDest, int iDestSize)
{
    int i;
    unsigned int *pwDest;
	unsigned int pwDestBuff[iDestSize+1];
	pwDest = pwDestBuff; 

    for (i=0; i<iDestSize; ++i){
        pwDest = (unsigned int *)(padDest+i);
        //*(pwDest+0) = pawSrc[i];
        *(pwDest+i) = pawSrc[i];
    }        
	return padDest[0];
}

/*
 * simple int to float conversion
	d = Get_uInt32_DDR2(DDR3_RX_REG+4);
	f = *(float *)&d;

*/
/*
 *  http://www.h-schmidt.net/FloatApplet/IEEE754.html
 *	Test passed
 *
 */
float    Hex32_to_Float ( unsigned int pawSrc, float *padDest)
{
    unsigned int *pwDest;
	unsigned int pwDestBuff[1];
	
	pwDest = pwDestBuff; 
    pwDest = (unsigned int *)(padDest+0);
    *(pwDest+0) = pawSrc;
      
	return padDest[0];
}

/*
 *	This function for epics floating point communication 
 *	Microblaze Float -> Long    --------> EPICS Long -> Float
 */
void	WriteFloat2DWordReg(unsigned int RegAddr, float val)
{
Xuint32* pRegValue;

	pRegValue= (Xuint32*)(&val);
	//DFE_CONTROL_REGS->ioReg[RegAddr] = *(pRegValue+0);
	pSrvTxDdrOffAddr->data[RegAddr] = *(pRegValue+0);
	//printf("0x%X ,  %f\r\n", pSrvTxDdrOffAddr->data[RegAddr], val);
}

void	ArrayFloatData2MbusClient(int RegAddr, float *vals, int N)
{
	int	i;
Xuint32* pRegValue;

	pRegValue= (Xuint32*)(&vals[0]);
	
	for(i=0; i<N; i++) {
		DFE_CONTROL_REGS->ioReg[RegAddr+i] = *(pRegValue+0);
		//xil_printf("%X\r\n", DFE_CONTROL_REGS->ioReg[RegAddr+i]);
	}
}



/*
 *	http://www.h-schmidt.net/FloatApplet/IEEE754.html
 *
 *
 */
void Float2Word( float* padSrc, int iSrcSize, Xuint16* pawDest, Xuint16 wRealType)
{
    
	Xuint16* pRegValue; 
	int i;

	for(i=0;i<iSrcSize;i++) {		
		pRegValue= (Xuint16*)(padSrc+i);

		if (REAL_NORMAL== wRealType) { 
			
			pawDest[2*i+0]= *(pRegValue+0); 
			pawDest[2*i+1]= *(pRegValue+1); 
		}
		else {			
			pawDest[2*i+0]= *(pRegValue+1); 
			pawDest[2*i+1]= *(pRegValue+0); 
		}
			
	}		
}

/*
 *	Read 2 words form flash and convertion to FLOAT
 *
 */
void    Word2Float( Xuint16* pawSrc, float* padDest, int iDestSize, Xuint16 wRealType)
{
    int i;
    Xuint16 *pwDest;
        
        for (i=0; i<iDestSize; ++i){
                pwDest = (Xuint16 *)(padDest+i);

                if (REAL_NORMAL==wRealType) {
                        *(pwDest+0)= (WORD)pawSrc[2*i+0];
                        *(pwDest+1)= (WORD)pawSrc[2*i+1];
                }
                else {
                        *(pwDest+0)= (WORD)(pawSrc[2*i+1]);
                        *(pwDest+1)= (WORD)(pawSrc[2*i+0]);                    
                }
                               
        }        
}

/////////////////////////////////////////////////////////////////////////
#if(AAA)
/*
 *
 */
void uInt16ToBytes( Xuint16* padSrc, int iSrcSize, Xuint8* pawDest)
{  
Xuint8* pRegValue; 
int i;

	for(i=0; i<iSrcSize; i++) {		
		pRegValue= (Xuint8*)(padSrc+i);
		//
		pawDest[2*i+0]= *(pRegValue+1); 			
		pawDest[2*i+1]= *(pRegValue+0);
	}		
}

/*
 *
 */
void uInt32ToBytes( Xuint32* padSrc, int iSrcSize, Xuint8* pawDest)
{  
Xuint8* pRegValue; 
int i;

	for(i=0; i<iSrcSize; i++) {		
		pRegValue= (Xuint8*)(padSrc+i);
		//
		pawDest[4*i+0]= *(pRegValue+3); 	//0,1,2,3  
		pawDest[4*i+1]= *(pRegValue+2); 
		pawDest[4*i+2]= *(pRegValue+1); 
		pawDest[4*i+3]= *(pRegValue+0); 			
	}		
}


/*
 *
 */
void Float2Bytes( Xfloat32* padSrc, int iSrcSize, Xuint8* pawDest)
{  
Xuint8* pRegValue; 
int i;

	for(i=0; i<iSrcSize; i++) {		
		pRegValue= (Xuint8*)(padSrc+i);
		//
		pawDest[4*i+0]= *(pRegValue+3); 
		pawDest[4*i+1]= *(pRegValue+2); 
		pawDest[4*i+2]= *(pRegValue+1); 
		pawDest[4*i+3]= *(pRegValue+0); 			
	}		
}


/*
 *	Read 2 words form flash and convertion to FLOAT
 *
 */
void    Byte2Float( Xuint8* pawSrc, Xfloat32* padDest, int iDestSize)
{
int i;
Xuint8 *pwDest;
        
	for (i=0; i<iDestSize; ++i){
    	pwDest = (Xuint8 *)(padDest+i);
        *(pwDest+0)= (Xuint8)(pawSrc[4*i+3]);
        *(pwDest+1)= (Xuint8)(pawSrc[4*i+2]);             
        *(pwDest+2)= (Xuint8)(pawSrc[4*i+1]);
        *(pwDest+3)= (Xuint8)(pawSrc[4*i+0]);                                
	}        
}

void Dbl2Word(const double* padSrc, int iSrcSize,WORD* pawDest, int iDestSize, WORD wRealType){

    
	WORD* pRegValue; 
	int i;

	//ASSERT((4*iSrcSize)==(iDestSize));
	
	//if( (4*iSrcSize) != (iDestSize) ) break; 
	for(i=0;i<iSrcSize;i++) {
		
		pRegValue= (WORD*)(padSrc+i);
		
		if (REAL_NORMAL== wRealType) { 
			
			pawDest[4*i+0]= *(pRegValue+0); 
			pawDest[4*i+1]= *(pRegValue+1); 
			pawDest[4*i+2]= *(pRegValue+2); 
			pawDest[4*i+3]= *(pRegValue+3); 
			
		}
		else {
			
			pawDest[4*i+0]= *(pRegValue+3); 
			pawDest[4*i+1]= *(pRegValue+2); 
			pawDest[4*i+2]= *(pRegValue+1); 
			pawDest[4*i+3]= *(pRegValue+0); 
			
		}
	}
}


void Word2Dbl(const WORD* pawSrc, int iSrcSize,double* padDest, int iDestSize,WORD wRealType){
    int i;
	WORD* pwDest;

	//ASSERT(iSrcSize==(4*iDestSize));

	for (i=0;i<iDestSize;++i){
		pwDest = (WORD*)(padDest+i);
		if (REAL_NORMAL==wRealType) {
			*(pwDest+0)= pawSrc[4*i+0];
			*(pwDest+1)= pawSrc[4*i+1];
			*(pwDest+2)= pawSrc[4*i+2];
			*(pwDest+3)= pawSrc[4*i+3];
		}
		else {
			*(pwDest+0)= pawSrc[4*i+3];
			*(pwDest+1)= pawSrc[4*i+2];
			*(pwDest+2)= pawSrc[4*i+1];
			*(pwDest+3)= pawSrc[4*i+0];
		}
	}
}

#endif
