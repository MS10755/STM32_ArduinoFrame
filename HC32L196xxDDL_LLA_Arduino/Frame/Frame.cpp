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
