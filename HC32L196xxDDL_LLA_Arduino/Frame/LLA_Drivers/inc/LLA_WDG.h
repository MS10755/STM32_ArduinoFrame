#ifndef __LLA_WDG_H__
#define __LLA_WDG_H__


/* std lib includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>



#ifdef __cplusplus
extern "C" {
#endif


void lLA_WDG_Start(uint16_t ms);
void LLA_WDG_Feed(void);
void LLA_WDG_Stop(void);
float LLA_WDG_GetOVRTime(void);
#ifdef __cplusplus
}
#endif


#endif
