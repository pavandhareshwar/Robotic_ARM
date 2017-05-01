/*
 * Project_LESENSE_Main.h
 *
 *  Created on: Apr 1, 2017
 *      Author: pavandhareshwar
 */

#ifndef INC_PROJECT_LESENSE_MAIN_H_
#define INC_PROJECT_LESENSE_MAIN_H_

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_acmp.h"
#include "em_gpio.h"

#ifdef USE_INT
#include "em_int.h"
#else
#include "em_core.h"
#endif

#include "em_letimer.h"

/************************************* MACROS ***************************************/

//#define LED_PORT gpioPortE
//#define LED_PIN  2

#define CHANNEL_EIGHT_TOUCH_VAL	256
#define CHANNEL_NINE_TOUCH_VAL	512
#define CHANNEL_TEN_TOUCH_VAL	1024
#define CHANNEL_ELEVEN_TOUCH_VAL	2048

/************************************* MACROS ***************************************/

/**************************** STRUCTURES/ENUMERATIONS *******************************/

/**************************** STRUCTURES/ENUMERATIONS *******************************/

/************************************ GLOBALS ***************************************/

/************************************ GLOBALS ***************************************/

/****************************** FUNCTION PROTOTYPES *********************************/

void LESENSE_SetUp(void);

/****************************** FUNCTION PROTOTYPES *********************************/

#endif /* INC_PROJECT_LESENSE_MAIN_H_ */
