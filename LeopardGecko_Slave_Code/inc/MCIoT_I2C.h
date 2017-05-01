/*
 * MCIoT_I2C.h
 *
 *  Created on: Feb 19, 2017
 *      Author: pavandhareshwar
 */

#ifndef INC_MCIOT_I2C_H_
#define INC_MCIOT_I2C_H_

/************************************ INCLUDES **************************************/

#include "em_i2c.h"

/************************************ INCLUDES **************************************/

/************************************* MACROS ***************************************/
#define I2C_BUS_FREQUENCY				I2C_FREQ_FAST_MAX	/* Set to fast rate */
#define I2C_CLOCK_RATE					i2cClockHLRStandard	/* Set to use 4:4 low/high duty cycle */
#define I2C_REF_CLOCK					0					/* Use currently configured reference clock */
#define I2C_MASTER_MODE					true				/* Set to master mode */
#define I2C_ENABLE						true				/* Enable when init done */

#define I2C_TSL2561_SLAVE_ADDR			0x39				/* TSL2561 Slave Address */

/* TSL Register Addresses */
#define I2C_TSL_CTRL_REG				0x00
#define I2C_TSL_TIMING_REG				0x01
#define I2C_TSL_THRES_LOW_LOW_REG		0x02
#define I2C_TSL_THRES_LOW_HIGH_REG		0x03
#define I2C_TSL_THRES_HIGH_LOW_REG		0x04
#define I2C_TSL_THRES_HIGH_HIGH_REG		0x05
#define I2C_TSL_INTERRUPT_REG			0x06
#define I2C_TSL_DATA0LOW_REG			0x0C
#define I2C_TSL_DATA0HIGH_REG			0x0D
#define I2C_TSL_DATA1LOW_REG			0x0E
#define I2C_TSL_DATA1HIGH_REG			0x0F

#define I2C_TSL_INIT_CMD_REG_VAL		0xD0				/* Command Code to start block write
																to TSL registers from 0x00*/
#define I2C_TSL_INIT_NUM_REGS_TO_WRITE	7
#define I2C_TSL_INIT_CTRL_REG_VAL		0x03
#define I2C_TSL_INIT_TIM_REG_VAL		0x01
#define I2C_TSL_INIT_THRES_LOW_REG_VAL	0x000f
#define I2C_TSL_INIT_THRES_HIGH_REG_VAL	0x0800
#define I2C_TSL_INIT_INT_CTRL_REG_VAL	0x14

#define BITMASK_LOWER_BYTE				0x0f
#define BITMASK_UPPER_BYTE				0xf0
#define SHIFT_BY_EIGHT					8

#define	I2C_TSL_INT_CLEAR_FLAG			0x2
#define I2C_TSL_INT_EN_ON_RIS_EDGE		false
#define I2C_TSL_INT_EN_ON_FALL_EDGE		true

#define I2C_TSL_INIT_CMD_REG_VAL_2		0x9C				/* Command Code to start block read
																from 0x0C TSL register*/

/************************************* MACROS ***************************************/

/********************************** ENUMERATIONS ************************************/

typedef enum _I2C_ALS_STATE_X_
{
	ALS_STATE_DARK = 0,
	ALS_STATE_LIGHT
} I2C_ALS_STATE_X;

/********************************** ENUMERATIONS ************************************/

/************************************ GLOBALS  **************************************/
I2C_ALS_STATE_X i2c_als_state;
/************************************ GLOBALS  **************************************/

/****************************** FUNCTION PROTOTYPES *********************************/

void I2C_SetUp(void);

void I2C_Write_And_Wait_for_ACK(uint8_t data);

void I2C_Wait_For_Ack(void);

void I2C_Send_Start(void);

void I2C_Send_Stop(void);

void Initialize_TSL2651(void);

void I2C_TSL2561_Interrupt_Enable(void);

void I2C_TSL2651_GPIO_Enable(void);

void I2C_TSL2561_Interrupt_Disable(void);

void I2C_TSL2651_GPIO_Disable(void);

void I2C_TSL_PowerOn_Routine(void);

void I2C_TSL_Init(void);

void I2C_TSL_PowerOff_Routine(void);

void I2C_TSL_Deinit(void);

/****************************** FUNCTION PROTOTYPES *********************************/

#endif /* INC_MCIOT_I2C_H_ */
