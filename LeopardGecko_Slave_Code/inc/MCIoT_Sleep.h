#ifndef _MCIOT_SLEEP_H_
#define _MCIOT_SLEEP_H_
#endif

/************************************ INCLUDES **************************************/

#ifdef USE_INT
#include "em_int.h"
#else
#include "em_core.h"
#endif

/************************************ INCLUDES **************************************/

/****************************** FUNCTION PROTOTYPES *********************************/

void blockSleepMode(ENERGY_MODES e_letimer_energy_modes);

void unblockSleepMode(ENERGY_MODES e_letimer_energy_modes);

void Sleep(void);

/****************************** FUNCTION PROTOTYPES *********************************/
