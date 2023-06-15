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
                  espchrono::millis_clock::duration loopInterval, bool intervalImportant = false,
                  std::string (*perfInfo)() = nullptr);

    const char *name() const { return m_name; }
    const espchrono::millis_clock::duration&lastElapsed() const { return m_lastElapsed; }
    const espchrono::millis_clock::duration&averageElapsed() const { return m_averageElapsed; }
    const espchrono::millis_clock::duration&maxElapsed() const { return m_maxElapsed; }
    int callCount() const { return m_callCount; }

    void setup() const { m_setupCallback(); m_wasInitialized = true; }
    void loop();
    void pushStats(bool printTask);

private:
    const char * const m_name;
    void (&m_setupCallback)();
    void (&m_loopCallback)();
    const espchrono::millis_clock::duration m_loopInterval;
    const bool m_intervalImportant;
    std::string (* const m_perfInfo)();

    std::optional<espchrono::millis_clock::time_point> m_lastUpdate;

    espchrono::millis_clock::duration m_lastElapsed;

    espchrono::millis_clock::duration m_totalElapsed;
    espchrono::millis_clock::duration m_averageElapsed;
    espchrono::millis_clock::duration m_maxElapsed;
    int m_callCount{};

    espchrono::millis_clock::duration m_totalElapsedTemp;
    espchrono::millis_clock::duration m_maxElapsedTemp;
    int m_callCountTemp{};
    bool m_wasInitialized{};
};
} // namespace espcpputils
