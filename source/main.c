/*!******************************************************************************************
   File Name           : main.c
   Author Name         : Ananth Deshpande
                         Professional Masters in Embedded Systems
                         Fall 2021, UCB.
   Author email id     : ande9392@colorado.edu
   IDE used for Coding : MCUXpresso
   Compiler            : GCC
   Date                : 10th November 2021
*******************************************************************************************/

/*header files*/
#include "MKL25Z4.h"
#include "sysclock.h"
#include <stdio.h>
#include "uart.h"
#include "cmd_handler.h"
#include "test_cbfifo.h"

/*main function: point of entry*/
int main(void)
{
	int size = 100;
	char ipStr[size];

#ifdef DEBUG
	/*to check health of Tx and Rx circular buffers*/
	test_cbfifo();
#endif
	/*! initialising clock and Uart*/
    sysclock_init();
    initUart();

    printf("Welcome to BreakfastSerial!");

    /*!
     * repeatedly seek for commands, parse and interpret the commands,
     * thus provide respective information to the user.
     */
	while (1)
	{
		printf("\r\n? ");

		accumulateLine(ipStr, size);
		parseInputString(ipStr);
	}

    return 0 ;
}

/**************************************end of file******************************************/
