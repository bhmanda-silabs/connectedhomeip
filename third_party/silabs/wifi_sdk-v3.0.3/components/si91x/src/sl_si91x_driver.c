/***************************************************************************/ /**
 * @file
 * @brief
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#include "sl_wifi.h"
#include "sl_si91x_host_interface.h"
#include "sl_wifi_types.h"
#include "sl_si91x_status.h"
#include "sl_si91x_types.h"
#include "sl_si91x_protocol_types.h"
#include "sl_wifi_device.h"
#include "sl_rsi_utility.h"
#include "sl_si91x_driver.h"
#include "cmsis_os2.h"
#include <stdint.h>
#include <string.h>
#ifdef RSI_M4_INTERFACE
#include "rsi_rom_clks.h"
#include "rsi_m4.h"
#include "rsi_wisemcu_hardware_setup.h"
#endif
#ifdef BLE_ENABLE
#include "rsi_common_apis.h"
#endif

#ifdef sockets_FEATURE_REQUIRED
#include "sl_si91x_socket.h"
#endif

#ifndef NULL
#define NULL (void *)0
#endif

#define htole16(x) (x)
#define htole32(x) (x)

#ifndef SL_WIFI_ALLOCATE_COMMAND_BUFFER_WAIT_TIME
#define SL_WIFI_ALLOCATE_COMMAND_BUFFER_WAIT_TIME 1000 // 1 second to wait for a command buffer
#endif

#ifndef SL_WIFI_SET_MAC_COMMAND_TIME_OUT
#define SL_WIFI_SET_MAC_COMMAND_TIME_OUT 30100 // Retrieved from SAPI 1.0
#endif

#define SL_SI91X_INVALID_MODE 0xFFFF

// Enterprise configuration command parameters
/*=======================================================================*/

// Enterprise method ,should be one of among TLS, TTLS, FAST or PEAP

#define SL_EAP_TLS_METHOD  "TLS"
#define SL_EAP_TTLS_METHOD "TTLS"

// This parameter is used to configure the module in Enterprise security mode
#define SL_EAP_INNER_METHOD "\"auth=MSCHAPV2\""

// Private Key Password is required for encrypted private key, format is like "\"12345678\""
#define SL_DEFAULT_PRIVATE_KEY_PASSWORD ""

/*========================================================================*/
// 11ax params
/*========================================================================*/
#define GUARD_INTERVAL 3
// /*=======================================================================*/

#ifdef RSI_M4_INTERFACE
#define TX_PKT_TRANSFER_DONE_INTERRUPT BIT(2)
#define RX_PKT_TRANSFER_DONE_INTERRUPT BIT(1)
sl_status_t sli_si91x_submit_rx_pkt(void);
static sl_status_t sl_si91x_soft_reset(void);
void sli_siwx917_update_system_core_clock(void);
#endif

typedef struct {
  uint16_t packet_id;
  si91x_queue_packet_t *packet;
  void *payload;
} sl_si91x_driver_context_t;

sl_status_t sl_si91x_driver_send_command_packet(uint32_t command,
                                                si91x_queue_type_t comamnd_type,
                                                sl_wifi_buffer_t *buffer,
                                                sl_si91x_wait_period_t wait_period,
                                                void *sdk_context,
                                                sl_wifi_buffer_t **data_buffer);
sl_status_t sl_si91x_driver_send_data_packet(si91x_queue_type_t queue_type,
                                             sl_wifi_buffer_t *buffer,
                                             uint32_t wait_time);
sl_status_t sl_si91x_driver_raw_send_command(uint8_t command,
                                             const void *data,
                                             uint32_t data_length,
                                             uint32_t wait_time);
sl_status_t sl_si91x_allocate_command_buffer(sl_wifi_buffer_t **host_buffer,
                                             void **buffer,
                                             uint32_t requested_buffer_size,
                                             uint32_t wait_duration_ms);
sl_status_t sl_si91x_allocate_data_buffer(sl_wifi_buffer_t **host_buffer,
                                          void **buffer,
                                          uint32_t data_size,
                                          uint32_t wait_duration_ms);

sl_status_t sl_si91x_enable_radio(void);

sl_status_t si91x_bootup_firmware(const uint8_t select_option);

sl_status_t si91x_host_power_cycle(void);
//static void convert_sl_wifi_to_si91x_security_type( sl_wifi_security_t security_type, uint8_t* security, uint8_t* encryption );

// This variable stores the frame status of response packet in case of API executed being failed.
// Note: This will not store the error values of asynchronous events.
sl_wifi_event_handler_t si91x_event_handler = NULL;

sl_wifi_interface_t default_interface;
bool device_initialized                           = false;
bool interface_is_up[SL_WIFI_MAX_INTERFACE_INDEX] = { false, false, false, false };
bool bg_enabled                                   = false;
static uint16_t packet_id[SI91X_CMD_MAX]          = { 0 };

//! Currently, initialized_opermode is used only to handle concurrent mode using sl_net_init()
static uint16_t initialized_opermode = SL_SI91X_INVALID_MODE;

sl_si91x_performance_profile_t performance_profile;

#ifdef BLE_ENABLE
//! Memory length for driver
#define GLOBAL_BUFF_LEN 1500

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN] = { 0 };
#endif

const sl_wifi_scan_configuration_t default_wifi_scan_configuration = { .type  = SL_WIFI_SCAN_TYPE_ACTIVE,
                                                                       .flags = 0,
                                                                       .periodic_scan_interval = 0,
                                                                       .channel_bitmap_2g4     = 0xFFFF,
                                                                       .channel_bitmap_5g      = { 0xFFFFFFFF,
                                                                                                   0xFFFFFFFF,
                                                                                                   0xFFFFFFFF,
                                                                                                   0xFFFFFFFF,
                                                                                                   0xFFFFFFFF,
                                                                                                   0xFFFFFFFF,
                                                                                                   0xFFFFFFFF,
                                                                                                   0xFFFFFFFF } };

const sl_wifi_ap_configuration_t default_wifi_ap_configuration = {
  .ssid                = { .value = "SILICON_LABS_AP", .length = sizeof("SILICON_LABS_AP") - 1 },
  .security            = SL_WIFI_WPA2,
  .encryption          = SL_WIFI_CCMP_ENCRYPTION,
  .channel             = { .channel = 11 },
  .rate_protocol       = SL_WIFI_RATE_PROTOCOL_AUTO,
  .options             = 0,
  .credential_id       = 2, //SL_NET_DEFAULT_WIFI_AP_CREDENTIAL_ID,
  .keepalive_type      = SI91X_AP_NULL_BASED_KEEP_ALIVE,
  .beacon_interval     = 100,
  .client_idle_timeout = 120000,
  .dtim_beacon_count   = 4,
  .maximum_clients     = 4
};

const si91x_set_region_ap_request_t default_US_region_2_4GHZ_configurations = {
  .set_region_code_from_user_cmd = SET_REGION_CODE_FROM_USER,
  .country_code                  = "US ",
  .no_of_rules                   = 1,
  .channel_info[0]               = { .first_channel = 1, .no_of_channels = 11, .max_tx_power = 27 }
};

