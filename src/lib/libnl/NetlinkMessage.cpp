#include "NetlinkMessage.hpp"
#include <stdlib.h>
#include <string.h>

using namespace netlink;

NetlinkMessage::NetlinkMessage(const buffer::Buffer &buf) {
    m_hdr = buf.getData(0, sizeof(struct nlmsghdr));
    m_body = buf.getData(sizeof(struct nlmsghdr), buf.length());
}

NetlinkMessage::NetlinkMessage(const NetlinkMessage &other)
{
    *this = other;
}

NetlinkMessage::~NetlinkMessage()
{}

NetlinkMessage& NetlinkMessage::operator=(const NetlinkMessage &rhs)
{
    this->m_hdr  = rhs.m_hdr;
    this->m_body = rhs.m_body;
    return *this;
}

const struct nlmsghdr* NetlinkMessage::header(
    buffer::BufferSegmentFactory &bufFac
) {
    return m_hdr.getDataAs<struct nlmsghdr>(bufFac, 0, m_hdr.length());
}
