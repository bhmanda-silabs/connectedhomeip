/***************************************************************************/ /**
 * @file
 * @brief  Twt Tcp Client Application
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
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
#include <string.h>
#include <stdint.h>
#include "sl_si91x_socket.h"
#include "errno.h"
#include "sl_wifi_callback_framework.h"
#include "sl_status.h"
#include "sl_board_configuration.h"
#include "cmsis_os2.h"
#include "string.h"
#include "sl_wifi.h"
#include "socket.h"
#include "sl_utility.h"
#include "sl_net.h"
#include "sl_ip_types.h"
#include "sl_string.h"
#include "sl_net_si91x.h"

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#define SERVER_IP         "192.168.0.247"
#define SERVER_PORT       5001
#define NUMBER_OF_PACKETS 1000
#define DATA              "hello from tcp client"
#define TWT_SCAN_TIMEOUT  10000
#define SEND_TCP_DATA     0

/******************************************************
 *               Variable Definitions
 ******************************************************/
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

sl_wifi_twt_request_t default_twt_setup_configuration = {
  .twt_enable              = 1,
  .twt_flow_id             = 1,
  .wake_duration           = 0x60,
  .wake_duration_unit      = 0,
  .wake_duration_tol       = 0x60,
  .wake_int_exp            = 13,
  .wake_int_exp_tol        = 13,
  .wake_int_mantissa       = 0x1D4C,
  .wake_int_mantissa_tol   = 0x1D4C,
  .implicit_twt            = 1,
  .un_announced_twt        = 1,
  .triggered_twt           = 0,
  .twt_channel             = 0,
  .twt_protection          = 0,
  .restrict_tx_outside_tsp = 1,
  .twt_retry_limit         = 6,
  .twt_retry_interval      = 10,
  .req_type                = 1,
  .negotiation_type        = 0,
};
volatile bool twt_results_complete   = false;
volatile sl_status_t callback_status = SL_STATUS_OK;
/******************************************************
  *               Function Declarations
  ******************************************************/
void application_start();
sl_status_t set_twt(void);
static sl_status_t show_twt_results(si91x_twt_response_t *twt_result);
static sl_status_t twt_callback_handler(sl_wifi_event_t event,
                                        si91x_twt_response_t *result,
                                        uint32_t result_length,
                                        void *arg);

/******************************************************
  *               Function Definitions
  ******************************************************/

void app_init(const void *unused)
{
  UNUSED_PARAMETER(unused);
  osThreadNew((osThreadFunc_t)application_start, NULL, &thread_attributes);
}

void application_start()
{
  sl_status_t status;

  status = sl_net_init(SL_NET_DEFAULT_WIFI_CLIENT_INTERFACE, &sl_wifi_default_client_configuration, NULL, NULL);
  if (status != SL_STATUS_OK) {
    printf("Failed to start Wi-Fi client interface: 0x%lx\r\n", status);
    return;
  }
  printf("Wi-Fi Init Done\r\n");

  status = sl_net_up(SL_NET_DEFAULT_WIFI_CLIENT_INTERFACE, 0);
  if (status != SL_STATUS_OK) {
    printf("Failed to bring Wi-Fi client interface up: 0x%lx\r\n", status);
    return;
  }
  printf("Wi-Fi Client Connected\r\n");

  status = set_twt();
  if (status != SL_STATUS_OK) {
    printf("\r\nError while configuring TWT Parmas: 0x%lx \r\n", status);
    return;
  }
  printf("\r\nTWT Config Done\r\n");

  status = sl_net_deinit(SL_NET_DEFAULT_WIFI_CLIENT_INTERFACE, NULL);
  if (status != SL_STATUS_OK) {
    printf("\r\nError while wifi deinit: 0x%lx \r\n", status);
    return;
  }
  printf("\r\nSL NET Deinit Success\r\n");
}

