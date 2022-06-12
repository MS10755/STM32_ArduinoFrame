# LLA_assert模块
[toc]
### 1. API一览
|序号|宏原型|作用|
|:--:|--|--|
|1|LLA_ASSERT(e,driver,error)|LLA层断言|
|2|LLA_AssertPrintf(...)|断言输出，默认使用串口1进行断言输出。该函数用户不可调用|
### 2. 宏定义介绍
#### 2.1 LLA_ASSERT（LLA层断言）
宏定义`#define LLA_ASSERT(e,driver,error)`

输入参数：3
`e`表达式，若表达式为真则断言成功，若表达式为假则断言失败。失败后会打印错误信息，并进入`LLA_errorCode_Handler`
`driver`驱动代码。
`error`错误代码。

返回值：无

##### 例程
```cpp
LLA_ASSERT(IS_IO_NAME(name),DriverCode_BaseIO,BaseIO_errorIOName);//断言输入的name是否为正确的引脚名称。断言失败则会触发`BaseIO_errorIOName`错误
```

