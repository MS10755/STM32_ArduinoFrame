# LLA_EXTI模块
[toc]

### 1. API一览
|序号|函数原型|作用|
|:--:|--|--|
|1|`void LLA_EXTI_AttachInterrupt(BaseIO_name_t name, LLA_EXTI_cb_t cb,LLA_EXTI_TYPE_t type)`|初始化引脚外部中断，并设置回调|
|2|`void LLA_EXTI_DetachInterrupt(BaseIO_name_t name)`|关闭引脚外部中断|

### 2. 类型介绍
#### 2.1 LLA_EXTI_TYPE_t（外部中断类型）
```cpp
typedef enum{
	LLA_EXTI_TYPE_LOW,//低电平触发
	LLA_EXTI_TYPE_HIGH,//高电平触发
	LLA_EXTI_TYPE_FALLING,//下降沿触发
	LLA_EXTI_TYPE_RISING,//上升沿触发
	LLA_ExTI_TYPE_CHANGE,//下降和上升沿都触发
	
	_LLA_EXTI_TYPE_MAX
}LLA_EXTI_TYPE_t;
```
#### 2.2 LLA_EXTI_cb_t（外部中断回调）
`typedef void (*LLA_EXTI_cb_t)(void);`

### 3. 函数介绍
#### 3.1 LLA_EXTI_AttachInterrupt（设置外部中断）
函数原型：`void LLA_EXTI_AttachInterrupt(BaseIO_name_t name, LLA_EXTI_cb_t cb,LLA_EXTI_TYPE_t type)`

输入参数：3
`BaseIO_name_t name`引脚名称，枚举类型（见 [LLA_baseIO](baseio.md) 中定义）。
`LLA_EXTI_cb_t cb`回调函数，函数指针。
`LLA_EXTI_TYPE_t type`外部中断类型，枚举类型。

返回值：无

##### 例程
```cpp
/* 定义外部中断回调 */
void exti_cb(void){

}
LLA_EXTI_AttachInterrupt(PA4,exti_cb,LLA_EXTI_TYPE_FALLING);//为PA4设置外部中断及回调函数
```

#### 3.2 LLA_EXTI_DetachInterrupt（关闭外部中断）
函数原型：`void LLA_EXTI_DetachInterrupt(BaseIO_name_t name)`

输入参数：1
`BaseIO_name_t name`引脚名称，枚举类型（见 [LLA_baseIO](baseio.md) 中定义）。

返回值：无

##### 例程
```cpp
/* 定义外部中断回调 */
void exti_cb(void){
 LLA_EXTI_DetachInterrupt(PA4);//关闭PA4的外部回调
}
LLA_EXTI_AttachInterrupt(PA4,exti_cb,LLA_EXTI_TYPE_FALLING);//为PA4设置外部中断及回调函数

/* 以上程序当外部中断触发满足后只会触发一次exti_cb */
```