#pragma once
#include "NetlinkMessage.hpp"
#include "NetlinkMessageHeader.hpp"
#include "libbuffer/Buffer.hpp"

#include <linux/rtnetlink.h>
#include <vector>

namespace netlink
{

template <typename T>
class RtnlMessage : public NetlinkMessage
{
public:
    RtnlMessage(
        buffer::BufferSegmentFactory &bufFac,
        const buffer::Buffer &buf
    );

    RtnlMessage(
        buffer::BufferSegmentFactory &bufFac,
        const NetlinkMessage &msg
    );

    RtnlMessage(const RtnlMessage<T> &other);

    virtual ~RtnlMessage();

    const T* msg();
    const std::vector<const struct rtattr*>& rtattrs();

protected:
    const T                           *m_msg;
    std::vector<const struct rtattr*>  m_attrs;
};

using RtnlAddrMessage    = RtnlMessage<struct ifaddrmsg>;
using RtnlRouteMessage   = RtnlMessage<struct rtmsg>;
using RtnlNeighMessage   = RtnlMessage<struct ndmsg>;
using RtnlRuleMessage    = RtnlMessage<struct rtmsg>;
using RtnlQdiscMessage   = RtnlMessage<struct tcmsg>;
using RtnlTClassMessage  = RtnlMessage<struct tcmsg>;
using RtnlTFilterMessage = RtnlMessage<struct tcmsg>;

}

#include "RtnlMessage.cpp"
