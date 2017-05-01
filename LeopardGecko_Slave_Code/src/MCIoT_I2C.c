/*****************************************************************************
 * @file 	MCIoT_I2C.c
 * @brief 	This file describes the functions pertaining to I2C setup and
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
#include "MCIoT_GPIO.h"
#include "MCIoT_I2C.h"
#include "MCIoT_Sleep.h"
#include "MCIoT_LEUART.h"

/************************************************************************************
 * @function 	I2C_SetUp
 * @params 		None
 * @brief 		Routine to setup I2C.
 ************************************************************************************/
void I2C_SetUp(void)
{
#if 0
	uint32_t i = 0;
#endif

	uint32_t i2c_flags = 0;

	const I2C_Init_TypeDef i2c1_init =
	{
		.enable		= I2C_ENABLE,
		.master		= I2C_MASTER_MODE,
		.refFreq	= I2C_REF_CLOCK,
		.freq		= I2C_BUS_FREQUENCY,
		.clhr		= I2C_CLOCK_RATE
	};

	/* Route the I2C pins to I2C peripherals */
	I2C1->ROUTE |= (I2C_ROUTE_LOCATION_LOC0 | I2C_ROUTE_SCLPEN | I2C_ROUTE_SDAPEN);

	/* I2C1 init */
	I2C_Init(I2C1, &i2c1_init);

	i2c_als_state = ALS_STATE_DARK;

	/* The I2C bus and peripherals may be out of sync upon I2C bus setup, so reset the I2C bus */
	if (I2C1->STATE & I2C_STATE_BUSY)
	{
		I2C1->CMD = I2C_CMD_ABORT;
	}

	/* Clearing any interrupt that would have been inadvertently configured */
	i2c_flags = I2C1->IF;
	I2C1->IFC = i2c_flags;

	I2C_GPIO_Enable();
}

/************************************************************************************
 * @function 	Initialize_TSL2651
 * @params 		None
 * @brief 		Routine to initialize TSL2561 ALS.
 ************************************************************************************/
void Initialize_TSL2651(void)
{
	uint8_t 	i2c_slave_address = I2C_TSL2561_SLAVE_ADDR;
	uint8_t		i2c_slave_data = 0;
	uint8_t 	read_write_bit = 0;
	uint8_t		i = 0;

	/* Write Operation*/
	read_write_bit = 0;

	I2C_Send_Start();

	i2c_slave_data = (i2c_slave_address << 1) | read_write_bit;
	I2C_Write_And_Wait_for_ACK(i2c_slave_data);

	/* Specify the address of consecutive registers to be written
	 * using the command register of TSL2561 */
	I2C_Write_And_Wait_for_ACK(I2C_TSL_INIT_CMD_REG_VAL);

	for (i = 0; i < 30; i++);

	I2C_Write_And_Wait_for_ACK(I2C_TSL_INIT_NUM_REGS_TO_WRITE);

	I2C_Write_And_Wait_for_ACK(I2C_TSL_INIT_CTRL_REG_VAL);

	for (i = 0; i < 30; i++);

	I2C_Write_And_Wait_for_ACK(I2C_TSL_INIT_TIM_REG_VAL);

	I2C_Write_And_Wait_for_ACK((I2C_TSL_INIT_THRES_LOW_REG_VAL & BITMASK_LOWER_BYTE));
	I2C_Write_And_Wait_for_ACK(((I2C_TSL_INIT_THRES_LOW_REG_VAL >> SHIFT_BY_EIGHT) & BITMASK_LOWER_BYTE));

	for (i = 0; i < 30; i++);

	I2C_Write_And_Wait_for_ACK((I2C_TSL_INIT_THRES_HIGH_REG_VAL & BITMASK_LOWER_BYTE));
	I2C_Write_And_Wait_for_ACK(((I2C_TSL_INIT_THRES_HIGH_REG_VAL >> SHIFT_BY_EIGHT) & BITMASK_LOWER_BYTE));

	I2C_Write_And_Wait_for_ACK(I2C_TSL_INIT_INT_CTRL_REG_VAL);

	for (i = 0; i < 30; i++);

	I2C_Send_Stop();

}

/************************************************************************************
 * @function 	I2C_Write_And_Wait_for_ACK
 * @params 		None
 * @brief 		Routine to write to transmit buffer of I2C and wait for acknowledgement.
 ************************************************************************************/
void I2C_Write_And_Wait_for_ACK(uint8_t data)
{
	I2C1->TXDATA = data;

	I2C_Wait_For_Ack();
}

/************************************************************************************
 * @function 	I2C_Wait_For_Ack
 * @params 		None
 * @brief 		Routine to wait for acknowledgement.
 ************************************************************************************/
