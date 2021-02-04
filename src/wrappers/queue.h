#pragma once

// espressif includes
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

// local includes
#include "cppmacros.h"

namespace espcpputils {
class queue
{
    CPP_DISABLE_COPY_MOVE(queue)

public:
    queue(const UBaseType_t uxQueueLength, const UBaseType_t uxItemSize) :
        handle{xQueueCreate(uxQueueLength, uxItemSize)}
    {}

    ~queue() { if (handle) vQueueDelete(handle); }

    BaseType_t send(const void * pvItemToQueue, TickType_t xTicksToWait)        { return xQueueSend(handle, pvItemToQueue, xTicksToWait); }

    BaseType_t sendToBack(const void * pvItemToQueue, TickType_t xTicksToWait)  { return xQueueSendToBack(handle, pvItemToQueue, xTicksToWait); }

    BaseType_t sendToFront(const void * pvItemToQueue, TickType_t xTicksToWait) { return xQueueSendToFront(handle, pvItemToQueue, xTicksToWait); }

    BaseType_t receive(void *pvBuffer, TickType_t xTicksToWait)                 { return xQueueReceive(handle, pvBuffer, xTicksToWait); }

    UBaseType_t messagesWaiting() const                                         { return uxQueueMessagesWaiting(handle); }

    UBaseType_t spacesAvailable() const                                         { return uxQueueSpacesAvailable(handle); }

    BaseType_t reset()                                                          { return xQueueReset(handle); }

    BaseType_t overwrite(const void * pvItemToQueue)                            { return xQueueOverwrite(handle, pvItemToQueue); }

    BaseType_t peek(void *pvBuffer, TickType_t xTicksToWait) const              { return xQueuePeek(handle, pvBuffer, xTicksToWait); }

#if( configQUEUE_REGISTRY_SIZE > 0 )
    void addToRegistry(char *pcQueueName)                                       { return vQueueAddToRegistry(handle, pcQueueName); }

    void unregisterQueue()                                                      { return vQueueUnregisterQueue(handle); }

    const char *getName() const                                                 { return pcQueueGetName(handle); }
#endif

    const QueueHandle_t handle;
};
} // namespace espcpputils
