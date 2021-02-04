#pragma once

// espressif includes
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

// local includes
#include "cppmacros.h"

namespace espcpputils {
class mutex_semaphore
{
    CPP_DISABLE_COPY_MOVE(mutex_semaphore)

public:
    mutex_semaphore() :
        handle{xSemaphoreCreateMutex()}
    {}

    ~mutex_semaphore() { if (handle) vSemaphoreDelete(handle); }

    TaskHandle_t getMutexHolder() const { return xSemaphoreGetMutexHolder(handle); }

    UBaseType_t getCount() const        { return uxSemaphoreGetCount(handle); }

    bool take(TickType_t xTicksToWait)  { return xSemaphoreTake(handle, xTicksToWait); }

    bool give()                         { return xSemaphoreGive(handle); }

    const SemaphoreHandle_t handle;
};
} // namespace espcpputils
