#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "spi.h"
#include "gpio.h"
#include "ddl.h"
#include "reset.h"

/************************ User type define ****************************/
typedef struct{
	BaseIO_name_t name;
	en_gpio_af_t gpio_af;
}gpio_af_t;

typedef struct{
	M0P_SPI_TypeDef *SPIx;
	gpio_af_t cs;
	gpio_af_t sck;
	gpio_af_t miso;
	gpio_af_t mosi;
}spi_gpio_info_t;

/************************ User user variables ****************************/
static const spi_gpio_info_t spi_gpio_info_list[]={
	{M0P_SPI0,{PA4,GpioAf1},{PA5,GpioAf1},{PA6,GpioAf1},{PA7,GpioAf1},},
	{M0P_SPI1,{PB12,GpioAf1},{PB13,GpioAf1},{PB14,GpioAf1},{PB15,GpioAf1},},
};

/************************ User micro define ****************************/
#define IS_CHIP_SPI(x) (x<=LLA_SPI1)
#define SPI_CLK 			48000000

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
		LLA_ASSERT(0,DriverCode_SPI,SPI_errorIOOutChipSupport);
	}
	return (en_gpio_port_t)0xFFFFF;// Just for no compile warnning.
}

static en_gpio_pin_t get_pin(BaseIO_name_t name){
	return (en_gpio_pin_t)(name % 16);
}

static const spi_gpio_info_t * get_info(LLA_SPI_t spi){
	if(spi == LLA_SPI0){
		return &spi_gpio_info_list[0];
	}else if(spi == LLA_SPI1){
		return &spi_gpio_info_list[1];
	}else{
		LLA_ASSERT(0,DriverCode_SPI,SPI_errorSPIOutChipSupport);
	}
	return NULL;
}

/************************ LLA common micro define ****************************/
#define IS_LLA_SPI(x) (x<_LLA_SPI_MAX)
#define IS_SPI_MODE(x) (x<_LLA_SPI_MODE_MAX)

/************************ LLA common variables ****************************/


/************************ LLA common function ****************************/


/************************ LLA Interface function ****************************/
void LLA_SPI_Init(LLA_SPI_t spi,uint32_t clock,LLA_SPI_Mode_t mode,LLA_SPI_CS_t cs_mode){
	LLA_ASSERT(IS_CHIP_SPI(spi),DriverCode_SPI,SPI_errorSPIOutChipSupport);
	LLA_ASSERT(IS_SPI_MODE(mode),DriverCode_SPI,SPI_errorMode);
	
	/* 1. init gpio */
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
	
	stc_gpio_cfg_t GpioInitStruct;
  DDL_ZERO_STRUCT(GpioInitStruct);
	
	GpioInitStruct.enDrv = GpioDrvH;
	GpioInitStruct.enDir = GpioDirOut;
	
	const spi_gpio_info_t* spi_info = get_info(spi);
	
	cs_mode = LLA_SPI_CS_soft;// disable auto cs.
	
	if(cs_mode == LLA_SPI_CS_Auto){
		/* CS */
		Gpio_Init(get_port(spi_info->cs.name),get_pin(spi_info->cs.name),&GpioInitStruct);
		Gpio_SetAfMode(get_port(spi_info->cs.name),get_pin(spi_info->cs.name),spi_info->cs.gpio_af);
	}

	/* SCK */
	Gpio_Init(get_port(spi_info->sck.name),get_pin(spi_info->sck.name),&GpioInitStruct);
	Gpio_SetAfMode(get_port(spi_info->sck.name),get_pin(spi_info->sck.name),spi_info->sck.gpio_af);

	/* MOSI */
	Gpio_Init(get_port(spi_info->mosi.name),get_pin(spi_info->mosi.name),&GpioInitStruct);
	Gpio_SetAfMode(get_port(spi_info->mosi.name),get_pin(spi_info->mosi.name),spi_info->mosi.gpio_af);
	
	
	GpioInitStruct.enDir = GpioDirIn;
	/* MISO */
	Gpio_Init(get_port(spi_info->miso.name),get_pin(spi_info->miso.name),&GpioInitStruct);
	Gpio_SetAfMode(get_port(spi_info->miso.name),get_pin(spi_info->miso.name),spi_info->miso.gpio_af);
	
	if(spi == LLA_SPI0){
		Sysctrl_SetPeripheralGate(SysctrlPeripheralSpi0,TRUE);
		Reset_RstPeripheral0(ResetMskSpi0);
	}else if(spi == LLA_SPI1){
		Sysctrl_SetPeripheralGate(SysctrlPeripheralSpi1,TRUE);
		Reset_RstPeripheral0(ResetMskSpi1);
	}else{
		return;
	}
	
	stc_spi_cfg_t SpiInitStruct;
	DDL_ZERO_STRUCT(SpiInitStruct);
	SpiInitStruct.enSpiMode = SpiMskMaster;     //配置位主机模式
  
	uint8_t clkDiv = 0;
	uint32_t now_clock = SPI_CLK / 2;
	while(clkDiv<7 && clock< now_clock){
		now_clock /=2;
		clkDiv++;
	}
	
	if(clkDiv<=3){
		SpiInitStruct.enPclkDiv = (en_spi_clk_div_t)clkDiv;
	}else if(clkDiv==4){
		SpiInitStruct.enPclkDiv = SpiClkMskDiv32;
	}else if(clkDiv==5){
		SpiInitStruct.enPclkDiv = SpiClkMskDiv64;
	}else if(clkDiv==6){
		SpiInitStruct.enPclkDiv = SpiClkMskDiv128;
	}else{
		SpiInitStruct.enPclkDiv = SpiClkMskDiv128;
	}
	
	if(mode == LLA_SPI_Mode_CLK_L_CPHA_L){
		
		SpiInitStruct.enCPHA = SpiMskCphafirst;  //第一边沿采样
		SpiInitStruct.enCPOL = SpiMskcpollow;    //极性为低
		
	}else if(mode == LLA_SPI_Mode_CLK_L_CPHA_H){
		
		SpiInitStruct.enCPHA = SpiMskCphafirst;  
		SpiInitStruct.enCPOL = SpiMskcpolhigh;  
		
	}else if(mode == LLA_SPI_Mode_CLK_H_CPHA_L){
		
		SpiInitStruct.enCPHA = SpiMskCphasecond;  
		SpiInitStruct.enCPOL = SpiMskcpollow;    
		
	}else if(mode == LLA_SPI_Mode_CLK_H_CPHA_H){
		
		SpiInitStruct.enCPHA = SpiMskCphasecond;  
		SpiInitStruct.enCPOL = SpiMskcpolhigh;    
		
	}
  
  Spi_Init(spi_info->SPIx, &SpiInitStruct);
}

