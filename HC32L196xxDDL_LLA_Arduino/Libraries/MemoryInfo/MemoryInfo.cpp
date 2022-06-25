#include "Arduino.h"
#include "StackInfo.h"
#include <stdarg.h>
#include <stdlib.h>
#include <Arduino_DebugUtils.h>


char StackInfo[200]={0,};
static void Memory_ShowStackInfo()
{
	char buffer[200]={0,};
	sprintf(buffer,"Stack total:%d  max use:%d  Utilization rate:%.2f\r\n",StackInfo_GetTotalSize(),StackInfo_GetMaxUsageSize(),StackInfo_GetMaxUtilization());
	Debug.print(DBG_INFO,buffer);
	strcpy(StackInfo,buffer);
}

char HeapInfo[100]={0,};
static int Memory_HeapPrint(void* param, char const* format, ...)
{
    char printf_buff[64]={0};

    va_list args;
    va_start(args, format);

    int ret_status = vsnprintf(printf_buff, sizeof(printf_buff), format, args);

    va_end(args);

    Debug.print(DBG_INFO,(String("Heap: ")+ String(printf_buff)).c_str());
		if(strstr(printf_buff,"bytes")){
			strcpy(HeapInfo,printf_buff);
		}
    return ret_status;
}

static void Memory_ShowHeapInfo()
{
    int size = 0;
    __heapstats((__heapprt)Memory_HeapPrint, &size);
}


const char* const Memory_PrintStackInfo(){
	return StackInfo;
}

const char* const Memory_PrintHeapInfo(){
	return HeapInfo;
}


void Memory_DumpInfo()
{

    Memory_ShowStackInfo();

    Memory_ShowHeapInfo();

}

