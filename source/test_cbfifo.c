/*!******************************************************************************************
   File Name           : test_cbfifo.c
   Author Name         : Ananth Deshpande
                         Professional Masters in Embedded Systems
                         Fall 2021, UCB.
   Author email id     : ande9392@colorado.edu
   IDE used for Coding : MCUXpresso
   Compiler            : GCC
   Date                : 10th November 2021
*******************************************************************************************/

/*!header files*/
#include <stdio.h>
#include "cirBuf.h"
#include "cbfifo.h"
#include <assert.h>
#include <string.h>
#include <assert.h>
#include "test_cbfifo.h"

extern cbfifo_t rxCirBuf;
extern cbfifo_t txCirBuf;

/***************************************************************************
 * tests cbfifos by queueing and dequeuing strings in various orders
 * and checking the expected output via assert function calls
 *
 * Parameters:
 *   none
 *
 * Returns: 1 on success or else assert function informs
 **************************************************************************/
int test_cbfifo_1()
{
   uint8_t buf[20];
   int i = 0;
   size_t bytesWritten = 0;
   size_t bytesRead = 0;

   initCirBuf(&rxCirBuf);
   initCirBuf(&txCirBuf);

   /*!enqueue upto 252 bytes*/
   for(i = 0; i < 42; i++)
   {
      bytesWritten = cbfifo_enqueue(&rxCirBuf,"Rafael",6);
      assert(bytesWritten == 6);
   }

   /*!attempt to enqueue beyond 256 bytes*/
   bytesWritten = cbfifo_enqueue(&rxCirBuf,"Nadal",6);
   assert(bytesWritten == 4);

   /*!dequeue till the last "Rafael"*/
   for(i = 0; i < 41; i++)
   {
      bytesRead = cbfifo_dequeue(&rxCirBuf,buf,6);
      assert(bytesRead == 6);
      assert(buf[0] == 'R');
      assert(buf[1] == 'a');
      assert(buf[2] == 'f');
      assert(buf[3] == 'a');
      assert(buf[4] == 'e');
      assert(buf[5] == 'l');
   }

   /*!checking the last 8 bytes in fifo*/
   bytesRead = cbfifo_dequeue(&rxCirBuf,buf,5);
   assert(bytesRead == 5);
   assert(buf[0] == 'R');
   assert(buf[1] == 'a');
   assert(buf[2] == 'f');
   assert(buf[3] == 'a');
   assert(buf[4] == 'e');

   bytesRead = cbfifo_dequeue(&rxCirBuf,buf,5);
   assert(bytesRead == 5);
   assert(buf[0] == 'l');
   assert(buf[1] == 'N');
   assert(buf[2] == 'a');
   assert(buf[3] == 'd');
   assert(buf[4] == 'a');

   assert(cbfifo_length(&rxCirBuf) == 0);
   assert(cbfifo_capacity(&rxCirBuf) == FIFOCAPACITY);

   return SUCCESS;
}

/***************************************************************************
 * tests cbfifos by queueing and dequeuing strings in various orders at the
 * edge of fifo(near fifostart and fifoend pointers) and checking the
 * expected output via assert function calls
 *
 * Parameters:
 *   none
 *
 * Returns: 1 on success or else assert function informs
 **************************************************************************/
int test_cbfifo_2()
{
   uint8_t buf[300];
   int i = 0;
   size_t bytesWritten = 0;
   size_t bytesRead = 0;

   initCirBuf(&rxCirBuf);
   initCirBuf(&txCirBuf);

   /*!enqueue upto 250 bytes*/
   for(i = 0; i < 50; i++)
   {
      bytesWritten = cbfifo_enqueue(&txCirBuf,"Hello",5);
      assert(bytesWritten == 5);
   }
   assert(cbfifo_length(&txCirBuf) == 250);

   /*!
      dequeue 250 bytes, in order to test near
      end of fifo
   */
   bytesRead = cbfifo_dequeue(&txCirBuf,buf,250);
   assert(bytesRead == 250);
   assert(cbfifo_length(&txCirBuf) == 0);

   /*!
      In the below enqueue write pointer
      rotates back to start of fifo
   */
   bytesWritten = cbfifo_enqueue(&txCirBuf,"CUBoulder",9);
   assert(bytesWritten == 9);

   /*!
      In the below dequeue read pointer
      rotates back to start of fifo
   */
   bytesRead = cbfifo_dequeue(&txCirBuf,buf,5);
   assert(bytesRead == 5);
   assert(buf[0] == 'C');
   assert(buf[1] == 'U');
   assert(buf[2] == 'B');
   assert(buf[3] == 'o');
   assert(buf[4] == 'u');

   assert(cbfifo_length(&txCirBuf) == 4);

   bytesRead = cbfifo_dequeue(&txCirBuf,buf,5);
   assert(bytesRead == 4);
   assert(buf[0] == 'l');
   assert(buf[1] == 'd');
   assert(buf[2] == 'e');
   assert(buf[3] == 'r');

   assert(cbfifo_length(&txCirBuf) == 0);
   assert(cbfifo_capacity(&txCirBuf) == FIFOCAPACITY);

   return SUCCESS;
}

/***************************************************************************
 * tests cbfifo by executing various cbfifo test cases
 *
 * Parameters:
 *   none
 *
 * Returns: 1 on success or assert function informs
 **************************************************************************/
int test_cbfifo()
{
   int retVal1 = 0, retVal2 = 0;

   retVal1 = test_cbfifo_1();
   retVal2 = test_cbfifo_2();

   return (retVal1 && retVal2);
}

/**************************************end of file******************************************/
