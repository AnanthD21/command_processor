/*!******************************************************************************************
   File Name           : cirBuf.h
   Author Name         : Ananth Deshpande
                         Professional Masters in Embedded Systems
                         Fall 2021, UCB.
   Author email id     : ande9392@colorado.edu
   IDE used for Coding : MCUXpresso
   Compiler            : GCC
   Date                : 10th November 2021

*******************************************************************************************/

#ifndef CIRBUF_H_INCLUDED
#define CIRBUF_H_INCLUDED

#include <stdint.h>

#define FIFOCAPACITY 256

typedef struct cbfifo
{
   uint8_t fifo[FIFOCAPACITY];
   size_t capacity;
   size_t length;
   uint8_t *writePtr;
   uint8_t *readPtr;
   uint8_t *fifoStartPtr;
   uint8_t *fifoendPtr;
}cbfifo_t;

#endif // CIRBUF_H_INCLUDED

/**************************************end of file******************************************/
