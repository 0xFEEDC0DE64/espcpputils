// I dont know why but without this hack here the linker
// fails for asio and I could not fix it otherwise

extern "C" char *lwip_if_indextoname(unsigned int ifindex, char *ifname);
extern "C" unsigned int lwip_if_nametoindex(const char *ifname);

extern "C" char *if_indextoname(unsigned int ifindex, char *ifname)
{
    return lwip_if_indextoname(ifindex, ifname);
}

extern "C" unsigned int if_nametoindex(const char *ifname)
{
    return lwip_if_nametoindex(ifname);
}