void I2C_Wait_For_Ack(void)
{
	/* Wait for the slave to respond with an acknowlegdement (ACK) */
	while ((I2C1->IF & I2C_IF_ACK) == 0);

	/* Once we have received ACK from slave, clear it from the IF register */
	I2C1->IFC = I2C_IF_ACK;
}

/************************************************************************************
 * @function 	I2C_Send_Start
 * @params 		None
 * @brief 		Routine to send I2C start bit.
 ************************************************************************************/
void I2C_Send_Start(void)
{
	I2C1->CMD |= I2C_CMD_START;
}

/************************************************************************************
 * @function 	I2C_Send_Stop
 * @params 		None
 * @brief 		Routine to send I2C stop bit.
 ************************************************************************************/
void I2C_Send_Stop(void)
{
	I2C1->CMD |= I2C_CMD_STOP;
}

/************************************************************************************
 * @function 	I2C_Check_Data_Valid_and_Store
 * @params 		None
 * @brief 		Routine to check if received data is valid and store it.
 ************************************************************************************/
void I2C_Check_Data_Valid_and_Store(uint8_t *data)
{
	while(((I2C1->IF & I2C_IF_RXDATAV) == 0) && ((I2C1->STATUS & I2C_STATUS_RXDATAV) == 0));

	*data = I2C1->RXDATA;
}

/************************************************************************************
 * @function 	I2C_TSL2651_GPIO_Enable
 * @params 		None
 * @brief 		Routine to enable GPIO for TSL2561.
 ************************************************************************************/
void I2C_TSL2651_GPIO_Enable(void)
{
	/* Configure TSL power pin as push pull output */
	GPIO_PinModeSet(GPIO_PORT_D, TSL2651_POWER_PIN, gpioModePushPull, 0);

	/* Configure TSL interrupt pin as input */
	GPIO_PinModeSet(GPIO_PORT_D, TSL2651_INTERRUPT_PIN, gpioModeInput, 1);
}

/************************************************************************************
 * @function 	I2C_TSL2561_Interrupt_Enable
 * @params 		None
 * @brief 		Routine to enable interrupt for TSL2561.
 ************************************************************************************/
void I2C_TSL2561_Interrupt_Enable(void)
{
	/* Enable the GPIO Interrupt */
	GPIO_IntConfig(GPIO_PORT_D, TSL2651_INTERRUPT_PIN,
			I2C_TSL_INT_EN_ON_RIS_EDGE, I2C_TSL_INT_EN_ON_FALL_EDGE, true); /* Interrupt configured for rising edge */

	GPIO_IntClear(I2C_TSL_INT_CLEAR_FLAG);

	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);

	NVIC_EnableIRQ(GPIO_ODD_IRQn);
}

/************************************************************************************
 * @function 	I2C_TSL2651_GPIO_Disable
 * @params 		None
 * @brief 		Routine to disable GPIO for TSL2561.
 ************************************************************************************/
void I2C_TSL2651_GPIO_Disable(void)
{
	/* Configure TSL interrupt pin as input */
	GPIO_PinModeSet(GPIO_PORT_D, TSL2651_INTERRUPT_PIN, gpioModeDisabled, 0);

	/* Configure TSL power pin as push pull output */
	GPIO_PinModeSet(GPIO_PORT_D, TSL2651_POWER_PIN, gpioModeDisabled, 0);
}

/************************************************************************************
 * @function 	I2C_TSL2561_Interrupt_Disable
 * @params 		None
 * @brief 		Routine to disable interrupt for TSL2561.
 ************************************************************************************/
void I2C_TSL2561_Interrupt_Disable(void)
{
	NVIC_DisableIRQ(GPIO_ODD_IRQn);

	/* Disable the GPIO Interrupt */
	GPIO_IntConfig(GPIO_PORT_D, TSL2651_INTERRUPT_PIN,
			I2C_TSL_INT_EN_ON_RIS_EDGE, I2C_TSL_INT_EN_ON_FALL_EDGE, false);
}

/************************************************************************************
 * @function 	I2C_TSL_PowerOn_Routine
 * @params 		None
 * @brief 		Power On Routine for LPM for TSL2561.
 ************************************************************************************/
void I2C_TSL_PowerOn_Routine(void)
{
	uint32_t i = 0;

	/* Enable the TSL2651 onto the I2C bus */
	I2C_TSL2651_GPIO_Enable();

	/* Turn on power to TSL device */
	GPIO_PinOutSet(GPIO_PORT_D, TSL2651_POWER_PIN);

	for (i = 0; i < 30000; i++);
}

/************************************************************************************
 * @function 	I2C_TSL_Init
 * @params 		None
 * @brief 		Routine to initialize TSL2561.
 ************************************************************************************/
void I2C_TSL_Init(void)
{
	I2C_GPIO_Enable();

	Initialize_TSL2651();

	/* Enable TSL Interrupt */
	I2C_TSL2561_Interrupt_Enable();
}

