#include "touch_sensors.h"

// private definitions
// private function definitions
// private memory declarations

void TouchSensors_Init(void){
	SIM->SCGC5 |=  SIM_SCGC5_PORTB_MASK;	// connecting port B to clock
	PORTB->PCR[B6] |= PORT_PCR_MUX(1);	// Pin PTB7 is GPIO
	PORTB->PCR[B7] |= PORT_PCR_MUX(1);	// Pin PTB10 is GPIO
	PORTB->PCR[B0] |= PORT_PCR_MUX(1);	// Pin PTB11 is GPIO
	
	// enabling rising edge interrupts 
	PORTB->PCR[B6] |= 	PORT_PCR_IRQC(9);	
	PORTB->PCR[B7] |=  PORT_PCR_IRQC(9);
	PORTB->PCR[B0] |=  PORT_PCR_IRQC(9);

	NVIC_ClearPendingIRQ(PORTB_IRQn);	// Clear NVIC any pending interrupts on PORTC_B
	NVIC_EnableIRQ(PORTB_IRQn);	// Enable NVIC interrupts source for PORTC_B module
	
	NVIC_SetPriority (PORTB_IRQn, 1);
}
