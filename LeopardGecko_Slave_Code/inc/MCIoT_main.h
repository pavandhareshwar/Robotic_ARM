
#ifndef _MCIOT_MAIN_H_
#define _MCIOT_MAIN_H_
#endif

/************************************* MACROS ***************************************/

/* Maximum value for compare registers */
/* #define COMPMAX 500 */

/* Max Energy Mode */
#define MAX_ENERGY_MODE 4

/* Energy Modes */
#define EM0 0
#define EM1 1
#define EM2 2
#define EM3 3
#define EM4 4

/* Max frequencies */
#define LFXO_FREQUENCY 				32768		/* 32.768 kHz */
#define ULFRCO_FREQUENCY 			1000		/* 1 kHz */
#define ULFRCO_SELF_CALIBRATE 		1			/* Self Calibrate ULFRCO */

/* One Second and One millisecond definitions */
#define ONE_SEC 					1
#define ONE_MS 						1/1000

/* Calibration Period */
#define CALIBRATION_PERIOD 			1*ONE_SEC 	/* ULFRCO calibration period*/

#define MAX_PERIOD_32kHZ   			2*ONE_SEC 	/* Max time duration that can be generated using 32kHz clock */

/* LED On Time */
#define ALS_MIN_EXCITE_PERIOD 		4*ONE_MS 	/* Keep the ambient light sensor excited for a minimum of 4 ms */

#define DARK_REFERENCE_VDDLEVEL 	2
#define LIGHT_REFERENCE_VDDLEVEL 	61

#define ADC_EM						EM1

#define USE_DMA_FOR_ADC				1			/* Enable this to use DMA to transfer ADC samples to RAM */

//#define USE_ANY_ALS							/* Enable this to use ALS. Active or Passive*/
//#define USE_ACTIVE_ALS				1		/* Enable this to use active ALS */

#define ALS_EXCITE_PERIOD  			3.75*ONE_SEC/* Ambient Light Sensor Excite Time */

#define I2C_EM						EM3

#define LEUART_EM					EM2

//#define USE_INT						1

#define ENABLE_ADC_MODULE			1

#define ENABLE_LEUART_MODULE		1

//#define USE_LFRCO_FOR_LEUART_IN_EM3 1

/************************************* MACROS ***************************************/

/********************************** ENUMERATIONS ************************************/

/* Enumeration for LETimer Energy Modes */
typedef enum _ENERGY_MODES
{
	ENERGY_MODE_INVALID = -1,
	ENERGY_MODE_EM0,
	ENERGY_MODE_EM1,
	ENERGY_MODE_EM2,
	ENERGY_MODE_EM3,
	ENERGY_MODE_EM4,
	LETIMER_ENERGY_MODE_MAX
} ENERGY_MODES;

/********************************** ENUMERATIONS ************************************/

/************************************ GLOBALS ***************************************/

/* COMP1 is changed throughout the code to vary the PWM duty cycle
   but starts with the maximum value (100% duty cycle) */
/* uint16_t comp1 = COMPMAX; */

/* Sleep Block Counter */
uint32_t sleep_block_counter[MAX_ENERGY_MODE+1];

/* Initializing LETimer in energy mode EM2 */
ENERGY_MODES e_letimer_energy_modes;

float osc_ratio;

uint32_t prescaled_two_power;
uint32_t letimer0_prescaler;

bool LEUART_is_Enabled;

bool is_lesense_auth_done;

/************************************ GLOBALS ***************************************/
