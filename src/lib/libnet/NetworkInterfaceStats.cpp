#include "NetworkInterfaceStats.hpp"
#include <string.h>
#include <linux/rtnetlink.h>

using namespace net;

NetworkInterfaceStats::NetworkInterfaceStats()
{}

NetworkInterfaceStats::NetworkInterfaceStats(netlink::RtnlLinkMessage &msg)
    : m_stats(*msg.stats())
    , m_name(msg.name())
{}

NetworkInterfaceStats::NetworkInterfaceStats(
    const NetworkInterfaceStats &other
)
    : m_stats(other.m_stats)
    , m_name(other.m_name)
{}

NetworkInterfaceStats::~NetworkInterfaceStats()
{}

NetworkInterfaceStats& NetworkInterfaceStats::operator=(
    const NetworkInterfaceStats &rhs
) {
    this->m_stats = rhs.m_stats;
    return *this;
}

std::string NetworkInterfaceStats::str() const {
    std::string result;

    result.append("Interface stats for ");
    result.append(m_name);
    result.append("\n");

    result.append("\trx_packets: ");
    result.append(std::to_string(m_stats.rx_packets));
    result.append("\n");

    result.append("\n");
    return result;
}
