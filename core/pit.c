#include "pit.h"

// private definitions
//typedef enum {CHANNEL0=0, CHANNEL1=1} ChannelNumbers;

// private function definitions
void PIT_IRQHandler(void);

// private memory declarations
static uint8_t pitEnabled = 0;

void PIT_Init_Generator(void) {

	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;	// setting PIT mask in SCGC6 register
	
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;	// clearing MDIS mask in MCR register
	PIT->MCR |= PIT_MCR_FRZ_MASK;	// setting FRZ mask in MCR register
	 
	// channel zero configuration
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(40000*50);	// period->50ms
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;	// generate interrupts
	
	// channel one configuration
	PIT->CHANNEL[1].LDVAL = PIT_LDVAL_TSV(40000*30);	// period->30ms
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK;	// generate interrupts

  NVIC_SetPriority(PIT_IRQn, 2);
	NVIC_ClearPendingIRQ(PIT_IRQn); 
	NVIC_EnableIRQ(PIT_IRQn);
	
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;	// enable counter
	
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;	// enable counter
	
	pitEnabled = 1;

}

void PIT_SetTSV(uint32_t period, uint8_t channelNumber) {
	if (channelNumber == 0 || channelNumber == 1){
			if (pitEnabled) PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(40000*period);	// period in ms
	}
}

void PIT_IRQHandler(void) {
	
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		
		// logic for interrupt
		Servo_Rotate();
		// Servo_RotateOnceHandler();
		
		PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;	// clear status flag
	}
	
	if (PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK) {
		
		// logic for interrupt
		Servo_RotateOnceHandler();
		
		PIT->CHANNEL[1].TFLG |= PIT_TFLG_TIF_MASK;	// clear status flag
	}
}
