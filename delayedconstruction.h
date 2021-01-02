#pragma once

// system includes
#include <cassert>
#include <type_traits>
#include <utility>

// local includes
#include "esputils.h"

namespace espcpputils {
template<typename T>
class DelayedConstruction
{
    ESP_DISABLE_COPY_MOVE(DelayedConstruction)

public:
    DelayedConstruction() = default;
    ~DelayedConstruction()
    {
        if (m_constructed)
            destruct();
    }

    template<typename ...Targs>
    void construct(Targs &&...args)
    {
        assert(!m_constructed);
        new (&helper.value) T {std::forward<Targs>(args)...};
        m_constructed = true;
    }

    void destruct()
    {
        assert(m_constructed);
        helper.value.~T();
        m_constructed = false;
    }

    T &get()
    {
        assert(m_constructed);
        return helper.value;
    }

    const T &get() const
    {
        assert(m_constructed);
        return helper.value;
    }

    T *operator->()
    {
        assert(m_constructed);
        return &helper.value;
    }

    const T *operator->() const
    {
        assert(m_constructed);
        return &helper.value;
    }

    T &operator*()
    {
        assert(m_constructed);
        return helper.value;
    }

    const T &operator*() const
    {
        assert(m_constructed);
        return helper.value;
    }

    //! allows for getting the address before the object has been constructed
    T &getUnsafe()
    {
        return helper.value;
    }

    //! allows for getting the address before the object has been constructed
    const T &getUnsafe() const
    {
        return helper.value;
    }

    bool constructed() const { return m_constructed; }

private:
    bool m_constructed{};
    union Helper
    {
        Helper() {}
        ~Helper() {}
        T value;
    } helper;
};
} // namespace espcpputils
