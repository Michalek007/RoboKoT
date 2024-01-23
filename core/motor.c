#include "motor.h"

// private definitions
typedef enum{ A6 = 6, A7= 7, A10 = 10, A12 = 12 } MotorControls;	// A6, A7 -> left motor, A10, A12 -> right motor

// private function definitions
// private memory declarations


void Motors_Init(void){
	SIM->SCGC5 |=  SIM_SCGC5_PORTA_MASK;	// connecting port A to clock
	PORTA->PCR[A6] |= PORT_PCR_MUX(1);	// Pin PTA6 is GPIO
	PORTA->PCR[A7] |= PORT_PCR_MUX(1);	// Pin PTA7 is GPIO
	PORTA->PCR[A10] |= PORT_PCR_MUX(1);	// Pin PTA10 is GPIO
	PORTA->PCR[A12] |= PORT_PCR_MUX(1);	// Pin PTA12 is GPIO
	
	PTA->PDDR |= (1<<A6) | (1<<A7) | (1<<A10) | (1<<A12);	// setting PTA{6, 7, 10, 12} as outputs
	//PTA->PCOR |= (1<<A6) | (1<<A7) | (1<<A10) | (1<<A12);	// clearing PTA{8, 9, 10 ,12}
}

void Motors_LeftForward(void){
	PTA->PCOR |= (1<<A6);
	PTA->PSOR |= (1<<A7);
}

void Motors_LeftBackward(void){
	PTA->PSOR |= (1<<A6);
	PTA->PCOR |= (1<<A7);
}
void Motors_LeftStop(void){
	PTA->PCOR |= (1<<A6) | (1<<A7);
}

void Motors_RightForward(void){
	PTA->PCOR |= (1<<A10);
	PTA->PSOR |= (1<<A12);
}
void Motors_RightBackward(void){
	PTA->PSOR |= (1<<A10);
	PTA->PCOR |= (1<<A12);
}
void Motors_RightStop(void){
	PTA->PCOR |= (1<<A10) | (1<<A12);
}
