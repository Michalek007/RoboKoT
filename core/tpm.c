#include "MKL05Z4.h"
#include "tpm.h"

// private definitions
//typedef enum {CHANNEL2=2, CHANNEL3=3} ChannelNumbers;

// private function definitions
// private memory declarations
static uint8_t tpm0Enabled = 0;


void TPM0_Init_PWM(void) {
		
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;	// setting TPM0 mask in SCGC6 register
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);	// MCGFLLCLK as clock source

	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; // connecting port B to clock
	PORTB->PCR[9] = PORT_PCR_MUX(2);	// setting multiplekser to TMP0 channel 2 for PTB9
	PORTB->PCR[8] = PORT_PCR_MUX(2);	// setting multiplekser to TMP0 channel 3 for PTB9
//	PORTB->PCR[10] = PORT_PCR_MUX(2);	// setting multiplekser to TMP0 channel 0 for PTB10
//	PORTB->PCR[11] = PORT_PCR_MUX(2);	// setting multiplekser to TMP0 channel 1 for PTB11
	
	// clock -> 48 MHz
	// prescaler -> 64
	// mod -> 15_000
	// up counting
	// T = mod * prescaler/zegar ~ 20ms
	
	TPM0->SC |= TPM_SC_PS(6);	// prescaler->128
	TPM0->SC |= TPM_SC_CMOD(1);	// internal input clock source
	TPM0->MOD = 15000;	// modulo->15_000
	TPM0->SC &= ~TPM_SC_CPWMS_MASK;	// up counting

	// channels confugratuion
	
	TPM0->CONTROLS[2].CnSC &= ~TPM_CnSC_ELSA_MASK;
	TPM0->CONTROLS[2].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);	// clear Output on match, set Output on reload
	TPM0->CONTROLS[2].CnV = 750;
	
	TPM0->CONTROLS[3].CnSC &= ~TPM_CnSC_ELSA_MASK;
	TPM0->CONTROLS[3].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK); // clear Output on match, set Output on reload
	TPM0->CONTROLS[3].CnV = 750;
	
//	TPM0->CONTROLS[0].CnSC &= ~TPM_CnSC_ELSA_MASK;
//	TPM0->CONTROLS[0].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);	// clear Output on match, set Output on reload
//	TPM0->CONTROLS[0].CnV = 0;
//	
//	TPM0->CONTROLS[1].CnSC &= ~TPM_CnSC_ELSA_MASK;
//	TPM0->CONTROLS[1].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK); // clear Output on match, set Output on reload
//	TPM0->CONTROLS[1].CnV = 0;
	
	tpm0Enabled = 1;	// set local flag
}


void TPM0_SetVal(uint32_t value, uint8_t channelNumber) { // min -> 0, max -> 100
	// to control servo value should be from 750 to 1500
	value = 7.5 * value + 750;
	if (channelNumber == 2 || channelNumber == 3){
		if (tpm0Enabled) TPM0->CONTROLS[channelNumber].CnV = value;
	}	
}
