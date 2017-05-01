/*****************************************************************************
 * @file 	MCIoT_CMU.c
 * @brief 	This file describes the functions pertaining to CMU setup and
 * 			ULFRCO self calibration.
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
#include "em_cmu.h"
#include "MCIoT_main.h"
#include "MCIoT_LETimer.h"
#include "MCIoT_CMU.h"
#include "MCIoT_Timer.h"

/************************************************************************************
 * @function 	CMU_SetUp
 * @params 		None
 * @brief 		Configures and starts necessary clocks.
 ************************************************************************************/
void CMU_SetUp(void)
{
	CMU_ClockEnable(cmuClock_CORELE, true); /* To enable the low frequency clock tree */

	if (e_letimer_energy_modes == ENERGY_MODE_EM3)
	{
		/* Enable necessary clocks */
		CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true); /* To enable the LFXO */
		CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO); /* To support energy modes EM0-EM2 */
		CMU_ClockEnable(cmuClock_LETIMER0, true); /* To enable the LFA clock tree to LETimer0 */

#ifdef ULFRCO_SELF_CALIBRATE

		/* Perform self calibration of ULFRCO */
		CMU_ClockEnable(cmuClock_TIMER0, true); /* To enable HFPER clock tree for Timer0 peripheral */
		CMU_ClockEnable(cmuClock_TIMER1, true); /* To enable HFPER clock tree for Timer1 peripheral */

		/* Routine to self calibrate ULFRCO */
		Calibrate_ULFRCO();

		CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true); /* To enable the LFXO */
		CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO); /* To support energy modes EM0-EM2 */

#endif

	}
	else
	{
		/* Enable necessary clocks */
		CMU_OscillatorEnable(cmuOsc_LFXO, true, true); /* To enable the LFXO */
		CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO); /* To support energy modes EM0-EM2 */
		CMU_ClockEnable(cmuClock_LETIMER0, true); /* To enable the LFA clock tree to LETimer0 */
	}

	CMU_ClockEnable(cmuClock_HFPER, true); 	/* To enable the clock tree for Timer peripherals */

#ifndef USE_ACTIVE_ALS
	CMU_ClockEnable(cmuClock_ACMP0, true); 	/* To enable clock to ACMP0 */
#endif

#ifdef ENABLE_ADC_MODULE
	CMU_ClockEnable(cmuClock_ADC0, true); 	/* To enable clock to ADC0 */
#endif

#ifdef USE_DMA_FOR_ADC
	CMU_ClockEnable(cmuClock_DMA, true); 	/* To enable clock to DMA */
#endif

	CMU_ClockEnable(cmuClock_GPIO, true);	/* To enable clock to GPIO */

#ifdef USE_ACTIVE_ALS
	CMU_ClockEnable(cmuClock_I2C1, true); 	/* To enable clock to I2C1 */
#endif

#ifdef ENABLE_LEUART_MODULE
	if (e_letimer_energy_modes == ENERGY_MODE_EM3)
	{
#ifdef USE_LFRCO_FOR_LEUART_IN_EM3
		CMU_OscillatorEnable(cmuOsc_LFXO, false, false); /* To enable the LFXO */
		CMU_OscillatorEnable(cmuOsc_LFRCO, true, true); /* To enable the LFXO */
		CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFRCO); /* To support energy modes EM0-EM2 */
#else
		CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO); /* To support energy modes EM0-EM2 */
#endif
	}
	else
	{
		CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO); /* To support energy modes EM0-EM2 */
	}

	CMU_ClockEnable(cmuClock_LEUART0, true);    /* Enable LEUART0 clock */
#endif

}

/************************************************************************************
 * @function 	Calibrate_ULFRCO
 * @params 		None
 * @brief 		Routine to self calibrate ULFRCO.
 ************************************************************************************/
