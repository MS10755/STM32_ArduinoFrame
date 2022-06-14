#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "stm32f10x.h"                  // Device header


/************************ User type define ****************************/
typedef struct{
	GPIO_TypeDef * GPIO_x;
	uint16_t Pin_x;
}IOMAPS_t;

/************************ User user variables ****************************/
const IOMAP_Port_t IOMAP_Port_list[_LlA_PORT_MAX]={
	{(uint32_t *)(GPIOA_BASE+0x0C),(uint32_t *)(GPIOA_BASE+0x08)},
	{(uint32_t *)(GPIOB_BASE+0x0C),(uint32_t *)(GPIOB_BASE+0x08)},
	{(uint32_t *)(GPIOC_BASE+0x0C),(uint32_t *)(GPIOC_BASE+0x08)},
	{(uint32_t *)(GPIOD_BASE+0x0C),(uint32_t *)(GPIOD_BASE+0x08)},
	{(uint32_t *)(GPIOE_BASE+0x0C),(uint32_t *)(GPIOE_BASE+0x08)},
	{(uint32_t *)(GPIOF_BASE+0x0C),(uint32_t *)(GPIOF_BASE+0x08)},
	{(uint32_t *)(GPIOG_BASE+0x0C),(uint32_t *)(GPIOG_BASE+0x08)},
};

/************************ User micro define ****************************/


/************************ User static function ****************************/


/************************ LLA common micro define ****************************/

/************************ LLA common variables ****************************/


/************************ LLA common function ****************************/


/************************ LLA Interface function ****************************/
IOMAP_NameMask_t LLA_IOMaps_NameMask(BaseIO_name_t name){
	return (IOMAP_NameMask_t) 1<<(name % 16);
}

IOMAP_Port_t LLA_IOMaps_NameToPort(BaseIO_name_t name){
	return IOMAP_Port_list[name/16];
}

/************************ Your Interrupt handling function ****************************/


