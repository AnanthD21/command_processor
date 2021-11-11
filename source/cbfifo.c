/*!******************************************************************************************
   File Name           : cbfifo.c
   Author Name         : Ananth Deshpande
                         Professional Masters in Embedded Systems
                         Fall 2021, UCB.
   Author email id     : ande9392@colorado.edu
   IDE used for Coding : MCUXpresso
   Compiler            : GCC
   Date                : 10th November 2021

   Info on Code obtained from other sources:

   In order to refresh my concepts on circular buffers I referred below link:
   https://embeddedartistry.com/blog/2017/05/17/creating-a-circular-buffer-in-c-and-c/

   But NO code has been copied from here or anywhere

*******************************************************************************************/

/*!header files*/
#include <stdio.h>
#include <stdint.h>
#include "cirBuf.h"
#include <cbfifo.h>
#include <MKL25Z4.h>

/*!macros*/
#define MIN(A,B) ((A < B) ? A : B)

/*!global variables*/
cbfifo_t rxCirBuf;
cbfifo_t txCirBuf;

/*****************************************************************
 * initialises the circular buffer elements,
 * it is done only once in any of the cbfifo functions,
 * since we do not know which function can called first
 * time
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   none
 ****************************************************************/
void initCirBuf(cbfifo_t *cirBufPtr)
{
   uint8_t *ptr = cirBufPtr->fifo;
   int i = 0;

   cirBufPtr->writePtr         = ptr;
   cirBufPtr->readPtr          = ptr;
   cirBufPtr->capacity         = FIFOCAPACITY;
   cirBufPtr->length           = 0;
   cirBufPtr->fifoStartPtr     = ptr;

   /*!
      move ptr to point to 127th (starting from 0)
      i.e last element in fifo
   */
   for(i = 1 ; i <= (FIFOCAPACITY - 1); i++)
   {
      ptr++;
   }
   cirBufPtr->fifoendPtr      = ptr;
}

/***************************************************************
 * Update the write ptr of cbfifo, it also takes
 * care of write ptr when fifo reaches its end
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   none
 **************************************************************/
void updateWritePtr(cbfifo_t *cirBufPtr)
{
   if(cirBufPtr->writePtr < cirBufPtr->fifoendPtr)
   {
      cirBufPtr->writePtr++;
   }
   else
   {
      cirBufPtr->writePtr = cirBufPtr->fifoStartPtr;
   }
}

/**************************************************************
 * Update the read ptr of cbfifo, it also takes
 * care of write ptr when fifo reaches its end
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   none
 *************************************************************/
void updateReadPtr(cbfifo_t *cirBufPtr)
{
   if(cirBufPtr->readPtr < cirBufPtr->fifoendPtr)
   {
      cirBufPtr->readPtr++;
   }
   else
   {
      cirBufPtr->readPtr = cirBufPtr->fifoStartPtr;
   }
}

/*********************************************************************
 * Enqueues data onto the FIFO, up to the limit of the available FIFO
 * capacity.
 *
 * Parameters:
 *   buf      Pointer to the data
 *   nbyte    Max number of bytes to enqueue
 *
 * Returns:
 *   The number of bytes actually enqueued, which could be 0. In case
 * of an error, returns -1.
 ********************************************************************/
size_t cbfifo_enqueue(cbfifo_t *cirBufPtr, void *buf, size_t nbyte)
{
   uint32_t maskingState;

   /*
    * To disable interrupts in order to prevent race condition
    *and thus avoid any corruption of data/pointers
    */
   maskingState = __get_PRIMASK();
   __disable_irq();

   if(NULL == buf)
   {
      return -1;
   }

   int i = 0;
   size_t actualBytesWrtn = 0;
   uint8_t *ipBufPtr = (uint8_t *)buf;
   size_t emptySpace = (cirBufPtr->capacity) - (cirBufPtr->length);

   if((0 == nbyte)||(cirBufPtr->length == cirBufPtr->capacity))
   {
      return 0;
   }

   actualBytesWrtn = MIN(emptySpace,nbyte);

   for(i = 0; i < actualBytesWrtn; i++)
   {
      *(cirBufPtr->writePtr) = *ipBufPtr;
      updateWritePtr(cirBufPtr);
      cirBufPtr->length++;
      ipBufPtr++;
   }

   __set_PRIMASK(maskingState);

   return actualBytesWrtn;
}

/**************************************************************************
 * Attempts to remove ("dequeue") up to nbyte bytes of data from the
 * FIFO. Removed data will be copied into the buffer pointed to by buf.
 *
 * Parameters:
 *   buf      Destination for the dequeued data
 *   nbyte    Bytes of data requested
 *
 * Returns:
 *   The number of bytes actually copied, which will be between 0 and
 * nbyte.
 *
 * To further explain the behavior: If the FIFO's current length is 24
 * bytes, and the caller requests 30 bytes, cbfifo_dequeue should
 * return the 24 bytes it has, and the new FIFO length will be 0. If
 * the FIFO is empty (current length is 0 bytes), a request to dequeue
 * any number of bytes will result in a return of 0 from
 * cbfifo_dequeue.
 *************************************************************************/
size_t cbfifo_dequeue(cbfifo_t *cirBufPtr, void *buf, size_t nbyte)
{
   uint32_t maskingState;

   /*
    * To disable interrupts in order to prevent race condition
    *and thus avoid any corruption of data/pointers
    */
   maskingState = __get_PRIMASK();
   __disable_irq();

   if(0 == nbyte)
   {
      return 0;
   }

   if(NULL == buf)
   {
      return -1;
   }

   int i = 0;
   size_t actualBytesRead = 0;
   uint8_t *opBufPtr = (uint8_t *)buf;

   if(0 == cirBufPtr->length)
   {
      return 0;
   }

   actualBytesRead = MIN(cirBufPtr->length,nbyte);

   for(i = 0; i < actualBytesRead; i++)
   {
      *opBufPtr = *(cirBufPtr->readPtr);
      updateReadPtr(cirBufPtr);
      cirBufPtr->length--;
      opBufPtr++;
   }

   __set_PRIMASK(maskingState);

   return actualBytesRead;
}

/*********************************************************************
 * Returns the number of bytes currently on the FIFO.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   Number of bytes currently available to be dequeued from the FIFO
 ********************************************************************/
size_t cbfifo_length(cbfifo_t *cirBufPtr)
{
   size_t length = 0;

   length = cirBufPtr->length;

   return length;
}

/********************************************************************
 * Returns the FIFO's capacity
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   The capacity, in bytes, for the FIFO
 *******************************************************************/
size_t cbfifo_capacity(cbfifo_t *cirBufPtr)
{
	size_t capacity = 0;
	uint32_t maskingState;

	/*
	 * To disable interrupts in order to prevent race condition
	 * and thus avoid any corruption of data/pointers
	 */
	maskingState = __get_PRIMASK();
	__disable_irq();

	capacity = cirBufPtr->capacity;

	__set_PRIMASK(maskingState);

	return capacity;
}

/**************************************end of file******************************************/

