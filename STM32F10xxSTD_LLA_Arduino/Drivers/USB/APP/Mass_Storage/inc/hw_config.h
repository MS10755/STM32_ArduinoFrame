#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H
#include "platform_config.h"
#include "usb_type.h" 

 
 
 
#define BULK_MAX_PACKET_SIZE  0x00000040	//包大小,最大64字节.



//USB通用代码函数声明
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config (FunctionalState NewState);
void USB_Port_Set(u8 enable);
void IntToUnicode (u32 value,u8 *pbuf,u8 len);
void Get_SerialNum(void);


//不同USB功能添加的函数声明 
void Led_RW_ON(void);
void Led_RW_OFF(void);
void USB_NotConfigured_LED(void);

#endif  
























