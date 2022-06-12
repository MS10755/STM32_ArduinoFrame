# LLA_ADC模块
[toc]

### 1. API一览
|序号|函数原型|作用|
|:--:|--|--|
|1|`void LLA_ADC_Init(void)`|初始化ADC模块|
|2|`uint32_t LLA_ADC_Read(BaseIO_name_t name)`|读引脚ADC值|
|3|`uint32_t LLA_ADC_ReadSpecial(LLA_ADC_Special_channel_t channel)`|读特殊通道ADC值|

### 2. 类型介绍
#### 2.1 LLA_ADC_Special_channel_t（ADC特殊通道）
```cpp
typedef enum{
	LLA_ADC_SPECIAL_CHANNEL_0,
	LLA_ADC_SPECIAL_CHANNEL_1,
	LLA_ADC_SPECIAL_CHANNEL_2,
	LLA_ADC_SPECIAL_CHANNEL_3,
	LLA_ADC_SPECIAL_CHANNEL_4,
	
	_LLA_ADC_SPECIAL_CHANNEL_MAX
}LLA_ADC_Special_channel_t;
```
### 3. 函数介绍
#### 3.1 LLA_ADC_Init（初始化ADC模块）
函数原型`void LLA_ADC_Init(void)`

输入参数：无

返回值：无
##### 例程
```cpp
该函数由框架自动调用，无需用户调用。
```
#### 3.2 LLA_ADC_Read（读引脚ADC值）
函数原型`uint32_t LLA_ADC_Read(BaseIO_name_t name)`

输入参数：1
`BaseIO_name_t name`引脚名称，枚举类型（见 [LLA_baseIO](baseio.md) 中定义）。

返回值：ADC采样值

##### 例程
```cpp
uint32_t val = LLA_ADC_Read(PA2);//读PA2引脚ADC值
```

#### 3.3 LLA_ADC_ReadSpecial（读特殊通道ADC值）
函数原型`uint32_t LLA_ADC_ReadSpecial(LLA_ADC_Special_channel_t channel)`

输入参数：1
`LLA_ADC_Special_channel_t channel`特殊通道名称，枚举类型。

返回值：ADC采样值

##### 例程
```cpp
uint32_t val = LLA_ADC_Read(LLA_ADC_SPECIAL_CHANNEL_0);//读特殊通道0的ADC值
```