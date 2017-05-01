/*****************************************************************************
 * @file 	MCIoT_GPIO.c
 * @brief 	This file describes the functions pertaining to GPIO setup.
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
#include "MCIoT_GPIO.h"

/************************************************************************************
 * @function 	GPIO_SetUp
 * @params 		None
 * @brief 		Configures LED GPIO's.
 ************************************************************************************/
void GPIO_SetUp(void)
{
	/* Set GPIO PE2 and PE3 as pushpull */
	GPIO_PinModeSet(LED0_1_GPIO_PORT, LED0_GPIO_PIN, gpioModePushPullDrive, 0); /* Configure LED0 pin as digital output (push-pull) */
	GPIO_PinModeSet(LED0_1_GPIO_PORT, LED1_GPIO_PIN, gpioModePushPullDrive, 0); /* Configure LED1 pin as digital output (push-pull) */

	GPIO_DriveModeSet(LED0_1_GPIO_PORT, gpioDriveModeLowest); /* Set DRIVEMODE to lowest setting (0.5 mA) for all LEDs configured with alternate drive strength */
}

/************************************************************************************
 * @function 	LED_On
 * @params 		[in] port - The GPIO port to access
 * 				[in] pin  - The pin to set
 * @brief 		Routine to turn on the LED.
 ************************************************************************************/
void LED_On(GPIO_Port_TypeDef port, unsigned int pin)
{
	/* Turning on LED */
	GPIO_PinOutSet(port, pin);
}

/************************************************************************************
 * @function 	LED_Off
 * @params 		[in] port - The GPIO port to access
 * 				[in] pin  - The pin to clear
 * @brief 		Routine to turn on the LED.
 ************************************************************************************/
void LED_Off(GPIO_Port_TypeDef port, unsigned int pin)
{
	/* Turning off LED */
	GPIO_PinOutClear(port, pin);
}

/************************************************************************************
 * @function 	I2C_GPIO_Enable
 * @params 		None
 * @brief 		Routine to turn on the SDA and SCL GPIO pins.
 ************************************************************************************/
void I2C_GPIO_Enable(void)
{
	uint32_t i = 0;

	/* Configure SCL pin as open-drain output */
	GPIO_PinModeSet(I2C_GPIO_PORT, I2C_SCL_PIN, gpioModeWiredAnd, 1);

	/* Configure SDA pin as open-drain output */
	GPIO_PinModeSet(I2C_GPIO_PORT, I2C_SDA_PIN, gpioModeWiredAnd, 1);

	/* Toggle I2C SCL 9 times to reset any I2C slave that may require it */
	for (i = 0; i < 9; i++)
	{
		GPIO_PinOutClear(I2C_GPIO_PORT, I2C_SCL_PIN);
		GPIO_PinOutSet(I2C_GPIO_PORT, I2C_SCL_PIN);
	}

	/* The I2C bus and peripherals may be out of sync upon I2C bus setup, so reset the I2C bus */
	if (I2C1->STATE & I2C_STATE_BUSY)
	{
		I2C1->CMD = I2C_CMD_ABORT;
	}
}
/************************************************************************************
 * @function 	I2C_GPIO_Disable
 * @params 		None
 * @brief 		Routine to turn off the SDA and SCL GPIO pins.
 ************************************************************************************/
void I2C_GPIO_Disable(void)
{
	/* Configure SCL pin as open-drain output */
	GPIO_PinModeSet(I2C_GPIO_PORT, I2C_SCL_PIN, gpioModeDisabled, 0);

	/* Configure SDA pin as open-drain output */
	GPIO_PinModeSet(I2C_GPIO_PORT, I2C_SDA_PIN, gpioModeDisabled, 0);
}
