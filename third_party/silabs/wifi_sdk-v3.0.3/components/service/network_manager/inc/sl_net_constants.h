/***************************************************************************/ /**
 * @file
 * @brief Networking constants
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
#pragma once

#include <stdint.h>
#include <limits.h>
#include "sl_wifi_constants.h"

#define NETWORK_INTERFACE_TYPE_MASK 0xFFF8
#define SL_NET_INTERFACE_TYPE(x)    (x & NETWORK_INTERFACE_TYPE_MASK)

#define SL_NET_WIFI_CLIENT_INTERFACE(x) (SL_NET_DEFAULT_WIFI_CLIENT_INTERFACE + x)
#define SL_NET_WIFI_AP_INTERFACE(x)     (SL_NET_DEFAULT_WIFI_AP_INTERFACE + x)
#define SL_NET_ETHERNET_INTERFACE(x)    (SL_NET_DEFAULT_ETHERNET_INTERFACE + x)
#define SL_NET_THREAD_INTERFACE(x)      (SL_NET_DEFAULT_THREAD_INTERFACE + x)
#define SL_NET_BLUETOOTH_INTERFACE(x)   (SL_NET_DEFAULT_BLUETOOTH_INTERFACE + x)
#define SL_NET_ZWAVE_INTERFACE(x)       (SL_NET_DEFAULT_ZWAVE_INTERFACE + x)

/** \addtogroup SL_NET_CONSTANTS Constants
 * @{ */

/// Enumeration of network interfaces
typedef enum {
  SL_NET_DEFAULT_WIFI_CLIENT_INTERFACE = (1 << 3),
  SL_NET_DEFAULT_WIFI_AP_INTERFACE     = (2 << 3),
  SL_NET_DEFAULT_ETHERNET_INTERFACE    = (3 << 3),
  SL_NET_DEFAULT_THREAD_INTERFACE      = (4 << 3),
  SL_NET_DEFAULT_BLUETOOTH_INTERFACE   = (5 << 3),
  SL_NET_DEFAULT_ZWAVE_INTERFACE       = (6 << 3),
} sl_net_interface_t;

/** @} */

#define SL_NET_DEFAULT_WIFI_CLIENT_INTERFACE (1 << 3)
#define SL_NET_DEFAULT_WIFI_AP_INTERFACE     (2 << 3)
#define SL_NET_DEFAULT_ETHERNET_INTERFACE    (3 << 3)
#define SL_NET_DEFAULT_THREAD_INTERFACE      (4 << 3)
#define SL_NET_DEFAULT_BLUETOOTH_INTERFACE   (5 << 3)
#define SL_NET_DEFAULT_ZWAVE_INTERFACE       (6 << 3)

/** \addtogroup SL_NET_CONSTANTS Constants
 * @{ */

/// Enumeration of network packet types
typedef enum {
  SL_NET_UDP_PACKET,
  SL_NET_TCP_PACKET,
  SL_NET_ETHERNET_PACKET,
  SL_NET_TLS_PACKET,
  SL_NET_DTLS_PACKET,
  SL_NET_THREAD_PACKET,
  SL_NET_BLUETOOTH_PACKET,
  SL_NET_ZWAVE_PACKET,
  SL_NET_ZIGBEE_PACKET,
  SL_NET_6LOWPAN_PACKET,
  SL_NET_RAW_PACKET,
} sl_net_packet_type_t;

/// Enumeration of IP address resolution methods
typedef enum {
  SL_NET_AUTOMATIC_ADDRESS_RESOLUTION,
  SL_NET_STATIC_ADDRESS_RESOLUTION,
  SL_NET_DHCP_ADDRESS_RESOLUTION = SL_NET_AUTOMATIC_ADDRESS_RESOLUTION,
  SL_NET_LINK_LOCAL_ADDRESS_RESOLUTION,
} sl_net_address_resolution_t;

typedef enum { SL_NET_DNS_TYPE_IPV4, SL_NET_DNS_TYPE_IPV6 } sl_net_dns_resolution_ip_type_t;

typedef enum {
  SL_NET_INTERFACE_UP_EVENT,
  SL_NET_INTERFACE_DOWN_EVENT,
  SL_NET_IP_UP_EVENT,
  SL_NET_IP_DOWN_EVENT,
  SL_NET_IP_EXPIRED_EVENT,
  SL_NET_PING_RESPONSE_EVENT,
  SL_NET_WIFI_CLIENT_CONNECTED_EVENT,
  SL_NET_WIFI_CLIENT_DISCONNECTED_EVENT,
  SL_NET_DNS_RESOLVE_EVENT,
} sl_net_event_t;

/// SL Network profile ID
typedef enum {
  SL_NET_PROFILE_ID_0  = 0,
  SL_NET_PROFILE_ID_1  = 1,
  SL_NET_PROFILE_ID_2  = 2,
  SL_NET_PROFILE_ID_3  = 3,
  SL_NET_PROFILE_ID_4  = 4,
  SL_NET_PROFILE_ID_5  = 5,
  SL_NET_PROFILE_ID_6  = 6,
  SL_NET_PROFILE_ID_7  = 7,
  SL_NET_PROFILE_ID_8  = 8,
  SL_NET_PROFILE_ID_9  = 9,
  SL_NET_PROFILE_ID_10 = 10,

  SL_NET_DEFAULT_WIFI_CLIENT_PROFILE = SL_NET_PROFILE_ID_0,
  SL_NET_DEFAULT_WIFI_AP_PROFILE     = SL_NET_PROFILE_ID_0,
  SL_NET_DEFAULT_ETHERNET_PROFILE    = SL_NET_PROFILE_ID_0,
  SL_NET_DEFAULT_THREAD_PROFILE      = SL_NET_PROFILE_ID_0,
  SL_NET_DEFAULT_ZWAVE_PROFILE       = SL_NET_PROFILE_ID_0,
} sl_net_profile_id_t;

/// Enumeration of network credential types
typedef enum {
  SL_NET_INVALID_CREDENTIAL_TYPE,
  SL_NET_WIFI_PSK,
  SL_NET_WIFI_PMK,
  SL_NET_WIFI_WEP,
  SL_NET_WIFI_EAP,
  SL_NET_WIFI_PEAP,
  SL_NET_MQTT_CLIENT_CREDENTIALS,
  SL_NET_HTTP_CLIENT_CREDENTIALS,
  SL_NET_TLS_SERVER_CERTIFICATE,
  SL_NET_TLS_SERVER_KEY,
  SL_NET_TLS_CLIENT_CERTIFICATE,
  SL_NET_TLS_CLIENT_KEY,
  SL_NET_TLS_ROOT_CERTIFICATE,
} sl_net_credential_type_t;

/// Enumeration of network credential identifiers
typedef enum {
  SL_NET_INVALID_CREDENTIAL_ID = 0,
  SL_NET_DEFAULT_WIFI_CLIENT_CREDENTIAL_ID,
  SL_NET_DEFAULT_WIFI_AP_CREDENTIAL_ID,
  SL_NET_MQTT_SERVER_CERTIFICATE_ID,
  SL_NET_MQTT_SERVER_KEY_ID,
  SL_NET_MQTT_CLIENT_CREDENTIALS_ID,
  SL_NET_HTTP_CLIENT_CREDENTIAL_ID,
  SL_NET_USER_CREDENTIAL_ID = (int)(1u << 7),
} sl_net_credential_id_t;

typedef uint32_t sl_net_certificate_id_t;

/** @} */
