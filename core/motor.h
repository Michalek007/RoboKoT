#ifndef MOTOR_H
#define MOTOR_H

#include "MKL05Z4.h"

void Motors_Init(void);

void Motors_LeftForward(void);
void Motors_LeftBackward(void);
void Motors_LeftStop(void);

void Motors_RightForward(void);
void Motors_RightBackward(void);
void Motors_RightStop(void);

#endif	// MOTOR_H
