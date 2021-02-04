#include "asyncudplistener.h"

// system includes
#include <cassert>

// esp-idf includes
#include <lwip/priv/tcpip_priv.h>
#include <lwip/prot/ethernet.h>
#include <esp_log.h>

// local utils
#include "taskutils.h"

namespace espcpputils {
namespace {
typedef struct
{
    void *arg;
    udp_pcb *pcb;
    pbuf *pb;
    const ip_addr_t *addr;
    uint16_t port;
    struct netif *netif;
} lwip_event_packet_t;

typedef struct
{
    struct tcpip_api_call_data call;
    udp_pcb * pcb;
    const ip_addr_t *addr;
    uint16_t port;
    struct pbuf *pb;
    struct netif *netif;
    err_t err;
} udp_api_call_t;

err_t _udp_bind_api(struct tcpip_api_call_data *api_call_msg)
{
    udp_api_call_t *msg = (udp_api_call_t *)api_call_msg;
    msg->err = udp_bind(msg->pcb, msg->addr, msg->port);
    return msg->err;
}

err_t _udp_bind(struct udp_pcb *pcb, const ip_addr_t *addr, u16_t port)
{
    udp_api_call_t msg;
    msg.pcb = pcb;
    msg.addr = addr;
    msg.port = port;
    tcpip_api_call(_udp_bind_api, (struct tcpip_api_call_data*)&msg);
    return msg.err;
}

err_t _udp_disconnect_api(struct tcpip_api_call_data *api_call_msg)
{
    udp_api_call_t *msg = (udp_api_call_t *)api_call_msg;
    msg->err = 0;
    udp_disconnect(msg->pcb);
    return msg->err;
}

void _udp_disconnect(struct udp_pcb *pcb)
{
    udp_api_call_t msg;
    msg.pcb = pcb;
    tcpip_api_call(_udp_disconnect_api, (struct tcpip_api_call_data*)&msg);
}

void _udp_recv(void *arg, udp_pcb *pcb, pbuf *pb, const ip_addr_t *addr, uint16_t port)
{
    while (pb != NULL)
    {
        pbuf *this_pb = pb;
        pb = pb->next;
        this_pb->next = NULL;

        if (!AsyncUdpListener::_udp_task_post(arg, pcb, this_pb, addr, port, ip_current_input_netif()))
            pbuf_free(this_pb);
    }
}
} // namespace

UdpPacketWrapper::UdpPacketWrapper(pbuf *pb, const ip_addr_t *raddr, uint16_t rport, struct netif *ntif)
{
    _pb = pb;
    _if = TCPIP_ADAPTER_IF_MAX;
    _data = (uint8_t*)(pb->payload);
    _len = pb->len;
    _index = 0;

    //memcpy(&_remoteIp, raddr, sizeof(ip_addr_t));
    _remoteIp.type = raddr->type;
    _localIp.type = _remoteIp.type;

    eth_hdr* eth = NULL;
    const udp_hdr* udphdr = reinterpret_cast<const udp_hdr*>(_data - UDP_HLEN);
    _localPort = ntohs(udphdr->dest);
    _remotePort = ntohs(udphdr->src);

    if (_remoteIp.type == IPADDR_TYPE_V4)
    {
        eth = (eth_hdr *)(((uint8_t *)(pb->payload)) - UDP_HLEN - IP_HLEN - SIZEOF_ETH_HDR);
        struct ip_hdr * iphdr = (struct ip_hdr *)(((uint8_t *)(pb->payload)) - UDP_HLEN - IP_HLEN);
        _localIp.u_addr.ip4.addr = iphdr->dest.addr;
        _remoteIp.u_addr.ip4.addr = iphdr->src.addr;
    }
    else
    {
        eth = (eth_hdr *)(((uint8_t *)(pb->payload)) - UDP_HLEN - IP6_HLEN - SIZEOF_ETH_HDR);
        struct ip6_hdr * ip6hdr = (struct ip6_hdr *)(((uint8_t *)(pb->payload)) - UDP_HLEN - IP6_HLEN);
        std::memcpy(&_localIp.u_addr.ip6.addr, (uint8_t *)ip6hdr->dest.addr, 16);
        std::memcpy(&_remoteIp.u_addr.ip6.addr, (uint8_t *)ip6hdr->src.addr, 16);
    }
    std::memcpy(_remoteMac, eth->src.addr, 6);

    struct netif *netif{NULL};
    void *nif{NULL};
    for (int i=0; i<TCPIP_ADAPTER_IF_MAX; i++)
    {
        tcpip_adapter_get_netif((tcpip_adapter_if_t)i, &nif);
        netif = (struct netif *)nif;
        if (netif && netif == ntif)
        {
            _if = (tcpip_adapter_if_t)i;
            break;
        }
    }
}

bool AsyncUdpListener::listen(const ip_addr_t *addr, uint16_t port)
{
    if (!_udp_queue.constructed())
    {
        _udp_queue.construct(UBaseType_t{32}, sizeof(lwip_event_packet_t *));
        if (!_udp_queue->handle)
        {
            _udp_queue.destruct();
            ESP_LOGE("AsyncUdpListener", "xQueueCreate failed");
            return false;
        }
    }

    if (!_init())
    {
        ESP_LOGE("AsyncUdpListener", "failed to init");
        return false;
    }

    close();

    if (addr)
    {
        IP_SET_TYPE_VAL(_pcb->local_ip,  addr->type);
        IP_SET_TYPE_VAL(_pcb->remote_ip, addr->type);
    }

    if (_udp_bind(_pcb, addr, port) != ERR_OK)
    {
        ESP_LOGE("AsyncUdpListener", "failed to bind");
        return false;
    }

    _connected = true;

    return true;
}

void AsyncUdpListener::poll(TickType_t xTicksToWait)
{
    if (!_udp_queue.constructed())
        return;

    lwip_event_packet_t *e{NULL};
    while (_udp_queue->receive(&e, 0) == pdTRUE)
    {
        if (!e->pb)
        {
            free((void*)(e));
            continue;
        }

        _recv(e->pcb, e->pb, e->addr, e->port, e->netif);

        free((void*)(e));
    }
}

bool AsyncUdpListener::_udp_task_post(void *arg, udp_pcb *pcb, pbuf *pb, const ip_addr_t *addr, uint16_t port, struct netif *netif)
{
    if (!arg)
        return false;

    auto &queue = reinterpret_cast<AsyncUdpListener*>(arg)->_udp_queue;
    if (!queue.constructed())
        return false;

    lwip_event_packet_t *e = (lwip_event_packet_t *)malloc(sizeof(lwip_event_packet_t));
    if (!e)
        return false;

    e->arg = arg;
    e->pcb = pcb;
    e->pb = pb;
    e->addr = addr;
    e->port = port;
    e->netif = netif;

    if (queue->send(&e, portMAX_DELAY) != pdPASS)
    {
        free((void*)(e));
        return false;
    }

    return true;
}

bool AsyncUdpListener::_init()
{
    if (_pcb)
        return true;

    _pcb = udp_new();
    if (!_pcb)
    {
        ESP_LOGE("AsyncUdpListener", "udp_new() failed");
        return false;
    }

    udp_recv(_pcb, &_udp_recv, (void *)this);

    return true;
}

void AsyncUdpListener::_recv(udp_pcb *upcb, pbuf *pb, const ip_addr_t *addr, uint16_t port, struct netif *netif)
{
    while (pb != NULL)
    {
        pbuf *this_pb = pb;
        pb = pb->next;
        this_pb->next = NULL;

        if (_handler)
            _handler(UdpPacketWrapper{this_pb, addr, port, netif});

        pbuf_free(this_pb);
    }
}

void AsyncUdpListener::close()
{
    if (_pcb)
    {
        if (_connected)
            _udp_disconnect(_pcb);

        _connected = false;
    }
}

} // namespace espcpputils
