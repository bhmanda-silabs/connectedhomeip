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

#include "sl_si91x_status.h"
#include "sl_si91x_types.h"
#include "sl_si91x_protocol_types.h"
#include "sl_bit.h"
#include "sl_wifi_types.h"
#include <stdint.h>
#include <stddef.h>

#ifndef BIT
#define BIT(a) ((uint32_t)1U << a)
#endif

/** \addtogroup SI91X_CONSTANTS Constants
 * \ingroup SL_SI91X_TYPES
 * @{ */

/*=========================================================================*/
// feature bit map parameters description !//
/*=========================================================================*/
//! Security type open
//! Note: Supported in client mode
#define SL_SI91X_FEAT_SECURITY_OPEN BIT(0)

//! Security type WPA/WPA2
//! Note: Supported in client mode
#define SL_SI91X_FEAT_SECURITY_PSK BIT(1)

//! Aggregation support
#define SL_SI91X_FEAT_AGGREGATION BIT(2)

//! LP mode GPIO handshake
#define SL_SI91X_FEAT_LP_GPIO_BASED_HANDSHAKE BIT(3)

//! ULP mode GPIO based handshake
#define SL_SI91X_FEAT_ULP_GPIO_BASED_HANDSHAKE BIT(4)

//! To select ULP GPIO 1 for wake up indication
#define SL_SI91X_FEAT_DEV_TO_HOST_ULP_GPIO_1 BIT(5)

//! To supply 3.3 volt supply
#define SL_SI91X_FEAT_RF_SUPPY_VOL_3_3_VOLT BIT(6)

//! To Disable WPS in AP mode
#define SL_SI91X_FEAT_WPS_DISABLE BIT(7)

//! To enable EAP-LEAP mode
#define SL_SI91X_FEAT_EAP_LEAP_IN_COEX BIT(8)

//! Note: bits 9 - 31 are reserved

/*=========================================================================*/
// TCP/IP feature bit map parameters description !//
/*=========================================================================*/
//! TCP/IP bypass feature
#define SL_SI91X_TCP_IP_FEAT_BYPASS BIT(0)

//! Enable HTTP server feature
#define SL_SI91X_TCP_IP_FEAT_HTTP_SERVER BIT(1)

//! Enable DHCPv4 client feature
#define SL_SI91X_TCP_IP_FEAT_DHCPV4_CLIENT BIT(2)

//! Enable DHCPv6 client feature
#define SL_SI91X_TCP_IP_FEAT_DHCPV6_CLIENT BIT(3)

//! Enable DHCPv4 server feature
#define SL_SI91X_TCP_IP_FEAT_DHCPV4_SERVER BIT(4)

//! Enable DHCPv6 server feature
#define SL_SI91X_TCP_IP_FEAT_DHCPV6_SERVER BIT(5)

//! Enable JSON objects
#define SL_SI91X_TCP_IP_FEAT_JSON_OBJECTS BIT(6)

//! Enable HTTP client
#define SL_SI91X_TCP_IP_FEAT_HTTP_CLIENT BIT(7)

//! Enable DNS client
#define SL_SI91X_TCP_IP_FEAT_DNS_CLIENT BIT(8)

//! Enable SNMP client
#define SL_SI91X_TCP_IP_FEAT_SNMP_AGENT BIT(9)

//! Enable SSL feature
#define SL_SI91X_TCP_IP_FEAT_SSL BIT(10)

//! Enable ICMP feature(ping)
#define SL_SI91X_TCP_IP_FEAT_ICMP BIT(11)

//! Enable HTTP server
//! Note: Only supported in opermode 0
#define SL_SI91X_TCP_IP_FEAT_HTTPS_SERVER BIT(12)

//! Note: Bit 13 is reserved

//! Enable sending web page configuration to host from wireless config page
#define SL_SI91X_TCP_IP_FEAT_SEND_CONFIGS_TO_HOST BIT(14)

//! Enable FTP client
#define SL_SI91X_TCP_IP_FEAT_FTP_CLIENT BIT(15)

//! Enable SNTP client
#define SL_SI91X_TCP_IP_FEAT_SNTP_CLIENT BIT(16)

//! Enable  IPV6 support
//! Note: IPv6 will also get enabled if DHCP v6 client/DHCP v6 server is enabled irrespective of tcp_ip_feature_bit_map[17]
#define SL_SI91X_TCP_IP_FEAT_IPV6 BIT(17)

//! Enable  Raw data support
//! Note: This feature is supported only in AP mode. TCP_BYPASS feature should be disabled for this feature to be supported. If any packet from host with frame type 0x1 is received by firmware, the packet will be sent on air without TCP/IP stack processing. ARP and broadcast packets (other than DHCP packets) which are coming on air will be sent to host
#define SL_SI91X_TCP_IP_FEAT_RAW_DATA BIT(18)

//! Enable MDNSD
#define SL_SI91X_TCP_IP_FEAT_MDNSD BIT(19)

//! Enable SMTP client
#define SL_SI91X_TCP_IP_FEAT_SMTP_CLIENT BIT(20)

//! Select no of sockets
//! Note: Max of 10 sockets are allowed
//! Bits 21- 24 are used to set TOTAL_SOCKETS
#define SL_SI91X_TCP_IP_TOTAL_SOCKETS(total_sockets) (total_sockets << 21)

//! Enable Single SSL socket
#define SL_SI91X_TCP_IP_FEAT_SINGLE_SSL_SOCKET BIT(25)

//! Enable to Load public and private keys for TLS and SSL handshake
//! Note: If Secure handshake is with CA-certificate alone , then disable loading private and public keys and erase these certificates from the flash using load_cert API. Or if Secure handshake is needed for verification of Private and Public keys , then enable loading of private and public keys.
#define SL_SI91X_TCP_IP_FEAT_LOAD_PUBLIC_PRIVATE_CERTS BIT(26)

//! Enable to Load SSL certificates in to RAM
#define SL_SI91X_TCP_IP_FEAT_LOAD_CERTS_INTO_RAM BIT(27)

//! Note: Bit 28 is reserved

//! Enable POP3 client
#define SL_SI91X_TCP_IP_FEAT_POP3_CLIENT BIT(29)

//! Enable OTAF client
#define SL_SI91X_TCP_IP_FEAT_OTAF BIT(30)

#define SL_SI91X_TCP_IP_FEAT_EXTENSION_VALID BIT(31)

/*=========================================================================*/
// Custom feature bit map parameters description !//
/*=========================================================================*/
//! Note: Bit 0 -1 are reserved

