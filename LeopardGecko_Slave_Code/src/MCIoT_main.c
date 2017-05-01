/*****************************************************************************
 * @file 	MCIoT_main.c
 * @brief 	This file describes the main function for Leopard Gecko. It 
 * 			describes the methodology to use the TSL2561 as an active light
 * 			sensor and communicate with the it using I2C protocol.
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
#include "em_device.h"
#include "em_chip.h"
#include "MCIoT_main.h"
#include "MCIoT_LETimer.h"
#include "MCIoT_ACMP.h"
#include "MCIoT_CMU.h"
#include "MCIoT_Timer.h"
#include "MCIoT_GPIO.h"
#include "MCIoT_Sleep.h"
#include "MCIoT_ADC.h"
#include "MCIoT_DMA.h"
#include "MCIoT_I2C.h"
#include "MCIoT_LEUART.h"
#include "MCIoT_LESENSE_Main.h"

/************************************ INCLUDES **************************************/

/************************************************************************************
 * @function 	main
 * @params 		None
 * @brief 		Main Function
 *				Main is called from __iar_program_start, see assembly startup file
 ************************************************************************************/
int main(void)
{
	/* Global Variables Initialization */
	e_letimer_energy_modes = ENERGY_MODE_EM2;

#ifdef ULFRCO_SELF_CALIBRATE
	/* Oscillator Ratio (used for self-calibration) */
	osc_ratio = 0;
#else
	osc_ratio = 1;
#endif

	prescaled_two_power = 0;
	letimer0_prescaler = 0;

	/* Align different chip revisions */
	CHIP_Init();

	/* Compute prescalar value if the energy mode is EM 0-2 */
	if (e_letimer_energy_modes != ENERGY_MODE_EM3)
	{
		check_and_comp_prescaler_if(ALS_EXCITE_PERIOD, LFXO_FREQUENCY);
	}

	CMU_SetUp();

	is_lesense_auth_done = false;

	LESENSE_SetUp();

	if (true == is_lesense_auth_done)
	{
		/* Align different chip revisions */
		CHIP_Init();

		/* Compute prescalar value if the energy mode is EM 0-2 */
		if (e_letimer_energy_modes != ENERGY_MODE_EM3)
		{
			check_and_comp_prescaler_if(ALS_EXCITE_PERIOD, LFXO_FREQUENCY);
		}

		CMU_SetUp();

		blockSleepMode(e_letimer_energy_modes);

		const LETIMER_Init_TypeDef letimer0_init_params =
		{
				.enable         = false,                  	/* Don't start counting when init completed. */
				.debugRun       = false,                  	/* Counter shall not keep running during debug halt. */
				.rtcComp0Enable = false,                  	/* Don't start counting on RTC COMP0 match. */
				.rtcComp1Enable = false,                  	/* Don't start counting on RTC COMP1 match. */
				.comp0Top       = true,                   	/* Load COMP0 register into CNT when counter underflows. COMP0 is used as TOP */
				.bufTop         = false,                  	/* Don't load COMP1 into COMP0 when REP0 reaches 0. */
				.out0Pol        = 0,                      	/* Idle value for output 0. */
				.out1Pol        = 0,                      	/* Idle value for output 1. */
				.ufoa0          = letimerUFOAPwm,         	/* PWM output on output 0 */
				.ufoa1          = letimerUFOAPulse,       	/* Pulse output on output 1*/
				.repMode        = letimerRepeatFree    		/* Count until stopped */
		};

		LETimer_Config_LETimer(LETIMER0, letimer0_init_params);

		GPIO_SetUp();

#ifndef USE_ACTIVE_ALS
		/* Configure ACMP */
		//	ACMP_SetUp();
#else
		I2C_SetUp();
#endif

#ifdef ENABLE_ADC_MODULE
		/* ADC SetUp */
		ADC_SetUp();
#endif

#ifdef USE_DMA_FOR_ADC
		/* DMA SetUp */
		DMA_SetUp();
#endif

#ifdef ENABLE_LEUART_MODULE
		LEUART_SetUp();
#endif

		/* Start LETIMER */
		LETIMER0->CMD |= LETIMER_CMD_START;

		/* Enable LETIMER0 interrupt vector in NVIC*/
		NVIC_EnableIRQ(LETIMER0_IRQn);

		while(1)
		{
			Sleep();
		}
	}
}

/* This code is originally copy righted by Silicon Labs and it grants permission
 * to anyone to use this software for any purpose, including commercial applications,
 * and to alter it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Unused part of the code is commented to avoid confusion.
 *
 * Name/argument list of routines may have been changed to confirm to the
 * naming convention of the developer.
 *
 * Routines include:
 *
 * void LETIMER0_IRQHandler(void);
 * void blockSleepMode(ENERGY_MODES e_letimer_energy_modes);
 * void LETIMER_Setup(void);
 * void LETIMER_Interrupt_Enable(void);
 * void Sleep(void);
 */
