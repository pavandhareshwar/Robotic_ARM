/***********************************************************************************************//**
 * \file   app_hw.h
 * \brief  Hardware specific application header file
 ***************************************************************************************************
 * <b> (C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

#ifndef APP_HW_H
#define APP_HW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/***********************************************************************************************//**
 * \defgroup app_hw Application Hardware Specific
 * \brief Hardware specific application file.
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
  Data Types
***************************************************************************************************/

/***************************************************************************************************
  Function Declarations
***************************************************************************************************/

/***********************************************************************************************//**
 *  \brief  Initialize buttons and Temperature sensor.
 **************************************************************************************************/
void appHwInit(void);

/***********************************************************************************************//**
 *  \brief  Perform a temperature measurement.  Return the measurement data.
 *  \param[out]  tempData  Result of temperature conversion.
 *  \return  0 if Temp Read successful, otherwise -1
 **************************************************************************************************/
int32_t appHwReadTm(int32_t* tempData);

int32_t appHwReadHumidity(uint32_t* humidityData);

/***********************************************************************************************//**
 *  \brief  Initialise temperature measurement.
 *  \return  true if a Si7013 is detected, false otherwise
 **************************************************************************************************/
bool appHwInitTempSens(void);


/** @} (end addtogroup app_hw) */
/** @} (end addtogroup Application) */

#ifdef __cplusplus
};
#endif

#endif /* APP_HW_H */
