#include "espstrutils.h"

#include "sdkconfig.h"
#define LOG_LOCAL_LEVEL CONFIG_ESPCPPUTILS_LOG_LOCAL_LEVEL

// system includes
#include <assert.h>
#include <utility>

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <fmt/core.h>

using namespace std::string_literals;

namespace espcpputils {
namespace {
constexpr const char * const TAG = "ESPCPPUTILS";

tl::expected<char *, std::string> sodium_bin2hex(char * const hex, const size_t hex_maxlen,
                     const unsigned char * const bin, const size_t bin_len)
    __attribute__ ((nonnull(1)));
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

std::string toHexString(std::basic_string_view<unsigned char> buf)
{
    std::string hex(buf.size() * 2 + 1, {});
    assert(hex.size() == buf.size() * 2 + 1);

    if (const auto result = sodium_bin2hex(hex.data(), hex.size(), buf.data(), buf.size()); !result)
    {
        auto msg = fmt::format("sodium_bin2hex() failed with {}", result.error());
        ESP_LOGW(TAG, "%.*s", msg.size(), msg.data());
        return {};
    }

    hex.resize(hex.size() - 1);
    assert(hex.size() == buf.size() * 2);

    return hex;
}

tl::expected<std::basic_string<unsigned char>, std::string> fromHexString(std::string_view hex)
{
    if (hex.size() % 2 != 0)
        return tl::make_unexpected("hex length not even");

    std::basic_string<unsigned char> result;
    result.reserve(hex.size() / 2);

    for (auto iter = std::cbegin(hex); iter != std::cend(hex); )
    {
        union {
            unsigned char c;
            struct {
                unsigned char nibble1:4;
                unsigned char nibble0:4;
            } nibbles;
        };

        switch (const auto c = *iter)
        {
        case '0'...'9': nibbles.nibble0 = c - '0'; break;
        case 'A'...'F': nibbles.nibble0 = c - 'A' + 10; break;
        case 'a'...'f': nibbles.nibble0 = c - 'a' + 10; break;
        default:
            return tl::make_unexpected(fmt::format("invalid character {} at pos {}", c, std::distance(std::begin(hex), iter)));
        }

        iter++;

        switch (const auto c = *iter)
        {
        case '0'...'9': nibbles.nibble1 = c - '0'; break;
        case 'A'...'F': nibbles.nibble1 = c - 'A' + 10; break;
        case 'a'...'f': nibbles.nibble1 = c - 'a' + 10; break;
        default:
            return tl::make_unexpected(fmt::format("invalid character {} at pos {}", c, std::distance(std::begin(hex), iter)));
        }

        iter++;

        result.push_back(c);
    }

    return result;
}

namespace {
/* Derived from original code by CodesInChaos */
tl::expected<char *, std::string>
sodium_bin2hex(char *const hex, const size_t hex_maxlen,
               const unsigned char *const bin, const size_t bin_len)
{
    size_t       i = (size_t) 0U;
    unsigned int x;
    int          b;
    int          c;

    if (bin_len >= SIZE_MAX / 2 || hex_maxlen <= bin_len * 2U) {
        return tl::make_unexpected("misuse because bin_len >= SIZE_MAX / 2 || hex_maxlen <= bin_len * 2U");
    }
    while (i < bin_len) {
        c = bin[i] & 0xf;
        b = bin[i] >> 4;
        x = (unsigned char) (87U + c + (((c - 10U) >> 8) & ~38U)) << 8 |
            (unsigned char) (87U + b + (((b - 10U) >> 8) & ~38U));
        hex[i * 2U] = (char) x;
        x >>= 8;
        hex[i * 2U + 1U] = (char) x;
        i++;
    }
    hex[i * 2U] = 0U;

    return hex;
}
} // namespace
} // namespace espcpputils
