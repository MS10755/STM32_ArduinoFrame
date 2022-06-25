#if defined(ARDUINO_STM32)
#include "stm32f10x.h"                  // Device header
#include "SdCardInfo.h"
#include "SdioCard.h"
#include <string.h>
#include "sdio_sdcard.h"


#define ASSERT_RET(e,deal) 	\
	do{											 	\
			if(!(e)){							\
				deal;								\
			}											\
		}												\
	while(0)									\



static uint8_t error_code_last;
#define SDIO_SD_DEBUG
//==============================================================================
#if defined(SDIO_SD_DEBUG)
#define SDIO_SD_DEBUG_Print(code)  printSdErrorText(&Serial,code);Serial.println()
#else
#define SDIO_SD_DEBUG_Print(code) 
#endif

extern "C" {
 void SD_LowLevel_Init(){
//	GPIO_InitTypeDef GPIO_InitStructure;
//	//SDIO IO口初始化
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);//使能PORTC,PORTD时钟
//	
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO|RCC_AHBPeriph_DMA2,ENABLE);//使能SDIO,DMA2时钟	 
//  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;	//PC.8~12 复用输出
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
//  GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化PC.8~12 
// 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//PD2 复用输出
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
//  GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化PD2
	 
}

void SD_LowLevel_DeInit(){


}

void SD_LowLevel_DMA_RxConfig(uint32_t * buf,uint32_t blocksize){


}

void SD_LowLevel_DMA_TxConfig(uint32_t * buf,uint32_t blocksize){


}

void SD_DMAEndOfTransferStatus(){


}

}


uint64_t getByteBit(uint32_t *buf,uint8_t start_bit,uint8_t end_bit){
	uint8_t buf_index,buf_bit_index;
	uint64_t d = 0,j=0;
	for(uint8_t i=start_bit;i<=end_bit;i++,j++){
		buf_index = i/32;
		buf_bit_index = i%32;
		if(buf[buf_index]&((uint32_t)1<<buf_bit_index)){
			d |= ((uint64_t)1<<j);
		}else{
			
		}
	}
	return d;
}

