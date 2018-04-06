/*
 * Copyright February 2018 - havencking@gmail.com
 * 
 * GPIO library fo PIO0_1 on the LPC810 MCU.
 * 
 */

#include "gpio.h"

void gpioInit(void)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (0x1 << 6);
    LPC_SYSCON->PRESETCTRL &= ~(0x1 << 10);
    LPC_SYSCON->PRESETCTRL |= (0x1 << 10);
}

void gpioPIO0_0Init()
{
    LPC_GPIO_PORT->DIR0 |= (0x1 << 0); // set PIO0_0 for output
}

void setPIO0_0High()
{
    LPC_GPIO_PORT->SET0 |= (0x1 << 0);
}

void setPIO0_0Low()
{
    LPC_GPIO_PORT->CLR0 |= (0x1 << 0);
}

void gpioPIO0_1Init()
{
    LPC_GPIO_PORT->DIR0 |= (0x1 << 1); // set PIO0_1 for output
}

void setPIO0_1High()
{
    LPC_GPIO_PORT->SET0 |= (0x1 << 1);
}

void setPIO0_1Low()
{
    LPC_GPIO_PORT->CLR0 |= (0x1 << 1);
}

void gpioPIO0_2InitOutput()
{
	LPC_GPIO_PORT->DIR0 |= (0x1 << 2); // set PIO0_2 for output
}

void setPIO0_2High()
{
    LPC_GPIO_PORT->SET0 |= (0x1 << 2);
}

void setPIO0_2Low()
{
    LPC_GPIO_PORT->CLR0 |= (0x1 << 2);
}



void gpioPIO0_3InitInput()
{
	LPC_GPIO_PORT->DIR0 &= ~(0x1 << 3); // set PIO0_3 for input
}

uint8_t getPIO0_3()
{
	return LPC_GPIO_PORT->B0[3];
}




void gpioPIO0_4Init()
{
    LPC_GPIO_PORT->DIR0 |= (0x1 << 4); // set PIO0_4 for output
}

void setPIO0_4High()
{
    LPC_GPIO_PORT->SET0 |= (0x1 << 4);
}

void setPIO0_4Low()
{
    LPC_GPIO_PORT->CLR0 |= (0x1 << 4);
}

void gpioPIO0_5Init()
{
    LPC_GPIO_PORT->DIR0 |= (0x1 << 5); // set PIO0_5 for output
}

void setPIO0_5High()
{
    LPC_GPIO_PORT->SET0 |= (0x1 << 5);
}

void setPIO0_5Low()
{
    LPC_GPIO_PORT->CLR0 |= (0x1 << 5);
}
