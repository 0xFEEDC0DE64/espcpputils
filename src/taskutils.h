#pragma once

// espressif includes
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// local includes
#include "cpptypesafeenum.h"

namespace espcpputils {
#define CoreAffinityValues(x) \
    x(Core0) \
    x(Core1) \
    x(Both)
DECLARE_TYPESAFE_ENUM(CoreAffinity, : uint8_t, CoreAffinityValues)

BaseType_t createTask(TaskFunction_t pvTaskCode,
                      const char * const pcName,
                      const uint32_t usStackDepth,
                      void * const pvParameters,
                      UBaseType_t uxPriority,
                      TaskHandle_t * const pvCreatedTask,
                      CoreAffinity coreAffinity);
} // namespace espcpputils
