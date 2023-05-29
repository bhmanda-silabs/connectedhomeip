/*******************************************************************************
* @file  rsi_hal_mcu_m4_rom.c
* @brief 
*******************************************************************************
* # License
* <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
*******************************************************************************
*
* The licensor of this software is Silicon Laboratories Inc. Your use of this
* software is governed by the terms of Silicon Labs Master Software License
* Agreement (MSLA) available at
* www.silabs.com/about-us/legal/master-software-license-agreement. This
* software is distributed to you in Source Code format and is governed by the
* sections of the MSLA applicable to Source Code.
*
******************************************************************************/

#include "sl_device.h"
#include "rsi_m4.h"
#include "sl_status.h"
#include "sl_si91x_host_interface.h"
#include "sl_constants.h"

//! TX & RX descriptors
extern rsi_m4ta_desc_t rx_desc[2];
extern rsi_m4ta_desc_t tx_desc[2];

#ifdef RSI_M4_INTERFACE
uint8_t rx_packet_pending_flag = 0x00;
#endif
/**
 * @fn          sl_status_t sli_si91x_submit_rx_pkt(void)
 * @brief       Submit receiver packets
 * @param[in]   None
 * @return      0 - Success \n
 * 			    Non-Zero - Failure
 */
sl_wifi_buffer_t *rx_pkt_buffer;
sl_status_t sli_si91x_submit_rx_pkt(void)
{
  sl_status_t status;
  uint16_t data_length = 0;
  sl_si91x_packet_t *packet;
  int8_t *pkt_buffer = NULL;

  if (M4SS_P2P_INTR_SET_REG & RX_BUFFER_VALID) {
    return -2;
  }

  // Allocate packet to receive packet from module
  status = si91x_host_allocate_buffer(&rx_pkt_buffer, SL_WIFI_RX_FRAME_BUFFER, 1616, 1000);
  VERIFY_STATUS_AND_RETURN(status);

  packet     = si91x_host_get_buffer_data(rx_pkt_buffer, 0, &data_length);
  pkt_buffer = (int8_t *)&packet->desc[0];

  // Fill source address in the TX descriptors
  rx_desc[0].addr = (M4_MEMORY_OFFSET_ADDRESS + (uint32_t)pkt_buffer);

  // Fill source address in the TX descriptors
  rx_desc[0].length = (16);

  // Fill source address in the TX descriptors
  rx_desc[1].addr = (M4_MEMORY_OFFSET_ADDRESS + (uint32_t)(pkt_buffer + 16));

  // Fill source address in the TX descriptors
  rx_desc[1].length = (1600);

  raise_m4_to_ta_interrupt(RX_BUFFER_VALID);

  return SL_STATUS_OK;
}

/** @addtogroup SOC4
* @{
*/
/**
 * @fn           void sli_si91x_raise_pkt_pending_interrupt_to_ta(void)
 * @brief        Raise the packet pending interrupt to TA
 * @param[in]    void  
 * @return       void
 */
void sli_si91x_raise_pkt_pending_interrupt_to_ta(void)
{
  // Write the packet pending interrupt to TA register
  M4SS_P2P_INTR_SET_REG = TX_PKT_PENDING_INTERRUPT;
}

/**
 * @fn          sl_status_t si91x_bus_write_frame(sl_si91x_packet_t *packet,
 * 									uint8_t *payloadparam, uint16_t size_param)
 * @brief       writing a command to the module.
 * @param[in]   payloadparam - pointer to the command payload parameter structure
 * @param[in]   size_param - size of the payload for the command
 * @return      0              - Success \n 
 *              Negative Value - Failure
 */

sl_status_t si91x_bus_write_frame(sl_si91x_packet_t *packet, const uint8_t *payloadparam, uint16_t size_param)
{

  // Fill source address in the TX descriptors
  tx_desc[0].addr = (M4_MEMORY_OFFSET_ADDRESS + (uint32_t)&packet->desc[0]);

  // Fill source address in the TX descriptors
  tx_desc[0].length = (16);

  // Fill source address in the TX descriptors
  tx_desc[1].addr = (M4_MEMORY_OFFSET_ADDRESS + (uint32_t)payloadparam);

  // Fill source address in the TX descriptors
  tx_desc[1].length = (size_param);

  sli_si91x_raise_pkt_pending_interrupt_to_ta();

  return SL_STATUS_OK;
}

/**
 * @fn          void raise_m4_to_ta_interrupt(uint32_t interrupt_no)
 * @brief       Set interrupt.
 * @param[in]   interrupt_no - Process of a interrupt number 
 * @return      void 
 */

void raise_m4_to_ta_interrupt(uint32_t interrupt_no)
{
  M4SS_P2P_INTR_SET_REG = interrupt_no;
}

