#pragma once

// espressif includes
#include <esp_websocket_client.h>

// local includes
#include "esputils.h"

namespace espcpputils {
class websocket_client
{
    ESP_DISABLE_COPY_MOVE(websocket_client)

public:
    websocket_client(const esp_websocket_client_config_t &config) :
        handle{esp_websocket_client_init(&config)}
    {
    }

    ~websocket_client() { if (handle) esp_websocket_client_destroy(handle); }

    esp_err_t set_uri        (const char *uri)                                         { return esp_websocket_client_set_uri        (handle, uri);                      }
    esp_err_t start          ()                                                        { return esp_websocket_client_start          (handle);                           }
    esp_err_t stop           ()                                                        { return esp_websocket_client_stop           (handle);                           }
    int       send           (const char *data, int len, TickType_t timeout)           { return esp_websocket_client_send           (handle, data, len, timeout);       }
    int       send_bin       (const char *data, int len, TickType_t timeout)           { return esp_websocket_client_send_bin       (handle, data, len, timeout);       }
    int       send_text      (const char *data, int len, TickType_t timeout)           { return esp_websocket_client_send_text      (handle, data, len, timeout);       }
    //esp_err_t close          (TickType_t timeout)                                      { return esp_websocket_client_close          (handle, timeout);                  }
    //esp_err_t close_with_code(int code, const char *data, int len, TickType_t timeout) { return esp_websocket_client_close_with_code(handle, code, data, len, timeout); }
    bool      is_connected   () const                                                  { return esp_websocket_client_is_connected   (handle); }
    esp_err_t register_events(esp_websocket_event_id_t event, esp_event_handler_t event_handler, void *event_handler_arg)
    { return esp_websocket_register_events(handle, event, event_handler, event_handler_arg); }

    const esp_websocket_client_handle_t handle;
};
} // namespace espcpputils
