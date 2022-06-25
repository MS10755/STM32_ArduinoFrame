/*
  This file is part of the ArduinoRS485 library.
  Copyright (c) 2018 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "RS485.h"
 
RS485Class::RS485Class(HardwareSerial& hwSerial, int pin_txrx_ctr , ctr_mode_t ctr_mode) :
  _serial(&hwSerial),
	_pin_txrx_ctr(pin_txrx_ctr),
	_ctr_mode(ctr_mode),
	_predelay(RS485_DEFAULT_PRE_DELAY),
	_postdelay(RS485_DEFAULT_POST_DELAY),
  _transmisionBegun(false)
{
}

void RS485Class::begin(unsigned long baudrate)
{
  begin(baudrate, SERIAL_8N1, RS485_DEFAULT_PRE_DELAY, RS485_DEFAULT_POST_DELAY);
}

void RS485Class::begin(unsigned long baudrate, int predelay, int postdelay)
{
  begin(baudrate, SERIAL_8N1, predelay, postdelay);
}

void RS485Class::begin(unsigned long baudrate, uint16_t config)
{
  begin(baudrate, config, RS485_DEFAULT_PRE_DELAY, RS485_DEFAULT_POST_DELAY);
}

void RS485Class::begin(unsigned long baudrate, uint16_t config, int predelay, int postdelay)
{
  _baudrate = baudrate;
  _config = config;

  // Set only if not already initialized with ::setDelays
  _predelay = _predelay == 0 ? predelay : _predelay;
  _postdelay = _postdelay == 0 ? postdelay : _postdelay;

  if (_pin_txrx_ctr > -1 && _ctr_mode!=CTR_NOTUSE) {
		pinMode(_pin_txrx_ctr,OUTPUT);
  }

  _transmisionBegun = false;

  _serial->begin(baudrate, (SERIAL_Config)config);
}

void RS485Class::end()
{
  _serial->end();
}

int RS485Class::available()
{
  return _serial->available();
}

int RS485Class::peek()
{
  return _serial->peek();
}

int RS485Class::read(void)
{
  return _serial->read();
}

void RS485Class::flush()
{
  return _serial->flush();
}

size_t RS485Class::write(uint8_t b)
{
  if (!_transmisionBegun) {
    setWriteError();
    return 0;
  }

  return _serial->write(b);
}

RS485Class::operator bool()
{
  return true;
}

//通信开始，默认为发送模式
void RS485Class::beginTransmission()
{
  if (_pin_txrx_ctr > -1 && _ctr_mode!=CTR_NOTUSE) {
		digitalWrite(_pin_txrx_ctr, _ctr_mode==CTR_THRL? HIGH:LOW);
    if (_predelay) delay(_predelay);
  }

  _transmisionBegun = true;
}

void RS485Class::endTransmission()
{
  _serial->flush();
  _transmisionBegun = false;
}

//接收模式
void RS485Class::receive()
{
  if (_pin_txrx_ctr > -1 && _ctr_mode!=CTR_NOTUSE) {
    digitalWrite(_pin_txrx_ctr, _ctr_mode==CTR_THRL? LOW:HIGH);
		
  }
}

//发送模式
void RS485Class::noReceive()
{
  if (_pin_txrx_ctr > -1 && _ctr_mode!=CTR_NOTUSE) {
    digitalWrite(_pin_txrx_ctr, _ctr_mode==CTR_THRL? HIGH:LOW);
  }
}

//发送终止
void RS485Class::sendBreak(unsigned int duration)
{
  _serial->flush();
  _serial->end();
  delay(duration);
  _serial->begin(_baudrate, (SERIAL_Config)_config);
}

void RS485Class::sendBreakMicroseconds(unsigned int duration)
{
  _serial->flush();
  _serial->end();
  delayMicroseconds(duration);
  _serial->begin(_baudrate, (SERIAL_Config)_config);
}

void RS485Class::setPins(int pin_txrx_ctr)
{
	_pin_txrx_ctr = pin_txrx_ctr;
}

void RS485Class::setDelays(int predelay, int postdelay)
{
  _predelay = predelay;
  _postdelay = postdelay;
}


//RS485Class RS485(Serial2,PD7,CTR_THRL);
RS485Class RS485(Serial4,PB12,CTR_THRL);
