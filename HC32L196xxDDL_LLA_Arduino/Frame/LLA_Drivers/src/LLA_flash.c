#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "flash.h"

/************************ User type define ****************************/


/************************ User user variables ****************************/


/************************ User micro define ****************************/
#define FLASH_START_ADDR	(0x3F000)
#define FLASH_END_ADDR		(0x3FFFF)
#define FLASH_SECTOR_SIZE	(512)


#define IS_FLASH_ADDR(x)  ((x>=FLASH_START_ADDR && x<=FLASH_END_ADDR))
/************************ User static function ****************************/


/************************ LLA common micro define ****************************/

/************************ LLA common variables ****************************/


/************************ LLA common function ****************************/


/************************ LLA Interface function ****************************/
void LLA_Flash_Init(){
	LLA_ASSERT(Flash_Init(12,TRUE)==Ok,DriverCode_Flash,FLASH_errorInit);
}


void LLA_Flash_Erase(uint32_t offset,uint32_t len){
	uint32_t first_sector_addr = FLASH_START_ADDR + ((offset)/FLASH_SECTOR_SIZE)*FLASH_SECTOR_SIZE;
	uint32_t last_sector_addr = FLASH_START_ADDR+((offset+len-1)/FLASH_SECTOR_SIZE)*FLASH_SECTOR_SIZE;
	
	LLA_ASSERT(IS_FLASH_ADDR(first_sector_addr),DriverCode_Flash,FLASH_errorAddr);
	LLA_ASSERT(IS_FLASH_ADDR(last_sector_addr),DriverCode_Flash,FLASH_errorAddr);
	
	for(uint32_t addr=first_sector_addr;addr<=last_sector_addr;addr+=FLASH_SECTOR_SIZE){
		LLA_ASSERT(Flash_SectorErase(addr)==Ok,DriverCode_Flash, FLASH_errorErase);
	}
}

void LLA_Flash_WriteBytes(uint32_t offset,uint8_t *data,uint32_t len){
	uint32_t first_addr = FLASH_START_ADDR + offset;
	uint32_t last_addr = FLASH_START_ADDR+ offset+ len -1;
	
	LLA_ASSERT(IS_FLASH_ADDR(first_addr),DriverCode_Flash,FLASH_errorAddr);
	LLA_ASSERT(IS_FLASH_ADDR(last_addr),DriverCode_Flash,FLASH_errorAddr);
	
	for(uint32_t addr = first_addr;addr<=last_addr;addr++){
		LLA_ASSERT(Flash_WriteByte(addr,*data)==Ok,DriverCode_Flash,FLASH_errorWrite);
		data++;
	}
}

void LLA_Flash_ReadBytes(uint32_t offset,uint8_t *data,uint32_t len){
	uint32_t first_addr = FLASH_START_ADDR + offset;
	uint32_t last_addr = FLASH_START_ADDR+ offset+ len -1;
	
	LLA_ASSERT(IS_FLASH_ADDR(first_addr),DriverCode_Flash,FLASH_errorAddr);
	LLA_ASSERT(IS_FLASH_ADDR(last_addr),DriverCode_Flash,FLASH_errorAddr);
	
	for(uint32_t addr = first_addr;addr<=last_addr;addr++){
		*data++=*(uint8_t*)addr;
	}
}

uint32_t LLA_Flash_GetSize(){
	return FLASH_END_ADDR-FLASH_START_ADDR+1;
}
/************************ Your Interrupt handling function ****************************/
uint32_t* Get_ChipID(void)
{
	static uint32_t ChipUniqueID[3];
	ChipUniqueID[0] = *(__IO uint32_t *)(0x00100E74+0x00); // ¸ß×Ö½Ú
	ChipUniqueID[1] = *(__IO uint32_t *)(0x00100E74+0x04); // 
	ChipUniqueID[2] = *(__IO uint32_t *)(0x00100E74+0x08); // µÍ×Ö½Ú
	return ChipUniqueID;
}

