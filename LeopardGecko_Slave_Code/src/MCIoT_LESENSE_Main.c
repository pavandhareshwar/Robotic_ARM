/**************************************************************************//**
 * @file MCIoT_LESENSE_Main.c
 * @brief LESENSE CAPACITIVE TOUCH code example
 * @author Energy Micro AS
 * @version 2.03
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
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 4. The source and compiled code may only be used on Energy Micro "EFM32"
 *    microcontrollers and "EFR4" radios.
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
 *****************************************************************************/
#include "em_emu.h"
#include "MCIoT_LESENSE_Main.h"
#include "MCIoT_LESENSE_LETouch.h"
#include "MCIoT_GPIO.h"
#include "MCIoT_main.h"

void LESENSE_SetUp(void)
{
	/* Bitmask for the currently touched channels */
	uint16_t channels_touched = 0;

	/* Four pins PC8, PC9, PC10 and PC11 corresponding to the four slider pads. */

	/* Four pins active, PC8, PC9, PC10 and PC11. A value of 0.0 indicates inactive channel. */
	//float sensitivity[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0};

	/* Sensitivity array with only one slider pad active, pin: PC8. */
	float sensitivity[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0};

	bool new_channel_event = false;
	bool awaiting_next_touch_event = false;
	uint8_t is_the_channel_to_be_checked[NUM_LESENSE_CHANNELS] = {0};
	uint8_t is_the_channel_to_be_checked_saved[NUM_LESENSE_CHANNELS] = {0};
	uint8_t valid_touch_count = 0;

	uint8_t num_channels_enabled = 0;

	CMU_ClockEnable(cmuClock_LETIMER0, false); /* To disable the clock for LETIMER */

	for (int i = 0; i < NUM_LESENSE_CHANNELS; i++)
	{
		if (sensitivity[i] != 0)
		{
			is_the_channel_to_be_checked[i] = 1;
			num_channels_enabled++;
		}
	}

	memcpy(is_the_channel_to_be_checked_saved, is_the_channel_to_be_checked, NUM_LESENSE_CHANNELS);

	uint8_t num_channels_enabled_saved = num_channels_enabled;

	/* Init GPIO for LED, turn LED off */
	//CMU_ClockEnable(cmuClock_GPIO, true);
	//GPIO_PinModeSet(LED_PORT, LED_PIN, gpioModePushPull, 0);

	/* Init Capacitive touch for channels configured in sensitivity array */
	LETOUCH_Init(sensitivity);

	/* If any channels are touched while starting, the calibration will not be correct. */
	/* Wait while channels are touched to be sure we continue in a calibrated state. */
	while(LETOUCH_GetChannelsTouched() != 0);

	/* Enter infinite loop. */
	while (1)
	{
		int i = 8;
		/* Get channels that are pressed, result is or'ed together */
		channels_touched = LETOUCH_GetChannelsTouched();

		if (num_channels_enabled > 0)
		{
			if (is_the_channel_to_be_checked[i++] != 0)
			{
				if (channels_touched == CHANNEL_EIGHT_TOUCH_VAL)
				{
					num_channels_enabled--;
					if (false == awaiting_next_touch_event)
					{
						awaiting_next_touch_event = true;
						valid_touch_count++;
					}
					is_the_channel_to_be_checked[i-1] = 0;
				}
			}
			else
			{
				if (channels_touched == CHANNEL_EIGHT_TOUCH_VAL)
				{
					num_channels_enabled--;
					if (true == awaiting_next_touch_event)
					{
						awaiting_next_touch_event = false;
						valid_touch_count--;
					}
				}
			}

			for (int j = 0; j < 10000; j++);


			if (is_the_channel_to_be_checked[i++] != 0)
			{
				if (channels_touched == CHANNEL_NINE_TOUCH_VAL)
				{
					num_channels_enabled--;
					if (false == awaiting_next_touch_event)
					{
						awaiting_next_touch_event = true;
						valid_touch_count++;
					}
					is_the_channel_to_be_checked[i-1] = 0;
				}
			}
			else
			{
				if (channels_touched == CHANNEL_NINE_TOUCH_VAL)
				{
					num_channels_enabled--;
					if (true == awaiting_next_touch_event)
					{
						awaiting_next_touch_event = false;
						valid_touch_count--;
					}
				}
			}

			for (int j = 0; j < 10000; j++);

			if (is_the_channel_to_be_checked[i++] != 0)
			{
				if (channels_touched == CHANNEL_TEN_TOUCH_VAL)
				{
					num_channels_enabled--;
					if (false == awaiting_next_touch_event)
					{
						awaiting_next_touch_event = true;
						valid_touch_count++;
					}
					is_the_channel_to_be_checked[i-1] = 0;
				}
			}
			else
			{
				if (channels_touched == CHANNEL_TEN_TOUCH_VAL)
				{
					num_channels_enabled--;
					if (true == awaiting_next_touch_event)
					{
						awaiting_next_touch_event = false;
						valid_touch_count--;
					}
				}
			}

			for (int j = 0; j < 10000; j++);

			if (is_the_channel_to_be_checked[i++] != 0)
			{
				if (channels_touched == CHANNEL_ELEVEN_TOUCH_VAL)
				{
					num_channels_enabled--;
					if (false == awaiting_next_touch_event)
					{
						awaiting_next_touch_event = true;
						valid_touch_count++;
					}
					is_the_channel_to_be_checked[i-1] = 0;
				}
			}
			else
			{
				if (channels_touched == CHANNEL_ELEVEN_TOUCH_VAL)
				{
					num_channels_enabled--;
					if (true == awaiting_next_touch_event)
					{
						awaiting_next_touch_event = false;
						valid_touch_count--;
					}
				}
			}

			for (int j = 0; j < 10000; j++);

		}
		else
		{
			num_channels_enabled = num_channels_enabled_saved;
			memcpy(is_the_channel_to_be_checked, is_the_channel_to_be_checked_saved, NUM_LESENSE_CHANNELS);
		}

		if (valid_touch_count == num_channels_enabled_saved)
		{
			new_channel_event = true;
		}


		/* Check if any channels are in the touched state. */

		if (new_channel_event == true)
		{
			/* Turn on LED */
			//GPIO_PinModeSet(LED_PORT, LED_PIN, gpioModePushPull, 1);
			LED_On(LED0_1_GPIO_PORT, LED0_GPIO_PIN);
			new_channel_event = false;

			is_lesense_auth_done = true;
			break;

		}
		else
		{
			/* Turn off LED */
			//GPIO_PinModeSet(LED_PORT, LED_PIN, gpioModePushPull, 0);
			LED_Off(LED0_1_GPIO_PORT, LED0_GPIO_PIN);
		}

		/* Enter EM2 */
		//EMU_EnterEM2(true);
	}
}