/************************************************************************************
 * @function 	I2C_TSL_PowerOff_Routine
 * @params 		None
 * @brief 		Power Off Routine for LPM for TSL2561.
 ************************************************************************************/
void I2C_TSL_PowerOff_Routine(void)
{
	I2C_TSL2561_Interrupt_Disable();

	I2C_TSL_Deinit();

	I2C_TSL2651_GPIO_Disable();

	/* Turn off power to TSL device */
	GPIO_PinOutClear(GPIO_PORT_D, TSL2651_POWER_PIN);

	GPIO_PinModeSet(GPIO_PORT_D, TSL2651_POWER_PIN, gpioModeDisabled, 0);
}

/************************************************************************************
 * @function 	I2C_TSL_Deinit
 * @params 		None
 * @brief 		Routine to deinitialize TSL2561.
 ************************************************************************************/
void I2C_TSL_Deinit(void)
{
	I2C_GPIO_Disable();
}

/************************************************************************************
 * @function 	GPIO_ODD_IRQHandler
 * @params 		None
 * @brief 		GPIO IRQ Handler.
 ************************************************************************************/
void GPIO_ODD_IRQHandler(void)
{
	uint8_t 	i2c_slave_address = I2C_TSL2561_SLAVE_ADDR;	/* TSL2561 Slave Address */
	uint8_t		i2c_slave_data = 0;
	uint8_t 	read_write_bit = 0;
	uint8_t		i2c_data0_low = 0;
	uint8_t		i2c_data0_high = 0;
	uint8_t		i2c_data1_low = 0;
	uint8_t		i2c_data1_high = 0;

	uint16_t	i2c_data0 = 0;
	//uint16_t	i2c_data1 = 0;

#ifdef USE_INT
	INT_Disable();
#else
	CORE_DECLARE_IRQ_STATE;
	CORE_ENTER_ATOMIC();
#endif

	/* Clearing the source of interrupt: LETIMER0 comp0 interrupt flag */
	GPIO_IntClear(I2C_TSL_INT_CLEAR_FLAG);

	/* Write Operation*/
	read_write_bit = 0;

	I2C_Send_Start();

	i2c_slave_data = (i2c_slave_address << 1) | read_write_bit;
	I2C_Write_And_Wait_for_ACK(i2c_slave_data);

	I2C_Write_And_Wait_for_ACK(I2C_TSL_INIT_CMD_REG_VAL_2);

	read_write_bit = 1;
	I2C_Send_Start(); 		/* Repeat Start */

	i2c_slave_data = (i2c_slave_address << 1) | read_write_bit;
	I2C_Write_And_Wait_for_ACK(i2c_slave_data);

	I2C_Check_Data_Valid_and_Store(&i2c_data0_low);
	I2C1->CMD |= I2C_CMD_ACK;

	I2C_Check_Data_Valid_and_Store(&i2c_data0_high);
	I2C1->CMD |= I2C_CMD_ACK;

	I2C_Check_Data_Valid_and_Store(&i2c_data1_low);
	I2C1->CMD |= I2C_CMD_ACK;

	I2C_Check_Data_Valid_and_Store(&i2c_data1_high);
	I2C1->CMD |= I2C_CMD_NACK;

	I2C_Send_Stop();

	i2c_data0 = (i2c_data0_high << 8) | i2c_data0_low;
	//i2c_data1 = (i2c_data1_high << 8) | i2c_data1_low;

	if (i2c_als_state == ALS_STATE_DARK)
	{
		/* Dark State */
		if (i2c_data0 < I2C_TSL_INIT_THRES_LOW_REG_VAL)
		{
			/* Turning on LED1 */
			LED_On(LED0_1_GPIO_PORT, LED0_GPIO_PIN);

			i2c_als_state = ALS_STATE_LIGHT;

			leuart_led_data = ((LEUART_LED_DATA_IDENTIFIER << 7) | 0);
		}
	}
	else
	{
		/* Bright State */
		if (i2c_data0 > I2C_TSL_INIT_THRES_HIGH_REG_VAL)
		{
			/* Turning off LED1 */
			LED_Off(LED0_1_GPIO_PORT, LED0_GPIO_PIN);

			i2c_als_state = ALS_STATE_DARK;

			leuart_led_data = ((LEUART_LED_DATA_IDENTIFIER << 7) | 1);
		}
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
	puart_buffer++;
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

	read_write_bit = 0;

	I2C_Send_Start();

	i2c_slave_data = (i2c_slave_address << 1) | read_write_bit;
	I2C_Write_And_Wait_for_ACK(i2c_slave_data);

	I2C_Write_And_Wait_for_ACK(I2C_TSL_INIT_CMD_REG_VAL);

	I2C_Send_Stop();

#ifdef USE_INT
	INT_Enable();
#else
	CORE_EXIT_ATOMIC();
#endif

}
