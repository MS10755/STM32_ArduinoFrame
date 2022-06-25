#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "timer3.h"
#include "gpio.h"

/************************ User type define ****************************/
typedef struct{
	BaseIO_name_t name;
	en_gpio_af_t gpio_af;
	en_tim3_mscr_ts_t mscr_ts;//  Tim3Ts6IAFP or Tim3Ts7IBFP
}timer3_cho_info_t;

/************************ User user variables ****************************/
static const timer3_cho_info_t timer3_cho_info_list[]={
	{PA7,GpioAf4,Tim3Ts7IBFP},
	{PA8,GpioAf2,Tim3Ts6IAFP},
	{PB3,GpioAf4,Tim3Ts6IAFP},
	{PB4,GpioAf6,Tim3Ts7IBFP},
	{PB6,GpioAf6,Tim3Ts6IAFP},
	{PB13,GpioAf3,Tim3Ts7IBFP},
	{PE8,GpioAf1,Tim3Ts7IBFP},
	{PE9,GpioAf1,Tim3Ts6IAFP},
	{PE14,GpioAf1,Tim3Ts7IBFP},
};

static volatile uint16_t timer3_ovr_count;
static volatile uint32_t timer3_pwc_val;
static volatile uint8_t measure_ok;

/************************ User micro define ****************************/
#define TIMER3_CHO_INFO_LIST_LEN	(sizeof(timer3_cho_info_list)/sizeof(timer3_cho_info_list[0]))
#define IS_PWC_TYPE(x) (x<_ADVANCEIO_PWC_MAX)
#define TIMER3_TICK_PERSECONDS	(48)

/************************ User static function ****************************/
static const timer3_cho_info_t* get_timer3_cho_info(BaseIO_name_t name){
	for(uint8_t i=0;i<TIMER3_CHO_INFO_LIST_LEN;i++){
		if(timer3_cho_info_list[i].name == name){
			return &timer3_cho_info_list[i];
		}
	}
	LLA_ASSERT(0,DriverCode_AdvanceIO,AdvanceIO_errorIONotSupportPWC);
	return NULL;
}

