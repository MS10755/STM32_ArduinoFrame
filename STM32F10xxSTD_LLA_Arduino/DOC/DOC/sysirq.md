# LLA_SYS_IRQ模块
[toc]

### 1. API一览
|序号|函数原型|作用|
|:--:|--|--|
|1|`void LLA_SYS_IRQ_Enable(void)`|允许系统发生中断|
|2|`void LLA_SYS_IRQ_Disable(void)`|不允许所有中断|


### 2. 函数介绍

#### 2.1 LLA_SYS_IRQ_Enable （允许系统发生中断）
函数原型`void LLA_SYS_IRQ_Enable(void)`

输入参数：无

返回值：无

##### 例程
```cpp
LLA_SYS_IRQ_Enable();//允许系统中断
```

#### 2.2 LLA_SYS_IRQ_Disable （不允许所有中断）
函数原型`void LLA_SYS_IRQ_Disable(void)`

输入参数：无

返回值：无

##### 例程
```cpp
LLA_SYS_IRQ_Disable();//禁止系统中断
```