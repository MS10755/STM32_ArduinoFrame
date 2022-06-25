#if defined(ARDUINO_STM32)
#include "stm32f10x.h"                  // Device header
#include "SdCardInfo.h"
#include "SdioCard.h"
#include <string.h>

#define SDIO_STATIC_FLAGS               ((u32)0x000005FF)

typedef struct{
	uint16_t RCA;
	uint32_t OCR;
	uint32_t CID[4];
	uint32_t CSD[4];
	uint64_t Capacity;
	uint32_t BlockSize;
	uint8_t Type;
}SD_card_info_t;

static SD_card_info_t card_info;

enum{
	SDIO_OK,
	SDIO_TIMEOUT,
	SDIO_CCRCFAIL,
};

#define VOLTAGE_ACCEPTED_3_3V		(0x0001)
#define VOLTAGE_ACCEPTED_LOW_V	(0x0010)


#define SD_VOLTAGE_WINDOW_SD            ((u32)0x80100000)
#define SD_HIGH_CAPACITY                ((u32)0x40000000)
#define SD_STD_CAPACITY                 ((u32)0x00000000)
#define SD_CHECK_PATTERN                ((u32)0x000001AA)
#define SD_VOLTAGE_WINDOW_MMC           ((u32)0x80FF8000)

#define ASSERT_RET(e,deal) 	\
	do{											 	\
			if(!(e)){							\
				deal;								\
			}											\
		}												\
	while(0)									\


static uint8_t SD_card_type = 0;


#define SDIO_SD_DEBUG
//==============================================================================
#if defined(SDIO_SD_DEBUG)
#define SDIO_SD_DEBUG_Print(code)  printSdErrorText(&Serial,code);Serial.println()
#else
#define SDIO_SD_DEBUG_Print(code) 
#endif


static void _SDIO_Init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	//SDIO IO口初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);//使能PORTC,PORTD时钟
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO|RCC_AHBPeriph_DMA2,ENABLE);//使能SDIO,DMA2时钟	 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;	//PC.8~12 复用输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化PC.8~12 
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//PD2 复用输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化PD2

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	//PD7 上拉输入
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化PD7	

	SDIO_InitTypeDef SDIO_InitStructure;
	/*初始化时的时钟不能大于400KHz*/ 
  SDIO_InitStructure.SDIO_ClockDiv = 0xB2;	/* HCLK = 72MHz, SDIOCLK = 72MHz, SDIO_CK = HCLK/(178 + 2) = 400 KHz */
  SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
  SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;  //不使用bypass模式，直接用HCLK进行分频得到SDIO_CK
  SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;	// 空闲时不关闭时钟电源
  SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;	 				//1位数据线
  SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;//硬件流
  SDIO_Init(&SDIO_InitStructure);
 
	SDIO_SetPowerState(SDIO_PowerState_ON);	//上电状态,开启卡时钟
 	  
	SDIO_ClockCmd(ENABLE);//SDIOCK使能 
}

static void SdioSendCmd(uint32_t cmdIndex,uint32_t arg,uint32_t responseType){
	SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
	SDIO_CmdInitStructure.SDIO_CmdIndex = cmdIndex;
	SDIO_CmdInitStructure.SDIO_Argument = arg;
	SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
	SDIO_CmdInitStructure.SDIO_Response = responseType;
	SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
	SDIO_SendCommand(&SDIO_CmdInitStructure);
}


/**
  * @brief	:	等待 SDIO 响应
  * @param	: 
  * @param	: 
  * @retval	:	
  */
static int8_t SdioWaitCmdResponse(uint32_t responseType,uint16_t timeout = 0xFFFF){
	while(timeout --){
		if(responseType == SDIO_Response_No){
			/* 期待 CMDSENT标志 */
				if(SDIO_GetFlagStatus(SDIO_FLAG_CMDSENT)){
					SDIO_ClearFlag(SDIO_STATIC_FLAGS);
					return SDIO_OK;
				}
		}else if(responseType == SDIO_Response_Short || responseType == SDIO_Response_Long){
			if(SDIO_GetFlagStatus(SDIO_FLAG_CMDREND)){
				SDIO_ClearFlag(SDIO_STATIC_FLAGS);
				return SDIO_OK;
			}
			if(SDIO_GetFlagStatus(SDIO_FLAG_CCRCFAIL)){
				SDIO_ClearFlag(SDIO_STATIC_FLAGS);
				return SDIO_CCRCFAIL;
			}
		}
	}
	return SDIO_TIMEOUT;
}

