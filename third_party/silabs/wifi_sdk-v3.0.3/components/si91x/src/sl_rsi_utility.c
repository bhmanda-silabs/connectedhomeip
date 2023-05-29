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
#include "sl_si91x_protocol_types.h"
#include "sl_si91x_constants.h"
#include "sl_si91x_driver.h"
#include "sl_net_constants.h"
#include "sl_wifi_constants.h"
#include "sl_status.h"
#include "sl_constants.h"
#include "sl_wifi_types.h"
#include "sl_rsi_utility.h"
#include <string.h>

// NOTE: Boolean value determines whether firmware automatically closes the TCP socket in case of receiving termination from remote node or not.
static bool tcp_auto_close_enabled;

// This value will be used in connect request/ AP configurations to set the TX power of the module
static uint8_t wifi_max_tx_power;

static sl_wifi_rate_t saved_wifi_data_rate;

static sl_wifi_ap_configuration_t ap_configuration;

static bool is_card_ready_required = true;

sl_performance_profile_t current_performance_profile = HIGH_PERFORMANCE;

static si91x_boot_configuration_t saved_boot_configuration;

void save_wifi_current_performance_profile(const sl_wifi_performance_profile_t *profile)
{
  SL_ASSERT(profile != NULL);
  memcpy(&performance_profile.wifi_performance_profile, profile, sizeof(sl_wifi_performance_profile_t));
}

void get_wifi_current_performance_profile(sl_wifi_performance_profile_t *profile)
{
  SL_ASSERT(profile != NULL);
  memcpy(profile, &performance_profile.wifi_performance_profile, sizeof(sl_wifi_performance_profile_t));
}

void get_coex_performance_profile(sl_performance_profile_t *profile)
{
  SL_ASSERT(profile != NULL);
  uint8_t mode_decision = 0;

  switch (performance_profile.coex_mode) {
    case SL_SI91X_WLAN_MODE: {
      // WIFI
      mode_decision = ((performance_profile.wifi_performance_profile.profile << 4)
                       | (performance_profile.wifi_performance_profile.profile));
    } break;
    case SL_SI91X_BLUETOOTH_MODE:
    case SL_SI91X_BLE_MODE:
    case SL_SI91X_DUAL_MODE: {
      //BT
      mode_decision = ((performance_profile.bt_performance_profile.profile << 4)
                       | (performance_profile.bt_performance_profile.profile));
    } break;
    case SL_SI91X_WLAN_BLUETOOTH_MODE:
    case SL_SI91X_WLAN_DUAL_MODE:
    case SL_SI91X_WLAN_BLE_MODE: {
      // WIFI + BT
      mode_decision = ((performance_profile.wifi_performance_profile.profile << 4)
                       | (performance_profile.bt_performance_profile.profile));
    } break;
    default:
      break;
  }

  switch (mode_decision) {
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
    case 0x10:
    case 0x20:
    case 0x30:
    case 0x40: {
      *profile = HIGH_PERFORMANCE;
    } break;
    case 0x11:
    case 0x12:
    case 0x31:
    case 0x13:
    case 0x14:
    case 0x41: {
      *profile = ASSOCIATED_POWER_SAVE;
    } break;
    case 0x22:
    case 0x21:
    case 0x32:
    case 0x23:
    case 0x42:
    case 0x24: {
      *profile = ASSOCIATED_POWER_SAVE_LOW_LATENCY;
    } break;
    case 0x33: {
      *profile = STANDBY_POWER_SAVE;
    } break;
    case 0x44: {
      *profile = STANDBY_POWER_SAVE_WITH_RAM_RETENTION;
    } break;
    default: {
      // Do nothing
    } break;
  }

  return;
}

void reset_coex_current_performance_profile(void)
{
  memset(&performance_profile, 0, sizeof(sl_si91x_performance_profile_t));
}

void save_boot_configuration(const si91x_boot_configuration_t *boot_configuration)
{
  saved_boot_configuration = *boot_configuration;
}

void get_saved_boot_configuration(si91x_boot_configuration_t boot_configuration)
{
  boot_configuration = saved_boot_configuration;
}

void save_bt_current_performance_profile(const sl_bt_performance_profile_t *profile)
{
  SL_ASSERT(profile != NULL);
  memcpy(&performance_profile.bt_performance_profile, profile, sizeof(sl_bt_performance_profile_t));
}

