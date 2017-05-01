#ifndef _MCIOT_GPIO_H_
#define _MCIOT_GPIO_H_
#endif

/************************************ INCLUDES **************************************/

#include "em_gpio.h"

/************************************ INCLUDES **************************************/

/************************************* MACROS ***************************************/

/* LED GPIO port name and pin */
#define GPIO_PORT_C					gpioPortC
#define GPIO_PORT_D					gpioPortD
#define GPIO_PORT_E					gpioPortE

#define LED0_1_GPIO_PORT 			GPIO_PORT_E
#define LED0_GPIO_PIN				2
#define LED1_GPIO_PIN				3

#define LIGHT_SENSE_PORT			GPIO_PORT_C
#define LIGHT_SENSE_PIN				6

#define ALS_GPIO_PORT 				gpioPortD
#define ALS_SENSE_GPIO_PIN 			6

#define I2C_GPIO_PORT				GPIO_PORT_C
#define I2C_SDA_PIN				4
#define I2C_SCL_PIN				5

#define TSL2651_POWER_PIN			0
#define TSL2651_INTERRUPT_PIN		        1

#define LEUART_GPIO_PORT                        GPIO_PORT_D
#define LEUART_GPIO_TX_PIN                      4
#define LEUART_GPIO_RX_PIN                      5

/************************************* MACROS ***************************************/

/****************************** FUNCTION PROTOTYPES *********************************/

void GPIO_SetUp(void);

void LED_On(GPIO_Port_TypeDef port, unsigned int pin);

void LED_Off(GPIO_Port_TypeDef port, unsigned int pin);

void I2C_GPIO_Enable(void);

void I2C_GPIO_Disable(void);

/****************************** FUNCTION PROTOTYPES *********************************/
