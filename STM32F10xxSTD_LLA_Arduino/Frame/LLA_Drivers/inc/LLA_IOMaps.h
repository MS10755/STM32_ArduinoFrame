#ifndef __LLA_IOMAPS_H__
#define __LLA_IOMAPS_H__


/* std lib includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum{
	LlA_PORT_A,
	LlA_PORT_B,
	LlA_PORT_C,
	LlA_PORT_D,
	LlA_PORT_E,
	LlA_PORT_F,
	LlA_PORT_G,
	
	_LlA_PORT_MAX,
}IOMAP_PortName_t;

typedef struct{
	uint32_t * ODR;
	uint32_t * IDR;
}IOMAP_Port_t;

typedef uint32_t IOMAP_NameMask_t;


extern const IOMAP_Port_t IOMAP_Port_list[_LlA_PORT_MAX];//GPIO Port 映射

IOMAP_NameMask_t LLA_IOMaps_NameMask(BaseIO_name_t name);
IOMAP_Port_t LLA_IOMaps_NameToPort(BaseIO_name_t name);

#ifdef __cplusplus
extern "C" {
#endif





#ifdef __cplusplus
}
#endif


#endif
