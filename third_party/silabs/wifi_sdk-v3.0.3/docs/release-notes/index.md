# WiSeConnect3_SDK_3.0.3 Release Notes
This SDK package supports SoC Mode

  - In SoC Mode: Customer Applications can run on Cortex M4.  Wireless stacks and Networking stacks run on the Wireless subsystem.  
  - Silicon Labs' SiWx917 includes an ultra-low power Wi-Fi 6 plus Bluetooth Low Energy (BLE) 5.1 wireless subsystem and an integrated micro-controller application subsystem.
  
   > Note: Release supports only Simplicity Studio 5.5 & above versions 
   
## SoC Highlights
 
  - **Operating Modes**                : Wi-Fi STA, Wi-Fi AP, Concurrent(AP + STA), Wi-Fi STA+BLE(CoEx), 
  - **Wireless Protocols**             : IEEE 802.11b, 802.11g, 802.11n , 802.11ax, BLE 5.1
  - **MCU Peripherals**                : ADC Static Mode, Analog Comparator, Blinky, Bod, CTS, DAC FIFO, DAC Static, OPAMP, GPIO, I2C, I2S, PWM, RTC, QEI, SPI, UART, USART, UDMA, WDT, Combined Peripheral App 
									   : ULP (Ultra Low Power) Peripherals - ulp_gpio, ulp_i2c, ulp_i2s, ulp_ir_decoder, ulp_spi, ulp_timer, ulp_uart, ulp_udma 
									   : UART and I2C Deepsleep Wakeup Without RAM Retention
  - **Multiple Ultra Low Power Modes** : Wi-Fi 6 TWT (Target Wake Time) - for improved network efficiency and device battery life
  - **Wi-Fi**                          : 2.4GHz Wi-Fi Support - WPA2/WPA3 Personal, Wi-Fi 6 OFDMA/MU-MIMO Support that enhances network capacity and latency
  - **Cloud**                          : Amazon Cloud Connectivity Support, Amazon FreeRTOS
  - **BLE**                            : BLE 5.1, BLE dual role (Central and Peripheral Support)
  - **Integrated Stacks and Profiles** : Wi-Fi Stack, BLE Stack and Profiles, TCP/IP stack with TLS 1.3, HTTP/HTTPS, DHCP, MQTT Client 
  - **IDE**                            : Simplicity Studio with Windows 10(64-bit)  
  - **TA/M4 Firmware Update Tool**     : Simplicity Commander
  - **Hardware kits to evaluate SoC Mode**   : Si917-BRD4325B (Dual flash Radio board)

## Release Details
| Item                                                | Details                                |
|-----------------------------------------------------|----------------------------------------|
| Release date                                        | MAY 22nd, 2023                          |
| API Version                                         | 3.0.3(Build 1)                         |
| Firmware Version                                    | 1711.1.0.2.0.2                        |
| Package Name                                        | WiSeConnect3_SDK_3.0.3                 |
| Supported RTOS                                      | FreeRTOS                               | 
| Operating Modes Supported                           | Wi-Fi STA, Wi-Fi AP, Concurrent(AP + STA), Wi-Fi STA+BLE(CoEx)|



## Updating to this Release
This release consists of Wireless Library - The Library runs on internal Cortex M4 in SoC Mode and supports 33 sample examples.

## Features and Bug Fixes
- Added SNTP client Application
- Handling DHCP of AP
- Update BLE and WLAN examples to Latest Firware version 1.0.2.0.2
- Update TWT Config Error handling
- Update http_client example to support Certificate Index
- Update RSI_WLAN_REQ_11AX_PARAMS to be updated with configurable guard interval
- Update RSI_WLAN_RSP_JOIN handling to support concurrent mode usecase
- API Documentation enhancement
- Implemented SNTP Client APIs
- Added support for printing floating point in Simplicity Studio IDE.
- APPS Team Feedback (Difficulty in making initial configurations using the new Wi-Fi SDK)
- Update Linker file for Common flash for TLS Issue
- Addressed SQA Blockers - SI91X-13105, SI91X-13111


## Known Issues
  
