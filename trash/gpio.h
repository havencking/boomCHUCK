/*
 * Copyright February 2018 - havencking@gmail.com
 * 
 * GPIO library fo PIO0_1 on the LPC810 MCU.
 * 
 */

#ifndef _GPIO_H_
#define _GPIO_H_

#include "LPC8xx.h"

void gpioInit(void);

void gpioPIO0_0Init();
void setPIO0_0High();
void setPIO0_0Low();

void gpioPIO0_1Init();
void setPIO0_1High();
void setPIO0_1Low();

void gpioPIO0_2InitOutput();
void setPIO0_2High();
void setPIO0_2Low();

void gpioPIO0_3InitInput();
uint8_t getPIO0_3();

void gpioPIO0_4Init();
void setPIO0_4High();
void setPIO0_4Low();

void gpioPIO0_5Init();
void setPIO0_5High();
void setPIO0_5Low();

#endif
