# LLA_baseIO模块
[toc]

### 1. API一览
|序号|函数原型|作用|
|:--:|--|--|
|1|`void LLA_BaseIO_Mode(BaseIO_name_t name,BaseIO_mode_t mode,BaseIO_status_t outputStatus)`|配置引脚的工作模式|
|2|`void LLA_BaseIO_Write(BaseIO_name_t name,BaseIO_status_t outputStatus)`|设置引脚电平|
|3|`BaseIO_status_t LLA_BaseIO_ReadInput(BaseIO_name_t name)`|读取引脚输入电平|
|4|`BaseIO_status_t LLA_BaseIO_ReadOutput(BaseIO_name_t name)`|读取引脚输出电平|
|5|`BaseIO_status_t LLA_BaseIO_Toggle(BaseIO_name_t name)`|翻转引脚输出电平|
### 2. 类型介绍
#### 2.1 BaseIO_status_t（IO电平状态）
```cpp
typedef enum{
	BaseIOStatus_LOW,
	BaseIOStatus_HIGH,
	
	_BASEIO_STATUS_MAX
}BaseIO_status_t;
```
#### 2.2 BaseIO_mode_t（IO口模式）
```cpp
typedef enum{
	BaseIOMode_INPUT,//浮空输入
	BaseIOMode_INPUT_PU,//上拉输入
	BaseIOMode_INPUT_PD,//上拉输出
	BaseIOMode_OUTPUT,//推挽输出
	BaseIOMode_OUTPUT_OD,//开漏输出
	BaseIOMode_OUTPUT_OD_PU,//开漏上拉输出
	BaseIOMode_OUTPUT_OD_PD,//开漏下拉输出
	
	_BASEIO_MODE_MAX
}BaseIO_mode_t;
```
#### 2.3 BaseIO_name_t（IO名称）
```cpp
typedef enum{
	PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PA8,PA9,PA10,PA11,PA12,PA13,PA14,PA15,
	PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15,
	PC0,PC1,PC2,PC3,PC4,PC5,PC6,PC7,PC8,PC9,PC10,PC11,PC12,PC13,PC14,PC15,
	PD0,PD1,PD2,PD3,PD4,PD5,PD6,PD7,PD8,PD9,PD10,PD11,PD12,PD13,PD14,PD15,
	PE0,PE1,PE2,PE3,PE4,PE5,PE6,PE7,PE8,PE9,PE10,PE11,PE12,PE13,PE14,PE15,
	PF0,PF1,PF2,PF3,PF4,PF5,PF6,PF7,PF8,PF9,PF10,PF11,PF12,PF13,PF14,PF15,
	PG0,PG1,PG2,PG3,PG4,PG5,PG6,PG7,PG8,PG9,PG10,PG11,PG12,PG13,PG14,PG15,
	
	_BASEIO_NAME_MAX,
}BaseIO_name_t;
```
### 3. 函数介绍

#### 3.1 LLA_BaseIO_Mode （引脚模式配置）

函数原型`void LLA_BaseIO_Mode(BaseIO_name_t name,BaseIO_mode_t mode,BaseIO_status_t outputStatus)`

输入参数：3
`BaseIO_name_t name`引脚名称，枚举类型。
`BaseIO_mode_t mode`引脚模式，枚举类型。
`BaseIO_status_t outputStatus`默认输出电平的状态，枚举类型。只有在输出模式时使用，输入模式该参数被忽略。

返回值：无
##### 例程
```cpp
LLA_BaseIO_Mode(PA4,BaseIOMode_OUTPUT,BaseIOStatus_LOW);//配置引脚PA4为推挽输出模式，并设置IO电平为低电平
LLA_BaseIO_Mode(PA5,BaseIOMode_INPUT_PU,BaseIOStatus_LOW);//配置引脚PA5为上拉输入模式。（输入模式设置IO口电平被忽略）
```
#### 3.2 LLA_BaseIO_Write（写引脚电平）
函数原型`void LLA_BaseIO_Write(BaseIO_name_t name,BaseIO_status_t outputStatus)`

输入参数：2
`BaseIO_name_t name`引脚名称，枚举类型。
`BaseIO_status_t outputStatus`电平状态，枚举类型。

返回值：无

##### 例程
```cpp
LLA_BaseIO_Mode(PA4,BaseIOMode_OUTPUT,BaseIOStatus_LOW);//配置PA4为输出模式
LLA_BaseIO_Write(PA4,BaseIOStatus_HIGH);//设置PA4为高电平
```
#### 3.3 LLA_BaseIO_ReadInput（读引脚输入电平）
函数原型`BaseIO_status_t LLA_BaseIO_ReadInput(BaseIO_name_t name)`

输入参数：1
`BaseIO_name_t name`引脚名称，枚举类型。

返回值：`BaseIO_status_t `输入电平状态，枚举类型。

##### 例程
```cpp
LLA_BaseIO_Mode(PA4,BaseIOMode_INPUT_PU);//配置PA4为上拉输入模式
BaseIO_status_t input = LLA_BaseIO_ReadInput(PA4);//读取PA4输入电平
```
#### 3.4 LLA_BaseIO_ReadOutput（读引脚输出电平）
函数原型`BaseIO_status_t LLA_BaseIO_ReadOutput(BaseIO_name_t name)`

输入参数：1
`BaseIO_name_t name`引脚名称，枚举类型。

返回值：`BaseIO_status_t `输出电平状态，枚举类型。

##### 例程
```cpp
LLA_BaseIO_Mode(PA4,BaseIOMode_OUTPUT,BaseIOStatus_LOW);//配置PA4为输出模式
LLA_BaseIO_Write(PA4,BaseIOStatus_HIGH);//设置PA4为高电平
BaseIO_status_t output = LLA_BaseIO_ReadOutput(PA4);//读取PA4输入电平,此时output为高电平
```
#### 3.5 LLA_BaseIO_Toggle（翻转引脚输出电平）
函数原型`BaseIO_status_t LLA_BaseIO_Toggle(BaseIO_name_t name)`

输入参数：1

返回值：`BaseIO_status_t name`翻转后的电平状态，枚举类型。

##### 例程
```cpp
LLA_BaseIO_Mode(PA4,BaseIOMode_OUTPUT,BaseIOStatus_LOW);//配置PA4为输出模式
BaseIO_status_t output=LLA_BaseIO_Toggle(PA4);//执行该语句后output为高
output=LLA_BaseIO_Toggle(PA4);//执行该语句后outpu为低
```