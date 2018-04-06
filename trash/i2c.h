/*
 * Copyright March 2018 - havencking@gmail.com
 * 
 * I2C library for SPI0 on LPC810 MCU.
 * Written with a Wii Nunchuck in mind.
 * 
 */

#ifndef _I2C_H_
#define _I2C_H_

#include "LPC8xx.h"

void i2cInit();
uint8_t i2cIsBusy();
uint8_t i2cIsIdle();
uint8_t i2cIsReadyToRx();
uint8_t i2cIsReadyToTx();
void i2cWriteData(uint8_t data);
uint8_t i2cReadData();
void i2cStart();
void i2cContinue();
void i2cStop();

/*void i2cStart(uint8_t address);
void i2cTx(uint8_t data);
uint8_t i2cRx();
void i2cStop();
*/
#endif
