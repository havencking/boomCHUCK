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

#ifndef _I2C_NUNCHUCK_H_
#define _I2C_NUNCHUCK_H_

#include "LPC8xx.h"

#define I2C_DEBUG	1

#define START 		0
#define CONTINUE_TX	1
#define STOP		2

struct nunchuck
{
	uint8_t sx;
	uint8_t sy;
	uint16_t ax;
	uint16_t ay;
	uint16_t az;
	uint8_t bc;
	uint8_t bz;
	uint8_t good;	
};

void i2cPrintStatus();
uint8_t nunchuckStartI2C(uint8_t data);
uint8_t nunchuckContinueTxI2C(uint8_t data);
uint8_t nunchuckStopI2C(uint8_t data);
uint8_t nunchuckInit();
struct nunchuck readNunchuck();
void i2c_IRQHandler();
void i2cInit();
uint8_t i2cIsBusy();
uint8_t i2cIsIdle();
uint8_t i2cIsReadyToRx();
uint8_t i2cIsReadyToTx();
uint8_t i2cSlaveNACKed();
void i2cWriteData(uint8_t data);
uint8_t i2cReadData();
void i2cContinue();
void i2cStart();
void i2cStop();

#endif

