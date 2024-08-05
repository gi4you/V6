#ifndef UART_PRINT_H
#define UART_PRINT_H


#include "config.h" // must include xmk.h if we're using xilkernel
#include <stdio.h> // to define xil_printf(), print()
#include "xparameters.h"
#include "xbasic_types.h"

//#define	UART_ADDRESS	XPAR_RS232_UART_1_BASEADDR	/* ML507 */
#define	UART_ADDRESS	XPAR_RS232_BASEADDR		/* SX95 */

#define DEBUG 	1

#if (DEBUG)
#define DPRINTF(x)	xil_printf x
#else
#define DPRINTF(x)
#endif

#define checkStatus(status,message) \
if((status)) { \
    xil_printf("epicsMutex %s failed: error %s\n",(message), strerror((status)));}
    
#define safe_printf(x) 	xil_printf x
#define	printsize(x)	printf ("\r\nsizeof (" #x ")	= %d bytes\r\n", sizeof (x))


extern void	uart_mutex_Lock();
extern void	uart_mutex_Ulock();
extern void safe_print( char *text );

int GetUserInput(char* Prompt, char* Response, int MaxChars);
void displayBits(Xuint32 value);
void delay_1ms(int loop_count);

#endif

