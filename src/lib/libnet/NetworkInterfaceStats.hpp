#pragma once

#include "libbuffer/BufferSegmentFactory.hpp"
#include "libnl/RtnlMessage.hpp"
#include "libnl/RtnlLinkMessage.hpp"

#include <linux/if_link.h>

namespace net
{

class NetworkInterfaceStats
{
public:
    NetworkInterfaceStats();
    NetworkInterfaceStats(netlink::RtnlLinkMessage &msg);
    NetworkInterfaceStats(const NetworkInterfaceStats &other);
    ~NetworkInterfaceStats();
    NetworkInterfaceStats& operator=(const NetworkInterfaceStats &rhs);
    std::string str() const;

private:
    struct rtnl_link_stats m_stats;
    std::string            m_name;
};

}
