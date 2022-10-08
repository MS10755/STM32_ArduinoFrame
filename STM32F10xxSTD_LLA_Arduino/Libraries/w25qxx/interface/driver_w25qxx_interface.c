/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
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
 *
 * @file      driver_w25qxx_interface_template.c
 * @brief     driver w25qxx interface template source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-07-15
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/07/15  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_w25qxx_interface.h"
#include "stm32f10x.h"                  // Device header
#include "LLA_Drivers.h"
/**
 * @brief  interface spi qspi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi qspi init failed
 * @note   none
 */
uint8_t w25qxx_interface_spi_qspi_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
		
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//F_CS PIN on PB12
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI2,&SPI_InitStructure);
	SPI_Cmd(SPI2,ENABLE);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
    return 0;
}

/**
 * @brief  interface spi qspi bus deinit
 * @return status code
 *         - 0 success
 *         - 1 spi qspi deinit failed
 * @note   none
 */
uint8_t w25qxx_interface_spi_qspi_deinit(void)
{
	SPI_I2S_DeInit(SPI2);
    return 0;
}

/**
 * @brief      interface spi qspi bus write read
 * @param[in]  instruction is the sent instruction
 * @param[in]  instruction_line is the instruction phy lines
 * @param[in]  address is the register address
 * @param[in]  address_line is the address phy lines
 * @param[in]  address_len is the address length
 * @param[in]  alternate is the register address
 * @param[in]  alternate_line is the alternate phy lines
 * @param[in]  alternate_len is the alternate length
 * @param[in]  dummy is the dummy cycle
 * @param[in]  *in_buf points to a input buffer
 * @param[in]  in_len is the input length
 * @param[out] *out_buf points to a output buffer
 * @param[in]  out_len is the output length
 * @param[in]  data_line is the data phy lines
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       none
 */
uint8_t w25qxx_interface_spi_qspi_write_read(uint8_t instruction, uint8_t instruction_line,
                                             uint32_t address, uint8_t address_line, uint8_t address_len,
                                             uint32_t alternate, uint8_t alternate_line, uint8_t alternate_len,
                                             uint8_t dummy, uint8_t *in_buf, uint32_t in_len,
                                             uint8_t *out_buf, uint32_t out_len, uint8_t data_line)
{
	   if ((instruction_line != 0) || (address_line != 0) || (alternate_line != 0) || (dummy != 0) || (data_line != 1))
    {
        return 1;
    }
		const timeout = 100;
		uint32_t start_ms = LLA_SYS_Time_Millis();
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		for(uint32_t i=0;i<in_len;i++){
			while(1){
				if(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==SET){
					break;
				}
				if(LLA_SYS_Time_ConsumeMillis(start_ms)>timeout){
					goto time_out;
				}
			};
			
			SPI_I2S_SendData(SPI2,in_buf[i]);
			while(1){
				if(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==SET){
					break;
				}
				if(LLA_SYS_Time_ConsumeMillis(start_ms)>timeout){
					goto time_out;
				}
			};
			SPI_I2S_ReceiveData(SPI2);
		}
		
		for(uint32_t i=0;i<out_len;i++){
			while(1){
				if(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==SET){
					break;
				}
				if(LLA_SYS_Time_ConsumeMillis(start_ms)>timeout){
					goto time_out;
				}
			};
			SPI_I2S_SendData(SPI2,0xFF);
			while(1){
				if(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==SET){
					break;
				}
				if(LLA_SYS_Time_ConsumeMillis(start_ms)>timeout){
					goto time_out;
				}
			};
			out_buf[i] = SPI_I2S_ReceiveData(SPI2);
		}
	GPIO_SetBits(GPIOB,GPIO_Pin_12);		
	return 0;

time_out:
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	return 1;
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void w25qxx_interface_delay_ms(uint32_t ms)
{
	LLA_SYS_Time_DelayMS(ms);
}

/**
 * @brief     interface delay us
 * @param[in] us
 * @note      none
 */
void w25qxx_interface_delay_us(uint32_t us)
{
	LLA_SYS_Time_DelayUS(us);
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void w25qxx_interface_debug_print(const char *const fmt, ...)
{
		static char str[256];
    uint8_t len;
    va_list args;
    
    memset((char *)str, 0, sizeof(char) * 256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 256, (char const *)fmt, args);
    va_end(args);
    
    len = strlen((char *)str);
    (void)LLA_UART_WriteBuffer(LLA_UART1,(uint8_t *)str, len);
}
