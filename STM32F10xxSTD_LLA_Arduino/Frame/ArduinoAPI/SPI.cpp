/*
 * MIT License
 * Copyright (c) 2019 _VIFEXTech
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "SPI.h"
#include "Frame.h"

SPIClass::SPIClass(LLA_SPI_t _SPIx)
{
    SPIx = _SPIx;
}

void SPIClass::SPI_Settings(  	LLA_SPI_t SPIx,
                                uint16_t SPI_Mode_x,
                                uint16_t SPI_NSS_x,
                                uint32_t clock,
                                uint16_t SPI_FirstBit_x)
{
	(void)SPI_FirstBit_x;
	LLA_SPI_Init(SPIx,clock,(LLA_SPI_Mode_t)SPI_Mode_x,SPI_NSS_x==0xFF?LLA_SPI_CS_Auto:LLA_SPI_CS_soft);
}

void SPIClass::begin(void)
{
	SPI_Settings(SPIx,SPI_MODE0,0xFF,24000000,MSBFIRST);
}

void SPIClass::begin(uint32_t clock, uint16_t dataOrder, uint16_t dataMode)
{
	SPI_Settings(SPIx,dataMode,0xFF,clock,dataOrder);
}

void SPIClass::begin(SPISettings settings)
{
	SPI_Settings(SPIx,settings.dataMode,settings._SSPin,settings.clock,settings.bitOrder);
}

void SPIClass::beginSlave(void)
{
	
}

void SPIClass::end(void)
{
	LLA_SPI_DeInit(SPIx);
}

void SPIClass::setClock(uint32_t clock)
{
	settings.clock = clock;
	begin(settings);
}

void SPIClass::setClockDivider(uint32_t Div)
{

}

void SPIClass::setBitOrder(uint16_t bitOrder)
{
	settings.bitOrder = bitOrder;
	begin(settings);
}


void SPIClass::setDataMode(uint8_t dataMode)
{
   /* Notes.  As far as I can tell, the AVR numbers for dataMode appear to match the numbers required by the STM32

    From the AVR doc http://www.atmel.com/images/doc2585.pdf section 2.4

    SPI Mode    CPOL    CPHA    Shift SCK-edge  Capture SCK-edge
    0           0       0       Falling         Rising
    1           0       1       Rising          Falling
    2           1       0       Rising          Falling
    3           1       1       Falling         Rising


    On the STM32 it appears to be

    bit 1 - CPOL : Clock polarity
        (This bit should not be changed when communication is ongoing)
        0 : CLK to 0 when idle
        1 : CLK to 1 when idle

    bit 0 - CPHA : Clock phase
        (This bit should not be changed when communication is ongoing)
        0 : The first clock transition is the first data capture edge
        1 : The second clock transition is the first data capture edge

    If someone finds this is not the case or sees a logic error with this let me know ;-)
     */
  settings.dataMode = dataMode;
	begin(settings);
}

void SPIClass::beginTransaction(SPISettings settings)
{
	this->settings = settings;
	begin(settings);
}

void SPIClass::beginTransactionSlave(void)
{
    beginSlave();
}

void SPIClass::endTransaction(void)
{
	LLA_SPI_DeInit(SPIx);
}


uint16_t SPIClass::read(void)
{
	uint8_t data;
	LLA_SPI_ReadBytes(SPIx,&data,1);
  return data;
}

void SPIClass::read(uint8_t *buf, uint32_t len)
{
	LLA_SPI_ReadBytes(SPIx,buf,len);
}

void SPIClass::write(uint16_t data)
{
	uint8_t _data = data;
	LLA_SPI_WriteBytes(SPIx,&_data,1);
}

void SPIClass::write(uint16_t data, uint32_t n)
{
	while(n--){
		write(data);
	}
}

void SPIClass::write(const uint8_t *data, uint32_t length)
{
	LLA_SPI_WriteBytes(SPIx,(uint8_t*)data,length);
}

void SPIClass::write(const uint16_t *data, uint32_t length)
{
	LLA_SPI_WriteBytes(SPIx,(uint8_t*)data,length);
}

uint8_t SPIClass::transfer(uint8_t wr_data) const
{
	LLA_SPI_WriteBytes(SPIx,&wr_data,1);
	return wr_data;
}

uint16_t SPIClass::transfer16(uint16_t wr_data) const
{
	uint8_t _wr_data = wr_data;
	LLA_SPI_WriteBytes(SPIx,&_wr_data,1);
	return _wr_data;
}

uint8_t SPIClass::send(uint8_t data)
{
    this->write(data);
    return 1;
}

uint8_t SPIClass::send(uint8_t *buf, uint32_t len)
{
    this->write(buf, len);
    return len;
}

uint8_t SPIClass::recv(void)
{
    return this->read();
}

SPIClass SPI(LLA_SPI0);//SCK-PA5 MISO-PA6 MOSI-PA7
//SPIClass SPI_2(LLA_SPI1);//SCK-PB13 MISO-PB14 MOSI-PB15

//SPIClass SPI(LLA_SPI1);//SCK-PB13 MISO-PB14 MOSI-PB15
