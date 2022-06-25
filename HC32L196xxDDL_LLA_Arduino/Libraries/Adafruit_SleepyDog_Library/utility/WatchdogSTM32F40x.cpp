// Be careful to use a platform-specific conditional include to only make the
// code visible for the appropriate platform.  Arduino will try to compile and
// link all .cpp files regardless of platform.
#if defined(ARDUINO_STM32F40x)

#include <avr/interrupt.h>


#include "WatchdogSTM32F40x.h"
#include "iwdg.h"

int WatchdogSTM32F40x::enable(int maxPeriodMS) {
	IWDG_Start(maxPeriodMS);
  return maxPeriodMS;
}

void WatchdogSTM32F40x::reset() {
 IWDG_Reload();
}

void WatchdogSTM32F40x::disable() {
  // Disable the watchdog and clear any saved watchdog timer value.
	
}

int WatchdogSTM32F40x::sleep(int maxPeriodMS) {
  int actualMS;
  // Return how many actual milliseconds were spent sleeping.
  return actualMS;
}

void WatchdogSTM32F40x::_setPeriod(int maxMS, int &wdto, int &actualMS) {
 
}

#endif
