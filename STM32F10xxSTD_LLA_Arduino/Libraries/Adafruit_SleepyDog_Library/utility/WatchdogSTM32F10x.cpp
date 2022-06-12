// Be careful to use a platform-specific conditional include to only make the
// code visible for the appropriate platform.  Arduino will try to compile and
// link all .cpp files regardless of platform.
#if defined(ARDUINO_STM32)

#include <avr/interrupt.h>


#include "WatchdogSTM32F10x.h"
#include "Frame.h"


int WatchdogSTM32F10x::enable(int maxPeriodMS) {
	lLA_WDG_Start(maxPeriodMS);
  return maxPeriodMS;
}

void WatchdogSTM32F10x::reset() {
 LLA_WDG_Feed();
}

void WatchdogSTM32F10x::disable() {
  // Disable the watchdog and clear any saved watchdog timer value.
	
}

int WatchdogSTM32F10x::sleep(int maxPeriodMS) {
  int actualMS;
  // Return how many actual milliseconds were spent sleeping.
  return actualMS;
}

void WatchdogSTM32F10x::_setPeriod(int maxMS, int &wdto, int &actualMS) {
 
}

#endif
