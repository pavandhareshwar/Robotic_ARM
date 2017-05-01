#ifndef _MCIOT_ACMP_H_
#define _MCIOT_ACMP_H_
#endif

/************************************ INCLUDES **************************************/

#include "em_acmp.h"

/************************************ INCLUDES **************************************/

/************************************* MACROS ***************************************/

#define ACMP_POS_CHANNEL			acmpChannel6
#define ACMP_NEG_CHANNEL			acmpChannelVDD

/************************************* MACROS ***************************************/

/****************************** FUNCTION PROTOTYPES *********************************/

void ACMP_SetUp(void);

void ACMP_Interrupt_Enable(void);

/****************************** FUNCTION PROTOTYPES *********************************/
