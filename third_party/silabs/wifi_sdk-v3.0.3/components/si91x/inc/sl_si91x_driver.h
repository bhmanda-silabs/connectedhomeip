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

#include "sl_status.h"
#include "sl_wifi_device.h"
#include "sl_wifi_host_interface.h"
#include "sl_si91x_host_interface.h"

#define VERIFY_STATUS(s)   \
  do {                     \
    if (s != SL_STATUS_OK) \
      return s;            \
  } while (0);

typedef enum { SL_NCP_NORMAL_POWER_MODE, SL_NCP_LOW_POWER_MODE, SL_NCP_ULTRA_LOW_POWER_MODE } sl_si91x_power_mode_t;

typedef void (*sl_si91x_event_handler_t)(sl_si91x_event_t event, sl_status_t status, void *data, void *user_data);

typedef struct sl_si91x_power_configuration sl_si91x_power_configuration_t;

extern sl_si91x_performance_profile_t performance_profile;

/** \addtogroup SI91X_DRIVER_FUNCTIONS Core API
 * \ingroup SL_SI91X_API
 * @{ */

sl_status_t sl_si91x_driver_init(const sl_wifi_device_configuration_t *config, sl_wifi_event_handler_t event_handler);
sl_status_t sl_si91x_driver_deinit(void);

sl_status_t sl_si91x_driver_send_command(uint32_t command,
                                         si91x_queue_type_t queue_type,
                                         const void *data,
                                         uint32_t data_length,
                                         sl_si91x_wait_period_t wait_period,
                                         void *sdk_context,
                                         sl_wifi_buffer_t **data_buffer);
sl_status_t sl_si91x_driver_wait_for_response(rsi_wlan_cmd_request_t command, sl_si91x_wait_period_t wait_period);
sl_status_t sl_si91x_driver_send_socket_data(sl_si91x_socket_send_request_t *request,
                                             const void *data,
                                             uint32_t wait_time);
sl_status_t sl_si91x_driver_send_bt_command(rsi_wlan_cmd_request_t command,
                                            si91x_queue_type_t queue_type,
                                            sl_wifi_buffer_t *data);
sl_status_t sl_si91x_wifi_set_certificate_index(uint8_t certificate_type,
                                                uint8_t certificate_index,
                                                const uint8_t *buffer,
                                                uint32_t certificate_length);


sl_status_t sl_si91x_register_callback(sl_si91x_event_t event, sl_si91x_event_handler_t, void *user_data);
sl_status_t sl_si91x_default_handler(sl_si91x_event_t event, sl_wifi_buffer_t *buffer);

/** @} */

/** \addtogroup SI91X_RADIO_FUNCTIONS Radio API
 * \ingroup SL_SI91X_API
 * @{ */

sl_status_t sl_si91x_enable_radio(void);
sl_status_t sl_si91x_disable_radio(void);
sl_status_t sl_si91x_write_calibration_data(const si91x_calibration_data_t *data);

sl_status_t sl_si91x_set_device_region(sl_si91x_operation_mode_t operation_mode,
                                       sl_si91x_band_mode_t band,
                                       sl_si91x_region_code_t region_code);

sl_status_t sl_si91x_set_power_mode(sl_si91x_power_mode_t mode, const sl_si91x_power_configuration_t *config);

#ifdef __cplusplus
extern "C" {
#endif
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

#ifdef __cplusplus
}
#endif


/** @} */

/** \addtogroup SI91X_FIRMWARE_UPDATE_FUNCTIONS Firmware Update API
 * \ingroup SL_SI91X_API
 * @{ */

/***************************************************************************/ /**
 * @brief   
 *   Send the RPS header content of firmware file.This is a blocking API.
 * @param[in] rps_header   
 *   Pointer to the RPS header content
 * @return
 *   sl_status_t. See https://docs.silabs.com/gecko-platform/4.1/common/api/group-status for details.
 ******************************************************************************/
sl_status_t sl_si91x_fwup_start(uint8_t *rps_header);

/***************************************************************************/ /**
 * @brief      
 *   Send the firmware file content.This is a blocking API.
 * @param[in] content       
 *   Pointer to the firmware file content
 * @param[in] length     
 *   Length of the content
 * @return
 *   sl_status_t. See https://docs.silabs.com/gecko-platform/4.1/common/api/group-status for details.
 ******************************************************************************/
sl_status_t sl_si91x_fwup_load(uint8_t *content, uint16_t length);

/** @} */

/**
 * @brief      to write content on TA flash from M4. This is a blocking API.
 * @param[in]  write_address        - address at which data will be written on TA flash memory
 * @param[in]  write_data           - Input data
 * @param[in]  write_data_length    - total length
 * @return       sl_status_t. See https://docs.silabs.com/gecko-platform/4.1/common/api/group-status for details.
*/
sl_status_t sl_si91x_command_to_write_common_flash(uint32_t write_address,
                                                   uint8_t *write_data,
                                                   uint16_t write_data_length);

/*==============================================*/
/**
 * @brief      Secure handshake. This is a blocking API.
 * @param[in]  sub_cmd_type - Sub command
 * @param[in]  input_data  - Input data
 * @param[in]  input_len  - Length length
 * @param[in]  output_len  - Output length
 * @param[in]  output_data - Output data
 * @return       sl_status_t. See https://docs.silabs.com/gecko-platform/4.1/common/api/group-status for details.
 *
 */
sl_status_t sl_si91x_m4_ta_secure_handshake(uint8_t sub_cmd_type,
                                            uint8_t input_len,
                                            uint8_t *input_data,
                                            uint8_t output_len,
                                            uint8_t *output_data);

/** @} */

/**
 * @brief      Informs when assertion happens in the firmware.
 * @return     sl_status_t. See https://docs.silabs.com/gecko-platform/4.1/common/api/group-status for details.
*/
sl_status_t sl_si91x_assert(void);

/***************************************************************************/ /**
 * @brief       
 *   Get ram log on Firmware UART/UART2.
 *  @param[in]   address    
 *    Address in Si91x module
 *  @param[in]   length       
 *    Chunk length to read from Si91x module
 * @return
 *   sl_status_t. See https://docs.silabs.com/gecko-platform/4.1/common/api/group-status for details.
 ******************************************************************************/
sl_status_t sl_si91x_get_ram_log(uint32_t address, uint32_t length);
