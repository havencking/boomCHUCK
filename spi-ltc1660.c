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

#include "spi-ltc1660.h"

void spi0Init()
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (0x1 << 11); // enable clock for SPI0
    LPC_SYSCON->PRESETCTRL &= ~(0x1 << 0); // assert the reset for SPI0
    LPC_SYSCON->PRESETCTRL |= (0x1 << 0); // clear the reset for SPI0

    LPC_SPI0->DIV = 0; // divide master clock by 1
    LPC_SPI0->DLY = 0; // no delay for transmitting data

    LPC_SPI0->CFG = SPI_CFG_MASTER | SPI_CFG_ENABLE;
}

uint8_t spi0IsReadyToTx()
{
	return (uint8_t)((LPC_SPI0->STAT & SPI_STAT_TXRDY) >> 1);
}

void spi0Write(uint16_t data)
{
	LPC_SPI0->TXDATCTL = SPI_TXDATCTL_SSEL & (SPI_TXDATCTL_EOT | \
	                     SPI_TXDATCTL_EOF | SPI_TXDATCTL_RXIGNORE \
					     | SPI_TXDATCTL_LEN | data );
    while(((LPC_SPI0->STAT & SPI_STAT_MSTIDLE) >> 8) == 0) {}
}

// channels 1-8 are valid, and data is 10-bits
void writeDAC(int8_t channel, int16_t data)
{
	spi0Write((channel << 12) | (data << 2));	
}
