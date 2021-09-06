#pragma once

// espressif includes
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

// local includes
#include "cppmacros.h"

namespace espcpputils {
class RecursiveLockHelper
{
    CPP_DISABLE_COPY_MOVE(RecursiveLockHelper)

public:
    RecursiveLockHelper(SemaphoreHandle_t xMutex, TickType_t xTicksToWait = portMAX_DELAY) :
        m_xMutex{xMutex},
        m_locked{xSemaphoreTakeRecursive(xMutex, xTicksToWait) == pdPASS}
    {}

    ~RecursiveLockHelper()
    {
        if (m_locked)
            xSemaphoreGiveRecursive(m_xMutex);
    }

    bool lock(TickType_t xTicksToWait = portMAX_DELAY)
    {
        if (m_locked)
            return false;

        m_locked = xSemaphoreTakeRecursive(m_xMutex, xTicksToWait);

        return m_locked;
    }

    bool unlock()
    {
        if (!m_locked)
            return false;

        m_locked = !xSemaphoreGiveRecursive(m_xMutex);

        return m_locked;
    }

    bool locked() const
    {
        return m_locked;
    }

private:
    const SemaphoreHandle_t m_xMutex;
    bool m_locked;
};
} // namespace espcpputils
