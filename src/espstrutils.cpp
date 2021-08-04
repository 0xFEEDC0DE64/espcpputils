#include "espstrutils.h"

// esp-idf includes
#include <esp_log.h>
#include <http_parser.h>

// 3rdparty lib includes
#include <fmt/core.h>

// local includes
#include "futurecpp.h"

namespace espcpputils {
namespace {
constexpr const char * const TAG = "ESPCPPUTILS";
}

std::string toString(sntp_sync_mode_t val)
{
    switch (val)
    {
    case SNTP_SYNC_MODE_IMMED:  return "IMMED";
    case SNTP_SYNC_MODE_SMOOTH: return "SMOOTH";
    default:
        ESP_LOGW(TAG, "unknown sntp_sync_mode_t(%i)", std::to_underlying(val));
        return fmt::format("Unknown sntp_sync_mode_t({})", std::to_underlying(val));
    }
}

void urldecode(char *dst, const char *src)
{
    while (*src) {
        char a, b;
        if ((*src == '%') && ((a = src[1]) && (b = src[2])) && (std::isxdigit(a) && std::isxdigit(b))) {
            if (a >= 'a')
                a -= 'a'-'A';
            if (a >= 'A')
                a -= ('A' - 10);
            else
                a -= '0';

            if (b >= 'a')
                b -= 'a'-'A';
            if (b >= 'A')
                b -= ('A' - 10);
            else
                b -= '0';

            *dst++ = 16*a+b;
            src+=3;
        } else if (*src == '+') {
            *dst++ = ' ';
            src++;
        } else
            *dst++ = *src++;
    }

    *dst++ = '\0';
}

tl::expected<void, std::string> urlverify(std::string_view str)
{
    http_parser_url puri;
    http_parser_url_init(&puri);
    if (const auto result = http_parser_parse_url(str.data(), str.size(), 0, &puri); result != 0)
        return tl::make_unexpected(fmt::format("http_parser_parse_url() failed parsing the url with {}", result));
    return {};
}

} // namespace espcpputils
