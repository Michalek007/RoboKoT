#include "controls.h"

// private definitions
// private function definitions
// private memory declarations
static uint8_t controlsState = 0;

void Controls_MoveForward(void){
	Motors_LeftForward();
	Motors_RightForward();
	controlsState = 1;
}
void Controls_MoveBackward(void){
	Motors_LeftBackward();
	Motors_RightBackward();
	controlsState = 1;
}
void Controls_TurnLeft(void){
	Motors_LeftBackward();
	Motors_RightForward();
	controlsState = 1;
}
void Controls_TurnRight(void){
	Motors_LeftForward();
	Motors_RightBackward();
	controlsState = 1;
}
void Controls_Stop(void){
	Motors_LeftStop();
	Motors_RightStop();
	controlsState = 0;
}

uint8_t Controls_GetState(void){
	return controlsState;
}