//! Disables gateway config sent to STA from RSI AP
//! Note: If this bit is set to 1, the DHCP server behavior changes when the device is in AP mode. The DHCP server, when it assigns IP addresses to the client nodes, does not send out a Gateway address, and sends only the assigned IP and Subnet values to the client. It is highly recommended to keep this value at '0' as the changed behavior is required in only very specialized use cases and not in normal AP functionality. The default value of this bit is '0'.
#define SL_SI91X_CUSTOM_FEAT_DISABLE_GATEWAY_IN_RSI_AP BIT(2)

//! To configure the clock for NWP SOC 160Mhz
//! If higher performance is needed(like High throughput)
//! then this configuration is needed
//! Note: Need to set pll_mode to 1 in feature frame command
#define SL_SI91X_CUSTOM_FEAT_SOC_CLK_CONFIG_160MHZ BIT(4)
//! If this bit is set, AP is created in hidden mode
//! This bit is valid only in case of AP mode
#define SL_SI91X_CUSTOM_FEAT_AP_IN_HIDDEN_MODE BIT(5)

//! DNS server IP address in DHCP offer response in AP mode
#define SL_SI91X_CUSTOM_FEAT_DNS_SERVER_IN_DHCP_OFFER BIT(6)

//! Note: Bit 7 is reserved

//! Support for scanning in DFS channels() in 5GHZ band
//! This bit is valid in WiFi client mode
//! Note: it's mandatory to set region before scanning DFS channel.
#define SL_SI91X_CUSTOM_FEAT_DFS_CHANNEL_SUPPORT BIT(8)

//! If this bit is set it enables the LED blinking feature
//! after module initialisation.LED (GPIO_16) is used for
//! this feature and blinks when any TX Packet on air happens
//! and when we get a unicast packets addressing to our MAC
#define SL_SI91X_CUSTOM_FEAT_LED_FEATURE BIT(9)

//! If this bit is enabled,module indicates the host
//! the wlan connection status asynchronously
//! This bit is valid in case of Wifi client mode
#define SL_SI91X_CUSTOM_FEAT_ASYNC_CONNECTION_STATUS BIT(10)

//! Wake on wireless indication in UART mode
#define SL_SI91X_CUSTOM_FEAT_WAKE_ON_WIRELESS BIT(11)

//! Enables AP blacklisting in STA mode
//! Note:By default client maintains AP blacklist internally to avoid some access points.
//! To bypass AP blacklist feature in client mode during roaming or rejoin,
//! this feature should be enabled.
#define SL_SI91X_CUSTOM_FEAT_ENABLE_AP_BLACKLIST BIT(12)

//! Number of clients to support in AP/WFD mode
//! Bit 13 -16 are used to set MAX_NUM_OF_CLIENTS
#define SL_SI91X_CUSTOM_FEAT_MAX_NUM_OF_CLIENTS(max_num_of_clients) (max_num_of_clients << 13)

//! select between de-authentication or null data (with power management bit set) based roaming.
#define SL_SI91X_CUSTOM_FEAT_ROAM_WITH_DEAUTH_OR_NULLDATA BIT(17)

//! Note: Bit 18 - 19 are reserved

//! Trigger Auto Configuration
#define SL_SI91X_CUSTOM_FEAT_TRIGGER_AUTO_CONFIG BIT(20)

//! Note: Bit 21 is reserved

//! In AP mode, If set only two packets per STA will be buffered when STA is in PS
#define SL_SI91X_CUSTOM_FEAT_LIMIT_PACKETS_PER_STA BIT(22)

#define SL_SI91X_CUSTOM_FEAT_HTTP_HTTPS_AUTH BIT(23)

//! To configure the clock for NWP SOC 120Mhz
//! If higher performance is needed(like High throughput)
//! then this configuration is needed
//! Note: Need to set pll_mode to 1 in feature frame command
#define SL_SI91X_CUSTOM_FEAT_SOC_CLK_CONFIG_120MHZ BIT(24)

//! HTTP server credentials to host in get configuration command
#define SL_SI91X_CUSTOM_FEAT_HTTP_SERVER_CRED_TO_HOST BIT(25)

//! For a LTCP socket when maximum clients are connected if a new connection request is received, then this connection request will be rejected immediately
//! Note: By default this bit value is zero. When BIT[26] = 0: For a LTCP socket when maximum clients are connected if a new connection request is received, then this connection request will not be rejected. Instead device will maintain this connection request in LTCP pending list. This request will be served when any of the connected client is disconnected. When BIT[26] = 1: For a LTCP socket when maximum clients are connected if a new connection request is received, then this connection request will be rejected immediately. Device will not maintain this connection request in LTCP pending list.
#define SL_SI91X_CUSTOM_FEAT_REJECT_CONNECT_REQ_IMMEDIATELY BIT(26)

//! Enables Dual band roaming and vcsafd feature
#define SL_SI91X_CUSTOM_FEAT_DUAL_BAND_ROAM_VCSAFD BIT(27)

//! Real time clock from host
#define SL_SI91X_CUSTOM_FEAT_RTC_FROM_HOST BIT(28)

//! Custom feat BT IAP
#define SL_SI91X_CUSTOM_FEAT_BT_IAP BIT(29)

//! Note: Bit 30 is reserved

//! Extention valid to use Extended custom feature bitmap
#define SL_SI91X_FEAT_CUSTOM_FEAT_EXTENTION_VALID BIT(31)

/*=========================================================================*/

// Extended custom feature bitmap !//
/*=========================================================================*/
//! Note: Bit 0 is reserved

//! To support 4096 size RSA KEY certificate
#define SL_SI91X_EXT_FEAT_RSA_KEY_WITH_4096_SUPPORT BIT(1)

//! Extended custom bitmap to support TELEC
#define SL_SI91X_EXT_FEAT_TELEC_SUPPORT BIT(2)

//! To support 4096 size KEY SSL certificate
#define SL_SI91X_EXT_FEAT_SSL_CERT_WITH_4096_KEY_SUPPORT BIT(3)

//! Extended custom bitmap for AP Broadcast customization
//! Note: If this bit is enable then connected client who is in power save may miss the packet.
#define SL_SI91X_EXT_FEAT_AP_BROADCAST_PKT_SND_B4_DTIM BIT(4)

//! Extended custom bitmap to support FCC
#define SL_SI91X_EXT_FEAT_FCC_LOW_PWR BIT(5)

