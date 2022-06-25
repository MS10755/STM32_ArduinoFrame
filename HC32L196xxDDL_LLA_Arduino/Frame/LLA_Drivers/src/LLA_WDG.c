#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "wdt.h"

/************************ User type define ****************************/


/************************ User user variables ****************************/
static const float wdg_ovr_time_list[]={1.6,3.2,6.4,13,26,51,102,205,500,820,1640,3280,6550,13100,26200,52400};
static uint8_t ovr_index;
/************************ User micro define ****************************/
#define WDG_OVR_TIME_LIST_LEN	(sizeof(wdg_ovr_time_list)/sizeof(wdg_ovr_time_list[0]))

/************************ User static function ****************************/
static en_wdt_time_t get_best_time(uint16_t ms){
	for(uint8_t i=0;i<WDG_OVR_TIME_LIST_LEN;i++){
		if(wdg_ovr_time_list[i]> ms){
			return (en_wdt_time_t)i;
		}
	}
	return (en_wdt_time_t)0xFF;
}

/************************ LLA common micro define ****************************/

/************************ LLA common variables ****************************/


/************************ LLA common function ****************************/


/************************ LLA Interface function ****************************/
void lLA_WDG_Start(uint16_t ms){
	Sysctrl_SetPeripheralGate(SysctrlPeripheralWdt,TRUE);

	en_wdt_time_t wdt_time = get_best_time(ms);
	LLA_ASSERT(wdt_time!=(en_wdt_time_t)0xFF,DriverCode_WDG,WDG_errorTimeOutChipSupport);
	Wdt_Init(WdtResetEn,wdt_time);
	Wdt_Start();
	ovr_index = (uint8_t)wdt_time;
}

float LLA_WDG_GetOVRTime(){
	if(Wdt_ReadwdtStatus()){
		return wdg_ovr_time_list[ovr_index];
	}
	return 0;
}
void LLA_WDG_Feed(){
	Wdt_Feed();
}

void LLA_WDG_Stop(){
	LLA_ASSERT(0,DriverCode_WDG,WDG_errorStop);
}

/************************ Your Interrupt handling function ****************************/


