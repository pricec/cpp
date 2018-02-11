#pragma once
#include "MacAddress.hpp"
#include "IPv4Address.hpp"
#include <string>

namespace net
{

class NetworkInterface
{
public:
    NetworkInterface(
        int         ifindex,
        std::string name,
        uint16_t    mtu,
        std::string qdisc,
        MacAddress  hwaddr,
        MacAddress  broadcastaddr,
        IPv4Address ip4addr,
        IPv4Address ip4broadcast
    );
    NetworkInterface(const NetworkInterface &other);
    NetworkInterface& operator=(const NetworkInterface &rhs);
    std::string str() const;
    int index() const { return m_ifindex; }
    const std::string& name() const { return m_name; }

private:
    int         m_ifindex;
    std::string m_name;
    uint16_t    m_mtu;
    std::string m_qdisc;
    MacAddress  m_hwaddr;
    MacAddress  m_broadcastaddr;
    IPv4Address m_ip4addr;
    IPv4Address m_ip4broadcast;
};

}
