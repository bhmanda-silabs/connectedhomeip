#include "sl_wifi_callback_framework.h"
#include "sl_si91x_host_interface.h"
#include "sl_constants.h"
#include "sl_rsi_utility.h"

/// Entry in the callback table
typedef struct {
  sl_wifi_callback_function_t function; /// User provided callback function pointer
  void *arg;                            /// User provided callback argument
} sl_wifi_callback_entry_t;

//#define EXECUTE_CALLBACK(id, packet) do { if (registered_callbacks[id].function) {return registered_callbacks[id].function(packet->command, packet->data, packet->length, registered_callbacks[id].arg); } } while(0)
static sl_wifi_callback_entry_t *get_callback_entry(sl_wifi_event_t event);

sl_wifi_callback_entry_t registered_callbacks[SL_WIFI_EVENT_COUNT];

sl_status_t sl_wifi_set_callback(sl_wifi_event_t event, sl_wifi_callback_function_t function, void *optional_arg)
{
  sl_wifi_callback_entry_t *entry = get_callback_entry(event);
  if (entry != NULL) {
    entry->function = function;
    entry->arg      = optional_arg;
    return SL_STATUS_OK;
  }
  return SL_STATUS_FAIL;
}

sl_status_t default_wifi_event_handler(sl_wifi_event_t event, sl_wifi_buffer_t *buffer)
{
  sl_wifi_callback_entry_t *entry = get_callback_entry(event);
  sl_si91x_packet_t *packet = (sl_si91x_packet_t *)si91x_host_get_buffer_data((sl_wifi_buffer_t *)buffer, 0, NULL);
  sl_status_t status        = convert_firmware_status(get_si91x_frame_status(packet));

  if (entry != NULL && entry->function != NULL) {
    if (status != SL_STATUS_OK) {
      // If the event has failure bit set then data will be carrying sl_status_t and the data_length will be 0
      return entry->function(event | SL_WIFI_FAIL_STATUS_INDICATION, &status, 0, entry->arg);
    }
    return entry->function(event, packet->data, packet->length, entry->arg);
  }
  return SL_STATUS_OK;
}

static sl_wifi_callback_entry_t *get_callback_entry(sl_wifi_event_t event)
{
  return &registered_callbacks[event];
}
