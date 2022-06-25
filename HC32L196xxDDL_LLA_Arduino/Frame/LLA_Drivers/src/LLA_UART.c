#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "uart.h"
#include "gpio.h"
#include "reset.h"

/************************ LLA common micro define ****************************/
#define IS_UART_NAME(x) 	(x<_LLA_UART_MAX)
#define IS_UART_CONFIG(x) (x<_UART_CONCIG_MAX)

/************************ LLA common variables ****************************/
int LLA_UART_Printf(UART_name_t name,const char *__restrict __format, ...){
	LLA_ASSERT(IS_UART_NAME(name),DriverCode_UART,UART_errorName);
	
	static char printf_buff[LLA_UART_PRINTF_BUFFER_LENGTH];//此处使用静态的原因是防止用户不熟悉框架底层而设的						\
																									栈区太小，导致内存分配失败进入HardFault（略微浪费内存）
	va_list args;
	va_start(args, __format);
	int ret_status = vsnprintf(printf_buff, sizeof(printf_buff), __format, args);
	va_end(args);
	LLA_UART_WriteBuffer(name,(uint8_t*)printf_buff,ret_status);
	return ret_status;

}


/************************ LLA common function ****************************/
LLA_WEAK void LLA_UART1_IRQHandler(uint8_t data){};
LLA_WEAK void LLA_UART2_IRQHandler(uint8_t data){};
LLA_WEAK void LLA_UART3_IRQHandler(uint8_t data){};
LLA_WEAK void LLA_UART4_IRQHandler(uint8_t data){};
LLA_WEAK void LLA_UART5_IRQHandler(uint8_t data){};
LLA_WEAK void LLA_UART6_IRQHandler(uint8_t data){};
LLA_WEAK void LLA_UART7_IRQHandler(uint8_t data){};
LLA_WEAK void LLA_UART8_IRQHandler(uint8_t data){};

/************************ LLA Interface function ****************************/

static M0P_UART_TypeDef* get_uart(UART_name_t name){
	LLA_ASSERT(IS_UART_NAME(name),DriverCode_UART,UART_errorName);
	if(name == LLA_UART1){
		return M0P_UART0;
	}else if(name == LLA_UART2){
		return M0P_UART1;
	}else if(name == LLA_UART3){
		return M0P_UART2;
	}else if(name == LLA_UART4){
		return M0P_UART3;
	}else {
		LLA_ASSERT(0,DriverCode_UART,UART_errorOutChipSupport);
	}
	return NULL;
}


/**
  * @brief	:	Initialize serial port and corresponding gpio.
  * @param	: 
  * @param	: 
  * @retval	:	
  */
