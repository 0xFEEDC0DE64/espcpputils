#include "schedulertask.h"

#include "sdkconfig.h"
#define LOG_LOCAL_LEVEL CONFIG_ESPCPPUTILS_LOG_LOCAL_LEVEL

// esp-idf includes
#include <esp_log.h>
#include <esp_heap_caps.h>

using namespace std::chrono_literals;

namespace espcpputils {
namespace {
constexpr const char * const TAG = "ESPCPPUTILS";
} // namespace

SchedulerTask::SchedulerTask(const char *name, void (&setupCallback)(), void (&loopCallback)(), std::chrono::milliseconds loopInterval, bool intervalImportant, std::string (*perfInfo)()) :
    m_name{name}, m_setupCallback{setupCallback}, m_loopCallback{loopCallback}, m_loopInterval{loopInterval}, m_intervalImportant{intervalImportant}, m_perfInfo{perfInfo}
{
}

void SchedulerTask::loop()
{
    if (m_lastUpdate && espchrono::ago(*m_lastUpdate) < m_loopInterval)
        return;

    ESP_LOGV(TAG, "start %s", m_name);

    if (m_intervalImportant)
    {
        if (!m_lastUpdate)
            goto trotzdem;
        *m_lastUpdate += m_loopInterval;
        if (espchrono::ago(*m_lastUpdate) > m_loopInterval/2)
            goto trotzdem;
    }
    else
trotzdem:
        m_lastUpdate = espchrono::millis_clock::now();

    const auto timeBefore = espchrono::millis_clock::now();
    m_loopCallback();
    const auto timeAfter = espchrono::millis_clock::now();

    ESP_LOGV(TAG, "end %s", m_name);

    m_callCountTemp++;

    m_lastElapsed = timeAfter-timeBefore;
    m_totalElapsedTemp += m_lastElapsed;
    if (m_lastElapsed > m_maxElapsedTemp)
        m_maxElapsedTemp = m_lastElapsed;

    if (m_lastElapsed < 100ms)
        ESP_LOGV(TAG, "task %s hang for %lldms (heap8=%zd)",
                 m_name, std::chrono::milliseconds{m_lastElapsed}.count(), heap_caps_get_free_size(MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT));
    else
        ESP_LOGW(TAG, "task %s hang for %lldms (heap8=%zd) %s",
                 m_name, std::chrono::milliseconds{m_lastElapsed}.count(), heap_caps_get_free_size(MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT),
                 m_perfInfo ? m_perfInfo().c_str() : "");
}

void SchedulerTask::pushStats(bool printTask)
{
    m_callCount = m_callCountTemp;
    m_totalElapsed = m_totalElapsedTemp;
    m_averageElapsed = m_callCount > 0 ? (m_totalElapsed / m_callCount) : std::chrono::milliseconds{};
    m_maxElapsed = m_maxElapsedTemp;

    m_callCountTemp = 0;
    m_totalElapsedTemp = {};
    m_maxElapsedTemp = {};

    if (printTask)
        ESP_LOGI(TAG, "name=%s, count=%i, last=%lldms, total=%lldms, max=%lldms",
                 m_name, m_callCount,
                 std::chrono::milliseconds{m_lastElapsed}.count(),
                 std::chrono::milliseconds{m_totalElapsed}.count(),
                 std::chrono::milliseconds{m_maxElapsed}.count()
        );
}

} // namespace espcpputils