void Calibrate_ULFRCO(void)
{
	uint32_t lfxo_count = 0;
	uint32_t ulfrco_count = 0;

	const LETIMER_Init_TypeDef letimer_calibration_init_params =
	{
	.enable	= false,            	/* Don't start counting when init completed. */
	.debugRun = false,         		/* Counter shall not keep running during debug halt. */
	.rtcComp0Enable = false,         /* Don't start counting on RTC COMP0 match. */
	.rtcComp1Enable = false,         /* Don't start counting on RTC COMP1 match. */
	.comp0Top = true,               	/* Load COMP0 register into CNT when counter underflows. COMP0 is used as TOP */
	.bufTop = false,                 /* Don't load COMP1 into COMP0 when REP0 reaches 0. */
	.out0Pol = 0,                    /* Idle value for output 0. */
	.out1Pol = 0,                    /* Idle value for output 1. */
	.ufoa0 = letimerUFOAPwm,         /* PWM output on output 0 */
	.ufoa1 = letimerUFOAPulse,       /* Pulse output on output 1*/
	.repMode = letimerRepeatOneshot /* Count once */
	};

	/* Timer0 init params */
	TIMER_Init_TypeDef timer0Init =
	{
	.enable = false,						/* Don't start counting when init completed. */
	.debugRun = false,            		/* Counter shall not keep running during debug halt. */
	.prescale = 0,						/* Use a prescale of 0 */
	.clkSel = timerClkSelHFPerClk, 		/* Using HFPER clock */
	.fallAction = timerInputActionNone,	/* Don't start/stop/reload counter on falling edge */
	.riseAction = timerInputActionNone,	/* Don't start/stop/reload counter on rising edge */
	.mode	= timerModeUp,					/* Count upwards */
	.dmaClrAct = false,					/* Don't clear DMA request on active */
	.quadModeX4 = false,					/* Don't use quadrature decode mode */
	.oneShot = false,						/* Not counting up just once */
	.sync	= false						/* Timer0 start not in sync with anything */
	};

	/* Timer0 init params */
	TIMER_Init_TypeDef timer1Init =
	{
	.enable = false,						/* Don't start counting when init completed. */
	.debugRun = false,            		/* Counter shall not keep running during debug halt. */
	.prescale = 0,						/* Use a prescale of 0 */
	.clkSel = timerClkSelCascade, 		/* Using HFPER clock */
	.fallAction = timerInputActionNone,	/* Don't start/stop/reload counter on falling edge */
	.riseAction = timerInputActionNone,	/* Don't start/stop/reload counter on rising edge */
	.mode	= timerModeUp,					/* Count upwards */
	.dmaClrAct = false,					/* Don't clear DMA request on active */
	.quadModeX4 = false,					/* Don't use quadrature decode mode */
	.oneShot = false,						/* Not counting up just once */
	.sync	= true						/* Timer0 start not in sync with anything */
	};

	/* Initialize LETIMER for ULFRCO calibration */
	LETIMER_Setup(LETIMER0, letimer_calibration_init_params);

	/* Initialize Timer peripherals */
	TIMER_Setup(TIMER0, timer0Init);
	TIMER_Setup(TIMER1, timer1Init);

	CMU_Obtain_Freq_Count(&ulfrco_count, ULFRCO_FREQUENCY, CALIBRATION_PERIOD);

	TIMER1->CNT = 0;
	TIMER0->CNT = 0;

	CMU_OscillatorEnable(cmuOsc_ULFRCO, false, false); /* To disable the ULFRCO */

	CMU_OscillatorEnable(cmuOsc_LFXO, true, true); /* To enable the LFXO */
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO); /* To support energy modes EM0-EM2 */
	CMU_ClockEnable(cmuClock_LETIMER0, true); /* To enable the LFA clock tree to LETimer0 */

	/* Obtain LFXO count */
	CMU_Obtain_Freq_Count(&lfxo_count, LFXO_FREQUENCY, CALIBRATION_PERIOD);

	osc_ratio = (float)lfxo_count/(float)ulfrco_count;

	CMU_OscillatorEnable(cmuOsc_LFXO, false, false); /* To disable the LFXO */

	CMU_ClockEnable(cmuClock_TIMER0, false); /* To disable HFPER clock tree for Timer0 peripheral */
	CMU_ClockEnable(cmuClock_TIMER1, false); /* To disable HFPER clock tree for Timer1 peripheral */

}

void CMU_Obtain_Freq_Count(uint32_t *count, uint32_t freq,
							uint32_t calibration_period)
{
	uint32_t sync_busy = 0;

	/* Obtain ULFRCO/LFXO count */
	LETIMER0->CNT = freq*calibration_period;

	/* Start LETIMER, TIMER0 and TIMER1 */
	LETIMER0->CMD |= LETIMER_CMD_START;
	while ((sync_busy = LETIMER0->SYNCBUSY & LETIMER_SYNCBUSY_CMD) == LETIMER_SYNCBUSY_CMD);

	TIMER1->CMD |= TIMER_CMD_START;
	TIMER0->CMD |= TIMER_CMD_START;

	while(LETIMER0->CNT != 0);

	LETIMER0->CMD |= LETIMER_CMD_STOP;

	TIMER1->CMD |= TIMER_CMD_STOP;
	TIMER0->CMD |= TIMER_CMD_STOP;

	*count = (TIMER1->CNT << 16) | TIMER0->CNT;
}

/************************************************************************************
 * @function 	check_and_comp_prescaler_if
 * @params 		[in] period - duration that has to be achieved (greater than 2 secs)
 * 				[in] osc_cnt - Oscillator count value
 * @brief 		Check if prescaler value needs to be computed and write the PRESCALER
 *				register in CMU to reduce the LFXO frequency.
 ************************************************************************************/
void check_and_comp_prescaler_if(float period, uint32_t osc_cnt)
{
	//uint32_t cmu_sync_busy;

	if (period > MAX_PERIOD_32kHZ)
	{
		cal_prescaler_value(period, osc_cnt);
	}
	else
	{
		letimer0_prescaler = 0;
		prescaled_two_power = 1;
	}

	CMU->LFAPRESC0 |= letimer0_prescaler << _CMU_LFAPRESC0_LETIMER0_SHIFT;

	//while ((cmu_sync_busy = CMU->SYNCBUSY & CMU_SYNCBUSY_LFAPRESC0) == CMU_SYNCBUSY_LFAPRESC0);
}

/************************************************************************************
 * @function 	cal_prescaler_value
 * @params 		[in] period - duration that has to be achieved (greater than 2 secs)
 * 				[in] osc_cnt - Oscillator count value
 * @brief 		Calculates the prescaler value that will divide the clock to achieve
 *				time duration greater than 2 seconds.
 * @credits		Slides from Professor Keith Graham
 ************************************************************************************/
void cal_prescaler_value(float period, uint32_t osc_cnt)
{
	uint32_t desired_period = 0;
	uint32_t temp = 0;

	desired_period = period * osc_cnt;
	temp = desired_period/1;
	prescaled_two_power = 1;

	while(temp > LETIMER0_MAX_COUNT)
	{
		letimer0_prescaler++;
		prescaled_two_power = prescaled_two_power * 2;
		temp = desired_period/prescaled_two_power;
	}
}
