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

#include "uart.h"

void uart0Init(uint32_t baud)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (0x1 << 14); // enable clock for uart0
    LPC_SYSCON->PRESETCTRL &= ~(0x1 << 3); // assert the reset for uart0
    LPC_SYSCON->PRESETCTRL |= (0x1 << 3); // clear the reset for uart0
    
    // setup uart clock using calculation from NXP example code
    LPC_SYSCON->UARTCLKDIV = 1; // choose 1 to keep it simple
    LPC_SYSCON->UARTFRGDIV = 0xFF; // set to 0xFF per manual
    // added *2 to UARTSysClk, works empirically
    uint32_t UARTSysClk = SystemCoreClock*2/LPC_SYSCON->UARTCLKDIV;
    LPC_USART0->BRG = UARTSysClk/16/baud-1;
    LPC_SYSCON->UARTFRGMULT = (((UARTSysClk / 16) \
                              * (LPC_SYSCON->UARTFRGDIV + 1)) \
                              / (baud * (LPC_USART0->BRG + 1))) \
                              - (LPC_SYSCON->UARTFRGDIV + 1);
    
    // enable uart0
    LPC_USART0->CFG = UART_CFG_ENABLE | UART_CFG_DATALEN(8) | \
                      UART_CFG_NO_PARITY | UART_CFG_STOPLEN | \
                      UART_CFG_NO_FLOW_CTL | UART_CFG_ASYNC | \
                      UART_CFG_CLKPOL;

}

uint8_t uart0IsReadyToRx()
{
    return (uint8_t)(LPC_USART0->STAT & UART_STAT_RXRDY);
}

uint8_t uart0ReadByte()
{
    return (uint8_t)(LPC_USART0->RXDATA);
}

void uart1Init(uint32_t baud)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (0x1 << 15); // enable clock for uart1
    LPC_SYSCON->PRESETCTRL &= ~(0x1 << 4); // assert the reset for uart1
    LPC_SYSCON->PRESETCTRL |= (0x1 << 4); // clear the reset for uart1
    
    // setup uart clock using calculation from NXP example code
    LPC_SYSCON->UARTCLKDIV = 1; // choose 1 to keep it simple
    LPC_SYSCON->UARTFRGDIV = 0xFF; // set to 0xFF per manual
    // added *2 to UARTSysClk, works empirically
    uint32_t UARTSysClk = SystemCoreClock*2/LPC_SYSCON->UARTCLKDIV;
    LPC_USART1->BRG = UARTSysClk/16/baud-1;
    LPC_SYSCON->UARTFRGMULT = (((UARTSysClk / 16) \
                              * (LPC_SYSCON->UARTFRGDIV + 1)) \
                              / (baud * (LPC_USART1->BRG + 1))) \
                              - (LPC_SYSCON->UARTFRGDIV + 1);
    
    // enable uart1
    LPC_USART1->CFG = UART_CFG_ENABLE | UART_CFG_DATALEN(8) | \
                      UART_CFG_NO_PARITY | UART_CFG_STOPLEN | \
                      UART_CFG_NO_FLOW_CTL | UART_CFG_ASYNC | \
                      UART_CFG_CLKPOL;

}

uint8_t uart1IsReadyToRx()
{
    return (uint8_t)(LPC_USART1->STAT & UART_STAT_RXRDY);
}

uint8_t uart1ReadByte()
{
    return (uint8_t)(LPC_USART1->RXDATA);
}



