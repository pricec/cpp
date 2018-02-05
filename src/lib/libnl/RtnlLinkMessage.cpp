#include "RtnlLinkMessage.hpp"

using namespace netlink;

RtnlLinkMessage::RtnlLinkMessage(
    buffer::BufferSegmentFactory &bufFac,
    const buffer::Buffer &buf
)
    : NetlinkMessage(bufFac, buf)
    , m_ifinfomsg((const struct ifinfomsg *)NLMSG_DATA(header()))
{}

RtnlLinkMessage::RtnlLinkMessage(
    buffer::BufferSegmentFactory &bufFac,
    const NetlinkMessage &msg
)
    : NetlinkMessage(bufFac, msg.buffer())
    , m_ifinfomsg((const struct ifinfomsg *)NLMSG_DATA(header()))
{}

RtnlLinkMessage::RtnlLinkMessage(const RtnlLinkMessage &other)
    : NetlinkMessage(other)
{
    *this = other;
}

RtnlLinkMessage::~RtnlLinkMessage()
{}

RtnlLinkMessage& RtnlLinkMessage::operator=(const RtnlLinkMessage &rhs)
{
    this->m_ifinfomsg = rhs.m_ifinfomsg;
    this->m_attrs     = rhs.m_attrs;
    return *this;
}

const struct ifinfomsg* RtnlLinkMessage::ifinfomsg()
{
    return m_ifinfomsg;
}

const std::vector<const struct rtattr*>& RtnlLinkMessage::rtattrs()
{
    if (m_attrs.size() == 0)
    {
        size_t len = m_hdr->nlmsg_len - NLMSG_LENGTH(sizeof(*m_ifinfomsg));
        for (
            const struct rtattr *rta = IFLA_RTA(m_ifinfomsg);
            RTA_OK(rta, len);
            rta = RTA_NEXT(rta, len)
            ) {
            m_attrs.emplace_back(rta);
        }
    }
    return m_attrs;
}
