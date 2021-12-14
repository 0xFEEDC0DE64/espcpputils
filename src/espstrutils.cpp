#include "espstrutils.h"

#include "sdkconfig.h"
#define LOG_LOCAL_LEVEL CONFIG_ESPCPPUTILS_LOG_LOCAL_LEVEL

#include <assert.h>

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <fmt/core.h>

// local includes
#include "futurecpp.h"

using namespace std::string_literals;

namespace espcpputils {
namespace {
constexpr const char * const TAG = "ESPCPPUTILS";

tl::expected<char *, std::string> sodium_bin2hex(char * const hex, const size_t hex_maxlen,
                     const unsigned char * const bin, const size_t bin_len)
    __attribute__ ((nonnull(1)));
tl::expected<void, std::string> sodium_hex2bin(unsigned char * const bin, const size_t bin_maxlen,
                   const char * const hex, const size_t hex_len,
                   const char * const ignore, size_t * const bin_len,
                   const char ** const hex_end)
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

tl::expected<std::basic_string_view<unsigned char>, std::string> fromHexString(std::string_view str)
{
    const auto binMaxLen = (str.size()+1)/2;
    uint8_t binBuf[binMaxLen];

    size_t binLen;
    if (const auto result = sodium_hex2bin(binBuf, binMaxLen, str.data(), str.size(), NULL, &binLen, NULL); !result)
    {
        auto msg = fmt::format("sodium_hex2bin() failed with {}", result.error());
        ESP_LOGW(TAG, "%.*s", msg.size(), msg.data());
        return tl::make_unexpected(std::move(msg));
    }

    if (binLen != str.size() / 2)
    {
        ESP_LOGW(TAG, "invalid hex");
        return tl::make_unexpected("invalid hex");
    }

    const std::basic_string_view<unsigned char> bin{binBuf, binLen};

    return bin;
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

tl::expected<void, std::string>
sodium_hex2bin(unsigned char *const bin, const size_t bin_maxlen,
               const char *const hex, const size_t hex_len,
               const char *const ignore, size_t *const bin_len,
               const char **const hex_end)
{
    size_t        bin_pos = (size_t) 0U;
    size_t        hex_pos = (size_t) 0U;
    tl::expected<void, std::string> ret;
    unsigned char c;
    unsigned char c_acc = 0U;
    unsigned char c_alpha0, c_alpha;
    unsigned char c_num0, c_num;
    unsigned char c_val;
    unsigned char state = 0U;

    while (hex_pos < hex_len) {
        c        = (unsigned char) hex[hex_pos];
        c_num    = c ^ 48U;
        c_num0   = (c_num - 10U) >> 8;
        c_alpha  = (c & ~32U) - 55U;
        c_alpha0 = ((c_alpha - 10U) ^ (c_alpha - 16U)) >> 8;
        if ((c_num0 | c_alpha0) == 0U) {
            if (ignore != NULL && state == 0U && strchr(ignore, c) != NULL) {
                hex_pos++;
                continue;
            }
            break;
        }
        c_val = (c_num0 & c_num) | (c_alpha0 & c_alpha);
        if (bin_pos >= bin_maxlen) {
            ret = tl::make_unexpected("ERANGE because bin_pos >= bin_maxlen"s);
            break;
        }
        if (state == 0U) {
            c_acc = c_val * 16U;
        } else {
            bin[bin_pos++] = c_acc | c_val;
        }
        state = ~state;
        hex_pos++;
    }
    if (state != 0U) {
        hex_pos--;
        ret   = tl::make_unexpected("EINVAL because state != 0U"s);
    }
    if (!ret) {
        bin_pos = (size_t) 0U;
    }
    if (hex_end != NULL) {
        *hex_end = &hex[hex_pos];
    } else if (hex_pos != hex_len) {
        ret   = tl::make_unexpected("EINVAL because hex_pos != hex_len"s);
    }
    if (bin_len != NULL) {
        *bin_len = bin_pos;
    }
    return ret;
}
} // namespace
} // namespace espcpputils
