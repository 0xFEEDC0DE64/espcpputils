#pragma once

// system includes
#include <array>
#include <string>

namespace espcpputils {
class mac_t : public std::array<uint8_t, 6>
{
public:
    using std::array<uint8_t, 6>::array;

    explicit mac_t(const uint8_t *ptr)
    {
        std::copy(ptr, ptr+6, std::begin(*this));
    }

    std::string toString() const
    {
        char macStr[18]{0};
        snprintf(macStr, 18, "%02X:%02X:%02X:%02X:%02X:%02X", at(0), at(1), at(2), at(3), at(4), at(5));
        return std::string{macStr};
    }
};
} // namespace
