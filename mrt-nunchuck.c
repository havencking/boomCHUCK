/*
 * Copyright April 2018 - Haven C. King <havencking@gmail.com>
 * 
 * Multi-Rate Timer library for LPC810.
 *  
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include "LPC8xx.h"
#include "mrt-nunchuck.h"

volatile uint32_t mrt_counter[] = {0, 0, 0, 0};
volatile uint8_t triggered = 0;

uint8_t isTriggered()
{
	return triggered;
}

uint32_t getCount0()
{
	return mrt_counter[0];
}

void setIntervalMRT0(uint16_t time)
{
	LPC_MRT->Channel[0].INTVAL = time * 300;
}

void setIntervalMRT1(uint16_t ms)
{
	LPC_MRT->Channel[1].INTVAL = ms * 30000;
}

void TriggeredOff()
{
	triggered = 0;
}

void pause(uint32_t ms)
{
	mrt_counter[0] = 0;
	LPC_MRT->Channel[0].INTVAL = CLOCK_HZ / 1000;
	LPC_MRT->Channel[0].INTVAL |= 0x1UL<<31;
	while (mrt_counter[0] < ms) {}
}

void MRT_IRQHandler(void)
{  
	if ( LPC_MRT->Channel[0].STAT & MRT_STAT_IRQ_FLAG )
	{
		LPC_MRT->Channel[0].STAT = MRT_STAT_IRQ_FLAG; // clear flag
		mrt_counter[0]++;
	}
	if ( LPC_MRT->Channel[1].STAT & MRT_STAT_IRQ_FLAG )
	{
		LPC_MRT->Channel[1].STAT = MRT_STAT_IRQ_FLAG; // clear flag
		triggered = 1;
	}
/*	if ( LPC_MRT->Channel[2].STAT & MRT_STAT_IRQ_FLAG )
	{
		LPC_MRT->Channel[2].STAT = MRT_STAT_IRQ_FLAG; // clear flag
		mrt_counter[2]++;
	}
	if ( LPC_MRT->Channel[3].STAT & MRT_STAT_IRQ_FLAG )
	{
		LPC_MRT->Channel[3].STAT = MRT_STAT_IRQ_FLAG; // clear flag
		mrt_counter[3]++;
	}*/
	return;
}

// maximum of 4 channels
void mrtInit(uint8_t num_channels) 
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (0x1<<10); // enable clock to MRT
	LPC_SYSCON->PRESETCTRL &= ~(0x1<<7); // assert the reset for MRT
	LPC_SYSCON->PRESETCTRL |= (0x1<<7); // clear the reset for MRT
	
	// enable channels with 1 ms timer in repeating mode
	uint8_t channel;
	for (channel = 0; channel < num_channels; channel++)
	{
		mrt_counter[channel] = 0;
		LPC_MRT->Channel[channel].INTVAL = CLOCK_HZ / 1000;
		LPC_MRT->Channel[channel].INTVAL |= 0x1UL<<31;
		LPC_MRT->Channel[channel].CTRL = MRT_REPEATED_MODE|MRT_INT_ENA;
	}

	NVIC_EnableIRQ(MRT_IRQn);
	return;
}
