#include "NetlinkMessage.hpp"
#include <stdlib.h>
#include <string.h>

using namespace netlink;

NetlinkMessage::NetlinkMessage(
    buffer::BufferSegmentFactory &bufFac,
    const buffer::Buffer &buf
)
    : m_msg(buf)
    , m_hdr(bufFac, buf)
{
    m_msg.flatten(bufFac);
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
    this->m_hdr = rhs.m_hdr;
    return *this;
}

const NetlinkMessageHeader& NetlinkMessage::header() const
{
    return m_hdr;
}
