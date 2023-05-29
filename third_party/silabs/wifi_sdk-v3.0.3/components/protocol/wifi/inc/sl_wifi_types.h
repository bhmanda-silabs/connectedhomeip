/*
 * EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND
 * CONDITIONS OF THE CONTROLLING LICENSE AGREEMENT FOUND AT LICENSE.md
 * IN THIS SDK. IF YOU DO NOT AGREE TO THE LICENSE TERMS AND CONDITIONS,
 * PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.
 * (c) Copyright 2018, Silicon Laboratories Inc.  All rights reserved.
 */

#pragma once

#include "sl_wifi_constants.h"
#include "sl_wifi_host_interface.h"
#include "sl_ieee802_types.h"
#include "sl_status.h"
#include "sl_ip_types.h"
#include <stdint.h>

/** @addtogroup SL_WIFI_TYPES Types
  * @{ */

/// Wi-Fi event handler
typedef sl_status_t (*sl_wifi_event_handler_t)(sl_wifi_event_t event, sl_wifi_buffer_t *buffer);

/// Wi-Fi credential handle
typedef uint32_t sl_wifi_credential_id_t;

/// Wi-Fi channel
typedef struct {
  uint16_t channel;              ///< Channel number
  sl_wifi_band_t band;           ///< Wi-Fi Radio Band
  sl_wifi_bandwidth_t bandwidth; ///< Channel bandwidth
} sl_wifi_channel_t;

/// SSID data structure
typedef struct {
  uint8_t value[32]; ///< SSID value
  uint8_t length;    ///< Length of SSID
} sl_wifi_ssid_t;

/// Roam configuration structure
typedef struct {
  int32_t
    trigger_level; ///< RSSI level to trigger roam algorithm, Setting the value to SL_WIFI_NEVER_ROAM will disable roaming configuration
  uint32_t trigger_level_change; ///< RSSI level delta change to trigger roam algorithm
} sl_wifi_roam_configuration_t;

/// Wi-Fi firmware version information
typedef struct {
  char version[32]; ///< Version string data
} sl_wifi_version_string_t;

/// Wi-Fi scan result
typedef struct {
  uint32_t scan_count; ///< Number of available scan results
  uint32_t reserved;   ///< Reserved
  struct {
    uint8_t rf_channel;
    uint8_t security_mode;
    uint8_t rssi_val;
    uint8_t network_type;
    uint8_t ssid[34];
    uint8_t bssid[6];
    uint8_t reserved[2];
  } scan_info[]; ///< Array of scan result data
} sl_wifi_scan_result_t;

/** Wi-Fi scan configuration.
 *  @note channel_bitmap_2g4 uses the lower 14 bits to represent channels from 1 - 14 where channel 1 = (1 << 0), channel 2 = (1 << 1), etc
 */
typedef struct {
  sl_wifi_scan_type_t type;
  uint32_t flags;
  uint32_t periodic_scan_interval; ///< Duration in milliseconds between periodic scans
  uint16_t channel_bitmap_2g4;     ///< Bitmap of selected 2.4GHz channels
  uint32_t channel_bitmap_5g[8];   ///< Bitmap of selected 5GHz channels
} sl_wifi_scan_configuration_t;

/// Wi-Fi advanced scan configuration options
typedef struct {
  int32_t trigger_level;         ///< RSSI level to trigger advanced scan
  uint32_t trigger_level_change; ///< RSSI level to trigger advanced scan
  uint16_t active_channel_time;  ///< Time spent on each channel when doing active scan (milliseconds)
  uint16_t passive_channel_time; ///< Time spent on each channel when doing passive scan (milliseconds)
  uint8_t enable_instant_scan;   ///< Flag to start advanced scan immediately
  uint8_t
    enable_multi_probe; ///< Flag to indicate to send multiple probes to AP, If set an probe request would be sent to all access points, in addition to connected SSID
} sl_wifi_advanced_scan_configuration_t;

