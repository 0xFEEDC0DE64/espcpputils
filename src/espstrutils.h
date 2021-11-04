#pragma once

// system includes
#include <string>
#include <string_view>

// 3rdparty lib includes
#include <tl/expected.hpp>

// esp-idf includes
#include <esp_sntp.h>
#include <esp_log.h>

namespace espcpputils {

std::string toString(sntp_sync_mode_t val);
std::string toString(sntp_sync_status_t val);
std::string toString(esp_log_level_t val);
std::string toString(esp_reset_reason_t val);

std::string toHexString(std::basic_string_view<unsigned char> buf);
inline std::string toHexString(std::string_view str)
{
    return toHexString(std::basic_string_view<unsigned char>{reinterpret_cast<const unsigned char *>(str.data()), str.size()});
}

tl::expected<std::basic_string_view<unsigned char>, std::string> fromHexString(std::string_view str);
inline tl::expected<std::basic_string_view<unsigned char>, std::string> fromHexString(std::basic_string_view<unsigned char> str)
{
    return fromHexString(std::string_view{reinterpret_cast<const char *>(str.data()), str.size()});
}

} // namespace espcpputils