void get_bt_current_performance_profile(sl_bt_performance_profile_t *profile)
{
  SL_ASSERT(profile != NULL);
  memcpy(profile, &performance_profile.bt_performance_profile, sizeof(sl_bt_performance_profile_t));
}

void save_tcp_auto_close_choice(bool is_tcp_auto_close_enabled)
{
  tcp_auto_close_enabled = is_tcp_auto_close_enabled;
}

bool is_tcp_auto_close_enabled()
{
  return tcp_auto_close_enabled;
}

void save_max_tx_power(uint8_t max_tx_power)
{
  wifi_max_tx_power = max_tx_power;
}

uint8_t get_max_tx_power()
{
  return wifi_max_tx_power;
}

void reset_max_tx_power()
{
  wifi_max_tx_power = 0;
}

void set_card_ready_required(bool card_ready_required)
{
  is_card_ready_required = card_ready_required;
}

bool get_card_ready_required()
{
  return is_card_ready_required;
}

sl_status_t convert_si91x_wifi_client_info(sl_wifi_client_info_response *client_info_response,
                                           si91x_client_info_response *si91x_client_info_response)
{

  ARGS_CHECK_NULL_POINTER(si91x_client_info_response);
  ARGS_CHECK_NULL_POINTER(client_info_response);

  client_info_response->client_count = si91x_client_info_response->sta_count[0]
                                       | si91x_client_info_response->sta_count[1] << 8;

  for (uint8_t station_index = 0; station_index < client_info_response->client_count; station_index++) {
    uint8_t *si91x_ip_address, *sl_ip_address;

    sl_wifi_client_info *sl_client_info     = &client_info_response->client_info[station_index];
    si91x_station_info_t *si91x_client_info = &si91x_client_info_response->sta_info[station_index];

    uint8_t ip_address_size = si91x_client_info->ip_version[0] | si91x_client_info->ip_version[1] << 8;

    si91x_ip_address = ip_address_size == SL_IPV4_ADDRESS_LENGTH ? si91x_client_info->ip_address.ipv4_address
                                                                 : si91x_client_info->ip_address.ipv6_address;
    sl_ip_address    = ip_address_size == SL_IPV4_ADDRESS_LENGTH ? sl_client_info->ip_address.ip.v4.bytes
                                                                 : sl_client_info->ip_address.ip.v6.bytes;

    sl_client_info->ip_address.type = ip_address_size == SL_IPV4_ADDRESS_LENGTH ? SL_IPV4 : SL_IPV6;

    memcpy(&sl_client_info->mac_adddress, si91x_client_info->mac, sizeof(sl_mac_address_t));
    memcpy(sl_ip_address, si91x_ip_address, ip_address_size);
  }

  return SL_STATUS_OK;
}

sl_status_t convert_si91x_event_to_sl_wifi_event(rsi_wlan_cmd_response_t command, sl_wifi_event_t *sl_wifi_event)
{
  ARGS_CHECK_NULL_POINTER(sl_wifi_event);

  switch (command) {
    case RSI_WLAN_RSP_BG_SCAN:
    case RSI_WLAN_RSP_SCAN:
      *sl_wifi_event = SL_WIFI_SCAN_RESULT_EVENT;
      break;
    case RSI_WLAN_RSP_JOIN:
      *sl_wifi_event = SL_WIFI_JOIN_EVENT;
      break;
    case RSI_WLAN_RSP_RX_STATS:
      *sl_wifi_event = SL_WIFI_RECEIVE_STATS_RESPONSE_EVENT;
      break;
    case RSI_WLAN_RSP_HTTP_OTAF:
      *sl_wifi_event = SL_WIFI_HTTP_OTA_FW_UPDATE_EVENT;
      break;
    case RSI_WLAN_RSP_CLIENT_CONNECTED:
      *sl_wifi_event = SL_WIFI_CLIENT_CONNECTED;
      break;
    case RSI_WLAN_RSP_CLIENT_DISCONNECTED:
      *sl_wifi_event = SL_WIFI_CLIENT_DISCONNECTED;
      break;
    case RSI_WLAN_RSP_TWT_ASYNC:
      *sl_wifi_event = SL_WIFI_TWT_RESPONSE_EVENT;
      break;
    case RSI_WLAN_RSP_FILTER_BCAST_PACKETS:
      *sl_wifi_event = SL_WIFI_FILTER_BCAST_RESPONSE_EVENT;
      break;
    default:
      return SL_STATUS_FAIL;
  }
  return SL_STATUS_OK;
}

