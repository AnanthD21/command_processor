/*!******************************************************************************************
   File Name           : uart.c
   Author Name         : Ananth Deshpande
                         Professional Masters in Embedded Systems
                         Fall 2021, UCB.
   Author email id     : ande9392@colorado.edu
   IDE used for Coding : MCUXpresso
   Compiler            : GCC
   Date                : 10th November 2021
*******************************************************************************************/

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

/**********************************************************************
 * Initialise UART0 of the board with our required baud rate, stop bits
 *
 * Parameters:
 *   baud_rate --> the baud rate for serial communication
 *
 * Returns:
 *   void
 *********************************************************************/
void initUart();

#endif /* UART_H_ */

/**************************************end of file******************************************/