const si91x_set_region_ap_request_t default_US_region_5GHZ_configurations = {
  .set_region_code_from_user_cmd = SET_REGION_CODE_FROM_USER,
  .country_code                  = "US ",
  .no_of_rules                   = 5,
  .channel_info[0]               = { .first_channel = 36, .no_of_channels = 4, .max_tx_power = 16 },
  .channel_info[1]               = { .first_channel = 52, .no_of_channels = 4, .max_tx_power = 23 },
  .channel_info[2]               = { .first_channel = 100, .no_of_channels = 5, .max_tx_power = 23 },
  .channel_info[3]               = { .first_channel = 132, .no_of_channels = 3, .max_tx_power = 23 },
  .channel_info[4]               = { .first_channel = 149, .no_of_channels = 5, .max_tx_power = 29 }
};

const si91x_set_region_ap_request_t default_EU_region_2_4GHZ_configurations = {
  .set_region_code_from_user_cmd = SET_REGION_CODE_FROM_USER,
  .country_code                  = "EU ",
  .no_of_rules                   = 1,
  .channel_info[0]               = { .first_channel = 1, .no_of_channels = 13, .max_tx_power = 20 }
};

const si91x_set_region_ap_request_t default_EU_region_5GHZ_configurations = {
  .set_region_code_from_user_cmd = SET_REGION_CODE_FROM_USER,
  .country_code                  = "EU ",
  .no_of_rules                   = 3,
  .channel_info[0]               = { .first_channel = 36, .no_of_channels = 4, .max_tx_power = 23 },
  .channel_info[1]               = { .first_channel = 52, .no_of_channels = 4, .max_tx_power = 23 },
  .channel_info[2]               = { .first_channel = 100, .no_of_channels = 11, .max_tx_power = 30 }
};

const si91x_set_region_ap_request_t default_JP_region_2_4GHZ_configurations = {
  .set_region_code_from_user_cmd = SET_REGION_CODE_FROM_USER,
  .country_code                  = "JP ",
  .no_of_rules                   = 1,
  .channel_info[0]               = { .first_channel = 1, .no_of_channels = 14, .max_tx_power = 20 }
};

const si91x_set_region_ap_request_t default_JP_region_5GHZ_configurations = {
  .set_region_code_from_user_cmd = SET_REGION_CODE_FROM_USER,
  .country_code                  = "JP ",
  .no_of_rules                   = 3,
  .channel_info[0]               = { .first_channel = 36, .no_of_channels = 4, .max_tx_power = 20 },
  .channel_info[1]               = { .first_channel = 52, .no_of_channels = 4, .max_tx_power = 20 },
  .channel_info[2]               = { .first_channel = 100, .no_of_channels = 11, .max_tx_power = 30 }
};

const si91x_set_region_ap_request_t default_KR_region_2_4GHZ_configurations = {
  .set_region_code_from_user_cmd = SET_REGION_CODE_FROM_USER,
  .country_code                  = "KR ",
  .no_of_rules                   = 1,
  .channel_info[0]               = { .first_channel = 1, .no_of_channels = 13, .max_tx_power = 27 }
};

const si91x_set_region_ap_request_t default_KR_region_5GHZ_configurations = {
  .set_region_code_from_user_cmd = SET_REGION_CODE_FROM_USER,
  .country_code                  = "KR ",
  .no_of_rules                   = 1,
  .channel_info[0]               = { .first_channel = 149, .no_of_channels = 4, .max_tx_power = 29 }
};

const si91x_set_region_ap_request_t default_SG_region_2_4GHZ_configurations = {
  .set_region_code_from_user_cmd = SET_REGION_CODE_FROM_USER,
  .country_code                  = "SG ",
  .no_of_rules                   = 1,
  .channel_info[0]               = { .first_channel = 1, .no_of_channels = 13, .max_tx_power = 27 }
};

const si91x_set_region_ap_request_t default_SG_region_5GHZ_configurations = {
  .set_region_code_from_user_cmd = SET_REGION_CODE_FROM_USER,
  .country_code                  = "SG ",
  .no_of_rules                   = 5,
  .channel_info[0]               = { .first_channel = 36, .no_of_channels = 4, .max_tx_power = 16 },
  .channel_info[1]               = { .first_channel = 52, .no_of_channels = 4, .max_tx_power = 23 },
  .channel_info[2]               = { .first_channel = 100, .no_of_channels = 5, .max_tx_power = 23 },
  .channel_info[3]               = { .first_channel = 132, .no_of_channels = 3, .max_tx_power = 23 },
  .channel_info[4]               = { .first_channel = 149, .no_of_channels = 4, .max_tx_power = 29 }
};

void si91x_default_event_handler(uint32_t event, void *data, void *arg)
{
  UNUSED_PARAMETER(event);
  UNUSED_PARAMETER(data);
  UNUSED_PARAMETER(arg);
  // Handle things
}