sl_status_t save_sl_wifi_rate(sl_wifi_rate_t transfer_rate)
{
  if (transfer_rate == SL_WIFI_RATE_INVALID) {
    return SL_STATUS_INVALID_CONFIGURATION;
  }

  saved_wifi_data_rate = transfer_rate;

  return SL_STATUS_OK;
}

sl_status_t get_saved_sl_wifi_rate(sl_wifi_rate_t *transfer_rate)
{
  ARGS_CHECK_NULL_POINTER(transfer_rate);

  *transfer_rate = saved_wifi_data_rate;
  return SL_STATUS_OK;
}

void reset_sl_wifi_rate()
{
  saved_wifi_data_rate = 0;
}

sl_status_t save_ap_configuration(const sl_wifi_ap_configuration_t *wifi_ap_configuration)
{
  ARGS_CHECK_NULL_POINTER(wifi_ap_configuration);

  ap_configuration = *wifi_ap_configuration;
  return SL_STATUS_OK;
}

sl_status_t get_saved_ap_configuration(sl_wifi_ap_configuration_t *wifi_ap_configuration)
{
  ARGS_CHECK_NULL_POINTER(wifi_ap_configuration);

  *wifi_ap_configuration = ap_configuration;
  return SL_STATUS_OK;
}

void reset_ap_configuration()
{
  ap_configuration = (sl_wifi_ap_configuration_t){ 0 };
}

sl_status_t get_rate_protocol_and_data_rate(const uint8_t data_rate,
                                            sl_wifi_rate_protocol_t *rate_protocol,
                                            sl_wifi_rate_t *mask)
{
  switch (data_rate) {
    case SL_WIFI_AUTO_RATE: {
      *rate_protocol = SL_WIFI_RATE_PROTOCOL_AUTO;
      break;
    }
    case SL_WIFI_RATE_11B_1:
    case SL_WIFI_RATE_11B_2:
    case SL_WIFI_RATE_11B_5_5:
    case SL_WIFI_RATE_11B_11: {
      *rate_protocol = SL_WIFI_RATE_PROTOCOL_B_ONLY;
      break;
    }
    case SL_WIFI_RATE_11G_6:
    case SL_WIFI_RATE_11G_9:
    case SL_WIFI_RATE_11G_12:
    case SL_WIFI_RATE_11G_18:
    case SL_WIFI_RATE_11G_24:
    case SL_WIFI_RATE_11G_36:
    case SL_WIFI_RATE_11G_48:
    case SL_WIFI_RATE_11G_54: {
      *rate_protocol = SL_WIFI_RATE_PROTOCOL_G_ONLY;
      break;
    }
    case SL_WIFI_RATE_11N_MCS0:
    case SL_WIFI_RATE_11N_MCS1:
    case SL_WIFI_RATE_11N_MCS2:
    case SL_WIFI_RATE_11N_MCS3:
    case SL_WIFI_RATE_11N_MCS4:
    case SL_WIFI_RATE_11N_MCS5:
    case SL_WIFI_RATE_11N_MCS6:
    case SL_WIFI_RATE_11N_MCS7: {
      *rate_protocol = SL_WIFI_RATE_PROTOCOL_N_ONLY;
      break;
    }
    case SL_WIFI_RATE_11AX_MCS0:
    case SL_WIFI_RATE_11AX_MCS1:
    case SL_WIFI_RATE_11AX_MCS2:
    case SL_WIFI_RATE_11AX_MCS3:
    case SL_WIFI_RATE_11AX_MCS4:
    case SL_WIFI_RATE_11AX_MCS5:
    case SL_WIFI_RATE_11AX_MCS6:
    case SL_WIFI_RATE_11AX_MCS7: {
      *rate_protocol = SL_WIFI_RATE_PROTOCOL_AX_ONLY;
      break;
    }
    default: {
      return SL_STATUS_INVALID_CONFIGURATION;
    }
  }

  *mask = data_rate;

  return SL_STATUS_OK;
}

