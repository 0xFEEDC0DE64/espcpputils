#pragma once

// espressif includes
#include <esp_http_client.h>

// local includes
#include "cppmacros.h"

namespace espcpputils {
class http_client
{
    CPP_DISABLE_COPY_MOVE(http_client)

public:
    http_client(const esp_http_client_config_t *config) :
        handle{esp_http_client_init(config)}
    {}

    ~http_client() { if (handle) esp_http_client_cleanup(handle); }

    esp_err_t perform()                                      { return esp_http_client_perform(handle); }
    esp_err_t set_url(const char *url)                       { return esp_http_client_set_url(handle, url); }
    esp_err_t set_post_field(const char *data, int len)      { return esp_http_client_set_post_field(handle, data, len); }
    int       get_post_field(char **data)                    { return esp_http_client_get_post_field(handle, data); }
    esp_err_t set_header(const char *key, const char *value) { return esp_http_client_set_header(handle, key, value); }
    esp_err_t get_header(const char *key, char **value)      { return esp_http_client_get_header(handle, key, value); }
    esp_err_t get_username(char **value)                     { return esp_http_client_get_username(handle, value); }
    esp_err_t set_username(const char *username)             { return esp_http_client_set_username(handle, username); }
    esp_err_t get_password(char **value)                     { return esp_http_client_get_password(handle, value); }
    esp_err_t set_password(char *password)                   { return esp_http_client_set_password(handle, password); }
    esp_err_t set_method(esp_http_client_method_t method)    { return esp_http_client_set_method(handle, method); }
    esp_err_t delete_header(const char *key)                 { return esp_http_client_delete_header(handle, key); }
    esp_err_t open(int write_len)                            { return esp_http_client_open(handle, write_len); }
    int       write(const char *buffer, int len)             { return esp_http_client_write(handle, buffer, len); }
    int       fetch_headers()                                { return esp_http_client_fetch_headers(handle); }
    bool      is_chunked_response() const                    { return esp_http_client_is_chunked_response(handle); }
    int       read(char *buffer, int len)                    { return esp_http_client_read(handle, buffer, len); }
    int       get_status_code() const                        { return esp_http_client_get_status_code(handle); }
    int       get_content_length() const                     { return esp_http_client_get_content_length(handle); }
    esp_err_t close()                                        { return esp_http_client_close(handle); }
    esp_http_client_transport_t get_transport_type() const   { return esp_http_client_get_transport_type(handle); }
    esp_err_t set_redirection()                              { return esp_http_client_set_redirection(handle); }
    void      add_auth()                                     { return esp_http_client_add_auth(handle); }
    bool      is_complete_data_received() const              { return esp_http_client_is_complete_data_received(handle); }

    static esp_http_client_config_t zeroInitializedConfig()
    {
        return esp_http_client_config_t {
            .url = {},
            .host = {},
            .port = {},
            .username = {},
            .password = {},
            .auth_type = {},
            .path = {},
            .query = {},
            .cert_pem = {},
            .client_cert_pem = {},
            .client_key_pem = {},
            .method = {},
            .timeout_ms = {},
            .disable_auto_redirect = {},
            .max_redirection_count = {},
            .event_handler = {},
            .transport_type = {},
            .buffer_size = {},
            .buffer_size_tx = {},
            .user_data = {},
            .is_async = {},
            .use_global_ca_store = {},
            .skip_cert_common_name_check = {}
        };
    }

    const esp_http_client_handle_t handle;
};
} // namespace espcpputils
