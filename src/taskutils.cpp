#include "taskutils.h"

#include "sdkconfig.h"
#define LOG_LOCAL_LEVEL CONFIG_ESPCPPUTILS_LOG_LOCAL_LEVEL

// system includes
#include <utility>

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <futurecpp.h>

namespace espcpputils {
namespace {
constexpr const char * const TAG = "ESPCPPUTILS";
} // namespace

BaseType_t createTask(TaskFunction_t pvTaskCode,
                      const char * const pcName,
                      const uint32_t usStackDepth,
                      void * const pvParameters,
                      UBaseType_t uxPriority,
                      TaskHandle_t * const pvCreatedTask,
                      CoreAffinity coreAffinity)
{
    switch (coreAffinity)
    {
    case CoreAffinity::Core0:
    case CoreAffinity::Core1:
    {
        const auto result = xTaskCreatePinnedToCore(pvTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pvCreatedTask, int(coreAffinity));
        if (result != pdPASS)
            ESP_LOGW(TAG, "xTaskCreatePinnedToCore() %s failed with %i", pcName, result);
        return result;
    }
    case CoreAffinity::Both:
    {
        const auto result = xTaskCreate(pvTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pvCreatedTask);
        if (result != pdPASS)
            ESP_LOGW(TAG, "xTaskCreate() %s failed with %i", pcName, result);
        return result;
    }
    default:
        ESP_LOGE(TAG, "unknown coreAffinity %i", std::to_underlying(coreAffinity));
        return pdFAIL;
    }
}

} // namespace espcpputils
