#pragma once

// espressif includes
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

// local includes
#include "cppmacros.h"

namespace espcpputils {
class LockHelper
{
    CPP_DISABLE_COPY_MOVE(LockHelper)

public:
    LockHelper(SemaphoreHandle_t xMutex, TickType_t xTicksToWait = portMAX_DELAY) :
        m_xMutex{xMutex},
        m_locked{xSemaphoreTake(m_xMutex, xTicksToWait) == pdPASS}
    {}

    ~LockHelper()
    {
        if (m_locked)
            xSemaphoreGive(m_xMutex);
    }

    bool lock(TickType_t xTicksToWait = portMAX_DELAY)
    {
        if (m_locked)
            return false;

        m_locked = xSemaphoreTake(m_xMutex, xTicksToWait);

        return m_locked;
    }

    bool unlock()
    {
        if (!m_locked)
            return false;

        m_locked = !xSemaphoreGive(m_xMutex);

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
