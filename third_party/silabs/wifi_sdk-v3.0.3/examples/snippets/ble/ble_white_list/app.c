/*******************************************************************************
* @file  app.c
* @brief 
*******************************************************************************
* # License
* <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
*******************************************************************************
*
* The licensor of this software is Silicon Laboratories Inc. Your use of this
* software is governed by the terms of Silicon Labs Master Software License
* Agreement (MSLA) available at
* www.silabs.com/about-us/legal/master-software-license-agreement. This
* software is distributed to you in Source Code format and is governed by the
* sections of the MSLA applicable to Source Code.
*
******************************************************************************/

/**
 * Include files
 * */

//! SL Wi-Fi SDK includes
#include "sl_board_configuration.h"
#include "sl_constants.h"
#include "sl_wifi.h"
#include "sl_wifi_callback_framework.h"
#include "cmsis_os2.h"

//! BLE include file to refer BLE APIs
#include <string.h>

#include "ble_config.h"
#include "rsi_ble_apis.h"
#include "rsi_ble_common_config.h"
#include "rsi_bt_common.h"
#include "rsi_bt_common_apis.h"

//! Common include file
#include "rsi_common_apis.h"

//! Address type of the device to connect
#define RSI_BLE_DEV_ADDR_TYPE LE_PUBLIC_ADDRESS

//! Address of the device to connect. should match with the device in white list
#define RSI_BLE_DEV_ADDR "00:1A:7D:DA:71:48"

//! Address type of the device to connect
#define RSI_BLE_WHITELIST_DEV_ADDR1_TYPE LE_PUBLIC_ADDRESS

//! Address type of the device to connect
#define RSI_BLE_WHITELIST_DEV_ADDR2_TYPE LE_PUBLIC_ADDRESS

//! Address of the device to add in white list
uint8_t ble_whitelist_addr1[6] = { 0x48, 0x71, 0xDA, 0x7D, 0x1A, 0x00 };

//! Address of the device to add in white list
uint8_t ble_whitelist_addr2[6] = { 0xB9, 0x70, 0x80, 0xA7, 0x23, 0x00 };

//! Remote Device Name to connect
#define RSI_REMOTE_DEVICE_NAME "SILABS_DEV"

//! To clear whitlist, If this define is enabled specify the bd address of the device which has to remove from whitelist
#define DELETE_WHITELIST 0

#define RSI_REM_DEV_ADDR_LEN 18

//! Application supported events list
#define RSI_APP_EVENT_ADV_REPORT   0
#define RSI_APP_EVENT_CONNECTED    1
#define RSI_APP_EVENT_DISCONNECTED 2

//! Application global parameters.
static uint8_t remote_addr_type = 0;
static uint8_t remote_name[31];
static uint8_t remote_dev_addr[18]   = { 0 };
static uint8_t remote_dev_bd_addr[6] = { 0 };
static uint8_t device_found          = 0;

static rsi_ble_event_conn_status_t rsi_app_connected_device;
static rsi_ble_event_disconnect_t rsi_app_disconnected_device;
uint8_t str_remote_address[RSI_REM_DEV_ADDR_LEN] = { '\0' };

osSemaphoreId_t ble_main_task_sem;
static volatile uint32_t ble_app_event_map;
static volatile uint32_t ble_app_event_map1;

