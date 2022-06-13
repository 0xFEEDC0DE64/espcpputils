#pragma once

// system includes
#include <limits>

// esp-idf includes
#include <esp_random.h>

namespace espcpputils {

class esp_random_device
{
public:
    using result_type = decltype(esp_random()); // should be uint32_t

    result_type operator()() const { return esp_random(); }

    double entropy() const { return 1.; }
    static constexpr result_type min()  { return std::numeric_limits<result_type>::min(); }
    static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
};

} // namespace espcpputils
