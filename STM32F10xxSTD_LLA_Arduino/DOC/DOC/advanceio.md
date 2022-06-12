# LLA_advanceIO模块
[toc]

### 1. API一览
|序号|函数原型|作用|
|:--:|--|--|
|1|`uint8_t LLA_AdvanceIO_ShiftIn(BaseIO_name_t dataPin, BaseIO_name_t clockPin, LLA_BitOrder_t bitOrder)`|串行移位输入|
|2|`void LLA_AdvanceIO_ShiftOut(BaseIO_name_t dataPin, BaseIO_name_t clockPin, LLA_BitOrder_t bitOrder, uint8_t val)`|串行移位输出|
|3|`uint32_t LLA_AdvanceIO_PluseMeasure(BaseIO_name_t name,AdvanceIO_PWC_t pwc,BaseIO_status_t idle_status,uint32_t time_out)`|脉宽测量|
### 2. 类型介绍
#### 2.1 LLA_BitOrder_t （比特顺序）
```cpp
typedef enum{
	LLA_LSBFIRST,//低位先出
	LLA_MSBFIRST,//高位先出
}LLA_BitOrder_t;
```
#### 2.2 AdvanceIO_PWC_t（脉宽测量模式）

```cpp
typedef enum{
	AdvanceIO_PWC_F2F,//下降沿到下降沿模式
	AdvanceIO_PWC_R2R,//上升沿到上升沿模式
	AdvanceIO_PWC_F2R,//下降沿到上升沿模式
	AdvanceIO_PWC_R2F,//上升沿到下降沿模式
	
	_ADVANCEIO_PWC_MAX
}AdvanceIO_PWC_t;
```
### 3. 函数介绍
#### 3.1 LLA_AdvanceIO_ShiftIn（串行移位输入）
函数原型`uint8_t LLA_AdvanceIO_ShiftIn(BaseIO_name_t dataPin, BaseIO_name_t clockPin, LLA_BitOrder_t bitOrder)`

输入参数：3
`BaseIO_name_t dataPin`数据引脚名称，枚举类型（见 [LLA_baseIO](baseio.md) 中定义）。
`BaseIO_name_t clockPin`时钟引脚名称，枚举类型（见 [LLA_baseIO](baseio.md) 中定义）。
`LLA_BitOrder_t bitOrder`比特顺序，枚举类型。

返回值：`uint8_t` 输入的1字节数据

##### 例程
```cpp
LLA_BaseIO_Mode(PA3,BaseIOMode_INPUT);
LLA_BaseIO_Mode(PA2,BaseIOMode_OUTPUT);
uint8_t data = LLA_AdvanceIO_ShiftIn(PA3,PA2,LLA_MSBFIRST);//PA3引脚作为数据输入引脚，PA2引脚作为时钟输出引脚。高位先出。
```

#### 3.2 LLA_AdvanceIO_ShiftOut（串行移位输出）
函数原型`void LLA_AdvanceIO_ShiftOut(BaseIO_name_t dataPin, BaseIO_name_t clockPin, LLA_BitOrder_t bitOrder, uint8_t val)`

输入参数：4
`BaseIO_name_t dataPin`数据引脚名称，枚举类型（见 [LLA_baseIO](baseio.md) 中定义）。
`BaseIO_name_t clockPin`时钟引脚名称，枚举类型（见 [LLA_baseIO](baseio.md) 中定义）。
`LLA_BitOrder_t bitOrder`比特顺序，枚举类型。
`uint8_t val`要移位输出的字节数据。

返回值：无

##### 例程
```cpp
LLA_BaseIO_Mode(PA3,BaseIOMode_OUTPUT);
LLA_BaseIO_Mode(PA2,BaseIOMode_OUTPUT);
LLA_AdvanceIO_ShiftOut(PA3,PA2,LLA_MSBFIRST,0x68);//PA3引脚作为数据输出引脚，PA2引脚作为时钟输出引脚。高位先出。输出0x68
```

#### 3.3 LLA_AdvanceIO_PluseMeasure（PWC脉宽测量）
函数原型：`uint32_t LLA_AdvanceIO_PluseMeasure(BaseIO_name_t name,AdvanceIO_PWC_t pwc,BaseIO_status_t idle_status,uint32_t time_out)`

输入参数：4
`BaseIO_name_t name` 测量引脚名称，枚举类型（见 [LLA_baseIO](baseio.md) 中定义）。
`AdvanceIO_PWC_t pwc`脉宽测量模式，枚举类型。
`BaseIO_status_t idle_status` 引脚空闲状态，枚举类型（见 [LLA_baseIO](baseio.md) 中定义）。
`uint32_t time_out` 最大超时时间,（单位 ms，自函数调用时开始计时。发生超时，返回值为0，并触发断言。

返回值：`uint32_t `脉宽持续时间，（单位 us）。测量失败则返回值恒等于0。

##### 例程
```cpp
/* 测量高电平持续为230us的脉宽 */
uint32_t val = LLA_AdvanceIO_PluseMeasure(PB3,AdvanceIO_PWC_R2F,BaseIOStatus_LOW,10);//测量PB3引脚的高脉宽持续时间
```