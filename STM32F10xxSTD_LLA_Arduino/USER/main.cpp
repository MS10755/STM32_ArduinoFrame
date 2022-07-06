extern "C" {
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_istr.h"
#include "sdio_sdcard.h"
#include "memory.h"
#include "mass_mal.h"
}
#include <Arduino.h>
static uint8_t data_buf[BULK_MAX_PACKET_SIZE*2*4];
static uint8_t bulk_buf[BULK_MAX_PACKET_SIZE];
void setup(){

	SD_Init();
 	Mass_Memory_Size[0]=SDCardInfo.CardCapacity;		//得到SD卡容量（字节），当SD卡容量超过4G的时候,需要用到两个u32来表示
	Mass_Block_Size[0] =512;							//因为我们在Init里面设置了SD卡的操作字节为512个,所以这里一定是512个字节.
	Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];			
	USB_Port_Set(0); 	//USB先断开
	USB_Port_Set(1);	//USB再次连接 
  Data_Buffer=(uint32_t *)data_buf;	//为USB数据缓存区申请内存
	Bulk_Data_Buff=bulk_buf;	//申请内存
 	//USB配置
 	USB_Interrupts_Config();    
 	Set_USBClock();   
 	USB_Init();	     	    
}

void loop(){



}


