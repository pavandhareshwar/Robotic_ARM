#ifndef _MCIOT_CMU_H_
#define _MCIOT_CMU_H_
#endif

/************************************ INCLUDES **************************************/

#include "em_cmu.h"

/************************************ INCLUDES **************************************/

/****************************** FUNCTION PROTOTYPES *********************************/

void CMU_SetUp(void);

void CMU_Obtain_Freq_Count(uint32_t *count, uint32_t freq,
							uint32_t calibration_period);

void Calibrate_ULFRCO(void);

void check_and_comp_prescaler_if(float period, uint32_t osc_cnt);

void cal_prescaler_value(float period, uint32_t osc_cnt);

/****************************** FUNCTION PROTOTYPES *********************************/
