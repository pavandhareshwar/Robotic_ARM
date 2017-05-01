/***********************************************************************************************//**
 * \file   app.c
 * \brief  Application code
 ***************************************************************************************************
 * <b> (C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

/* standard library headers */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/* BG stack headers */
#include "bg_types.h"
#include "native_gecko.h"

/* profiles */
#include "htm.h"
#include "ia.h"

/* BG stack headers*/
#include "gatt_db.h"

/* em library */
#include "em_system.h"
#include "em_cmu.h"
#include "em_gpio.h"

/* application specific headers */
#include "app_ui.h"
#include "app_hw.h"
#include "advertisement.h"
#include "beacon.h"
#include "app_timer.h"
#include "board_features.h"

/* emlib headers */
#include "em_emu.h"
#include "em_cmu.h"

/* Own header */
#include "app.h"

int power_led_status = 0;
int connected = 0;
int alarm_status = 0;
int alarm_count = 0;

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/


/***************************************************************************************************
  Local Macros and Definitions
 **************************************************************************************************/

/***************************************************************************************************
 Local Variables
 **************************************************************************************************/

/***************************************************************************************************
 Static Function Declarations
 **************************************************************************************************/
   #ifndef FEATURE_IOEXPANDER
 /* Periodically called Display Polarity Inverter Function for the LCD.
  Toggles the the EXTCOMIN signal of the Sharp memory LCD panel, which prevents building up a DC 
  bias according to the LCD's datasheet */
static void (*dispPolarityInvert)(void *);
  #endif /* FEATURE_IOEXPANDER */

/***************************************************************************************************
 Function Definitions
 **************************************************************************************************/

/***********************************************************************************************//**
 * \brief Function that initializes the device name, LEDs, buttons and services.
 **************************************************************************************************/
void appInit(void)
{
  /* Unique device ID */
  uint16_t devId;
  struct gecko_msg_system_get_bt_address_rsp_t* btAddr;
  char devName[APP_DEVNAME_LEN + 1];

  /* Init device name */
  /* Get the unique device ID */

  /* Create the device name based on the 16-bit device ID */
  btAddr = gecko_cmd_system_get_bt_address();
  devId = *((uint16*)(btAddr->address.addr));
  snprintf(devName, APP_DEVNAME_LEN + 1, APP_DEVNAME, devId);
  gecko_cmd_gatt_server_write_attribute_value(gattdb_device_name,
                                              0,
                                              strlen(devName),
                                              (uint8_t *)devName);

  /* Initialize LEDs, buttons, graphics. */
  appUiInit(devId);

  /* Hardware initialization. Initializes temperature sensor. */
  appHwInit();

  /* Initialize services */
  htmInit();
}

void manage_power_led()
{
	if (connected)
	{
		if (power_led_status == 0)
		{
			GPIO_PinOutSet(gpioPortF, 4);
			power_led_status = 1;
			gecko_cmd_hardware_set_soft_timer(3276, LED_TIMER, false);
		}
		else
		{
			GPIO_PinOutClear(gpioPortF, 4);
			power_led_status = 0;
			gecko_cmd_hardware_set_soft_timer(32768, LED_TIMER, false);
		}
	}
}

void manage_alarm_led()
{
	if (alarm_status == 0)
	{
		if (alarm_count == 5)
		{
			gecko_cmd_hardware_set_soft_timer(0, ALARM_TIMER, false);
		}
		else
		{
			alarm_count++;
			GPIO_PinOutSet(gpioPortF, 5);
			alarm_status = 1;
			gecko_cmd_hardware_set_soft_timer(3276, ALARM_TIMER, false);
		}
	}
	else
	{
		GPIO_PinOutClear(gpioPortF, 5);
		alarm_status = 0;
		gecko_cmd_hardware_set_soft_timer(32768, ALARM_TIMER, false);
	}
}

/***********************************************************************************************//**
 * \brief Event handler function
 * @param[in] evt Event pointer
 **************************************************************************************************/
void appHandleEvents(struct gecko_cmd_packet *evt)
{
	/* Flag for indicating DFU Reset must be performed */
	static uint8_t boot_to_dfu = 0;

	switch (BGLIB_MSG_ID(evt->header)) {

	/* Boot event and connection closed event */
	case gecko_evt_system_boot_id:

		/* All the clocks necessary are enabled here */
		/* Enable necessary clocks */
		CMU_OscillatorEnable(cmuOsc_LFXO, true, true); 		/* To enable the LFXO */

		CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO); 	/* To support energy modes EM0-EM2 */

		LEUART_Setup();

		LDMA_SetUp();

		is_leuart_data_available = false;
		leuart_rx_data_count = 0;

		/* Initialize app */
		appInit(); /* App initialization */
		htmInit(); /* Health thermometer initialization */
		advSetup(); /* Advertisement initialization */

		/* -------------------------------------------------------------------------- */
		/* Code for the link loss service example */

		/* LED0 - Power LED */
		GPIO_PinModeSet(gpioPortF, 4, gpioModePushPull, 0);

		/* LED1 - Alarm LED */
		GPIO_PinModeSet(gpioPortF, 5, gpioModePushPull, 0);

		/* -------------------------------------------------------------------------- */

		/* Set advertising parameters. 100ms advertisement interval. All channels used.
		 * The first two parameters are minimum and maximum advertising interval, both in
		 * units of (milliseconds * 1.6). The third parameter '7' sets advertising on all channels. */
		gecko_cmd_le_gap_set_adv_parameters(160,160,7);

		/* Start general advertising and enable connections. */
		gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);

		break;

		/* This event is generated when a connected client has either
		 * 1) changed a Characteristic Client Configuration, meaning that they have enabled
		 * or disabled Notifications or Indications, or
		 * 2) sent a confirmation upon a successful reception of the indication. */
	case gecko_evt_le_connection_closed_id:

		/* Initialize app */
		appInit(); /* App initialization */
		htmInit(); /* Health thermometer initialization */
		advSetup(); /* Advertisement initialization */

		/* Enter to DFU OTA mode if needed */
		if (boot_to_dfu) {
			gecko_cmd_system_reset(2);
		}

		connected = 0;
		gecko_cmd_hardware_set_soft_timer(32768, ALARM_TIMER, false);

		gecko_cmd_hardware_set_soft_timer(TIMER_STOP, LED_TIMER, false);

		GPIO_PinOutClear(gpioPortF, 4);

		break;

		/* Connection opened event */
	case gecko_evt_le_connection_opened_id:
		/* Call advertisement.c connection started callback */
		advConnectionStarted();
		/* Connection Open Event */

		connected = 1;

		gecko_cmd_hardware_set_soft_timer(32768, LED_TIMER, false);

		break;

		/* Value of attribute changed from the local database by remote GATT client */
	case gecko_evt_gatt_server_attribute_value_id:
		/* Check if changed characteristic is the Immediate Alert level */
		if ( gattdb_alert_level == evt->data.evt_gatt_server_attribute_value.attribute) {
			/* Write the Immediate Alert level value */
			iaImmediateAlertWrite(&evt->data.evt_gatt_server_attribute_value.value);
		}
		break;

		/* Indicates the changed value of CCC or received characteristic confirmation */
	case gecko_evt_gatt_server_characteristic_status_id:
		/* Check if changed client char config is for the temperature measurement */
		if ((gattdb_temp_measurement == evt->data.evt_gatt_server_attribute_value.attribute)
				&& (evt->data.evt_gatt_server_characteristic_status.status_flags == 0x01)) {
			// Call HTM temperature characteristic status changed callback
			htmTemperatureCharStatusChange(
					evt->data.evt_gatt_server_characteristic_status.connection,
					evt->data.evt_gatt_server_characteristic_status.client_config_flags);
		}

		break;

		/* Software Timer event */
	case gecko_evt_hardware_soft_timer_id:
		/* Check which software timer handle is in question */
		switch (evt->data.evt_hardware_soft_timer.handle) {
		case UI_TIMER: /* App UI Timer (LEDs, Buttons) */
			appUiTick();
			break;
		case ADV_TIMER: /* Advertisement Timer */
			advSetup();
			break;
		case TEMP_TIMER: /* Temperature measurement timer */
			if (is_leuart_data_available == true)
			{
				/* Make a temperature measurement */
				htmTemperatureMeasure();
				is_leuart_data_available = false;
			}
			break;
#ifndef FEATURE_IOEXPANDER
		case DISP_POL_INV_TIMER:
			/*Toggle the the EXTCOMIN signal, which prevents building up a DC bias  within the
			 * Sharp memory LCD panel */
			dispPolarityInvert(0);
			break;
#endif /* FEATURE_IOEXPANDER */
		case LED_TIMER:
			manage_power_led();
			break;
		case ALARM_TIMER:
			manage_alarm_led();
			break;
		default:
			break;
		}
		break;

		/* User write request event. Checks if the user-type OTA Control Characteristic was written.
		 * If written, boots the device into Device Firmware Upgrade (DFU) mode. */
		case gecko_evt_gatt_server_user_write_request_id:
			/* Handle OTA */
			if(evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control)
			{
				/* Set flag to enter to OTA mode */
				boot_to_dfu = 1;
				/* Send response to Write Request */
				gecko_cmd_gatt_server_send_user_write_response(
						evt->data.evt_gatt_server_user_write_request.connection,
						gattdb_ota_control,
						bg_err_success);

				/* Close connection to enter to DFU OTA mode */
				gecko_cmd_endpoint_close(evt->data.evt_gatt_server_user_write_request.connection);
			}
			break;

		default:
			break;
	}
}

