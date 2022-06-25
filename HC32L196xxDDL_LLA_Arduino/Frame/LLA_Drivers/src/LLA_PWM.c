#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "timer3.h"
#include "gpio.h"
#include "ddl.h"

/************************ User type define ****************************/
typedef struct{
	BaseIO_name_t name;
	en_gpio_af_t gpio_af;
	en_tim3_channel_t channelx;//通道
	en_tim3_m23_ccrx_t ccrx;//比较寄存器
}timer3_cmp_info_t;

/************************ User user variables ****************************/
static const timer3_cmp_info_t timer3_cmp_info_list[]={
	{PA8,GpioAf2,Tim3CH0,Tim3CCR0A},
	{PB3,GpioAf4,Tim3CH0,Tim3CCR0A},
	{PB6,GpioAf6,Tim3CH0,Tim3CCR0A},
	{PE9,GpioAf1,Tim3CH0,Tim3CCR0A},
	{PA7,GpioAf4,Tim3CH0,Tim3CCR0B},
	{PB4,GpioAf6,Tim3CH0,Tim3CCR0B},
	{PB13,GpioAf3,Tim3CH0,Tim3CCR0B},
	{PE8,GpioAf1,Tim3CH0,Tim3CCR0B},
	{PE14,GpioAf1,Tim3CH0,Tim3CCR0B},
	{PA3,GpioAf6,Tim3CH1,Tim3CCR1A},
	{PA9,GpioAf2,Tim3CH1,Tim3CCR1A},
	{PA15,GpioAf6,Tim3CH1,Tim3CCR1A},
	{PB10,GpioAf5,Tim3CH1,Tim3CCR1A},
	{PE11,GpioAf1,Tim3CH1,Tim3CCR1A},
	{PB0,GpioAf2,Tim3CH1,Tim3CCR1B},
	{PB14,GpioAf3,Tim3CH1,Tim3CCR1B},
	{PC13,GpioAf3,Tim3CH1,Tim3CCR1B},
	{PE10,GpioAf1,Tim3CH1,Tim3CCR1B},
	{PA10,GpioAf2,Tim3CH2,Tim3CCR2A},
	{PA14,GpioAf3,Tim3CH2,Tim3CCR2A},
	{PB8,GpioAf6,Tim3CH2,Tim3CCR2A},
	{PE13,GpioAf1,Tim3CH2,Tim3CCR2A},
	{PA4,GpioAf7,Tim3CH2,Tim3CCR2B},
	{PB1,GpioAf3,Tim3CH2,Tim3CCR2B},
	{PB15,GpioAf2,Tim3CH2,Tim3CCR2B},
	{PE12,GpioAf1,Tim3CH2,Tim3CCR2B},
};

static const uint32_t timer3_clk_table[]={48000000,24000000,12000000,6000000,3000000,1500000,750000,187500};
static uint8_t timer3_clk_index;
static uint32_t pwm_hz;
/************************ User micro define ****************************/
#define TIMER3_CMP_INFO_LIST_LEN	(sizeof(timer3_cmp_info_list)/sizeof(timer3_cmp_info_list[0]))
#define TIMER3_CLK_TABLE_LEN	(sizeof(timer3_clk_table)/sizeof(timer3_clk_table[0]))
#define IS_PWM_POLAR(x) (x<_PWM_POLAR_MAX)
#define TIMER3_CLK	(timer3_clk_table[timer3_clk_index])
#define HZ2ARR(x)	((x<4 || x>TIMER3_CLK)?(0):TIMER3_CLK/x)
#define IS_PWM_HZ(x) (x>4 || x<timer3_clk_table[0])

/************************ User static function ****************************/
static en_gpio_port_t get_port(BaseIO_name_t name){
	uint8_t port_index = name / 16;
	if(port_index == 0)return GpioPortA;
	if(port_index == 1)return GpioPortB;
	if(port_index == 2)return GpioPortC;
	if(port_index == 3)return GpioPortD;
	if(port_index == 4)return GpioPortE;
	if(port_index == 5)return GpioPortF;
	else{
		LLA_ASSERT(0,DriverCode_PWM,PWM_errorIOOutChipSupport);
	}
	return (en_gpio_port_t)0xFFFFF;// Just for no compile warnning.
}

static en_gpio_pin_t get_pin(BaseIO_name_t name){
	return (en_gpio_pin_t)(name % 16);
}

static const timer3_cmp_info_t * get_cmp_info(BaseIO_name_t name){
	for(uint8_t i=0;i<TIMER3_CMP_INFO_LIST_LEN;i++){
		if(name== timer3_cmp_info_list[i].name){
			return &timer3_cmp_info_list[i];
		}
	}
	LLA_ASSERT(0,DriverCode_PWM,PWM_errorIONotSupportPWM);
	return NULL;
}

static uint8_t get_best_clk_div(uint32_t hz){
	for(uint8_t i=0;i<TIMER3_CLK_TABLE_LEN;i++){
		if((float)timer3_clk_table[i]/hz <=65535){
			return i;
		}
	}
	return 0;
}

static void pwm_pin_init(en_gpio_port_t port,en_gpio_pin_t pin,en_gpio_af_t af){
	stc_gpio_cfg_t stcTIM3Port;
	
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
	DDL_ZERO_STRUCT(stcTIM3Port);
	stcTIM3Port.enDir = GpioDirOut;
	Gpio_Init(port,pin,&stcTIM3Port);
	Gpio_SetAfMode(port,pin,af);
}

