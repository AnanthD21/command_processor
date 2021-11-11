/*!******************************************************************************************
   File Name           : sys_read_write.c
   Author Name         : Ananth Deshpande
                         Professional Masters in Embedded Systems
                         Fall 2021, UCB.
   Author email id     : ande9392@colorado.edu
   IDE used for Coding : MCUXpresso
   Compiler            : GCC
   Date                : 10th November 2021
*******************************************************************************************/

/*header files*/
#include "uart.h"
#include "cbfifo.h"
#include "MKL25Z4.h"

/*reference to variables used outside this file*/
extern cbfifo_t rxCirBuf;
extern cbfifo_t txCirBuf;

/**********************************************************************
 * __sys_write writes to the tx buffer
 *
 * Parameters:
 *   handle --> handle to stderr or stdout
 *   buf   --> pointer to buffer from which tx buffer is to bb enqueued
 *   size  --> amount of data to be written to tx buffer
 *
 * Returns:
 *   returns number of bytes written to tx buffer
 *********************************************************************/
int __sys_write(int handle, char *buf, int size)
{
	(void)handle;
	char * bp = buf;

	int retVal = size;

	while(size != 0)
	{
		/*wait for space to open up*/
		while(FIFOCAPACITY == cbfifo_length(&txCirBuf));
		cbfifo_enqueue(&txCirBuf, bp, 1);
		size--;
		bp++;
	}

	/* start transmitter if it isn't already running*/
	if (!(UART0->C2 & UART0_C2_TIE_MASK))
	{
		UART0->C2 |= UART0_C2_TIE(1);
	}

	return retVal;
}

/**********************************************************************
 * __sys_readc reads from rx buffer
 *
 * Parameters:
 *   void
 *
 * Returns:
 *   returns the character dequeued from rx buffer
 *********************************************************************/
int __sys_readc(void)
{
	int c = 0;

	while(0 == cbfifo_length(&rxCirBuf));

	cbfifo_dequeue(&rxCirBuf, &c,1);
	return c;
}

/**************************************end of file******************************************/
