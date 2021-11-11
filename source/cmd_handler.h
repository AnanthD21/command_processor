/*
 * cmd_handler.h
 *
 *  Created on: 09-Nov-2021
 *      Author: Ananth Deshpande
 */

#ifndef CMD_HANDLER_H_
#define CMD_HANDLER_H_

/**********************************************************************
 * accumulates the input characters fed over UART into a string
 *
 * Parameters:
 *   ipStrPtr --> pointer to input string that gets created
 *   size     --> the maximum size of input buffer
 *
 * Returns:
 *   void
 *********************************************************************/
void accumulateLine(char *ipStrPtr, int size);

/**********************************************************************
 * parses input string to generate tokens
 *
 * Parameters:
 *   ipStrPtr --> pointer to the input string
 *
 * Returns:
 *   void
 *********************************************************************/
void parseInputString(char *ipStrPtr);

#endif /* CMD_HANDLER_H_ */

/**************************************end of file******************************************/
