# 底层驱动抽象层
>底层驱动抽象层 (**Low-lever drivers abstraction layer**) 以后简称**LLA**，向上层程序提供了一个API集合，向下规定了一种移植的规范（函数原型）。而上层程序都是通过LLA层API实现的，所以移植者只需要完成所有函数原型的实现即可完成整体项目在不同芯片平台的迁移。

### LLA层模块介绍
|序号|模块|作用|是否需要移植|
|:--:|--|--|:-:|
|1|[LLA_ADC](adc.md)|ADC功能的初始化，以及ADC采集|YES|
|2|[LLA_advanceIO](advanceio.md)|串行移位输入输出，脉宽测量|YES|
|3|[LLA_assert](assert.md)|LLA层内部断言,对函数输入参数进行断言|YES|
|4|[LLA_baseIO](baseio.md)|基础引脚操作，模式配置，电平读写|YES|
|5|LLA_compiler|编译器相关|NO|
|6|LLA_errorCode|断言错误提示组件|NO|
|7|LLA_errorCodeInfos|错误提示中英文信息|NO|
|8|[LLA_EXTI](exti.md)|外部中断|YES|
|9|[LLA_flash](flash.md)|片内Flash读写|YES|
|10|[LLA_PWM](pwm.md)|PWM输出|YES|
|11|[LLA_SPI](spi.md)|SPI通讯|YES|
|12|[LLA_SYS_clock](sysclock.md)|系统时钟初始化，获取|YES|
|13|[LLA_SYS_IRQ](sysirq.md)|系统中断开关|YES|
|14|[LLA_SYS_rest](sysrest.md)|系统软件复位|YES|
|15|[LLA_SYS_time](systime.md)|系统计时，精确的阻塞延迟|YES|
|16|[LLA_timer](timer.md)|硬件定时器|YES|
|17|[LLA_UART](uart.md)|串口通讯|YES|
|18|[LLA_WDG](wdg.md)|独立看门狗|YES|