//! To enable PUF
#define SL_SI91X_EXT_FEAT_PUF BIT(7)

//! Nokia Spectral mask extended custom bitmap
#define SL_SI91X_EXT_FEAT_SPECTRAL_MASK_NOKIA BIT(8)

//! Extended feature bit map to skip default leading character '\' in HTTP header
#define SL_SI91X_EXT_HTTP_SKIP_DEFAULT_LEADING_CHARACTER BIT(9)

//! To enable PUF private key
#define SL_SI91X_EXT_FEAT_PUF_PRIVATE_KEY BIT(10)

//! To enable 802.11R Over The Air Roaming
//! Note: 1. Resource Request Support is not Present. 2. If both BIT[11] and BIT[16] are not enabled then it will select as Legacy Roaming.
#define SL_SI91X_EXT_FEAT_ENABLE_11R_OTA BIT(11)

//! To enable 802.11J protocol
//! Note: If this bit is enable, set region command is mandatory with setting it to Japan region and band value must be 1 (5GHz).
#define SL_SI91X_EXT_FEAT_IEEE_80211J BIT(12)

//! To enable 802.11W protocol
//! Note: This bit must be set for enabling WPA3 Personal Mode and WPA3 Personal Transition mode.
#define SL_SI91X_EXT_FEAT_IEEE_80211W BIT(13)

//! To enable the Multiverion TCP over SSL support
#define SL_SI91X_EXT_FEAT_SSL_VERSIONS_SUPPORT BIT(14)

//! To Enable 16 client support
//! Note: If this bit is enable then 16 stations can connect in AP mode otherwise Maximum of 8 stations can connect.
#define SL_SI91X_EXT_FEAT_16th_STATION_IN_AP_MODE BIT(15)

//! To enable 802.11R Over the Distribution System Roaming
//! Note: 1. Resource Request Support is not Present. 2. If both BIT[11] and BIT[16] are not enabled then it will select as Legacy Roaming.
#define SL_SI91X_EXT_FEAT_ENABLE_11R_ODS BIT(16)

//! Note: Bit 17 is reserved

//! To enable http otaf support
#define SL_SI91X_EXT_FEAT_HTTP_OTAF_SUPPORT BIT(18)

//! To enable low power mode in Wlan
//! Note: EXT_FEAT_LOW_POWER_MODE is not supported for 1.3 version chipset.
#define SL_SI91X_EXT_FEAT_LOW_POWER_MODE BIT(19)

#ifdef CHIP_9117
//! To enable 448K memory for TA
#define SL_SI91X_EXT_FEAT_448K_M4SS_256K BIT(21)
//! To enable 512K memory for TA
#define SL_SI91X_EXT_FEAT_512K_M4SS_192K BIT(20)
#ifndef RSI_M4_INTERFACE
// To enable 704K memory for TA; only supported in NCP
#define SL_SI91X_EXT_FEAT_704K_M4SS_0K (BIT(20) | BIT(21))
#endif
#define SL_SI91X_EXT_FEAT_256K_MODE 0
#else //defaults
//! Default memory configuration (RAM) is 192KB. User can set these bits to change the memory configuration as below:
//! ===============================
//! | Mode(KB) | BIT[20] | BIT[21] |
//! ===============================
//! | 192     | 0       | 0       |
//! | 256     | 0       | 1       |
//! | 320     | 1       | 0       |
//! | 384     | 1       | 1       |
//! ===============================
//! To enable 320K memory for TA
#define SL_SI91X_EXT_FEAT_320K_MODE BIT(20)
//! To enable 256K memory for TA
#define SL_SI91X_EXT_FEAT_256K_MODE BIT(21)
//! To enable 384K memory for TA
#define SL_SI91X_EXT_FEAT_384K_MODE (BIT(20) | BIT(21))
#endif

//! To enable CRYSTAL for TA
//! For 9117 EVK set EXT_FEAT_XTAL_CLK_ENABLE to BIT(22)
//! Note: If BIT[23] = 1 and BIT[22] = 0 in ext_custom_feature_bit_map, then user has to use UULP_GPIO_0 for sleep indication to host.
//! Bit 22 - 23 is used to set XTAL_CLK_ENABLE
//! To configure sleep clock source selection, either crystal clock or RC clock
//! =====================================================================
//! | Selection                                     | BIT[23] | BIT[22] |
//! =====================================================================
//! | Use RC clock as sleep clock                   | 0       | 0       |
//! | Use 32KHz clock from external XTAL OSCILLATOR | 0       | 1       |
//! | Use 32KHz bypass clock on UULP_GPIO_3         | 1       | 0       |
//! | Use 32KHz bypass clock on UULP_GPIO_4         | 1       | 1       |
//! =====================================================================

#define SL_SI91X_EXT_FEAT_XTAL_CLK_ENABLE(xtal_clk_enable) (xtal_clk_enable << 22)

//! To intimate FW not to modify MDNS text record
#define SL_SI91X_EXT_FEAT_HOMEKIT_WAC_ENABLED BIT(24)

//! To enable 1.8v support for TA
#define SL_SI91X_EXT_FEAT_1P8V_SUPPORT BIT(25)

//! Note: Bit 26 is reserved

//! To select UART debug prints pin selection
//! If BIT(27) is enabled,Debug prints are supported on UART1
//! If BIT(27) is disabled,Debug prints are supported on UART2
//! Note: By default all the debug prints from device network processor will be coming on UART2 if this bit is not enabled. UART1 pins are mapped to the following pins w.r.t to the device network processor. User needs to ensure that these pins are not used in MCU applications in SoC mode to avoid conflicts of pins usage based on the requirement. This bit is valid only if BIT[28] in ext_custom_feature_bit_map is set to 0. UART1-TX: GPIO_9 UART1-RX: GPIO_8 UART2-TX: GPIO_6 UART2-RX: GPIO_10 There is no functionality on rx pins for debug prints.
#define SL_SI91X_EXT_FEAT_UART_SEL_FOR_DEBUG_PRINTS BIT(27)
//! If this bit is enabled,NWP disables Debug prints support
#define SL_SI91X_EXT_FEAT_DISABLE_DEBUG_PRINTS BIT(28)
//! Note: Bit 29 - 30 is reserved

//! Enable BT Custom Features
#define SL_SI91X_EXT_FEAT_BT_CUSTOM_FEAT_ENABLE BIT(31)

