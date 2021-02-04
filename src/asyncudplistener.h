#pragma once

// system includes
#include <functional>
#include <cstring>
#include <optional>
#include <array>

// esp-idf includes
#include <lwip/ip_addr.h>
#include <lwip/udp.h>
#include <lwip/pbuf.h>
#include <esp_netif.h>

// local includes
#include "cppmacros.h"
#include "delayedconstruction.h"
#include "wrappers/queue.h"

namespace espcpputils {
class UdpPacketWrapper
{
    CPP_DISABLE_COPY_MOVE(UdpPacketWrapper)

public:
    UdpPacketWrapper(pbuf *pb, const ip_addr_t *addr, uint16_t port, struct netif * netif);

    const uint8_t * data() const { return _data; }
    size_t length() const { return _len; }
    bool isBroadcast() const
    {
        if (_localIp.type == IPADDR_TYPE_V6)
            return false;
        uint32_t ip = _localIp.u_addr.ip4.addr;
        return ip == 0xFFFFFFFF || ip == 0 || (ip & 0xFF000000) == 0xFF000000;
    }
    bool isMulticast() const { return ip_addr_ismulticast(&(_localIp)); }
    bool isIPv6() const { return _localIp.type == IPADDR_TYPE_V6; }

    tcpip_adapter_if_t interface() const { return _if; }

    std::optional<u32_t> localIP() const
    {
        if (_localIp.type != IPADDR_TYPE_V4)
            return std::nullopt;
        return _localIp.u_addr.ip4.addr;
    }

    std::optional<std::array<u32_t, 4>> localIPv6() const
    {
        if (_localIp.type != IPADDR_TYPE_V6)
            return std::nullopt;
        return *reinterpret_cast<const std::array<u32_t, 4>*>(_localIp.u_addr.ip6.addr);
    }

    uint16_t localPort() const { return _localPort; }

    std::optional<u32_t> remoteIP() const
    {
        if (_remoteIp.type != IPADDR_TYPE_V4)
            return std::nullopt;
        return _remoteIp.u_addr.ip4.addr;
    }

    std::optional<std::array<u32_t, 4>> remoteIPv6() const
    {
        if (_remoteIp.type != IPADDR_TYPE_V6)
            return std::nullopt;
        return *reinterpret_cast<const std::array<u32_t, 4>*>(_remoteIp.u_addr.ip6.addr);
    }

    uint16_t remotePort() const { return _remotePort; }

    void remoteMac(uint8_t * mac) const { std::memcpy(mac, _remoteMac, 6); }

private:
    const pbuf *_pb;
    tcpip_adapter_if_t _if;
    ip_addr_t _localIp;
    uint16_t _localPort;
    ip_addr_t _remoteIp;
    uint16_t _remotePort;
    uint8_t _remoteMac[6];
    const uint8_t *_data;
    size_t _len;
    size_t _index;
};

class AsyncUdpListener
{
    CPP_DISABLE_COPY_MOVE(AsyncUdpListener)

public:
    AsyncUdpListener() = default;

    void onPacket(std::function<void(const UdpPacketWrapper &packet)> cb) { _handler = cb; }

    bool listen(const ip_addr_t *addr, uint16_t port);

//    bool listen(const IPAddress addr, uint16_t port)
//    {
//        ip_addr_t laddr;
//        laddr.type = IPADDR_TYPE_V4;
//        laddr.u_addr.ip4.addr = addr;
//        return listen(&laddr, port);
//    }

//    bool listen(const IPv6Address addr, uint16_t port)
//    {
//        ip_addr_t laddr;
//        laddr.type = IPADDR_TYPE_V6;
//        memcpy((uint8_t*)(laddr.u_addr.ip6.addr), (const uint8_t*)addr, 16);
//        return listen(&laddr, port);
//    }

    bool listen(uint16_t port)
    {
        return listen(IP_ANY_TYPE, port);
    }

    void poll(TickType_t xTicksToWait = 0);

    static bool _udp_task_post(void *arg, udp_pcb *pcb, pbuf *pb, const ip_addr_t *addr, uint16_t port, struct netif *netif);

private:
    bool _init();
    void _recv(udp_pcb *upcb, pbuf *pb, const ip_addr_t *addr, uint16_t port, struct netif *netif);
    void close();

private:
    cpputils::DelayedConstruction<espcpputils::queue> _udp_queue;
    udp_pcb *_pcb{};
    bool _connected{};
    std::function<void(const UdpPacketWrapper &packet)> _handler;
};
} // namespace espcpputils
