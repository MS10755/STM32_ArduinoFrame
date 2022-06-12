# LLA_SYS_time模块
[toc]

### 1. API一览
|序号|函数原型|作用|
|:--:|--|--|
|1|`void LLA_SYS_Time_Init(void)`|初始化系统时间模块|
|2|`uint32_t LLA_SYS_Time_Millis(void)`|获取系统运行毫秒数|
|3|`uint32_t LLA_SYS_Time_Micros(void)`|获取系统运行微秒数|
|4|`void LLA_SYS_Time_DelayMS(uint32_t ms)`|系统毫秒级延时|
|5|`void LLA_SYS_Time_DelayUS(uint32_t us)`|系统微秒级延时|
|6|`uint32_t LLA_SYS_Time_ConsumeMillis(uint32_t pre_ms)`|计算经过的毫秒数|
|7|`uint32_t LLA_SYS_Time_ConsumeMicros(uint32_t pre_us)`|计算经过的微秒数|

### 2. 函数介绍

#### 2.1 LLA_SYS_Time_Init （初始化系统时间模块）
函数原型`void LLA_SYS_Time_Init(void)`

输入参数：无

返回值：无
##### 例程
```cpp
该函数由框架自动调用，无需用户调用。
```

#### 2.2 LLA_SYS_Time_Millis （获取系统运行毫秒数）
函数原型`uint32_t LLA_SYS_Time_Millis(void)`

输入参数：无

返回值：`uint32_t`系统运行毫秒数
##### 例程
```cpp
uint32_t current = LLA_SYS_Time_Millis();//获取系统自上电以来经过的毫秒数
```

#### 2.3 LLA_SYS_Time_Micros （获取系统运行微秒数）
函数原型`LLA_SYS_Time_Micros(void)`

输入参数：无

返回值：`uint32_t`系统运行微秒数
##### 例程
```cpp
uint32_t current = LLA_SYS_Time_Micros();//获取系统自上电以来经过的微秒数
```

#### 2.4 LLA_SYS_Time_DelayMS （系统毫秒级延时）
函数原型`void LLA_SYS_Time_DelayMS(uint32_t ms)`

输入参数：1
`uint32_t ms`延迟的毫秒数

返回值：无

##### 例程
```cpp
LLA_SYS_Time_DelayMS(1000);//延迟1000毫秒
```

#### 2.5 LLA_SYS_Time_DelayUS （系统微秒级延时）
函数原型`void LLA_SYS_Time_DelayUS(uint32_t us)`

输入参数：1
`uint32_t us`延迟的微秒数

返回值：无

##### 例程
```cpp
LLA_SYS_Time_DelayUS(200);//延迟200微秒
```

#### 2.6 LLA_SYS_Time_ConsumeMillis （计算经过的毫秒数）
函数原型`uint32_t LLA_SYS_Time_ConsumeMillis(uint32_t pre_ms)`

输入参数：1
`uint32_t pre_ms`上一次记录的系统毫秒数

返回值：`uint32_t`经过的毫秒数

##### 例程
```cpp
uint32_t ms = LLA_SYS_Time_Millis();
do_something();
uint32_t ms_consume = LLA_SYS_Time_ConsumeMillis(ms);//计算do_something用时
```

#### 2.7 LLA_SYS_Time_ConsumeMicros （计算经过的微秒数）
函数原型`uint32_t LLA_SYS_Time_ConsumeMicros(uint32_t pre_us)`

输入参数：1
`uint32_t pre_us`上一次记录的系统微秒数

返回值：`uint32_t`经过的微秒数

##### 例程
```cpp
uint32_t us = LLA_SYS_Time_Micros();
do_something();
uint32_t us_consume = LLA_SYS_Time_ConsumeMicros(us);//计算do_something用时
```