static const sl_wifi_device_configuration_t config = {
  .boot_option = LOAD_NWP_FW,
  .mac_address = NULL,
  .band        = SL_SI91X_WIFI_BAND_2_4GHZ,
  .region_code = US,
  .boot_config = { .oper_mode = SL_SI91X_CLIENT_MODE,
                   .coex_mode = SL_SI91X_WLAN_BLE_MODE,
#ifdef RSI_M4_INTERFACE
                   .feature_bit_map = (SL_SI91X_FEAT_WPS_DISABLE | RSI_FEATURE_BIT_MAP),
#else
                   .feature_bit_map        = RSI_FEATURE_BIT_MAP,
#endif
#if RSI_TCP_IP_BYPASS
                   .tcp_ip_feature_bit_map = RSI_TCP_IP_FEATURE_BIT_MAP,
#else
                   .tcp_ip_feature_bit_map = (RSI_TCP_IP_FEATURE_BIT_MAP | SL_SI91X_TCP_IP_FEAT_EXTENSION_VALID),
#endif
                   .custom_feature_bit_map = (SL_SI91X_FEAT_CUSTOM_FEAT_EXTENTION_VALID | RSI_CUSTOM_FEATURE_BIT_MAP),
                   .ext_custom_feature_bit_map = (
#ifdef CHIP_917
                     (RSI_EXT_CUSTOM_FEATURE_BIT_MAP)
#else //defaults
#ifdef RSI_M4_INTERFACE
                     (SL_SI91X_EXT_FEAT_256K_MODE | RSI_EXT_CUSTOM_FEATURE_BIT_MAP)
#else
                     (SL_SI91X_EXT_FEAT_384K_MODE | RSI_EXT_CUSTOM_FEATURE_BIT_MAP)
#endif
#endif
                     | (SL_SI91X_EXT_FEAT_BT_CUSTOM_FEAT_ENABLE)
#if (defined A2DP_POWER_SAVE_ENABLE)
                     | SL_SI91X_EXT_FEAT_XTAL_CLK_ENABLE(2)
#endif
                       ),
                   .bt_feature_bit_map = (RSI_BT_FEATURE_BITMAP
#if (RSI_BT_GATT_ON_CLASSIC)
                                          | SL_SI91X_BT_ATT_OVER_CLASSIC_ACL /* to support att over classic acl link */
#endif
                                          ),
#ifdef RSI_PROCESS_MAX_RX_DATA
                   .ext_tcp_ip_feature_bit_map = (RSI_EXT_TCPIP_FEATURE_BITMAP | SL_SI91X_CONFIG_FEAT_EXTENTION_VALID
                                                  | SL_SI91X_EXT_TCP_MAX_RECV_LENGTH),
#else
                   .ext_tcp_ip_feature_bit_map = (RSI_EXT_TCPIP_FEATURE_BITMAP | SL_SI91X_CONFIG_FEAT_EXTENTION_VALID),
#endif
                   //!ENABLE_BLE_PROTOCOL in bt_feature_bit_map
                   .ble_feature_bit_map =
                     ((SL_SI91X_BLE_MAX_NBR_SLAVES(RSI_BLE_MAX_NBR_SLAVES)
                       | SL_SI91X_BLE_MAX_NBR_MASTERS(RSI_BLE_MAX_NBR_MASTERS)
                       | SL_SI91X_BLE_MAX_NBR_ATT_SERV(RSI_BLE_MAX_NBR_ATT_SERV)
                       | SL_SI91X_BLE_MAX_NBR_ATT_REC(RSI_BLE_MAX_NBR_ATT_REC))
                      | SL_SI91X_FEAT_BLE_CUSTOM_FEAT_EXTENTION_VALID | SL_SI91X_BLE_PWR_INX(RSI_BLE_PWR_INX)
                      | SL_SI91X_BLE_PWR_SAVE_OPTIONS(RSI_BLE_PWR_SAVE_OPTIONS)
                      | SL_SI91X_916_BLE_COMPATIBLE_FEAT_ENABLE
#if RSI_BLE_GATT_ASYNC_ENABLE
                      | SL_SI91X_BLE_GATT_ASYNC_ENABLE
#endif
                      ),

                   .ble_ext_feature_bit_map =
                     ((SL_SI91X_BLE_NUM_CONN_EVENTS(RSI_BLE_NUM_CONN_EVENTS)
                       | SL_SI91X_BLE_NUM_REC_BYTES(RSI_BLE_NUM_REC_BYTES))
#if RSI_BLE_INDICATE_CONFIRMATION_FROM_HOST
                      | SL_SI91X_BLE_INDICATE_CONFIRMATION_FROM_HOST //indication response from app
#endif
#if RSI_BLE_MTU_EXCHANGE_FROM_HOST
                      | SL_SI91X_BLE_MTU_EXCHANGE_FROM_HOST //MTU Exchange request initiation from app
#endif
#if RSI_BLE_SET_SCAN_RESP_DATA_FROM_HOST
                      | (SL_SI91X_BLE_SET_SCAN_RESP_DATA_FROM_HOST) //Set SCAN Resp Data from app
#endif
#if RSI_BLE_DISABLE_CODED_PHY_FROM_HOST
                      | (SL_SI91X_BLE_DISABLE_CODED_PHY_FROM_HOST) //Disable Coded PHY from app
#endif
#if BLE_SIMPLE_GATT
                      | SL_SI91X_BLE_GATT_INIT
#endif
                      ),
                   .config_feature_bit_map = (SL_SI91X_FEAT_SLEEP_GPIO_SEL_BITMAP | RSI_CONFIG_FEATURE_BITMAP) }
};

const osThreadAttr_t thread_attributes = {
  .name       = "application_thread",
  .attr_bits  = 0,
  .cb_mem     = 0,
  .cb_size    = 0,
  .stack_mem  = 0,
  .stack_size = 3072,
  .priority   = 0,
  .tz_module  = 0,
  .reserved   = 0,
};
/*==============================================*/
/**
 * @fn         rsi_ble_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used during BLE initialization.
 */