sl_status_t sl_si91x_driver_init(const sl_wifi_device_configuration_t *config, sl_wifi_event_handler_t event_handler)
{
  sl_status_t status;
  const uint8_t select_option = config->boot_option; //LOAD_NWP_FW;

  if (config->boot_config.oper_mode == SL_SI91X_ACCESS_POINT_MODE) {
    default_interface = SL_WIFI_AP_INTERFACE;
  } else {
    default_interface = SL_WIFI_CLIENT_INTERFACE;
  }

  if (config->band == SL_SI91X_WIFI_BAND_5GHZ) {
    default_interface |= SL_WIFI_5GHZ_INTERFACE;
  } else {
    default_interface |= SL_WIFI_2_4GHZ_INTERFACE;
  }

  si91x_event_handler = event_handler;

  if (device_initialized) {
    if (initialized_opermode == SL_SI91X_CONCURRENT_MODE) {
      return (initialized_opermode == config->boot_config.oper_mode) ? SL_STATUS_OK : SL_STATUS_WIFI_INVALID_OPERMODE;
    }
    return SL_STATUS_ALREADY_INITIALIZED;
  }
#ifdef BLE_ENABLE
  int32_t rsi_status = 0;
  rsi_status         = rsi_ble_driver_init(global_buf, GLOBAL_BUFF_LEN);
  if ((rsi_status < 0) || (rsi_status > GLOBAL_BUFF_LEN)) {
    return SL_STATUS_FAIL;
  }
#endif

#ifndef ipv6_FEATURE_REQUIRED
  uint32_t *tcp_ip_feature_bit_map = (uint32_t *)&(config->boot_config.tcp_ip_feature_bit_map);

  // check if the module network stack is in IPV6 mode.
  if (*tcp_ip_feature_bit_map & (SI91X_IPV6_MODE)) {
    return SL_STATUS_NOT_AVAILABLE;
  }
#endif

  status = si91x_host_init();
  VERIFY_STATUS_AND_RETURN(status);

  status = si91x_host_power_cycle();
  VERIFY_STATUS_AND_RETURN(status);

  // si91x_bus_init() will be implemented for all available buses
  status = si91x_bus_init();
  VERIFY_STATUS_AND_RETURN(status);

#ifdef RSI_M4_INTERFACE
  // firmware bootup is require only for the first time, no need to do it again if we call init after deinit
  static bool is_bootup_firmware_required = true;
  if (is_bootup_firmware_required) {
    status = si91x_bootup_firmware(select_option);
    VERIFY_STATUS_AND_RETURN(status);
    is_bootup_firmware_required = false;
  } else {
    sli_m4_ta_interrupt_init();
    sli_si91x_submit_rx_pkt();
  }
#else
  status = si91x_bootup_firmware(select_option);
  VERIFY_STATUS_AND_RETURN(status);
#endif

  // Enable high speed bus on the device and the host
  status = si91x_bus_enable_high_speed();
  VERIFY_STATUS_AND_RETURN(status);
  si91x_host_enable_high_speed_bus();
  si91x_host_enable_bus_interrupt();

// Wait for card ready command response
#ifdef RSI_M4_INTERFACE
  // TA would not send card ready command response, if we call init after deinit

  if (get_card_ready_required()) {
    uint32_t events = si91x_host_wait_for_event(NCP_HOST_COMMON_RESPONSE_EVENT, 3000);
    if (!(events & NCP_HOST_COMMON_RESPONSE_EVENT)) {
      return SL_STATUS_CARD_READY_TIMEOUT;
    }
    set_card_ready_required(false);
  }
#else
  uint32_t events = si91x_host_wait_for_event(NCP_HOST_COMMON_RESPONSE_EVENT, 3000);
  if (!(events & NCP_HOST_COMMON_RESPONSE_EVENT)) {
    return SL_STATUS_CARD_READY_TIMEOUT;
  }
#endif

#ifdef RSI_M4_INTERFACE
  sl_si91x_hardware_setup();
#endif

  status = sl_si91x_driver_send_command(RSI_COMMON_REQ_OPERMODE,
                                        SI91X_COMMON_CMD_QUEUE,
                                        &config->boot_config,
                                        sizeof(si91x_boot_configuration_t),
                                        SL_SI91X_WAIT_FOR_COMMAND_SUCCESS,
                                        NULL,
                                        NULL);
  VERIFY_STATUS_AND_RETURN(status);

  si91x_feature_frame_request feature_frame_request = { .pll_mode        = PLL_MODE,
                                                        .rf_type         = RF_TYPE,
                                                        .wireless_mode   = WIRELESS_MODE,
                                                        .enable_ppp      = ENABLE_PPP,
                                                        .afe_type        = AFE_TYPE,
                                                        .feature_enables = FEATURE_ENABLES };

  status = sl_si91x_driver_send_command(RSI_COMMON_REQ_FEATURE_FRAME,
                                        SI91X_COMMON_CMD_QUEUE,
                                        &feature_frame_request,
                                        sizeof(feature_frame_request),
                                        SL_SI91X_WAIT_FOR(10000),
                                        NULL,
                                        NULL);

  VERIFY_STATUS_AND_RETURN(status);

  // if 16th bit of ext_tcp_ip_feature_bit_map is not set, then firmware auto closes the TCP socket on remote termination.
  save_tcp_auto_close_choice(
    (config->boot_config.ext_tcp_ip_feature_bit_map & SL_SI91X_EXT_TCP_IP_WAIT_FOR_SOCKET_CLOSE) == 0);

  if (config->mac_address != NULL) {
    sl_wifi_set_mac_address(default_interface, config->mac_address);
  }

#ifdef HIGH_EFFICIENCY_PARAMS_SUPPORT
  status = sl_wifi_set_11ax_config(GUARD_INTERVAL);
  VERIFY_STATUS_AND_RETURN(status);
#endif

  status = sl_si91x_driver_send_command(RSI_WLAN_REQ_BAND,
                                        SI91X_WLAN_CMD_QUEUE,
                                        &config->band,
                                        1,
                                        SL_SI91X_WAIT_FOR_COMMAND_SUCCESS,
                                        NULL,
                                        NULL);
  VERIFY_STATUS_AND_RETURN(status);

  status = sl_si91x_driver_send_command(RSI_WLAN_REQ_INIT,
                                        SI91X_WLAN_CMD_QUEUE,
                                        NULL,
                                        0,
                                        SL_SI91X_WAIT_FOR_COMMAND_SUCCESS,
                                        NULL,
                                        NULL);
  VERIFY_STATUS_AND_RETURN(status);

  status = sl_si91x_set_device_region(config->boot_config.oper_mode, config->band, config->region_code);
  VERIFY_STATUS_AND_RETURN(status);

  si91x_config_request_t config_request = { .config_type = CONFIG_RTSTHRESHOLD, .value = RSI_RTS_THRESHOLD };
  status                                = sl_si91x_driver_send_command(RSI_WLAN_REQ_CONFIG,
                                        SI91X_WLAN_CMD_QUEUE,
                                        &config_request,
                                        sizeof(config_request),
                                        SL_SI91X_WAIT_FOR_COMMAND_SUCCESS,
                                        NULL,
                                        NULL);
  VERIFY_STATUS_AND_RETURN(status);

  device_initialized = true;
  save_boot_configuration(&config->boot_config);
  initialized_opermode = config->boot_config.oper_mode;

  if ((config->boot_config.oper_mode == SL_SI91X_CLIENT_MODE)
      || (config->boot_config.oper_mode == SL_SI91X_ENTERPRISE_CLIENT_MODE)
      || (config->boot_config.oper_mode == SL_SI91X_CONCURRENT_MODE)) {
    if (config->band == SL_SI91X_WIFI_BAND_2_4GHZ) {
      interface_is_up[SL_WIFI_CLIENT_2_4GHZ_INTERFACE_INDEX] = true;
    } else if (config->band == SL_SI91X_WIFI_BAND_5GHZ) {
      interface_is_up[SL_WIFI_CLIENT_5GHZ_INTERFACE_INDEX] = true;
    }
  }

  performance_profile.coex_mode = config->boot_config.coex_mode;
  return status;
}

sl_status_t sl_si91x_driver_deinit(void)
{
  sl_status_t status = SL_STATUS_OK;

  if (!device_initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }

#ifdef RSI_M4_INTERFACE
  status = sl_si91x_soft_reset();
  VERIFY_STATUS(status);
  mask_ta_interrupt(TX_PKT_TRANSFER_DONE_INTERRUPT | RX_PKT_TRANSFER_DONE_INTERRUPT);
#endif

#ifdef BLE_ENABLE
  int32_t rsi_status = 0;
  rsi_status         = rsi_ble_driver_deinit();
  if (rsi_status != RSI_SUCCESS) {
    return SL_STATUS_FAIL;
  }
#endif

#if defined(SI91X_NETWORK_OFFLOAD_ENABLED) && defined(sockets_FEATURE_REQUIRED)
  // Free all allocated sockets
  status = sl_si91x_socket_deinit();
  VERIFY_STATUS(status);
#endif

  status = si91x_host_deinit();
  VERIFY_STATUS(status);

  si91x_host_disable_bus_interrupt();

  status = si91x_host_power_cycle();
  VERIFY_STATUS(status);

  si91x_event_handler  = NULL;
  device_initialized   = false;
  initialized_opermode = SL_SI91X_INVALID_MODE;

  // Reset all the interfaces
  memset(interface_is_up, 0, sizeof(interface_is_up));

  return status;
}

