#pragma once

// system includes
#include <type_traits>
#include <cstddef>
#include <utility>
#include <cstdint>

namespace espcpputils {
class EspFlag
{
    using uint = unsigned int;
    using ushort = unsigned short;

    int i;
public:
    constexpr inline EspFlag(int value) noexcept : i(value) {}
    constexpr inline operator int() const noexcept { return i; }

    constexpr inline EspFlag(uint value) noexcept : i(int(value)) {}
    constexpr inline EspFlag(short value) noexcept : i(int(value)) {}
    constexpr inline EspFlag(ushort value) noexcept : i(int(uint(value))) {}
    constexpr inline operator uint() const noexcept { return uint(i); }
};

template<typename Enum>
class EspFlags
{
    static_assert((sizeof(Enum) <= sizeof(int)),
                      "EspFlags uses an int as storage, so an enum with underlying "
                      "long long will overflow.");
    static_assert((std::is_enum<Enum>::value), "EspFlags is only usable on enumeration types.");

    using uint = unsigned int;
    using ushort = unsigned short;

public:
    typedef typename std::conditional<
            std::is_unsigned<typename std::underlying_type<Enum>::type>::value,
            unsigned int,
            signed int
        >::type Int;

    typedef Enum enum_type;

    constexpr inline EspFlags() noexcept : i(0) {}
    constexpr inline EspFlags(Enum flags) noexcept : i(Int(flags)) {}

    constexpr inline EspFlags(EspFlag flag) noexcept : i(flag) {}

    constexpr inline EspFlags(std::initializer_list<Enum> flags) noexcept
        : i(initializer_list_helper(flags.begin(), flags.end())) {}

    constexpr inline EspFlags &operator&=(int mask) noexcept { i &= mask; return *this; }
    constexpr inline EspFlags &operator&=(uint mask) noexcept { i &= mask; return *this; }
    constexpr inline EspFlags &operator&=(Enum mask) noexcept { i &= Int(mask); return *this; }
    constexpr inline EspFlags &operator|=(EspFlags other) noexcept { i |= other.i; return *this; }
    constexpr inline EspFlags &operator|=(Enum other) noexcept { i |= Int(other); return *this; }
    constexpr inline EspFlags &operator^=(EspFlags other) noexcept { i ^= other.i; return *this; }
    constexpr inline EspFlags &operator^=(Enum other) noexcept { i ^= Int(other); return *this; }

    constexpr inline operator Int() const noexcept { return i; }

    constexpr inline EspFlags operator|(EspFlags other) const noexcept { return EspFlags(EspFlag(i | other.i)); }
    constexpr inline EspFlags operator|(Enum other) const noexcept { return EspFlags(EspFlag(i | Int(other))); }
    constexpr inline EspFlags operator^(EspFlags other) const noexcept { return EspFlags(EspFlag(i ^ other.i)); }
    constexpr inline EspFlags operator^(Enum other) const noexcept { return EspFlags(EspFlag(i ^ Int(other))); }
    constexpr inline EspFlags operator&(int mask) const noexcept { return EspFlags(EspFlag(i & mask)); }
    constexpr inline EspFlags operator&(uint mask) const noexcept { return EspFlags(EspFlag(i & mask)); }
    constexpr inline EspFlags operator&(Enum other) const noexcept { return EspFlags(EspFlag(i & Int(other))); }
    constexpr inline EspFlags operator~() const noexcept { return EspFlags(EspFlag(~i)); }

    constexpr inline bool operator!() const noexcept { return !i; }

    constexpr inline bool testFlag(Enum flag) const noexcept { return (i & Int(flag)) == Int(flag) && (Int(flag) != 0 || i == Int(flag) ); }
    constexpr inline EspFlags &setFlag(Enum flag, bool on = true) noexcept
    {
        return on ? (*this |= flag) : (*this &= ~Int(flag));
    }

private:
    constexpr static inline Int initializer_list_helper(typename std::initializer_list<Enum>::const_iterator it,
                                                               typename std::initializer_list<Enum>::const_iterator end)
    noexcept
    {
        return (it == end ? Int(0) : (Int(*it) | initializer_list_helper(it + 1, end)));
    }

    Int i;
};

class EspIncompatibleFlag
{
    int i;
public:
    constexpr inline explicit EspIncompatibleFlag(int i) noexcept;
    constexpr inline operator int() const noexcept { return i; }
};

constexpr inline EspIncompatibleFlag::EspIncompatibleFlag(int value) noexcept : i(value) {}
} // namespace espcpputils

#define ESP_DECLARE_FLAGS(Flags, Enum)\
typedef ::espcpputils::EspFlags<Enum> Flags;

#define ESP_DECLARE_INCOMPATIBLE_FLAGS(Flags) \
constexpr inline ::espcpputils::EspIncompatibleFlag operator|(Flags::enum_type f1, int f2) noexcept \
{ return ::espcpputils::EspIncompatibleFlag(int(f1) | f2); }

#define ESP_DECLARE_OPERATORS_FOR_FLAGS(Flags) \
constexpr inline ::espcpputils::EspFlags<Flags::enum_type> operator|(Flags::enum_type f1, Flags::enum_type f2) noexcept \
{ return ::espcpputils::EspFlags<Flags::enum_type>(f1) | f2; } \
constexpr inline ::espcpputils::EspFlags<Flags::enum_type> operator|(Flags::enum_type f1, ::espcpputils::EspFlags<Flags::enum_type> f2) noexcept \
{ return f2 | f1; } ESP_DECLARE_INCOMPATIBLE_FLAGS(Flags)
