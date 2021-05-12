#pragma once

// system includes
#include <atomic>
#include <optional>
#include <queue>

// espressif includes
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

// local includes
#include "delayedconstruction.h"
#include "wrappers/mutex_semaphore.h"
#include "lockhelper.h"

namespace espcpputils {
template<typename T>
class LockingQueue
{
public:
    void push(const T &val);
    void push(T &&val);

    std::optional<T> tryPop();

    void clear();

    std::size_t size() const { return m_size.load(); }

private:
    espcpputils::mutex_semaphore m_lock;
    std::queue<T> m_queue;
    std::atomic<std::size_t> m_size{}; // double-buffered to allow for reading without taking a lock
};

template<typename T>
void LockingQueue<T>::push(const T &val)
{
    LockHelper helper{m_lock.handle};
    m_queue.push(val);
    m_size = m_queue.size();
}

template<typename T>
void LockingQueue<T>::push(T &&val)
{
    LockHelper helper{m_lock.handle};
    m_queue.emplace(std::move(val));
    m_size = m_queue.size();
}

template<typename T>
std::optional<T> LockingQueue<T>::tryPop()
{
    LockHelper helper{m_lock.handle};
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
    std::queue<T> empty;

    {
        LockHelper helper{m_lock.handle};
        //m_queue.clear();
        //m_queue = {};
        std::swap(empty, m_queue);
        m_size = {};
    }

    // Deconstructing here the queue as the lock is already given back again
}
} // namespace espcpputils