bool SdioCard::begin(SdioConfig sdioConfig) {
	ASSERT_RET(SD_Init()==SD_OK,SDIO_SD_DEBUG_Print(SD_CARD_ERROR_INIT_FAILED);return false;);
	return true;
}

 bool SdioCard::erase(uint32_t firstSector, uint32_t lastSector){
//	 ASSERT_RET(SD_Erase(firstSector,lastSector)==SD_OK,SDIO_SD_DEBUG_Print(SD_CARD_ERROR_ERASE_FAILED);return false;);
	 
	 return false;
 };
  /**
   * \return code for the last error. See SdCardInfo.h for a list of error codes.
   */
  uint8_t SdioCard::errorCode() const{
		
		return error_code_last;
	};
  /** \return error data for last error. */
  uint32_t SdioCard::errorData() const{
		return 0;
	};
  /** \return error line for last error. Tmp function for debug. */
  uint32_t SdioCard::errorLine() const{
	
		return 0;
	}
  /**
   * Check for busy with CMD13.
   *
   * \return true if busy else false.
   */
  bool SdioCard::isBusy(){
		SDCardState state;
		state = SD_GetState();
		return false;
	}
  /** \return the SD clock frequency in kHz. */
  uint32_t SdioCard::kHzSdClk(){
		return (72000/(2 + SDIO_TRANSFER_CLK_DIV));
	}
  /**
   * Read a 512 byte sector from an SD card.
   *
   * \param[in] sector Logical sector to be read.
   * \param[out] dst Pointer to the location that will receive the data.
   * \return true for success or false for failure.
   */
  bool SdioCard::readSector(uint32_t sector, uint8_t* dst){
		SD_Error error;
		ASSERT_RET((error=SD_ReadDisk(dst,sector,1))==SD_OK,Serial.printf("readSector[%X,%X] error code:%d\r\n",sector,dst,error);return false;);
		return true;
	}
  /**
   * Read multiple 512 byte sectors from an SD card.
   *
   * \param[in] sector Logical sector to be read.
   * \param[in] ns Number of sectors to be read.
   * \param[out] dst Pointer to the location that will receive the data.
   * \return true for success or false for failure.
   */
  bool SdioCard::readSectors(uint32_t sector, uint8_t* dst, size_t ns){
		SD_Error error;
		ASSERT_RET((error=SD_ReadDisk(dst,sector,ns))==SD_OK,Serial.printf("readSector[%X,%X] error code:%d\r\n",sector,dst,error);return false;);
		return true;
	}
  /**
   * Read a card's CID register. The CID contains card identification
   * information such as Manufacturer ID, Product name, Product serial
   * number and Manufacturing date.
   *
   * \param[out] cid pointer to area for returned data.
   *
   * \return true for success or false for failure.
   */
  bool SdioCard::readCID(cid_t* cid){
		SD_CardInfo info;
		ASSERT_RET(SD_GetCardInfo(&info)==SD_OK,return false);
		cid->crc = info.SD_cid.CID_CRC;
		cid->always1 = 1;
		cid->mdt_month = info.SD_cid.ManufactDate&0x0F;
		cid->mdt_year_low = (info.SD_cid.ManufactDate>>4) & 0x0F;
		cid->mdt_year_high = (info.SD_cid.ManufactDate>>8) & 0x0F;
		cid->psn = info.SD_cid.ProdSN;
		cid->prv_m = info.SD_cid.ProdRev& 0x0F;
		cid->prv_n = (info.SD_cid.ProdRev>>4)& 0x0F;
		memcpy(cid->pnm,(uint8_t*)&(info.SD_cid.ProdName1),4);
		cid->pnm[4] = info.SD_cid.ProdName2;
		memcpy(cid->oid,(uint8_t*)&(info.SD_cid.OEM_AppliID),2);
		cid->mid = info.SD_cid.ManufacturerID;
		return true;
	}
  /**
   * Read a card's CSD register. The CSD contains Card-Specific Data that
   * provides information regarding access to the card's contents.
   *
   * \param[out] csd pointer to area for returned data.
   *
   * \return true for success or false for failure.
   */
  bool SdioCard::readCSD(csd_t* csd){
		SD_CardInfo info;
		ASSERT_RET(SD_GetCardInfo(&info)==SD_OK,return false);
		if(info.SD_csd.CSDStruct == 1){
			/*V2.0*/
			csd->v2.crc = info.SD_csd.CSD_CRC;
			csd->v2.always1 = 1;
			csd->v2.file_format_grp = info.SD_csd.FileFormatGrouop;
			csd->v2.copy =  info.SD_csd.CopyFlag;
			csd->v2.perm_write_protect =  info.SD_csd.PermWrProtect;
			csd->v2.tmp_write_protect =  info.SD_csd.TempWrProtect;
			csd->v2.file_format =  info.SD_csd.FileFormat;
			csd->v2.write_bl_len_low =  info.SD_csd.MaxWrBlockLen & 0x03;
			csd->v2.write_bl_len_high =  (info.SD_csd.MaxWrBlockLen>>2) & 0x03;
			csd->v2.write_partial =  info.SD_csd.WriteBlockPaPartial;
			csd->v2.wp_grp_enable =  info.SD_csd.WrProtectGrEnable;
			csd->v2.wp_grp_size =  info.SD_csd.WrProtectGrSize;
			csd->v2.r2w_factor =  info.SD_csd.WrSpeedFact;
			csd->v2.sector_size_low =  info.SD_csd.EraseGrMul & 0x01;
			csd->v2.sector_size_high =  info.SD_csd.EraseGrMul >>1;
			csd->v2.erase_blk_en =  info.SD_csd.EraseGrSize;
			csd->v2.c_size_low =  info.SD_csd.DeviceSize & 0xFF;
			csd->v2.c_size_mid =  (info.SD_csd.DeviceSize>>8) & 0xFF;
			csd->v2.c_size_high =  (info.SD_csd.DeviceSize>>16) & 0xFF;
			csd->v2.read_bl_partial =  info.SD_csd.PartBlockRead;
			csd->v2.write_blk_misalign =  info.SD_csd.WrBlockMisalign;
			csd->v2.read_blk_misalign =  info.SD_csd.RdBlockMisalign;
			csd->v2.dsr_imp =  info.SD_csd.DSRImpl;
			csd->v2.ccc_low =  info.SD_csd.CardComdClasses & 0x0F;
			csd->v2.ccc_high =  info.SD_csd.CardComdClasses >>4;
			csd->v2.read_bl_len =  info.SD_csd.RdBlockLen;
			csd->v2.tran_speed = info.SD_csd.MaxBusClkFrec;
			csd->v2.nsac = info.SD_csd.NSAC;
			csd->v2.taac = info.SD_csd.TAAC;
			csd->v2.csd_ver = info.SD_csd.CSDStruct;
		}else{
			/*V1.0*/
		
		
		}
		return true;
	}
  /** Read one data sector in a multiple sector read sequence
   *
   * \param[out] dst Pointer to the location for the data to be read.
   *
   * \return true for success or false for failure.
   */
  bool SdioCard::readData(uint8_t* dst){

		
		return false;
	}
  /** Read OCR register.
   *
   * \param[out] ocr Value of OCR register.
   * \return true for success or false for failure.
   */
  bool SdioCard::readOCR(uint32_t* ocr){
//		ASSERT_RET(SD_ReadOCR(ocr)==SD_OK,return false;);
		*ocr = 0;
		return true;
	}
  /** Start a read multiple sectors sequence.
   *
   * \param[in] sector Address of first sector in sequence.
   *
   * \note This function is used with readData() and readStop() for optimized
   * multiple sector reads.  SPI chipSelect must be low for the entire sequence.
   *
   * \return true for success or false for failure.
   */
  bool SdioCard::readStart(uint32_t sector){
	
	return false;
	}
  /** Start a read multiple sectors sequence.
   *
   * \param[in] sector Address of first sector in sequence.
   * \param[in] count Maximum sector count.
   * \note This function is used with readData() and readStop() for optimized
   * multiple sector reads.  SPI chipSelect must be low for the entire sequence.
   *
   * \return true for success or false for failure.
   */
  bool SdioCard::readStart(uint32_t sector, uint32_t count){
	
	return false;
	}
  /** End a read multiple sectors sequence.
   *
   * \return true for success or false for failure.
   */
  bool SdioCard::readStop(){
		return false;
	}
  /** \return SDIO card status. */
  uint32_t SdioCard::status(){
		uint32_t sd_status;
//		ASSERT_RET(SD_SendStatus(&sd_status)==SD_OK,return 0;);
		return sd_status;
	}
    /**
   * Determine the size of an SD flash memory card.
   *
   * \return The number of 512 byte data sectors in the card
   *         or zero if an error occurs.
   */
  uint32_t SdioCard::sectorCount(){
		SD_CardInfo info;
		ASSERT_RET(SD_GetCardInfo(&info)==SD_OK,return 0);
		return (info.SD_csd.DeviceSize+1)*1024;
	}
  /**
   *  Send CMD12 to stop read or write.
   *
   * \param[in] blocking If true, wait for command complete.
   *
   * \return true for success or false for failure.
   */
  bool SdioCard::stopTransmission(bool blocking){
	
	return false;
	}
  /** \return success if sync successful. Not for user apps. */
  bool SdioCard::syncDevice(){
//		if(SD_GetStatus()!=SD_TRANSFER_OK){
//			return false;
//		};
		return true;
	}
  /** Return the card type: SD V1, SD V2 or SDHC
   * \return 0 - SD V1, 1 - SD V2, or 3 - SDHC.
   */
  uint8_t SdioCard::type() const{
		SD_CardInfo info;
		ASSERT_RET(SD_GetCardInfo(&info)==SD_OK,return 0);;
		return info.CardType+1;
	}
  /**
   * Writes a 512 byte sector to an SD card.
   *
   * \param[in] sector Logical sector to be written.
   * \param[in] src Pointer to the location of the data to be written.
   * \return true for success or false for failure.
   */
  bool SdioCard::writeSector(uint32_t sector, const uint8_t* src){
		SD_Error error;
		ASSERT_RET((error=SD_WriteDisk((uint8_t *)src,sector,1))==SD_OK,Serial.printf("writeSector error code:%d\r\n",error);return false;);
		return true;
	}
  /**
   * Write multiple 512 byte sectors to an SD card.
   *
   * \param[in] sector Logical sector to be written.
   * \param[in] ns Number of sectors to be written.
   * \param[in] src Pointer to the location of the data to be written.
   * \return true for success or false for failure.
   */
  bool SdioCard::writeSectors(uint32_t sector, const uint8_t* src, size_t ns){
		SD_Error error;
		ASSERT_RET((error=SD_WriteDisk((uint8_t *)src,sector,ns))==SD_OK,Serial.printf("writeSector error code:%d\r\n",error);return false;);
		return true;
	}
  /** Write one data sector in a multiple sector write sequence.
   * \param[in] src Pointer to the location of the data to be written.
   * \return true for success or false for failure.
   */
  bool SdioCard::writeData(const uint8_t* src){
	
	return false;
	}
  /** Start a write multiple sectors sequence.
   *
   * \param[in] sector Address of first sector in sequence.
   *
   * \note This function is used with writeData() and writeStop()
   * for optimized multiple sector writes.
   *
   * \return true for success or false for failure.
   */
  bool SdioCard::writeStart(uint32_t sector){
	
	return false;
	}
  /** Start a write multiple sectors sequence.
   *
   * \param[in] sector Address of first sector in sequence.
   * \param[in] count Maximum sector count.
   * \note This function is used with writeData() and writeStop()
   * for optimized multiple sector writes.
   *
   * \return true for success or false for failure.
   */
  bool SdioCard::writeStart(uint32_t sector, uint32_t count){
	
	return false;
	}

  /** End a write multiple sectors sequence.
   *
   * \return true for success or false for failure.
   */
  bool SdioCard::writeStop(){
	
	return false;
	}

#endif