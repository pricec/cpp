#pragma once

#include "RtnlMessage.hpp"

namespace netlink
{

class RtnlLinkMessage : public RtnlMessage<ifinfomsg>
{
public:
    RtnlLinkMessage(
        buffer::BufferSegmentFactory &bufFac,
        const buffer::Buffer &buf
    );

    RtnlLinkMessage(
        buffer::BufferSegmentFactory &bufFac,
        const NetlinkMessage &msg
    );

    RtnlLinkMessage(const RtnlLinkMessage &other);

    ~RtnlLinkMessage();

    const std::string& name() const { return m_name; }
    const struct rtnl_link_stats* stats() const { return m_stats; }

private:
    std::string                   m_name;
    const struct rtnl_link_stats *m_stats;
};

}
