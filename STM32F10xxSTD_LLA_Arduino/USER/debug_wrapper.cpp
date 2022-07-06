#include <Arduino.h>>

/* 当前文件是提供C程序的debug接口包装，帮助modbus.c 输出调试信息到udp客户端 */
/* 参考文章:https://blog.csdn.net/z1002137615/article/details/50772185 */

extern "C" {
void Debug_printf(int const debug_level,char * fmt,...){
	char printf_buff[1024]={0};
	va_list args;
	va_start(args, fmt);
	int ret_status = vsnprintf(printf_buff, sizeof(printf_buff), fmt, args);
	va_end(args);
//	Debug.print(debug_level,"%S",printf_buff);
	Serial.println(printf_buff);
}

void Debug_fprintf(int const debug_level,void *f,char * fmt,...){
	(void)(f);
	char printf_buff[1024]={0};
	va_list args;
	va_start(args, fmt);
	int ret_status = vsnprintf(printf_buff, sizeof(printf_buff), fmt, args);
	va_end(args);
//	Debug.print(debug_level,"%S",printf_buff);
	Serial.println(printf_buff);
}

}

void Debug_printfCPP(int const debug_level,char * fmt,...){
	char printf_buff[1024]={0};
	va_list args;
	va_start(args, fmt);
	int ret_status = vsnprintf(printf_buff, sizeof(printf_buff), fmt, args);
	va_end(args);
//	Debug.print(debug_level,"%S",printf_buff);
	Serial.println(printf_buff);
}

void Debug_fprintfCPP(int const debug_level,void *f,char * fmt,...){
	(void)(f);
	char printf_buff[1024]={0};
	va_list args;
	va_start(args, fmt);
	int ret_status = vsnprintf(printf_buff, sizeof(printf_buff), fmt, args);
	va_end(args);
//	Debug.print(debug_level,"%S",printf_buff);
	Serial.println(printf_buff);
}


