## BreakfastSerial

Command Processor has been implemented using UART as the serial communication protocol on freedom KL25Z board.
UART0 is kl25z has been employed with 24MHz as clock source.

In order to recieve data from terminal window and also to output data to terminal window, two circular buffer
called rxbuffer and txbuffer has been implemented.

![UART_overview](https://user-images.githubusercontent.com/89766346/141261501-53d1938f-611c-4588-8735-d8fce274703c.PNG)

The above picture provides an overview of communication from UART hardware through buffers via sys functions till main.

A sample command line interation has been pasted below:
Welcome to BreakfastSerial! 
? author
Ananth Deshpande

? Author
Ananth Deshpande

? DUMP 0 64

00000000 00 30 00 20 D5 00 00 00 43 01 00 00 ED 06 00 00 
00000010 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
00000020 00 00 00 00 00 00 00 00 00 00 00 00 47 01 00 00 
00000030 00 00 00 00 00 00 00 00 49 01 00 00 4B 01 00 00 
? dump a0 0x20

000000A0 0F 02 00 00 17 02 00 00 1F 02 00 00 27 02 00 00 
000000B0 2F 02 00 00 37 02 00 00 3F 02 00 00 47 02 00 00 
? print
Unknown command: print

? help
author 
       Provides the name of author 
dump 
      <loc> <len> 
         Provides hexdump of memory starting at location loc
         Always specify loc in hex (0xNN); specify len in hex(0xNN) or dec (NN) 
help 
     Provides help menu 