void LLA_UART_Init(UART_name_t name,uint32_t baudRate,UART_config_t config){
	LLA_ASSERT(IS_UART_NAME(name),DriverCode_UART,UART_errorName);
	LLA_ASSERT(IS_UART_CONFIG(config),DriverCode_UART,UART_errorConfig);
	
	M0P_UART_TypeDef * uart_x = get_uart(name);
	
//config uart gpio
	stc_gpio_cfg_t gpio_cfg;
	DDL_ZERO_STRUCT(gpio_cfg);
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
	
	if(uart_x == M0P_UART0){
		
//		/* For General Design GPIO */
//		gpio_cfg.enDir = GpioDirOut;
//		Gpio_Init(GpioPortA, GpioPin9, &gpio_cfg);
//		Gpio_SetAfMode(GpioPortA,GpioPin9,GpioAf1);
//		
//		gpio_cfg.enDir = GpioDirIn;
//		Gpio_Init(GpioPortA, GpioPin10, &gpio_cfg);
//		Gpio_SetAfMode(GpioPortA,GpioPin10,GpioAf1);
		
		/* For BaierHUB GPIO */
		gpio_cfg.enDir = GpioDirOut;
		Gpio_Init(GpioPortB, GpioPin6, &gpio_cfg);
		Gpio_SetAfMode(GpioPortB,GpioPin6,GpioAf2);
		
		gpio_cfg.enDir = GpioDirIn;
		Gpio_Init(GpioPortB, GpioPin7, &gpio_cfg);
		Gpio_SetAfMode(GpioPortB,GpioPin7,GpioAf2);
		
	}else	if(uart_x == M0P_UART1){
		
		gpio_cfg.enDir = GpioDirOut;
		Gpio_Init(GpioPortA, GpioPin2, &gpio_cfg);
		Gpio_SetAfMode(GpioPortA,GpioPin2,GpioAf1);
		
		gpio_cfg.enDir = GpioDirIn;
		Gpio_Init(GpioPortA, GpioPin3, &gpio_cfg);
		Gpio_SetAfMode(GpioPortA,GpioPin3,GpioAf1);
		
	}else	if(uart_x == M0P_UART2){
		
		gpio_cfg.enDir = GpioDirOut;
		Gpio_Init(GpioPortC, GpioPin3, &gpio_cfg);
		Gpio_SetAfMode(GpioPortC,GpioPin3,GpioAf5);
		
		gpio_cfg.enDir = GpioDirIn;
		Gpio_Init(GpioPortC, GpioPin2, &gpio_cfg);
		Gpio_SetAfMode(GpioPortC,GpioPin2,GpioAf4);
		
	}else	if(uart_x == M0P_UART3){
		
		gpio_cfg.enDir = GpioDirOut;
		Gpio_Init(GpioPortC, GpioPin7, &gpio_cfg);
		Gpio_SetAfMode(GpioPortC,GpioPin7,GpioAf6);
		
		gpio_cfg.enDir = GpioDirIn;
		Gpio_Init(GpioPortC, GpioPin6, &gpio_cfg);
		Gpio_SetAfMode(GpioPortC,GpioPin6,GpioAf6);
		
	}
	
	if(uart_x == M0P_UART0){
		Sysctrl_SetPeripheralGate(SysctrlPeripheralUart0,TRUE);
	}else if(uart_x == M0P_UART1){
		Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1,TRUE);
	}else if(uart_x == M0P_UART2){
		Sysctrl_SetPeripheralGate(SysctrlPeripheralUart2,TRUE);
	}else if(uart_x == M0P_UART3){
		Sysctrl_SetPeripheralGate(SysctrlPeripheralUart3,TRUE);
	}else {
		return;
	}
	
	stc_uart_cfg_t uart_cfg;
	DDL_ZERO_STRUCT(uart_cfg);
	
	
	if(config == UART_CONFIG_8N1 || config == UART_CONFIG_8N1_5 || config == UART_CONFIG_8N2){
		uart_cfg.enRunMode = UartMskMode1;
	}else{
		uart_cfg.enRunMode = UartMskMode3;
	}
	
	if(config == UART_CONFIG_8E1 || config == UART_CONFIG_8E1_5 || config == UART_CONFIG_8E2){
		uart_cfg.enMmdorCk = UartMskEven;
	}else if(config == UART_CONFIG_8O1 || config == UART_CONFIG_8O1_5 || config == UART_CONFIG_8O2){
		uart_cfg.enMmdorCk  = UartMskOdd;
	}else{
		uart_cfg.enMmdorCk = UartMskDataOrAddr;
	}
	
	if(config == UART_CONFIG_8N1 || config == UART_CONFIG_8E1 || config == UART_CONFIG_8O1){
		uart_cfg.enStopBit = UartMsk1bit;
	}else if(config == UART_CONFIG_8N1_5 || config == UART_CONFIG_8E1_5 || config == UART_CONFIG_8O1_5){
		uart_cfg.enStopBit = UartMsk1_5bit;
	}else {
		uart_cfg.enStopBit = UartMsk2bit;
	}
	
	uart_cfg.stcBaud.u32Baud = baudRate;
	uart_cfg.stcBaud.enClkDiv = UartMsk8Or16Div;
	uart_cfg.stcBaud.u32Pclk = Sysctrl_GetPClkFreq();
	Uart_Init(uart_x,&uart_cfg);
	
	Uart_ClrStatus(uart_x,UartRC);
	Uart_ClrStatus(uart_x,UartTC);
	Uart_EnableIrq(uart_x,UartRxIrq);
	if(uart_x == M0P_UART0 || uart_x == M0P_UART2){
		
		NVIC_ClearPendingIRQ(UART0_2_IRQn);
		NVIC_SetPriority(UART0_2_IRQn,IrqLevel1);
		NVIC_EnableIRQ(UART0_2_IRQn);
		
	}else if(uart_x == M0P_UART1 || uart_x == M0P_UART3){
		
		NVIC_ClearPendingIRQ(UART1_3_IRQn);
		NVIC_SetPriority(UART1_3_IRQn,IrqLevel1);
		NVIC_EnableIRQ(UART1_3_IRQn);
		
	}
	
	
}

