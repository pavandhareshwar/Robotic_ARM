/*****************************************************************************
 * @file 	MCIoT_LEUART.c
 * @brief 	This file describes the functions pertaining to LEUART setup and
  *   		interrupt handler.
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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "MCIoT_main.h"
#include "MCIoT_LEUART.h"
#include "MCIoT_GPIO.h"
#include "MCIoT_Sleep.h"

/************************************************************************************
 * @function 	LEUART_setup
 * @params 		None
 * @brief 		Configures the LEUART.
 ************************************************************************************/
void LEUART_SetUp(void)
{
    const LEUART_Init_TypeDef leuart0_init =
    {
    	.enable     = LEUART_ENABLE_SEL,
		.refFreq    = LEUART_REF_FREQ,
		.baudrate   = LEUART_BAUD_RATE,
		.databits   = LEUART_DATA_BITS,
		.parity     = LEUART_PARITY,
		.stopbits   = LEUART_STOP_BITS
    };

    /* Resetting and initializing LEUART0 */
    LEUART_Reset(LEUART0);
    LEUART_Init(LEUART0, &leuart0_init);

    /* Enable LEUART0 Interrupts */
    LEUART_Interrupt_Enable(LEUART0);

#ifdef ENABLE_RX_FOR_LOOPBACK

    LEUART0->CMD |= LEUART_CMD_TXEN | LEUART_CMD_RXEN;

    /* LoopBack Enable */
    LEUART0->CTRL |= LEUART_CTRL_LOOPBK;

    LEUART0->ROUTE = (LEUART_ROUTE_TXPEN | LEUART_ROUTE_RXPEN
    		| LEUART_ROUTE_LOCATION_LOC0);
#else
    LEUART0->ROUTE = (LEUART_ROUTE_TXPEN | LEUART_ROUTE_LOCATION_LOC0);
#endif

    /* Enable GPIO for LEUART0. TX is on D4 */
    GPIO_PinModeSet(LEUART_GPIO_PORT, LEUART_GPIO_TX_PIN, gpioModePushPull, 0);

#ifdef ENABLE_RX_FOR_LOOPBACK
    /* Enable GPIO for LEUART0. RX is on D5 */
    GPIO_PinModeSet(LEUART_GPIO_PORT, LEUART_GPIO_RX_PIN, gpioModeInput, 1);
#endif

    /* Initialize leuart data */
    leuart_led_data = 0;
    leuart_temp_mantissa_data = 0;
    leuart_temp_exponent_data = 0;
    leuart_temp_sign_data = 0;

    state_is_em1_for_leuart_tx = false;
    led_data_available = false;
    temp_data_available = false;
    leuart_nvic_is_enabled = false;

#ifdef USE_CIRC_BUFFER_FOR_LEUART
    /* LEUART circular buffer params init */
    leuart_circ_buff.circ_buff = (float *)malloc(CIRC_BUFF_MAX_LENGTH*sizeof(uint8_t));
    leuart_circ_buff.write = 0;
    leuart_circ_buff.read = 0;
    leuart_circ_buff.maxLength = CIRC_BUFF_MAX_LENGTH;
#else
    puart_buffer = leuart_buffer;
    read_count = 0;
    temp_data_count = 0;
#endif
}

/************************************************************************************
 * @function 	LEUART_Interrupt_Enable
 * @params 		None
 * @brief 		Enables the necessary interrupts for LEUART0.
 ************************************************************************************/
void LEUART_Interrupt_Enable(LEUART_TypeDef *LEUART)
{
	uint32_t leuart_if = LEUART->IF;

	/* Clear all the interrupts that may have been set-up inadvertently */
	LEUART->IFC |= leuart_if;

	/* Enable TXBL interrupt */
	LEUART_IntEnable(LEUART, LEUART_IF_TXBL);
}

/************************************************************************************
 * @function 	LEUART0_IRQHandler
 * @params 		None
 * @brief 		Interrupt Service Routine for LEUART0.
 ************************************************************************************/
void LEUART0_IRQHandler(void)
{
#ifdef USE_CIRC_BUFFER_FOR_LEUART
	float 		circ_buff_read_val = 0;
#endif
	static bool temp_data_read_in_progress = false;
	static bool is_next_temp_data_mantissa = false;
	uint8_t		flex_sensor_value = 0;
	uint8_t 	led_data = 0;
	uint8_t 	temp_sign_data = 0;
	uint8_t 	temp_mantissa_data = 0;
	uint8_t 	temp_exponent_data = 0;

#ifdef ENABLE_RX_FOR_LOOPBACK
	uint32_t 	rx_data = 0;
#endif

#ifdef USE_INT
	INT_Disable();
#else
	CORE_DECLARE_IRQ_STATE;
	CORE_ENTER_ATOMIC();
#endif

	/* TXBL Interrupt */
	if ((LEUART0->IF & LEUART_IF_TXBL) == LEUART_IF_TXBL)
	{
		LEUART_IntClear(LEUART0, LEUART_IF_TXBL);

#ifdef USE_CIRC_BUFFER_FOR_LEUART
		if (-1 != ReadDataFromCircBuff(&leuart_circ_buff, &circ_buff_read_val))
		{
			flex_sensor_value = circ_buff_read_val;

			LEUART0->TXDATA = flex_sensor_value;

			while((LEUART0->IF & LEUART_IF_TXC) == 0);

			if (e_letimer_energy_modes == ENERGY_MODE_EM2)
				for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM2; i++);
			else
				for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM3; i++);
		}

		/* Send LED data */
		/* Circular Buffer Read */
