#pragma once

// espressif includes
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>

// local includes
#include "cppmacros.h"

namespace espcpputils {
class event_group
{
    CPP_DISABLE_COPY_MOVE(event_group)

public:
    event_group() :
        handle{xEventGroupCreate()}
    {}

    ~event_group() { if (handle) vEventGroupDelete(handle); }

    EventBits_t waitBits(const EventBits_t uxBitsToWaitFor, const BaseType_t xClearOnExit, const BaseType_t xWaitForAllBits, TickType_t xTicksToWait)
    { return xEventGroupWaitBits(handle, uxBitsToWaitFor, xClearOnExit, xWaitForAllBits, xTicksToWait); }

    EventBits_t clearBits(const EventBits_t uxBitsToClear) { return xEventGroupClearBits(handle, uxBitsToClear); }

    EventBits_t setBits(const EventBits_t uxBitsToSet) { return xEventGroupSetBits(handle, uxBitsToSet); }

    EventBits_t groupSync(const EventBits_t uxBitsToSet, const EventBits_t uxBitsToWaitFor, TickType_t xTicksToWait)
    { return xEventGroupSync(handle, uxBitsToSet, uxBitsToWaitFor, xTicksToWait); }

    EventBits_t getBits() const { return xEventGroupClearBits(handle, 0); }

    const EventGroupHandle_t handle;
};
} // namespace espcpputils
