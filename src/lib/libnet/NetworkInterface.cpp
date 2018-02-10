#include "NetworkInterface.hpp"

using namespace net;

NetworkInterface::NetworkInterface(
    int         ifindex,
    std::string name,
    uint16_t    mtu,
    std::string qdisc,
    MacAddress  hwaddr,
    MacAddress  broadcastaddr,
    IPv4Address ip4addr,
    IPv4Address ip4broadcast
)
    : m_ifindex(ifindex)
    , m_name(name)
    , m_mtu(mtu)
    , m_qdisc(qdisc)
    , m_hwaddr(hwaddr)
    , m_broadcastaddr(broadcastaddr)
    , m_ip4addr(ip4addr)
    , m_ip4broadcast(ip4broadcast)
{}

NetworkInterface::NetworkInterface(const NetworkInterface &other)
    : m_ifindex(other.m_ifindex)
    , m_name(other.m_name)
    , m_mtu(other.m_mtu)
    , m_qdisc(other.m_qdisc)
    , m_hwaddr(other.m_hwaddr)
    , m_broadcastaddr(other.m_broadcastaddr)
    , m_ip4addr(other.m_ip4addr)
    , m_ip4broadcast(other.m_ip4broadcast)
{}

NetworkInterface& NetworkInterface::operator=(const NetworkInterface &rhs)
{
    this->m_ifindex       = rhs.m_ifindex;
    this->m_name          = rhs.m_name;
    this->m_mtu           = rhs.m_mtu;
    this->m_qdisc         = rhs.m_qdisc;
    this->m_hwaddr        = rhs.m_hwaddr;
    this->m_broadcastaddr = rhs.m_broadcastaddr;
    this->m_ip4addr       = rhs.m_ip4addr;
    this->m_ip4broadcast  = rhs.m_ip4broadcast;

    return *this;
}

std::string NetworkInterface::str() const
{
    std::string result("Network Interface:\n");

    result.append("\tName: ");
    result.append(m_name);
    result.append("\n");

    result.append("\tQueuing discipline: ");
    result.append(m_qdisc);
    result.append("\n");

    result.append("\tMTU: ");
    result.append(std::to_string(m_mtu));
    result.append("\n");

    result.append("\tMAC Address: ");
    result.append(m_hwaddr.str());
    result.append("\n");

    result.append("\tHW broadcast address: ");
    result.append(m_broadcastaddr.str());
    result.append("\n");

    result.append("\tIPv4 Address: ");
    result.append(m_ip4addr.str());
    result.append("\n");

    result.append("\tIPv4 Broadcast Address: ");
    result.append(m_ip4broadcast.str());
    result.append("\n");

    return result;
}
