/*
 * Copyright February 2018 - Haven C. King <havencking@gmail.com>
 * 
 * Asynchronous Serial library for UART0 and UART1 on LPC810 MCU.
 * Written with receiving MIDI data in mind.
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
 
#ifndef _UART_H_
#define _UART_H_

#include "LPC8xx.h"

#define UART_CFG_ENABLE			(0x1 << 0)
#define UART_CFG_DATALEN(d)		(((d)-7) << 2)
#define UART_CFG_NO_PARITY		(0x0 << 4)
#define UART_CFG_STOPLEN		(0x0 << 6)
#define UART_CFG_NO_FLOW_CTL	(0x0 << 9)
#define UART_CFG_ASYNC			(0x0 << 11)
#define UART_CFG_CLKPOL			(0x0 << 12)
#define UART_STAT_RXRDY			(0x1 << 0)
#define UART_STAT_RXIDLE		(0x1 << 1)
#define UART_STAT_TXRDY			(0x1 << 2)
#define UART_STAT_TXIDLE		(0x1 << 3)

void uart0Init(uint32_t baud);
uint8_t uart0IsReadyToRx();
uint8_t uart0ReadByte();
void uart1Init(uint32_t baud);
uint8_t uart1IsReadyToRx();
uint8_t uart1ReadByte();

#endif
