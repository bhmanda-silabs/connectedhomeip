# Access Point

## 1 Purpose/Scope

This application demonstrates how to configure the SiWx91x as a soft Access point and allows stations to connect to it. 

## 2 Prerequisites/Setup Requirements

### 2.1 Hardware Requirements

- A Windows PC.
- **SoC Mode**: 
  - Silicon Labs [BRD4325A,BRD4325B](https://www.silabs.com/)
- **NCP Mode**:
  - Silicon Labs [(BRD4180A, BRD4280B)](https://www.silabs.com/) **AND**
  - Host MCU Eval Kit. This example has been tested with:
    - Silicon Labs [WSTK + EFR32MG21](https://www.silabs.com/development-tools/wireless/efr32xg21-bluetooth-starter-kit)

### 2.2 Software Requirements

- Simplicity Studio IDE 

	- Download the latest [Simplicity Studio IDE](https://www.silabs.com/developers/simplicity-studio)
	- Follow the [Simplicity Studio user guide](https://docs.silabs.com/simplicity-studio-5-users-guide/1.1.0/ss-5-users-guide-getting-started/install-ss-5-and-software#install-ssv5) to install Simplicity Studio IDE


### 2.3 Setup Diagram

#### SoC Mode :  

![Figure: Setup Diagram SoC Mode for Access point Example](resources/readme/ap_soc.png)

Follow the [Getting Started with SiWx91x SoC](https://docs.silabs.com/) guide to set up the hardware connections and Simplicity Studio IDE.

#### NCP Mode :  

![Figure: Setup Diagram NCP Mode for Access point Example](resources/readme/ap_ncp.png)

Follow the [Getting Started with EFx32](https://docs.silabs.com/rs9116-wiseconnect/latest/wifibt-wc-getting-started-with-efx32/) guide to setup the hardware connections and Simplicity Studio IDE.

**NOTE**: 

- The Host MCU platform (EFR32MG21) and the SiWx91x interact with each other through the SPI interface. 

## 3 Project Environment

- Ensure the SiWx91x loaded with the latest firmware following the [Getting started with a PC](https://docs.silabs.com/rs9116/latest/wiseconnect-getting-started)


### 3.1 Creating the project

#### 3.1.1 SoC mode

- In the Simplicity Studio IDE, the SiWx91x SoC board will be detected under **Debug Adapters** pane as shown below.

    **![Soc Board detection](resources/readme/soc_board_detection.png)**

- Ensure the latest Gecko SDK along with the  WiSeConnect3 extension is added to Simplicity Studio.

- Studio should detect your board. Your board will be shown here. Click on the board detected and go to **EXAMPLE PROJECTS & DEMOS** section.

- Filter for Wi-Fi examples from the Gecko SDK added. For this, check the *Wi-Fi* checkbox under **Wireless Technology** 

    **![Access Point project](resources/readme/ap_example_soc.png)**

- Click 'Create'. The "New Project Wizard" window appears. Click 'Finish'

    **![Create Access Point project](resources/readme/create_project.png)**

#### 3.1.2 NCP mode

- In the Simplicity Studio IDE, the EFR32 board will be detected under **Debug Adapters** pane as shown below.

  **![EFR32 Board detection](resources/readme/efr32.png)**

- Ensure the latest Gecko SDK along with the  WiSeConnect3 extension is added to  Simplicity Studio.

- Go to the 'EXAMPLE PROJECT & DEMOS' tab and select Wi-Fi - NCP Access Point example.

  **![Access Point project](resources/readme/ap_example.png)**

- Click 'Create'. The "New Project Wizard" window appears. Click 'Finish'

   **![Create Access Point project](resources/readme/create_project.png)**


### 3.2 Set up for application prints

#### 3.2.1 SoC mode

  You can use either of the below USB to UART converters for application prints.

1. Set up using USB to UART converter board.

  	- Connect Tx (Pin-6) to P27 on WSTK
  	- Connect GND (Pin 8 or 10) to GND on WSTK

    **![FTDI_prints](resources/readme/usb_to_uart_1.png)**

2. Set up using USB to UART converter cable.

  	- Connect RX (Pin 5) of TTL convertor to P27 on WSTK
  	- Connect GND (Pin1) of TTL convertor to GND on WSTK

    **![FTDI_prints](resources/readme/usb_to_uart_2.png)**


**Tera Term set up - for NCP and SoC modes**

1. Open the Tera Term tool.

	- For SoC mode, choose the serial port to which USB to UART converter is connected and click on **OK**. 

      **![UART - SoC](resources/readme/port_selection_soc.png)**

	- For NCP mode, choose the J-Link port and click on **OK**.
    
      **![J-link - NCP](resources/readme/port_selection.png)**

2. Navigate to the Setup → Serial port and update the baud rate to **115200** and click on **OK**.

	**![Serial port](resources/readme/serial_port_setup.png)**

	**![Baud rate](resources/readme/serial_port.png)**


## 4 Application Build Environment

The application can be configured to suit your requirements and development environment. 

### 4.1 Configure the application

- The application uses the default configurations as provided in the **default_wifi_ap_profile** in **sl_net_default_valus.h** and user can choose to configure these parameters as needed.

### 4.2 Build the application

-	SoC mode: Build as Access Point Example

  **![Build as](resources/readme/build_accesspoint.png)**

-	NCP mode: 

### 4.3 Run and Test the application

- Once the build was successful, right click on project and click on Debug As->Silicon Labs ARM Program as shown in below image.

  - SoC
    
    **![debug_mode_NCP](resources/readme/program_device.png)**

  - NCP

    **![debug_mode_NCP](resources/readme/program_device.png)**

### 4.4 Application Output

- After the application gets execueted successfully, the MAC addresses of the remote clients that gets connected or disconnected to the newly created Access point are printed on the console.

  **![Access_point_Output](resources/readme/access_point_output.png)**
