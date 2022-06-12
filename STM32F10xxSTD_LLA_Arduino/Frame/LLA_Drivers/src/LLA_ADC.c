/*
 * @Description: 
 * @Autor: Juan
 * @Date: 2022-04-01 14:49:54
 * @LastEditors: Juan
 * @LastEditTime: 2022-04-01 15:18:35
 */
#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "stm32f10x.h"                  // Device header


/************************ LLA common type define ****************************/
typedef struct{
	BaseIO_name_t name;
	ADC_TypeDef *ADC_x;
	uint16_t Channel_x;
}adcPinInfo_t;

/************************ LLA common micro define ****************************/


/************************ LLA common variables ****************************/


/************************ User variables **********************************/
static const adcPinInfo_t adcPinInfos[]={
	{PA0,ADC1,ADC_Channel_0,},
	{PA1,ADC1,ADC_Channel_1,},
	{PA2,ADC1,ADC_Channel_2,},
	{PA3,ADC1,ADC_Channel_3,},
	{PA4,ADC1,ADC_Channel_4,},
	{PA5,ADC1,ADC_Channel_5,},
	{PA6,ADC1,ADC_Channel_6,},
	{PA7,ADC1,ADC_Channel_7,},
	{PB0,ADC1,ADC_Channel_8,},
	{PB1,ADC1,ADC_Channel_9,},
	{PC0,ADC1,ADC_Channel_10,},
	{PC1,ADC1,ADC_Channel_11,},
	{PC2,ADC1,ADC_Channel_12,},
	{PC3,ADC1,ADC_Channel_13,},
	{PC4,ADC1,ADC_Channel_14,},
	{PC5,ADC1,ADC_Channel_15,},
};


/************************ User function ****************************/
static GPIO_TypeDef * nameToPort(BaseIO_name_t name){
	switch(name/16){
		case 0:
			return GPIOA;
		case 1:
			return GPIOB;
		case 2:
			return GPIOC;
		case 3:
			return GPIOD;
		case 4:
			return GPIOE;
		case 5:
			return GPIOF;
		case 6:
			return GPIOG;
		default:
			LLA_ASSERT(0,DriverCode_BaseIO,BaseIO_errorIOOutChipSupport);
			break;
	};
	return NULL;
}

static uint16_t nameToPin(BaseIO_name_t name){
	return (uint16_t) 1 << ( name % 16 );
}

static const adcPinInfo_t* pinToInfo(BaseIO_name_t name){
	for(uint8_t i=0;i<sizeof(adcPinInfos)/sizeof(adcPinInfos[0]);i++){
		if(adcPinInfos[i].name == name){
			return &adcPinInfos[i];
		}
	}
	return NULL;
};

static void _ADC_init(ADC_TypeDef * ADC_x) {
    if(ADC_x==NULL) return;

    //1.初始化ADC时钟
    if(ADC_x==ADC1) 			RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
    else if(ADC_x==ADC2) 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);
    else if(ADC_x==ADC3) 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
    else return;

    ADC_DeInit(ADC_x);

    //2.设置ADC
    ADC_InitTypeDef ADC_InitStructure;

    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_Init(ADC_x,&ADC_InitStructure);

    ADC_Cmd(ADC_x,ENABLE);

    //校准ADC
    ADC_ResetCalibration(ADC_x);
    while(ADC_GetResetCalibrationStatus(ADC_x));
    ADC_StartCalibration(ADC_x);
    while(ADC_GetCalibrationStatus(ADC_x));
}

static uint16_t _ADC_GetValue(ADC_TypeDef* ADC_x, uint16_t ADC_Channel_x)
{
    if(ADC_x==NULL) return 0;
    ADC_RegularChannelConfig(ADC_x, ADC_Channel_x, 1, ADC_SampleTime_41Cycles5);
    ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
    while(!ADC_GetFlagStatus(ADC_x, ADC_FLAG_EOC));
    return ADC_GetConversionValue(ADC_x);
}

/************************ LLA common function ****************************/


/************************ LLA Interface function ****************************/
/**
 * @description: Init ADC module, this fun will be calling at Frame start.
 * @param {*}
 * @return {*}
 * @author: Your name
 */
void LLA_ADC_Init(){
	_ADC_init(ADC1); // Only Init ADC1 now.
}

/**
 * @description: 
 * @param {BaseIO_name_t} name: pin name
 * @return {*}
 * @author: Your name
 */
uint32_t LLA_ADC_Read(BaseIO_name_t name){
	const adcPinInfo_t *info =pinToInfo(name);
	LLA_ASSERT(info, DriverCode_ADC, ADC_errorPinNotChannel);
	if(info==NULL) return 0;
	
	GPIO_TypeDef * GPIO_x = nameToPort(name);
	if(GPIO_x == GPIOA){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	}else if(GPIO_x == GPIOB){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	}else if(GPIO_x == GPIOC){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	}else if(GPIO_x == GPIOD){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	}
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = nameToPin(name);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_x,&GPIO_InitStructure);
	
	return _ADC_GetValue(info->ADC_x, info->Channel_x);
}

/**
 * @description: 
 * @param {LLA_ADC_Special_channel_t} channel: special ADC channel
 * @return {*}
 * @author: Juan
 */
uint32_t LLA_ADC_ReadSpecial(LLA_ADC_Special_channel_t channel){
	if(channel == LLA_ADC_SPECIAL_CHANNEL_0){
		return _ADC_GetValue(ADC1, ADC_Channel_16);//有内部温度传感器被连接到 ADC1_IN16 通道
	}else{
		LLA_ASSERT(0, DriverCode_ADC, ADC_errorSpecialChannel);
	}
	return 0;
}

/************************ Your Interrupt handling function ****************************/


