/*!******************************************************************************************
   File Name           : cmd_handler.c
   Editor Name         : Ananth Deshpande
                         Professional Masters in Embedded Systems
                         Fall 2021, UCB.
   Author email id     : ande9392@colorado.edu
   IDE used for Coding : MCUXpresso
   Compiler            : GCC
   Date                : 10th November 2021
*******************************************************************************************/
/*header files*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*macros*/
#define MAX_HEXDUMP_SIZE 640

/*!handleCommand function pointer*/
typedef void (*command_handler_t)(int argc, char *argv[]);

/*functions for various commands*/
void handleAuthor(int argc, char *argv[]);
void handleDump(int argc, char *argv[]);
void handleHelp(int argc, char *argv[]);

/*collation of various command parameters into a structure*/
typedef struct
{
	const char* name;
	command_handler_t handler;
	const char* helpString;
}command_table_t;

/*look up table*/
static const command_table_t commands[] =
{
		{"author", handleAuthor, "Provides the name of author"},
		{"dump", handleDump, " <loc> <len> \r\n\t Provides hexdump of memory starting at location loc\r\n"\
				                         "\t Always specify loc in hex (""0xNN""); specify len in"\
		                                 " hex(""0xNN"") or dec (""NN"")"},
		{"help", handleHelp, "Provides help menu"}
};

/*static variable containing number of commands*/
static const int numOfCmds = sizeof(commands)/sizeof(command_table_t);

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
void accumulateLine(char *ipStrPtr, int size)
{
	char ch;
	int i = 0;

	do
	{
		ch = getchar();

		/*!echo the character*/
		printf("%c",ch);

		/*!handle the backspace*/
		if((ch == '\b') && (i > 0))
		{
			printf(" ");
			printf("\b");
			i--;
		}
		else
		{
			/*!to store input characters for further parsing*/
			ipStrPtr[i] = ch;
			i++;
		}

	} while((ch != '\r') && (i < (size - 1)));

	/*append null termination character at the end of string*/
	ipStrPtr[i] = '\0';

	printf("\r\n");
}

/**********************************************************************
 * handle Author command
 *
 * Parameters:
 *   argc --> the number of words in the command
 *   argv --> array of pointers pointing to each word in the command
 *
 * Returns:
 *   void
 *********************************************************************/
void handleAuthor(int argc, char *argv[])
{
	printf("Ananth Deshpande\r\n");
}

/**********************************************************************
 * handle Help command
 *
 * Parameters:
 *   argc --> the number of words in the command
 *   argv --> array of pointers pointing to each word in the command
 *
 * Returns:
 *   void
 *********************************************************************/
void handleHelp(int argc, char* argv[])
{
	for(int i = 0; i < numOfCmds; i++)
	{
		printf("%s \n", commands[i].name);
		printf("%s \r\n", commands[i].helpString);
	}
}

/**********************************************************************
 * Provides the hexdump  of memory from given address location
 *
 * Parameters:
 *	address --> the starting address location
 *	length  --> the number bytes to be dumped
 *
 * Returns:
 *   void
 *********************************************************************/
void hexdump(long address, long length)
{
	long i = 0;
	char *ptr = address;

	/*!
	 * since we must dump only 640 bytes at the max
	 */
	if(length > MAX_HEXDUMP_SIZE)
	{
		length = MAX_HEXDUMP_SIZE;
	}

	/*
	 * In the below loop, we print the address location
	 * at each line of the dump followed by 16 bytes of
	 * data. All in hexadecimal format.
	 */
	for(i = 0; i < length; i++)
	{
		if(0 == (i % 16))
		{
			printf("\r\n%08X ", ptr);
		}

		printf("%02X ",*ptr);
		ptr++;
	}
}

/**********************************************************************
 * handle dump command
 *
 * Parameters:
 *   argc --> the number of words in the command
 *   argv --> array of pointers pointing to each word in the command
 *
 * Returns:
 *   void
 *********************************************************************/
void handleDump(int argc, char *argv[])
{
	char *ptr;
	long startAddr;
	long length;

	/*!
	 * get the start address for hexdump
	 */
	startAddr = strtol(argv[1], &ptr, 16);

	/*
	 * get the length of the hexdump
	 */
	if((argv[2][0] == '0') && (argv[2][1] == 'x'))
	{
		length = strtol(argv[2], &ptr, 16);
	}
	else
	{
		length = strtol(argv[2], &ptr, 10);
	}

	hexdump(startAddr, length);
}

/**********************************************************************
 * compares the input cmd with that in lookup table and calls respective
 * handler function
 *
 * Parameters:
 *   argc --> the number of words in the command
 *   argv --> array of pointers pointing to each word in the command
 *
 * Returns:
 *   void
 *********************************************************************/
void processCommand(int argc, char * argv[])
{
	int i = 0;

	for(i = 0; i < numOfCmds; i++)
	{
		/*execute command devoid of input command case*/
		if(0 == strcasecmp(argv[0],commands[i].name))
		{
			commands[i].handler(argc, argv);
			break;
		}
	}

	/*!
	 * below if condition checks that if the
	 * input command never matched those
	 * in look up table. If so, then informs
	 * user about the "invalid command" provided
	 */
	if(numOfCmds == i)
	{
		printf("Unknown command: %s\r\n", argv[0]);
	}
}

/**********************************************************************
 * parses input string to generate tokens
 *
 * Parameters:
 *   ipStrPtr --> pointer to the input string
 *
 * Returns:
 *   void
 *********************************************************************/
void parseInputString(char *ipStrPtr)
{
	char* currCharPtr = ipStrPtr;
	char* end;

	/*flag to know if current pointer is inside a token/word*/
	bool inToken = false;

	/*flag to obtain start address of each word/token*/
	bool startOfWord = true;

	char *argv[10];
	int argc = 0;

	/*!
	 * goto end of string, since we shall add '\0' to
	 * each word i.e. token
	 */
	for(end = ipStrPtr; *end != '\0'; end++)
	;

	memset(argv, 0, sizeof(argv));

	/*loop to traverse the complete input string*/
	for(currCharPtr = ipStrPtr; currCharPtr != end; currCharPtr++)
	{
		/*to append null for last token*/
		if('\r' == *currCharPtr)
		{
			*currCharPtr = '\0';
			continue;
		}

		/*
		 * if we encounter multiple spaces then we shall only increment
		 * current pointer
		 */
		if((' ' == *currCharPtr) && (false == inToken))
		{
			continue;
		}
		/*fresh out of a word*/
		else if((argc != 0) && (true == inToken) && (' ' == *currCharPtr))
		{
			*currCharPtr = '\0';
			startOfWord = true;
			inToken = false;
		}
		else
		{
			inToken = true;

			/*store pointer to start of each token/word*/
			if(true == startOfWord)
			{
				argv[argc] = currCharPtr;
				argc++;
				startOfWord = false;
			}
		}
	}

	/*if there was'nt any command provided*/
	if(0 == argc)
	{
		return;
	}

	/*further process the input command*/
	processCommand(argc, argv);
}

/**************************************end of file******************************************/
