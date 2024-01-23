#ifndef UART0_H
#define UART0_H

#include "MKL05Z4.h"

#define CLK_DIS 					0x00
#define MCGFLLCLK 				0x01
#define OSCERCLK					0x02
#define MCGIRCLK					0x03

void UART0_Init(void);

void UARTO_Send(uint8_t data);

#endif	// UART0_H