/*=========================================================================*/
// Extended TCP/IP feature bit map parameters description !//
/*=========================================================================*/
#define SL_SI91X_EXT_TCP_FEAT_DHCP_OPT77 BIT(1)
//! Enable HTTP server root path (Default configuration page) bypass
#define SL_SI91X_EXT_TCP_IP_HTTP_SERVER_BYPASS BIT(2)
//! TCP bi-dir ack update
//! Note: Need to enable this bit if user wants to run the bi-directional data transfer.
#define SL_SI91X_EXT_TCP_IP_BI_DIR_ACK_UPDATE BIT(3)
//! TCP RX window division
#define SL_SI91X_EXT_TCP_IP_WINDOW_DIV BIT(4)
//! SSL server certificate bypass, validation from the host
#define SL_SI91X_EXT_TCP_IP_CERT_BYPASS BIT(5)
//! SSL 16K record size support
#define SL_SI91X_EXT_TCP_IP_SSL_16K_RECORD BIT(6)
//! Enable DNS client byass
#define SL_SI91X_EXT_TCP_IP_DNS_CLIENT_BYPASS BIT(7)
//! Enable TCP window scaling feature
//! Note: If this feature is not enabled, then the maximum possible rx window size is 64 KB. If user wants to use more than 64KB window size, tcp_rx_window_size_cap in socket configuration is used to increase the window size.
#define SL_SI91X_EXT_TCP_IP_WINDOW_SCALING BIT(8)
//! Enables both TCP/IP bypass mode & embedded modes
//! Note: Enabling this feature allows to use both bypass and non bypass modes simultaneously.
#define SL_SI91X_EXT_TCP_IP_DUAL_MODE_ENABLE BIT(9)
//! Enables Ethernet to WIFI bridge
#define SL_SI91X_EXT_TCP_IP_ETH_WIFI_BRIDGE BIT(10)
//! Enables the Dynamic coex memory
//! Note: To enable or disable the coex and update TCP RX window accordingly.
#define SL_SI91X_EXT_DYNAMIC_COEX_MEMORY BIT(11)
//! Configures the number of selects
//! Note: Bit 12 -15 are used for TOTAL_SELECTS and max value can be 10
#define SL_SI91X_EXT_TCP_IP_TOTAL_SELECTS(total_selects) (total_selects << 12)

//! To enable socket wait close
//! Note: If it is set socket will not be closed until close() is called from host. It is recommended to enable this bit when using TCP sockets.
//! This is always set internally for si91x chips
#define SL_SI91X_EXT_TCP_IP_WAIT_FOR_SOCKET_CLOSE BIT(16)
//! Enable Embedded/internal MQTT
//! Note: If user wants to use AT command for MQTT, enable this bit in the Opermode Command
#define SL_SI91X_EXT_EMB_MQTT_ENABLE BIT(17)
//! Enables the SSL_HIGH_PERFORMANCE
//! Note: To do firmware upgrade with HTTP this bit should be enabled
#define SL_SI91X_EXT_TCP_IP_FEAT_SSL_HIGH_PERFORMANCE BIT(18)
//! Enabled to update TCP window from host
#define SL_SI91X_EXT_TCP_DYNAMIC_WINDOW_UPDATE_FROM_HOST BIT(19)

//! Enable to update max receive length for TCP
#define SL_SI91X_EXT_TCP_MAX_RECV_LENGTH BIT(20)

//! Note: Bit 21-28 are reserved

//! Enable three SSL/TLS sockets
//! Note: Set tcp_ip_feature_bit_map[31] and ext_tcp_ip_feature_bit_map[29] to open 3 TLS sockets
#define SL_SI91X_EXT_TCP_IP_FEAT_SSL_THREE_SOCKETS BIT(29)

//! To configure additional memory for SSL/TLS connection typically to a cloud server
//! Note: If user connects to a cloud server using two SSL/TLS connections then it is required to set this bit to avoid 0xD2 error
#define SL_SI91X_EXT_TCP_IP_FEAT_SSL_MEMORY_CLOUD BIT(30)

//! config_feature_bit_map validity
#define SL_SI91X_CONFIG_FEAT_EXTENTION_VALID BIT(31)

/*=========================================================================*/
// BT feature bit map parameters description !//
/*=========================================================================*/
//! BT Mode Enable
//! Note:If this Bit sets, then our Controller will sends/receives the packets in 1Mbps mode. Else, Controller will be operatable in both BR/EDR mode
#define SL_SI91X_BT_BDR_MODE_ENABLE BIT(0)

//! BT Mode LP Chain Enable
#define SL_SI91X_BT_BDR_MODE_LP_CHAIN_ENABLE BIT(1)

//! BT Power Control Disable
//! Note: To make Fixed/Adaptive Power
#define SL_SI91X_BT_PWR_CTRL_DISABLE BIT(2)

//! Note Bit 3-7 are reserved

//! BT EDR 3Mbps Feature Disable
//! Note: 3Mbps Disable means using 2Mbps DataRate
#define SL_SI91X_BT_EDR_3MBPS_DISABLEL BIT(8)

//! BT EDR 2Mbps Feature Disable
//! Note:2Mbps Disable means using 3Mbps DataRate
#define SL_SI91X_BT_EDR_2MBPS_DISABLE BIT(9)

//! BT 5 Slot Packet Feature Disable
#define SL_SI91X_BT_5_SLOT_PACKETS_DISABLE BIT(10)

//! BT 3 Slot Packet Feature Disable
#define SL_SI91X_BT_3_SLOT_PACKETS_DISABLE BIT(11)

//! Note: Bits 12-13 bit are reserved

//! TA based encoder
//! Note! To Enable/Disable SBC Encoder in Firmware.This feature is not supported in the current release
#define SL_SI91X_TA_BASED_ENCODER_ENABLE BIT(14)
//! To enable HFP profile
//! Note: This feature is not supported in the current release
#define SL_SI91X_HFP_PROFILE_ENABLE BIT(15)

//! Note: Bits 16 - 22 are reserved

//! To enable A2DP profile
#define SL_SI91X_A2DP_PROFILE_ENABLE BIT(23)
//! To enable A2DP Profile role as source/sink
#define SL_SI91X_A2DP_SOURCE_ROLE_ENABLE BIT(24)
//! To enable A2DP Accelerator mode
//! Note: This feature is not supported in the current release
#define SL_SI91X_A2DP_ACCELERATOR_MODE_ENABLE BIT(25)

//! Note: Bit 26 is reserved

//! To enable Buffer Alignment for Test Mode
//! Note: To get 512 bytes from common pool, need to enable this, else 320 bytes will be fixed
#define SL_SI91X_BT_BLE_CP_BUFF_SIZE BIT(27)

