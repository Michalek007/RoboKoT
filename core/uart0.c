#include "uart0.h"

// private definitions
// private function definitions
// private memory declarations

void UART0_Init(void)
{
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;	// connecting UART0 to clock
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;	// connecting port B to clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(MCGFLLCLK);	// clock->MCGFLLCLK=41943040Hz

	// setting multiplekser to UART_TX_D & UART_RX_D
	PORTB->PCR[1] = PORT_PCR_MUX(2);	// PTB1->TX_D
	PORTB->PCR[2] = PORT_PCR_MUX(2);	// PTB2->RX_D
	
	UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK );	// block for sender and receiver
	
	// BDH=0, BDL=91
	// OSR=15
	// BUADRATE = clock / (BDH  * OSR + 1) = 28_800 
	UART0->BDH = 0;
	UART0->BDL =91;
	UART0->C4 &= ~UART0_C4_OSR_MASK;
	UART0->C4 |= UART0_C4_OSR(15);
	
	UART0->C5 |= UART0_C5_BOTHEDGE_MASK;	// sampling receiver on both edges
	UART0->C2 |= UART0_C2_RIE_MASK;	// generate interrupts on receive
	
	UART0->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK);	// enable sender and receiver
	
	NVIC_EnableIRQ(UART0_IRQn);
	NVIC_ClearPendingIRQ(UART0_IRQn);
}

void UARTO_Send(uint8_t data) {
	while(!(UART0->S1 & UART0_S1_TDRE_MASK));
	UART0->D = data;
}
