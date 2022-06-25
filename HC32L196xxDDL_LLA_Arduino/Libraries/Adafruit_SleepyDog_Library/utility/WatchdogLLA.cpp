// Be careful to use a platform-specific conditional include to only make the
// code visible for the appropriate platform.  Arduino will try to compile and
// link all .cpp files regardless of platform.
#if defined(LLA_DRIVERS)

#include <avr/interrupt.h>


#include "WatchdogLLA.h"
#include "LLA_Drivers.h"

int WatchdogLLA::enable(int maxPeriodMS) {
	lLA_WDG_Start(maxPeriodMS);
  return LLA_WDG_GetOVRTime()+1;
}

void WatchdogLLA::reset() {
	LLA_WDG_Feed();
}

void WatchdogLLA::disable() {
  // Disable the watchdog and clear any saved watchdog timer value.
	LLA_WDG_Stop();
}

int WatchdogLLA::sleep(int maxPeriodMS) {
  int actualMS;
  // Return how many actual milliseconds were spent sleeping.
  return actualMS;
}

void WatchdogLLA::_setPeriod(int maxMS, int &wdto, int &actualMS) {
 
}

#endif
