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
#ifndef __HardwareSerial_h
#define __HardwareSerial_h

#include "Arduino.h"
#include "Print.h"
#include "Stream.h"

#define SERIAL_RX_BUFFER_SIZE 2048
#define SERIAL_Config_Default SERIAL_8N1

typedef enum {
    SERIAL_8N1 = UART_CONFIG_8N1,
    SERIAL_8N2 = UART_CONFIG_8N2,
    SERIAL_8E1 = UART_CONFIG_8E1,
    SERIAL_8E2 = UART_CONFIG_8N2,
    SERIAL_8O1 = UART_CONFIG_8O1,
    SERIAL_8O2 = UART_CONFIG_8O2,
    SERIAL_8N1_5 = UART_CONFIG_8O1_5,
    SERIAL_8E1_5 = UART_CONFIG_8E1_5,
    SERIAL_8O1_5 = UART_CONFIG_8O1_5,
} SERIAL_Config;

class HardwareSerial : public Stream
{
    typedef uint16_t rx_buffer_index_t;
    typedef void(*USART_CallbackFunction_t)(void);
public:
    HardwareSerial(UART_name_t USARTx);
    void IRQHandler(uint8_t data);
    void begin(uint32_t BaudRate);
    void begin(uint32_t BaudRate, SERIAL_Config Config);
    void begin(uint32_t BaudRate, SERIAL_Config Config, uint8_t PreemptionPriority, uint8_t SubPriority);
    void end(void);
    void attachInterrupt(USART_CallbackFunction_t Function);
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual void flush(void);

    virtual size_t write(uint8_t n);
    inline size_t write(unsigned long n) {
        return write((uint8_t)n);
    }
    inline size_t write(long n) {
        return write((uint8_t)n);
    }
    inline size_t write(unsigned int n) {
        return write((uint8_t)n);
    }
    inline size_t write(int n) {
        return write((uint8_t)n);
    }
    using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool() {
        return true;
    }

private:
    UART_name_t USARTx;
    USART_CallbackFunction_t USART_Function;
    volatile uint16_t _rx_buffer_head;
    volatile uint16_t _rx_buffer_tail;
    uint8_t _rx_buffer[SERIAL_RX_BUFFER_SIZE];
};

extern HardwareSerial Serial;
//extern HardwareSerial Serial2;
//extern HardwareSerial Serial3;
extern HardwareSerial Serial4;
#endif