#if 0

		if (-1 != ReadDataFromCircBuff(&leuart_circ_buff, &circ_buff_read_val))
		{
			if (temp_data_read_in_progress == false)
			{
				if ((circ_buff_read_val >> 7) == LEUART_LED_DATA_IDENTIFIER)
				{
					/* LED Data */
					led_data = circ_buff_read_val;

					LEUART0->TXDATA = led_data;

					while((LEUART0->IF & LEUART_IF_TXC) == 0);

#ifdef ENABLE_RX_FOR_LOOPBACK
					rx_data = LEUART0->RXDATA;
#endif
					if (e_letimer_energy_modes == ENERGY_MODE_EM2)
						for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM2; i++);
					else
						for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM3; i++);

				}
				else if ((circ_buff_read_val >> 7) == LEUART_TEMP_DATA_IDENTIFIER)
				{
					temp_data_read_in_progress = true;
					temp_sign_data = circ_buff_read_val;

					LEUART0->TXDATA = temp_sign_data;

					while((LEUART0->IF & LEUART_IF_TXC) == 0);
					is_next_temp_data_mantissa = true;

#ifdef ENABLE_RX_FOR_LOOPBACK
					rx_data = LEUART0->RXDATA;
#endif
					if (e_letimer_energy_modes == ENERGY_MODE_EM2)
						for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM2; i++);
					else
						for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM3; i++);
				}
				else
				{
					/* Invalid Data Identifier */
				}
			}
			else
			{
				if (true == is_next_temp_data_mantissa)
				{
					temp_mantissa_data = circ_buff_read_val;

					LEUART0->TXDATA = temp_mantissa_data;

					while((LEUART0->IF & LEUART_IF_TXC) == 0);

					is_next_temp_data_mantissa = false;
#ifdef ENABLE_RX_FOR_LOOPBACK
					rx_data = LEUART0->RXDATA;

#endif
					if (e_letimer_energy_modes == ENERGY_MODE_EM2)
						for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM2; i++);
					else
						for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM3; i++);
				}
				else
				{
					temp_exponent_data = circ_buff_read_val;

					LEUART0->TXDATA = temp_exponent_data;

					while((LEUART0->IF & LEUART_IF_TXC) == 0);

#ifdef ENABLE_RX_FOR_LOOPBACK
					rx_data = LEUART0->RXDATA;
#endif
					if (e_letimer_energy_modes == ENERGY_MODE_EM2)
						for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM2; i++);
					else
						for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM3; i++);

					temp_data_read_in_progress = false;
				}
			}

		}
		else
		{
			if (false == led_data_available)
			{
				led_data = ((LEUART_LED_DATA_IDENTIFIER << 7) | 7);

				LEUART0->TXDATA = led_data;

				while((LEUART0->IF & LEUART_IF_TXC) == 0);

#ifdef ENABLE_RX_FOR_LOOPBACK
				rx_data = LEUART0->RXDATA;
#endif
				if (e_letimer_energy_modes == ENERGY_MODE_EM2)
					for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM2; i++);
				else
					for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM3; i++);
			}

			unblockSleepMode(LEUART_EM);
			state_is_em1_for_leuart_tx = false;
#endif
			NVIC_DisableIRQ(LEUART0_IRQn);
			leuart_nvic_is_enabled = false;
		}
