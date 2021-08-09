#pragma once

// system includes
#include <optional>
#include <chrono>
#include <string>

// local includes
#include "espchrono.h"
#include "arrayview.h"

namespace espcpputils {
class SchedulerTask
{
public:
    SchedulerTask(const char *name, void (&setupCallback)(), void (&loopCallback)(),
                  std::chrono::milliseconds loopInterval, bool intervalImportant = false,
                  std::string (*perfInfo)() = nullptr);

    const char *name() const { return m_name; }
    const std::chrono::milliseconds &lastElapsed() const { return m_lastElapsed; }
    const std::chrono::milliseconds &averageElapsed() const { return m_averageElapsed; }
    const std::chrono::milliseconds &maxElapsed() const { return m_maxElapsed; }
    int callCount() const { return m_callCount; }

    void setup() const { m_setupCallback(); }
    void loop();
    void pushStats(bool printTask);

private:
    const char * const m_name;
    void (&m_setupCallback)();
    void (&m_loopCallback)();
    const std::chrono::milliseconds m_loopInterval;
    const bool m_intervalImportant;
    std::string (* const m_perfInfo)();

    std::optional<espchrono::millis_clock::time_point> m_lastUpdate;

    std::chrono::milliseconds m_lastElapsed;

    std::chrono::milliseconds m_totalElapsed;
    std::chrono::milliseconds m_averageElapsed;
    std::chrono::milliseconds m_maxElapsed;
    int m_callCount{};

    std::chrono::milliseconds m_totalElapsedTemp;
    std::chrono::milliseconds m_maxElapsedTemp;
    int m_callCountTemp{};
};
} // namespace espcpputils
