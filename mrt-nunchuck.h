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
 
#ifndef __MRT_NUNCHUCK_H 
#define __MRT_NUNCHUCK_H

#define MRT_INT_ENA				(0x1 << 0) // enable
#define MRT_REPEATED_MODE		(0x0 << 1) // repeat mode
#define MRT_ONE_SHOT_INT		(0x1 << 1) // one shot mode
#define MRT_ONE_SHOT_STALL		(0x2 << 1) // bus stall mode
#define MRT_STAT_IRQ_FLAG		(0x1 << 0) // IRQ flag

#define CLOCK_HZ 30000000

void pause(uint32_t ms);
void MRT_IRQHandler(void);
void mrtInit(uint8_t num_channels);

#endif 