/// Wi-Fi Access Point configuration
typedef struct {
  sl_wifi_ssid_t ssid;                   ///< SSID
  sl_wifi_security_t security;           ///< Security mode
  sl_wifi_encryption_t encryption;       ///< Encryption mode
  sl_wifi_channel_t channel;             ///< Channel
  sl_wifi_rate_protocol_t rate_protocol; ///< Rate protocol
  sl_wifi_ap_flag_t options;
  sl_wifi_credential_id_t credential_id; ///< ID of secure credentials
  uint8_t keepalive_type;                ///< Keep alive type of the access point
  uint16_t beacon_interval;              ///< Beacon interval of the access point in milliseconds
  uint32_t client_idle_timeout;          ///< Duration in milliseconds to kick idle client
  uint16_t dtim_beacon_count;            ///< How many beacons per DTIM
  uint8_t maximum_clients;               ///< Maximum number of associated clients
} sl_wifi_ap_configuration_t;

/// Wi-Fi Access Point advanced configuration
typedef struct {
  uint8_t csa_announcement_delay; ///< In beacon periods
  uint32_t tbd;                   ///< Advanced configuration option to be added
} sl_wifi_advanced_ap_configuration_t;

/// Wi-Fi Client interface configuration
typedef struct {
  sl_wifi_ssid_t ssid;             ///< SSID
  sl_wifi_channel_t channel;       ///< Channel
  sl_mac_address_t bssid;          ///< BSSID
  sl_wifi_bss_type_t bss_type;     ///< BSS type
  sl_wifi_security_t security;     ///< Security mode
  sl_wifi_encryption_t encryption; ///< Encryption mode
  sl_wifi_client_flag_t client_options;
  sl_wifi_credential_id_t credential_id; ///< ID of secure credentials
} sl_wifi_client_configuration_t;

/// Wi-Fi PSK security credentials
typedef struct {
  uint8_t value[SL_WIFI_MAX_PSK_LENGTH];
} sl_wifi_psk_credential_t;

/// Wi-Fi PMK security credentials
typedef struct {
  uint8_t value[SL_WIFI_MAX_PMK_LENGTH];
} sl_wifi_pmk_credential_t;

/// Wi-Fi WEP security credentials
typedef struct {
  uint8_t index[2];
  uint8_t key[SL_WEP_KEY_COUNT][SL_WEP_KEY_LENGTH];
} sl_wifi_wep_credential_t;

/// Wi-Fi Enterprise security credentials
typedef struct {
  uint8_t username[SL_EAP_USER_NAME_LENGTH];
  uint8_t password[SL_EAP_PASSWORD_LENGTH];
  uint32_t certificate_id;
} sl_wifi_eap_credential_t;

/// Wi-Fi security credentials
typedef struct {
  sl_wifi_credential_type_t type;
  union {
    sl_wifi_psk_credential_t psk;
    sl_wifi_pmk_credential_t pmk;
    sl_wifi_wep_credential_t wep;
    sl_wifi_eap_credential_t eap;
  };
} sl_wifi_credential_t;

/// TWT request structure to configure a session
typedef struct {
  uint8_t wake_duration;           ///< Nominal minimum wake duration. Range : 0 - 255
  uint8_t wake_duration_tol;       ///< Wake duration tolerance. Range : 0 - 255
  uint8_t wake_int_exp;            ///< Wake interval exponent. Range : 0 - 31
  uint8_t wake_int_exp_tol;        ///< Wake interval exponent tolerance. Range : 0 - 31
  uint16_t wake_int_mantissa;      ///< Wake interval mantissa. Range : 0 - 65535
  uint16_t wake_int_mantissa_tol;  ///< Wake interval mantissa tolerance. Range : 0 - 65535
  uint8_t implicit_twt;            ///< Implicit TWT : 0 or 1
  uint8_t un_announced_twt;        ///< Un-announced TWT : 0 or 1
  uint8_t triggered_twt;           ///< Triggered TWT : 0 or 1
  uint8_t negotiation_type;        ///< Negotiation Type : 0 - Individual TWT; 1 - Broadcast TWT
  uint8_t twt_channel;             ///< TWT Channel. Range : 0 - 7
  uint8_t twt_protection;          ///< TWT protection : 0 or 1
  uint8_t twt_flow_id;             ///< TWT Session Flow ID. 0 - 7 valid. 0xFF to disable all active TWT sessions.
  uint8_t restrict_tx_outside_tsp; ///< Restrict TX outside TSP : 0 or 1
  uint8_t twt_retry_limit;         ///< TWT Retry limit. Range : 0 - 15
  uint8_t twt_retry_interval;      ///< TWT retry interval. Range : 5 - 255
  uint8_t req_type;                ///< TWT Request type. 0 - Request TWT; 1 - Suggest TWT; 2 - Demand TWT
  uint8_t twt_enable;              ///< TWT Enable. 0 - TWT session teardown; 1 - TWT session setup
  uint8_t wake_duration_unit;      ///< Wake duration unit. 0 - 256 TU; 1 - 1024 TU
} sl_wifi_twt_request_t;

