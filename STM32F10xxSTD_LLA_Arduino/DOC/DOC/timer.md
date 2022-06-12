# LLA_timer模块
[toc]

### 1. API一览
|序号|函数原型|作用|
|:--:|--|--|
|1|`void LLA_Timer_Set(LLA_Timer_t timer,uint32_t period,Timer_Handler_t handler)`|配置定时器|
|2|`void LLA_Timer_Start(LLA_Timer_t timer)`|启动定时器|
|3|`void LLA_Timer_Stop(LLA_Timer_t timer)`|停止定时器|

### 2. 类型介绍
#### 2.1 LLA_Timer_t（定时器名称）
```cpp
typedef enum{
	LLA_TIMER0,
	LLA_TIMER1,
	LLA_TIMER2,
	LLA_TIMER3,
	LLA_TIMER4,
	LLA_TIMER5,
	LLA_TIMER6,
	LLA_TIMER7,
	
	_LLA_TIMER_MAX
}LLA_Timer_t;
```

#### 2.2 Timer_Handler_t（定时器回调类型）
```cpp
typedef void (*Timer_Handler_t)(void);
```

### 3. 函数介绍

#### 3.1 LLA_Timer_Set （配置定时器）
函数原型`void LLA_Timer_Set(LLA_Timer_t timer,uint32_t period,Timer_Handler_t handler)`

输入参数：3

返回值：无
`LLA_Timer_t timer`定时器名称，枚举类型。
`uint32_t period`定时周期，单位ms。
`Timer_Handler_t handler`回调函数，该函数在中断内执行，请保证快进快出。

##### 例程
```cpp
void timer1_handler(){
do_something();//Do not use blocked delay in do_somethin() function.
}

void setup(){
LLA_Timer_Set(LLA_TIMER0,100,timer1_handler);
}
```

#### 3.2 LLA_Timer_Start （启动定时器）
函数原型`void LLA_Timer_Start(LLA_Timer_t timer)`

输入参数：1

返回值：无
`LLA_Timer_t timer`定时器名称，枚举类型。

##### 例程
```cpp
void timer1_handler(){
do_something();//Do not use blocked delay in do_somethin() function.
}

void setup(){
LLA_Timer_Set(LLA_TIMER0,100,timer1_handler);//配置定时器1,此时定时器并未开始运行
LLA_Timer_Start(LLA_TIMER0);//启动定时器1
}
```

#### 3.3 LLA_Timer_Stop （停止定时器）
函数原型`void LLA_Timer_Stop(LLA_Timer_t timer)`

输入参数：1

返回值：无
`LLA_Timer_t timer`定时器名称，枚举类型。

##### 例程
```cpp
LLA_Timer_Stop(LLA_TIMER0);//停止定时器1
```