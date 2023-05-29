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
#include "cmsis_os2.h"

typedef struct {
  uint32_t tv_sec;  /* Seconds      */
  uint32_t tv_usec; /* Microseconds */
} si91x_timeval_t;

typedef struct {
  osMutexId_t mutex;
  sl_ip_address_t remote_ip;
  sl_ip_address_t local_ip;
  uint16_t remote_port;
  uint16_t local_port;
  uint16_t socket_id;
  int32_t backlog;
  int32_t current_backlog_count;
  int32_t id;
  int32_t protocol_family;
  int32_t type;
  int32_t protocol;
  uint8_t is_primary_socket;
  bool is_connected;
  bool is_socket_terminated;
  uint16_t tcp_keepalive_initial_time;
  uint16_t read_timeout;
} si91x_iot_socket_t;