/// Wi-Fi device status
typedef struct {
  uint8_t client_active : 1;
  uint8_t ap_active : 1;
  uint8_t monitor_mode_active : 1;
  uint8_t wfd_go_active : 1;
  uint8_t wfd_client_active : 1;
  uint8_t scan_active : 1;
  uint8_t _reserved : 1;
  uint8_t _reserved2 : 1;
} sl_wifi_status_t;

/// Wi-Fi access point client information
typedef struct {
  sl_mac_address_t mac_addres;
  // TODO. Fill in client details
} sl_wifi_client_info_t;

/// Wi-Fi interface statistics
typedef struct {
  uint32_t beacon_lost_count; ///< Number of missed beacons
  uint32_t beacon_rx_count;   ///< Number of received beacons
  uint32_t mcast_rx_count;    ///< Multicast packets received
  uint32_t mcast_tx_count;    ///< Multicast packets transmitted
  uint32_t ucast_rx_count;    ///< Unicast packets received
  uint32_t ucast_tx_count;    ///< Unicast packets transmitted
  uint32_t
    overrun_count; ///< Number of packets dropped either at ingress or egress, due to lack of buffer memory to retain all packets.
} sl_wifi_statistics_t;

/// Wi-Fi Direct (P2P) configuration
typedef struct {
  uint16_t group_owner_intent; ///< Group owner intent
  const char *device_name;     ///< Device name
  sl_wifi_channel_t channel;   ///< Wi-Fi channel
  char ssid_suffix[6];         ///< SSID suffix
} sl_wifi_p2p_configuration_t;

/// Wi-Fi event data
typedef union {
  sl_wifi_scan_result_t scan_results;
  uint32_t join_status;
} sl_wifi_event_data_t;

/// Wi-Fi WPS PIN object that is an 8 digit number
typedef struct {
  char digits[8];
} sl_wifi_wps_pin_t;

/// Wi-Fi listen interval
typedef struct {
  uint8_t value;
  sl_wifi_listen_interval_time_unit_t time_units;
} sl_wifi_listen_interval_t;

typedef struct {
  sl_mac_address_t mac_adddress;
  sl_ip_address_t ip_address;
} sl_wifi_client_info;

typedef struct {
  uint8_t client_count;
  sl_wifi_client_info client_info[SL_MAX_CLIENT_COUNT];
} sl_wifi_client_info_response;

/// Wi-Fi per stats structure
typedef struct {
  uint16_t tx_pkts;
  uint8_t reserved_1[2];
  uint16_t tx_retries;
  uint16_t crc_pass;
  uint16_t crc_fail;
  uint16_t cca_stk;
  uint16_t cca_not_stk;
  uint16_t pkt_abort;
  uint16_t fls_rx_start;
  uint16_t cca_idle;
  uint8_t reserved_2[26];
  uint16_t rx_retries;
  uint8_t reserved_3[2];
  uint16_t cal_rssi;
  uint8_t reserved_4[4];
  uint16_t xretries;
  uint16_t max_cons_pkts_dropped;
  uint8_t reserved_5[2];
  uint16_t bss_broadcast_pkts;
  uint16_t bss_multicast_pkts;
  uint16_t bss_filter_matched_multicast_pkts;
} sl_wifi_per_stats_response;

/** @} */
