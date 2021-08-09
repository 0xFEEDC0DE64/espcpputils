#pragma once

// system includes
#include <string>

// esp-idf includes
#include <esp_sntp.h>
#include <esp_log.h>

namespace espcpputils {

std::string toString(sntp_sync_mode_t val);
std::string toString(esp_log_level_t val);

} // namespace espcpputils
