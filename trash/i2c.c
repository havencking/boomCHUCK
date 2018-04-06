/*
 * Copyright March 2018 - havencking@gmail.com
 * 
 * I2C Master library for LPC810 MCU.
 * Written with a Wii Nunchuck in mind with pin 3 SDA and pin 4 SCL.
 * 
 */

#include "i2c.h"

void printStatus()
{
	if (((LPC_I2C->STAT >> 1) & 0x7) == 0x3)
	{
		printf ("NACK address\r\n");
	}
	if (((LPC_I2C->STAT >> 1) & 0x7) == 0x4)
	{
		printf ("NACK data\r\n");
	}
	if (((LPC_I2C->STAT >> 1) & 0x7) == 0x0)
	{
		printf ("idle\r\n");
	}
	if (((LPC_I2C->STAT >> 1) & 0x7) == 0x1)
	{
		printf ("Receive ready\r\n");
	}
	if (((LPC_I2C->STAT >> 1) & 0x7) == 0x2)
	{
		printf ("Transmit ready\r\n");
	}
	if (LPC_I2C->STAT >> 25 & 0x1)
	{
		printf ("SCL timeout\r\n");
	}
}

void i2cInit()
{
    // from User Manual chapter 16
    LPC_SYSCON->SYSAHBCLKCTRL |= (0x1 << 5); // enable clock for I2C
    LPC_SYSCON->PRESETCTRL &= ~(0x1 << 6); // assert the reset for I2C
    LPC_SYSCON->PRESETCTRL |= (0x1 << 6); // clear the reset for I2C

    LPC_SYSCON->SYSAHBCLKCTRL |= (0x1 << 18); // enable clock for IOCON

	// pin 3 is PIO0_3, and pin 4 is PIO0_2
	LPC_IOCON->PIO0_3 &= ~(0x3 << 3); // no pull-down or pull-up resistor
	LPC_IOCON->PIO0_3 |= (0x1 << 10); // "not true" open drain mode
	LPC_IOCON->PIO0_2 &= ~(0x3 << 3); // no pull-down or pull-up resistor
	LPC_IOCON->PIO0_2 |= (0x1 << 10); // "not true" open drain mode
	
	//LPC_SYSCON->SYSAHBCLKCTRL &= ~(0x1 << 18); // disable clock for IOCON
	
	LPC_I2C->DIV = 0x0100; // divide PCLK by 2 for I2C - not sure what to use 0x800 seems to work
	LPC_I2C->MSTTIME = 0x0; // 2 clocks per SCL low time and high time
	LPC_I2C->CFG = 0x1; // enable as I2C master
	
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

void i2cWriteData(uint8_t data)
{
	LPC_I2C->MSTDAT = data;
}

uint8_t i2cReadData()
{
	return LPC_I2C->MSTDAT;
}

void i2cStart()
{
	LPC_I2C->MSTCTL = 0x2;
}

void i2cContinue()
{
	LPC_I2C->MSTCTL = 0x1;
}

void i2cStop()
{
	LPC_I2C->MSTCTL = 0x4;
}


//************************

/*void i2cStart(uint8_t address)
{
	while (i2cIsBusy()) {}
	if (i2cIsIdle())
	{
		LPC_I2C->MSTDAT = address;
		LPC_I2C->MSTCTL = 0x2; // start
		printf("i2cStart: ");
		printStatus();
	}
	else
	{
		printf("i2cStart: not idle: ");
		printStatus();
	}
}

void i2cTx(uint8_t data)
{
	while (i2cIsBusy()) {}
	if (i2cIsReadyToTx()) 
	{
		LPC_I2C->MSTDAT = data;
		printf("i2cTx: ");
		printStatus();
		LPC_I2C->MSTCTL = 0x1; // continue
		
	}
	else
	{
		printf("i2cTx: not ready to Tx: ");
		printStatus();
	}
}

uint8_t i2cRx()
{
	uint8_t data = 0xF0;
	while (i2cIsBusy()) {}
	if (i2cIsReadyToRx())
	{
		printf("i2cRx: ");
		printStatus();
		data = LPC_I2C->MSTDAT;
		
		return data;
		
	}
	else
	{
		printf("i2cRx: not ready to Rx: ");
		printStatus();
	}
}

void i2cStop()
{
	printf("i2cStop-before: ");
	printStatus();
	LPC_I2C->MSTCTL = 0x4; // stop	
	printf("i2cStop-after: ");
	printStatus();
}*/


/*

#define I2C_CFG_MSTEN (0x1)

#define I2C_STAT_MSTPENDING (0x1)
#define I2C_STAT_MSTSTATE (0xe)
#define I2C_STAT_MSTST_IDLE (0x0)
#define I2C_STAT_MSTST_RX (0x2)
#define I2C_STAT_MSTST_TX (0x4)

#define I2C_MSTCTL_MSTCONTINUE (0x1)
#define I2C_MSTCTL_MSTSTART (0x2)
#define I2C_MSTCTL_MSTSTOP (0x4)

// Master write one byte to slave. Address 0x23, Data 0xdd. Polling mode.
//LPC_I2C->CFG = I2C_CFG_MSTEN;
while(!(LPC_I2C->STAT & I2C_STAT_MSTPENDING));
if((LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_IDLE) abort();

LPC_I2C->MSTDAT = (0x23 << 1) | 0; // address and 0 for RWn bit
LPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTART; // send start

while(!(LPC_I2C->STAT & I2C_STAT_MSTPENDING));
if((LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MST_TX) abort();
LPC_I2C->MSTDAT = 0xdd; // send data
LPC_I2C->MSTCTL = I2C_MSTCTL_MSTCONTINUE; // continue transaction
while(!(LPC_I2C->STAT & I2C_STAT_MSTPENDING));
if((LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MST_TX) abort();
LPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTOP; // send stop
while(!(LPC_I2C->STAT & I2C_STAT_MSTPENDING));
if((LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_IDLE) abort();


// Master read one byte from slave. Address 0x23. Polling mode. No error checking.
//LPC_I2C->CFG = I2C_CFG_MSTEN;
while(!(LPC_I2C->STAT & I2C_STAT_MSTPENDING));
if((LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_IDLE) abort();

LPC_I2C->MSTDAT = (0x23 << 1) | 1; // address and 1 for RWn bit
LPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTART; // send start

while(!(LPC_I2C->STAT & I2C_STAT_MSTPENDING));
if((LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_RX) abort();
data = LPC_I2C->MSTDAT; // read data
if(data != 0xdd) abort();
LPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTOP; // send stop
while(!(LPC_I2C->STAT & I2C_STAT_MSTPENDING));
if((LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_IDLE) abort();


// Master write one byte to subaddress on slave. Address 0x23, subaddress 0xaa, Data 0xdd.
// Polling mode. No error checking.
//LPC_I2C->CFG = I2C_CFG_MSTEN;
while(!(LPC_I2C->STAT & I2C_STAT_MSTPENDING));
if((LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_IDLE) abort();
LPC_I2C->MSTDAT = (0x23 << 1) | 0; // address and 0 for RWn bit in order to write
// subaddress
LPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTART; // send start
while(!(LPC_I2C->STAT & I2C_STAT_MSTPENDING));
if((LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTSTX) abort();
LPC_I2C->MSTDAT = 0xaa; // send subaddress
LPC_I2C->MSTCTL = I2C_MSTCTL_MSTCONTINUE; // continue transaction
while(!(LPC_I2C->STAT & I2C_STAT_MSTPENDING));
if((LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTSTX) abort();
LPC_I2C->MSTDAT = 0xdd; // send data
LPC_I2C->MSTCTL = I2C_MSTCTL_MSTCONTINUE; // continue transaction
while(!(LPC_I2C->STAT & I2C_STAT_MSTPENDING));
if((LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTSTX) abort();
LPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTOP; // send stop
while(!(LPC_I2C->STAT & I2C_STAT_MSTPENDING));
if((LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_IDLE) abort();
*/
