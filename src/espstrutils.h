#pragma once

// system includes
#include <string>

// esp-idf includes
#include <esp_sntp.h>

namespace espcpputils {

std::string toString(sntp_sync_mode_t val);

} // namespace espcpputils
