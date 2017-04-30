/***********************************************************************************************//**
 * \file   main.c
 * \brief  Silabs HTM IAS and Beaconing Demo Application
 *         This application is intended to be used with the iOS Silicon Labs
 *         app for demonstration purposes
 ***************************************************************************************************
 * <b> (C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/
#ifndef GENERATION_DONE
#error You must run generate first!
#endif

#include "boards.h"
#include "ble-configuration.h"
#include "board_features.h"

/* BG stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"
#include "aat.h"

/* application specific files */
#include "app.h"

/* libraries containing default gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"

#ifdef FEATURE_BOARD_DETECTED
#include "bspconfig.h"
#include "pti.h"
#else
#error This sample app only works with a Silicon Labs Board
#endif

#ifdef FEATURE_IOEXPANDER
#include "bsp.h"
#include "bsp_stk_ioexp.h"
#endif /* FEATURE_IOEXPANDER */ 

/* Device initialization header */
#include "InitDevice.h"

#ifdef FEATURE_SPI_FLASH
#include "em_usart.h"
#include "mx25flash_spi.h"
#endif /* FEATURE_SPI_FLASH */


/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

#ifdef FEATURE_PTI_SUPPORT
static const RADIO_PTIInit_t ptiInit = RADIO_PTI_INIT;
#endif

/* Gecko configuration parameters (see gecko_configuration.h) */
static const gecko_configuration_t config = {
  .config_flags=0,
  .sleep.flags=SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
  .bluetooth.max_connections=MAX_CONNECTIONS,
  .bluetooth.heap=bluetooth_stack_heap,
  .bluetooth.heap_size=sizeof(bluetooth_stack_heap),
  .bluetooth.sleep_clock_accuracy = 100, // ppm
  .gattdb=&bg_gattdb_data,
  .ota.flags=0,
  .ota.device_name_len=3,
  .ota.device_name_ptr="OTA",
  #ifdef FEATURE_PTI_SUPPORT
  .pti = &ptiInit,
  #endif
};

void Clock_SetUp(void)
{
	CMU_ClockEnable(cmuClock_CORELE, true); 			/* To enable the low frequency clock tree */

	/* Enable necessary clocks */
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true); 		/* To enable the LFXO */
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO); 	/* To support energy modes EM0-EM2 */

	CMU_ClockEnable(cmuClock_HFPER, true); 				/* To enable the clock tree for Timer peripherals */

	CMU_ClockEnable(cmuClock_GPIO, true);				/* To enable clock to GPIO */
}

int main(void)
{
#ifdef FEATURE_SPI_FLASH
	/* Put the SPI flash into Deep Power Down mode for those radio boards where it is available */
	MX25_init();
	MX25_DP();
	/* We must disable SPI communication */
	USART_Reset(USART1);

#endif /* FEATURE_SPI_FLASH */

	/* Initialize peripherals */
	enter_DefaultMode_from_RESET();
	/* Initialize stack */
	gecko_init(&config);

	Clock_SetUp();

#ifdef FEATURE_IOEXPANDER
	if ( BSP_IOEXP_DEVICE_ID == BSP_IOExpGetDeviceId()) {
		BSP_PeripheralAccess(BSP_IOEXP_VCOM,   0); // Disable VCOM
		BSP_PeripheralAccess(BSP_IOEXP_DISPLAY,   1); // Enables the display by pulling DISP_ENABLE high.
		BSP_PeripheralAccess(BSP_IOEXP_SENSORS, 1); // Enables the Si7021 sensor on the Wireless STK by pulling SENSOR_ENABLE high
		BSP_PeripheralAccess(BSP_IOEXP_LEDS,    0); // The LEDs follow the bits LED0 and LED1 when this bit is set
	}
#endif /* FEATURE_IOEXPANDER */

	while (1) {
		struct gecko_cmd_packet* evt;
		/* Check for stack event. */
		evt = gecko_wait_event();
		/* Run application and event handler. */
		appHandleEvents(evt);
	}
	return 0;
}



/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */


