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

/* Own header */
#include "app.h"
#include "MotorDriver.h"

bd_addr slave_bluetooth_addr;
uint8_t slave_bluetooth_addr_type = 0;
uint8_t slave_conn_handle = 0;
uint8 connection;
uint16 interval;
uint16 latency;
uint16 timeout;
uint16 result;
uint8 security_mode;
uint16 txsize;

uint8array temp_service_uuid;
uint8array temp_char_uuid;
uint8array env_service_uuid;
uint8array humidity_char_uuid;
uint8_t slave_addr_to_match[] = {0x00, 0x0B, 0x57, 0x05, 0xE0, 0xD7};
bool slave_addr_match = false;

device_role e_dev_role = DEVICE_ROLE_INVALID;

uint32_t first_time_advertisement = 1;

int master_role_led_status = 0;
int slave_role_led_status = 0;
int connected = 0;

#if 0
uint8_t temp_service_uuid_string[5];
#endif

bool service_handling_in_progress = false;

static connState state = eStateDisconnected;

uint8 humid_rcvd_data[10];
uint8 temp_rcvd_data[20];

#define ENABLE_MASTER_ROLE
//#define ENABLE_SLAVE_ROLE

#ifdef ENABLE_SLAVE_ROLE
#define MEASURE_ONBOARD_HUMIDITY
#define MEASURE_ONBOARD_TEMPERATURE
#endif

#ifdef ENABLE_MASTER_ROLE
//#define READ_FLEX_SENSOR_DATA
//#define MEASURE_TEMPERATURE
#define MEASURE_HUMIDITY
#endif

#if defined(ENABLE_MASTER_ROLE) && defined(ENABLE_SLAVE_ROLE)
#define ENABLE_PERIODIC_ROLE_REVERSAL
#define DEV_SLAVE_PERIOD	327680*2			/* Defines the period for which the blue gecko remains in master/slave mode */
#endif

#if defined(MEASURE_TEMPERATURE)
uint32 slave_temp_service_handle = 0;
uint8array slave_temp_service_uuid;
uint16 slave_temp_characteristic_handle = 0;
uint8 slave_temp_characteristic_connection = 0;
uint8 slave_temp_characteristic_properties = 0;
uint8array slave_temp_characteristic_uuid;
#endif

#if defined(MEASURE_HUMIDITY)
uint32 slave_humid_service_handle = 0;
uint8array slave_humid_service_uuid;
uint16 slave_humid_characteristic_handle = 0;
uint8 slave_humid_characteristic_connection = 0;
uint8 slave_humid_characteristic_properties = 0;
uint8array slave_humid_characteristic_uuid;
#endif

int device_role_in_conn = -1;
bool device_is_slave = false;
bool slave_initialized = false;
bool master_initialized = false;
int gatt_proc_completed_res = -1;

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
  //appUiInit(devId);

  /* Hardware initialization. Initializes temperature sensor. */
  //appHwInit();

  /* Initialize services */
  //htmInit();
}

#if 0
/***********************************************************************************************//**
 * \brief Function that decodes the advertising data
 **************************************************************************************************/
static bool decode_adv_data(uint8array* data, const uint8_t* uuid )
{

  uint8_t i = 0;

  while ( i < (data->len - 1) )
  {
    uint8_t ad_len  = data->data[i];
    uint8_t ad_type = data->data[i+1];

    if ( ( ad_type == 0x06 || ad_type == 0x07 ) && memcmp(uuid, &data->data[i+2], 2) )
    {
      return true;
    } else {
      i += ad_len + 1;
    }
  }

  return false;

}

static void format_bluetooth_address(uint8_t *pBluetoothAddr, uint8_t *pBluetoothAddrSwapped)
{
	uint8_t bluetoothAddr[6];

	if (NULL != pBluetoothAddr)
	{
		bluetoothAddr[0] = *(pBluetoothAddr+5);
		bluetoothAddr[1] = *(pBluetoothAddr+4);
		bluetoothAddr[2] = *(pBluetoothAddr+3);
		bluetoothAddr[3] = *(pBluetoothAddr+2);
		bluetoothAddr[4] = *(pBluetoothAddr+1);
		bluetoothAddr[5] = *(pBluetoothAddr);
	}

	if (pBluetoothAddrSwapped != NULL)
	{
		memcpy(pBluetoothAddrSwapped, bluetoothAddr, 6);
	}

	return;
}
#endif

