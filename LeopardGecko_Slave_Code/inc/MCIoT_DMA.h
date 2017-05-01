#ifndef _MCIOT_ADC_H_
#define _MCIOT_ADC_H_
#endif

/************************************ INCLUDES **************************************/

#include "em_dma.h"
#include "dmactrl.h"

/************************************ INCLUDES **************************************/

/************************************* MACROS ***************************************/

#define DMA_CHANNEL_ADC       		0
#define ADC_SAMPLE_TRANSFER_SIZE	1

/* DMA Channel Configuration Macros */
#define DMA_SIGNAL_SOURCE			DMAREQ_ADC0_SINGLE

/* DMA Channel Descriptor Macros */
#define DMA_DST_INCREMENT_SIZE		dmaDataInc2
#define DMA_SRC_INCREMENT_SIZE		dmaDataIncNone
#define DMA_TRANSFER_SIZE			dmaDataSize2
#define DMA_TRANSFER_ARBRATE		dmaArbitrate1

/************************************* MACROS ***************************************/

/************************************ GLOBALS ***************************************/

DMA_CB_TypeDef dma_cb_fn;

volatile uint16_t ADCDataRAMBuffer[ADC_SAMPLES];
volatile uint16_t *p_ADCDataRAMBuffer;

/************************************ GLOBALS ***************************************/

/****************************** FUNCTION PROTOTYPES *********************************/

void DMA_SetUp(void);

void ADC_dma_ch0_TransferComplete(uint32_t channel, bool primary, void *user);

/****************************** FUNCTION PROTOTYPES *********************************/
