#pragma once

// Arduino includes
#include <WiFi.h>
#include <WiFiType.h>

namespace espcpputils {
namespace {
template<typename T>
T scanResultToString(int16_t result)
{
    if (result < 0)
    {
        switch (result)
        {
        case WIFI_SCAN_RUNNING: return "WIFI_SCAN_RUNNING";
        case WIFI_SCAN_FAILED:  return "WIFI_SCAN_FAILED";
        }

        return T{"Failed ("} + result + ')';
    }

    return T{"Finished ("} + result + ')';
}

template<typename T>
T toString(wifi_auth_mode_t encryptionType)
{
    switch (encryptionType)
    {
    case WIFI_AUTH_OPEN:            return "WIFI_AUTH_OPEN";
    case WIFI_AUTH_WEP:             return "WIFI_AUTH_WEP";
    case WIFI_AUTH_WPA_PSK:         return "WIFI_AUTH_WPA_PSK";
    case WIFI_AUTH_WPA2_PSK:        return "WIFI_AUTH_WPA2_PSK";
    case WIFI_AUTH_WPA_WPA2_PSK:    return "WIFI_AUTH_WPA_WPA2_PSK";
    case WIFI_AUTH_WPA2_ENTERPRISE: return "WIFI_AUTH_WPA2_ENTERPRISE";
    case WIFI_AUTH_WPA3_PSK:        return "WIFI_AUTH_WPA3_PSK";
    case WIFI_AUTH_WPA2_WPA3_PSK:   return "WIFI_AUTH_WPA2_WPA3_PSK";
    case WIFI_AUTH_MAX:             return "WIFI_AUTH_MAX";
    }

    return T{"Unknown wifi_auth_mode_t("} + encryptionType + ')';
}

template<typename T>
T toString(wl_status_t status)
{
    switch (status)
    {
    case WL_NO_SHIELD:       return "WL_NO_SHIELD";
    case WL_IDLE_STATUS:     return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL:   return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED:  return "WL_SCAN_COMPLETED";
    case WL_CONNECTED:       return "WL_CONNECTED";
    case WL_CONNECT_FAILED:  return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED:    return "WL_DISCONNECTED";
    }

    return T{"Unknown wl_status_t("} + int(status) + ')';
}
} // namespace
} // namespace espcpputils
