#ifndef PIT_H
#define PIT_H

#include "frdm_bsp.h"
#include "servo.h"

void PIT_Init_Generator(void);

void PIT_SetTSV(uint32_t period, uint8_t channelNumber);

#endif	// PIT_H