//! Note: Bit 28 is reserved
#define SL_SI91X_BT_ATT_OVER_CLASSIC_ACL BIT(29)

//! To bt rf type
#define SL_SI91X_BT_RF_TYPE BIT(30)
//! To enable ble protocol
#define SL_SI91X_ENABLE_BLE_PROTOCOL BIT(31)

/*=========================================================================*/
// BLE feature bit map
/*=========================================================================*/
//! BLE nbr of attributes,
//! Note: Maximum No of BLE attributes = 80, refer NOTE given below for more info
//! Bits 0 -7 are used to set MAX_NBR_ATT_REC
#define SL_SI91X_BLE_MAX_NBR_ATT_REC(max_num_of_att_rec) (max_num_of_att_rec << 0)

//! BLE number of GATT services
//! Note: Maximum no services - 10, refer NOTE given below for more info
//! Bits 8 - 11 are used to set MAX_NBR_ATT_SERV
#define SL_SI91X_BLE_MAX_NBR_ATT_SERV(max_num_of_att_serv) (max_num_of_att_serv << 8)
//! BLE number of peripherals
//! Note: Maximum No of BLE peripherals = 8, refer NOTE given below for more info
//! Bits 12 - 15 are used to set MAX_NBR_SLAVES
#define SL_SI91X_BLE_MAX_NBR_SLAVES(max_num_of_slaves) (max_num_of_slaves << 12)
//! BLE Tx power index
//! Note: Give 31 as BLE Tx power index (eg: 31<<16) This variable is used to select the BLE tx power index value. The following are the possible values. Default Value for BLE Tx Power Index is 31 Range for the BLE Tx Power Index is 1 to 75 (0, 32 index is invalid) 1 - 31 BLE -0DBM Mode 33 - 63 BLE- 10DBM Mode 64- 75 BLE - HP Mode.
//! Bits 16 - 23 are used to set PWR_INX
#define SL_SI91X_BLE_PWR_INX(power_index) (power_index << 16)

//! BLE powersave options
//! Note: This feature is not supported in the current release
//! Bit 24 for BLE_DUTY_CYCLING
//! Bit 25 for BLR_DUTY_CYCLING
//! Bit 26 for BLE_4X_PWR_SAVE_MODE
//! For BLE_DISABLE_DUTY_CYCLING, BITs 24-26 are set to be zero
#define SL_SI91X_BLE_PWR_SAVE_OPTIONS(duty_cycle) (duty_cycle << 24)

//! Number of Masters
//! Note: Maximum number of BLE Masters = 2. refer to the note below for more info.
//! Bits 27 - 28 are used to set BLE_PWR_INX
#define SL_SI91X_BLE_MAX_NBR_MASTERS(max_num_of_masters) (max_num_of_masters << 27)
//! GATT SYNC BIT
//! Note: Default Disabled Expectation of GATT Async Bit Enable: Response structure will be filled in the Event and Event will come later. Not in sync with response for query command.
#define SL_SI91X_BLE_GATT_ASYNC_ENABLE BIT(29)

//! 0 for 9113 compatible; 1 for enabling 9116 ble compatible features.
#define SL_SI91X_916_BLE_COMPATIBLE_FEAT_ENABLE BIT(30)

//! Extention valid to use Extended custom feature bitmap
#define SL_SI91X_FEAT_BLE_CUSTOM_FEAT_EXTENTION_VALID BIT(31)

/*=========================================================================*/
// Extended BLE custom feature bit map parameters description !//
/*=========================================================================*/
//! BLE number of Connection Events
//! Note: Describes the number of buffers need to be allocated for BLE on the opermode. By default each role (central/peripheral) will be allocated with 1 buffer for the notify/write command We increase the buffer capacity for the notify/write cmds to achive the best throughput. See rsi_ble_set_wo_resp_notify_buf_info() to set more buffers for the notify/write commands
//! Bits 0 - 4 are used to set NUM_CONN_EVENTS
#define SL_SI91X_BLE_NUM_CONN_EVENTS(num_conn_events) (num_conn_events << 0)

//! BLE number of record size in bytes (n)
//! Note: n*16 : (n=60, Default 1024 bytes(1K))
//! Bits 5 - 12 are used to set NUM_REC_BYTES
#define SL_SI91X_BLE_NUM_REC_BYTES(num_rec_bytes) (num_rec_bytes << 5)

//! GATT INIT
//! Note: 0 - GATT Init in Firmware i.e both the GAP service and GATT service will be maintained by Firmware 1 - Gatt Init in Host i.e GAP service and GATT service should be created by the APP/Host/User and the ATT transactions like read, write, notify and indicate shall be handled by the APP/Host/User. Default Gatt Init in Firmware
#define SL_SI91X_BLE_GATT_INIT BIT(13)
//! Indication response from APP
//! Note: As per ATT protocol for every indication received from the server should be acknowledged (indication response) by the Client. If this bit is disabled then firmware will send the acknowledgment(indication response) and if the bit is enabled then APP/Host/User needs to send the acknowledgment(indication response).
#define SL_SI91X_BLE_INDICATE_CONFIRMATION_FROM_HOST BIT(14)

//! MTU Exchange request initiation from APP
//! Note: If this bit is disabled, the firmware will initiate the MTU request to the remote device on the successful connection. And if Peer initiates MTU exchange Request then firmware will send Exchange MTU Response in reply to a received Exchange MTU Request. If this bit enabled then APP/Host/User need to initiate the MTU request by using the rsi_ble_mtu_exchange_event API. And if Peer initiates MTU exchange Request then APP/Host/User shall send Exchange MTU Response in reply to a received Exchange MTU Request using rsi_ble_mtu_exchange_resp API
#define SL_SI91X_BLE_MTU_EXCHANGE_FROM_HOST BIT(15)
//! Set SCAN Resp Data from APP
//! Note: Device will maintain some default scan reponse data and will be used in the scan_response controller frame. By enabling this bit we can make the defalut data as Null(empty).
#define SL_SI91X_BLE_SET_SCAN_RESP_DATA_FROM_HOST BIT(16)
//! Disable Coded PHY from APP
//! Note: Device will support the LE-coded phy feature (i.e LR - 125kbps and 500kbps) by default. If this bit is enabled, the device will not the support of the LE-coded phy rates.
#define SL_SI91X_BLE_DISABLE_CODED_PHY_FROM_HOST BIT(17)

