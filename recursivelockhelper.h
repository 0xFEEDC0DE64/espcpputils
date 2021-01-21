#pragma once

// espressif includes
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

// local includes
#include "esputils.h"

namespace espcpputils {
class RecursiveLockHelper
{
    ESP_DISABLE_COPY_MOVE(RecursiveLockHelper)

public:
    RecursiveLockHelper(QueueHandle_t _xMutex, TickType_t xTicksToWait=portMAX_DELAY) :
        xMutex{_xMutex}
    {
        locked = xSemaphoreTakeRecursive(xMutex, xTicksToWait);
    }

    ~RecursiveLockHelper()
    {
        if (locked)
            xSemaphoreGiveRecursive(xMutex);
    }

    bool lock(TickType_t xTicksToWait=portMAX_DELAY)
    {
        if (locked)
            return false;

        locked = xSemaphoreTakeRecursive(xMutex, xTicksToWait);

        return locked;
    }

    bool unlock()
    {
        if (!locked)
            return false;

        locked = !xSemaphoreGiveRecursive(xMutex);

        return locked;
    }

private:
    const QueueHandle_t xMutex;
    bool locked;
};
} // namespace espcpputils