/*****************************************************************************
 * @file 	MCIoT_ADC.c
 * @brief 	This file describes the functions pertaining to ADC setup and
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
#include <string.h>
#include "MCIoT_main.h"
#include "MCIoT_ADC.h"
#include "MCIoT_GPIO.h"
#include "MCIoT_DMA.h"
#include "MCIoT_Sleep.h"
#include "MCIoT_CMU.h"
#include "MCIoT_LEUART.h"

uint8_t sampBuff_max = 4;

/************************************************************************************
 * @function 	ADC_setup
 * @params 		None
 * @brief 		Configures the ADC.
 ************************************************************************************/
void ADC_SetUp(void)
{
	uint8_t adc_prescale_val = 0;
	uint8_t	adc_timebase_val = 0;
	sampBuff_idx = 0;

	adc_timebase_val = ADC_TimebaseCalc(CMU_ClockFreqGet(cmuClock_HFPER));
	adc_prescale_val = ADC_PrescaleCalc(ADC_CLOCK_FREQ_REQD, 0); 	/* Corresponding to acquisition time of 1 ADC clock cycle */

	ADC_Init_TypeDef adc_Init =
	{
		.ovsRateSel 	= ADC_OVERSAMPLING_RATE_SEL,	/* 2x oversampling (if enabled). */
		.lpfMode 		= ADC_LPF_MODE,					/* No input filter selected. */
		.warmUpMode 	= ADC_WARMUPMODE,				/* ADC shutdown after each conversion. */
		.timebase		= (adc_timebase_val+1),			/* The timebase set to number of HFPER clock cycles that corresponds
														   to atleast 1 us. (adc_timebase_val+1) because timebase should
														   be 14 for HFPERCLK = 14MHz and adc_timebase_val was coming 13 */
		.prescale		= adc_prescale_val,				/* Use calculated prescale value */
		.tailgate		= false							/* Do not use tailgate. */
	};

	/* Using single conversion mode since ADC has to convert just one input -
	 * internal temperature sensor data */
	ADC_InitSingle_TypeDef adc_InitSingle =
	{
		.prsSel 		= ADC_PRS_CHANNEL,              /* PRS ch0 (if enabled). */
		.acqTime		= ADC_ACQ_TIME_ADC_CLK_CYCLES, 	/* 1 ADC_CLK cycle acquisition time. */
		.reference		= ADC_REFERENCE,                /* 1.25V internal reference. */
		.resolution		= ADC_RESOLUTION,               /* 12 bit resolution. */
		.input			= ADC_INPUT_CHANNEL,           	/* Channel 1 input selected as initial channel. */
		.diff			= false,                     	/* Single ended input. */
		.prsEnable		= false,                     	/* PRS disabled. */
		.leftAdjust		= false,                     	/* Right adjust. */
		.rep			= false                      	/* Deactivate conversion after one scan sequence. */
	};

	ADC_Init(ADC0, &adc_Init);

	ADC0->SINGLECTRL |= ADC_SINGLECTRL_REP;

	ADC_InitSingle(ADC0, &adc_InitSingle);

#ifndef USE_DMA_FOR_ADC

	adc_Samples_in_RAM = 0;
	p_ADCDataRAMBuffer = ADCDataRAMBuffer;
#endif

	ADC_Interrupt_Enable();

	NVIC_EnableIRQ(ADC0_IRQn);

}

/************************************************************************************
 * @function 	ADC_Interrupt_Enable
 * @params 		None
 * @brief 		Enables the necessary interrupts for ADC0.
 ************************************************************************************/
void ADC_Interrupt_Enable(void)
{
	/* Clearing all the interrupts that may have been set-up inadvertently */
	ADC0->IFC |= (ADC_IF_SCAN | ADC_IF_SINGLE);

	/* Enable Single Conversion Complete Interrupt  */
	ADC_IntEnable(ADC0, ADC_IF_SINGLE);
}

/************************************************************************************
 * @function 	ADC0_IRQHandler
 * @params 		None
 * @brief 		Interrupt Service Routine for ADC0.
 ************************************************************************************/