#if defined(ENABLE_SLAVE_ROLE) || defined(ENABLE_MASTER_ROLE)
void manage_master_slave_role_led(void)
{
	if (e_dev_role == DEVICE_ROLE_MASTER)
	{
		if (master_role_led_status == 0)
		{
			GPIO_PinOutSet(gpioPortF, 5);
			master_role_led_status = 1;
			gecko_cmd_hardware_set_soft_timer(3276, MASTER_ROLE_TIMER, false);
		}
		else
		{
			GPIO_PinOutClear(gpioPortF, 5);
			master_role_led_status = 0;
			gecko_cmd_hardware_set_soft_timer(32768, MASTER_ROLE_TIMER, false);
		}
	}
	else if (e_dev_role == DEVICE_ROLE_SLAVE)
	{
		if (slave_role_led_status == 0)
		{
			GPIO_PinOutSet(gpioPortF, 4);
			slave_role_led_status = 1;
			gecko_cmd_hardware_set_soft_timer(3276, SLAVE_ROLE_TIMER, false);
		}
		else
		{
			GPIO_PinOutClear(gpioPortF, 4);
			slave_role_led_status = 0;
			gecko_cmd_hardware_set_soft_timer(32768, SLAVE_ROLE_TIMER, false);
		}
	}
	else
	{
		/* Do Nothing */
	}
}
#endif

/***********************************************************************************************//**
 * \brief Event handler function
 * @param[in] evt Event pointer
 **************************************************************************************************/
