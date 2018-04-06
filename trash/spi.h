/*
 * Copyright February 2018 - havencking@gmail.com
 * 
 * SPI library for SPI0 on LPC810 MCU.
 * Written with sending 16-bits to a 12-bit MCP4922 DAC in mind.
 * 
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
void spi0WriteDAC1(uint16_t data);
void spi0WriteDAC2(uint16_t data);

#endif
