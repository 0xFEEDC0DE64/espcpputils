#pragma once

// system includes
#include <algorithm>
#include <cstdint>
#include <functional>
#include <vector>
#include <string>
#include <string_view>

/*
   Avoid "unused parameter" warnings
*/
#define ESP_UNUSED(x) (void)x;

/* These two macros make it possible to turn the builtin line expander into a
 * string literal. */
#define ESP_STRINGIFY2(x) #x
#define ESP_STRINGIFY(x) ESP_STRINGIFY2(x)

/*
   Some classes do not permit copies to be made of an object. These
   classes contains a private copy constructor and assignment
   operator to disable copying (the compiler gives an error message).
*/
#define ESP_DISABLE_COPY(Class) \
    Class(const Class &) = delete;\
    Class &operator=(const Class &) = delete;
#define ESP_DISABLE_MOVE(Class) \
    Class(Class &&) = delete; \
    Class &operator=(Class &&) = delete;
#define ESP_DISABLE_COPY_MOVE(Class) \
    ESP_DISABLE_COPY(Class) \
    ESP_DISABLE_MOVE(Class)


/* These two macros make it possible to define a typesafe enum with parse and
 * toString methods */
#define DECLARE_TYPESAFE_ENUM_HELPER1(name) name,
#define DECLARE_TYPESAFE_ENUM_HELPER2(name) case TheEnum::name: return #name;
#define DECLARE_TYPESAFE_ENUM_HELPER3(name) else if (str == ESP_STRINGIFY(name)) return TheEnum::name;

#define DECLARE_TYPESAFE_ENUM(Name, Derivation, Values) \
    enum class Name Derivation \
    { \
        Values(DECLARE_TYPESAFE_ENUM_HELPER1) \
    }; \
    std::string toString(Name value); \
    std::optional<Name> parse##Name(std::string_view str);

#define IMPLEMENT_TYPESAFE_ENUM(Name, Derivation, Values) \
    std::string toString(Name value) \
    { \
        switch (value) \
        { \
        using TheEnum = Name; \
        Values(DECLARE_TYPESAFE_ENUM_HELPER2) \
        } \
        return std::string{"Unknown " #Name "("} + std::to_string(int(value)) + ')'; \
    } \
    std::optional<Name> parse##Name(std::string_view str) \
    { \
    using TheEnum = Name; \
    if (false) {} \
    Values(DECLARE_TYPESAFE_ENUM_HELPER3) \
    return std::nullopt; \
    }

namespace espcpputils {

template<typename... T>
class Signal
{
public:
    using Slot = std::function<void(T...)>;

    Signal &operator+=(Slot &&slot)
    {
        m_slots.emplace_back(std::move(slot));
        return *this;
    }

    Signal &operator+=(const Slot &slot)
    {
        m_slots.emplace_back(slot);
        return *this;
    }

    template<typename ...Targs>
    void operator()(Targs && ...args) const
    {
        for (const auto &slot : m_slots)
            slot(std::forward<Targs>(args)...);
    }

private:
    std::vector<Slot> m_slots;
};

namespace literals {
namespace {
/**
 * User-defined Literals
 *  usage:
 *
 *   uint32_t = test = 10_MHz; // --> 10000000
 */

constexpr unsigned long long operator"" _kHz(unsigned long long x)
{
    return x * 1000;
}

constexpr unsigned long long operator"" _MHz(unsigned long long x)
{
    return x * 1000 * 1000;
}

constexpr unsigned long long operator"" _GHz(unsigned long long x)
{
    return x * 1000 * 1000 * 1000;
}

constexpr unsigned long long operator"" _kBit(unsigned long long x)
{
    return x * 1024;
}

constexpr unsigned long long operator"" _MBit(unsigned long long x)
{
    return x * 1024 * 1024;
}

constexpr unsigned long long operator"" _GBit(unsigned long long x)
{
    return x * 1024 * 1024 * 1024;
}

constexpr unsigned long long operator"" _kB(unsigned long long x)
{
    return x * 1024;
}

constexpr unsigned long long operator"" _MB(unsigned long long x)
{
    return x * 1024 * 1024;
}

constexpr unsigned long long operator"" _GB(unsigned long long x)
{
    return x * 1024 * 1024 * 1024;
}
} // namespace
} // namespace literals

namespace {
template<typename T>
T vmin(T&&t)
{
    return std::forward<T>(t);
}

template<typename T0, typename T1, typename... Ts>
typename std::common_type<T0, T1, Ts...>::type vmin(T0&& val1, T1&& val2, Ts&&... vs)
{
    if (val1 < val2)
        return vmin(val1, std::forward<Ts>(vs)...);
    else
        return vmin(val2, std::forward<Ts>(vs)...);
}

template<class T>
constexpr const T& clamp( const T& v, const T& lo, const T& hi )
{
    // assert( !(hi < lo) );
    return (v < lo) ? lo : (hi < v) ? hi : v;
}

template<typename T>
T mapValue(T x, T in_min, T in_max, T out_min, T out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

template <typename... Args>
struct EspNonConstOverload
{
    template <typename R, typename T>
    constexpr auto operator()(R (T::*ptr)(Args...)) const noexcept -> decltype(ptr)
    { return ptr; }
    template <typename R, typename T>
    static constexpr auto of(R (T::*ptr)(Args...)) noexcept -> decltype(ptr)
    { return ptr; }
};
template <typename... Args>
struct EspConstOverload
{
    template <typename R, typename T>
    constexpr auto operator()(R (T::*ptr)(Args...) const) const noexcept -> decltype(ptr)
    { return ptr; }
    template <typename R, typename T>
    static constexpr auto of(R (T::*ptr)(Args...) const) noexcept -> decltype(ptr)
    { return ptr; }
};
template <typename... Args>
struct EspOverload : EspConstOverload<Args...>, EspNonConstOverload<Args...>
{
    using EspConstOverload<Args...>::of;
    using EspConstOverload<Args...>::operator();
    using EspNonConstOverload<Args...>::of;
    using EspNonConstOverload<Args...>::operator();
    template <typename R>
    constexpr auto operator()(R (*ptr)(Args...)) const noexcept -> decltype(ptr)
    { return ptr; }
    template <typename R>
    static constexpr auto of(R (*ptr)(Args...)) noexcept -> decltype(ptr)
    { return ptr; }
};

template <typename... Args> constexpr __attribute__((__unused__)) EspOverload<Args...> espOverload = {};
template <typename... Args> constexpr __attribute__((__unused__)) EspConstOverload<Args...> espConstOverload = {};
template <typename... Args> constexpr __attribute__((__unused__)) EspNonConstOverload<Args...> espNonConstOverload = {};

template<typename First, typename ... T>
bool is_in(First &&first, T && ... t)
{
    return ((first == t) || ...);
}

} // namespace
} // namespace espcpputils
