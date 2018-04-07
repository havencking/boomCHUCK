/*
 * Copyright March 2018 - Haven C. King <havencking@gmail.com>
 * 
 * I2C library for nunchuck controller with LPC810 MCU.
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
 
#include "i2c-nunchuck.h"

void i2cPrintStatus()
{
#if I2C_DEBUG
	printf("status: ");
	if (((LPC_I2C->STAT >> 1) & 0x7) == 0x3)
	{
		printf("NACK addr ");
	}
	if (((LPC_I2C->STAT >> 1) & 0x7) == 0x4)
	{
		printf("NACK data ");
	}
	if (((LPC_I2C->STAT >> 1) & 0x7) == 0x0)
	{
		printf("idle ");
	}
	if (((LPC_I2C->STAT >> 1) & 0x7) == 0x1)
	{
		printf("rx rdy ");
	}
	if (((LPC_I2C->STAT >> 1) & 0x7) == 0x2)
	{
		printf("tx rdy ");
	}
	if ((LPC_I2C->STAT >> 24) & 0x1)
	{
		printf("event timeout ");
	}
	if ((LPC_I2C->STAT >> 25) & 0x1)
	{
		printf("SCL timeout ");
	}
	printf("\r\n");
#endif
}

uint8_t nunchuckStartI2C(uint8_t data)
{

	while (i2cIsBusy()) {}
	if (i2cIsIdle())
	{
		i2cWriteData(data);
		i2cStart();
#if I2C_DEBUG
		printf("start: ");
		i2cPrintStatus();
#endif
		while (i2cSlaveNACKed())
		{
			i2cStop();
		}
	}
	else
	{
#if I2C_DEBUG		
		printf("start error: not idle: ");
		i2cPrintStatus();
#endif
		return 0;
	}
return 1;

}	

uint8_t nunchuckContinueTxI2C(uint8_t data)
{
	while (i2cIsBusy()) {}
	if (i2cIsReadyToTx())
	{
		i2cWriteData(data);
		i2cContinue();
#if I2C_DEBUG
		printf("cont: ");
		i2cPrintStatus();
#endif
		while (i2cSlaveNACKed())
		{
			i2cStop();
		}
	}
	else
	{
#if I2C_DEBUG
		printf("cont error: not tx ready: ");
		i2cPrintStatus();
#endif
		return 0;
	}
	return 1;
}

uint8_t nunchuckStopI2C(uint8_t data)
{		
	while (i2cIsBusy()) {}
	i2cStop();
#if I2C_DEBUG
	printf("stop: ");
	i2cPrintStatus();
#endif
	return 1;
}


// initialize Nunchuck I2C using "the new way"
// see: http://wiibrew.org/wiki/Wiimote/Extension_Controllers 
// write 0xa4, 0xf0, 0x55, stop
// write 0xa4, 0xfb, 0x00, stop
// returns 1 for success or 0 for failure
uint8_t nunchuckInit()
{
	
#if I2C_DEBUG
    printf("INIT\r\n");
#endif
	
	if (nunchuckStartI2C(0xa4))
	{
		if (nunchuckContinueTxI2C(0xf0))
		{
			if (nunchuckContinueTxI2C(0x55))
			{
				if (nunchuckStopI2C(0))
				{
					if (nunchuckStartI2C(0xa4))
					{
						if (nunchuckContinueTxI2C(0xfb))
						{
							if (nunchuckContinueTxI2C(0x00))
							{
								if (nunchuckStopI2C(0))
								{
									return 1;
								}
							}
						}
					}
				}
			}
		}
	}
	return 0;
}

// write 0xa4 0x00, stop
// write 0xa5, read 6 bytes
struct nunchuck readNunchuck()
{
#if I2C_DEBUG
	printf("READ\r\n");
#endif

	uint8_t i = 0;
    uint8_t data[6];
    struct nunchuck n;
    
    
	if (nunchuckStartI2C(0xa4))
	{
		if (nunchuckContinueTxI2C(0x00))
		{
			if (nunchuckStopI2C(0))
			{
				if (nunchuckStartI2C(0xa5))
				{
					for (i = 0; i < 6; i++)
					{
						while (i2cIsBusy()) {}
						if (i2cIsReadyToRx())
						{
							i2cContinue();
							data[i] = i2cReadData();
						}
						else
						{
							nunchuckStopI2C(0);
							n.good = 0;
							return n;
						}
					}
				}
				else
				{
					nunchuckStopI2C(0);
					n.good = 0;
					return n;
				}
			}
			else
			{
				nunchuckStopI2C(0);
				n.good = 0;
				return n;
			}
		}
		else
		{
			nunchuckStopI2C(0);
			n.good = 0;
			return n;
		}
	}
	else
	{
		nunchuckStopI2C(0);
		n.good = 0;
		return n;
	}
	
	nunchuckStopI2C(0);
	
	n.sx = data[0];
	n.sy = data[1];
	n.ax = (data[2] << 2) + ((data[5] >> 2) & 0x3);
	n.ay = (data[3] << 2) + ((data[5] >> 4) & 0x3);
	n.az = (data[4] << 2) + ((data[5] >> 6) & 0x3);
	n.bc = (data[5] >> 1) & 0x1;
	n.bz = data[5] & 0x1;
	n.good = 1;
#if I2C_DEBUG
	printf("END READ\r\n");
#endif
	return n;
}

void I2C_IRQHandler() 
{
#if I2C_DEBUG
	uint32_t intstat = LPC_I2C->INTSTAT;

	if (intstat & (0x1 << 0))
	{
		printf("MSTPENDING\r\n");
	}
	if (intstat & (0x1 << 4))
	{
		printf("MSTARBLOSS\r\n");
	}
	if (intstat & (0x1 << 6))
	{
		printf("MSTSTSTPERR\r\n");
	}
	if (intstat & (0x1 << 24))
	{
		printf("EVENTTIMEOUT\r\n");
		i2cStop();
	}
	if (intstat & (0x1 << 25))
	{
		printf("SCLTIMEOUT\r\n");
		i2cStop();
	}
#endif
}

void i2cInit()
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (0x1 << 5); // enable clock for I2C
    LPC_SYSCON->PRESETCTRL &= ~(0x1 << 6); // assert the reset for I2C
    LPC_SYSCON->PRESETCTRL |= (0x1 << 6); // clear the reset for I2C

	// configure physical pins 1 (PIO0_5) & 5 (PIO0_1) for I2C
	// note: LPC810 only supports "pseudo" open drain mode
    LPC_SYSCON->SYSAHBCLKCTRL |= (0x1 << 18); // enable clock for IOCON
	LPC_IOCON->PIO0_5 &= ~(0x3 << 3); // no pull-down or pull-up resistor
	LPC_IOCON->PIO0_5 |= (0x1 << 10); // pseudo open drain mode
	LPC_IOCON->PIO0_1 &= ~(0x3 << 3); // no pull-down or pull-up resistor
	LPC_IOCON->PIO0_1 |= (0x1 << 10); // pseudo open drain mode
	// disable clock for IOCON to save power
	LPC_SYSCON->SYSAHBCLKCTRL &= ~(0x1 << 18);
	
	// configure I2C clock
	LPC_I2C->DIV = 0x0100; // not sure what to use, 0x100 seems to work
	LPC_I2C->MSTTIME = 0x0; // 2 clocks per SCL low time and high time
	
	LPC_I2C->TIMEOUT = 0xFFFF; // I2C function clocks before timeout
	LPC_I2C->INTENSET |= (0x3 << 24); // enable timeout interrupts
	
	LPC_I2C->CFG = 0x9; // enable as I2C master and enable timeout

#if I2C_DEBUG	
	NVIC_EnableIRQ(I2C_IRQn);
#endif
}

uint8_t i2cIsBusy()
{
	return !(LPC_I2C->STAT & 0x1);
}

uint8_t i2cIsIdle()
{
	return (((LPC_I2C->STAT >> 1) & 0x7) == 0x0);
}

uint8_t i2cIsReadyToRx()
{
	return (((LPC_I2C->STAT >> 1) & 0x7) == 0x1);
}

uint8_t i2cIsReadyToTx()
{
	return (((LPC_I2C->STAT >> 1) & 0x7) == 0x2);
}

uint8_t i2cSlaveNACKed()
{
	return (((LPC_I2C->STAT >> 1) & 0x7) == 0x3) || 
		   (((LPC_I2C->STAT >> 1) & 0x7) == 0x4);
}

void i2cWriteData(uint8_t data)
{
	LPC_I2C->MSTDAT = data;
}

uint8_t i2cReadData()
{
	return LPC_I2C->MSTDAT;
}

void i2cContinue()
{
	LPC_I2C->MSTCTL = 0x1;
}

void i2cStart()
{
	LPC_I2C->MSTCTL = 0x2;
}

void i2cStop()
{
	LPC_I2C->MSTCTL = 0x4;
}

