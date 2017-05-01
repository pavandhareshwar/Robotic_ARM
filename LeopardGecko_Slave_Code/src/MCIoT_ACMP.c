/*****************************************************************************
 * @file 	MCIoT_ACMP.c
 * @brief 	This file describes the functions pertaining to ACMP setup and
 * 			interrupts.
 * @author 	Pavan Dhareshwar
 * @version 1.0
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2013 Energy Micro AS, http://www.energymicro.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 4. The source and compiled code may only be used on Energy Micro "EFM32"
 *    microcontrollers and "EFR4" radios.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
 * obligation to support this Software. Energy Micro AS is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Energy Micro AS will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ************************************************************************************/

/************************************ INCLUDES **************************************/
#include <stdint.h>
#include <stdbool.h>
#include "MCIoT_main.h"
#include "MCIoT_ACMP.h"
#include "MCIoT_GPIO.h"

/************************************************************************************
 * @function 	ACMP_setup
 * @params 		None
 * @brief 		Configures the ACMP.
 ************************************************************************************/
void ACMP_SetUp(void)
{
	/* Set configurations for ACMP 0 */
	const ACMP_Init_TypeDef acmpInit =
	{
	  .enable         			= false,             		/* Enable ACMP */
	  .fullBias       			= false,                  	/* Full Bias Current */
	  .halfBias 	  			= true,                   	/* Half Bias Current */
	  .biasProg 	 			= 0,                  		/* BiasProg Current Configuration */
	  .interruptOnFallingEdge 	= false,                   	/* Enable interrupt on falling edge */
	  .interruptOnRisingEdge  	= false,                  	/* Enable interrupt on rising edge */
	  .warmTime        			= acmpWarmTime256,        	/* Warm-Up Time for ACMP, should be atleast 10us */
	  .hysteresisLevel        	= _ACMP_CTRL_HYSTSEL_HYST1, /* Hysteresis Configuration */
	  .inactiveValue          	= false,         			/* Inactive comparator active value */
	  .lowPowerReferenceEnabled	= false,       				/* Enable low power mode */
	  .vddLevel        			= DARK_REFERENCE_VDDLEVEL  	/* Vdd reference scaling */
	};

	/* Initialize LETIMER */
	ACMP_Init(ACMP0, &acmpInit);

	/* Configure the ACMP inputs - positive and negative */
	ACMP_ChannelSet(ACMP0, ACMP_NEG_CHANNEL, ACMP_POS_CHANNEL);

	/* Set GPIO PD6 as pushpull */
	GPIO_PinModeSet(ALS_GPIO_PORT, ALS_SENSE_GPIO_PIN, gpioModePushPull, 0);
}

/************************************************************************************
 * @function 	ACMP_Interrupt_Enable
 * @params 		None
 * @brief 		Enables the necessary interrupts for ACMP0.
 ************************************************************************************/
void ACMP_Interrupt_Enable(void)
{
	/* Clearing all the interrupts that may have been set-up inadvertently */
	ACMP0->IFC |= (ACMP_IF_EDGE | ACMP_IF_WARMUP);

	/* Enable edge interrupt */
	ACMP_IntEnable(ACMP0, ACMP_IF_EDGE);
}
