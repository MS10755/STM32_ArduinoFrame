# STM32_ArduinoFrame
## 框架介绍
STM32_ArduinoFrame 框架可以使Arduino 平台编写的应用程序几乎不需做任何更改，就可以在KEIL （目前支持AC5和AC6编译器）完美编译并执行。
注意：Arduino平台的部分第三方库必须使用AC6编译器才能编译通过。因此强烈建议大家抛弃AC5编译器而改用AC6编译器以更好支持C++代码的编译。

---

STM32_ The arduinorframe framework enables applications written on the Arduino platform to be perfectly compiled and executed in keil (currently supporting AC5 and AC6 compilers) without any changes.
Note: some third-party libraries of the Arduino platform must be compiled with the AC6 compiler. Therefore, it is strongly recommended that you abandon the AC5 compiler and use the AC6 compiler to better support the compilation of c++ code.

## 框架结构

![软件框架](STM32F10xxSTD_LLA_Arduino/DOC/DOC/images/frame.jpg)



## 各个层级说明

![软件框架](DOC/images/frameDescription.PNG)

## 第三方库移植说明
使用Arduino 标准API 编写的第三方库或者硬件无关库（例如算法库等抽象库）理论上只需要只需要将其源文件添加进工程中，并在工程 Include Paths 中添加其头文件目录即可使用。若第三方库直接使用寄存器级别代码编写，则需要根据所使用的硬件进行修改。

---

Third party libraries or hardware independent libraries (such as algorithm libraries and other abstract libraries) written using Arduino standard API theoretically only need to add their source files to the project and add their header file directories in the project include paths. If the third-party library is written directly using register level code, it needs to be modified according to the hardware used.

## 鸣谢
特别感谢 [**@_VIFEXTech**](https://github.com/FASTSHIFT) 大佬对我开发过程中的启发和帮助，使用Arduino 在Keil中开发STM32也是他的灵感。我只是按照自己的使用习惯进行了重写。

---

Special thanks [**@_VIFEXTech**](https://github.com/FASTSHIFT) He inspired and helped me in the development process. He was also inspired to use Arduino to develop STM32 in keil. I just rewritten it according to my own usage habits.
