#pragma once

namespace espcpputils {

#define CALL_AND_EXIT_ON_ERROR(METHOD, ...) \
    if (const auto result##__LINE__ = METHOD(__VA_ARGS__); result##__LINE__ != ESP_OK) \
    { \
        ESP_LOGE(TAG, "%s() failed with %s", #METHOD, esp_err_to_name(result##__LINE__)); \
        return result##__LINE__; \
    }

#define CALL_AND_EXIT(METHOD, ...) \
    if (const auto result##__LINE__ = METHOD(__VA_ARGS__); result##__LINE__ != ESP_OK) \
    { \
        ESP_LOGE(TAG, "%s() failed with %s", #METHOD, esp_err_to_name(result##__LINE__)); \
        return result##__LINE__; \
    } \
    else \
        return result##__LINE__;

} // namespace espcpputils
