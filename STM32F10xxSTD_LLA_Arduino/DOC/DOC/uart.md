# LLA_UART模块
[toc]

### 1. API一览
|序号|函数原型|作用|
|:--:|--|--|
|1|`void LLA_UART_Init(UART_name_t name,uint32_t baudRate,UART_config_t config)`|初始化串口|
|2|`void LLA_UART_DeInit(UART_name_t name)`|失能串口|
|3|`void LLA_UART_Write(UART_name_t name,uint8_t data)`|串口写数据|
|4|`void LLA_UART_WriteBuffer(UART_name_t name,uint8_t *data,uint16_t len)`|串口写多字节数据|
|5|`int LLA_UART_Printf(UART_name_t name,const char *__restrict __format, ...)`|串口格式化输出|
|6|`LLA_WEAK void LLA_UART1_IRQHandler(uint8_t data)`|串口1数据接收钩子函数|
|7|`LLA_WEAK void LLA_UART2_IRQHandler(uint8_t data)`|串口2数据接收钩子函数|
|8|`LLA_WEAK void LLA_UART3_IRQHandler(uint8_t data)`|串口3数据接收钩子函数|
|9|`LLA_WEAK void LLA_UART4_IRQHandler(uint8_t data)`|串口4数据接收钩子函数|
|10|`LLA_WEAK void LLA_UART5_IRQHandler(uint8_t data)`|串口5数据接收钩子函数|
|11|`LLA_WEAK void LLA_UART6_IRQHandler(uint8_t data)`|串口6数据接收钩子函数|
|12|`LLA_WEAK void LLA_UART7_IRQHandler(uint8_t data)`|串口7数据接收钩子函数|
|13|`LLA_WEAK void LLA_UART8_IRQHandler(uint8_t data)`|串口8数据接收钩子函数|

#### 2. 宏定义介绍
##### 2.1 LLA_UART_PRINTF_BUFFER_LENGTH（串口格式化输出缓冲区大小）
```cpp
#define LLA_UART_PRINTF_BUFFER_LENGTH 512
```
该宏用于配置串口格式化输出函数的静态缓冲区大小，表示一次格式化输出最多输出多少字节字符。所有串口共用一个静态缓冲区，该函数线程不安全，请勿在多线程中使用。

#### 3. 类型介绍
##### 3.1 UART_name_t（串口名称）
```cpp
typedef enum{
	LLA_UART1,
	LLA_UART2,
	LLA_UART3,
	LLA_UART4,
	LLA_UART5,
	LLA_UART6,
	LLA_UART7,
	LLA_UART8,
	
	_LLA_UART_MAX
}UART_name_t;
```
##### 3.2 UART_config_t（串口配置）
```cpp
typedef enum{
	UART_CONFIG_8N1,//8数据位，无校验，1停止位
	UART_CONFIG_8N1_5,//8数据位，无校验，1.5停止位
	UART_CONFIG_8N2,//8数据位，无校验，2停止位
	UART_CONFIG_8E1,//8数据位，偶校验，1停止位
	UART_CONFIG_8E1_5,//8数据位，偶校验，1.5停止位
	UART_CONFIG_8E2,//8数据位，偶校验，2停止位
	UART_CONFIG_8O1,//8数据位，奇校验，1停止位
	UART_CONFIG_8O1_5,//8数据位，奇校验，1.5停止位
	UART_CONFIG_8O2,//8数据位，奇校验，2停止位
	
	_UART_CONCIG_MAX
}UART_config_t;
```

#### 4. 函数介绍
##### 4.1 LLA_UART_Init（初始化串口）
函数原型`void LLA_UART_Init(UART_name_t name,uint32_t baudRate,UART_config_t config)`

输入参数：3
`UART_name_t name`串口名称，枚举类型。
`uint32_t baudRate`波特率。
`UART_config_t config`串口配置，枚举类型。

返回值：无

##### 例程
```cpp
LLA_UART_Init(LLA_UART1,9600,UART_CONFIG_8N1);//初始化串口1为9600波特率，8数据位无校验1停止位。
```

