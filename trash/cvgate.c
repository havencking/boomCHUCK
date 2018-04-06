/*
 * Copyright February 2018 - havencking@gmail.com
 * 
 * CV/gate library for the LPC810 MCU.
 * 
 */

#include "cvgate.h"

void setPitchCV1(uint16_t pitch)
{
    LPC_SPI0->CFG &= (0xFFFFFEFF); // choose DAC1
    spi0Write(pitch | DAC_A_CNTL);
}

void setVelocityCV1(uint16_t velocity)
{
    LPC_SPI0->CFG &= (0xFFFFFEFF); // choose DAC1
    spi0Write(velocity | DAC_B_CNTL);
}

void setPitchCV2(uint16_t pitch)
{
    LPC_SPI0->CFG |= (0x1 << 8); // choose DAC2
    spi0Write(pitch | DAC_A_CNTL);
}

void setVelocityCV2(uint16_t velocity)
{
    LPC_SPI0->CFG |= (0x1 << 8); // choose DAC2
    spi0Write(velocity | DAC_B_CNTL);
}

void setGate1High()
{
	setPIO0_4Low();
}

void setGate1Low()
{
	setPIO0_4High();
}

void setGate2High()
{
	setPIO0_0Low();
}

void setGate2Low()
{
	setPIO0_0High();
}

