#pragma once

// system includes
#include <utility>
#include <optional>

namespace espcpputils {
template<typename T>
class CleanupHelper
{
public:
    CleanupHelper(T && cleanup) :
        m_cleanup{std::move(cleanup)}
    {}

    ~CleanupHelper()
    {
        if (m_cleanup)
            (*m_cleanup)();
    }

    void disarm()
    {
        m_cleanup = std::nullopt;
    }

private:
    std::optional<T> m_cleanup;
};

template<typename T>
CleanupHelper<T> makeCleanupHelper(T && cleanup)
{
    return CleanupHelper<T>(std::move(cleanup));
}
} // namespace espcpputils
