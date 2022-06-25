/*
  wiring_digital.c - digital input and output functions
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

  Modified 28 September 2010 by Mark Sproul
*/

#define ARDUINO_MAIN
#include "wiring_private.h"

void pinMode(uint8_t pin, uint8_t mode)
{
	BaseIO_name_t name = (BaseIO_name_t)pin;
	switch(mode){
		case INPUT:
			LLA_BaseIO_Mode(name,BaseIOMode_INPUT,BaseIOStatus_LOW);
			break;
		case INPUT_ADC:
			break;
		case OUTPUT:
			LLA_BaseIO_Mode(name,BaseIOMode_OUTPUT,BaseIOStatus_LOW);
			break;
		case OUTPUT_OPEN_DRAIN:
			LLA_BaseIO_Mode(name,BaseIOMode_OUTPUT_OD,BaseIOStatus_LOW);
			break;
		case OUTPUT_PWM:
			break;
		case INPUT_PULLUP:
			LLA_BaseIO_Mode(name,BaseIOMode_INPUT_PU,BaseIOStatus_LOW);
			break;
		case INPUT_PULLDOWN:
			LLA_BaseIO_Mode(name,BaseIOMode_INPUT_PD,BaseIOStatus_LOW);
			break;
	}
}

// Forcing this inline keeps the callers from having to push their own stuff
// on the stack. It is a good performance win and only takes 1 more byte per
// user than calling. (It will take more bytes on the 168.)
//
// But shouldn't this be moved into pinMode? Seems silly to check and do on
// each digitalread or write.
//
// Mark Sproul:
// - Removed inline. Save 170 bytes on atmega1280
// - changed to a switch statment; added 32 bytes but much easier to read and maintain.
// - Added more #ifdefs, now compiles for atmega645
//
//static inline void turnOffPWM(uint8_t timer) __attribute__ ((always_inline));
//static inline void turnOffPWM(uint8_t timer)
//static void turnOffPWM(uint8_t timer)
//{
//	
//}

void digitalWrite(uint8_t pin, uint8_t val)
{
	LLA_BaseIO_Write((BaseIO_name_t)pin,(BaseIO_status_t)val);
}

int digitalRead(uint8_t pin)
{
	return LLA_BaseIO_ReadInput((BaseIO_name_t)pin);
}
