#include "espstrutils.h"

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

} // namespace espcpputils