### SoC
  - The "Browser File Viewer" plugin has issues. It can't render the last image included in the markdown files and observed issues accessing some hyperlinks
  - In SoC mode, M4 power save will work with 384k TA and 320 M4 memory configuration only. It is not supported for "448K TA and 256k M4" and "512k TA and 192k M4" memory configurations
### Network Stack
  
### Wi-Fi
  - WPA3 connection takes 2.5 sec more than WPA2 connection
  - Issue observed with WPA2 Enterprise Connectivity using Microsoft RADIUS Server.
  - power_save_deep_sleep example is not working

### BLE
  - For BLE, dual role scenario issues might occur when connecting as central if the advertising is also going out for the peripheral role. Central connections can be made after the peripheral connections are established and advertising is stopped.
  - LMP timeout BLE disconnection observed with the latest Realme mobile handsets.
  - BLE Powersave + RTOS is not supported in SoC Mode.

## Limitations and Unsupported Features

### Wi-Fi/Network Stack
  - Maximum of 3 SSL connections are supported in WiFi alone mode.
  - TWT is not verified in Coex mode.
  - BGscan is not verified with TWT Feature.
  - No support for DTIM skip feature in power save.

### BLE
  - For BLE, if the connection is established with a small connection interval (less than 15 ms), simultaneous roles (i.e., Central + Scanning and Peripheral + Advertising) are not supported.
  - BLE maximum two concurrent connections are supported, which can be either a connection to two peripheral devices, to one central and one peripheral device or two central devices.
  - BLE Slave latency value is valid up to 32 only.
  
### SoC
  - RTOS support is not available for MCU peripheral drivers. Application-level RTOS support exists.

## Terminology
| Term             | Description |
| -----------------|---------------------------------------- |
| **New Features** | These items are new to this release |
| **Changes/Issues Fixed** | Changes made to existing features found in previous software releases. |
|| Enhancements to existing product flow and feature set. |
||Bug fixes done in the Release |
| **Deprecated Items** | Features or functions or APIs that are removed from the distributed release to align with the software roadmap |
| **Known Issues** | Features or functions that do not work as planned at the time of release. Workarounds may be offered to meet short-term development goals, but longer-term solutions will be added in future software releases |
| **Limitations/Recommendations** | Describes the limitations on product usage and recommendations for optimal use cases |

<br />

## Notes
This release supports 917 A0(Common flash & Dual flash).

# WiSeConnect3_SDK_3.0.2 Release Notes
This SDK package supports SoC Mode

  - In SoC Mode: Customer Applications can run on Cortex M4.  Wireless stacks and Networking stacks run on the Wireless subsystem.  
  - Silicon Labs' SiWx917 includes an ultra-low power Wi-Fi 6 plus Bluetooth Low Energy (BLE) 5.1 wireless subsystem and an integrated micro-controller application subsystem.
  
   > Note: Release supports only Simplicity Studio 5.5 & above versions 
   
## SoC Highlights
 
  - **Operating Modes**                : Wi-Fi STA, Wi-Fi AP, Concurrent(AP + STA), Wi-Fi STA+BLE(CoEx), 
  - **Wireless Protocols**             : IEEE 802.11b, 802.11g, 802.11n , 802.11ax, BLE 5.1
  - **MCU Peripherals**                : ADC Static Mode, Analog Comparator, Blinky, Bod, CTS, DAC FIFO, DAC Static, OPAMP, GPIO, I2C, I2S, PWM, RTC, QEI, SPI, UART, USART, UDMA, WDT, Combined Peripheral App 
									   : ULP (Ultra Low Power) Peripherals - ulp_gpio, ulp_i2c, ulp_i2s, ulp_ir_decoder, ulp_spi, ulp_timer, ulp_uart, ulp_udma 
									   : UART and I2C Deepsleep Wakeup Without RAM Retention
  - **Multiple Ultra Low Power Modes** : Wi-Fi 6 TWT (Target Wake Time) - for improved network efficiency and device battery life
  - **Wi-Fi**                          : 2.4GHz Wi-Fi Support - WPA2/WPA3 Personal, Wi-Fi 6 OFDMA/MU-MIMO Support that enhances network capacity and latency
  - **Cloud**                          : Amazon Cloud Connectivity Support, Amazon FreeRTOS
  - **BLE**                            : BLE 5.1, BLE dual role (Central and Peripheral Support)
  - **Integrated Stacks and Profiles** : Wi-Fi Stack, BLE Stack and Profiles, TCP/IP stack with TLS 1.3, HTTP/HTTPS, DHCP, MQTT Client 
  - **IDE**                            : Simplicity Studio with Windows 10(64-bit)  
  - **TA/M4 Firmware Update Tool**     : Simplicity Commander
  - **Hardware kits to evaluate SoC Mode**   : Si917-BRD4325B (Dual flash Radio board)

