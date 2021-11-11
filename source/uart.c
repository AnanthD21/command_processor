/*!******************************************************************************************
   File Name           : uart.c
   Editor Name         : Ananth Deshpande
                         Professional Masters in Embedded Systems
                         Fall 2021, UCB.
   Author email id     : ande9392@colorado.edu
   IDE used for Coding : MCUXpresso
   Compiler            : GCC
   Date                : 10th November 2021
*******************************************************************************************/
/*header files*/
#include <stdint.h>
#include <MKL25Z4.h>
#include "uart.h"
#include "cbfifo.h"
#include "cirBuf.h"

/*Macros*/
#define UART_OVERSAMPLING_RATE (16)
#define SYS_CLOCK (24e6)

#define BAUD_RATE 38400
#define EIGHT_BIT_DATA_SIZE 0
#define NO_PARITY 0
#define TWO_STOP_BITS 1

/*reference to variables outside file*/
extern cbfifo_t rxCirBuf;
extern cbfifo_t txCirBuf;

/**********************************************************************
 * Initialise UART0 of the board with our required baud rate, stop bits
 *
 * Parameters:
 *   baud_rate --> the baud rate for serial communication
 *
 * Returns:
 *   void
 *********************************************************************/
void Init_UART0(uint32_t baud_rate)
{
	uint16_t sbr;
	uint8_t temp;

	/*Enable clock gating for UART0 and Port A*/
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	/*Make sure transmitter and receiver are disabled before init*/
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	/*Set UART clock to FLL and 24 MHz clock */
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK;

	/*Set pins to UART0 Rx and Tx*/
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); /* Rx */
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); /* Tx */

	/*Set baud rate and oversampling ratio*/
	sbr = (uint16_t)((SYS_CLOCK)/(baud_rate * UART_OVERSAMPLING_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLING_RATE-1);

	/*Disable interrupts for RX active edge and LIN break detect, select two stop bit*/
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(TWO_STOP_BITS) | UART0_BDH_LBKDIE(0);

	/* Don't enable loopback mode, use 8 data bit mode, don't use parity*/
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(EIGHT_BIT_DATA_SIZE) | UART0_C1_PE(NO_PARITY);
	/* Don't invert transmit data, don't enable interrupts for errors*/
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
			| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	/*Clear error flags*/
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	/*Try it a different way*/
	UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
									UART0_S1_FE_MASK | UART0_S1_PF_MASK;

	/*Send LSB first, do not invert received data*/
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

	/*Enable interrupts*/
	initCirBuf(&txCirBuf);
	initCirBuf(&rxCirBuf);

	NVIC_SetPriority(UART0_IRQn, 2); /* 0, 1, 2, or 3*/
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	/*Enable receive interrupts but not transmit interrupts yet*/
	UART0->C2 |= UART_C2_RIE(1);

	/*Enable UART receiver and transmitter*/
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

	/*Clear the UART RDRF flag*/
	temp = UART0->D;
	UART0->S1 &= ~UART0_S1_RDRF_MASK;

	(void) temp;
}

/**********************************************************************
 * Initialise UART
 *
 * Parameters:
 *   void
 *
 * Returns:
 *   void
 *********************************************************************/
void initUart()
{
	Init_UART0(BAUD_RATE);
}


/**********************************************************************
 * UART0 interrupt handler
 *
 * Parameters:
 *   void
 *
 * Returns:
 *   void
 *********************************************************************/
void UART0_IRQHandler(void) {
	uint8_t ch = 'A';
	uint8_t retChar;

	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK |
		UART_S1_FE_MASK | UART_S1_PF_MASK)) {
			/* clear the error flags */
			UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
									UART0_S1_FE_MASK | UART0_S1_PF_MASK;
			/* read the data register to clear RDRF */
			ch = UART0->D;
	}
	if (UART0->S1 & UART0_S1_RDRF_MASK) {
		/* received a character */
		ch = UART0->D;

		if(FIFOCAPACITY != cbfifo_length(&rxCirBuf))
		{
			cbfifo_enqueue(&rxCirBuf, &ch, 1);
		}

	}
	if ( (UART0->C2 & UART0_C2_TIE_MASK) && /* transmitter interrupt enabled */
			(UART0->S1 & UART0_S1_TDRE_MASK) ) { /*tx buffer empty*/
		/*can send another character*/
		if(0 != cbfifo_length(&txCirBuf))
		{
			cbfifo_dequeue(&txCirBuf, &retChar, 1);
			UART0->D = retChar;
		}
		else
		{
			/*queue is empty so disable transmitter interrupt*/
			UART0->C2 &= ~UART0_C2_TIE_MASK;
		}
	}
}

/**************************************end of file******************************************/