//! Note: Bits 18 -31 are reserved
//!
/*=========================================================================*/
// Config feature bitmap parameters description !//
/*=========================================================================*/
//! To select wakeup indication to host. If it is disabled UULP_GPIO_3 is used as a wakeup indication to host. If it is enabled UULP_GPIO_0 is used as a wakeup indication to host.
#define SL_SI91X_FEAT_SLEEP_GPIO_SEL_BITMAP BIT(0)

//! Note: Bit 1 is reserved

//! DVS Dynamic Voltage Selection
//! Note: These bits are used for dynamic voltage selection
#define SL_SI91X_FEAT_DVS_SEL_CONFIG_1 BIT(2)
#define SL_SI91X_FEAT_DVS_SEL_CONFIG_2 BIT(3)
#define SL_SI91X_FEAT_DVS_SEL_CONFIG_3 BIT(4)
#define SL_SI91X_FEAT_DVS_SEL_CONFIG_4 BIT(5)
//! External PMU Selection
//! Note: These bits are used to select external PMU good time.1 to 15 means 100usec to 1500usec (in 100usec granularity)
#define SL_SI91X_EXTERNAL_PMU_GOOD_TIME_100us  BIT(6)
#define SL_SI91X_EXTERNAL_PMU_GOOD_TIME_200us  BIT(7)
#define SL_SI91X_EXTERNAL_PMU_GOOD_TIME_300us  (BIT(6) | BIT(7))
#define SL_SI91X_EXTERNAL_PMU_GOOD_TIME_400us  BIT(8)
#define SL_SI91X_EXTERNAL_PMU_GOOD_TIME_500us  (BIT(6) | BIT(8))
#define SL_SI91X_EXTERNAL_PMU_GOOD_TIME_600us  (BIT(7) | BIT(8))
#define SL_SI91X_EXTERNAL_PMU_GOOD_TIME_700us  (BIT(6) | BIT(7) | BIT(8))
#define SL_SI91X_EXTERNAL_PMU_GOOD_TIME_800us  BIT(9)
#define SL_SI91X_EXTERNAL_PMU_GOOD_TIME_900us  (BIT(6) | BIT(9))
#define SL_SI91X_EXTERNAL_PMU_GOOD_TIME_1000us (BIT(7) | BIT(9))
#define SL_SI91X_EXTERNAL_PMU_GOOD_TIME_1100us (BIT(6) | BIT(7) | BIT(9))
#define SL_SI91X_EXTERNAL_PMU_GOOD_TIME_1200us (BIT(8) | BIT(9))
#define SL_SI91X_EXTERNAL_PMU_GOOD_TIME_1300us (BIT(6) | BIT(8) | BIT(9))
#define SL_SI91X_EXTERNAL_PMU_GOOD_TIME_1400us (BIT(7) | BIT(8) | BIT(9))
#define SL_SI91X_EXTERNAL_PMU_GOOD_TIME_1500us (BIT(6) | BIT(7) | BIT(8) | BIT(9))
//! Extrenal LDO voltage selection
//! Note: These bits are used for External LDO selection External PMU : 1.In case of External PMU, User has to set EXTERNAL_PMU_GOOD_TIME_CONFIGURATION value to external PMU good time, If this is zero then it indicates using Internal PMU. 2. Incase of External PMU 1.0v or 1.05v, User has to set both the bits config_feature_bit_map[11] & config_feature_bit_map[10].
#define SL_SI91X_FEAT_EXTERNAL_LDO_SEL BIT(10)
//! This field valid only if RSI_FEAT_EXTERNAL_LDO_SEL is enabled i.e BIT(10) is set
//! if this bit set means 1.0V selected else 1.1V selected
#define SL_SI91X_FEAT_EXTERNAL_LDO_VOL BIT(11)

//! Note: Bit 12 -13 are reserved

//! TLS version selection
#define SL_SI91X_FEAT_EAP_TLS_V1P0 BIT(14)
#define SL_SI91X_FEAT_EAP_TLS_V1P2 BIT(15)

//! Note: Bit 16 is reserved

#define SL_SI91X_FEAT_CONC_STA_AP_DYN_SWITCH_SEL BIT(17)
//! Note: If this bit is not set, then by default UART2-TX GPIO_6 will be used.
#define SL_SI91X_ULP_GPIO9_FOR_UART2_TX           BIT(18)
#define SL_SI91X_FEAT_DISABLE_MCS_5_6_7_DATARATES BIT(19)
#define SL_SI91X_FEAT_DISABLE_SHORT_GI            BIT(20)

//! To enable PTA-3WIRE
//! Note: Should be set to enable and use the PTA 3 wire feature followed by available configurations
#define SL_SI91X_PTA_3WIRE_EN BIT(21)

//! To choose PTA-3WIRE
//! Note: Configurability options for config selection among 1,2 & 3
//! Bit 22 - 23 are used to set NUM_CONN_EVENTS
//! 0 kept reserved for future. 3wire used at dut as ULP_GPIO_0(Grant pin driven by dut), ULP_GPIO_1(Request i/p pin for dut) and ULP_GPIO_6(Priority i/p pin for dut)
//! ===============================
//! | Mode(KB) | BIT[23] | BIT[22] |
//! ===============================
//! | Reserved | 0       | 0       |
//! | config1  | 0       | 1       |
//! | config2  | 1       | 0       |
//! | config3  | 1       | 1       |
//! ===============================
#define SL_SI91X_PTA_3WIRE_CONFIG_SEL(config_sel) (config_sel << 22)

//! XTAL goodtime configurations
//! Note: These bits are used to select XTAL good time. These changes are available from Release 2.3.0 onwards. Release prior to 2.3.0 these config_feature_bitmap[31:17] are reserved. Its only applicable for customers using chip not the device. Contact Support for more details Default value is 1000 us.
#define SL_SI91X_XTAL_GOODTIME_1000us 0
#define SL_SI91X_XTAL_GOODTIME_2000us BIT(24)
#define SL_SI91X_XTAL_GOODTIME_3000us BIT(25)
#define SL_SI91X_XTAL_GOODTIME_600us  (BIT(24) | BIT(25))

//! To enable Enhanced Max PSP
#define SL_SI91X_ENABLE_ENHANCED_MAX_PSP    BIT(26)
#define SL_SI91X_ENABLE_DEBUG_BBP_TEST_PINS BIT(27)

/*=========================================================================*/
// HTTP Client feature bitmap parameters description !//
/*=========================================================================*/
//! Bit to enable SSL feature
#define SL_SI91X_ENABLE_SSL BIT(0)

