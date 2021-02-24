#pragma once

// system includes
#include <chrono>
#include <ratio>

// esp-idf includes
#include <freertos/FreeRTOS.h>
#include <freertos/projdefs.h>

namespace espcpputils {

using ticks = std::chrono::duration<TickType_t, std::ratio<1, configTICK_RATE_HZ>>;

static_assert(pdMS_TO_TICKS(1000) == ticks(std::chrono::seconds(1)).count());
static_assert(pdMS_TO_TICKS(100) == std::chrono::floor<ticks>(std::chrono::milliseconds(100)).count());
static_assert(pdMS_TO_TICKS(10) == std::chrono::floor<ticks>(std::chrono::milliseconds(10)).count());

} // namespace espcpputils

