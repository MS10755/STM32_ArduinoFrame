#include "LLA_Drivers.h"
#include "../inc/LLA_errorCodeInfos.h"

#define LLA_ERRORCODE_NUM	(sizeof(LLA_ErrorCode_List)/sizeof(LLA_ErrorCode_List[0]))

const char * LAA_errorInfo(DriverCode_t driverCode,uint8_t errorCode){
	for(uint8_t i = 0;i< LLA_ERRORCODE_NUM; i++){
		if(
			LLA_ErrorCode_List[i].driverCode == driverCode 
			&& 
			LLA_ErrorCode_List[i].errorCode == errorCode
		){
			return LLA_ErrorCode_List[i].info;
		}
	}
	return NULL;
}


LLA_WEAK void LLA_errorCode_Handler(DriverCode_t driverCode,uint8_t errorCode){
	LLA_AssertPrintf("\r\nEnter LLA_errorCode_Handler, Program Pause!");
	LLA_SYS_IRQ_Disable();
	while(1){
	};
}

