/*
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

#include "wiring_private.h"



unsigned long millis()
{
	return LLA_SYS_Time_Millis();
}

unsigned long micros() {
	return LLA_SYS_Time_Micros();
}

void delay(unsigned long ms)
{
	LLA_SYS_Time_DelayMS(ms);
}

/* Delay for the given number of microseconds.  Assumes a 1, 8, 12, 16, 20 or 24 MHz clock. */
void delayMicroseconds(unsigned int us)
{
	LLA_SYS_Time_DelayUS(us);
}

void init()
{
	LLA_SYS_IRQ_Init();
	LLA_SYS_clock_Init();
	LLA_UART_Init(LLA_UART1,115200,UART_CONFIG_8N1);
	LLA_Flash_Init();
	LLA_PWM_BaseHZ(1000);
	LLA_ADC_Init();
	LLA_SYS_Time_Init();
}
