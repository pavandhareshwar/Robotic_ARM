#ifndef _MCIOT_ADC_H_
#define _MCIOT_ADC_H_
#endif

/************************************ INCLUDES **************************************/

#include "em_adc.h"

/************************************ INCLUDES **************************************/

/************************************* MACROS ***************************************/

#define ADC_FREQUENCY 					14000000

#define ADC_SAMPLES        				500
#define ADC_SAMPLESPERSEC       		80000

/* ADC Init Param Macros */
#define ADC_INPUT_CHANNEL				adcSingleInputCh1
#define flexSensorCh1					adcSingleInputCh1
#define flexSensorCh2					adcSingleInputCh2
#define flexSensorCh3					adcSingleInputCh3
#define flexSensorCh4					adcSingleInputCh6
#define ADC_OVERSAMPLING_RATE_SEL		adcOvsRateSel2
#define ADC_LPF_MODE					adcLPFilterBypass
#define ADC_WARMUPMODE					adcWarmupNormal

#define ADC_CLOCK_FREQ_REQD				1400000

/* ADC Single Init Param Macros*/
#define ADC_REFERENCE					adcRef1V25
#define ADC_RESOLUTION					adcRes12Bit
#define ADC_PRS_CHANNEL					adcPRSSELCh0

#define ADC_ACQ_TIME_ADC_CLK_CYCLES		adcAcqTime4


/* Temperature Limits */
#define TEMPERATURE_LOWER_LIMIT 		15
#define TEMPERATURE_UPPER_LIMIT 		35
#define TEMPERATURE_READING_1V25_REF	0x0FE081BE
#define CALIBRATION_ADDRESS				0x0FE081B2
#define TEMPERATURE_GRADIENT			-6.27

#define ADC_CONVERSION_TYPE_CMD			adcStartSingle 	/* Command indicating which type of sampling to start */

/************************************* MACROS ***************************************/

/************************************ GLOBALS ***************************************/

/* Number of ADC samples transferred to RAM buffer for computation */
uint32_t adc_Samples_in_RAM;
uint8_t sampBuff_idx;
uint8_t sampBuff_max;
uint8_t DataValue;
float sampleBuffer[4];
ADC_InitSingle_TypeDef adc_InitSingle;

/************************************ GLOBALS ***************************************/

/****************************** FUNCTION PROTOTYPES *********************************/

void ADC_SetUp(void);

void ADC_Interrupt_Enable(void);

void ADC0_IRQHandler(void);

float converttoCelsius(int32_t adcSample);

float compute_adc_data_average(void);

/****************************** FUNCTION PROTOTYPES *********************************/