##### 4.2 LLA_UART_DeInit（失能串口）
函数原型`void LLA_UART_DeInit(UART_name_t name)`

输入参数：1
`UART_name_t name`串口名称，枚举类型。

返回值：无

##### 例程
```cpp
LLA_UART_Init(LLA_UART1,9600,UART_CONFIG_8N1);//初始化串口1为9600波特率，8数据位无校验1停止位。
do_something();
LLA_UART_DeInit(LLA_UART1);//失能串口，关闭串口外设。
```


##### 4.3 LLA_UART_Write（串口写数据）
函数原型`void LLA_UART_Write(UART_name_t name,uint8_t data)`

输入参数：2
`UART_name_t name`串口名称，枚举类型。
`uint8_t data`要发送的数据。

返回值：无

##### 例程
```cpp
/* 串口1发送 "HELLO" */
LLA_UART_Init(LLA_UART1,9600,UART_CONFIG_8N1);//初始化串口1为9600波特率，8数据位无校验1停止位。
LLA_UART_Write(LLA_UART1,'H');
LLA_UART_Write(LLA_UART1,'E');
LLA_UART_Write(LLA_UART1,'L');
LLA_UART_Write(LLA_UART1,'L');
LLA_UART_Write(LLA_UART1,'O');
```

##### 4.4 LLA_UART_Write（串口写多字节数据）
函数原型`void LLA_UART_WriteBuffer(UART_name_t name,uint8_t *data,uint16_t len)`

输入参数：3
`UART_name_t name`串口名称，枚举类型。
`uint8_t *data`要发送数据缓冲区的指针。
`uint16_t len`数据字节数。

返回值：无

##### 例程
```cpp
/* 串口1发送 "HELLO" */
LLA_UART_Init(LLA_UART1,9600,UART_CONFIG_8N1);//初始化串口1为9600波特率，8数据位无校验1停止位。
char buf[]="HELLO"
LLA_UART_WriteBuffer(LLA_UART1,(uint8_t*)buf,strlen(buf));
```

##### 4.5 LLA_UART_Printf（串口格式化输出）
函数原型`int LLA_UART_Printf(UART_name_t name,const char *__restrict __format, ...)`

输入参数：2+n
`UART_name_t name`串口名称，枚举类型。
`const char *__restrict __format`格式化字符。
`...`可变参数。


返回值：`int`发送字符串的长度

##### 例程
```cpp
/* 串口1发送 "HELLO" */
LLA_UART_Init(LLA_UART1,9600,UART_CONFIG_8N1);//初始化串口1为9600波特率，8数据位无校验1停止位。
char buf[]="HELLO"
LLA_UART_Printf(LLA_UART1,"%s",buf);
```

#### 5. 钩子函数介绍
##### 5.1 LLA_UART1_IRQHandler（串口1数据接收钩子函数）
当发生串口接收到数据后，由系统自动调用该函数。其中`uint8_t data`就是串口所接收到的数据。由用户进行接收逻辑编写。若使用ArduinoAPI，则由ArduinoAPI层编写的钩子函数处理。该函数只可以重写，不可用调用。
函数原型`LLA_WEAK void LLA_UART1_IRQHandler(uint8_t data)`

系统输入参数：
`uint8_t data`串口1所接收到的数据。

返回值：无

##### 5.2 LLA_UART2_IRQHandler（串口2数据接收钩子函数）
*见 5.1*

##### 5.3 LLA_UART2_IRQHandler（串口3数据接收钩子函数）
*见 5.1*

##### 5.4 LLA_UART2_IRQHandler（串口4数据接收钩子函数）
*见 5.1*

##### 5.5 LLA_UART2_IRQHandler（串口5数据接收钩子函数）
*见 5.1*

##### 5.6 LLA_UART2_IRQHandler（串口6数据接收钩子函数）
*见 5.1*

##### 5.7 LLA_UART2_IRQHandler（串口7数据接收钩子函数）
*见 5.1*

##### 5.8 LLA_UART2_IRQHandler（串口8数据接收钩子函数）
*见 5.1*
