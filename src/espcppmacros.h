#pragma once

namespace espcpputils {

#define CALL_AND_EXIT_ON_ERROR(METHOD, ...) \
    if (const auto result = METHOD(__VA_ARGS__); result != ESP_OK) \
    { \
        ESP_LOGE(TAG, "%s() failed with %s", #METHOD, esp_err_to_name(result)); \
        return result; \
    }

#define CALL_AND_EXIT(METHOD, ...) \
    if (const auto result = METHOD(__VA_ARGS__); result != ESP_OK) \
    { \
        ESP_LOGE(TAG, "%s() failed with %s", #METHOD, esp_err_to_name(result)); \
        return result; \
    } \
    else \
        return result;

} // namespace espcpputils
