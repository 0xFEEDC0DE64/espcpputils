#pragma once

// system includes
#include <limits>
#include <random>
#include <iterator>
#include <cstring>

// esp-idf includes
#include <esp_system.h>

namespace espcpputils {
namespace {

class esp_random_device
{
public:
    using result_type = decltype(esp_random()); // should be uint32_t

    result_type operator()() const { return esp_random(); }

    double entropy() const { return 1.; }
    static result_type min() { return std::numeric_limits<result_type>::min(); }
    static result_type max() { return std::numeric_limits<result_type>::max(); }
};

std::string randomString(std::size_t length)
{
    static constexpr auto chars =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    auto rng = esp_random_device{};

    auto dist = std::uniform_int_distribution{{}, std::strlen(chars) - 1};

    auto result = std::string(length, '\0');
    std::generate_n(std::begin(result), length, [&]() { return chars[dist(rng)]; });
    return result;
}

} // namespace
} // namespace espcpputils