## Release Details
| Item                                                | Details                                |
|-----------------------------------------------------|----------------------------------------|
| Release date                                        | MAY 2nd, 2023                          |
| API Version                                         | 3.0.2(Build 1)                         |
| Firmware Version                                    | 1711.1.0.0.0.29                        |
| Package Name                                        | WiSeConnect3_SDK_3.0.2                 |
| Supported RTOS                                      | FreeRTOS                               | 
| Operating Modes Supported                           | Wi-Fi STA, Wi-Fi AP, Concurrent(AP + STA), Wi-Fi STA+BLE(CoEx)|



## Updating to this Release
This release consists of Wireless Library - The Library runs on internal Cortex M4 in SoC Mode and supports 27 sample examples.

## Features and Bug Fixes
- Added Two Applications
    TLS client
    BLE throughput
- Enhancement of example readme files
- Added migration guide, Getting Started Guide​
- Updated API documentation
- Added UTF support for SL_WIFI APIs, SL_NET APIs, etc ..
- Added M4 Powersave APIs and updated M4 Powersave changes in powersave_standby_associated application.
- Added Support for following APIs/Frame 
    RSI_WLAN_REQ_GAIN_TABLE, 
    RSI_WLAN_REQ_IPCONFV6,
    RSI_COMMON_REQ_SOFT_RESET
    RSI_COMMON_REQ_ASSERT
- Added .bat utility for common flash 

## Known Issues
  
### SoC
  - No support for floating point print in Simplicity Studio IDE.
  - The "Browser File Viewer" plugin has issues. It can't render the last image included in the markdown files and observed issues accessing some hyperlinks
  - In SoC mode, M4 power save will work with 384k TA and 320 M4 memory configuration only. It is not supported for "448K TA and 256k M4" and "512k TA and 192k M4" memory configurations
### Network Stack
  
### Wi-Fi
  - WPA3 connection takes 2.5 sec more than WPA2 connection
  - Issue observed with WPA2 Enterprise Connectivity using Microsoft RADIUS Server.

### BLE
  - For BLE, dual role scenario issues might occur when connecting as central if the advertising is also going out for the peripheral role. Central connections can be made after the peripheral connections are established and advertising is stopped.
  - LMP timeout BLE disconnection observed with the latest Realme mobile handsets.
  - BLE Powersave + RTOS is not supported in SoC Mode.

## Limitations and Unsupported Features

### Wi-Fi/Network Stack
  - Maximum of 3 SSL connections are supported in WiFi alone mode.
  - TWT is not verified in Coex mode.
  - BGscan is not verified with TWT Feature.
  - No support for DTIM skip feature in power save.

### BLE
  - For BLE, if the connection is established with a small connection interval (less than 15 ms), simultaneous roles (i.e., Central + Scanning and Peripheral + Advertising) are not supported.
  - BLE maximum two concurrent connections are supported, which can be either a connection to two peripheral devices, to one central and one peripheral device or two central devices.
  - BLE Slave latency value is valid up to 32 only.
  
### SoC
  - RTOS support is not available for MCU peripheral drivers. Application-level RTOS support exists.

## Terminology
| Term             | Description |
| -----------------|---------------------------------------- |
| **New Features** | These items are new to this release |
| **Changes/Issues Fixed** | Changes made to existing features found in previous software releases. |
|| Enhancements to existing product flow and feature set. |
||Bug fixes done in the Release |
| **Deprecated Items** | Features or functions or APIs that are removed from the distributed release to align with the software roadmap |
| **Known Issues** | Features or functions that do not work as planned at the time of release. Workarounds may be offered to meet short-term development goals, but longer-term solutions will be added in future software releases |
| **Limitations/Recommendations** | Describes the limitations on product usage and recommendations for optimal use cases |

