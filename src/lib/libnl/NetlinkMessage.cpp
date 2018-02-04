#include "NetlinkMessage.hpp"
#include <stdlib.h>
#include <string.h>

using namespace netlink;

NetlinkMessage::NetlinkMessage(const buffer::Buffer &buf) {
    m_msg = buf;
}

NetlinkMessage::NetlinkMessage(const NetlinkMessage &other)
{
    *this = other;
}

NetlinkMessage::~NetlinkMessage()
{}

NetlinkMessage& NetlinkMessage::operator=(const NetlinkMessage &rhs)
{
    this->m_msg = rhs.m_msg;
    return *this;
}

const struct nlmsghdr* NetlinkMessage::header(
    buffer::BufferSegmentFactory &bufFac
) {
    return m_msg.getDataAs<struct nlmsghdr>(bufFac, 0, m_msg.length());
}
