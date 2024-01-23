#include "tpm_pcm.h"
#include "music_samples.h"

// private definitions
#define UPSAMPLING 10
//typedef enum {MEOW=1, PURR=2, HISS=3} CatSounds;

// private function definitions
void TPM1_IRQHandler(void);

// private memory declarations
static uint8_t TPM1Enabled = 0;

static uint16_t upSampleCNT = 0;
static uint16_t sampleCNT = 0;
static uint8_t  playFlag = 1;

static uint8_t chosenWave = 1;


void TPM1_Init_PCM(void) {
		
  SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;		
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);    // CLK ~ 41,9MHz (CLOCK_SETUP=0)

	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; 
	PORTB->PCR[5] = PORT_PCR_MUX(2);			// PCM output -> PTB5, TPM1 channel 1
	
	TPM1->SC |= TPM_SC_PS(2);  					  // 41,9MHz / 4 ~ 10,48MHz
	TPM1->SC |= TPM_SC_CMOD(1);					  // internal input clock source

	TPM1->MOD = 255; 										  // 8bit PCM
																				// 10,48MHz / 256 ~ 40,96kHz
	
// "Edge-aligned PWM true pulses" mode -> PCM output
	TPM1->SC &= ~TPM_SC_CPWMS_MASK; 		
	TPM1->CONTROLS[1].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK); 
	TPM1->CONTROLS[1].CnV = 0; 
	
	
// "Output compare" -> for intterrupt
	TPM1->SC &= ~TPM_SC_CPWMS_MASK;
	TPM1->CONTROLS[0].CnSC |= (TPM_CnSC_MSA_MASK | TPM_CnSC_ELSA_MASK);
	TPM1->CONTROLS[0].CnV = 0;
  
	TPM1->CONTROLS[0].CnSC |= TPM_CnSC_CHIE_MASK; // Enable interrupt
	
	NVIC_SetPriority(TPM1_IRQn, 1);

	NVIC_ClearPendingIRQ(TPM1_IRQn); 
	NVIC_EnableIRQ(TPM1_IRQn);
	
	TPM1Enabled = 1;	// setting local flag
}

void TPM1_PCM_Play(uint8_t waveNumber) {
	if (TPM1Enabled){
		if (waveNumber == 1){
		chosenWave = 1;
		}
		if (waveNumber == 2){
			chosenWave = 2;
		}
		if (waveNumber == 3){
			chosenWave = 3;
		}
		sampleCNT = 0;   /* start from the beginning */
		playFlag = 1;	
	}
}

void TPM1_IRQHandler(void) {
	
	if (playFlag) {
		if (chosenWave == 1){
			if (upSampleCNT == 0) TPM1->CONTROLS[1].CnV = wave1[sampleCNT++]; // load new sample
			if (sampleCNT > WAVE1_SAMPLES) {
				playFlag = 0;         // stop if at the end
				TPM1->CONTROLS[1].CnV = 0;
			}
		}
		if (chosenWave == 2){
			if (upSampleCNT == 0) TPM1->CONTROLS[1].CnV = wave2[sampleCNT++]; // load new sample
			if (sampleCNT > WAVE2_SAMPLES) {
				playFlag = 0;         // stop if at the end
				TPM1->CONTROLS[1].CnV = 0;
			}
		}
		if (chosenWave == 3){
			if (upSampleCNT == 0) TPM1->CONTROLS[1].CnV = wave3[sampleCNT++]; // load new sample
			if (sampleCNT > WAVE3_SAMPLES) {
				playFlag = 0;         // stop if at the end
				TPM1->CONTROLS[1].CnV = 0;
			}
		}
		
		// 40,96kHz / 10 ~ 4,1kHz ~ WAVE_RATE
		if (++upSampleCNT > (UPSAMPLING-1)) upSampleCNT = 0;
	}
	
	TPM1->CONTROLS[0].CnSC |= TPM_CnSC_CHF_MASK; 
}
