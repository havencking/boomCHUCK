/*
 * Copyright February 2018 - Haven C. King <havencking@gmail.com>
 * 
 * SPI library for SPI0 on LPC810 MCU.
 * Written with sending 16-bits to a 10-bit LTC1660 DAC in mind.
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

#ifndef _SPI_H_
#define _SPI_H_

#include "LPC8xx.h"

#define SPI_CFG_ENABLE			(0x1 << 0)
#define SPI_CFG_MASTER			(0x1 << 2)

#define SPI_STAT_RXRDY			(0x1 << 0)
#define SPI_STAT_TXRDY			(0x1 << 1)
#define SPI_STAT_MSTIDLE 		(0x1 << 8)

#define SPI_TXDATCTL_SSEL	 	(0xFFFEFFFF)
#define SPI_TXDATCTL_EOT 		(0x1 << 20)
#define SPI_TXDATCTL_EOF 		(0x1 << 21)
#define SPI_TXDATCTL_RXIGNORE 	(0x1 << 22)
#define SPI_TXDATCTL_LEN	 	(0xF << 24) // 16 bits

void spi0Init();
uint8_t spi0IsReadyToTx();
void spi0Write(uint16_t data);
void writeDAC(int8_t channel, int16_t data);

#endif
