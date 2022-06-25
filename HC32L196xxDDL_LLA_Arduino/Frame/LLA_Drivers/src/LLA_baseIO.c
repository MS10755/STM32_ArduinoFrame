#include "LLA_Drivers.h"



/* Chip SDK headers includes */
#include "gpio.h"


/* Your static function must be declare in here. */
static en_gpio_port_t get_port(BaseIO_name_t name){
	uint8_t port_index = name / 16;
	if(port_index == 0)return GpioPortA;
	if(port_index == 1)return GpioPortB;
	if(port_index == 2)return GpioPortC;
	if(port_index == 3)return GpioPortD;
	if(port_index == 4)return GpioPortE;
	if(port_index == 5)return GpioPortF;
	else{
		LLA_ASSERT(0,DriverCode_BaseIO,BaseIO_errorIOOutChipSupport);
	}
	return (en_gpio_port_t)0xFFFFF;// Just for no compile warnning.
}

static en_gpio_pin_t get_pin(BaseIO_name_t name){
	return (en_gpio_pin_t)(name % 16);
}

/* Your static function END. */


/**
  * @brief	:	Set IO Mode
  * @param	: 
  * @param	: 
  * @retval	:	None
  */
void LLA_BaseIO_Mode(BaseIO_name_t name,BaseIO_mode_t mode,BaseIO_status_t outputStatus){
	LLA_ASSERT(IS_IO_NAME(name),DriverCode_BaseIO,BaseIO_errorIOName);
	LLA_ASSERT(IS_IO_MODE(mode),DriverCode_BaseIO,BaseIO_errorIOMode);
	LLA_ASSERT(IS_IO_STATUS(outputStatus),DriverCode_BaseIO,BaseIO_errorIOStatus);
	
	en_gpio_port_t port = get_port(name);
	if(port == (en_gpio_port_t)0xFFFF) return;
	en_gpio_pin_t pin = get_pin(name);
	
	stc_gpio_cfg_t gpio_cfg={0};
	gpio_cfg.enCtrlMode = GpioAHB;
	gpio_cfg.bOutputVal = outputStatus;
	if(mode == BaseIOMode_INPUT || mode == BaseIOMode_INPUT_PD || mode == BaseIOMode_INPUT_PU){
		gpio_cfg.enDir = GpioDirIn;
	}else{
		gpio_cfg.enDir = GpioDirOut;
	}
	gpio_cfg.enDrv = GpioDrvH;
	if(mode == BaseIOMode_OUTPUT_OD || mode == BaseIOMode_OUTPUT_OD_PD || mode == BaseIOMode_OUTPUT_OD_PU){
		gpio_cfg.enOD = GpioOdEnable;
	}
	
	if(mode == BaseIOMode_INPUT_PD || mode == BaseIOMode_OUTPUT_OD_PD){
		gpio_cfg.enPd = GpioPdEnable;
	}
	if(mode == BaseIOMode_INPUT_PU || mode == BaseIOMode_OUTPUT_OD_PU){
		gpio_cfg.enPu = GpioPuEnable;
	}
	
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
	Gpio_Init(port,pin,&gpio_cfg);
	Gpio_SetAfMode(port,pin,GpioAf0);
}

/**
  * @brief	:	Set IO output level
  * @param	: 
  * @param	: 
  * @retval	:	None
  */
void LLA_BaseIO_Write(BaseIO_name_t name,BaseIO_status_t outputStatus){
	LLA_ASSERT(IS_IO_NAME(name),DriverCode_BaseIO,BaseIO_errorIOName);
	LLA_ASSERT(IS_IO_STATUS(outputStatus),DriverCode_BaseIO,BaseIO_errorIOStatus);
	
	en_gpio_port_t port = get_port(name);
	en_gpio_pin_t pin = get_pin(name);
	
	Gpio_WriteOutputIO(port,pin,outputStatus);
}

/**
  * @brief	:	Read IO input level
  * @param	: 
  * @param	: 
  * @retval	:	Input level
  */
BaseIO_status_t LLA_BaseIO_ReadInput(BaseIO_name_t name){
	LLA_ASSERT(IS_IO_NAME(name),DriverCode_BaseIO,BaseIO_errorIOName);
	
	en_gpio_port_t port = get_port(name);
	en_gpio_pin_t pin = get_pin(name);
	return (BaseIO_status_t)Gpio_GetInputIO(port,pin);
}

/**
  * @brief	:	Read IO output level
  * @param	: 
  * @param	: 
  * @retval	:	Output level
  */
BaseIO_status_t LLA_BaseIO_ReadOutput(BaseIO_name_t name){
	LLA_ASSERT(IS_IO_NAME(name),DriverCode_BaseIO,BaseIO_errorIOName);
	
	en_gpio_port_t port = get_port(name);
	en_gpio_pin_t pin = get_pin(name);
	return (BaseIO_status_t)Gpio_ReadOutputIO(port,pin);
}

/**
  * @brief	:	Toggle IO output level
  * @param	: 
  * @param	: 
  * @retval	:	Current IO output level after toggle
  */
BaseIO_status_t LLA_BaseIO_Toggle(BaseIO_name_t name){
	LLA_ASSERT(IS_IO_NAME(name),DriverCode_BaseIO,BaseIO_errorIOName);
	
	en_gpio_port_t port = get_port(name);
	en_gpio_pin_t pin = get_pin(name);
	
	if(Gpio_ReadOutputIO(port,pin)){
		Gpio_ClrIO(port,pin);
		return BaseIOStatus_LOW;
	}else{
		Gpio_SetIO(port,pin);
		return BaseIOStatus_HIGH;
	}
}

