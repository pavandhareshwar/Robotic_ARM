#ifndef _MCIOT_LETIMER_H_
#define _MCIOT_LETIMER_H_
#endif

/************************************ INCLUDES **************************************/

#include "em_letimer.h"

/************************************ INCLUDES **************************************/

/************************************* MACROS ***************************************/

#define LETIMER0_MAX_COUNT			65536

#define LETIMER_MIN_ENERGY_MODE		EM3

/************************************* MACROS ***************************************/

/************************************ GLOBALS ***************************************/
#ifdef USE_ACTIVE_ALS
uint32_t letimer0_comp0_period_count;
uint32_t letimer0_comp1_period_count;
#endif

/************************************ GLOBALS ***************************************/

/****************************** FUNCTION PROTOTYPES *********************************/

void LETimer_Config_LETimer(LETIMER_TypeDef *LETimer,
							LETIMER_Init_TypeDef letimer_init_params);

void LETIMER_Setup(LETIMER_TypeDef *LETimer,
					LETIMER_Init_TypeDef letimer_init_params);

void LETimer_Config(LETIMER_TypeDef *LETimer,
					LETIMER_Init_TypeDef letimer_init_params,
					bool config_comp0_val, uint32_t comp0_val,
					bool config_comp1_val, uint32_t comp1_val);

void LETIMER_Interrupt_Enable(LETIMER_TypeDef *LETimer);

void LETIMER0_IRQHandler(void);

/****************************** FUNCTION PROTOTYPES *********************************/