//! Bit to enable NULL delimiter for HTTP buffer instead of comma
#define SL_SI91X_ENABLE_NULL_DELIMETER BIT(1)

//! TLS version feature bitmap
#define SL_SI91X_TLS_V_1_0 BIT(2)
#define SL_SI91X_TLS_V_1_2 BIT(3)
#define SL_SI91X_TLS_V_1_1 BIT(4)

//! HTTP client post big data support feature bitmap
#define SL_SI91X_SUPPORT_HTTP_POST_DATA BIT(5)

//! HTTP version 1.1 support feature bitmap
#define SL_SI91X_HTTP_V_1_1 BIT(6)

//! Bit to enable user given content type in extended header
#define SL_SI91X_HTTP_USER_DEFINED_CONTENT_TYPE BIT(7)

typedef enum {
  SL_SI91X_CLIENT_MODE            = 0,
  SL_SI91X_ENTERPRISE_CLIENT_MODE = 2,
  SL_SI91X_ACCESS_POINT_MODE      = 6,
  SL_SI91X_TRANSMIT_TEST_MODE     = 8,
  SL_SI91X_CONCURRENT_MODE        = 9,
  __FORCE_OPERATION_ENUM_16BIT    = 0xFFFF
} sl_si91x_operation_mode_t;

//STATIC_ASSERT(sizeof(sl_si91x_operation_mode_t) == 2);

typedef enum {
  SL_SI91X_WLAN_ONLY_MODE      = 0,
  SL_SI91X_WLAN_MODE           = 1,
  SL_SI91X_BLUETOOTH_MODE      = 4,
  SL_SI91X_WLAN_BLUETOOTH_MODE = 5,
  SL_SI91X_DUAL_MODE           = 8,
  SL_SI91X_WLAN_DUAL_MODE      = 9,
  SL_SI91X_BLE_MODE            = 12,
  SL_SI91X_WLAN_BLE_MODE       = 13,
  __FORCE_COEX_ENUM_16BIT      = 0xFFFF
} sl_si91x_coex_mode_t;

//STATIC_ASSERT(sizeof(sl_si91x_coex_mode_t) == 2);

// Device configuration for 911x. This should be in the 911x driver folder
typedef struct sl_wifi_device_configuration_s {
  uint8_t boot_option;
  sl_mac_address_t *mac_address;
  sl_si91x_band_mode_t band;
  sl_si91x_region_code_t region_code;
  si91x_boot_configuration_t boot_config;
} sl_wifi_device_configuration_t;

static const sl_wifi_device_configuration_t sl_wifi_default_client_configuration = {
  .boot_option = LOAD_NWP_FW,
  .mac_address = NULL,
  .band        = SL_SI91X_WIFI_BAND_2_4GHZ,
  .region_code = US,
  .boot_config = { .oper_mode = SL_SI91X_CLIENT_MODE,
                   .coex_mode = SL_SI91X_WLAN_ONLY_MODE,
                   .feature_bit_map =
#ifdef RSI_M4_INTERFACE
                     (SL_SI91X_FEAT_SECURITY_OPEN | SL_SI91X_FEAT_WPS_DISABLE),
#else
                     (SL_SI91X_FEAT_SECURITY_OPEN | SL_SI91X_FEAT_AGGREGATION),
#endif
                   .tcp_ip_feature_bit_map =
                     (SL_SI91X_TCP_IP_FEAT_BYPASS | SL_SI91X_TCP_IP_FEAT_DHCPV4_CLIENT | SL_SI91X_TCP_IP_FEAT_DNS_CLIENT | SL_SI91X_TCP_IP_FEAT_SSL
                      | SL_SI91X_TCP_IP_FEAT_ICMP | SL_SI91X_TCP_IP_FEAT_EXTENSION_VALID),
                   .custom_feature_bit_map = SL_SI91X_FEAT_CUSTOM_FEAT_EXTENTION_VALID,
                   .ext_custom_feature_bit_map =
                     (SL_SI91X_EXT_FEAT_XTAL_CLK_ENABLE(2) | SL_SI91X_EXT_FEAT_UART_SEL_FOR_DEBUG_PRINTS |
#ifdef CHIP_9117
#ifndef RSI_M4_INTERFACE
                      SL_SI91X_EXT_FEAT_704K_M4SS_0K
#else
                      SL_SI91X_EXT_FEAT_512K_M4SS_192K
#endif // RSI_M4_INTERFACE
#else
                      SL_SI91X_EXT_FEAT_384K_MODE
#endif // CHIP_9117
                      ),
                   .bt_feature_bit_map = 0,
                   .ext_tcp_ip_feature_bit_map =
                     (SL_SI91X_EXT_TCP_IP_WINDOW_SCALING | SL_SI91X_EXT_TCP_IP_TOTAL_SELECTS(10)
                      | SL_SI91X_EXT_TCP_IP_FEAT_SSL_THREE_SOCKETS | SL_SI91X_EXT_TCP_IP_FEAT_SSL_MEMORY_CLOUD),
                   .ble_feature_bit_map     = 0,
                   .ble_ext_feature_bit_map = 0,
                   .config_feature_bit_map  = 0 }
};

static const sl_wifi_device_configuration_t sl_wifi_default_enterprise_client_configuration = {
  .boot_option = LOAD_NWP_FW,
  .mac_address = NULL,
  .band        = SL_SI91X_WIFI_BAND_2_4GHZ,
  .region_code = US,
  .boot_config = { .oper_mode                  = SL_SI91X_ENTERPRISE_CLIENT_MODE,
                   .coex_mode                  = SL_SI91X_WLAN_ONLY_MODE,
                   .feature_bit_map            = (SL_SI91X_FEAT_SECURITY_OPEN | SL_SI91X_FEAT_AGGREGATION),
                   .tcp_ip_feature_bit_map     = (SL_SI91X_TCP_IP_FEAT_DHCPV4_CLIENT | SL_SI91X_TCP_IP_FEAT_ICMP
                                              | SL_SI91X_TCP_IP_FEAT_EXTENSION_VALID),
                   .custom_feature_bit_map     = SL_SI91X_FEAT_CUSTOM_FEAT_EXTENTION_VALID,
                   .ext_custom_feature_bit_map = (
#ifdef CHIP_9117
#ifndef RSI_M4_INTERFACE
                     SL_SI91X_EXT_FEAT_704K_M4SS_0K
#else
                     SL_SI91X_EXT_FEAT_512K_M4SS_192K
#endif
                     | SL_SI91X_EXT_FEAT_XTAL_CLK_ENABLE(2)
#else
                     SL_SI91X_EXT_FEAT_320K_MODE | SL_SI91X_EXT_FEAT_384K_MODE | SL_SI91X_EXT_FEAT_XTAL_CLK_ENABLE(2)
#endif
                     | SL_SI91X_EXT_FEAT_UART_SEL_FOR_DEBUG_PRINTS),
                   .bt_feature_bit_map = 0,
                   .ext_tcp_ip_feature_bit_map =
                     (SL_SI91X_EXT_TCP_IP_WINDOW_SCALING | SL_SI91X_EXT_TCP_IP_TOTAL_SELECTS(10)),
                   .ble_feature_bit_map     = 0,
                   .ble_ext_feature_bit_map = 0,
                   .config_feature_bit_map  = 0 }
};

