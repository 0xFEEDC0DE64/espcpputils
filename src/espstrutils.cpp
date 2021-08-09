#include "espstrutils.h"

#include "sdkconfig.h"
#define LOG_LOCAL_LEVEL CONFIG_ESPCPPUTILS_LOG_LOCAL_LEVEL

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <fmt/core.h>

// local includes
#include "futurecpp.h"

namespace espcpputils {
namespace {
constexpr const char * const TAG = "ESPCPPUTILS";
}

std::string toString(sntp_sync_mode_t val)
{
    switch (val)
    {
    case SNTP_SYNC_MODE_IMMED:  return "IMMED";
    case SNTP_SYNC_MODE_SMOOTH: return "SMOOTH";
    default:
        ESP_LOGW(TAG, "unknown sntp_sync_mode_t(%i)", std::to_underlying(val));
        return fmt::format("Unknown sntp_sync_mode_t({})", std::to_underlying(val));
    }
}

std::string toString(esp_log_level_t val)
{
    switch (val)
    {
    case ESP_LOG_NONE:    return "NONE";
    case ESP_LOG_ERROR:   return "ERROR";
    case ESP_LOG_WARN:    return "WARN";
    case ESP_LOG_INFO:    return "INFO";
    case ESP_LOG_DEBUG:   return "DEBUG";
    case ESP_LOG_VERBOSE: return "VERBOSE";
    default:
        ESP_LOGW(TAG, "unknown esp_log_level_t(%i)", std::to_underlying(val));
        return fmt::format("Unknown esp_log_level_t({})", std::to_underlying(val));
    }
}

} // namespace espcpputils
