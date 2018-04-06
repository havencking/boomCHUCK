/*
 * Copyright February 2018 - havencking@gmail.com
 * 
 * CV/gate library for the LPC810 MCU.
 * 
 */

#ifndef _CVGATE_H_
#define _CVGATE_H_

#include "LPC8xx.h"
#include "spi.h"

#define DAC_A_CNTL          0x3000
#define DAC_B_CNTL          0xB000

void setPitchCV1(uint16_t pitch);
void setVelocityCV1(uint16_t velocity);

void setPitchCV2(uint16_t pitch);
void setVelocityCV2(uint16_t velocity);

void setGate1High();
void setGate1Low();

void setGate2High();
void setGate2Low();

#endif

