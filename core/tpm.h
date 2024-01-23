#ifndef TPM_H
#define TPM_H

#include "frdm_bsp.h"
#include "led.h"

void TPM0_Init_PWM(void);

void TPM0_SetVal(uint32_t value, uint8_t channelNumber);

#endif	// TPM_H