static void rsi_ble_app_init_events()
{
  ble_app_event_map  = 0;
  ble_app_event_map1 = 0;
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_set_event
 * @brief      set the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to set/raise the specific event.
 */
void rsi_ble_app_set_event(uint32_t event_num)
{

  if (event_num < 32) {
    ble_app_event_map |= BIT(event_num);
  } else {
    ble_app_event_map1 |= BIT((event_num - 32));
  }

  if (ble_main_task_sem) {
    osSemaphoreRelease(ble_main_task_sem);
  }

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_clear_event
 * @brief      clears the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to clear the specific event.
 */
static void rsi_ble_app_clear_event(uint32_t event_num)
{

  if (event_num < 32) {
    ble_app_event_map &= ~BIT(event_num);
  } else {
    ble_app_event_map1 &= ~BIT((event_num - 32));
  }

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_get_event
 * @brief      returns the first set event based on priority
 * @param[in]  none.
 * @return     int32_t
 *             > 0  = event number
 *             -1   = not received any event
 * @section description
 * This function returns the highest priority event among all the set events
 */
static int32_t rsi_ble_app_get_event(void)
{
  uint32_t ix;

  for (ix = 0; ix < 64; ix++) {
    if (ix < 32) {
      if (ble_app_event_map & (1 << ix)) {
        return ix;
      }
    } else {
      if (ble_app_event_map1 & (1 << (ix - 32))) {
        return ix;
      }
    }
  }

  return (-1);
}

/*==============================================*/
/**
 * @fn         rsi_ble_whitelist_on_adv_report_event
 * @brief      invoked when advertise report event is received
 * @param[in]  adv_report, pointer to the received advertising report
 * @return     none.
 * @section description
 * This callback function updates the scanned remote devices list
 */
void rsi_ble_whitelist_on_adv_report_event(rsi_ble_event_adv_report_t *adv_report)
{
  if (device_found == 1) {
    return;
  }

  memset(remote_name, 0, 31);
  BT_LE_ADPacketExtract(remote_name, adv_report->adv_data, adv_report->adv_data_len);

  remote_addr_type = adv_report->dev_addr_type;
  rsi_6byte_dev_address_to_ascii(remote_dev_addr, (uint8_t *)adv_report->dev_addr);
  memcpy(remote_dev_bd_addr, (uint8_t *)adv_report->dev_addr, 6);
  if (((device_found == 0) && ((strcmp((const char *)remote_name, (const char *)RSI_REMOTE_DEVICE_NAME))) == 0)
      || (((remote_addr_type == RSI_BLE_DEV_ADDR_TYPE)
           && ((strcmp((const char *)remote_dev_addr, (const char *)RSI_BLE_DEV_ADDR) == 0))))) {
    device_found = 1;
    rsi_ble_app_set_event(RSI_APP_EVENT_ADV_REPORT);
  }
}

/*==============================================*/
/**
 * @fn         rsi_ble_whitelist_on_enhance_conn_status_event
 * @brief      invoked when enhanced connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_whitelist_on_enhance_conn_status_event(rsi_ble_event_enhance_conn_status_t *resp_enh_conn)
{
  rsi_app_connected_device.dev_addr_type = resp_enh_conn->dev_addr_type;
  memcpy(rsi_app_connected_device.dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
  rsi_app_connected_device.status = resp_enh_conn->status;
  rsi_ble_app_set_event(RSI_APP_EVENT_CONNECTED);
}

/*==============================================*/
/**
 * @fn         rsi_ble_whitelist_on_conn_status_event
 * @brief      invoked when connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_whitelist_on_conn_status_event(rsi_ble_event_conn_status_t *resp_conn)
{
  memcpy(&rsi_app_connected_device, resp_conn, sizeof(rsi_ble_event_conn_status_t));

  rsi_ble_app_set_event(RSI_APP_EVENT_CONNECTED);
}

/*==============================================*/
/**
 * @fn         rsi_ble_whitelist_on_disconnect_event
 * @brief      invoked when disconnection event is received
 * @param[in]  resp_disconnect, disconnected remote device information
 * @param[in]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This Callback function indicates disconnected device information and status
 */
void rsi_ble_whitelist_on_disconnect_event(rsi_ble_event_disconnect_t *resp_disconnect, uint16_t reason)
{
  UNUSED_PARAMETER(reason); //This statement is added only to resolve compilation warning, value is unchanged
  memcpy(&rsi_app_disconnected_device, resp_disconnect, sizeof(rsi_ble_event_disconnect_t));
  rsi_6byte_dev_address_to_ascii(str_remote_address, resp_disconnect->dev_addr);
  LOG_PRINT("Dis-connected - str_remote_address : %s \r\n", str_remote_address);

  rsi_ble_app_set_event(RSI_APP_EVENT_DISCONNECTED);
}

/*==============================================*/
/**
 * @fn         ble_whitelist
 * @brief      add the devices in whitelist.
 * @param[in]  none
 * @return    none.
 * @section description
 * This function is used to add the devices in to whitelist.
 */
void ble_whitelist(void *argument)
{
  UNUSED_PARAMETER(argument);
  int32_t status         = 0;
  int32_t temp_event_map = 0;

  status = sl_wifi_init(&config, default_wifi_event_handler);
  if (status != SL_STATUS_OK) {
    LOG_PRINT("\r\nWi-Fi Initialization Failed, Error Code : 0x%lX\r\n", status);
    return;
  } else {
    LOG_PRINT("\r\n Wi-Fi Initialization Success\n");
  }

  //! BLE register GAP callbacks
  rsi_ble_gap_register_callbacks(rsi_ble_whitelist_on_adv_report_event,
                                 rsi_ble_whitelist_on_conn_status_event,
                                 rsi_ble_whitelist_on_disconnect_event,
                                 NULL,
                                 NULL,
                                 NULL,
                                 rsi_ble_whitelist_on_enhance_conn_status_event,
                                 NULL,
                                 NULL,
                                 NULL);
  //! create ble main task if ble protocol is selected
  ble_main_task_sem = osSemaphoreNew(1, 0, NULL);

  //! initialize the event map
  rsi_ble_app_init_events();

  //! add device 1 to white list
  status = rsi_ble_addto_whitelist((int8_t *)ble_whitelist_addr1, RSI_BLE_WHITELIST_DEV_ADDR1_TYPE);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\n BLE add to whitelist failed, Error Code : 0x%lx\r\n", status);
    return;
  }

  //! add device 2 to white list
  status = rsi_ble_addto_whitelist((int8_t *)ble_whitelist_addr2, RSI_BLE_WHITELIST_DEV_ADDR2_TYPE);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\n BLE add to whitelist failed, Error Code : 0x%lX\r\n", status);
    return;
  }

#if (DELETE_WHITELIST == 1)
  //! delete the device 2 from whitelist
  status = rsi_ble_deletefrom_whitelist(ble_whitelist_addr2, RSI_BLE_WHITELIST_DEV_ADDR2_TYPE);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\n BLE delete from white list failed, Error Code : 0x%lX\r\n", status);
    return;
  }
#endif

#if (CLEAR_WHITELIST == 1)
  //! start clear
  status = rsi_ble_clear_whitelist();
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\n BLE clear whitelist failed, Error Code : 0x%lX\r\n", status);
    return;
  }