sl_status_t sl_si91x_driver_raw_send_command(uint8_t command,
                                             const void *data,
                                             uint32_t data_length,
                                             uint32_t wait_time)
{
  sl_wifi_buffer_t *buffer;
  sl_si91x_packet_t *packet;
  sl_status_t status = SL_STATUS_OK;

  status = sl_si91x_allocate_command_buffer(&buffer,
                                            (void **)&packet,
                                            sizeof(sl_si91x_packet_t) + data_length,
                                            SL_WIFI_ALLOCATE_COMMAND_BUFFER_WAIT_TIME);
  VERIFY_STATUS_AND_RETURN(status);
  if (packet == NULL) {
    return SL_STATUS_ALLOCATION_FAILED;
  }

  memset(packet->desc, 0, sizeof(packet->desc));
  if (data != NULL) {
    memcpy(packet->data, data, data_length);
  }

  // Fill frame type
  packet->length  = data_length & 0xFFF;
  packet->command = command;
  return sl_si91x_driver_send_data_packet(SI91X_WLAN_CMD_QUEUE, buffer, wait_time);
}

sl_status_t sl_si91x_driver_send_socket_data(sl_si91x_socket_send_request_t *request,
                                             const void *data,
                                             uint32_t wait_time)
{
  sl_wifi_buffer_t *buffer;
  sl_si91x_packet_t *packet;
  sl_si91x_socket_send_request_t *send;

  sl_status_t status     = SL_STATUS_OK;
  uint16_t header_length = (request->data_offset - sizeof(sl_si91x_socket_send_request_t));
  uint32_t data_length   = request->length;

  if (data == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  status = si91x_host_allocate_buffer(
    &buffer,
    SL_WIFI_TX_FRAME_BUFFER,
    sizeof(sl_si91x_packet_t) + sizeof(sl_si91x_socket_send_request_t) + header_length + data_length,
    SL_WIFI_ALLOCATE_COMMAND_BUFFER_WAIT_TIME);

  VERIFY_STATUS_AND_RETURN(status);
  packet = si91x_host_get_buffer_data(buffer, 0, NULL);

  if (packet == NULL) {
    return SL_STATUS_WIFI_BUFFER_ALLOC_FAIL;
  }

  memset(packet->desc, 0, sizeof(packet->desc));

  send = (sl_si91x_socket_send_request_t *)packet->data;
  memcpy(send, request, sizeof(sl_si91x_socket_send_request_t));
  memcpy((send->send_buffer + header_length), data, data_length);

  // Fill frame type
  packet->length = (sizeof(sl_si91x_socket_send_request_t) + header_length + data_length) & 0xFFF;
  return sl_si91x_driver_send_data_packet(SI91X_SOCKET_CMD_QUEUE, buffer, wait_time);
}

sl_status_t sl_si91x_driver_send_command(uint32_t command,
                                         si91x_queue_type_t queue_type,
                                         const void *data,
                                         uint32_t data_length,
                                         sl_si91x_wait_period_t wait_period,
                                         void *sdk_context,
                                         sl_wifi_buffer_t **data_buffer)
{
  sl_wifi_buffer_t *buffer;
  sl_si91x_packet_t *packet;
  sl_status_t status;

  if (queue_type >= (si91x_queue_type_t)SI91X_CMD_MAX) {
    return SL_STATUS_INVALID_INDEX;
  }

  status = sl_si91x_allocate_command_buffer(&buffer,
                                            (void **)&packet,
                                            sizeof(sl_si91x_packet_t) + data_length,
                                            SL_WIFI_ALLOCATE_COMMAND_BUFFER_WAIT_TIME);
  VERIFY_STATUS_AND_RETURN(status);

  memset(packet->desc, 0, sizeof(packet->desc));
  if (data != NULL) {
    memcpy(packet->data, data, data_length);
  }

  // Fill frame type
  packet->length  = data_length & 0xFFF;
  packet->command = command;
  return sl_si91x_driver_send_command_packet(command, queue_type, buffer, wait_period, sdk_context, data_buffer);
}

sl_status_t sl_si91x_driver_send_bt_command(rsi_wlan_cmd_request_t command,
                                            si91x_queue_type_t queue_type,
                                            sl_wifi_buffer_t *data)
{
  sl_si91x_wait_period_t wait_period = SL_SI91X_RETURN_IMMEDIATELY;

  if (queue_type >= (si91x_queue_type_t)SI91X_CMD_MAX) {

    return SL_STATUS_INVALID_INDEX;
  }

  return sl_si91x_driver_send_command_packet(command, queue_type, data, wait_period, NULL, NULL);
}

sl_status_t sl_si91x_driver_wait_for_response(rsi_wlan_cmd_request_t command, sl_si91x_wait_period_t wait_period)
{
  UNUSED_PARAMETER(command);
  UNUSED_PARAMETER(wait_period);
#ifdef SI91x_ENABLE_WAIT_ON_RESULTS
  uint32_t events =
    si91x_host_wait_for_event(NCP_HOST_WLAN_RESPONSE_EVENT, (wait_period & ~SL_SI91X_WAIT_FOR_RESPONSE_BIT));

  //TODO: Change error handling from event based to reponse
  if (events & NCP_HOST_WLAN_RESPONSE_EVENT) {
    return convert_firmware_status(si91x_frame_error_status);
  } else if (events == 0) {
    return SL_STATUS_TIMEOUT;
  }
#endif
  return SL_STATUS_NOT_SUPPORTED;
}

static void sl_si91x_atomic_packet_id_allocator(void *user_data)
{
  sl_si91x_driver_context_t *context = (sl_si91x_driver_context_t *)user_data;
  uint16_t *packet_id_tracker        = (uint16_t *)context->payload;

  context->packet_id         = *packet_id_tracker;
  context->packet->packet_id = *packet_id_tracker;

  *packet_id_tracker = (*packet_id_tracker + 1) & 0xFFFF;

  return;
}

static bool si91x_packet_identification_function(sl_wifi_buffer_t *node, void *user_data)
{
  si91x_queue_packet_t *packet       = NULL;
  sl_si91x_driver_context_t *context = (sl_si91x_driver_context_t *)user_data;
  uint16_t data_length               = 0;

  packet = (si91x_queue_packet_t *)si91x_host_get_buffer_data(node, 0, &data_length);

  if (packet->packet_id == context->packet_id) {
    return true;
  }

  return false;
}

sl_status_t sl_si91x_driver_wait_for_response_packet(si91x_queue_type_t queue_type,
                                                     uint32_t event_mask,
                                                     uint16_t packet_id,
                                                     sl_si91x_wait_period_t wait_period,
                                                     sl_wifi_buffer_t **packet_buffer)
{
  sl_si91x_driver_context_t context = { 0 };
  sl_status_t status;
  uint32_t events = 0;

  while (true) {
    events = si91x_host_wait_for_event(event_mask, (wait_period & ~SL_SI91X_WAIT_FOR_RESPONSE_BIT));
    SL_DEBUG_LOG("><<<< Got event : %u for queue %u\n", events, queue_type);
    if (events == 0) {
      return SL_STATUS_TIMEOUT;
    }

    context.packet_id = packet_id;
    status =
      si91x_host_remove_node_from_queue(queue_type, packet_buffer, &context, si91x_packet_identification_function);
    if (SL_STATUS_OK == status) {
      if (0 == si91x_host_queue_status(queue_type)) {
        si91x_host_clear_events(event_mask);
      }
      return SL_STATUS_OK;
    } else if (SL_STATUS_NOT_FOUND != status) {
      VERIFY_STATUS_AND_RETURN(status);
    }
  }

  return SL_STATUS_INVALID_STATE;
}

// This function is still command specific
sl_status_t sl_si91x_driver_send_command_packet(uint32_t command,
                                                si91x_queue_type_t queue_type,
                                                sl_wifi_buffer_t *buffer,
                                                sl_si91x_wait_period_t wait_period,
                                                void *sdk_context,
                                                sl_wifi_buffer_t **data_buffer)
{
  uint16_t firmware_status;
  si91x_queue_packet_t *node = NULL;
  sl_status_t status;
  sl_wifi_buffer_t *packet;
  sl_wifi_buffer_t *response;
  uint8_t flags                                     = 0;
  uint16_t data_length                              = 0;
  sl_si91x_driver_context_t context                 = { 0 };
  sl_si91x_wait_period_t wait_time                  = 0;
  static uint8_t firmware_queue_id[SI91X_CMD_MAX]   = { [SI91X_COMMON_CMD]  = RSI_WLAN_MGMT_Q,
                                                        [SI91X_WLAN_CMD]    = RSI_WLAN_MGMT_Q,
                                                        [SI91X_NETWORK_CMD] = RSI_WLAN_MGMT_Q,
                                                        [SI91X_SOCKET_CMD]  = RSI_WLAN_MGMT_Q,
                                                        [SI91X_BT_CMD]      = RSI_BT_Q };
  static uint32_t response_event_map[SI91X_CMD_MAX] = { [SI91X_COMMON_CMD]  = NCP_HOST_COMMON_RESPONSE_EVENT,
                                                        [SI91X_WLAN_CMD]    = NCP_HOST_WLAN_RESPONSE_EVENT,
                                                        [SI91X_NETWORK_CMD] = NCP_HOST_NETWORK_RESPONSE_EVENT,
                                                        [SI91X_SOCKET_CMD]  = NCP_HOST_SOCKET_RESPONSE_EVENT,
                                                        [SI91X_BT_CMD]      = NCP_HOST_BT_RESPONSE_EVENT };

  status = sl_si91x_allocate_command_buffer(&packet,
                                            (void **)&node,
                                            sizeof(si91x_queue_packet_t),
                                            SL_WIFI_ALLOCATE_COMMAND_BUFFER_WAIT_TIME);
  if (status != SL_STATUS_OK) {
    si91x_host_free_buffer(buffer, SL_WIFI_TX_FRAME_BUFFER);
    return status;
  }

#ifdef RSI_CHIP_MFG_EN
  // WLAN soc frequency
  if (command == RSI_BOOTUP_PARAMS) {
    host_desc[14] = RSI_SOC_FREQ;
  }
#endif

  if (SL_SI91X_RETURN_IMMEDIATELY == wait_period) {
    flags = 0;
  } else {
    flags |= SI91X_PACKET_RESPONSE_STATUS;
    flags |= (((wait_period & SL_SI91X_WAIT_FOR_RESPONSE_BIT) != 0) ? SI91X_PACKET_RESPONSE_PACKET : 0);
  }

  switch (command) {
    case RSI_COMMON_REQ_PWRMODE:
    case RSI_COMMON_REQ_OPERMODE:
    case RSI_COMMON_REQ_SOFT_RESET:
      flags |= SI91X_PACKET_GLOBAL_QUEUE_BLOCK;
      break;
    default:
      break;
  }

  node->host_packet       = buffer;
  node->firmware_queue_id = firmware_queue_id[queue_type];
  node->command_type      = (si91x_command_type_t)queue_type;
  node->sdk_context       = sdk_context;
  node->flags             = flags;

  context.packet  = node;
  context.payload = &(packet_id[queue_type]);
  si91x_host_add_to_queue_with_atomic_action(queue_type, packet, &context, sl_si91x_atomic_packet_id_allocator);
  si91x_host_set_event(NCP_HOST_DATA_TX_EVENT);

  if (wait_period == SL_SI91X_RETURN_IMMEDIATELY) {
    return SL_STATUS_IN_PROGRESS;
  }

  if ((wait_period & ~SL_SI91X_WAIT_FOR_RESPONSE_BIT) == SL_SI91X_WAIT_FOR_EVER) {
    wait_time = osWaitForever;
  } else {
    wait_time = (wait_period & ~SL_SI91X_WAIT_FOR_RESPONSE_BIT);
  }

  status = sl_si91x_driver_wait_for_response_packet((queue_type + SI91X_CMD_MAX),
                                                    response_event_map[queue_type],
                                                    context.packet_id,
                                                    wait_time,
                                                    &response);
  VERIFY_STATUS_AND_RETURN(status);
  node            = (si91x_queue_packet_t *)si91x_host_get_buffer_data(response, 0, &data_length);
  firmware_status = node->frame_status;

  if (NULL != data_buffer) {
    *data_buffer = node->host_packet;
  } else if (SI91X_PACKET_RESPONSE_PACKET == (node->flags & SI91X_PACKET_RESPONSE_PACKET)) {
    si91x_host_free_buffer(node->host_packet, SL_WIFI_CONTROL_BUFFER);
  }

  si91x_host_free_buffer(response, SL_WIFI_CONTROL_BUFFER);
  return convert_firmware_status(firmware_status);
}

sl_status_t sl_si91x_driver_send_data_packet(si91x_queue_type_t queue_type,
                                             sl_wifi_buffer_t *buffer,
                                             uint32_t wait_time)
{
  UNUSED_PARAMETER(wait_time);
  si91x_queue_packet_t *node = NULL;
  sl_status_t return_status;
  sl_wifi_buffer_t *packet;
  sl_si91x_driver_context_t context = { 0 };

  return_status = sl_si91x_allocate_command_buffer(&packet,
                                                   (void **)&node,
                                                   sizeof(si91x_queue_packet_t),
                                                   SL_WIFI_ALLOCATE_COMMAND_BUFFER_WAIT_TIME);
  if (return_status != SL_STATUS_OK) {
    si91x_host_free_buffer(buffer, SL_WIFI_TX_FRAME_BUFFER);
    return return_status;
  }

#ifdef RSI_CHIP_MFG_EN
  // WLAN soc frequency
  if (command == RSI_BOOTUP_PARAMS)
    host_desc[14] = RSI_SOC_FREQ;
#endif

  node->host_packet       = buffer;
  node->firmware_queue_id = RSI_WLAN_DATA_Q;
  node->command_type      = (si91x_command_type_t)queue_type;
  node->sdk_context       = NULL;
  node->flags             = SI91X_PACKET_WITH_NO_RESPONSE;

  context.packet  = node;
  context.payload = &(packet_id[queue_type]);
  si91x_host_add_to_queue_with_atomic_action(queue_type, packet, &context, sl_si91x_atomic_packet_id_allocator);
  si91x_host_set_event(NCP_HOST_DATA_TX_EVENT);

  return SL_STATUS_OK;
}

/*
 * Verifies that the device has booted successfully.
 * @return SL_STATUS_OK if verification successful
 *         otherwise RSI_ERROR_WAITING_FOR_BOARD_READY, RSI_ERROR_BOOTUP_OPTIONS_NOT_SAVED, RSI_ERROR_BOOTUP_OPTIONS_CHECKSUM_FAIL
 */
sl_status_t sli_verify_device_boot(uint32_t *rom_version)
{
  sl_status_t status;
  uint8_t value[2] = { 0, 0 };

  status = si91x_bus_read_memory(RSI_HOST_INTF_REG_OUT, 2, &value[0]);
  VERIFY_STATUS(status);

  // Verify register read was valid
  if (value[1] != SLI_WIFI_REGISTER_VALID) {
    return SL_STATUS_WAITING_FOR_BOARD_READY;
  }

  // Verify register value
  if (value[0] == RSI_BOOTUP_OPTIONS_LAST_CONFIG_NOT_SAVED) {
    return SL_STATUS_BOOTUP_OPTIONS_NOT_SAVED;
  } else if (value[0] == RSI_BOOTUP_OPTIONS_CHECKSUM_FAIL) {
    return SL_STATUS_BOOTUP_OPTIONS_CHECKSUM_FAILURE;
  }
#if RSI_BOOTLOADER_VERSION_CHECK
  else if (value[0] == RSI_BOOTLOADER_VERSION) {
  } else {
    return SL_STATUS_BOOTLOADER_VERSION_MISMATCH;
  }
#endif

  // Extract ROM version info
  if (value[0] == RSI_BOOTLOADER_VERSION_1P0) {
    *rom_version = RSI_ROM_VERSION_1P0;
  } else if (value[0] == RSI_BOOTLOADER_VERSION_1P1) {
    *rom_version = RSI_ROM_VERSION_1P1;
  }

  return SL_STATUS_OK;
}

/**************************************************************************/ /**
 * @brief Allocate a buffer for the Wi-Fi driver
 *
 * @param buffer
 * @param buffer_size is the size of the buffer to allocate
 * @param wait_duration_ms is the duration before returning SL_TIMEOUT
 * @return SL_STATUS_OK if the values are retrieved correctly,
 * SL_TIMEOUT if the buffer is not allocated in time, SL_ERROR otherwise
 *****************************************************************************/
sl_status_t sl_si91x_allocate_command_buffer(sl_wifi_buffer_t **host_buffer,
                                             void **buffer,
                                             uint32_t requested_buffer_size,
                                             uint32_t wait_duration_ms)
{
  sl_status_t status =
    si91x_host_allocate_buffer(host_buffer, SL_WIFI_CONTROL_BUFFER, requested_buffer_size, wait_duration_ms);
  VERIFY_STATUS(status);

  uint16_t temp;
  *buffer = si91x_host_get_buffer_data(*host_buffer, 0, &temp);
  //  SL_ASSERT(data_length >= requested_buffer_size, "Allocation failed");
  return SL_STATUS_OK;
}

sl_status_t sl_si91x_allocate_data_buffer(sl_wifi_buffer_t **host_buffer,
                                          void **buffer,
                                          uint32_t data_size,
                                          uint32_t wait_duration_ms)
{
  sl_status_t status =
    si91x_host_allocate_buffer(host_buffer,
                               SL_WIFI_TX_FRAME_BUFFER,
                               sizeof(sl_si91x_packet_t) + sizeof(sl_si91x_socket_send_request_t) + data_size,
                               wait_duration_ms);
  VERIFY_STATUS(status);

  uint16_t temp;
  *buffer = si91x_host_get_buffer_data(*host_buffer, 0, &temp);
  //  SL_ASSERT(data_length >= requested_buffer_size, "Allocation failed");
  return SL_STATUS_OK;
}

sl_status_t sli_wifi_select_option(const uint8_t configuration)
{
  uint16_t boot_command = 0;
  sl_status_t status    = 0;
  uint16_t read_value   = 0;

  status = si91x_bus_write_memory(RSI_HOST_INTF_REG_OUT, 2, (uint8_t *)&boot_command);
  VERIFY_STATUS_AND_RETURN(status);

  if (configuration == BURN_NWP_FW) {
    boot_command = RSI_HOST_INTERACT_REG_VALID_FW | configuration;
  } else {
    boot_command = RSI_HOST_INTERACT_REG_VALID | configuration;
  }
  status = si91x_bus_write_memory(RSI_HOST_INTF_REG_IN, 2, (uint8_t *)&boot_command);
  VERIFY_STATUS_AND_RETURN(status);

  // Check for load firmware
  if ((configuration != LOAD_NWP_FW) && (configuration != LOAD_DEFAULT_NWP_FW_ACTIVE_LOW)) {
    uint32_t timestamp = si91x_host_get_timestamp();
    while (si91x_host_elapsed_time(timestamp) < 300) {
      status = si91x_bus_read_memory(RSI_HOST_INTF_REG_OUT, 2, (uint8_t *)&read_value);
      VERIFY_STATUS_AND_RETURN(status);

      if (configuration == BURN_NWP_FW) {
        if (read_value == (RSI_HOST_INTERACT_REG_VALID | RSI_SEND_RPS_FILE)) {
          return SL_STATUS_OK;
        }
      } else if (read_value == (RSI_HOST_INTERACT_REG_VALID | configuration)) {
        return SL_STATUS_OK;
      }
    }
  } else {
    // Check up to 3 seconds
    uint32_t timestamp = si91x_host_get_timestamp();
    while (si91x_host_elapsed_time(timestamp) < 3000) {
      status = si91x_bus_read_memory(RSI_HOST_INTF_REG_OUT, 2, (uint8_t *)&read_value);
      if (status != SL_STATUS_OK)
        continue;

      if ((read_value & 0xF000) == (RSI_HOST_INTERACT_REG_VALID_FW & 0xF000)) {
        if ((read_value & 0xFF) == VALID_FIRMWARE_NOT_PRESENT) {
          return SL_STATUS_VALID_FIRMWARE_NOT_PRESENT;
        }
        if ((read_value & 0xFF) == RSI_INVALID_OPTION) {
          return SL_STATUS_INVALID_OPTION;
        }
        if ((read_value & 0xFF) == RSI_CHECKSUM_SUCCESS) {
          return status;
        }
      }
    }
  }
  return SL_STATUS_FW_LOAD_OR_UPGRADE_TIMEOUT;
}

sl_status_t sl_si91x_enable_radio(void)
{
  uint8_t data       = 1;
  sl_status_t status = sl_si91x_driver_send_command(RSI_WLAN_REQ_RADIO,
                                                    SI91X_WLAN_CMD_QUEUE,
                                                    &data,
                                                    1,
                                                    SL_SI91X_WAIT_FOR_COMMAND_SUCCESS,
                                                    NULL,
                                                    NULL);

  VERIFY_STATUS_AND_RETURN(status);
  return status;
}

sl_status_t sl_si91x_disable_radio(void)
{
  uint8_t data       = 0;
  sl_status_t status = sl_si91x_driver_send_command(RSI_WLAN_REQ_RADIO,
                                                    SI91X_WLAN_CMD_QUEUE,
                                                    &data,
                                                    1,
                                                    SL_SI91X_WAIT_FOR_COMMAND_SUCCESS,
                                                    NULL,
                                                    NULL);

  VERIFY_STATUS_AND_RETURN(status);
  return status;
}

sl_status_t sl_si91x_write_calibration_data(const si91x_calibration_data_t *data)
{
  sl_status_t status = sl_si91x_driver_send_command(RSI_WLAN_REQ_CALIB_WRITE,
                                                    SI91X_WLAN_CMD_QUEUE,
                                                    data,
                                                    sizeof(si91x_calibration_data_t),
                                                    SL_SI91X_WAIT_FOR_COMMAND_SUCCESS,
                                                    NULL,
                                                    NULL);

  VERIFY_STATUS_AND_RETURN(status);
  return status;
}

sl_status_t sl_si91x_wifi_set_certificate_index(uint8_t certificate_type,
                                                uint8_t certificate_index,
                                                const uint8_t *buffer,
                                                uint32_t certificate_length)
{
  uint32_t rem_len                      = 0;
  uint16_t chunk_size                   = 0;
  uint16_t data_size                    = 0;
  uint8_t chunks_remaining              = 0;
  uint32_t offset                       = 0;
  sl_status_t status                    = SL_STATUS_OK;
  si91x_req_set_certificate_t chunk_ptr = { 0 };

  if (!device_initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }

  // Get the certificate chunk size
  chunk_size = (SI91X_MAX_CERT_SEND_SIZE - sizeof(si91x_cert_info_t));

  // Get certificate length
  rem_len = certificate_length;

  do {
    // Memset the PKT
    memset(&chunk_ptr, 0, sizeof(si91x_req_set_certificate_t));

    // If certificate length is 0, erase certificate
    if (rem_len == 0) {
      // Mark Data Size
      data_size = 0;

      // More chunks to send
      chunks_remaining = 0;
    } else if (rem_len >= chunk_size) {
      // Mark Data Size
      data_size = chunk_size;

      // More chunks to send
      chunks_remaining = 1;
    } else {
      // Mark Data Size
      data_size = rem_len;

      // More chunks to send
      chunks_remaining = 0;
    }

    if ((data_size > 0) && (NULL != buffer)) {
      // Copy the certificate chunk
      memcpy(chunk_ptr.certificate, buffer + offset, data_size);
    }

    // Move the offset by chunk size
    offset += data_size;

    // Subtract the rem_len by the chunk size
    rem_len -= data_size;

    //Set the total length of certificate
    memcpy(&chunk_ptr.cert_info.total_len, &certificate_length, sizeof(chunk_ptr.cert_info.total_len));

    // Set the certificate type
    chunk_ptr.cert_info.certificate_type = certificate_type;

    // Set the certificate index
    chunk_ptr.cert_info.certificate_inx = certificate_index;

    // More chunks to send
    chunk_ptr.cert_info.more_chunks = chunks_remaining;

    // Set the length of the certificate chunk
    chunk_ptr.cert_info.certificate_length = data_size;

    // Send the driver command
    status = sl_si91x_driver_send_command(RSI_WLAN_REQ_SET_CERTIFICATE,
                                          SI91X_WLAN_CMD_QUEUE,
                                          &chunk_ptr,
                                          (sizeof(si91x_cert_info_t) + data_size),
                                          SL_SI91X_WAIT_FOR_COMMAND_SUCCESS,
                                          NULL,
                                          NULL);
    VERIFY_STATUS_AND_RETURN(status);
  } while (rem_len > 0);

  // Return status
  return status;
}

sl_status_t sl_si91x_set_device_region(sl_si91x_operation_mode_t operation_mode,
                                       sl_si91x_band_mode_t band,
                                       sl_si91x_region_code_t region_code)
{
  sl_status_t status = SL_STATUS_OK;

  switch (operation_mode) {
    case SL_SI91X_CLIENT_MODE:
    case SL_SI91X_ENTERPRISE_CLIENT_MODE: {
      si91x_set_region_request_t request = { 0 };

      request.set_region_code_from_user_cmd = SET_REGION_CODE_FROM_USER;
      request.region_code                   = region_code;

      status = sl_si91x_driver_send_command(RSI_WLAN_REQ_SET_REGION,
                                            SI91X_WLAN_CMD_QUEUE,
                                            &request,
                                            sizeof(si91x_set_region_request_t),
                                            SL_SI91X_WAIT_FOR_COMMAND_SUCCESS,
                                            NULL,
                                            NULL);
      VERIFY_STATUS_AND_RETURN(status);
      break;
    }

    case SL_SI91X_CONCURRENT_MODE:
    case SL_SI91X_ACCESS_POINT_MODE: {
      if (operation_mode == SL_SI91X_CONCURRENT_MODE) {
        si91x_set_region_request_t request = { 0 };

        request.set_region_code_from_user_cmd = SET_REGION_CODE_FROM_USER;
        request.region_code                   = region_code;

        status = sl_si91x_driver_send_command(RSI_WLAN_REQ_SET_REGION,
                                              SI91X_WLAN_CMD_QUEUE,
                                              &request,
                                              sizeof(si91x_set_region_request_t),
                                              SL_SI91X_WAIT_FOR_COMMAND_SUCCESS,
                                              NULL,
                                              NULL);
        VERIFY_STATUS_AND_RETURN(status);
      }

      si91x_set_region_ap_request_t request = { 0 };

      request.set_region_code_from_user_cmd = SET_REGION_CODE_FROM_USER;
      switch (region_code) {
        case DEFAULT_REGION:
        case US: {
          if (band == SL_SI91X_WIFI_BAND_2_4GHZ) {
            request = default_US_region_2_4GHZ_configurations;
          } else {
            request = default_US_region_5GHZ_configurations;
          }
          break;
        }
        case EU: {
          if (band == SL_SI91X_WIFI_BAND_2_4GHZ) {
            request = default_EU_region_2_4GHZ_configurations;
          } else {
            request = default_EU_region_5GHZ_configurations;
          }
          break;
        }
        case JP: {
          if (band == SL_SI91X_WIFI_BAND_2_4GHZ) {
            request = default_JP_region_2_4GHZ_configurations;
          } else {
            request = default_JP_region_5GHZ_configurations;
          }
          break;
        }
        case KR: {
          if (band == SL_SI91X_WIFI_BAND_2_4GHZ) {
            request = default_KR_region_2_4GHZ_configurations;
          } else {
            request = default_KR_region_5GHZ_configurations;
          }
          break;
        }
        case SG: {
          if (band == SL_SI91X_WIFI_BAND_2_4GHZ) {
            request = default_SG_region_2_4GHZ_configurations;
          } else {
            request = default_SG_region_5GHZ_configurations;
          }
          break;
        }
        default:
          return SL_STATUS_NOT_SUPPORTED;
      }
      status = sl_si91x_driver_send_command(RSI_WLAN_REQ_SET_REGION_AP,
                                            SI91X_WLAN_CMD_QUEUE,
                                            &request,
                                            sizeof(si91x_set_region_ap_request_t),
                                            SL_SI91X_WAIT_FOR_COMMAND_SUCCESS,
                                            NULL,
                                            NULL);
      VERIFY_STATUS_AND_RETURN(status);
      break;
    }

    default:
      break;
  }

  return status;
}

#ifdef RSI_M4_INTERFACE
#ifdef CHIP_917
sl_status_t sl_si91x_command_to_write_common_flash(uint32_t write_address,
                                                   uint8_t *write_data,
                                                   uint16_t write_data_length)
{
  sl_status_t status     = SL_STATUS_OK;
  uint32_t rem_len       = write_data_length;
  static uint32_t offset = 0;
  uint16_t chunk_size    = MAX_CHUNK_SIZE - (sizeof(si91x_request_ta2m4_t) - MAX_CHUNK_SIZE);

  si91x_request_ta2m4_t ta_to_m4_request = { 0 };
  ta_to_m4_request.sub_cmd               = SI91X_WRITE_TO_COMMON_FLASH;
  ta_to_m4_request.addr                  = write_address;

  SL_VERIFY_POINTER_OR_RETURN(write_data, SL_STATUS_INVALID_PARAMETER);

  while (rem_len) {
    ta_to_m4_request.input_buffer_length = rem_len;
    if (rem_len >= chunk_size) {
      ta_to_m4_request.chunk_len   = chunk_size;
      ta_to_m4_request.more_chunks = 1;
      memcpy(&ta_to_m4_request.input_data, write_data + offset, chunk_size);
      offset += chunk_size;
      rem_len -= chunk_size;
    } else {
      ta_to_m4_request.chunk_len   = rem_len;
      ta_to_m4_request.more_chunks = 0;
      memcpy(&ta_to_m4_request.input_data, write_data + offset, rem_len);
      rem_len = 0;
      offset  = 0;
    }
    status = sl_si91x_driver_send_command(RSI_COMMON_REQ_TA_M4_COMMANDS,
                                          SI91X_WLAN_CMD_QUEUE,
                                          &ta_to_m4_request,
                                          sizeof(si91x_request_ta2m4_t),
                                          SL_SI91X_WAIT_FOR_COMMAND_SUCCESS,
                                          NULL,
                                          NULL);

    VERIFY_STATUS_AND_RETURN(status);
  }

  return status;
}
#endif

sl_status_t sl_si91x_m4_ta_secure_handshake(uint8_t sub_cmd_type,
                                            uint8_t input_len,
                                            uint8_t *input_data,
                                            uint8_t output_len,
                                            uint8_t *output_data)
{
  UNUSED_PARAMETER(output_len);
  UNUSED_PARAMETER(output_data);
  ta_m4_handshake_parameters_t handshake_request = { 0 };
  sl_status_t status                             = SL_STATUS_OK;

  SL_VERIFY_POINTER_OR_RETURN(input_data, SL_STATUS_INVALID_PARAMETER);

  handshake_request.sub_cmd         = sub_cmd_type;
  handshake_request.input_data_size = input_len;
  if (handshake_request.input_data != NULL)
    memcpy(handshake_request.input_data, input_data, input_len);

  status = sl_si91x_driver_send_command(RSI_COMMON_REQ_TA_M4_COMMANDS,
                                        SI91X_WLAN_CMD_QUEUE,
                                        &handshake_request,
                                        sizeof(ta_m4_handshake_parameters_t),
                                        SL_SI91X_WAIT_FOR_COMMAND_SUCCESS,
                                        NULL,
                                        NULL);
  VERIFY_STATUS_AND_RETURN(status);
  return status;
}

static sl_status_t sl_si91x_soft_reset(void)
{
  sl_status_t status;
  if (!device_initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  status = sl_si91x_driver_send_command(RSI_COMMON_REQ_SOFT_RESET,
                                        SI91X_COMMON_CMD_QUEUE,
                                        NULL,
                                        0,
                                        SL_SI91X_WAIT_FOR_COMMAND_SUCCESS,
                                        NULL,
                                        NULL);
  return status;
}
#endif

sl_status_t sl_si91x_assert()
{
  sl_status_t status = SL_STATUS_OK;

  if (!device_initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  status = sl_si91x_driver_send_command(RSI_COMMON_REQ_ASSERT,
                                        SI91X_WLAN_CMD_QUEUE,
                                        NULL,
                                        0,
                                        SL_SI91X_WAIT_FOR(30000),
                                        NULL,
                                        NULL);
  VERIFY_STATUS_AND_RETURN(status);
  return status;
}

sl_status_t sl_si91x_get_ram_log(uint32_t address, uint32_t length)
{
  if (!device_initialized) {
    return SL_STATUS_NOT_INITIALIZED;
  }
  sl_status_t status      = SL_STATUS_OK;
  sl_si91x_ram_dump_t ram = { .address = address, .length = length };
  if (length == 0) {
    return SL_STATUS_INVALID_PARAMETER;
  }
  // Send RAM log request
  status = sl_si91x_driver_send_command(RSI_COMMON_REQ_GET_RAM_DUMP,
                                        SI91X_COMMON_CMD_QUEUE,
                                        &ram,
                                        sizeof(sl_si91x_ram_dump_t),
                                        SL_SI91X_WAIT_FOR(31000),
                                        NULL,
                                        NULL);
  VERIFY_STATUS_AND_RETURN(status);
  return status;
}