sl_status_t set_twt(void)
{
  int client_socket                                 = -1;
  int return_value                                  = 0;
  sl_ipv4_address_t ip                              = { 0 };
  sl_wifi_performance_profile_t performance_profile = { 0 };
  sl_status_t status                                = SL_STATUS_OK;

  struct sockaddr_in server_address = { 0 };

  convert_string_to_sl_ipv4_address(SERVER_IP, &ip);

  server_address.sin_family      = AF_INET;
  server_address.sin_port        = SERVER_PORT;
  server_address.sin_addr.s_addr = ip.value;

  //!Create socket
  client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  printf("\n Client Socket: %d\n", client_socket);
  if (client_socket < 0) {
    printf("\r\nSocket Create failed with bsd error: %d\r\n", errno);
    return SL_STATUS_FAIL;
  }

  //! Socket connect
  return_value = connect(client_socket, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in));
  if (return_value < 0) {
    printf("\r\nSocket Connect failed with bsd error: %d\r\n", errno);
    close(client_socket);
    return SL_STATUS_FAIL;
  }
  printf("\r\n Socket Connected\n");

  //! Set TWT Config
  sl_wifi_set_twt_config_callback(twt_callback_handler, NULL);
  performance_profile.twt_request = default_twt_setup_configuration;
  status                          = sl_wifi_enable_target_wake_time(&performance_profile.twt_request);
  if (SL_STATUS_IN_PROGRESS == status) {
    const uint32_t start = osKernelGetTickCount();

    while (!twt_results_complete && (osKernelGetTickCount() - start) <= TWT_SCAN_TIMEOUT) {
      osThreadYield();
    }

    status = twt_results_complete ? callback_status : SL_STATUS_TIMEOUT;
  }
  VERIFY_STATUS_AND_RETURN(status);
  printf("\nTWT Setup Successful\n");

  //! Enable Broadcast data filter
  status = sl_wifi_filter_broadcast(5000, 1, 1);
  VERIFY_STATUS_AND_RETURN(status);
  printf("\r\nEnabled Broadcast Data Filter\n");

  //! Apply power save profile
  performance_profile.profile = ASSOCIATED_POWER_SAVE;
  status                      = sl_wifi_set_performance_profile(&performance_profile);
  if (status != SL_STATUS_OK) {
    printf("\r\nPowersave Configuration Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
  printf("\r\nAssociated Power Save Enabled\n");

  //! send data
#if SEND_TCP_DATA
  int sent_bytes   = 0;
  int packet_count = 0;
  while (packet_count < NUMBER_OF_PACKETS) {
    sent_bytes = send(client_socket, DATA, strlen(DATA), 0);
    if (sent_bytes < 0) {
      PRINT_STATUS(ERROR_TAG, SL_STATUS_FAIL);
      close(client_socket);
      return SL_STATUS_FAIL;
    }
    packet_count++;
  }
  printf("\r\n Data sent successfully\r\n");
#endif

  return SL_STATUS_OK;
}

static sl_status_t twt_callback_handler(sl_wifi_event_t event,
                                        si91x_twt_response_t *result,
                                        uint32_t result_length,
                                        void *arg)
{
  UNUSED_PARAMETER(result_length);
  UNUSED_PARAMETER(arg);
  if (CHECK_IF_EVENT_FAILED(event)) {
    callback_status      = *(sl_status_t *)result;
    twt_results_complete = true;
    return SL_STATUS_FAIL;
  }
  callback_status = show_twt_results(result);

  twt_results_complete = true;
  return SL_STATUS_OK;
}

static sl_status_t show_twt_results(si91x_twt_response_t *response)
{
  printf("\r\n wake duration : 0x%x", response->wake_duration);
  printf("\r\n wake_duration_unit: 0x%x", response->wake_duration_unit);
  printf("\r\n wake_int_exp : 0x%x", response->wake_int_exp);
  printf("\r\n negotiation_type : 0x%x", response->negotiation_type);
  printf("\r\n wake_int_mantissa : 0x%x", response->wake_int_mantissa);
  printf("\r\n implicit_twt : 0x%x", response->implicit_twt);
  printf("\r\n un_announced_twt : 0x%x", response->un_announced_twt);
  printf("\r\n triggered_twt : 0x%x", response->triggered_twt);
  printf("\r\n twt_channel : 0x%x", response->twt_channel);
  printf("\r\n twt_protection : 0x%x", response->twt_protection);
  printf("\r\n twt_flow_id : 0x%x\r\n", response->twt_flow_id);
  return SL_STATUS_OK;
}
