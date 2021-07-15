#include "espstrutils.h"

// system includes
#include <cctype>

namespace espcpputils {

std::string toString(sntp_sync_mode_t val)
{
    switch (val)
    {
    case SNTP_SYNC_MODE_IMMED:  return "SNTP_SYNC_MODE_IMMED";
    case SNTP_SYNC_MODE_SMOOTH: return "SNTP_SYNC_MODE_SMOOTH";
    }

    return std::string{"Unknown sntp_sync_mode_t("} + std::to_string(int(val)) + ')';
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

} // namespace espcpputils
