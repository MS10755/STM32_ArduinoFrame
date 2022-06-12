/*
 * @Description: Frame start sequence for initialization module like ADC / PWM / SystemClock etc.
 * @Autor: Juan
 * @Date: 2022-04-01 14:49:54
 * @LastEditors: Juan
 * @LastEditTime: 2022-04-01 15:08:02
 */
#include "Frame.h"

extern void setup(void);
extern void loop(void);
int main(){
	init();
	setup();
	for(;;){
		loop();
	}
}
