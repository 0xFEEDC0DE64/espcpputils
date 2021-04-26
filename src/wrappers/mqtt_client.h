#pragma once

// system includes
#include <utility>
#include <string_view>

// esp-idf includes
#include <mqtt_client.h>

namespace espcpputils {
class mqtt_client
{
public:
    mqtt_client() : handle{NULL} {}

    mqtt_client(const esp_mqtt_client_config_t *config) : handle{esp_mqtt_client_init(config)} {}

    mqtt_client(const mqtt_client &) = delete;

    mqtt_client(mqtt_client &&other) : handle{std::move(other.handle)} { other.handle = NULL; }

    mqtt_client &operator=(const mqtt_client &) = delete;

    mqtt_client &operator=(mqtt_client &&other) { if (handle) esp_mqtt_client_destroy(handle); handle = std::move(other.handle); other.handle = NULL; return *this; }

    ~mqtt_client() { if (handle) esp_mqtt_client_destroy(handle); }

    operator bool() const { return handle != NULL; }

    //esp_err_t set_uri        (const char *uri)                                                                       { return esp_mqtt_client_set_uri        (handle, uri);                                      }
    esp_err_t set_uri        (std::string_view uri)                                                                  { return esp_mqtt_client_set_uri        (handle, uri.data());                               }
    esp_err_t start          ()                                                                                      { return esp_mqtt_client_start          (handle);                                           }
    esp_err_t reconnect      ()                                                                                      { return esp_mqtt_client_reconnect      (handle);                                           }
    esp_err_t disconnect     ()                                                                                      { return esp_mqtt_client_disconnect     (handle);                                           }
    esp_err_t stop           ()                                                                                      { return esp_mqtt_client_stop           (handle);                                           }
    //int       subscribe      (const char *topic, int qos)                                                            { return esp_mqtt_client_subscribe      (handle, topic, qos);                               }
    int       subscribe      (std::string_view topic, int qos)                                                       { return esp_mqtt_client_subscribe      (handle, topic.data(), qos);                        }
    //int       unsubscribe    (const char *topic)                                                                     { return esp_mqtt_client_unsubscribe    (handle, topic);                                    }
    int       unsubscribe    (std::string_view topic)                                                                { return esp_mqtt_client_unsubscribe    (handle, topic.data());                             }
    //int       publish        (const char *topic, const char *data, int len, int qos, int retain)                     { return esp_mqtt_client_publish        (handle, topic, data, len, qos, retain);            }
    int       publish        (std::string_view topic, std::string_view buf, int qos, int retain)                     { return esp_mqtt_client_publish        (handle, topic.data(), buf.data(), buf.size(), qos, retain); }
    //int       enqueue        (const char *topic, const char *data, int len, int qos, int retain, bool store)         { return esp_mqtt_client_enqueue        (handle, topic, data, len, qos, retain, store);     }
    int       enqueue        (std::string_view topic, std::string_view buf, int qos, int retain, bool store)         { return esp_mqtt_client_enqueue        (handle, topic.data(), buf.data(), buf.size(), qos, retain, store); }
    esp_err_t set_config     (const esp_mqtt_client_config_t *config)                                                { return esp_mqtt_set_config            (handle, config);                                   }
    esp_err_t register_event (esp_mqtt_event_id_t event, esp_event_handler_t event_handler, void* event_handler_arg) { return esp_mqtt_client_register_event (handle, event, event_handler, event_handler_arg); }
    int       get_outbox_size() const                                                                                { return esp_mqtt_client_get_outbox_size(handle);                                           }

private:
    esp_mqtt_client_handle_t handle;
};
}