void LLA_SPI_DeInit(LLA_SPI_t spi){
	LLA_ASSERT(IS_CHIP_SPI(spi),DriverCode_SPI,SPI_errorSPIOutChipSupport);
	if(spi == LLA_SPI0){
		Sysctrl_SetPeripheralGate(SysctrlPeripheralSpi0,FALSE);
		Reset_RstPeripheral0(ResetMskSpi0);
	}else if(spi == LLA_SPI1){
		Sysctrl_SetPeripheralGate(SysctrlPeripheralSpi1,FALSE);
		Reset_RstPeripheral0(ResetMskSpi1);
	}else{
		return;
	}
	const spi_gpio_info_t* spi_info = get_info(spi);
	Spi_DeInit(spi_info->SPIx);
}


void LLA_SPI_WriteBytes(LLA_SPI_t spi,uint8_t * data,uint32_t len){
	LLA_ASSERT(IS_CHIP_SPI(spi),DriverCode_SPI,SPI_errorSPIOutChipSupport);
	if(data==NULL || len==0)return;
	const spi_gpio_info_t * spi_info = get_info(spi);
	Spi_SetCS(spi_info->SPIx,FALSE);
	LLA_ASSERT(Spi_SendBuf(spi_info->SPIx,data,len)==Ok,DriverCode_SPI,SPI_errorWrite);
	Spi_SetCS(spi_info->SPIx,TRUE);
}

void LLA_SPI_ReadBytes(LLA_SPI_t spi,uint8_t * buf,uint32_t len){
	LLA_ASSERT(IS_CHIP_SPI(spi),DriverCode_SPI,SPI_errorSPIOutChipSupport);
	if(buf==NULL || len==0)return;
	const spi_gpio_info_t * spi_info = get_info(spi);
	Spi_SetCS(spi_info->SPIx,FALSE);
	LLA_ASSERT(Spi_ReceiveBuf(spi_info->SPIx,buf,len)==Ok,DriverCode_SPI,SPI_errorRead);
	Spi_SetCS(spi_info->SPIx,TRUE);
}

/************************ Your Interrupt handling function ****************************/


