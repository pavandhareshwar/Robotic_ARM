#ifndef _MCIOT_LEUART_H_
#define _MCIOT_LEUART_H_
#endif

/************************************ INCLUDES **************************************/

#include "em_leuart.h"

/************************************ INCLUDES **************************************/

/************************************* MACROS ***************************************/

//#define ENABLE_RX_FOR_LOOPBACK

/* LEUART Init Parameters */
#ifdef ENABLE_RX_FOR_LOOPBACK
#define LEUART_ENABLE_SEL             	leuartEnable       		/* Activate both data transmission and reception for LEUART */
#else
#define LEUART_ENABLE_SEL             	leuartEnableTx       	/* Activate data transmission only for LEUART */
#endif

#define USE_CIRC_BUFFER_FOR_LEUART

#define LEUART_REF_FREQ               	0                    	/* Inherit the clock frequenzy from the LEUART clock source */
#define LEUART_BAUD_RATE              	9600                 	/* Baudrate = 9600 bps */
#define LEUART_DATA_BITS              	leuartDatabits8     	/* Each LEUART frame containes 8 databits */
#define LEUART_PARITY                 	leuartNoParity       	/* No parity bits */
#define LEUART_STOP_BITS              	leuartStopbits1      	/* 1 Stop Bit */

#ifdef USE_CIRC_BUFFER_FOR_LEUART
#define CIRC_BUFF_MAX_LENGTH			10
#define LEUART_BUFF_LENGTH				4
#else
#define LEUART_BUFF_LENGTH				4
#endif

#define LEUART_LED_DATA_IDENTIFIER		0
#define LEUART_TEMP_DATA_IDENTIFIER		1

#define LEUART_POST_TX_WAIT_TIME_EM2	0
#ifdef USE_LFRCO_FOR_LEUART_IN_EM3
#define LEUART_POST_TX_WAIT_TIME_EM3	1620
#else
#define LEUART_POST_TX_WAIT_TIME_EM3	2500
#endif

/************************************* MACROS ***************************************/

/************************************ GLOBALS ***************************************/
uint8_t		leuart_led_data;
uint8_t 	leuart_temp_mantissa_data;
uint8_t		leuart_temp_exponent_data;
uint8_t    	leuart_temp_sign_data;

#ifdef USE_CIRC_BUFFER_FOR_LEUART
typedef struct _LEUART_CIRC_BUFFER_
{
	float *circ_buff; 			/* data pointer */
	int write;					/* Current write location of buffer */
	int read;					/* Current read location of buffer */
	int maxLength;				/* Max number of data in the buffer */

}LEUART_CIRC_BUFFER;

LEUART_CIRC_BUFFER leuart_circ_buff;
#else
char leuart_buffer[LEUART_BUFF_LENGTH];
char *puart_buffer;
unsigned int read_count;
unsigned int temp_data_count;
#endif

bool state_is_em1_for_leuart_tx;
bool leuart_nvic_is_enabled;

bool led_data_available;
bool temp_data_available;

/************************************ GLOBALS ***************************************/

/****************************** FUNCTION PROTOTYPES *********************************/

void LEUART_SetUp(void);

void LEUART_Interrupt_Enable(LEUART_TypeDef *LEUART);

#ifdef USE_CIRC_BUFFER_FOR_LEUART
int WriteDataToCircBuff(LEUART_CIRC_BUFFER *leuart_circ_buff, float data);

int ReadDataFromCircBuff(LEUART_CIRC_BUFFER *leuart_circ_buff, float *data);
#endif

/****************************** FUNCTION PROTOTYPES *********************************/