/**************************************************************************//**
 * @brief   Register a callback function at the given frequency.
 *
 * @param[in] pFunction  Pointer to function that should be called at the
 *                       given frequency.
 * @param[in] argument   Argument to be given to the function.
 * @param[in] frequency  Frequency at which to call function at.
 *
 * @return  0 for successful or
 *         -1 if the requested frequency does not match the RTC frequency.
 *****************************************************************************/
int rtcIntCallbackRegister(void (*pFunction)(void*),
                           void* argument,
                           unsigned int frequency)
{
  #ifndef FEATURE_IOEXPANDER
  
  dispPolarityInvert =  pFunction;
  /* Start timer with required frequency */
  gecko_cmd_hardware_set_soft_timer(TIMER_MS_2_TIMERTICK(1000/frequency), DISP_POL_INV_TIMER, false);
  
  #endif /* FEATURE_IOEXPANDER */

  return 0;
}


/************************************************************************************
 * @function 	LEUART_Interrupt_Enable
 * @params 		None
 * @brief 		Enables the necessary interrupts for LEUART0.
 ************************************************************************************/
void LEUART_Interrupt_Enable(LEUART_TypeDef *LEUART)
{
	uint32_t leuart_if = LEUART->IF;

	/* Clear all the interrupts that may have been set-up inadvertently */
	LEUART->IFC |= leuart_if;

#ifdef ENABLE_TX_FOR_LOOPBACK
	/* Enable TXBL interrupt */
	LEUART_IntEnable(LEUART, LEUART_IF_TXBL);
#endif

	LEUART_IntEnable(LEUART, LEUART_IF_RXDATAV);
}