static en_gpio_port_t get_port(BaseIO_name_t name){
	uint8_t port_index = name / 16;
	if(port_index == 0)return GpioPortA;
	if(port_index == 1)return GpioPortB;
	if(port_index == 2)return GpioPortC;
	if(port_index == 3)return GpioPortD;
	if(port_index == 4)return GpioPortE;
	if(port_index == 5)return GpioPortF;
	else{
		LLA_ASSERT(0,DriverCode_AdvanceIO,AdvanceIO_errorIOOutChipSupport);
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

/************************ LLA common micro define ****************************/


/************************ LLA common variables ****************************/


/************************ LLA common function ****************************/
uint8_t LLA_AdvanceIO_ShiftIn(BaseIO_name_t dataPin, BaseIO_name_t clockPin, LLA_BitOrder_t bitOrder) {
	uint8_t value = 0;
	uint8_t i;

	for (i = 0; i < 8; ++i) {
		LLA_BaseIO_Write(clockPin,BaseIOStatus_HIGH);
		if (bitOrder == LLA_LSBFIRST)
			value |= LLA_BaseIO_ReadInput(dataPin) << i;
		else
			value |= LLA_BaseIO_ReadInput(dataPin) << (7 - i);
		LLA_BaseIO_Write(clockPin,BaseIOStatus_LOW);
	}
	return value;
}

void LLA_AdvanceIO_ShiftOut(BaseIO_name_t dataPin, BaseIO_name_t clockPin, LLA_BitOrder_t bitOrder, uint8_t val)
{
	uint8_t i;

	for (i = 0; i < 8; i++)  {
		if (bitOrder == LLA_LSBFIRST) {
			LLA_BaseIO_Write(dataPin,(BaseIO_status_t)(val & 1));
			val >>= 1;
		} else {	
			LLA_BaseIO_Write(dataPin,(BaseIO_status_t)((val & 128) != 0));
			val <<= 1;
		}
		LLA_BaseIO_Write(clockPin, BaseIOStatus_HIGH);
		LLA_BaseIO_Write(clockPin, BaseIOStatus_LOW);		
	}
}

/************************ LLA Interface function ****************************/
uint32_t LLA_AdvanceIO_PluseMeasure(BaseIO_name_t name,AdvanceIO_PWC_t pwc,BaseIO_status_t idle_status,uint32_t time_out){
	LLA_ASSERT(IS_PWC_TYPE(pwc),DriverCode_AdvanceIO,AdvanceIO_errorPWC);
	
	const timer3_cho_info_t * cho_info = get_timer3_cho_info(name);
	if(!cho_info) return 0;
	
	en_gpio_port_t port = get_port(name);
	en_gpio_pin_t pin = get_pin(name);
	
	/* GPIO Settings. */
	stc_gpio_cfg_t stcTIM3Port;
	DDL_ZERO_STRUCT(stcTIM3Port);
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); //端口外设时钟使能
	stcTIM3Port.enDir  = GpioDirIn;

	if(idle_status == BaseIOStatus_HIGH){
		stcTIM3Port.enPu = GpioPuEnable;
	}else if(idle_status == BaseIOStatus_LOW){
		stcTIM3Port.enPd = GpioPdEnable;
	}
	
	Gpio_Init(port, pin, &stcTIM3Port);
	Gpio_SetAfMode(port,pin,cho_info->gpio_af);

	
	/* Timer3 Mode.*/
	stc_tim3_mode1_cfg_t stcTim3BaseCfg;
	DDL_ZERO_STRUCT(stcTim3BaseCfg);
	Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3, TRUE);
	
	stcTim3BaseCfg.enWorkMode = Tim3WorkMode1;
	stcTim3BaseCfg.enCT = Tim3Timer;
	stcTim3BaseCfg.enPRS = Tim3PCLKDiv1;
	stcTim3BaseCfg.enOneShot = Tim3PwcOneShotDetect;
	Tim3_Mode1_Init(&stcTim3BaseCfg);
	
	/* Timer3 PWC */
	stc_tim3_pwc_input_cfg_t stcTim3PwcInCfg;
	DDL_ZERO_STRUCT(stcTim3PwcInCfg);
	stcTim3PwcInCfg.enTsSel = cho_info->mscr_ts;
	if(cho_info->mscr_ts == Tim3Ts6IAFP){
		stcTim3PwcInCfg.enIA0Sel = Tim3IA0Input;
		stcTim3PwcInCfg.enFltIA0 = Tim3FltPCLKDiv16Cnt3;
	}else{
		stcTim3PwcInCfg.enIB0Sel = Tim3IB0Input;
		stcTim3PwcInCfg.enFltIB0 = Tim3FltPCLKDiv16Cnt3;
	}
	Tim3_M1_Input_Cfg(&stcTim3PwcInCfg);
	
	if(pwc == AdvanceIO_PWC_R2R){
		
		Tim3_M1_PWC_Edge_Sel(Tim3PwcRiseToRise);
		
	}else if(pwc == AdvanceIO_PWC_F2F){
		
		Tim3_M1_PWC_Edge_Sel(Tim3PwcFallToFall);
		
	}else if(pwc == AdvanceIO_PWC_R2F){
		
		Tim3_M1_PWC_Edge_Sel(Tim3PwcRiseToFall);
		
	}else if(pwc == AdvanceIO_PWC_F2R){
		
		Tim3_M1_PWC_Edge_Sel(Tim3PwcFallToRise);
		
	}else{
		LLA_ASSERT(0,DriverCode_AdvanceIO,AdvanceIO_errorPWC);
		return 0;
	}
	
	Tim3_M1_Cnt16Set(0);
	
	Tim3_ClearIntFlag(Tim3UevIrq);                            //清Uev中断标志
	Tim3_ClearIntFlag(Tim3CA0Irq);                            //清捕捉中断标志
	Tim3_Mode1_EnableIrq(Tim3UevIrq);                         //使能TIM3溢出中断
	Tim3_Mode1_EnableIrq(Tim3CA0Irq);                         //使能TIM3捕获中断    
	EnableNvic(TIM3_IRQn, IrqLevel3, TRUE);                   //TIM3中断使能
	
	timer3_ovr_count = 0;
	Tim3_M1_Run();
	
	/* block untill measure finished */
	measure_ok = 0;
	uint32_t start_ms = LLA_SYS_Time_Millis();
	if(time_out){
		while(!measure_ok && LLA_SYS_Time_ConsumeMillis(start_ms)< time_out);
		return measure_ok? timer3_pwc_val/TIMER3_TICK_PERSECONDS : 0;
	}else{
		while(!measure_ok);
		return timer3_pwc_val/TIMER3_TICK_PERSECONDS;
	}
}



/************************ Your Interrupt handling function ****************************/
void TIM3_IRQHandler(void){
	if(Tim3_GetIntFlag(Tim3UevIrq)){
		timer3_ovr_count++;
		Tim3_ClearIntFlag(Tim3UevIrq);
	}
	if(Tim3_GetIntFlag(Tim3CA0Irq)){
		
		uint16_t  CCR0A_val = Tim3_M1_PWC_CapValueGet();
		timer3_pwc_val = timer3_ovr_count * 0x10000 + CCR0A_val;
		measure_ok = 1;
		Tim3_ClearIntFlag(Tim3CA0Irq);
	}
	
}

