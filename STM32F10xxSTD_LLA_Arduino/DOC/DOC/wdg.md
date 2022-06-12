# LLA_WDG模块
[toc]

### 1. API一览
|序号|函数原型|作用|
|:--:|--|--|
|1|`void LLA_WDG_Start(uint16_t ms)`|开启看门狗|
|2|`void LLA_WDG_Feed(void)`|喂狗|
|3|`void LLA_WDG_Stop(void)`|停止看门狗|
|4|`float LLA_WDG_GetOVRTime(void)`|获取最大喂狗时间|

### 2. 函数介绍

#### 2.1 LLA_WDG_Start （开启看门狗）

函数原型`void LLA_WDG_Start(uint16_t ms)`

输入参数：1
`uint16_t ms`喂狗时间。

返回值：无

##### 例程
```cpp
LLA_WDG_Start(20000);//设置看门狗喂狗时间为20s，若在20s内没有执行LLA_WDG_Feed,则会复位系统
```

#### 2.2 LLA_WDG_Feed （喂狗）

函数原型`void LLA_WDG_Feed(void)`

输入参数：无

返回值：无

##### 例程
```cpp
LLA_WDG_Start(20000);//设置看门狗喂狗时间为20s，若在20s内没有执行LLA_WDG_Feed,则会复位系统
do_something();//执行时间小于20s
LLA_WDG_Feed();
```

#### 2.3 LLA_WDG_Stop （停止看门狗）

函数原型`void LLA_WDG_Stop(void)`

输入参数：无

返回值：无


#### 2.4 LLA_WDG_GetOVRTime （获取最大喂狗时间）

函数原型`float LLA_WDG_GetOVRTime(void)`

输入参数：无

返回值：`float`最大喂狗时间。

##### 例程
```cpp
LLA_WDG_Start(20000);//设置看门狗喂狗时间为20s，若在20s内没有执行LLA_WDG_Feed,则会复位系统
float feed_time = LLA_WDG_GetOVRTime();//feed_time == 20s
```