#include "libbuffer/Buffer.hpp"
#include "NetlinkMessageHeader.hpp"

using namespace netlink;

NetlinkMessageHeader::NetlinkMessageHeader()
{}

NetlinkMessageHeader::NetlinkMessageHeader(
    buffer::BufferSegmentFactory &bufFac,
    const buffer::Buffer &buf
)
    : m_buf(buf.getData(0, sizeof(struct nlmsghdr)))
    , m_hdr(m_buf.getDataAs<struct nlmsghdr>(
                bufFac,
                0,
                sizeof(struct nlmsghdr)
           ))
{}

NetlinkMessageHeader::NetlinkMessageHeader(const NetlinkMessageHeader &other)
{
    *this = other;
}

NetlinkMessageHeader& NetlinkMessageHeader::operator=(
    const NetlinkMessageHeader &rhs
) {
    this->m_buf = rhs.m_buf;
    this->m_hdr = rhs.m_hdr;
    return *this;
}

uint32_t NetlinkMessageHeader::nlmsg_len() const
{
    return m_hdr->nlmsg_len;
}

uint16_t NetlinkMessageHeader::nlmsg_type() const
{
    return m_hdr->nlmsg_type;
}

uint16_t NetlinkMessageHeader::nlmsg_flags() const
{
    return m_hdr->nlmsg_flags;
}

uint32_t NetlinkMessageHeader::nlmsg_seq() const
{
    return m_hdr->nlmsg_seq;
}

uint32_t NetlinkMessageHeader::nlmsg_pid() const
{
    return m_hdr->nlmsg_pid;
}

const struct nlmsghdr* NetlinkMessageHeader::ptr() const
{
    return m_hdr;
}
