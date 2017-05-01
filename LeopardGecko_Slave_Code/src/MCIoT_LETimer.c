/*****************************************************************************
 * @file 	MCIoT_LETimer.c
 * @brief 	This file describes the functions pertaining to LETIMER setup and
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
#include "MCIoT_LETimer.h"
#include "MCIoT_CMU.h"
#include "MCIoT_ACMP.h"
#include "MCIoT_GPIO.h"
#include "MCIoT_Sleep.h"
#include "MCIoT_ADC.h"
#include "MCIoT_DMA.h"
#include "MCIoT_I2C.h"
#include "MCIoT_LEUART.h"

/************************************ INCLUDES **************************************/

/************************************************************************************
 * @function 	LETimer_Config_LETimer
 * @params 		None
 * @brief 		Configures and starts the LETIMER0.
 ************************************************************************************/
void LETimer_Config_LETimer(LETIMER_TypeDef *LETimer,
							LETIMER_Init_TypeDef letimer_init_params)
{
	uint32_t comp0_val = 0;
	uint32_t comp1_val = 0;
	uint32_t letimer_sync_busy = 0;

	blockSleepMode(LETIMER_MIN_ENERGY_MODE);

	if (e_letimer_energy_modes == ENERGY_MODE_EM3)
	{
		comp0_val = ULFRCO_FREQUENCY*osc_ratio*ALS_EXCITE_PERIOD;
		comp1_val = comp0_val - ULFRCO_FREQUENCY*osc_ratio*ALS_MIN_EXCITE_PERIOD;
	}
	else
	{
		comp0_val = (LFXO_FREQUENCY/prescaled_two_power)*ALS_EXCITE_PERIOD;
		comp1_val = comp0_val - (LFXO_FREQUENCY/prescaled_two_power)*ALS_MIN_EXCITE_PERIOD;
	}

#ifdef USE_ACTIVE_ALS
	letimer0_comp0_period_count = 0;
	letimer0_comp1_period_count = 0;
#endif

	LETimer_Config(LETimer, letimer_init_params, true, comp0_val, true, comp1_val);

	while ((letimer_sync_busy = LETimer->SYNCBUSY & LETIMER_SYNCBUSY_CMD) == LETIMER_SYNCBUSY_CMD);

	letimer_sync_busy = 1;

	while ((letimer_sync_busy = LETimer->SYNCBUSY & LETIMER_SYNCBUSY_CTRL) == LETIMER_SYNCBUSY_CTRL);

	/* Enable the LETIMER Interrupts */
	LETIMER_Interrupt_Enable(LETimer);

}

/************************************************************************************
 * @function 	LETIMER_Setup
 * @params 		None
 * @brief 		Sets up the LETimer0.
 ************************************************************************************/
void LETIMER_Setup(LETIMER_TypeDef *LETimer,
					LETIMER_Init_TypeDef letimer_init_params)
{
	uint32_t letimer_sync_busy;

	/* Initialize LETIMER */
	LETIMER_Init(LETimer, &letimer_init_params);

	/* Wait for setting of a cmd register to synchronize into low frequency domain */
	while ((letimer_sync_busy = LETimer->SYNCBUSY & LETIMER_SYNCBUSY_CMD) == LETIMER_SYNCBUSY_CMD);

	letimer_sync_busy = 1;

	/* Wait for setting of a ctrl register to synchronize into low frequency domain */
	while ((letimer_sync_busy = LETimer->SYNCBUSY & LETIMER_SYNCBUSY_CTRL) == LETIMER_SYNCBUSY_CTRL);
}

/************************************************************************************
 * @function 	LETimer_Config
 * @params 		None
 * @brief 		Configures the LETIMER0.
 ************************************************************************************/
void LETimer_Config(LETIMER_TypeDef *LETimer,
					LETIMER_Init_TypeDef letimer_init_params,
					bool config_comp0_val, uint32_t comp0_val,
					bool config_comp1_val, uint32_t comp1_val)
{
	uint32_t letimer_sync_busy;

	/* Enable necessary clocks */
	if (e_letimer_energy_modes == ENERGY_MODE_EM3)
	{
		CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true); /* To enable the ULFRCO */
		CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO); /* To support energy modes EM3 */
	}
	else
	{
		CMU_OscillatorEnable(cmuOsc_LFXO, true, true); /* To enable the LFXO */
		CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO); /* To support energy modes EM0-EM2 */
	}

	LETIMER_Setup(LETimer, letimer_init_params);

	/*  Set initial compare values for COMP0 and COMP1
	    COMP1 keeps it's value and is used as TOP value for the LETIMER.
		COMP1 gets decremented through the program execution
		to generate a different PWM duty cycle */

	if (true == config_comp0_val)
	{
		/* Setting the LETimer compare registers comp0 and comp1 */
		LETIMER_CompareSet(LETimer, 0, comp0_val);

		/* Waiting for setting of a compare register 0 to synchronize into low frequency domain */
		while ((letimer_sync_busy = LETimer->SYNCBUSY & LETIMER_SYNCBUSY_COMP0) == 1);

		letimer_sync_busy = 1;
	}

	if (true == config_comp1_val)
	{
		/* Setting the LETimer compare registers comp0 and comp1 */
		LETIMER_CompareSet(LETimer, 1, comp1_val);

		/* Waiting for setting of a compare register 1 to synchronize into low frequency domain */
		while ((letimer_sync_busy = LETimer->SYNCBUSY & LETIMER_SYNCBUSY_COMP1) == 1);
	}
}

