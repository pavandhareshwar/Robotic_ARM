/*
 * Project_LESENSE_LETouch.h
 *
 *  Created on: Apr 2, 2017
 *      Author: pavandhareshwar
 */

#ifndef INC_PROJECT_LESENSE_LETOUCH_H_
#define INC_PROJECT_LESENSE_LETOUCH_H_

/************************************* MACROS ***************************************/

/* RTC nominal frequency */
#define RTC_FREQ               			32768

/* LESENSE number of channels possible to use, should be 16 */
#define NUM_LESENSE_CHANNELS    		16

/* GPIO Port for analog comparators */
#define LESENSE_CH_PORT         		gpioPortC

/** Scan frequency for LESENSE, how often all the pads are scanned. */
#define LESENSE_SCAN_FREQUENCY    		5

/** Sample delay, how long the rc-oscillations are sampled. */
#define SAMPLE_DELAY                	30

/** Validate count is the number of consecutive scan-cycles a button needs to */
/** be in the changed state before an actual button press or release is acknowledged. */
#define VALIDATE_CNT                   	10

/** Number of calibration events used to calculate threshold. */
#define NUMBER_OF_CALIBRATION_VALUES    10

/** Interval between calibration, in seconds. */
#define CALIBRATION_INTERVAL            5

/* LESENSE number of channels possible to use, should be 16 */
#define NUM_LESENSE_CHANNELS    		16

/* LESENSE core ctrl descriptor structure */
#define LESENSE_CORE_CTRL_SCAN_MODE 	lesenseScanStartPeriodic
#define	LESENSE_CORE_CTRL_PRS_SOURCE	lesensePRSCh0
#define	LESENSE_CORE_CTRL_SCAN_CONFIG	lesenseScanConfDirMap
#define	LESENSE_CORE_CTRL_BUFTRIG_LEVEL	lesenseBufTrigHalf
#define	LESENSE_CORE_CTRL_DMA_WAKEUP	lesenseDMAWakeUpDisable
#define	LESENSE_CORE_CTRL_BIAS_MODE		lesenseBiasModeDutyCycle

/* LESENSE peripheral ctrl descriptor structure */
#define LESENSE_PER_CTRL_DAC_DATAOUT	lesenseDACIfData
#define LESENSE_PER_CTRL_DAC_CONV_MODE	lesenseDACConvModeDisable
#define LESENSE_PER_CTRL_DAC_OUT_MODE	lesenseDACOutModeDisable
#define LESENSE_PER_CTRL_DAC_REF		lesenseDACRefBandGap
#define LESENSE_PER_CTRL_ACMP_MODE		lesenseACMPModeMux			// only acmp mux controlled by lesense
#define LESENSE_PER_CTRL_WARMUP_MODE	lesenseWarmupModeNormal

/* LESENSE timing ctrl descriptor structure */
#define LESENSE_TIM_CTRL_START_DELAY	0x0

/* LESENSE decoder ctrl descriptor structure */
#define LESENSE_DEC_CTRL_DEC_INPUT		lesenseDecInputSensorSt
#define LESENSE_DEC_CTRL_PRS_CH0_SRC	lesensePRSCh0
#define LESENSE_DEC_CTRL_PRS_CH1_SRC	lesensePRSCh1
#define LESENSE_DEC_CTRL_PRS_CH2_SRC	lesensePRSCh2
#define LESENSE_DEC_CTRL_PRS_CH3_SRC	lesensePRSCh3

/* LESENSE channel descriptor structure */
#define LESENSE_CH_DESC_CH_PIN_MODE_EXCITE_PHASE	lesenseChPinExDis
#define LESENSE_CH_DESC_CH_PIN_MODE_IDLE_PHASE		lesenseChPinIdleDis
#define LESENSE_CH_DESC_CH_COMP_MODE				lesenseCompModeLess
#define LESENSE_CH_DESC_EXC_SAMPLE_CLK				lesenseClkLF
#define LESENSE_CH_DESC_EXCITE_TIME					0x0
#define LESENSE_CH_DESC_MEASURE_DELAY				0x0
#define LESENSE_CH_DESC_ACMP_THRES					0x0				// don't care, configured by ACMPInit
#define	LESENSE_CH_DESC_SAMPLE_MODE					lesenseSampleModeCounter
#define	LESENSE_CH_DESC_INT_MODE					lesenseSetIntLevel
#define	LESENSE_CH_DESC_DATA_COMP_DEC_THRESH		0x0             // Configured later by calibration function

/************************************* MACROS ***************************************/

/**************************** STRUCTURES/ENUMERATIONS *******************************/

/**************************** STRUCTURES/ENUMERATIONS *******************************/

/************************************ GLOBALS ***************************************/

/************************************ GLOBALS ***************************************/

/****************************** FUNCTION PROTOTYPES *********************************/

void LESENSE_SetUp(void);

void LESENSE_IRQHandler(void);
void LETOUCH_Init(float sensitivity[]);

uint16_t LETOUCH_GetChannelsTouched(void);
uint16_t LETOUCH_GetChannelMaxValue(uint8_t channel);
uint16_t LETOUCH_GetChannelMinValue(uint8_t channel);

void LETOUCH_Calibration(void);

/****************************** FUNCTION PROTOTYPES *********************************/

#endif /* INC_PROJECT_LESENSE_LETOUCH_H_ */
