# LLA_flash模块

[toc]

### 1. API一览
|序号|函数原型|作用|
|:--:|--|--|
|1|`void LLA_Flash_Init(void)`|初始化Flash控制模块|
|2|`uint32_t LLA_Flash_GetSize(void)`|获取Flash总可用大小|
|3|`void LLA_Flash_Erase(uint32_t offset,uint32_t len)`|擦除FLASH|
|4|`void LLA_Flash_WriteBytes(uint32_t offset,uint8_t *data,uint32_t len)`|写入Flash数据|
|5|`void LLA_Flash_ReadBytes(uint32_t offset,uint8_t *data,uint32_t len)`|读取Flash数据|
### 2. 函数介绍
#### 2.1 LLA_Flash_Init（初始化Flash控制模块）
函数原型`void LLA_Flash_Init(void)`

输入参数：无

返回值：无
##### 例程
```cpp
该函数由框架自动调用，无需用户调用。
```

#### 2.2 LLA_Flash_GetSize（获取Flash总可用大小）
函数原型`uint32_t LLA_Flash_GetSize(void)`

输入参数：无

返回值：`uint32_t`Flash可用大小（字节）
##### 例程
```cpp
uint32_t flash_size = LLA_Flash_GetSize(void);
```

#### 2.3 LLA_Flash_Erase（擦除Flash）
函数原型`void LLA_Flash_Erase(uint32_t offset,uint32_t len)`

输入参数：2
`uint32_t offset`擦除区域起始位置，0表示从Flash可用区域0位置开始。
`uint32_t len`擦除长度，`len`必须小于Flash总大小

返回值：无
##### 例程
```cpp
LLA_Flash_Erase(0,LLA_Flash_GetSize());//擦除整个Flash可用区域
LLA_Flash_Erase(0,256);//擦除第一扇区
```
#### 2.4 LLA_Flash_WriteBytes（写入Flash数据）
函数原型`void LLA_Flash_WriteBytes(uint32_t offset,uint8_t *data,uint32_t len)`

输入参数：3
`uint32_t offset`写入区域起始位置，0表示从Flash可用区域0位置开始。
`uint8_t *data`写入数据的指针。
`uint32_t len`写入长度，`len`必须小于Flash总大小。

返回值：无
##### 例程
```cpp
char buf[]="Apple";
float pi = 3.14;
int num = 10;
LLA_Flash_WriteBytes(0,(uint8_t*)buf,sizeof(buf));//写入数组
LLA_Flash_WriteBytes(6,(uint8_t*)&pi,sizeof(float));//写入浮点数
LLA_Flash_WriteBytes(10,(uint8_t*)&num,sizeof(int));//写入整数
```

#### 2.5 LLA_Flash_ReadBytes（读取Flash数据）
函数原型`void LLA_Flash_ReadBytes(uint32_t offset,uint8_t *data,uint32_t len)`

输入参数：3
`uint32_t offset`读取区域起始位置，0表示从Flash可用区域0位置开始。
`uint8_t *data`缓存数据的指针。
`uint32_t len`读取长度，`len`必须小于Flash总大小。

返回值：无
##### 例程
```cpp
uint8_t buf[200]={0};
LLA_Flash_ReadBytes(0,buf,200);//读取Flash前200字节到buf数组
```