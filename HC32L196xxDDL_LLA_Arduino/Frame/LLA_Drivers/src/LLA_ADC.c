#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "adc.h"
#include "gpio.h"
#include "bgr.h"

/************************ LLA common type define ****************************/


/************************ LLA common micro define ****************************/
#define IS_ADC_SPECIAL_CHANNEL(x) (x<_LLA_ADC_SPECIAL_CHANNEL_MAX)

/************************ LLA common variables ****************************/


/************************ User variables **********************************/


typedef struct{
	BaseIO_name_t pin;
	en_adc_samp_ch_sel_t adc_ch_x;
}ADC_channel_pin_map_t;

static const ADC_channel_pin_map_t ADC_channel_pin_map_list[]={
	{PA0, AdcExInputCH0},
	{PA1, AdcExInputCH1},
	{PA2, AdcExInputCH2},
	{PA3, AdcExInputCH3},
	{PA4, AdcExInputCH4},
	{PA5, AdcExInputCH5},
	{PA6, AdcExInputCH6},
	{PA7, AdcExInputCH7},
	{PB0, AdcExInputCH8},
	{PB1, AdcExInputCH9},
	{PC0, AdcExInputCH10},
	{PC1, AdcExInputCH11},
	{PC2, AdcExInputCH12},
	{PC3, AdcExInputCH13},
	{PC4, AdcExInputCH14},
	{PC5, AdcExInputCH15},
	{PB2, AdcExInputCH16},
	{PB10, AdcExInputCH17},
	{PB11, AdcExInputCH18},
	{PB12, AdcExInputCH19},
	{PB13, AdcExInputCH20},
	{PB14, AdcExInputCH21},
	{PB15, AdcExInputCH22},
	{PE15, AdcExInputCH23},
	{PE14, AdcExInputCH24},
	{PE13, AdcExInputCH25},
};

#define ADC_CHANNEL_PIN_MAP_LIST_LEN	(sizeof(ADC_channel_pin_map_list)/sizeof(ADC_channel_pin_map_list[0]))
/************************ User function ****************************/
static en_adc_samp_ch_sel_t get_channel(BaseIO_name_t name){
	for(uint8_t i=0;i<ADC_CHANNEL_PIN_MAP_LIST_LEN;i++){
		if(ADC_channel_pin_map_list[i].pin == name){
			return ADC_channel_pin_map_list[i].adc_ch_x;
		}
	}
	LLA_ASSERT(0,DriverCode_ADC,ADC_errorPinNotChannel);
	return (en_adc_samp_ch_sel_t)-1;
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
		LLA_ASSERT(0,DriverCode_ADC,ADC_errorIOOutChipSupport);
	}
	return (en_gpio_port_t)0xFFFFF;// Just for no compile warnning.
}

static en_gpio_pin_t get_pin(BaseIO_name_t name){
	return (en_gpio_pin_t)(name % 16);
}


/************************ LLA common function ****************************/


/************************ LLA Interface function ****************************/
void LLA_ADC_Init(){
	
	stc_adc_cfg_t stcAdcCfg;
	DDL_ZERO_STRUCT(stcAdcCfg);
	
	Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr,TRUE); 
	Bgr_BgrEnable();
	Bgr_TempSensorEnable();
	
	stcAdcCfg.enAdcMode         = AdcScanMode;              ///<采样模式-扫描
	stcAdcCfg.enAdcClkDiv       = AdcMskClkDiv8;            ///<采样分频-1
	stcAdcCfg.enAdcSampCycleSel = AdcMskSampCycle8Clk;      ///<采样周期数-8
	stcAdcCfg.enAdcRefVolSel    = AdcMskRefVolSelAVDD;      ///<参考电压选择-VCC
	stcAdcCfg.enAdcOpBuf        = AdcMskBufEnable;         ///<OP BUF配置-开
	stcAdcCfg.enInRef           = AdcMskInRefDisable;       ///<内部参考电压使能-关
	stcAdcCfg.enAdcAlign        = AdcAlignRight;               ///<转换结果对齐方式-右
	Adc_Init(&stcAdcCfg);
}

uint32_t LLA_ADC_Read(BaseIO_name_t name){
	en_adc_samp_ch_sel_t channel = get_channel(name);
	if((uint8_t)channel == 0xFF) return 0;
	
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); 
	en_gpio_port_t port = get_port(name);
	if(port == (en_gpio_port_t)0xFFFF) return 0;
	en_gpio_pin_t pin = get_pin(name);
	
	Gpio_SetAnalogMode(port,pin);
	
	//启动顺序转换
	stc_adc_sqr_cfg_t stcAdcSqrCfg;
	DDL_ZERO_STRUCT(stcAdcSqrCfg);
	
	stcAdcSqrCfg.bSqrDmaTrig = FALSE;
	stcAdcSqrCfg.enResultAcc = AdcResultAccDisable;
	stcAdcSqrCfg.u8SqrCnt = 1;
	Adc_SqrModeCfg(&stcAdcSqrCfg);
	
	Adc_CfgSqrChannel(AdcSQRCH0MUX,channel);
	
	Adc_SQR_Start();
	
	while(Adc_GetIrqStatus(AdcMskIrqSqr)!=TRUE);
	Adc_ClrIrqStatus(AdcMskIrqSqr);
	uint32_t val = Adc_GetSqrResult(AdcSQRCH0MUX);
	Adc_SQR_Stop();
	return val;
}

uint32_t LLA_ADC_ReadSpecial(LLA_ADC_Special_channel_t channel){
	LLA_ASSERT(0,DriverCode_ADC,ADC_errorSpecialOutChipSupport);
	return 0;
}

/************************ Your Interrupt handling function ****************************/


