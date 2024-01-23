#ifndef CONTROLS_H
#define CONTROLS_H

#include "motor.h"

void Controls_MoveForward(void);

void Controls_MoveBackward(void);

void Controls_TurnLeft(void);

void Controls_TurnRight(void);

void Controls_Stop(void);

// void Controls_TurnAround(void);

uint8_t Controls_GetState(void);

#endif	// CONTROLS_H