#endif
  //! start scanning
  status = rsi_ble_start_scanning();
  if (status != RSI_SUCCESS) {
    return;
  }
  LOG_PRINT("\n BLE start scanning \n");
  LOG_PRINT("\n BLE remote device name to connect to : %s\n", RSI_REMOTE_DEVICE_NAME);

  while (1) {
    //! Application main loop
    //! checking for received events
    temp_event_map = rsi_ble_app_get_event();
    if (temp_event_map == RSI_FAILURE) {
      //! if events are not received, loop will be continued
      if (ble_main_task_sem) {
        osSemaphoreAcquire(ble_main_task_sem, 0);
      }
      continue;
    }

    switch (temp_event_map) {
      case RSI_APP_EVENT_ADV_REPORT: {
        //! advertise report event.
        LOG_PRINT("\r\n In Advertising Event\r\n");
        //! clear the advertise report event.
        rsi_ble_app_clear_event(RSI_APP_EVENT_ADV_REPORT);

        status = rsi_ble_connect(remote_addr_type, (int8_t *)remote_dev_bd_addr);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\n Connect status: 0x%lX\r\n", status);
        }

      } break;

      case RSI_APP_EVENT_CONNECTED: {
        //! remote device connected event
        rsi_6byte_dev_address_to_ascii(str_remote_address, rsi_app_connected_device.dev_addr);
        LOG_PRINT("\r\n Module connected to address : %s \r\n", str_remote_address);

        //! clear the connected event.
        rsi_ble_app_clear_event(RSI_APP_EVENT_CONNECTED);
      } break;

      case RSI_APP_EVENT_DISCONNECTED: {
        //! remote device disconnected event

        //! clear the disconnected event.
        rsi_ble_app_clear_event(RSI_APP_EVENT_DISCONNECTED);
        LOG_PRINT("\r\n Module got disconnected\r\n");

        device_found = 0;
        //! start scanning
        status = rsi_ble_start_scanning();
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\n start_scanning status: 0x%lX\r\n", status);
        }
        LOG_PRINT("\n BLE start scanning \n");
      } break;
      default: {
      }
    }
  }
}

void app_init(const void *unused)
{
  UNUSED_PARAMETER(unused);

  osThreadNew((osThreadFunc_t)ble_whitelist, NULL, &thread_attributes);
}