void appHandleEvents(struct gecko_cmd_packet *evt)
{
	/* Flag for indicating DFU Reset must be performed */
	static uint8_t boot_to_dfu = 0;

#if defined(ENABLE_SLAVE_ROLE) && defined(MEASURE_ONBOARD_HUMIDITY)
	int humidityData; /* Humidity data from the sensor */
#endif

	switch (BGLIB_MSG_ID(evt->header)) {

	/* Boot event and connection closed event */
	case gecko_evt_system_boot_id:

		/* Initialize app */
		appInit(); /* App initialization */
		//htmInit(); /* Health thermometer initialization */
		//advSetup(); /* Advertisement initialization */

		/* This code configures this blue gecko as a master.
		 * The master will try to discover all the primary services and characteristics
		 * supported by a slave device (another blue gecko device ) */

		/* LED0 - Power LED */
		//GPIO_PinModeSet(gpioPortF, 4, gpioModePushPull, 0);

#ifdef ENABLE_SLAVE_ROLE
		e_dev_role = DEVICE_ROLE_SLAVE;

		gecko_cmd_hardware_set_soft_timer(32768, SLAVE_ROLE_TIMER, false);

		/* Hardware initialization. Initializes temperature sensor. */
		appHwInit();
#endif

#ifdef ENABLE_MASTER_ROLE
		e_dev_role = DEVICE_ROLE_MASTER;
		gecko_cmd_hardware_set_soft_timer(32768, MASTER_ROLE_TIMER, false);
#endif

#ifdef ENABLE_PERIODIC_ROLE_REVERSAL
		gecko_cmd_hardware_set_soft_timer(49152, MASTER_SLAVE_RR_TIMER, true); /* Timer will expire in 1.5 seconds */
#endif

#ifdef ENABLE_SLAVE_ROLE
		/* LED0 - Power LED */
		GPIO_PinModeSet(gpioPortF, 4, gpioModePushPull, 0);
#endif

#ifdef ENABLE_MASTER_ROLE
		/* LED1 - Alarm LED */
		GPIO_PinModeSet(gpioPortF, 5, gpioModePushPull, 0);
#endif

		/* For the first 1.5 seconds, the device will be in master mode and
		 * and will scan for advertising packets from any slave */

		/* start the GAP discovery procedure to scan for advertising devices */
		/* Configure the scanning parameters */
		/* scan_interval - 800
		 * scan_window - 800
		 * passive scan mode */
		gecko_cmd_le_gap_set_scan_parameters(100, 100, 1);

		/* Start the GAP discovery */
		gecko_cmd_le_gap_discover(le_gap_general_discoverable); /* Discover using general discoverable mode */
		//gecko_cmd_le_gap_discover(le_gap_discover_limited); /* Discover only limited devices */
		state = eStateScanning;
		/* TODO: Try adding a sleep here just to have a slight delay for the discovery */
		gecko_sleep_for_ms(100);

		service_handling_in_progress = false;

		break;

		/* This event is generated when a connected client has either
		 * 1) changed a Characteristic Client Configuration, meaning that they have enabled
		 * or disabled Notifications or Indications, or
		 * 2) sent a confirmation upon a successful reception of the indication. */

	case gecko_evt_le_gap_scan_response_id:
		/* This event is triggered in response to gecko_cmd_le_gap_discover command */
		/* It reports any advertisement or scan response packet
		 * that is received by the device's radio while in scanning mode */
		/* TODO: Can add a check for bluetooth MAC address to identify the correct slave device */

#if 0
		sprintf((char *)temp_service_uuid_string, "%02x%02x", temp_service_uuid[0], temp_service_uuid[1]);
		//if ( state == eStateScanning && decode_adv_data( &evt->data.evt_le_gap_scan_response.data, temp_service_uuid_string ) )
		{
			if ((evt->data.evt_le_gap_scan_response.packet_type == 0x00)
					|| (evt->data.evt_le_gap_scan_response.packet_type == 0x04))
			{
				state = eStateScanning;
				if (NULL != slave_bluetooth_addr.addr
						&& NULL != evt->data.evt_le_gap_scan_response.address.addr)
					memcpy(slave_bluetooth_addr.addr, evt->data.evt_le_gap_scan_response.address.addr, sizeof(bd_addr));

				gecko_cmd_le_gap_end_procedure();
			}
		}

		//if ( state == eStateScanning && decode_adv_data( &evt->data.evt_le_gap_scan_response.data, temp_service_uuid_string ) )
		{
			if (NULL != slave_bluetooth_addr.addr
					&& NULL != evt->data.evt_le_gap_scan_response.address.addr)
			{
				memcpy(slave_bluetooth_addr.addr, evt->data.evt_le_gap_scan_response.address.addr, sizeof(bd_addr));

				slave_bluetooth_addr_type = evt->data.evt_le_gap_scan_response.address_type;
				gecko_cmd_le_gap_open(slave_bluetooth_addr, slave_bluetooth_addr_type);
			}
#else
			/* Device is set in active scan mode, so checking for scan response */
			//if (evt->data.evt_le_gap_scan_response.packet_type == 0x04 && state != eStateFindService)
			if (evt->data.evt_le_gap_scan_response.packet_type == 0x04)
			{
				state = eStateScanning;
				if (NULL != slave_bluetooth_addr.addr
						&& NULL != evt->data.evt_le_gap_scan_response.address.addr)
				{
					memcpy(slave_bluetooth_addr.addr, evt->data.evt_le_gap_scan_response.address.addr, sizeof(bd_addr));
					//format_bluetooth_address(slave_bluetooth_addr.addr, slave_bluetooth_addr.addr);
				}

				gecko_cmd_le_gap_end_procedure();

				slave_bluetooth_addr_type = evt->data.evt_le_gap_scan_response.address_type;
				gecko_cmd_le_gap_open(slave_bluetooth_addr, slave_bluetooth_addr_type);
			}
#endif
			break;

			/* Connection opened event */
	case gecko_evt_le_connection_opened_id:
		/* Call advertisement.c connection started callback */
		//advConnectionStarted();
		/* Connection Open Event */
		connected = 1;

		device_role_in_conn = evt->data.evt_le_connection_opened.master;

		if (device_role_in_conn == 0)
		{
			/* Blue Gecko device is slave in this connection */
			device_is_slave = true;
		}
		else
		{
			/* Blue Gecko device is master in this connection */
			device_is_slave = false;
		}

		if (e_dev_role == DEVICE_ROLE_SLAVE)
		{
#ifdef ENABLE_SLAVE_ROLE
			slave_connection = evt->data.evt_le_connection_opened.connection;
#endif

#ifdef MEASURE_ONBOARD_HUMIDITY
			gecko_cmd_hardware_set_soft_timer(32768, HUMIDITY_TIMER, false);
#endif

#ifdef MEASURE_ONBOARD_TEMPERATURE
			gecko_cmd_hardware_set_soft_timer(32768, TEMP_TIMER, true);
#endif

		}
		else
		{
			//if (NULL != evt->data.evt_le_connection_opened.address.addr && NULL != slave_bluetooth_addr.addr && state == eStateScanning)
			if (NULL != evt->data.evt_le_connection_opened.address.addr && NULL != slave_bluetooth_addr.addr)
			{
				if (memcmp(evt->data.evt_le_connection_opened.address.addr,
						slave_bluetooth_addr.addr, sizeof(bd_addr)) == 0)
				{
					slave_conn_handle = evt->data.evt_le_connection_opened.connection;

					/* Discover all the primary services defined in the
					 * GATT database for the slave identified by the
					 * newly acquired connection handle */

#ifdef MEASURE_TEMPERATURE
					/* Discover temperature measurement service -
					 * with temperature value and type as characteristic */
					temp_service_uuid.len = 2;
					temp_service_uuid.data[0] = 0x09;
					temp_service_uuid.data[1] = 0x18;

					struct gecko_msg_gatt_discover_primary_services_by_uuid_rsp_t *disc_temp_pri_serv_rsp =
							gecko_cmd_gatt_discover_primary_services_by_uuid(slave_conn_handle, temp_service_uuid.len,
									temp_service_uuid.data);
#endif

#ifdef MEASURE_HUMIDITY
					//service_handling_in_progress = true;

					/* Discover environmental sensing service - with humidity info as characteristic */
					env_service_uuid.len = 2;
					env_service_uuid.data[0] = 0x1A;
					env_service_uuid.data[1] = 0x18;
					struct gecko_msg_gatt_discover_primary_services_by_uuid_rsp_t *disc_env_pri_serv_rsp =
							gecko_cmd_gatt_discover_primary_services_by_uuid(slave_conn_handle, env_service_uuid.len,
									env_service_uuid.data);

					uint16 disc_env_pri_serv_rsp_result = disc_env_pri_serv_rsp->result;
#endif

					//gecko_cmd_gatt_discover_primary_services(slave_conn_handle);
					state = eStateFindService;
				}
			}
		}
		break;

	case gecko_evt_le_connection_parameters_id:
		connection = evt->data.evt_le_connection_parameters.connection;
		latency = evt->data.evt_le_connection_parameters.latency;
		interval = evt->data.evt_le_connection_parameters.interval;
		timeout = evt->data.evt_le_connection_parameters.timeout;
		security_mode = evt->data.evt_le_connection_parameters.security_mode;
		txsize = evt->data.evt_le_connection_parameters.txsize;

		break;

	case gecko_evt_gatt_service_id:
#ifdef MEASURE_TEMPERATURE
		/* This event is triggered when a service is discovered from the remote GATT database */
		if (memcmp(evt->data.evt_gatt_service.uuid.data,temp_service_uuid.data,
				evt->data.evt_gatt_service.uuid.len) == 0)
		{
			/* Temperature service discovered */
			slave_temp_service_handle = evt->data.evt_gatt_service.service;
			service_handling_in_progress = false;
		}
#endif

#ifdef MEASURE_HUMIDITY
		if (memcmp(evt->data.evt_gatt_service.uuid.data,env_service_uuid.data,
				evt->data.evt_gatt_service.uuid.len) == 0)
		{
			/* Humidity Service discovered */
			slave_humid_service_handle = evt->data.evt_gatt_service.service;
			service_handling_in_progress = false;
		}
#endif

		//gecko_cmd_le_gap_end_procedure();

		break;

	case gecko_evt_gatt_procedure_completed_id:
		gatt_proc_completed_res = evt->data.evt_gatt_procedure_completed.result;

		/* This event is triggered when a GATT procedure finished successfully or failed with error */
		if ( state == eStateFindService )
		{
#ifdef MEASURE_TEMPERATURE
			if (slave_temp_service_handle > 0)
			{
				//gecko_cmd_gatt_discover_characteristics(evt->data.evt_gatt_procedure_completed.connection, slave_service_handle);
				state = eStateFindCharacteristic;
				/* Discover humidity measurement characteristic */
				temp_char_uuid.len = 2;
				temp_char_uuid.data[0] = 0x1C;
				temp_char_uuid.data[1] = 0x2A;
				struct gecko_msg_gatt_discover_characteristics_by_uuid_rsp_t *disc_temp_val_char_rsp =
						gecko_cmd_gatt_discover_characteristics_by_uuid(evt->data.evt_gatt_service.connection,
								slave_temp_service_handle, temp_char_uuid.len, temp_char_uuid.data);

				uint16 disc_temp_val_char_rsp_res = disc_temp_val_char_rsp->result;
			}
#endif

#ifdef MEASURE_HUMIDITY
			if (slave_humid_service_handle > 0)
			{
				state = eStateFindCharacteristic;
				/* Discover humidity measurement characteristic */
				humidity_char_uuid.len = 2;
				humidity_char_uuid.data[0] = 0x6F;
				humidity_char_uuid.data[1] = 0x2A;
				struct gecko_msg_gatt_discover_characteristics_by_uuid_rsp_t *disc_humidity_char_rsp =
						gecko_cmd_gatt_discover_characteristics_by_uuid(evt->data.evt_gatt_service.connection,
								slave_humid_service_handle, humidity_char_uuid.len, humidity_char_uuid.data);

				uint16 disc_humidity_char_rsp_res = disc_humidity_char_rsp->result;
			}
#endif

			break;
		}

		if ( state == eStateFindCharacteristic )
		{
#ifdef MEASURE_TEMPERATURE
			if ( slave_temp_characteristic_handle > 0 )
			{
				struct gecko_msg_gatt_set_characteristic_notification_rsp_t* temp_set_notif_rsp =
						gecko_cmd_gatt_set_characteristic_notification(slave_conn_handle,
								slave_temp_characteristic_handle,gatt_notification);

				state = eStateEnableNotif;
			}
#endif

#ifdef MEASURE_HUMIDITY
			if (slave_humid_characteristic_handle > 0)
			{
				struct gecko_msg_gatt_read_characteristic_value_by_uuid_rsp_t* char_read_rsp =
						gecko_cmd_gatt_read_characteristic_value_by_uuid(slave_conn_handle,
								slave_humid_service_handle, humidity_char_uuid.len, humidity_char_uuid.data);

				uint16 char_rd_rsp_res = char_read_rsp->result;

				state = eStateEnableNotif;
			}
#endif
			break;
		}

		if ( state == eStateEnableNotif )
		{
			//notifications enabled -> transparent data mode
			state = eStateDataMode;
			break;
		}

		break;

#if 0
#ifdef MEASURE_TEMPERATURE
	case gecko_evt_gatt_server_attribute_value_id:
		if ( evt->data.evt_gatt_server_attribute_value.attribute == gattdb_temp_measurement)
		{
			uint8array* rdata = &evt->data.evt_gatt_server_attribute_value.value;
			gecko_sleep_for_ms(10);
		}
		break;
#endif
#endif

	case gecko_evt_gatt_characteristic_id:
		/* This event is triggered when a characteristic is discovered from the remote GATT database */
#ifdef MEASURE_TEMPERATURE
		if (memcmp(evt->data.evt_gatt_characteristic.uuid.data, temp_char_uuid.data, temp_char_uuid.len) == 0)
		{
			slave_temp_characteristic_handle = evt->data.evt_gatt_characteristic.characteristic;
		}
#endif

#ifdef MEASURE_HUMIDITY
		if (memcmp(evt->data.evt_gatt_characteristic.uuid.data, humidity_char_uuid.data, humidity_char_uuid.len) == 0)
		{
			slave_humid_characteristic_handle = evt->data.evt_gatt_characteristic.characteristic;
			slave_humid_characteristic_connection = evt->data.evt_gatt_characteristic.connection;
		}
		//gecko_cmd_le_gap_end_procedure();
#endif
		break;

	case gecko_evt_gatt_characteristic_value_id:
		//process receiving data
#ifdef MEASURE_TEMPERATURE
		if (evt->data.evt_gatt_characteristic_value.characteristic == slave_temp_characteristic_handle)
		{
			if (evt->data.evt_gatt_characteristic_value.att_opcode == gatt_handle_value_notification)
			{
				memcpy(temp_rcvd_data, evt->data.evt_gatt_characteristic_value.value.data, evt->data.evt_gatt_characteristic_value.value.len);
				//int temp_data = ((temp_rcvd_data[4] << 24 | temp_rcvd_data[3] << 16 | temp_rcvd_data[2] << 8 | temp_rcvd_data[1]) & 0x00FFFFFF);
				int temp_data = ((temp_rcvd_data[4] << 24 | temp_rcvd_data[3] << 16 | temp_rcvd_data[2] << 8 | temp_rcvd_data[1])~);
				float temp_val = (float)temp_data/1000;

#ifdef READ_FLEX_SENSOR_DATA
				parse_flex_sensor_data(temp_data);
				motor_control();
#endif
			}
		}
#endif

#ifdef MEASURE_HUMIDITY
		if (evt->data.evt_gatt_characteristic_value.characteristic == slave_humid_characteristic_handle)
		{
			memcpy(humid_rcvd_data, evt->data.evt_gatt_characteristic_value.value.data, evt->data.evt_gatt_characteristic_value.value.len);
		}
#endif
		break;

	case gecko_evt_le_connection_closed_id:

		/* Initialize app */
		appInit(); /* App initialization */
		//htmInit(); /* Health thermometer initialization */
		//advSetup(); /* Advertisement initialization */

		/* Enter to DFU OTA mode if needed */
		if (boot_to_dfu) {
			gecko_cmd_system_reset(2);
		}

		connected = 0;

		device_is_slave = false;

		device_role_in_conn = -1;

		if (e_dev_role == DEVICE_ROLE_SLAVE)
		{

#ifdef ENABLE_SLAVE_ROLE
			gecko_cmd_hardware_set_soft_timer(32768, SLAVE_ROLE_TIMER, false);

			/* Restart advertising after client has disconnected */
			gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);
#endif
		}
		else
		{
#ifdef ENABLE_MASTER_ROLE
			gecko_cmd_hardware_set_soft_timer(32768, MASTER_ROLE_TIMER, false);

			/* Start the GAP discovery */
			gecko_cmd_le_gap_discover(le_gap_general_discoverable); /* Discover using general discoverable mode */
#endif
		}
		break;

		/* Software Timer event */
	case gecko_evt_hardware_soft_timer_id:
		/* Check which software timer handle is in question */
		switch (evt->data.evt_hardware_soft_timer.handle) {
		case UI_TIMER: /* App UI Timer (LEDs, Buttons) */
			//appUiTick();
			break;
		case ADV_TIMER: /* Advertisement Timer */
			//advSetup();
			break;
#ifndef FEATURE_IOEXPANDER
		case DISP_POL_INV_TIMER:
			/*Toggle the the EXTCOMIN signal, which prevents building up a DC bias  within the
			 * Sharp memory LCD panel */
			//dispPolarityInvert(0);
			break;
#endif /* FEATURE_IOEXPANDER */
#ifdef ENABLE_PERIODIC_ROLE_REVERSAL
		case MASTER_SLAVE_RR_TIMER:
			if (e_dev_role == DEVICE_ROLE_SLAVE)
			{
				/* Set advertising parameters. 100ms advertisement interval. All channels used.
				 * The first two parameters are minimum and maximum advertising interval, both in
				 * units of (milliseconds * 1.6). The third parameter '7' sets advertising on all channels. */
				gecko_cmd_le_gap_set_adv_parameters(160,160,7);

				/* Start general advertising and enable connections. */
				gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);

				if (device_is_slave == true && connected == 1)
				{
					/* Device is connected to a master (like a cellphone) and
					 * will continue to stay until a disconnect event occurs */
					/* Resetting the timer to come and check after 5 seconds */
					gecko_cmd_hardware_set_soft_timer(DEV_SLAVE_PERIOD/4, MASTER_SLAVE_RR_TIMER, true);
				}
				else
				{
					e_dev_role = DEVICE_ROLE_MASTER;
					gecko_cmd_hardware_set_soft_timer(DEV_SLAVE_PERIOD, MASTER_SLAVE_RR_TIMER, true);
				}
			}
			else if (e_dev_role == DEVICE_ROLE_MASTER)
			{
				/* start the GAP discovery procedure to scan for advertising devices */
				/* Configure the scanning parameters */
				/* scan_interval - 800
				 * scan_window - 800
				 * passive scan mode */
				gecko_cmd_le_gap_set_scan_parameters(100, 100, 1);

				/* Start the GAP discovery */
				gecko_cmd_le_gap_discover(le_gap_general_discoverable); /* Discover using general discoverable mode */

				e_dev_role = DEVICE_ROLE_SLAVE;
				gecko_cmd_hardware_set_soft_timer(DEV_SLAVE_PERIOD, MASTER_SLAVE_RR_TIMER, true);
			}
			else
			{
				/* Do Nothing */
			}
			break;
#endif

		case HUMIDITY_TIMER:
#ifdef MEASURE_ONBOARD_HUMIDITY
			if (e_dev_role == DEVICE_ROLE_SLAVE)
			{
				appHwReadHumidity((uint32_t *)&humidityData);

				htmHumidityMeas.humidity = humidityData/1000;

				//htmHumidityMeas.humidity = 45;

				gecko_cmd_gatt_server_write_attribute_value(gattdb_humidity_measurement,
						0,
						sizeof(struct htmHumidityMeas_t),
						(uint8_t *)&htmHumidityMeas);
			}
#endif
			break;
		case TEMP_TIMER: /* Temperature measurement timer */
#ifdef MEASURE_ONBOARD_TEMPERATURE
			if (e_dev_role == DEVICE_ROLE_SLAVE)
			{
				/* Make a temperature measurement */
				htmTemperatureMeasure();
			}
#endif
			break;
		case SLAVE_ROLE_TIMER:
#ifdef ENABLE_SLAVE_ROLE
			manage_master_slave_role_led();
#endif
			break;
		case MASTER_ROLE_TIMER:
#ifdef ENABLE_MASTER_ROLE
			manage_master_slave_role_led();
#endif
			break;
		default:
			break;
		}
		break;
		/* User write request event. Checks if the user-type OTA Control Characteristic was written.
		 * If written, boots the device into Device Firmware Upgrade (DFU) mode. */
		case gecko_evt_gatt_server_user_write_request_id:
			/* Handle OTA */
			if(evt->data.evt_gatt_server_user_write_request.characteristic==gattdb_ota_control)
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

		case gecko_evt_gatt_server_characteristic_status_id:
#ifdef MEASURE_TEMPERATURE
			/* Check if changed client char config is for the temperature measurement */
			if ((gattdb_temp_measurement == evt->data.evt_gatt_server_attribute_value.attribute)
					&& (evt->data.evt_gatt_server_characteristic_status.status_flags == 0x01)) {
				// Call HTM temperature characteristic status changed callback
				htmTemperatureCharStatusChange(
						evt->data.evt_gatt_server_characteristic_status.connection,
						evt->data.evt_gatt_server_characteristic_status.client_config_flags);
			}
#endif

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


/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
