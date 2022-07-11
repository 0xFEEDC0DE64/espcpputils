#include "espstrutils.h"

#include "sdkconfig.h"
#define LOG_LOCAL_LEVEL CONFIG_ESPCPPUTILS_LOG_LOCAL_LEVEL

// system includes
#include <utility>

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <futurecpp.h>
#include <fmt/core.h>

using namespace std::string_literals;

namespace espcpputils {
namespace {
constexpr const char * const TAG = "ESPCPPUTILS";
} // namespace

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

std::string toString(sntp_sync_status_t val)
{
    switch (val)
    {
    case SNTP_SYNC_STATUS_RESET:       return "RESET";
    case SNTP_SYNC_STATUS_COMPLETED:   return "COMPLETED";
    case SNTP_SYNC_STATUS_IN_PROGRESS: return "IN_PROGRESS";
    default:
        ESP_LOGW(TAG, "unknown sntp_sync_status_t(%i)", std::to_underlying(val));
        return fmt::format("Unknown sntp_sync_status_t({})", std::to_underlying(val));
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

std::string toString(esp_reset_reason_t val)
{
    switch (val)
    {
    case ESP_RST_UNKNOWN:   return "UNKNOWN";
    case ESP_RST_POWERON:   return "POWERON";
    case ESP_RST_EXT:       return "EXT";
    case ESP_RST_SW:        return "SW";
    case ESP_RST_PANIC:     return "PANIC";
    case ESP_RST_INT_WDT:   return "INT_WDT";
    case ESP_RST_TASK_WDT:  return "TASK_WDT";
    case ESP_RST_WDT:       return "WDT";
    case ESP_RST_DEEPSLEEP: return "DEEPSLEEP";
    case ESP_RST_BROWNOUT:  return "BROWNOUT";
    case ESP_RST_SDIO:      return "SDIO";
    default:
        ESP_LOGW(TAG, "unknown esp_reset_reason_t(%i)", std::to_underlying(val));
        return fmt::format("Unknown esp_reset_reason_t({})", std::to_underlying(val));
    }
}

} // namespace espcpputils