static void SdioReadResponse(uint32_t responseType,uint32_t * buf){
	if(responseType == SDIO_Response_Short){
		buf[0] = SDIO->RESP1;
	}else if(responseType == SDIO_Response_Long){
		buf[0] = SDIO->RESP4;
		buf[1] = SDIO->RESP3;
		buf[2] = SDIO->RESP2;
		buf[3] = SDIO->RESP1;
	}
}

static bool SdioSendAPPCmd(uint32_t appCmd,uint32_t arg,uint32_t responseType){
	SdioSendCmd(55,0,SDIO_Response_Short);//先发送CMD55
	ASSERT_RET(SdioWaitCmdResponse(SDIO_Response_Short) == SDIO_OK, return false);
	SdioSendCmd(41,arg, responseType);
	return true;
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
	_SDIO_Init();
	SdioSendCmd(0,0,SDIO_Response_No);
	ASSERT_RET(SdioWaitCmdResponse(SDIO_Response_No)==SDIO_OK, SDIO_SD_DEBUG_Print(SD_CARD_ERROR_CMD0);return false);
	SdioSendCmd(8,0xAA | (VOLTAGE_ACCEPTED_3_3V<<8),SDIO_Response_Short);// 3.3V check: 0xAA
	if(SdioWaitCmdResponse(SDIO_Response_Short) == SDIO_OK){
		//能识别 CMD08 肯定是2.0的卡
		SD_card_type = SD_CARD_TYPE_SD2;
		uint32_t voltage_pattern;
		SdioReadResponse(SDIO_Response_Short,&voltage_pattern);
		if(voltage_pattern!=(0xAA | (VOLTAGE_ACCEPTED_3_3V<<8)))return false;//Unusable Card.
		
		/* 等待上电完成 */
		uint16_t retry = 0xFFFF;
		uint32_t ocr;
		do{
			SDIO_SD_DEBUG_Print(SD_CARD_ERROR_ACMD41);
			SdioSendAPPCmd(41,SD_VOLTAGE_WINDOW_SD | SD_HIGH_CAPACITY, SDIO_Response_Short);
			SdioWaitCmdResponse(SDIO_Response_Short);
			SdioReadResponse(SDIO_Response_Short,&ocr);
		}while((ocr>>31)!=1 && retry--);
		ASSERT_RET(retry!=0xFFFF,return false;);
		if( ocr & 0x40000000){
			SD_card_type = SD_CARD_TYPE_SDHC;
		}
		card_info.OCR = ocr;
	}else{
		/* MMC卡 */
		uint16_t retry = 0xFFFF;
		uint32_t res;
		do{
			SdioSendCmd(1,0x80FF8000,SDIO_Response_Short);
			ASSERT_RET(SdioWaitCmdResponse(SDIO_Response_Short)== SDIO_OK,return false;);
			SdioReadResponse(SDIO_Response_Short,&res);
		}while((res>>31)!=1 && retry--);
		ASSERT_RET(retry!=0xFFFF,return false;);
		/* 暂时不支持MMC卡，反正也没人用 */
		return false;
	}
	SDIO_SD_DEBUG_Print(SD_CARD_ERROR_NONE);
	
	
	/* 不管是V1卡还是V2还是HSD都需要发送CMD2、CMD3 */
	SdioSendCmd(2,0x00,SDIO_Response_Long);
	SdioWaitCmdResponse(SDIO_Response_Long);
	uint32_t CID[4];
	SdioReadResponse(SDIO_Response_Long,CID);
	
	uint32_t rca;
	SdioSendCmd(3,0x00,SDIO_Response_Short);
	SdioWaitCmdResponse(SDIO_Response_Short);
	SdioReadResponse(SDIO_Response_Short,&rca);
	rca >>= 16;
	
	//获取CSD
	uint32_t CSD[4];
	SdioSendCmd(9,rca<<16,SDIO_Response_Long);
	SdioWaitCmdResponse(SDIO_Response_Long);
	SdioReadResponse(SDIO_Response_Long,CSD);
	
	//获取OCR
	
	
	//解析卡信息
	card_info.RCA = rca;
	memcpy(card_info.CID,CID,sizeof(CID));
	memcpy(card_info.CSD,CSD,sizeof(CSD));
	card_info.Type = SD_card_type;
	
	//CMD7选择中卡
	SdioSendCmd(7,rca<<16,SDIO_Response_Short);
	SdioWaitCmdResponse(SDIO_Response_Short);
	
	
	
	return true;
}

 bool SdioCard::erase(uint32_t firstSector, uint32_t lastSector){
	 SdioSendCmd(32,firstSector,SDIO_Response_Short);
	 SdioWaitCmdResponse(SDIO_Response_Short);
	 
	 SdioSendCmd(33,lastSector,SDIO_Response_Short);
	 SdioWaitCmdResponse(SDIO_Response_Short);
	 
	 SdioSendCmd(38,0,SDIO_Response_Short);
	 SdioWaitCmdResponse(SDIO_Response_Short);
	 
	 uint32_t status;
	 do{
		 SdioSendCmd(13,card_info.RCA<<16,SDIO_Response_Short);
		 SdioWaitCmdResponse(SDIO_Response_Short);
		 SdioReadResponse(SDIO_Response_Short,&status);
	 }while((status&(1<<8))==0);
	 return true;
 };
  /**
   * \return code for the last error. See SdCardInfo.h for a list of error codes.
   */
  uint8_t SdioCard::errorCode() const{
		return 0;
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
	
	}
  /** \return the SD clock frequency in kHz. */
  uint32_t SdioCard::kHzSdClk(){
	
	}
  /**
   * Read a 512 byte sector from an SD card.
   *
   * \param[in] sector Logical sector to be read.
   * \param[out] dst Pointer to the location that will receive the data.
   * \return true for success or false for failure.
   */
  bool SdioCard::readSector(uint32_t sector, uint8_t* dst){
		
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
		cid->always1 = 1;
		cid->crc= getByteBit(card_info.CID,1,7);
		uint16_t mdt = getByteBit(card_info.CID,8,19);
		cid->mdt_month = mdt & 0x0F;
		cid->mdt_year_low = (mdt>>4)&0x0F;
		cid->mdt_year_high = mdt>>8;
		cid->psn = getByteBit(card_info.CID,24,55);
		uint8_t prv =  getByteBit(card_info.CID,56,63);
		cid->prv_m = prv&0x0F;
		cid->prv_n = prv>>4;
		uint64_t pnm = getByteBit(card_info.CID,64,103);
		memcpy(cid->pnm,&pnm,sizeof(cid->pnm));
		uint16_t oid = getByteBit(card_info.CID,104,119);
		memcpy(cid->oid,&oid,sizeof(cid->oid));
		cid->mid = getByteBit(card_info.CID,120,127);
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
		if(card_info.Type == SD_CARD_TYPE_SDHC){
			csd->v2.crc = getByteBit(card_info.CSD,1,7);
			csd->v2.always1 = 1;
			csd->v2.file_format_grp = getByteBit(card_info.CSD,15,15);
			csd->v2.copy = getByteBit(card_info.CSD,14,14);
			csd->v2.perm_write_protect = getByteBit(card_info.CSD,13,13);
			csd->v2.tmp_write_protect = getByteBit(card_info.CSD,12,12);
			csd->v2.file_format = getByteBit(card_info.CSD,10,11);
			csd->v2.reserved7 = getByteBit(card_info.CSD,8,9);
			uint8_t write_bl_len = getByteBit(card_info.CSD,22,25);
			csd->v2.write_bl_len_low = write_bl_len&0x03;
			csd->v2.write_bl_len_high = write_bl_len>>2;
			csd->v2.write_partial =  getByteBit(card_info.CSD,21,21);
			csd->v2.reserved6 = getByteBit(card_info.CSD,16,20);
			csd->v2.wp_grp_enable = getByteBit(card_info.CSD,31,31);
			csd->v2.wp_grp_size = getByteBit(card_info.CSD,32,38);
			uint8_t sector_size = getByteBit(card_info.CSD,39,45);
			csd->v2.sector_size_low = sector_size&0x01;
			csd->v2.sector_size_high = sector_size>>1;
			csd->v2.r2w_factor = getByteBit(card_info.CSD,26,28);
			csd->v2.erase_blk_en = getByteBit(card_info.CSD,46,46);
			uint32_t c_size = getByteBit(card_info.CSD,48,69);
			csd->v2.c_size_low = c_size & 0xFF;
			csd->v2.c_size_mid = (c_size>>8) & 0xFF;
			csd->v2.c_size_high = c_size>>16;
			csd->v2.read_bl_partial = getByteBit(card_info.CSD,79,79);
			csd->v2.read_blk_misalign = getByteBit(card_info.CSD,77,77);
			csd->v2.write_blk_misalign = getByteBit(card_info.CSD,78,78);
			csd->v2.read_bl_len = getByteBit(card_info.CSD,80,83);
			csd->v2.dsr_imp = getByteBit(card_info.CSD,76,76);
			uint16_t cc = getByteBit(card_info.CSD,84,95);
			csd->v2.ccc_low = cc& 0x0F;
			csd->v2.ccc_high = cc>>4;
			csd->v2.tran_speed = getByteBit(card_info.CSD,96,103);
			csd->v2.nsac = getByteBit(card_info.CSD,104,111);
			csd->v2.taac = getByteBit(card_info.CSD,112,119);
			csd->v2.csd_ver = getByteBit(card_info.CSD,126,127);
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
		
	}
  /** Read OCR register.
   *
   * \param[out] ocr Value of OCR register.
   * \return true for success or false for failure.
   */
  bool SdioCard::readOCR(uint32_t* ocr){
	
		*ocr = card_info.OCR;
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
	
	
	}
  /** End a read multiple sectors sequence.
   *
   * \return true for success or false for failure.
   */
  bool SdioCard::readStop(){
	
	}
  /** \return SDIO card status. */
  uint32_t SdioCard::status(){
	
	}
    /**
   * Determine the size of an SD flash memory card.
   *
   * \return The number of 512 byte data sectors in the card
   *         or zero if an error occurs.
   */
  uint32_t SdioCard::sectorCount(){
	
	
	
	}
  /**
   *  Send CMD12 to stop read or write.
   *
   * \param[in] blocking If true, wait for command complete.
   *
   * \return true for success or false for failure.
   */
  bool SdioCard::stopTransmission(bool blocking){
	
	
	}
  /** \return success if sync successful. Not for user apps. */
  bool SdioCard::syncDevice(){
	
	
	}
  /** Return the card type: SD V1, SD V2 or SDHC
   * \return 0 - SD V1, 1 - SD V2, or 3 - SDHC.
   */
  uint8_t SdioCard::type() const{
		return card_info.Type;
	}
  /**
   * Writes a 512 byte sector to an SD card.
   *
   * \param[in] sector Logical sector to be written.
   * \param[in] src Pointer to the location of the data to be written.
   * \return true for success or false for failure.
   */
  bool SdioCard::writeSector(uint32_t sector, const uint8_t* src){
	
	
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
	
	
	}
  /** Write one data sector in a multiple sector write sequence.
   * \param[in] src Pointer to the location of the data to be written.
   * \return true for success or false for failure.
   */
  bool SdioCard::writeData(const uint8_t* src){
	
	
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
	
	
	}

  /** End a write multiple sectors sequence.
   *
   * \return true for success or false for failure.
   */
  bool SdioCard::writeStop(){
	
	
	}

#endif