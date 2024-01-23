#include "servo.h"

// private definitions
typedef enum {LEFT=0, RIGHT=1} Direction;

// private function definitions
// private memory declarations
static uint8_t tiltValue = 0;  // min -> 0, max -> 100
static Direction direction = LEFT;

static uint8_t tiltValueRO = 0;  // min -> 0, max -> 100
static Direction directionRO = LEFT;
static uint8_t rotateOnceFlag = 0;

void Servo_Rotate(void){
	
	if (direction == LEFT){
		tiltValue += 1;
		if (tiltValue == 100){
			direction = RIGHT;
		}
	}
	else{
		tiltValue -= 1;
		if (tiltValue == 0){
			direction = LEFT;
		}
	}
	TPM0_SetVal(tiltValue, 2);
}

void Servo_RotateOnce(void){
	if (!rotateOnceFlag){
		rotateOnceFlag = 1;
		tiltValueRO = 0;
		directionRO = LEFT;
	}
}

void Servo_RotateOnceHandler(void){
	if (rotateOnceFlag){
		if (directionRO == LEFT){
			tiltValueRO += 1;
			if (tiltValueRO == 100){
				directionRO = RIGHT;
			}
		}
		else{
			tiltValueRO -= 1;
			if (tiltValueRO == 0){
				rotateOnceFlag = 0;
				directionRO = LEFT;
			}
		}
		TPM0_SetVal(tiltValueRO, 3);
	}
}
