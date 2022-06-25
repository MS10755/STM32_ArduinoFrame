#ifndef __LLA_ERRORCODE_H__
#define __LLA_ERRORCODE_H__

#include <stdint.h>

#define LLA_ERRORCODEINFOS_LANG	0

typedef enum{
	DriverCode_ADC,
	DriverCode_AdvanceIO,
	DriverCode_BaseIO,
	DriverCode_Delay,
	DriverCode_EXTI,
	DriverCode_Flash,
	DriverCode_PWM,
	DriverCode_SPI,
	DriverCode_SYS_Clock,
	DriverCode_SYS_IRQ,
	DriverCode_SYS_Time,
	DriverCode_Timer,
	DriverCode_UART,
	DriverCode_USB,
	DriverCode_WDG,

}DriverCode_t;

enum{
	BaseIO_errorNone,
	BaseIO_errorIOName,
	BaseIO_errorIOMode,
	BaseIO_errorIOStatus,
	BaseIO_errorIOOutChipSupport,
};

enum{
	UART_errorNone,
	UART_errorName,
	UART_errorConfig,
	UART_errorOutChipSupport,
};

enum{
	ADC_errorNone,
	ADC_errorIOName,
	ADC_errorPinNotChannel,
	ADC_errorSpecialChannel,
	ADC_errorIOOutChipSupport,
	ADC_errorSpecialOutChipSupport,
};

enum{
	EXTI_errorNone,
	EXTI_errorIOName,
	EXTI_errorIOOutChipSupport,
	EXTI_errorCallback,
	EXTI_errorType,
	EXTI_errorTypeOutChipSupport,
};

enum{
	AdvanceIO_errorNone,
	AdvanceIO_errorIOOutChipSupport,
	AdvanceIO_errorIONotSupportPWC,
	AdvanceIO_errorPWC,
	AdvanceIO_errorPWCTimeOut,
};

enum{
	PWM_errorNone,
	PWM_errorIOOutChipSupport,
	PWM_errorIONotSupportPWM,
	PWM_errorPolar,
	PWM_errorRange,
};

enum{
	TIMER_errorNone,
	TIMER_errorTimer,
	TIMER_errorTimerOutChipSupport,
	TIMER_errorCallback,
	TIMER_errorNotSet,
};

enum{
	FLASH_errorNone,
	FLASH_errorInit,
	FLASH_errorAddr,
	FLASH_errorErase,
	FLASH_errorWrite,
};

enum{
	SPI_errorNone,
	SPI_errorMode,
	SPI_errorIOOutChipSupport,
	SPI_errorSPIOutChipSupport,
	SPI_errorWrite,
	SPI_errorRead,
};

enum{
	WDG_errorNone,
	WDG_errorTimeOutChipSupport,
	WDG_errorStop,
};

#ifdef __cplusplus
extern "C" {
#endif

const char * LAA_errorInfo(DriverCode_t driverCode,uint8_t errorCode);
void LLA_errorCode_Handler(DriverCode_t driverCode,uint8_t errorCode);
	
#ifdef __cplusplus
}
#endif

#endif


