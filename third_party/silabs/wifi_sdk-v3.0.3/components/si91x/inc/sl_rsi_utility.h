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
#pragma once

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "sl_status.h"
#include "sl_constants.h"
#include "sl_si91x_host_interface.h"
#include "sl_si91x_protocol_types.h"
#include "sl_utility.h"
#include "sl_si91x_driver.h"
#include "sl_wifi_device.h"

extern sl_performance_profile_t current_performance_profile;

sl_status_t convert_si91x_wifi_client_info(sl_wifi_client_info_response *client_info_response,
                                           si91x_client_info_response *si91x_client_info_response);
sl_status_t convert_si91x_event_to_sl_wifi_event(rsi_wlan_cmd_response_t command, sl_wifi_event_t *sl_wifi_event);

sl_status_t save_sl_wifi_rate(sl_wifi_rate_t transfer_rate);
sl_status_t get_saved_sl_wifi_rate(sl_wifi_rate_t *transfer_rate);
void reset_sl_wifi_rate();

sl_status_t get_rate_protocol_and_data_rate(const uint8_t data_rate,
                                            sl_wifi_rate_protocol_t *rate_protocol,
                                            sl_wifi_rate_t *transfer_rate);

sl_status_t save_ap_configuration(const sl_wifi_ap_configuration_t *wifi_ap_configuration);
sl_status_t get_saved_ap_configuration(sl_wifi_ap_configuration_t *wifi_ap_confuguration);
void reset_ap_configuration();

void save_tcp_auto_close_choice(bool is_tcp_auto_close_enabled);
bool is_tcp_auto_close_enabled();
void set_card_ready_required(bool card_ready_required);
bool get_card_ready_required();

void save_max_tx_power(uint8_t max_tx_power);
uint8_t get_max_tx_power();
void reset_max_tx_power();

void save_wifi_current_performance_profile(const sl_wifi_performance_profile_t *profile);
void get_wifi_current_performance_profile(sl_wifi_performance_profile_t *profile);

void save_bt_current_performance_profile(const sl_bt_performance_profile_t *profile);
void get_bt_current_performance_profile(sl_bt_performance_profile_t *profile);

void get_coex_performance_profile(sl_performance_profile_t *profile);
void reset_coex_current_performance_profile(void);

void save_boot_configuration(const si91x_boot_configuration_t *boot_configuration);
void get_saved_boot_configuration(si91x_boot_configuration_t boot_configuration);

/*********************************************************************************************
 * @brief
 * 	 An utility function computes coex performance profile internally and converts into si91x_power_save_request_t.
 * @param profile
 *   performance profile which needs to be converted to its equivalent si91x_power_save_request structure.
 * @param power_save_request
 *   si91x specific structure that holds required configuration for the given performance profile.
 *   This is optional parameter.
 * @return coex_profile
 *   Coex power profile
 * @return
 * 	 sl_status_t
 *********************************************************************************************/
void convert_performance_profile_to_power_save_command(sl_performance_profile_t profile,
                                                       si91x_power_save_request_t *power_save_request);
sl_status_t sl_si91x_send_power_save_request(sl_performance_profile_t profile);

/******************************************************************************
 * A utility function to extract firmware status from RX packet.
 * The extracted firmware status can be given to convert_firmware_status() to get sl_status equivalent.
 * @param packet packet that contains the frame status which needs to be extracted.
 * @return  frame status
 *****************************************************************************/
static inline uint16_t get_si91x_frame_status(sl_si91x_packet_t *packet)
{
  return (uint16_t)(packet->desc[12] + (packet->desc[13] << 8));
}

/******************************************************************************
 * @brief
 *   A utility function that converts frame status sent by firmware to sl_status_t
 * @param[in] si91x_firmware_status
 *   si91x_firmware_status that needs to be converted to sl_status_t.
 * @return
 *   sl_status_t. See https://docs.silabs.com/gecko-platform/4.1/common/api/group-status for details.
 *****************************************************************************/
static inline sl_status_t convert_firmware_status(uint16_t si91x_firmware_status)
{
  SL_DEBUG_LOG("frame_status = %u\n", si91x_firmware_status);

  if (si91x_firmware_status == SL_STATUS_OK) {
    return si91x_firmware_status;
  } else {
    // Setting 16th bit to indicate si91x firmware status
    return (si91x_firmware_status | BIT(16));
  }
}

/**
 * An utility function to convert dBm value  to si91x specific power value
 * @param power_value_in_dbm power value with dBm as units.
 * @return si91x power level
 */
static inline uint8_t convert_dbm_to_si91x_power_level(uint8_t power_value_in_dBm)
{
  if (power_value_in_dBm >= SI91X_LOW_TRANSMIT_POWER_THRESHOLD) {
    return SL_SI91X_LOW_POWER_LEVEL;
  } else if (power_value_in_dBm >= SI91X_MEDIUM_TRANSMIT_POWER_THRESHOLD) {
    return SL_SI91X_MEDIUM_POWER_LEVEL;
  } else {
    return SL_SI91X_HIGH_POWER_LEVEL;
  }
}

/******************************************************************************
 * @brief
 *   A utility function that converts si91x_status_t to sl_status_t
 * @param[in] si91x_status
 *   si91x_status that needs to be converted to sl_status_t.
 * @return
 *   sl_status_t. See https://docs.silabs.com/gecko-platform/4.1/common/api/group-status for details.
 *****************************************************************************/
static inline sl_status_t convert_si91x_status_to_sl_status(si91x_status_t si91x_status)
{
  switch (si91x_status) {
    case RSI_ERROR_NONE:
      return SL_STATUS_OK;
    case RSI_ERROR_TIMEOUT:
      return SL_STATUS_TIMEOUT;
    case RSI_ERROR_INVALID_PARAM:
      return SL_STATUS_INVALID_PARAMETER;
    case RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE:
      return SL_STATUS_INVALID_STATE;
    case RSI_ERROR_PKT_ALLOCATION_FAILURE:
      return SL_STATUS_ALLOCATION_FAILED;
    case RSI_ERROR_COMMAND_NOT_SUPPORTED:
      return SL_STATUS_NOT_SUPPORTED;
    case RSI_ERROR_INSUFFICIENT_BUFFER:
      return SL_STATUS_NO_MORE_RESOURCE;
    case RSI_ERROR_IN_OS_OPERATION:
      return SL_STATUS_OS_OPERATION_FAILURE;
    case RSI_ERROR_BOOTUP_OPTIONS_NOT_SAVED:
      return SL_STATUS_BOOTUP_OPTIONS_NOT_SAVED;
    case RSI_ERROR_BOOTLOADER_VERSION_NOT_MATCHING:
      return SL_STATUS_BOOTLOADER_VERSION_MISMATCH;
    case RSI_ERROR_WAITING_FOR_BOARD_READY:
      return SL_STATUS_WAITING_FOR_BOARD_READY;
    case RSI_ERROR_VALID_FIRMWARE_NOT_PRESENT:
      return SL_STATUS_VALID_FIRMWARE_NOT_PRESENT;
    case RSI_ERROR_INVALID_OPTION:
      return SL_STATUS_INVALID_OPTION;
    case RSI_ERROR_SPI_BUSY:
      return SL_STATUS_SPI_BUSY;
    case RSI_ERROR_CARD_READY_TIMEOUT:
      return SL_STATUS_CARD_READY_TIMEOUT;
    case RSI_ERROR_FW_LOAD_OR_UPGRADE_TIMEOUT:
      return SL_STATUS_FW_LOAD_OR_UPGRADE_TIMEOUT;
    default:
      return SL_STATUS_FAIL;
  }
}
