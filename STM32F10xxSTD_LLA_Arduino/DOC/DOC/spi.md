# LLA_SPI模块
[toc]

### 1. API一览
|序号|函数原型|作用|
|:--:|--|--|
|1|`void LLA_SPI_Init(LLA_SPI_t spi,uint32_t clock,LLA_SPI_Mode_t mode,LLA_SPI_CS_t cs_mode)`|初始化SPI模块|
|2|`void LLA_SPI_DeInit(LLA_SPI_t spi)`|失能SPI模块|
|3|`void LLA_SPI_WriteBytes(LLA_SPI_t spi,uint8_t * data,uint32_t len)`|SPI写数据|
|4|`void LLA_SPI_ReadBytes(LLA_SPI_t spi,uint8_t * buf,uint32_t len)`|SPI读数据|

### 2. 类型介绍
#### 2.1 LLA_SPI_t（SPI名称）
```cpp
typedef enum{
	LLA_SPI0,
	LLA_SPI1,
	LLA_SPI2,
	LLA_SPI3,
	
	_LLA_SPI_MAX
}LLA_SPI_t;
```
#### 2.2 LLA_SPI_Mode_t（SPI时钟模式）
```cpp
typedef enum{
	LLA_SPI_Mode_CLK_L_CPHA_L,//时钟空闲为低电平，第一边沿采样
	LLA_SPI_Mode_CLK_L_CPHA_H,//时钟空闲为低电平，第二边沿采样
	LLA_SPI_Mode_CLK_H_CPHA_L,//时钟空闲为高电平，第一边沿采样
	LLA_SPI_Mode_CLK_H_CPHA_H,//时钟空闲为高电平，第二边沿采样
	
	_LLA_SPI_MODE_MAX
}LLA_SPI_Mode_t;
```

#### 2.3 LLA_SPI_CS_t（SPI片选模式）
```cpp
typedef enum{
	LLA_SPI_CS_soft,//软件控制片选
	LLA_SPI_CS_Auto,//自动片选
	
	_LLA_SPI_CS_MAX
}LLA_SPI_CS_t;
```

### 3. 函数介绍

#### 3.1 LLA_SPI_Init （初始化SPI模块）
函数原型`void LLA_SPI_Init(LLA_SPI_t spi,uint32_t clock,LLA_SPI_Mode_t mode,LLA_SPI_CS_t cs_mode)`

输入参数：4
`LLA_SPI_t spi`spi名称，枚举类型。
`uint32_t clock`时钟频率，单位hz
`LLA_SPI_Mode_t mode`时钟模式，枚举类型。
`LLA_SPI_CS_t cs_mode`片选模式，枚举类型。

返回值：无
##### 例程
```cpp
LLA_SPI_Init(LLA_SPI0,24000000,LLA_SPI_Mode_CLK_L_CPHA_L,LLA_SPI_CS_soft);//设置SPI0时钟频率24Mhz，上升沿采样，软件控制片选
```

#### 3.2 LLA_SPI_DeInit （失能SPI模块）
函数原型`void LLA_SPI_DeInit(LLA_SPI_t spi)`

输入参数：1
`LLA_SPI_t spi`spi名称，枚举类型。

返回值：无
##### 例程
```cpp
LLA_SPI_Init(LLA_SPI0,24000000,LLA_SPI_Mode_CLK_L_CPHA_L,LLA_SPI_CS_soft);//设置SPI0时钟频率24Mhz，上升沿采样，软件控制片选
LLA_SPI_DeInit(LLA_SPI0);//使能SPI0
```


#### 3.3 LLA_SPI_WriteBytes （SPI写数据）
函数原型`void LLA_SPI_WriteBytes(LLA_SPI_t spi,uint8_t * data,uint32_t len)`

输入参数：3
`LLA_SPI_t spi`spi名称，枚举类型。
`uint8_t * data`写入数据的指针
`uint32_t len` 数据长度

返回值：无
##### 例程
```cpp
uint8_t data[]={0xc2,0x63,0x7d};
LLA_BaseIO_Write(PA4,BaseIOStatus_LOW);//片选选中(见 LLA_baseIO)
LLA_SPI_WriteBytes(LLA_SPI0,data,sizeof(data));
LLA_BaseIO_Write(PA4,BaseIOStatus_HIGH);//片选释放
```

#### 3.4 LLA_SPI_ReadBytes （SPI读数据）
函数原型`void LLA_SPI_ReadBytes(LLA_SPI_t spi,uint8_t * buf,uint32_t len)`

输入参数：3
`LLA_SPI_t spi`spi名称，枚举类型。
`uint8_t * data`缓存数据的指针
`uint32_t len` 读取数据长度

返回值：无
##### 例程
```cpp
uint8_t data[6]={0};
LLA_BaseIO_Write(PA4,BaseIOStatus_LOW);//片选选中(见 LLA_baseIO)
LLA_SPI_ReadBytes(LLA_SPI0,data,sizeof(data));//从SPI0读取6字节数据到data数组
LLA_BaseIO_Write(PA4,BaseIOStatus_HIGH);//片选释放
```