void ADC0_IRQHandler(void)
{
	uint32_t adc_val = 0;
	float average = 0.0;
#ifdef USE_CIRC_BUFFER_FOR_LEUART
	int retVal = -1;
#endif

	//INT_Disable();
	CORE_DECLARE_IRQ_STATE;
	CORE_ENTER_ATOMIC();

	uint8_t flexValue;

	/* Single Conversion Complete Interrupt */
	if ((ADC0->IF & ADC_IF_SINGLE) == ADC_IF_SINGLE)
	{
		ADC_IntClear(ADC0, ADC_IF_SINGLE);

		//Taking value from the sensor

		sampleBuffer[sampBuff_idx] = ADC0->SINGLEDATA;
		sampleBuffer[sampBuff_idx] = (sampleBuffer[sampBuff_idx]/4096)*(1.25);
		flexValue = (sampleBuffer[sampBuff_idx])*10;
		DataValue = (sampBuff_idx<<6 | flexValue);
		WriteDataToCircBuff(&leuart_circ_buff, DataValue);

		//Configuring for next sensor
		sampBuff_idx++;
		if(sampBuff_idx > sampBuff_max-1)
		{
			sampBuff_idx = 0;
		}

		switch (sampBuff_idx)
		{
		case 0:
			adc_InitSingle.input = flexSensorCh1;
			break;
		case 1:
			adc_InitSingle.input = flexSensorCh2;
			break;
		case 2:
			adc_InitSingle.input = flexSensorCh3;
			break;
		case 3:
			adc_InitSingle.input = flexSensorCh4;
			break;
		default:
			break;
		}
		ADC_InitSingle(ADC0, &adc_InitSingle);

	}
	CMU_ClockEnable(cmuClock_ADC0, false);

	ADC0->CMD |= ADC_CMD_SINGLESTOP;

	unblockSleepMode(1);

#if 0
	if (flexValue > Flex_High_Val)
	{
		LED_On(LED0_1_GPIO_PORT,LED1_GPIO_PIN);
		LED_Off(LED0_1_GPIO_PORT,LED0_GPIO_PIN);
	}
	else if (flexValue < Flex_Low_Val)
	{
		LED_On(LED0_1_GPIO_PORT,LED0_GPIO_PIN);
		LED_Off(LED0_1_GPIO_PORT,LED1_GPIO_PIN);
	}
	else
	{
		LED_Off(LED0_1_GPIO_PORT,LED1_GPIO_PIN);
		LED_Off(LED0_1_GPIO_PORT,LED0_GPIO_PIN);
	}
#endif

	if(leuart_nvic_is_enabled== 0)
	{
		NVIC_EnableIRQ(LEUART0_IRQn);
		leuart_nvic_is_enabled = 1;
	}

#if 0
		/* Clearing the source of interrupt: ADC0 Single Conversion Complete Interrupt */
		ADC_IntClear(ADC0, ADC_IF_SINGLE);

		adc_val = ADC0->SINGLEDATA;

		if (adc_Samples_in_RAM < ADC_SAMPLES)
		{
			memcpy(p_ADCDataRAMBuffer, &adc_val, ADC_SAMPLE_TRANSFER_SIZE*sizeof(uint16_t));
			p_ADCDataRAMBuffer += ADC_SAMPLE_TRANSFER_SIZE;
			adc_Samples_in_RAM += ADC_SAMPLE_TRANSFER_SIZE;
		}
		else
		{
			/* We now have 750 ADC samples in the RAM buffer, so reset the array pointer
			 * and the count to intial values */

			p_ADCDataRAMBuffer = ADCDataRAMBuffer;
			adc_Samples_in_RAM = 0;

			/* Turn off ADC */
			ADC0->CMD |= ADC_CMD_SINGLESTOP;

			/* Disable clock to ADC0 */
			CMU_ClockEnable(cmuClock_ADC0, false); /* To enable clock to ADC0 */

			unblockSleepMode(ADC_EM);

			average = compute_adc_data_average();

#ifdef USE_INT
			INT_Disable();
#else
			CORE_DECLARE_IRQ_STATE;
			CORE_ENTER_ATOMIC();
#endif

			/* To ensure the following circular buffer writes are atomic */
			leuart_temp_sign_data = (average > 0) ? 1 : 0;
			leuart_temp_sign_data |= (LEUART_TEMP_DATA_IDENTIFIER << 7);

			if (false == state_is_em1_for_leuart_tx)
			{
				blockSleepMode(LEUART_EM);
				state_is_em1_for_leuart_tx = true;
			}

#ifdef USE_CIRC_BUFFER_FOR_LEUART
			retVal = WriteDataToCircBuff(&leuart_circ_buff, leuart_temp_sign_data);
			if (0 == retVal)
			{
				if (false == leuart_nvic_is_enabled)
				{
					/* Enable the NVIC for LEUART0 so that the IRQ handler can be
					 * triggered to transmit the data */
					NVIC_EnableIRQ(LEUART0_IRQn);
					leuart_nvic_is_enabled = true;
				}

				leuart_temp_mantissa_data = (average * 10) / 10;
				retVal = WriteDataToCircBuff(&leuart_circ_buff, leuart_temp_mantissa_data);

				if (0 == retVal)
				{
					leuart_temp_exponent_data = (int)(average * 10) % 10;
					WriteDataToCircBuff(&leuart_circ_buff, leuart_temp_exponent_data);
				}
			}
#else
			*puart_buffer = leuart_temp_sign_data;
			leuart_temp_mantissa_data = (average * 10) / 10;
			*(puart_buffer+1) = leuart_temp_mantissa_data;
			leuart_temp_exponent_data = (int)(average * 10) % 10;
			*(puart_buffer+2) = leuart_temp_exponent_data;

			puart_buffer += 2;
#endif

#ifdef USE_INT
			INT_Enable();
#else
			CORE_EXIT_ATOMIC();
#endif

			NVIC_EnableIRQ(LEUART0_IRQn);

			if ((TEMPERATURE_LOWER_LIMIT <= average)  && (average <= TEMPERATURE_UPPER_LIMIT))
			{
				/* Turning off LED1 */
				LED_Off(LED0_1_GPIO_PORT, LED1_GPIO_PIN);
			}
			else
			{
				/* Turning on LED1 */
				LED_On(LED0_1_GPIO_PORT, LED1_GPIO_PIN);
			}
		}
#endif
	//INT_Enable();
	CORE_EXIT_ATOMIC();
}

