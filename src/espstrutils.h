#pragma once

// system includes
#include <string>

// esp-idf includes
#include <esp_sntp.h>

// 3rdparty lib includes
#include <tl/expected.hpp>

namespace espcpputils {

std::string toString(sntp_sync_mode_t val);

void urldecode(char *dst, const char *src);

tl::expected<void, std::string> urlverify(std::string_view str);

} // namespace espcpputils
