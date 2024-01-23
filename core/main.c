#include "frdm_bsp.h" 
#include "led.h" 
#include "lcd1602.h" 
#include "tsi.h"
#include "pit.h"
#include "uart0.h"
#include "controls.h"
#include "tpm_pcm.h"
#include "tpm.h"
#include "touch_sensors.h"

// private definitions
typedef enum{ ANGRY=0, PEACEFUL=1, PLEASED=2} CatState;

// private function declarations
void SysTick_Handler(void);
void LCD_Default(void);
void LCD_Update(void);
void UART0_IRQHandler(void);

void Cat_AngryActions(void);
void Cat_PeacfulActions(void);
void Cat_PleasedActions(void);


// private memory declarations
static uint8_t sliderVal = 50;
static uint8_t sliderOld = 0;

static uint8_t msTicks = 0;
static uint8_t newTick = 0;

static uint8_t catGoodTouchCounter = 0;
static uint8_t catBadTouchCounter = 0;

static uint8_t catDelayTicks = 0;
static uint8_t motorDelayTicks = 0;

static char rxBuf[] = {0};
static char rxTemp;
static uint8_t rxFull = 0;
static uint8_t action = 0; 

static CatState catState = PEACEFUL;


int main (void) { 
	
	uint8_t sliderTemp;
	
	Motors_Init();	// motors initialization
	
	TouchSensors_Init();	// touch sensors initialization
	
	TSI_Init ();	// slider initialization
	
	LED_Init ();	// all LEDs initialization
	LED_Welcome();	//blink with all LEDs

	LCD1602_Init();	// LCD initialization
	LCD_Default();	//display default on LCD
	
	UART0_Init();	// UART initialization
	
	PIT_Init_Generator ();	// PIT initialization

	SysTick_Config(1000000);	// system timer initialization

	TPM0_Init_PWM();  // TPM0 as PWM initialization
	
  TPM1_Init_PCM ();	// TPM1 as PCM initialization

  while(1) {

		__WFI();	// sleep & wait for interrupt

		if (newTick) {
			newTick = 0;	// clear flag & choose task
			
			if( msTicks%2 == 0 ) {
				sliderTemp = TSI_ReadSlider();	// reading value from slider min->1, max->98
				if (sliderTemp > 0) {
					sliderVal = sliderTemp;
				
					if (sliderVal < 25 || sliderVal > 75){
//						catState = PLEASED;
//						Controls_MoveForward();
//						Cat_PleasedActions();
						
						if (sliderVal != sliderOld) {
								sliderOld = sliderVal;
								catGoodTouchCounter++;
						}
					}
					else{
//						catState = ANGRY;
//						Controls_MoveBackward();
//						Cat_AngryActions();
					
						if (sliderVal != sliderOld) {
								sliderOld = sliderVal;
								catBadTouchCounter++;
						}
					}
				}
				if (catBadTouchCounter > 5){
					catState = ANGRY;
					Controls_MoveBackward();
					Cat_AngryActions();
					catBadTouchCounter = 0;
					catGoodTouchCounter = 0;
				}
				if (catGoodTouchCounter > 5){
					catState = PLEASED;
					Controls_MoveForward();
					Cat_PleasedActions();
					catBadTouchCounter = 0;
					catGoodTouchCounter = 0;
				}
				
				if(rxFull)		// if buffer is full
					{
						action = rxBuf[0] - 48;	// convertion from char to int
						if (action == 1){
								Controls_MoveForward();
							}
						else if (action == 2){
							Controls_MoveBackward();
						}
						else if (action == 3){
							Controls_TurnRight();
						}
						else if (action == 4){
							Controls_TurnLeft();
						}
						else if (action == 5){
							TPM1_PCM_Play(2);
						}
						else if (action == 6){
							TPM1_PCM_Play(1);
						}
						else if (action == 7){
							TPM1_PCM_Play(3);
						}
						else if (action == 8){
							Servo_RotateOnce();
						}
						else{
						}
						
						rxFull=0;	// data used
					}
			}	// msTicks % 2
			if( msTicks%5 == 0) {
				LCD_Update();
			}	// msTicks % 5
			
//			if (msTicks%100 == 0){
//				catBadTouchCounter = 0;
//				catGoodTouchCounter = 0;
//			} // msTicks % 100
			
			if (catDelayTicks >= 150){
					catState = PEACEFUL;
					Cat_PeacfulActions();
					// touchCounter = 0;
					catDelayTicks = 0;
				}
	
			if (motorDelayTicks >= 20){
					motorDelayTicks = 0;
					Controls_Stop();
				}
		} // end new tick actions
	} // end_while
} // end main

void SysTick_Handler(void) {
	msTicks++;
	newTick = 1;
	
	if (catState == ANGRY || catState == PLEASED){
		catDelayTicks++;
	}
	if (Controls_GetState() == 1){
		motorDelayTicks++;
	}
}

void UART0_IRQHandler()
{
	if(UART0->S1 & UART0_S1_RDRF_MASK)
	{
		rxTemp=UART0->D;	// reading data from UART buffer and clearing RDRF flag
		if(!rxFull)  // if our buffer not full
		{
			rxBuf[0] = rxTemp;	// saving data in buffer	
			rxFull=1;
		}
	}
	NVIC_EnableIRQ(UART0_IRQn);
}

void PORTB_IRQHandler(void){

	if( PORTB->ISFR & (1 << B0) ){
		catState = ANGRY;
		Controls_MoveForward();
		Cat_AngryActions();

		PORTB->PCR[B0] |= PORT_PCR_ISF_MASK;	// clearing ISF flag
  }
	
	if( PORTB->ISFR & (1 << B6) ){
		catState = ANGRY;
		Controls_TurnRight();
		Cat_AngryActions();

		PORTB->PCR[B6] |= PORT_PCR_ISF_MASK;	// clearing ISF flag
  }
	
	if( PORTB->ISFR & (1 << B7) ){
		catState = ANGRY;
		Controls_TurnLeft();
		Cat_AngryActions();

		PORTB->PCR[B7] |= PORT_PCR_ISF_MASK;	// clearing ISF flag
  }
}

void LCD_Default(void) {
	LCD1602_ClearAll();
	LCD1602_SetCursor(0, 0);
	LCD1602_Print("  ( o ___ o )");
}

void LCD_Update(void) {
	
	LCD1602_SetCursor(0, 0);
	if (catState == ANGRY){
		LCD1602_Print("  ( O/ M /O )");
		//LCD1602_Print("     ANGRY");
	}
	else if (catState == PEACEFUL){
		LCD1602_Print("  ( o ___ o )"); 
		//LCD1602_Print("   PEACFUL");
	}
	else{
		LCD1602_Print("   ( U w U )  ");
		//LCD1602_Print("   PLEASED");
	}
}

void Cat_AngryActions(void){
	PIT_SetTSV(10, 0);
	TPM1_PCM_Play(3);
}

void Cat_PeacfulActions(void){
	PIT_SetTSV(30, 0);
	TPM1_PCM_Play(2);
	Servo_RotateOnce();
}

void Cat_PleasedActions(void){
	PIT_SetTSV(20, 0);
	TPM1_PCM_Play(1);
}
