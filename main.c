/*
 * Copyright March 2018 - Haven C. King <havencking@gmail.com>
 * 
 * boomCHUCK!
 * 
 * for LPC810 MCU, LTC1660 SPI DAC, and nunchuck I2C controller
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
 
#include <stdio.h>
#include "LPC8xx.h"
#include "i2c-nunchuck.h"
#include "spi-ltc1660.h"
#include "mrt-nunchuck.h"

#if I2C_DEBUG
#include "uart.h"
#endif

#define LEFT_NUNCHUCK 1 // zero for right, one for left
#define PRESSED 0

// pin 1: I2C0_SDA
// pin 2: U0_TXD
// pin 3: SPI0_SCK
// pin 4: SPI0_SSEL
// pin 5: I2C0_SCL
// pin 6: VDD
// pin 7: VSS
// pin 8: SPI0_MOSI
void SwitchMatrix_Init()
{
       /* Pin Assign 8 bit Configuration */
       /* U0_TXD */
       LPC_SWM->PINASSIGN[0] = 0xffffff04UL;
       /* SPI0_SCK */
       LPC_SWM->PINASSIGN[3] = 0x03ffffffUL;
       /* SPI0_MOSI */
       /* SPI0_SSEL */
       LPC_SWM->PINASSIGN[4] = 0xff02ff00UL;
       /* I2C0_SDA */
       LPC_SWM->PINASSIGN[7] = 0x05ffffffUL;
       /* I2C0_SCL */
       LPC_SWM->PINASSIGN[8] = 0xffffff01UL;

       /* Pin Assign 1 bit Configuration */
       LPC_SWM->PINENABLE0 = 0xffffffffUL;
}

int main(void)
{
	SwitchMatrix_Init();

#if I2C_DEBUG
	uart0Init(115200);
#endif

    i2cInit(); // pins 1 and 5 configured for open-drain
    spi0Init();
    mrtInit(1);
   
    pause(1000); // need a short delay for I2C and SPI hardware
	
	// initialize DAC 
    writeDAC(1,698);
	writeDAC(2,698);
	writeDAC(3,698);
	writeDAC(4,698);
	writeDAC(5,698);
	writeDAC(6,698);
	writeDAC(7,0);
	writeDAC(8,0);
	
	// initialize nunchuck I2C
#if I2C_DEBUG
    printf("try INIT\r\n");
#endif
    uint8_t initialized;
    uint8_t i;
    for (i = 0; i < 15; i++)
    {
		initialized = 0;
		while (!initialized)
		{
			initialized = nunchuckInit();
		}
#if I2C_DEBUG
		printf("INIT SUCCESS\r\n");
#endif
		pause(10);
	}

	struct nunchuck cur;
	struct nunchuck prev;

    while(1)
    {
		// reset drum triggers
		writeDAC(7,0);
		writeDAC(8,0);
		TriggeredOff();
		
		cur = readNunchuck();
		
		if (cur.good)
		{
			// trigger drums
			if (cur.bc == PRESSED && isTriggered())
			{
				writeDAC(7, 500);
			}			
			if (prev.bc != PRESSED && cur.bc == PRESSED)
			{
				writeDAC(7, 500);
				setIntervalMRT1((((710 - cur.ay) | 0x1 ) & 0xff) | (0x1 << 31));
			}
			if ((prev.bz != PRESSED && cur.bz == PRESSED) || (cur.bz == PRESSED && cur.az > 850)) 
			{
				writeDAC(8, 500);
			}
			prev.bc = cur.bc;
			prev.bz = cur.bz;

			// measured controller ranges with accelerometer resting
			// ax left-right 			290-695
			// ay point up-point down	302-700
			// az upright-upside down 	675-286
			// sx left-right			31-228
			// sy down-up				31-226
			
			// DAC values, control LEDs have ~1.8V forward voltage
		    // DAC 1023 => 5.5V
			// DAC 186 => 1.0V
			// DAC 372 => 2.0V
			// DAC 558 => 3.0V
			// DAC 744 => 4.0V
			
#if LEFT_NUNCHUCK			
			// resonance - left
			writeDAC(1, (cur.sx * 3 / 2) + 340);
			// filter mix - left
			writeDAC(4, (cur.sy * 3) + 300);
#else
			// modulate clip with a ramp - right
			// setIntervalMRT0((cur.sx - 30) | 0x1);
			// clip - right
			writeDAC(5, (cur.sy * 3) + 345); //(getCount0() % 345));
#endif			
			
			// cutoff 1 - left / cutoff 3 - right
			writeDAC(2, ((cur.ax * 3 / 2) - 55) & MASK_10_BITS);
			// cutoff 2 - left / cutoff 4 - right 
			writeDAC(3, ((cur.ax * 3 / 2) - 55) & MASK_10_BITS);
			
			// repeat trigger interval - both sides
			// zero stops timer, so choose only odd values
			// cap at 255 to keep interval < 256 ms in case of overflow
			setIntervalMRT1(((710 - cur.ay) | 0x1 ) & 0xff);
		
		}	
	}
}

