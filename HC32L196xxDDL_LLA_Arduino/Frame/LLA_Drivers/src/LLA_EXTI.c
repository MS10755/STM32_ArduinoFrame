#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "gpio.h"
#include "ddl.h"

/************************ LLA common micro define ****************************/
#define IS_EXTI_CB(x) 	(!!x)
#define IS_EXTI_TYPE(x)	(x<_LLA_EXTI_TYPE_MAX)

/************************ LLA common variables ****************************/
static LLA_EXTI_cb_t LLA_EXTI_cb_list[_BASEIO_NAME_MAX]={0};//This list consume 448 (7*16*4) Bytes RAM.


/************************ LLA common function ****************************/

/************************ User function ****************************/
static en_gpio_port_t get_port(BaseIO_name_t name){
	uint8_t port_index = name / 16;
	if(port_index == 0)return GpioPortA;
	if(port_index == 1)return GpioPortB;
	if(port_index == 2)return GpioPortC;
	if(port_index == 3)return GpioPortD;
	if(port_index == 4)return GpioPortE;
	if(port_index == 5)return GpioPortF;
	else{
		LLA_ASSERT(0,DriverCode_EXTI,EXTI_errorIOOutChipSupport);
	}
	return (en_gpio_port_t)0xFFFFF;// Just for no compile warnning.
}

static en_gpio_pin_t get_pin(BaseIO_name_t name){
	return (en_gpio_pin_t)(name % 16);
}


static void EnableNvic(IRQn_Type enIrq, en_irq_level_t enLevel, boolean_t bEn)
{
    NVIC_ClearPendingIRQ(enIrq);
    NVIC_SetPriority(enIrq, enLevel);
    if (TRUE == bEn)
    {
        NVIC_EnableIRQ(enIrq);
    }
    else
    {
        NVIC_DisableIRQ(enIrq);
    }
}


/************************ LLA Interface function ****************************/
void LLA_EXTI_AttachInterrupt(BaseIO_name_t name, LLA_EXTI_cb_t cb,LLA_EXTI_TYPE_t type){
	LLA_ASSERT(IS_IO_NAME(name),	DriverCode_EXTI,EXTI_errorIOName);
	LLA_ASSERT(IS_EXTI_CB(cb),		DriverCode_EXTI,EXTI_errorCallback);
	LLA_ASSERT(IS_EXTI_TYPE(type) && type!=LLA_ExTI_TYPE_CHANGE,DriverCode_EXTI,EXTI_errorType);
	
	en_gpio_port_t port = get_port(name);
	if(port == (en_gpio_port_t)0xFFFF) return;
	en_gpio_pin_t pin = get_pin(name);
	
	stc_gpio_cfg_t stcGpioCfg;
	DDL_ZERO_STRUCT(stcGpioCfg);
	
	///< 打开GPIO外设时钟门控
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
	
	///< 端口方向配置->输入
	stcGpioCfg.enDir = GpioDirIn;
	///< 端口驱动能力配置->高驱动能力
	stcGpioCfg.enDrv = GpioDrvL;

	//< 端口配置上下拉
	if(type == LLA_EXTI_TYPE_HIGH || type == LLA_EXTI_TYPE_RISING){
		stcGpioCfg.enPd = GpioPdEnable;
	}else if(type == LLA_EXTI_TYPE_LOW || type == LLA_EXTI_TYPE_FALLING){
		stcGpioCfg.enPu = GpioPuEnable;
	}else{
		LLA_ASSERT(0,DriverCode_EXTI,EXTI_errorType);
	}
	
	///< 端口开漏输出配置->开漏输出关闭
	stcGpioCfg.enOD = GpioOdDisable;
	///< 端口输入/输出值寄存器总线控制模式配置->AHB
	stcGpioCfg.enCtrlMode = GpioAHB;
	///< GPIO IO USER KEY初始化
	Gpio_Init(port, pin, &stcGpioCfg);
	
	if(type == LLA_EXTI_TYPE_HIGH){
		
		Gpio_EnableIrq(port, pin, GpioIrqHigh);
		
	}else if(type == LLA_EXTI_TYPE_LOW){
		
		Gpio_EnableIrq(port, pin, GpioIrqLow);
		
	}else if(type == LLA_EXTI_TYPE_RISING){
		
		Gpio_EnableIrq(port, pin, GpioIrqRising);
		
	}else if(type == LLA_EXTI_TYPE_FALLING){
		
		Gpio_EnableIrq(port, pin, GpioIrqFalling);
		
	}
	
	if(port == GpioPortA){
		
		EnableNvic(PORTA_IRQn, IrqLevel2, TRUE);
		
	}else if(port == GpioPortB){
		
		EnableNvic(PORTB_IRQn, IrqLevel2, TRUE);
		
	}else if(port == GpioPortC || port == GpioPortE){
		
		EnableNvic(PORTC_E_IRQn, IrqLevel2, TRUE);
		
	}else if(port == GpioPortD || port == GpioPortF){
		
		EnableNvic(PORTD_F_IRQn, IrqLevel2, TRUE);
		
	}
	
	LLA_EXTI_cb_list[name] = cb;
}


void LLA_EXTI_DetachInterrupt(BaseIO_name_t name){
	LLA_ASSERT(IS_IO_NAME(name),	DriverCode_EXTI,EXTI_errorIOName);
	LLA_EXTI_cb_list[(uint8_t)name] = NULL;//Set callback to NULL can Disable Interrupt on special pin. All EXTI on same PORT will be disabled,If you use NVIC functions.
}

/************************ Your Interrupt handling function ****************************/
static inline void PORTX_IRQHandler(en_gpio_port_t port,BaseIO_name_t pinFrom,BaseIO_name_t pinTo){
	for(BaseIO_name_t name = pinFrom;name<=pinTo;name++){			
		en_gpio_pin_t pin = get_pin(name);										
		if(Gpio_GetIrqStatus(port,pin)){												
			if(LLA_EXTI_cb_list[(uint8_t)name]){													
				LLA_EXTI_cb_list[(uint8_t)name]();													
			}
			Gpio_ClearIrq(port, pin);
		}																											
	}	
}

void PORTA_IRQHandler(void){
	PORTX_IRQHandler(GpioPortA,PA0,PA15);
}

void PORTB_IRQHandler(void){
	PORTX_IRQHandler(GpioPortB,PB0,PB15);
}

void PORTC_E_IRQHandler(void){
	PORTX_IRQHandler(GpioPortC,PC0,PC15);
	PORTX_IRQHandler(GpioPortE,PE0,PE15);
}
void PORTD_F_IRQHandler(void){
	PORTX_IRQHandler(GpioPortD,PD0,PD15);
	PORTX_IRQHandler(GpioPortF,PF0,PF15);
}