static const sl_wifi_device_configuration_t sl_wifi_default_ap_configuration = {
  .boot_option = LOAD_NWP_FW,
  .mac_address = NULL,
  .band        = SL_SI91X_WIFI_BAND_2_4GHZ,
  .region_code = US,
  .boot_config = { .oper_mode                  = SL_SI91X_ACCESS_POINT_MODE,
                   .coex_mode                  = SL_SI91X_WLAN_ONLY_MODE,
                   .feature_bit_map            = SL_SI91X_FEAT_SECURITY_OPEN,
                   .tcp_ip_feature_bit_map     = SL_SI91X_TCP_IP_FEAT_DHCPV4_SERVER,
                   .custom_feature_bit_map     = 0,
                   .ext_custom_feature_bit_map = 0,
                   .bt_feature_bit_map         = 0,
                   .ext_tcp_ip_feature_bit_map = 0,
                   .ble_feature_bit_map        = 0,
                   .ble_ext_feature_bit_map    = 0,
                   .config_feature_bit_map     = 0 }
};

static const sl_wifi_device_configuration_t sl_wifi_default_concurrent_configuration = {
  .boot_option = LOAD_NWP_FW,
  .mac_address = NULL,
  .band        = SL_SI91X_WIFI_BAND_2_4GHZ,
  .region_code = US,
  .boot_config = { .oper_mode              = SL_SI91X_CONCURRENT_MODE,
                   .coex_mode              = SL_SI91X_WLAN_ONLY_MODE,
                   .feature_bit_map        = SL_SI91X_FEAT_AGGREGATION,
                   .tcp_ip_feature_bit_map = (SL_SI91X_TCP_IP_FEAT_BYPASS | SL_SI91X_TCP_IP_FEAT_DHCPV4_CLIENT | SL_SI91X_TCP_IP_FEAT_DHCPV4_SERVER
                                              | SL_SI91X_TCP_IP_FEAT_ICMP | SL_SI91X_TCP_IP_FEAT_EXTENSION_VALID),
                   .custom_feature_bit_map = SL_SI91X_FEAT_CUSTOM_FEAT_EXTENTION_VALID,
                   .ext_custom_feature_bit_map = (
#ifdef CHIP_9117
#ifndef RSI_M4_INTERFACE
                     SL_SI91X_EXT_FEAT_704K_M4SS_0K
#else
                     SL_SI91X_EXT_FEAT_512K_M4SS_192K
#endif
                     | SL_SI91X_EXT_FEAT_XTAL_CLK_ENABLE(2)),
#else
                     SL_SI91X_EXT_FEAT_384K_MODE | SL_SI91X_EXT_FEAT_XTAL_CLK_ENABLE(2)),
#endif
                   .bt_feature_bit_map         = 0,
                   .ext_tcp_ip_feature_bit_map = SL_SI91X_CONFIG_FEAT_EXTENTION_VALID,
                   .ble_feature_bit_map        = 0,
                   .ble_ext_feature_bit_map    = 0,
                   .config_feature_bit_map     = SL_SI91X_FEAT_SLEEP_GPIO_SEL_BITMAP }
};

#define BURN_INTO_EFUSE      0
#define BURN_INTO_FLASH      1
#define BURN_GAIN_OFFSET     BIT(0)
#define BURN_FREQ_OFFSET     BIT(1)
#define SW_XO_CTUNE_VALID    BIT(2)
#define BURN_XO_FAST_DISABLE BIT(3)
/**
 * burn_target: BURN_INTO_EFUSE=Burn into efuse
 *              BURN_INTO_FLASH=Burn into flash
 * flags: BURN_GAIN_OFFSET
 *        BURN_FREQ_OFFSET
 *        SW_XO_CTUNE_VALID
 *        BURN_XO_FAST_DISABLE
 */
typedef struct sl_si91x_calibration_data_s {
  uint8_t burn_target;
  uint8_t reserved0[3];
  uint32_t flags;
  int8_t gain_offset;
  int8_t xo_ctune;
  uint8_t reserved1[2];
} si91x_calibration_data_t;

typedef struct {
  uint32_t stuff;
} sl_wifi_context_t;

extern void si91x_default_event_handler(uint32_t event, void *data, void *arg);

/// Performance profile
typedef enum {
  HIGH_PERFORMANCE,                  // Power save is disabled and throughput is maximum
  ASSOCIATED_POWER_SAVE,             // Power save mode when module is associated with either AP or station
  ASSOCIATED_POWER_SAVE_LOW_LATENCY, // Power save mode when module is associated with either AP or station, with higher throughput than ASSOCIATED_POWER_SAVE. This is not supported for BT/BLE
  STANDBY_POWER_SAVE, // Power save mode when module is not associated with either AP or station, ram is not retained in this mode
  STANDBY_POWER_SAVE_WITH_RAM_RETENTION // Power save mode when module is not associated with either AP or station, ram is retained in this mode
} sl_performance_profile_t;

///BT performance profile
typedef struct {
  sl_performance_profile_t profile;
} sl_bt_performance_profile_t;

/// Wi-Fi performance profile
typedef struct {
  sl_performance_profile_t profile;
  // set DTIM aligment required
  // 0 - module wakes up at beacon which is just before or equal to listen_interval
  // 1 - module wakes up at DTIM beacon which is just before or equal to listen_interval
  // default value to be used is 0
  uint8_t dtim_aligned_type;
  // default value to be used is 0
  uint8_t num_of_dtim_skip;
  uint16_t listen_interval;
  sl_wifi_twt_request_t twt_request;
} sl_wifi_performance_profile_t;

/** @} */
