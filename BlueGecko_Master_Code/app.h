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

//#define ENABLE_PERIODIC_ROLE_REVERSAL

/***************************************************************************************************
  Type Definitions
***************************************************************************************************/
typedef enum _connState{
	eStateInvalid = 0,
	eStateDisconnected,
	eStateConnected,
	eStateScanning,
	eStateFindService,
	eStateFindCharacteristic,
	eStateEnableNotif,
	eStateDataMode,
	eStateMax
}connState;

typedef enum
{
	DEVICE_ROLE_INVALID,
	DEVICE_ROLE_MASTER,
	DEVICE_ROLE_SLAVE,
	DEVICE_ROLE_MAX
}device_role;

uint8_t slave_connection;

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


/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */

#ifdef __cplusplus
};
#endif

#endif /* APP_H */
