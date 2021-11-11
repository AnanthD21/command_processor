## BreakfastSerial

Command Processor has been implemented using UART as the serial communication protocol on freedom KL25Z board.
UART0 of kl25z has been employed with 24MHz as clock source.

In order to recieve data from terminal window and also to output data to terminal window, two circular buffer
called rxbuffer and txbuffer has been implemented.

![UART_overview](https://user-images.githubusercontent.com/89766346/141261501-53d1938f-611c-4588-8735-d8fce274703c.PNG)

The above picture provides an overview of communication from UART hardware through buffers via sys functions till main.

A sample command line interation has been pasted below:

![command_terminal_screenshot](https://user-images.githubusercontent.com/89766346/141262617-e22452e5-4b64-4694-b657-782981538c88.PNG)