/**
 * @fn          void mask_ta_interrupt(uint32_t interrupt_no)
 * @brief       Process a interrupt mask.
 * @param[in]   void  
 * @return      void
 */
void mask_ta_interrupt(uint32_t interrupt_no)
{
  TASS_P2P_INTR_MASK_SET = interrupt_no;
}

/**
 * @fn          void clear_ta_to_m4_interrupt(uint32_t interrupt_no)
 * @brief       Clear interrupt raised by TA.
 * @param[in]   interrupt_no - Process of a interrupt number 
 * @return      void 
 */
void clear_ta_to_m4_interrupt(uint32_t interrupt_no)
{
  TASS_P2P_INTR_CLEAR   = interrupt_no;
  TASS_P2P_INTR_CLR_REG = interrupt_no;
}

sl_status_t si91x_bus_read_frame(sl_wifi_buffer_t **buffer)
{
  sl_status_t status = si91x_host_remove_from_queue(CCP_M4_TA_RX_QUEUE, buffer);
  VERIFY_STATUS_AND_RETURN(status);

  return SL_STATUS_OK;
}

/**
 * @fn           sl_status_t sli_receive_from_ta_done_isr(void)
 * @brief        Called when DMA done for RX packet is received
 * @param[in]    global_cb_p - pointer to the global control block
 * @return       void
 */
sl_status_t sli_receive_from_ta_done_isr(void)
{

  //! Add to rx packet to CCP queue
  sl_status_t status = si91x_host_add_to_queue(CCP_M4_TA_RX_QUEUE, rx_pkt_buffer);
  VERIFY_STATUS_AND_RETURN(status);

  //! Set event RX pending event to host
  si91x_host_set_event(NCP_HOST_BUS_RX_EVENT);

#ifdef RSI_M4_INTERFACE
  // By Setting this flag, make sure that BUS_THREAD will get a chance to read the status of Events when multiple TX Packets are being queued.
  rx_packet_pending_flag = 1;
#endif
  return SL_STATUS_OK;
}

/**
 * @fn           void sli_m4_interrupt_isr(void)
 * @brief        Raise the packet pending interrupt to TA
 * @param[in]    void
 * @return       void
 */

sl_status_t sli_m4_interrupt_isr(void)
{
  if (TASS_P2P_INTR_CLEAR & TX_PKT_TRANSFER_DONE_INTERRUPT) {

    // Clear the interrupt
    clear_ta_to_m4_interrupt(TX_PKT_TRANSFER_DONE_INTERRUPT);

  } else if (TASS_P2P_INTR_CLEAR & RX_PKT_TRANSFER_DONE_INTERRUPT) {

    // Call done interrupt isr
    sl_status_t status = sli_receive_from_ta_done_isr();
    VERIFY_STATUS_AND_RETURN(status);

    // Clear the interrupt
    clear_ta_to_m4_interrupt(RX_PKT_TRANSFER_DONE_INTERRUPT);

  } else {
    SL_DEBUG_LOG("\r\n INVALID INTERRUPT \r\n");
    __asm__("bkpt");
  }
  return SL_STATUS_OK;
}

/*==================================================*/
/**
 * @fn          sl_status_t si91x_bus_read_interrupt_status(uint8_t *int_status)
 * @brief       Returns the value of the Interrupt register
 * @param[in]   status
 * @param[out]  buffer full status reg value
 * @return      errorcode
 *               0 = Success
 *              -2 = Reg read failure
 */
sl_status_t si91x_bus_read_interrupt_status(uint16_t *int_status)
{

  //! copy buffer full status reg value
  *int_status = (uint8_t)HOST_INTR_STATUS_REG;

  return RSI_SUCCESS;
}

void sli_submit_rx_buffer(void)
{
  mask_ta_interrupt(RX_PKT_TRANSFER_DONE_INTERRUPT);

  //! submit to TA submit packet
  sli_si91x_submit_rx_pkt();

  unmask_ta_interrupt(RX_PKT_TRANSFER_DONE_INTERRUPT);
}

sl_status_t si91x_req_wakeup(void)
{
  P2P_STATUS_REG |= M4_wakeup_TA;
  P2P_STATUS_REG;
  P2P_STATUS_REG;
  P2P_STATUS_REG;
  if (!(P2P_STATUS_REG & TA_is_active)) {
    //!TBD Need add timeout
    while (!(P2P_STATUS_REG & TA_is_active))
      ;
  }
  return SL_STATUS_OK;
}

void si91x_host_clear_sleep_indicator(void)
{
  P2P_STATUS_REG &= ~M4_wakeup_TA;
}

void IRQ074_Handler(void)
{
  sli_m4_interrupt_isr();
}
/** @} */
