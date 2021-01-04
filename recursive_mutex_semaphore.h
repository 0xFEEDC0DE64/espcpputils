#pragma once

// espressif includes
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

// local includes
#include "esputils.h"

namespace espcpputils {
class recursive_mutex_semaphore
{
    ESP_DISABLE_COPY_MOVE(recursive_mutex_semaphore)

public:
    recursive_mutex_semaphore() :
        handle{xSemaphoreCreateRecursiveMutex()}
    {}

    ~recursive_mutex_semaphore() { if (handle) vSemaphoreDelete(handle); }

    TaskHandle_t getMutexHolder() const { return xSemaphoreGetMutexHolder(handle); }

    UBaseType_t getCount() const        { return uxSemaphoreGetCount(handle); }

    bool takeRecursive(TickType_t xTicksToWait) { return xSemaphoreTakeRecursive(handle, xTicksToWait); }

    bool giveRecursive()                { return xSemaphoreGiveRecursive(handle); }

    const SemaphoreHandle_t handle;
};
} // namespace espcpputils
