#pragma once

// system includes
#include <optional>
#include <queue>

// espressif includes
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

// local includes
#include "delayedconstruction.h"
#include "wrappers/recursive_mutex_semaphore.h"
#include "recursivelockhelper.h"

namespace espcpputils {
template<typename T>
class LockingQueue
{
public:
    void push(const T &val);
    void push(T &&val);

    std::optional<T> tryPop();

    void clear();

    std::size_t size() const { return m_size; }

private:
    espcpputils::recursive_mutex_semaphore m_lock;
    std::queue<T> m_queue;
    std::size_t m_size{}; // double-buffered to allow for reading without taking a lock
};

template<typename T>
void LockingQueue<T>::push(const T &val)
{
    RecursiveLockHelper helper{m_lock.handle};
    m_queue.push(val);
    m_size = m_queue.size();
}

template<typename T>
void LockingQueue<T>::push(T &&val)
{
    RecursiveLockHelper helper{m_lock.handle};
    m_queue.emplace(std::move(val));
    m_size = m_queue.size();
}

template<typename T>
std::optional<T> LockingQueue<T>::tryPop()
{
    RecursiveLockHelper helper{m_lock.handle};
    if (m_queue.empty())
        return std::nullopt;

    std::optional<T> temp = std::move(m_queue.front());
    m_queue.pop();
    m_size = m_queue.size();
    return temp;
}

template<typename T>
void LockingQueue<T>::clear()
{
    RecursiveLockHelper helper{m_lock.handle};
    m_queue = {};
}
} // namespace espcpputils
