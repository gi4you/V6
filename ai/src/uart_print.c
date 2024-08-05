/*
 *
 *	Module : UART port safe print
 *
 *
 */

#include <xmk.h>
#include "uart_print.h"
#include <pthread.h>
#include "xbasic_types.h"		/* contains basic types for Xilinx software IP */
#include "xuartlite_l.h"
#include "xparameters.h"
//#include "lwip/sockets.h"	/* EINTR */
#include "xuartns550_l.h"

/*
 * Mutex  macro initializes.
 */
static pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;


/*
 *
 */
static int mutexLock(pthread_mutex_t *id)
{
    int status;

    while(1) {
        status = pthread_mutex_lock(id);
      	//if(status!=EINTR) return status;
        //errlogPrintf("pthread_mutex_lock returned EINTR. Violates SUSv3\n");
    }
}


/*
 *
 */
void	uart_mutex_Create()
{
	pthread_mutex_init(&print_mutex, NULL);
}


/*
 *
 */
void	uart_mutex_Lock()
{
	pthread_mutex_lock(&print_mutex);	
}

/*
 *
 */
void	uart_mutex_Ulock()
{
	pthread_mutex_unlock(&print_mutex);
}

/*
 *
 */
void safe_print( char *text ) 
{
int	rv;
	
	rv = pthread_mutex_lock(&print_mutex);
	print(text);
	//print("\r");
	rv = pthread_mutex_unlock(&print_mutex);
}


/*****************************************************************************/
/**
* Retrieve a line of input from the user. A line is defined as all characters
* up to a newline.
*
* @param    Prompt Printed before string is accepted to queue the user to enter
*           something.
* @param    Response Null terminated string with newline stripped
* @param    MaxChars Maximum number of characters to read (excluding null)
*
* @return   Number of characters read (excluding newline)
*
* @note     None
*
******************************************************************************/
int GetUserInput(char* Prompt, char* Response, int MaxChars)
{
    u32 Index;
    u8 Finished;
    u8	key;
    /*
     * Display prompt
     */
    if (Prompt) xil_printf(Prompt);

    /*
     * This basically implements a fgets function. The standalone EDK stdin
     * is apparantly buggy because it behaves differently when newline is
     * entered by itself vs. when it is entered after a number of regular chars
     * have been entered.Characters entered are echoed back.
     */
    Finished = 0;
    Index = 0;

    while(!Finished)
    {
        /*
         * Flush out any output pending in stdout
         */
        fflush(stdout);

        /*
         * Wait for a character to arrive
         */
CHK_KEY:
		//if( XUartLite_IsReceiveEmpty(STDIN_BASEADDRESS) ) {
		if(! XUartNs550_IsReceiveData(STDIN_BASEADDRESS) ) {
          	sleep(10);
           	goto CHK_KEY;
        }

		if(Index > 16) Finished = 1;
        Response[Index] = inbyte();

        /*
         * Normal chars, add them to the string and keep going
         */
        if ((Response[Index] >= 0x20) && (Response[Index] <=0x7E))
        {
            xil_printf("%c", Response[Index++]);
            continue;
        }

        /*
         * Control chars
         */
        switch(Response[Index])
        {
            /*
             * Carriage return
             */
            case 0x0D:
                Response[Index] = 0;
                Finished = 1;
                xil_printf("\r\n");
                break;

            /*
             * Backspace
             */
            case 0x08:
                if (Index != 0)
                {
                    /*
                     * Erase previous character and move cursor back one space
                     */
                    xil_printf("\b \b");
                    Response[--Index] = 0;
                }
                break;

            /*
             * Ignore all other control chars
             */
            default:
                continue;
        }
    }

    return(Index);
}


/*
 *
 */
void	displayBits(Xuint32 value)
{
	Xuint32 i, mask=1 << 31;
	
	xil_printf("0x%7X = ", value);
	for(i=1; i<=32; i++) {
		outbyte(value & mask ? '1' : '0');
		value <<= 1;
		if(i %8 == 0)
			outbyte(' ');
	}
	xil_printf("\r\n");
}

void
delay_1ms(int loop_count)
 {
   int        i;
   for (i = 0; i < loop_count; i++){
     asm volatile ("or  r0, r0, r0\n\
                    or  r0, r0, r0\n\
                    or  r0, r0, r0\n\
                    or  r0, r0, r0\n\
                    or  r0, r0, r0\n\
                    or  r0, r0, r0\n\
                    or  r0, r0, r0\n");

   }
 }