void LEUART_Setup()
{
	/* Enable CORE LE clock in order to access LE modules */
	CMU_ClockEnable(cmuClock_CORELE, true);

	/* Enable peripheral clocks */
	CMU_ClockEnable(cmuClock_HFPER, true);
	/* Configure GPIO pins */
	CMU_ClockEnable(cmuClock_GPIO, true);

	/* To avoid false start, configure output as high */
	GPIO_PinModeSet(RETARGET_TXPORT, RETARGET_TXPIN, gpioModePushPull, 1);
	GPIO_PinModeSet(RETARGET_RXPORT, RETARGET_RXPIN, gpioModeInput, 0);

	LEUART_Init_TypeDef init = LEUART_INIT_DEFAULT;

	/* Select LFXO for LEUARTs (and wait for it to stabilize) */
	CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);

	CMU_ClockEnable(cmuClock_LEUART0, true);

	/* Do not prescale clock */
	CMU_ClockDivSet(cmuClock_LEUART0, cmuClkDiv_1);

	/* Configure LEUART */
	init.enable = leuartDisable;

	LEUART_Init(LEUART0, &init);

	/* Enable pins at default location */
	LEUART0->ROUTELOC0 = (LEUART0->ROUTELOC0 & ~(_LEUART_ROUTELOC0_TXLOC_MASK
										| _LEUART_ROUTELOC0_RXLOC_MASK))
	                    				| (RETARGET_TX_LOCATION << _LEUART_ROUTELOC0_TXLOC_SHIFT)
										| (RETARGET_RX_LOCATION << _LEUART_ROUTELOC0_RXLOC_SHIFT);

	LEUART0->ROUTEPEN  = USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;

	/* Set RXDMAWU to wake up the DMA controller in EM2 */
	LEUART_RxDmaInEM2Enable(LEUART0, true);

	/* Finally enable it */
	LEUART_Enable(LEUART0, leuartEnable);
}

void LDMA_SetUp(void)
{

	/** LDMA Descriptor initialization */
	static LDMA_Descriptor_t xfer =
			LDMA_DESCRIPTOR_LINKREL_P2M_BYTE(&LEUART0->RXDATA, 	/* Peripheral source address */
					sensor_data_buffer,  						/* Peripheral destination address */
					READ_SIZE,    								/* Number of bytes */
					0);               							/* Link to same descriptor */

	/* LDMA transfer configuration for LEUART */
	const LDMA_TransferCfg_t periTransferRx =
			LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_LEUART0_RXDATAV);

	xfer.xfer.dstInc  = ldmaCtrlDstIncOne;

	/* Set the bit to trigger the DMA Done Interrupt */
	xfer.xfer.doneIfs = 1;

	/* LDMA initialization mode definition */
	LDMA_Init_t init = LDMA_INIT_DEFAULT;

	/* Enable the interrupts */
	LDMA->IEN = 0x01;
	NVIC_EnableIRQ(LDMA_IRQn);

	/* LDMA initialization */
	LDMA_Init(&init);
	LDMA_StartTransfer(0, (LDMA_TransferCfg_t *)&periTransferRx, &xfer);

}

void LEUART0_IRQHandler(void)
{
	uint32_t 	rx_data = 0;
#ifdef ENABLE_TX_FOR_LOOPBACK
	uint32_t 	tx_data = 0xAA;
#endif

	CORE_DECLARE_IRQ_STATE;
	CORE_ENTER_ATOMIC();

	/* RXDATAV Interrupt */
	if ((LEUART0->IF & LEUART_IF_RXDATAV) == LEUART_IF_RXDATAV)
	{
		LEUART_IntClear(LEUART0, LEUART_IF_RXDATAV);

		if (( LEUART0->IF & LEUART_IF_RXDATAV) == LEUART_IF_RXDATAV)
		{
			if (leuart_rx_data_count != 4)
			{
				rx_data = LEUART0->RXDATA;
				leuart_rx_data_count++;
			}
			else
			{
				leuart_rx_data_count = 0;
				is_leuart_data_available = true;
			}
		}
	}

#ifdef ENABLE_TX_FOR_LOOPBACK
	/* TXBL Interrupt */
	if ((LEUART0->IF & LEUART_IF_TXBL) == LEUART_IF_TXBL)
	{
		LEUART_IntClear(LEUART0, LEUART_IF_TXBL);

		LEUART0->TXDATA = tx_data;
		while((LEUART0->IF & LEUART_IF_TXC) == 0);

		if (( LEUART0->IF & LEUART_IF_RXDATAV) == LEUART_IF_RXDATAV)
			rx_data = LEUART0->RXDATA;
	}
#endif

	CORE_EXIT_ATOMIC();
}


/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
