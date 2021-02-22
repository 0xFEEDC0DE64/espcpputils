#pragma once

// esp-idf includes
#include <esp32/rom/crc.h>

namespace espcpputils {

class EspCrc32Builder
{
public:
    std::size_t write(uint8_t c)
    {
        return write(&c, 1);
    }

    std::size_t write(const uint8_t *buf, std::size_t len)
    {
        crc = crc32_le(crc, buf, len);

        return len;
    };

    uint32_t hash() const
    {
        return crc;
    }

private:
    uint32_t crc{};
};

} // namespace espcpputils