void convert_performance_profile_to_power_save_command(sl_performance_profile_t profile,
                                                       si91x_power_save_request_t *power_save_request)
{
  SL_ASSERT(power_save_request != NULL);

  power_save_request->monitor_interval  = DEFAULT_MONITOR_INTERVAL;
  power_save_request->ulp_mode_enable   = SL_ULP_WITH_RAM_RETENTION;
  power_save_request->dtim_aligned_type = performance_profile.wifi_performance_profile.dtim_aligned_type;
  power_save_request->num_of_dtim_skip  = performance_profile.wifi_performance_profile.num_of_dtim_skip;
  power_save_request->listen_interval   = performance_profile.wifi_performance_profile.listen_interval;
  power_save_request->psp_type          = SL_MAX_PSP;

  switch (profile) {
    case HIGH_PERFORMANCE: {
      memset(power_save_request, 0, sizeof(si91x_power_save_request_t));
      break;
    }

    case ASSOCIATED_POWER_SAVE: {
#ifdef RSI_M4_INTERFACE
      power_save_request->power_mode = SL_CONNECTED_M4_BASED_PS;
#else
      power_save_request->power_mode = SL_CONNECTED_GPIO_BASED_PS;
#endif
      break;
    }

    case ASSOCIATED_POWER_SAVE_LOW_LATENCY: {
#ifdef RSI_M4_INTERFACE
      power_save_request->power_mode = SL_CONNECTED_M4_BASED_PS;
#else
      power_save_request->power_mode = SL_CONNECTED_GPIO_BASED_PS;
#endif
      power_save_request->psp_type = SL_FAST_PSP;
      break;
    }

    case STANDBY_POWER_SAVE: {
#ifdef RSI_M4_INTERFACE
      power_save_request->power_mode = SL_M4_BASED_DEEP_SLEEP;
#else
      power_save_request->power_mode = SL_GPIO_BASED_DEEP_SLEEP;
#endif
      power_save_request->ulp_mode_enable = SL_ULP_WITHOUT_RAM_RET_RETENTION;
      break;
    }

    case STANDBY_POWER_SAVE_WITH_RAM_RETENTION: {
#ifdef RSI_M4_INTERFACE
      power_save_request->power_mode = SL_M4_BASED_DEEP_SLEEP;
#else
      power_save_request->power_mode = SL_GPIO_BASED_DEEP_SLEEP;
#endif
      break;
    }
    default: {
      // Do nothing
    } break;
  }

  return;
}

/*********************************************************************************************
 * @brief
 * 	 An utility function powersave request to TA firmware. Host has to disable powersave before sending powersave mode
 * @param profile
 *   performance profile which needs to be converted to its equivalent si91x_power_save_request structure and send to TA
 * @return
 * 	 sl_status_t
 *********************************************************************************************/
sl_status_t sl_si91x_send_power_save_request(sl_performance_profile_t profile)
{
  sl_status_t status;
  si91x_power_save_request_t power_save_request = { 0 };
  // Set power save mode to disable first
  status = sl_si91x_driver_send_command(RSI_COMMON_REQ_PWRMODE,
                                        SI91X_COMMON_CMD_QUEUE,
                                        &power_save_request,
                                        sizeof(si91x_power_save_request_t),
                                        SL_SI91X_WAIT_FOR_COMMAND_SUCCESS,
                                        NULL,
                                        NULL);
  VERIFY_STATUS_AND_RETURN(status);
  // Power save need not to be sent if the current_performance_profile is HIGH_PERFORMANCE
  if (profile == HIGH_PERFORMANCE) {
    current_performance_profile = profile;
    return SL_STATUS_OK;
  }

  convert_performance_profile_to_power_save_command(profile, &power_save_request);

  status = sl_si91x_driver_send_command(RSI_COMMON_REQ_PWRMODE,
                                        SI91X_COMMON_CMD_QUEUE,
                                        &power_save_request,
                                        sizeof(si91x_power_save_request_t),
                                        SL_SI91X_WAIT_FOR_COMMAND_SUCCESS,
                                        NULL,
                                        NULL);
  VERIFY_STATUS_AND_RETURN(status);
  current_performance_profile = profile;
  return status;
}
