#include "espstrutils.h"

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

} // namespace espcpputils