<br />

## Notes


# WiSeConnect3_SDK_IR_1.0.1 Release Notes
This SDK package supports SoC Mode

  - In SoC Mode: Customer Applications can run on Cortex M4.  Wireless stacks and Networking stacks run on the Wireless subsystem.  
  - Silicon Labs' SiWx917 includes an ultra-low power Wi-Fi 6 plus Bluetooth Low Energy (BLE) 5.1 wireless subsystem and an integrated micro-controller application subsystem.
  
   > Note: Release supports only Simplicity Studio 5.5 & above versions 
   
## SoC Highlights
 
  - **Operating Modes**                : Wi-Fi STA, Wi-Fi AP, Concurrent(AP + STA), Wi-Fi STA+BLE(CoEx), 
  - **Wireless Protocols**             : IEEE 802.11b, 802.11g, 802.11n , 802.11ax, BLE 5.1
  - **MCU Peripherals**                : ADC Static Mode, Analog Comparator, Blinky, Bod, CTS, DAC FIFO, DAC Static, OPAMP, GPIO, I2C, I2S, PWM, RTC, QEI, SPI, UART, USART, UDMA, WDT, Combined Peripheral App 
									   : ULP (Ultra Low Power) Peripherals - ulp_gpio, ulp_i2c, ulp_i2s, ulp_ir_decoder, ulp_spi, ulp_timer, ulp_uart, ulp_udma 
									   : UART and I2C Deepsleep Wakeup Without RAM Retention
  - **Multiple Ultra Low Power Modes** : Wi-Fi 6 TWT (Target Wake Time) - for improved network efficiency and device battery life
  - **Wi-Fi**                          : 2.4GHz Wi-Fi Support - WPA2/WPA3 Personal, Wi-Fi 6 OFDMA/MU-MIMO Support that enhances network capacity and latency
  - **Cloud**                          : Amazon Cloud Connectivity Support, Amazon FreeRTOS
  - **BLE**                            : BLE 5.1, BLE dual role (Central and Peripheral Support)
  - **Integrated Stacks and Profiles** : Wi-Fi Stack, BLE Stack and Profiles, TCP/IP stack with TLS 1.3, HTTP/HTTPS, DHCP, MQTT Client 
  - **IDE**                            : Simplicity Studio with Windows 10(64-bit)  
  - **TA/M4 Firmware Update Tool**     : Simplicity Commander
  - **Hardware kits to evaluate SoC Mode**   : Si917-BRD4325B (Dual flash Radio board)

## Release Details
| Item                                                | Details                                |
|-----------------------------------------------------|----------------------------------------|
| Release date                                        | April 17th, 2023                       |
| API Version                                         | 1.0.1(Build 1)                         |
| Firmware Version                                    | 1711.1.0.0.0.29                        |
| Package Name                                        | WiSeConnect3_SDK_IR_1.0.1              |
| Supported RTOS                                      | FreeRTOS                               | 
| Operating Modes Supported                           | Wi-Fi STA, Wi-Fi AP, Concurrent(AP + STA), Wi-Fi STA+BLE(CoEx)|



## Updating to this Release
This release consists of Wireless Library - The Library runs on internal Cortex M4 in SoC Mode and supports 27 sample examples.

