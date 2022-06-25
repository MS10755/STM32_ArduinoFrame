#include "LLA_Drivers.h"

/************************ LLA Interface headers includes ****************************/
/* Chip SDK headers includes */
#include "HC32L196KCTA.h"               // Device header


/************************ User type define ****************************/


/************************ User user variables ****************************/


/************************ User micro define ****************************/


/************************ User static function ****************************/


/************************ LLA common micro define ****************************/

/************************ LLA common variables ****************************/


/************************ LLA common function ****************************/


/************************ LLA Interface function ****************************/
void LLA_SYS_IRQ_Enable(){
	__set_PRIMASK(0);
}



void LLA_SYS_IRQ_Disable(){
	__set_PRIMASK(1);
}


/************************ Your Interrupt handling function ****************************/


