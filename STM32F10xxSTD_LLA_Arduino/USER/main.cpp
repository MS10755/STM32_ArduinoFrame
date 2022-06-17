#include <Arduino.h>
#include "MillisTaskManager.h"


MillisTaskManager taskManager;

void task1(){

	Serial.printf("[%d] task1\r\n",millis());
}

void task2(){
	Serial.printf("[%d] task2\r\n",millis());
}

void setup(){
	Serial.begin(115200);
	taskManager.Register(task1,1000);
	taskManager.Register(task2,800);
}


void loop(){
	taskManager.Running(millis());

}
