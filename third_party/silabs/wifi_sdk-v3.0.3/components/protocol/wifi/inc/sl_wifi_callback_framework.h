#pragma once

#include "sl_wifi_host_interface.h"
#include "sl_wifi_device.h" // To access the device specific structs
#include <stdint.h>

/** \addtogroup WIFI_CALLBACK_FRAMEWORK Callback Framework
  * \ingroup SL_WIFI
  * @{ */

/// Generic macro for callback functions to check if the event has Failed.
#define CHECK_IF_EVENT_FAILED(event) ((event & SL_WIFI_FAIL_STATUS_INDICATION) ? true : false)

/// Generic Wi-Fi callback function type
typedef sl_status_t (*sl_wifi_callback_function_t)(sl_wifi_event_t event, void *data, uint32_t data_length, void *arg);

/// Wi-Fi scan result callback type.
/// Incase of event failure, SL_WIFI_FAIL_EVENT_STATUS_INDICATION bit is set in event, data will be of type sl_status_t and data_length can be ignored
typedef sl_status_t (*sl_wifi_scan_callback_t)(sl_wifi_event_t event,
                                               sl_wifi_scan_result_t *data,
                                               uint32_t data_length,
                                               void *arg);

/// Wi-Fi receive stats callback type.
/// Incase of event failure, SL_WIFI_FAIL_EVENT_STATUS_INDICATION bit is set in event, data will be of type sl_status_t and data_length can be ignored
typedef sl_status_t (*sl_wifi_recieve_stats_callback_t)(sl_wifi_event_t event,
                                                        sl_wifi_per_stats_response *data,
                                                        uint32_t data_length,
                                                        void *arg);

/// Wi-Fi join callback type.
/// Incase of event failure, SL_WIFI_FAIL_EVENT_STATUS_INDICATION bit is set in event, data will be of type sl_status_t and data_length can be ignored
typedef sl_status_t (*sl_wifi_join_callback_t)(sl_wifi_event_t event, char *data, uint32_t data_length, void *arg);

/// TWT Response callback type.
/// Incase of event failure, SL_WIFI_FAIL_EVENT_STATUS_INDICATION bit is set in event, data will be of type sl_status_t and data_length can be ignored
typedef sl_status_t (*sl_wifi_twt_config_callback_t)(sl_wifi_event_t event,
                                                     si91x_twt_response_t *data,
                                                     uint32_t data_length,
                                                     void *arg);

/***************************************************************************/ /**
 * @brief
 *   Registers a function and optional argument for a selected callback
 * @param[in] event
 *   ID of the event. See \ref sl_wifi_event_t
 * @param[in] function
 *   Function pointer to callback
 * @param[in] optional_arg
 *   Optional user provided argument. This will be passed back to callback handler.
 * @pre 
 *   @ref sl_wifi_init should be called before this API.
 * @return
 *   sl_status_t. See https://docs.silabs.com/gecko-platform/4.1/common/api/group-status for details.
 ******************************************************************************/
sl_status_t sl_wifi_set_callback(sl_wifi_event_t event, sl_wifi_callback_function_t function, void *optional_arg);

/// Default Wi-Fi event handler
extern sl_status_t default_wifi_event_handler(sl_wifi_event_t event, sl_wifi_buffer_t *buffer);

static inline sl_status_t sl_wifi_set_scan_callback(sl_wifi_scan_callback_t function, void *optional_arg)
{
  return sl_wifi_set_callback(SL_WIFI_SCAN_RESULT_EVENT, (sl_wifi_callback_function_t)function, optional_arg);
}

static inline sl_status_t sl_wifi_set_join_callback(sl_wifi_join_callback_t function, void *optional_arg)
{
  return sl_wifi_set_callback(SL_WIFI_JOIN_EVENT, (sl_wifi_callback_function_t)function, optional_arg);
}

static inline sl_status_t sl_wifi_set_twt_config_callback(sl_wifi_twt_config_callback_t function, void *optional_arg)
{
  return sl_wifi_set_callback(SL_WIFI_TWT_RESPONSE_EVENT, (sl_wifi_callback_function_t)function, optional_arg);
}

static inline sl_status_t sl_wifi_set_recieve_stats_callback(sl_wifi_recieve_stats_callback_t function,
                                                             void *optional_arg)
{
  return sl_wifi_set_callback(SL_WIFI_RECEIVE_STATS_RESPONSE_EVENT,
                              (sl_wifi_callback_function_t)function,
                              optional_arg);
}

/** @} */
