/*****************************************************************************
 * @file 	MCIoT_DMA.c
 * @brief 	This file describes the functions pertaining to DMA setup and
 * 			callback function.
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
#include "MCIoT_ADC.h"
#include "MCIoT_DMA.h"
#include "MCIoT_CMU.h"
#include "MCIoT_GPIO.h"
#include "MCIoT_Sleep.h"
#include "MCIoT_LEUART.h"

/************************************************************************************
 * @function 	DMA_SetUp
 * @params 		None
 * @brief 		Configures the DMA.
 ************************************************************************************/
void DMA_SetUp(void)
{
	DMA_Init_TypeDef        dmaInit; 	/* SetUp basic operations of the DMA peripherals */
	DMA_CfgChannel_TypeDef  chnlCfg; 	/* Configure DMA channels */
	DMA_CfgDescr_TypeDef    descrCfg;	/* Configure DMA descriptors */

	/* Initializing the DMA */
	dmaInit.hprot        = 0;                                          /* No description protection */
	dmaInit.controlBlock = (DMA_DESCRIPTOR_TypeDef *)dmaControlBlock;

	DMA_Init(&dmaInit);

	/* Setting up call-back function */
	/* Callback function is DMA's version of interrupt handling function */
	dma_cb_fn.cbFunc  = (DMA_FuncPtr_TypeDef)ADC_dma_ch0_TransferComplete;
	dma_cb_fn.userPtr = NULL;
	//dma_cb_fn.primary = true;

	/* Setting up channel */
	chnlCfg.highPri   = true;						/* channel priority is high */
	chnlCfg.enableInt = true;
	chnlCfg.select    = DMA_SIGNAL_SOURCE; 			/* ADC DMA request to use the ADC Single DMA request line */
	chnlCfg.cb        = &dma_cb_fn;
	DMA_CfgChannel(DMA_CHANNEL_ADC, &chnlCfg);

	/* Setting up channel descriptor */
	descrCfg.dstInc  = DMA_DST_INCREMENT_SIZE;		/* Increment dst address by 2 bytes */
	descrCfg.srcInc  = DMA_SRC_INCREMENT_SIZE;		/* Do not increment src address */
	descrCfg.size    = DMA_TRANSFER_SIZE;			/* Transfer size of 2 bytes */
	descrCfg.arbRate = DMA_TRANSFER_ARBRATE;		/* ADC DMA arbitration bit set to 0 */
	descrCfg.hprot   = 0;

	DMA_CfgDescr(DMA_CHANNEL_ADC, true, &descrCfg);
}

/************************************************************************************
 * @function 	ADCdmach0TransferComplete
 * @params 		[in] channel 	- (uint32_t) DMA channel
 * 				[in] primary 	- (bool) primary/alternate DMA descriptor
 * 				[in] user		- (void *)
 * @brief 		Callback function for DMA.
 ************************************************************************************/
void ADC_dma_ch0_TransferComplete(uint32_t channel, bool primary, void *user)
{
	float average = 0.0;
	int retVal = -1;

	/* Turn off ADC */
	ADC0->CMD |= ADC_CMD_SINGLESTOP;

	/* Disable clock to ADC0 */
	CMU_ClockEnable(cmuClock_ADC0, false); /* To enable clock to ADC0 */

	unblockSleepMode(ADC_EM);

	average = compute_adc_data_average();

#ifdef USE_INT
	INT_Disable();
#else
	CORE_DECLARE_IRQ_STATE;
	CORE_ENTER_ATOMIC();
#endif

	/* To ensure the following circular buffer writes are atomic */
	leuart_temp_sign_data = (average > 0) ? 1 : 0;
	leuart_temp_sign_data |= (LEUART_TEMP_DATA_IDENTIFIER << 7);

	if (false == state_is_em1_for_leuart_tx)
	{
		blockSleepMode(LEUART_EM);
		state_is_em1_for_leuart_tx = true;
	}

#if 0

#ifdef USE_CIRC_BUFFER_FOR_LEUART
	retVal = WriteDataToCircBuff(&leuart_circ_buff, leuart_temp_sign_data);
	if (0 == retVal)
	{
		if (false == leuart_nvic_is_enabled)
		{
			/* Enable the NVIC for LEUART0 so that the IRQ handler can be
			 * triggered to transmit the data */
			NVIC_EnableIRQ(LEUART0_IRQn);
			leuart_nvic_is_enabled = true;
		}

		leuart_temp_mantissa_data = (average * 10) / 10;
		retVal = WriteDataToCircBuff(&leuart_circ_buff, leuart_temp_mantissa_data);

		if (0 == retVal)
		{
			leuart_temp_exponent_data = (int)(average * 10) % 10;
			WriteDataToCircBuff(&leuart_circ_buff, leuart_temp_exponent_data);
		}
	}
#else
	*puart_buffer = leuart_temp_sign_data;
	/* Enable the NVIC for LEUART0 so that the IRQ handler can be
	 * triggered to transmit the data */
	NVIC_EnableIRQ(LEUART0_IRQn);

	leuart_temp_mantissa_data = (average * 10) / 10;
	*(puart_buffer+1) = leuart_temp_mantissa_data;

	leuart_temp_exponent_data = (int)(average * 10) % 10;
	*(puart_buffer+2) = leuart_temp_exponent_data;

	//puart_buffer += 2;
#endif
#endif

#ifdef USE_INT
	INT_Enable();
#else
	CORE_EXIT_ATOMIC();
#endif

	//NVIC_EnableIRQ(LEUART0_IRQn);

#if 0
	/* Data Corruption Part */
	char dummy_string[] = {'A', 'B', 'C', 'D'};
#ifndef USE_CIRC_BUFFER_FOR_LEUART
	for (int i=0; i < LEUART_BUFF_LENGTH; i++)
		*(puart_buffer + i) = dummy_string[i];
#else
	for (int i=0; i < LEUART_BUFF_LENGTH; i++)
		WriteDataToCircBuff(&leuart_circ_buff, dummy_string[i]);
#endif

	NVIC_EnableIRQ(LEUART0_IRQn);
#endif

	if ((TEMPERATURE_LOWER_LIMIT <= average)  && (average <= TEMPERATURE_UPPER_LIMIT))
	{
		/* Turning off LED1 */
		LED_Off(LED0_1_GPIO_PORT, LED1_GPIO_PIN);
	}
	else
	{
		/* Turning on LED1 */
		LED_On(LED0_1_GPIO_PORT, LED1_GPIO_PIN);
	}

}
