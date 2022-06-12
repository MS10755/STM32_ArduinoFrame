# LLA_SYS_clock模块
[toc]

### 1. API一览
|序号|函数原型|作用|
|:--:|--|--|
|1|`void LLA_SYS_clock_Init(void)`|初始化系统时钟|
|2|`void LLA_SYS_clock_Update(void)`|更新系统时钟|
|3|`uint32_t LLA_SYS_clock_Get(void)`|获取系统时钟|

### 2. 函数介绍

#### 2.1 LLA_SYS_clock_Init （初始化系统时钟）
函数原型`void LLA_SYS_clock_Init(void)`

输入参数：无

返回值：无

##### 例程
```cpp
该函数由框架自动调用，无需用户调用。
```

#### 2.2 LLA_SYS_clock_Update （更新系统时钟）
函数原型`void LLA_SYS_clock_Update(void)`

输入参数：无

返回值：无

##### 例程
```cpp
该函数由框架自动调用，无需用户调用。
```

#### 2.3 LLA_SYS_clock_Get （获取系统时钟）
函数原型`uint32_t LLA_SYS_clock_Get(void)`

输入参数：无

返回值：`uint32_t`系统时钟，单位hz

##### 例程
```cpp
uint32_t clk = LLA_SYS_clock_Get();//读取系统时钟速度到clk变量（单位hz）
```
