#include "RtnlLinkMessage.hpp"

using namespace netlink;

RtnlLinkMessage::RtnlLinkMessage(
    buffer::BufferSegmentFactory &bufFac,
    const buffer::Buffer &buf
)
    : RtnlMessage<ifinfomsg>(bufFac, buf)
{
    for (auto &attr : rtattrs())
    {
        switch (attr->rta_type)
        {
        case IFLA_STATS:
            m_stats = (const rtnl_link_stats *)RTA_DATA(attr);
            break;
        case IFLA_IFNAME:
            m_name = std::string((char *)RTA_DATA(attr));
            break;
        }
    }
}

RtnlLinkMessage::RtnlLinkMessage(
    buffer::BufferSegmentFactory &bufFac,
    const NetlinkMessage &msg
)
    : RtnlMessage<ifinfomsg>(bufFac, msg.buffer())
{
    for (auto &attr : rtattrs())
    {
        switch (attr->rta_type)
        {
        case IFLA_STATS:
            m_stats = (const rtnl_link_stats *)RTA_DATA(attr);
            break;
        case IFLA_IFNAME:
            m_name = std::string((char *)RTA_DATA(attr));
            break;
        }
    }
}

RtnlLinkMessage::RtnlLinkMessage(const RtnlLinkMessage &other)
    : RtnlMessage<ifinfomsg>(other)
    , m_name(other.m_name)
    , m_stats(other.m_stats)
{
    *this = other;
}

RtnlLinkMessage::~RtnlLinkMessage()
{}