void LLA_UART_DeInit(UART_name_t name){
	LLA_ASSERT(IS_UART_NAME(name),DriverCode_UART,UART_errorName);
	M0P_UART_TypeDef * uart_x = get_uart(name);
	if(uart_x == M0P_UART0){
		Sysctrl_SetPeripheralGate(SysctrlPeripheralUart0,FALSE);
		Reset_RstPeripheral0(ResetMskUart0);
	}else if(uart_x == M0P_UART1){
		Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1,FALSE);
		Reset_RstPeripheral0(ResetMskUart1);
	}else if(uart_x == M0P_UART2){
		Sysctrl_SetPeripheralGate(SysctrlPeripheralUart2,FALSE);
		Reset_RstPeripheral1(ResetMskUart2);
	}else if(uart_x == M0P_UART3){
		Sysctrl_SetPeripheralGate(SysctrlPeripheralUart3,FALSE);
		Reset_RstPeripheral1(ResetMskUart3);
	}else {
		return;
	}
}

void LLA_UART_Write(UART_name_t name,uint8_t data){
	LLA_ASSERT(IS_UART_NAME(name),DriverCode_UART,UART_errorName);
	
	M0P_UART_TypeDef * uart_x = get_uart(name);
	Uart_SendDataPoll(uart_x,data);
}

void LLA_UART_WriteBuffer(UART_name_t name,uint8_t *data,uint16_t len){
	LLA_ASSERT(IS_UART_NAME(name),DriverCode_UART,UART_errorName);
	
	M0P_UART_TypeDef * uart_x = get_uart(name);
	while(len--){
		Uart_SendDataPoll(uart_x,*data++);
	}
}

/************************ Your Interrupt handling function ****************************/

void UART0_2_IRQHandler(){
	if(Uart_GetStatus(M0P_UART0,UartRC)){
		LLA_UART1_IRQHandler(Uart_ReceiveData(M0P_UART0));
		Uart_ClrStatus(M0P_UART0, UartRC);
	}
	
	if(Uart_GetStatus(M0P_UART2,UartRC)){
		LLA_UART3_IRQHandler(Uart_ReceiveData(M0P_UART2));
		Uart_ClrStatus(M0P_UART2, UartRC);
	}
}


void UART1_3_IRQHandler(){
	if(Uart_GetStatus(M0P_UART1,UartRC)){ 
		LLA_UART2_IRQHandler(Uart_ReceiveData(M0P_UART1));
		Uart_ClrStatus(M0P_UART1, UartRC);
	}
	
	if(Uart_GetStatus(M0P_UART3,UartRC)){
		LLA_UART4_IRQHandler(Uart_ReceiveData(M0P_UART3));
		Uart_ClrStatus(M0P_UART3, UartRC);
	}

}