#else
		if (read_count != LEUART_BUFF_LENGTH)
		{
			if (temp_data_read_in_progress == false)
			{
				if ((leuart_buffer[0] >> 7) == LEUART_LED_DATA_IDENTIFIER)
				{
					/* LED Data */
					led_data = leuart_buffer[read_count];

					LEUART0->TXDATA = led_data;

					while((LEUART0->IF & LEUART_IF_TXC) == 0);

#ifdef ENABLE_RX_FOR_LOOPBACK
					rx_data = LEUART0->RXDATA;
#endif
					if (e_letimer_energy_modes == ENERGY_MODE_EM2)
						for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM2; i++);
					else
						for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM3; i++);

				}
				else if ((leuart_buffer[0] >> 7) == LEUART_TEMP_DATA_IDENTIFIER)
				{
					temp_data_read_in_progress = true;
					temp_sign_data = leuart_buffer[temp_data_count++];

					LEUART0->TXDATA = temp_sign_data;

					while((LEUART0->IF & LEUART_IF_TXC) == 0);
					is_next_temp_data_mantissa = true;

#ifdef ENABLE_RX_FOR_LOOPBACK
					rx_data = LEUART0->RXDATA;
#endif
					if (e_letimer_energy_modes == ENERGY_MODE_EM2)
						for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM2; i++);
					else
						for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM3; i++);
				}
				else
				{
					/* Invalid Data Identifier */
				}
			}
			else
			{
				if (true == is_next_temp_data_mantissa)
				{
					temp_mantissa_data = leuart_buffer[temp_data_count++];

					LEUART0->TXDATA = temp_mantissa_data;

					while((LEUART0->IF & LEUART_IF_TXC) == 0);

					is_next_temp_data_mantissa = false;
#ifdef ENABLE_RX_FOR_LOOPBACK
					rx_data = LEUART0->RXDATA;

#endif
					if (e_letimer_energy_modes == ENERGY_MODE_EM2)
						for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM2; i++);
					else
						for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM3; i++);
				}
				else
				{
					temp_exponent_data = leuart_buffer[temp_data_count++];

					LEUART0->TXDATA = temp_exponent_data;

					while((LEUART0->IF & LEUART_IF_TXC) == 0);

#ifdef ENABLE_RX_FOR_LOOPBACK
					rx_data = LEUART0->RXDATA;
#endif
					if (e_letimer_energy_modes == ENERGY_MODE_EM2)
						for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM2; i++);
					else
						for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM3; i++);

					temp_data_read_in_progress = false;
				}
			}

		}
		else
		{
			if (false == led_data_available)
			{
				led_data = ((LEUART_LED_DATA_IDENTIFIER << 7) | 7);

				LEUART0->TXDATA = led_data;

				while((LEUART0->IF & LEUART_IF_TXC) == 0);

#ifdef ENABLE_RX_FOR_LOOPBACK
				rx_data = LEUART0->RXDATA;
#endif
				if (e_letimer_energy_modes == ENERGY_MODE_EM2)
					for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM2; i++);
				else
					for (int i = 0; i < LEUART_POST_TX_WAIT_TIME_EM3; i++);
			}

			puart_buffer = leuart_buffer;
			read_count = 0;
			temp_data_count = 0;

			unblockSleepMode(LEUART_EM);
			state_is_em1_for_leuart_tx = false;

			NVIC_DisableIRQ(LEUART0_IRQn);

			leuart_nvic_is_enabled = false;
		}
		read_count++;
	}
#endif


#ifdef USE_INT
	INT_Enable();
#else
	CORE_EXIT_ATOMIC();
#endif
}

#ifdef USE_CIRC_BUFFER_FOR_LEUART
/************************************************************************************
 * @function 	WriteDataToCircBuff
 * @params
 * 				leuart_circ_buff 	- pointer to circular buffer structure
 * 				data				- data to be pushed to circular buffer
 *
 * @brief 		Function that describes how data is pushed to circular buffer.
 ************************************************************************************/
int WriteDataToCircBuff(LEUART_CIRC_BUFFER *leuart_circ_buff, float data)
{
	int retVal = -1;
	int next_write_loc = leuart_circ_buff->write + 1;
	if (next_write_loc >= leuart_circ_buff->maxLength)
		next_write_loc = 0;

	if (next_write_loc == leuart_circ_buff->read)
	{
		/* Circular buffer is full */
		retVal = -1;
		return retVal;
	}

	*(leuart_circ_buff->circ_buff + leuart_circ_buff->write) = data;
	leuart_circ_buff->write = next_write_loc;
	retVal = 0;

	return retVal;
}

/************************************************************************************
 * @function 	ReadDataFromCircBuff
 * @params
 * 				leuart_circ_buff 	- pointer to circular buffer structure
 * 				data				- data will be retrieved in this parameter
 * @brief 		Function that describes how data is retrieved from circular buffer.
 ************************************************************************************/
int ReadDataFromCircBuff(LEUART_CIRC_BUFFER *leuart_circ_buff, float *data)
{
	int retVal = -1;

	if (leuart_circ_buff->read == leuart_circ_buff->write)
	{
		/* Circular buffer is empty */
		retVal = -1;
		return retVal;
	}

	*data = *(leuart_circ_buff->circ_buff + leuart_circ_buff->read);

	int next_read_loc = leuart_circ_buff->read + 1;
	if (next_read_loc >= leuart_circ_buff->maxLength)
		next_read_loc = 0;

	leuart_circ_buff->read = next_read_loc;

	retVal = 0;
	return retVal;
}
#endif