/************************************************************************************
 * @function 	LETIMER_Interrupt_Enable
 * @params 		None
 * @brief 		Enables the necessary interrupts for LETIMER0.
 ************************************************************************************/
void LETIMER_Interrupt_Enable(LETIMER_TypeDef *LETimer)
{
	/* Clear all the interrupts that may have been set-up inadvertently */
	LETimer->IFC |= (LETIMER_IF_COMP0 | LETIMER_IF_COMP1 | LETIMER_IF_UF | LETIMER_IF_REP0 | LETIMER_IF_REP1);

	//-----------------------------------------------------------------------------------
	// THE INTERRUPT IS SIMPLY TO DECREASE THE VALUE OF COMP1 TO VARY THE PWM DUTY CYCLE
	//-----------------------------------------------------------------------------------

	/* Enable comp0 interrupt */
	LETIMER_IntEnable(LETimer, LETIMER_IF_COMP0);

	/* Enable comp1 interrupt */
	LETIMER_IntEnable(LETimer, LETIMER_IF_COMP1);
}

/************************************************************************************
 * @function 	LETIMER0_IRQHandler
 * @params 		None
 * @brief 		Interrupt Service Routine for LETIMER0.
 ************************************************************************************/
void LETIMER0_IRQHandler(void)
{
#ifndef USE_ACTIVE_ALS
	uint32_t acmp_out_val = 0;
#endif

#ifdef USE_INT
	INT_Disable();
#else
	CORE_DECLARE_IRQ_STATE;
	CORE_ENTER_ATOMIC();
#endif

	/* COMP0 Interrupt */
	if ((LETIMER0->IF & LETIMER_IF_COMP0) == LETIMER_IF_COMP0)
	{
		/* ACMP part */
		/* Clearing the source of interrupt: LETIMER0 comp0 interrupt flag */
		LETIMER_IntClear(LETIMER0, LETIMER_IF_COMP0);

		/* Turning on LED0 */
		//LED_On(LED0_1_GPIO_PORT, LED0_GPIO_PIN);
#ifdef USE_ANY_ALS
#ifndef USE_ACTIVE_ALS

		CMU_ClockEnable(cmuClock_ACMP0, true); /* To enable clock to ACMP0 */

		/* Turn on ACMP */
		ACMP0->CTRL |= ACMP_CTRL_EN;

		GPIO_PinModeSet(ALS_GPIO_PORT, ALS_SENSE_GPIO_PIN, gpioModePushPull, 1);

		/* Waiting for the ACMPACT bit to be set in ACMP0_STATUS register
		 * indicating that the ACMP is warmed up */
		while ((ACMP0->STATUS & ACMP_STATUS_ACMPACT) != ACMP_STATUS_ACMPACT);
#else

		if (letimer0_comp0_period_count % 3 == 1)
		{
			/* LETimer Periods 1,4,7,... */
			/* Monitor the TSL2651 via its interrupt line */
			/* Do Nothing */
		}
		else if (letimer0_comp0_period_count % 3 == 2)
		{
			/* LETimer Periods 2,5,8,... */
			/* Disable the TSL2651 */

			I2C_TSL_PowerOff_Routine();
		}
		else
		{
			/* LETimer Periods 0,3,6,... */

			/* 1. Enable the TSL2651 onto the I2C bus
			 * 2. Initialize/start the TSL2651
			 * 3. monitor the TSL2651 via its interrupt line */

			I2C_TSL_PowerOn_Routine();
		}
#endif
#endif

		/* ADC part*/
		CMU_ClockEnable(cmuClock_ADC0, true); /* To enable clock to ADC0 */

#ifndef USE_DMA_FOR_ADC
		/* Set up the DMA */
		DMA_ActivateBasic(DMA_CHANNEL_ADC, 		/* DMA channel to activate DMA cycle for */
				true,							/* Use primary descriptor */
				false,							/* Not using burst feature */
				(void *)ADCDataRAMBuffer,		/* Destination address */
				(void *) &(ADC0->SINGLEDATA),	/* Source address */
				ADC_SAMPLES-1); 				/* Number of elements to transfer */;
#endif

		blockSleepMode(ADC_EM);

		/* Turn on ADC */
		ADC_Start(ADC0, ADC_CONVERSION_TYPE_CMD);

		/* Waiting for the WARM bit to be set in ADC0_STATUS register
		 * indicating that the ADC is warmed up */
		while ((ADC0->STATUS & ADC_STATUS_WARM) != ADC_STATUS_WARM);

#ifdef USE_ACTIVE_ALS
		letimer0_comp0_period_count++;
#endif

	}

	/* COMP1 Interrupt */
	if ((LETIMER0->IF & LETIMER_IF_COMP1) == LETIMER_IF_COMP1)
	{
		/* Clearing the source of interrupt: LETIMER0 comp1 interrupt flag */
		LETIMER_IntClear(LETIMER0, LETIMER_IF_COMP1);

		/* Turning off LED0 */
		//LED_Off(LED0_1_GPIO_PORT, LED0_GPIO_PIN);

#ifdef USE_ANY_ALS
#ifdef USE_ACTIVE_ALS
		if (letimer0_comp1_period_count % 3 == 1)
		{
			/* LETimer Periods 1,4,7,... */
			/* Monitor the TSL2651 via its interrupt line */
			/* Do Nothing */
		}
		else if (letimer0_comp1_period_count % 3 == 2)
		{
			/* LETimer Periods 2,5,8,... */
			/* Disable the TSL2651 */

			I2C_TSL_Deinit();
		}
		else
		{
			/* LETimer Periods 0,3,6,... */

			/* 1. Enable the TSL2651 onto the I2C bus
			 * 2. Initialize/start the TSL2651
			 * 3. monitor the TSL2651 via its interrupt line */

			I2C_TSL_Init();
		}
#else
		/* Check the ACMP out value and decide the action and switch off ACMP */
		acmp_out_val = (ACMP0->STATUS & ACMP_STATUS_ACMPOUT) >> 1;

		if (!acmp_out_val)
		{
			/* Turning on LED1 */
			LED_On(LED0_1_GPIO_PORT, LED0_GPIO_PIN);

			leuart_led_data = ((LEUART_LED_DATA_IDENTIFIER << 7) | 0);

			/* Changing the ACMP reference to check the state of ALS GPIO
			 * for brightness on next iteration */
			ACMP0->INPUTSEL &= ~(DARK_REFERENCE_VDDLEVEL << _ACMP_INPUTSEL_VDDLEVEL_SHIFT);
			ACMP0->INPUTSEL |= (LIGHT_REFERENCE_VDDLEVEL << _ACMP_INPUTSEL_VDDLEVEL_SHIFT);
		}
		else
		{
			/* Turning off LED1 */
			LED_Off(LED0_1_GPIO_PORT, LED0_GPIO_PIN);

			leuart_led_data = ((LEUART_LED_DATA_IDENTIFIER << 7) | 1);

			/* Changing the ACMP reference to check the state of ALS GPIO
			 * for darkness on next iteration */
			ACMP0->INPUTSEL &= ~(LIGHT_REFERENCE_VDDLEVEL << _ACMP_INPUTSEL_VDDLEVEL_SHIFT);
			ACMP0->INPUTSEL |= (DARK_REFERENCE_VDDLEVEL << _ACMP_INPUTSEL_VDDLEVEL_SHIFT);
		}

		if (false == state_is_em1_for_leuart_tx)
		{
			blockSleepMode(LEUART_EM);
			state_is_em1_for_leuart_tx = true;
		}

#if 0
#ifdef USE_CIRC_BUFFER_FOR_LEUART
		WriteDataToCircBuff(&leuart_circ_buff, leuart_led_data);
#else
		*puart_buffer = leuart_led_data;
		//puart_buffer++;
#endif
#endif
		led_data_available = true;

		if (false == leuart_nvic_is_enabled)
		{
			/* Enable the NVIC for LEUART0 so that the IRQ handler can be
			 * triggered to transmit the data */
			NVIC_EnableIRQ(LEUART0_IRQn);
			leuart_nvic_is_enabled = true;
		}

		/* Turn off the ACMP */
		ACMP0->CTRL &= ~ACMP_CTRL_EN;

		CMU_ClockEnable(cmuClock_ACMP0, false); /* To disable clock to ACMP0 */

		GPIO_PinModeSet(ALS_GPIO_PORT, ALS_SENSE_GPIO_PIN, gpioModeDisabled, 0);
#endif


#ifdef USE_ACTIVE_ALS
		letimer0_comp1_period_count++;
#endif
#endif
	}

#ifdef USE_INT
	INT_Enable();
#else
	CORE_EXIT_ATOMIC();
#endif
}