/************************************************************************************
 * @function 	converttoCelsius
 * @params 		[in] adcSample - (int32_t) ADC sample
 * 				[out] temp - (float) temperature in celsius
 * @brief 		Routine to convert ADC sample to celsius.
 * @credits 	License (C) Copyright 2015 Silicon Labs, http://www.silabs.com/
 * @reference	Slides from Prof. Keith Graham
 ************************************************************************************/
float converttoCelsius(int32_t adcSample)
{
	float temp;

	/* Factory calibration temperature from device information page */
	float cal_temp_0 = (float)((DEVINFO->CAL & _DEVINFO_CAL_TEMP_MASK) >> _DEVINFO_CAL_TEMP_SHIFT);
	float cal_value_0 = (float)((DEVINFO->ADC0CAL2 & _DEVINFO_ADC0CAL2_TEMP1V25_MASK) >> _DEVINFO_ADC0CAL2_TEMP1V25_SHIFT);

	/* Temperature Gradient (from datasheet) */
	float t_grad = TEMPERATURE_GRADIENT;

	temp = (cal_temp_0 - ((cal_value_0 - adcSample) / t_grad));

	return temp;
}


/************************************************************************************
 * @function 	compute_adc_data_average
 * @params 		[in] - None
 * 				[out] average - (float) average of adc samples
 * @brief 		Routine to convert average of ADC samples.
 ************************************************************************************/
float compute_adc_data_average(void)
{
	uint32_t i = 0, sum = 0;
	float average = 0.0;

	/* Average the 750 conversion results */
	for (i = 0; i < ADC_SAMPLES; i++)
	{
		sum += ADCDataRAMBuffer[i];
	}

	average = sum/ADC_SAMPLES;
	average = converttoCelsius(average);

	return average;
}