## Features and Bug Fixes
- Implemented TLS APIs.
- Added Four Applications(wlan_station_ble_provisioning_aws, wlan_station_ble_provisioning, aws_device_shadow, wifi6_mqtt_twt)
- Enhancement of example readme files.
- Added additional host error codes.
- Zero build Warnings.
- Documentation support for sl_wifi_device.h.​ 
- MQTT Bug Fix (MQTT event handler isn't receiving failure event).
- Added support to switch M4 frequency for throughput and wifi6-mimo examples.
- Prepared repo for SDK.

## Known Issues
  
### SoC
  - No support for floating point print in Simplicity Studio IDE.
  - The "Browser File Viewer" plugin has issues. It can't render the last image included in the markdown files and observed issues accessing some hyperlinks
  - In SoC mode, M4 power save will work with 384k TA and 320 M4 memory configuration only. It is not supported for "448K TA and 256k M4" and "512k TA and 192k M4" memory configurations
### Network Stack
  - IPv6 support is not available in this release.
  
### Wi-Fi
  - WPA3 connection takes 2.5 sec more than WPA2 connection
  - Issue observed with WPA2 Enterprise Connectivity using Microsoft RADIUS Server.

### BLE
  - For BLE, dual role scenario issues might occur when connecting as central if the advertising is also going out for the peripheral role. Central connections can be made after the peripheral connections are established and advertising is stopped.
  - LMP timeout BLE disconnection observed with the latest Realme mobile handsets.
  - BLE Powersave + RTOS is not supported in SoC Mode.

## Limitations and Unsupported Features

### Wi-Fi/Network Stack
  - Maximum of 3 SSL connections are supported in WiFi alone mode.
  - TWT is not verified in Coex mode.
  - BGscan is not verified with TWT Feature.
  - No support for DTIM skip feature in power save.

### BLE
  - For BLE, if the connection is established with a small connection interval (less than 15 ms), simultaneous roles (i.e., Central + Scanning and Peripheral + Advertising) are not supported.
  - BLE maximum two concurrent connections are supported, which can be either a connection to two peripheral devices, to one central and one peripheral device or two central devices.
  - BLE Slave latency value is valid up to 32 only.
  
### SoC
  - RTOS support is not available for MCU peripheral drivers. Application-level RTOS support exists.

## Terminology
| Term             | Description |
| -----------------|---------------------------------------- |
| **New Features** | These items are new to this release |
| **Changes/Issues Fixed** | Changes made to existing features found in previous software releases. |
|| Enhancements to existing product flow and feature set. |
||Bug fixes done in the Release |
| **Deprecated Items** | Features or functions or APIs that are removed from the distributed release to align with the software roadmap |
| **Known Issues** | Features or functions that do not work as planned at the time of release. Workarounds may be offered to meet short-term development goals, but longer-term solutions will be added in future software releases |
| **Limitations/Recommendations** | Describes the limitations on product usage and recommendations for optimal use cases |

<br />

## Notes






# WiSeConnect3_SDK_IR_1.0.0 Release Notes

This SDK package supports SoC Mode

  - In SoC Mode: Customer Applications can run on Cortex M4.  Wireless stacks and Networking stacks run on the Wireless subsystem.  
  - Silicon Labs' SiWx917 includes an ultra-low power Wi-Fi 6 plus Bluetooth Low Energy (BLE) 5.1 wireless subsystem and an integrated micro-controller application subsystem.
  
   > Note: Release supports only Simplicity Studio 5.5 & above versions 
   
## SoC Highlights
 
  - **Operating Modes**                : Wi-Fi STA, Wi-Fi AP, Concurrent(AP + STA), Wi-Fi STA+BLE(CoEx), 
  - **Wireless Protocols**             : IEEE 802.11b, 802.11g, 802.11n , 802.11ax, BLE 5.1
  - **MCU Peripherals**                : ADC Static Mode, Analog Comparator, Blinky, Bod, CTS, DAC FIFO, DAC Static, OPAMP, GPIO, I2C, I2S, PWM, RTC, QEI, SPI, UART, USART, UDMA, WDT, Combined Peripheral App 
									   : ULP (Ultra Low Power) Peripherals - ulp_gpio, ulp_i2c, ulp_i2s, ulp_ir_decoder, ulp_spi, ulp_timer, ulp_uart, ulp_udma 
									   : UART and I2C Deepsleep Wakeup Without RAM Retention
  - **Multiple Ultra Low Power Modes** : Wi-Fi 6 TWT (Target Wake Time) - for improved network efficiency and device battery life
  - **Wi-Fi**                          : 2.4GHz Wi-Fi Support - WPA2/WPA3 Personal, Wi-Fi 6 OFDMA/MU-MIMO Support that enhances network capacity and latency
  - **Cloud**                          : Amazon Cloud Connectivity Support, Amazon FreeRTOS
  - **BLE**                            : BLE 5.1, BLE dual role (Central and Peripheral Support)
  - **Integrated Stacks and Profiles** : Wi-Fi Stack, BLE Stack and Profiles, TCP/IP stack with TLS 1.3, HTTP/HTTPS, DHCP, MQTT Client 
  - **IDE**                            : Simplicity Studio with Windows 10(64-bit)  
  - **TA/M4 Firmware Update Tool**     : Simplicity Commander
  - **Hardware kits to evaluate SoC Mode**   : Si917-BRD4325B (Dual flash Radio board)

## Release Details
| Item                                                | Details                                |
|-----------------------------------------------------|----------------------------------------|
| Release date                                        | April 3rd, 2023                        |
| API Version                                         | 1.0.0(Build 1)                         |
| Firmware Version                                    | 1711.1.0.0.0.26                        |
| Package Name                                        | WiSeConnect3_SDK_IR_1.0.0              |
| Supported RTOS                                      | FreeRTOS                               | 
| Operating Modes Supported                           | Wi-Fi STA, Wi-Fi AP, Concurrent(AP + STA), Wi-Fi STA+BLE                   |



## Updating to this Release
This release consists of Wireless Library - The Library runs on internal Cortex M4 in SoC Mode and supports 23 sample examples.

## Features
- Platform agnostic Wifi, Network APIs.
- FreeRTOS support
- Multi-threaded application development support
- AWS MQTT library support
- BSD and IOT socket application programming interface (API)
- WPA2/WPA3 Personal Support for Station Mode and Enterprise security Support for station Mode.
- Network Application protocol support for 
   Offloaded mode(TCP/IP internal stack) with HTTP/HTTPS, DHCP, MQTT Client, DNS Client.
- TA Power save support and Max PSP, Fast PSP, 11ax TWT profiles are supported
- 11ax and TWT support
- Bluetooth Low Energy(BLE) central & peripheral connections are supported
- Wireless firmware upgradation.

## Known Issues
  
### SoC
  - No support for floating point print in Simplicity Studio IDE.
  - The "Browser File Viewer" plugin has issues. It can't render the last image included in the markdown files and observed issues accessing some hyperlinks
  - In SoC mode, M4 power save will work with 384k TA and 320 M4 memory configuration only. It is not supported for "448K TA and 256k M4" and "512k TA and 192k M4" memory configurations
### Network Stack
  - IPv6 support is not available in this release.
  
### Wi-Fi
  - WPA3 connection takes 2.5 sec more than WPA2 connection
  - Issue observed with WPA2 Enterprise Connectivity using Microsoft RADIUS Server.

### BLE
  - For BLE, dual role scenario issues might occur when connecting as central if the advertising is also going out for the peripheral role. Central connections can be made after the peripheral connections are established and advertising is stopped.
  - LMP timeout BLE disconnection observed with the latest Realme mobile handsets.
  - BLE Powersave + RTOS is not supported in SoC Mode.

## Limitations and Unsupported Features

### Wi-Fi/Network Stack
  - Maximum of 3 SSL connections are supported in WiFi alone mode.
  - TWT is not verified in Coex mode.
  - BGscan is not verified with TWT Feature.
  - No support for DTIM skip feature in power save.

### BLE
  - For BLE, if the connection is established with a small connection interval (less than 15 ms), simultaneous roles (i.e., Central + Scanning and Peripheral + Advertising) are not supported.
  - BLE maximum two concurrent connections are supported, which can be either a connection to two peripheral devices, to one central and one peripheral device or two central devices.
  - BLE Slave latency value is valid up to 32 only.
  
### SoC
  - RTOS support is not available for MCU peripheral drivers. Application-level RTOS support exists.

## Terminology
| Term             | Description |
| -----------------|---------------------------------------- |
| **New Features** | These items are new to this release |
| **Changes/Issues Fixed** | Changes made to existing features found in previous software releases. |
|| Enhancements to existing product flow and feature set. |
||Bug fixes done in the Release |
| **Deprecated Items** | Features or functions or APIs that are removed from the distributed release to align with the software roadmap |
| **Known Issues** | Features or functions that do not work as planned at the time of release. Workarounds may be offered to meet short-term development goals, but longer-term solutions will be added in future software releases |
| **Limitations/Recommendations** | Describes the limitations on product usage and recommendations for optimal use cases |

<br />

## Notes
