/*
 * Copyright February 2018 - havencking@gmail.com
 * 
 * SPI library for SPI0 on LPC810 MCU.
 * Written with sending 16-bits to a 12-bit MCP4922 DAC in mind.
 * 
 */

#include "spi.h"

void spi0Init()
{
    // from User Manual chapter 17
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

