/***********************************************************************************************//**
 * \file   app.h
 * \brief  Application header file
 ***************************************************************************************************
 * <b> (C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

#ifndef APP_H
#define APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "em_leuart.h"
#include "em_core.h"
#include "em_ldma.h"

#include "retargetserial.h"

/***********************************************************************************************//**
 * \defgroup app Application Code
 * \brief Sample Application Implementation
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/


/***************************************************************************************************
  Type Definitions
***************************************************************************************************/
#define READ_SIZE	4

uint8_t sensor_data_buffer[READ_SIZE];

bool is_leuart_data_available;
uint8_t leuart_rx_data_count;

/***************************************************************************************************
  Function Declarations
***************************************************************************************************/

/***********************************************************************************************//**
 *  \brief  Initialise application, set device name.
 **************************************************************************************************/
void appInit (void);

/***********************************************************************************************//**
 *  \brief  Handle application events.
 *  \param[in]  evt  incoming event ID
 **************************************************************************************************/
void appHandleEvents(struct gecko_cmd_packet *evt);

void LEUART_Interrupt_Enable(LEUART_TypeDef *LEUART);

void LEUART_Setup();

void LDMA_SetUp(void);

void LEUART0_IRQHandler(void);

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */

#ifdef __cplusplus
};
#endif

#endif /* APP_H */