static void pwm_pin_deint(en_gpio_port_t port,en_gpio_pin_t pin){
	stc_gpio_cfg_t stcTIM3Port;
	DDL_ZERO_STRUCT(stcTIM3Port);
	Gpio_Init(port,pin,&stcTIM3Port);
};


static void timer3_init(uint32_t hz){
	timer3_clk_index = get_best_clk_div(hz);
	
	stc_tim3_mode23_cfg_t stcTim3BaseCfg;
	
	Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3, TRUE);
	DDL_ZERO_STRUCT(stcTim3BaseCfg);
	stcTim3BaseCfg.enWorkMode = Tim3WorkMode2;
	stcTim3BaseCfg.enCT = Tim3Timer;
	stcTim3BaseCfg.enPRS = (en_tim3_cr_timclkdiv_t)timer3_clk_index;//2.7hz ~178.5Khz
	stcTim3BaseCfg.enCntDir = Tim3CntUp;
	stcTim3BaseCfg.enPWMTypeSel = Tim3IndependentPWM;
	stcTim3BaseCfg.enPWM2sSel = Tim3SinglePointCmp;
	
	Tim3_Mode23_Init(&stcTim3BaseCfg);
	Tim3_M23_ARRSet(HZ2ARR(hz),TRUE);
	Tim3_M23_SetValidPeriod(0);
	Tim3_M23_Cnt16Set(0);
	Tim3_ClearAllIntFlag();
	Tim3_M23_Run();
}

/************************ LLA common micro define ****************************/

/************************ LLA common variables ****************************/


/************************ LLA common function ****************************/


/************************ LLA Interface function ****************************/

void LLA_PWM_BaseHZ(uint32_t hz){
	LLA_ASSERT(IS_PWM_HZ(hz),DriverCode_PWM, PWM_errorRange);
	timer3_init(hz);
	pwm_hz = hz;
}

void LLA_PWM_Output1(BaseIO_name_t name,uint8_t duty,PWM_Polar_t polar){
	LLA_ASSERT(IS_PWM_POLAR(polar),DriverCode_PWM,PWM_errorPolar);

	en_gpio_port_t port = get_port(name);
	if(port == (en_gpio_port_t)0xFFFFF) return;
	en_gpio_pin_t pin = get_pin(name);
	
	const timer3_cmp_info_t *cmp_info = get_cmp_info(name);
	
	stc_tim3_m23_compare_cfg_t stcTim3PortCmpCfg;
	DDL_ZERO_STRUCT(stcTim3PortCmpCfg);
	
	if(cmp_info->ccrx == Tim3CCR0A || cmp_info->ccrx == Tim3CCR1A || cmp_info->ccrx == Tim3CCR2A){
		stcTim3PortCmpCfg.enCHxACmpCtrl = (polar == PWM_Polar_HIGH? Tim3PWMMode1:Tim3PWMMode2);
    stcTim3PortCmpCfg.enCHxAPolarity = Tim3PortPositive;     
    stcTim3PortCmpCfg.bCHxACmpBufEn = TRUE;
    stcTim3PortCmpCfg.enCHxACmpIntSel = Tim3CmpIntNone;
	}else{
		stcTim3PortCmpCfg.enCHxBCmpCtrl = (polar == PWM_Polar_HIGH? Tim3PWMMode1:Tim3PWMMode2);
    stcTim3PortCmpCfg.enCHxBPolarity = Tim3PortPositive;
    stcTim3PortCmpCfg.bCHxBCmpBufEn = TRUE;
    stcTim3PortCmpCfg.enCHxBCmpIntSel = Tim3CmpIntNone;
	}
	Tim3_M23_PortOutput_Cfg(cmp_info->channelx,&stcTim3PortCmpCfg);
	
	Tim3_M23_CCR_Set(cmp_info->ccrx,HZ2ARR(pwm_hz)*((float)duty/256));
	
	/* 3. GPIO init */
	pwm_pin_init(port,pin,cmp_info->gpio_af);
	Tim3_M23_EnPWM_Output(TRUE, FALSE);
}

void LLA_PWM_OutPut2(BaseIO_name_t name,uint32_t hz,uint8_t duty,PWM_Polar_t polar){
	LLA_ASSERT(IS_PWM_POLAR(polar),DriverCode_PWM,PWM_errorPolar);
	LLA_ASSERT(IS_PWM_HZ(hz),DriverCode_PWM, PWM_errorRange);
	
	LLA_PWM_BaseHZ(hz);
	LLA_PWM_Output1(name,duty,polar);
}

void LLA_PWM_Stop(BaseIO_name_t name){
	en_gpio_port_t port = get_port(name);
	if(port == (en_gpio_port_t)0xFFFFF) return;
	en_gpio_pin_t pin = get_pin(name);
	pwm_pin_deint(port,pin);
}
void LLA_PWM_Resume(BaseIO_name_t name){
	en_gpio_port_t port = get_port(name);
	if(port == (en_gpio_port_t)0xFFFFF) return;
	en_gpio_pin_t pin = get_pin(name);
	const timer3_cmp_info_t *cmp_info = get_cmp_info(name);
	pwm_pin_init(port,pin,cmp_info->gpio_af);
}

/************************ Your Interrupt handling function ****************************/


