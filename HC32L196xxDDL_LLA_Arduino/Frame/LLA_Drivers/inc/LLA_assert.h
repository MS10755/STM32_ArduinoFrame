#ifndef __LLA_ASSERT_H__
#define __LLA_ASSERT_H__


/* std lib includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#if 1
#include "LLA_Drivers.h"
#define LLA_AssertPrintf(...)	LLA_UART_Printf(LLA_UART1,__VA_ARGS__)
#else
#define LLA_AssertPrintf(...)
#endif


#include "LLA_errorCode.h"

#define LLA_ASSERT(e,driver,error)																																												\
do{																																																												\
	if(!(e)){																																																									\
		LLA_AssertPrintf("LLA_Assert failed!\r\nFile:%s\r\nFunction:%s\r\nLine:%d\r\nReport:%s\r\n",__FILE__,__FUNCTION__,__LINE__,LAA_errorInfo(driver,error));	\
		LLA_errorCode_Handler(driver,error);																																									\
	}																																																												\
}while(0)

#endif
