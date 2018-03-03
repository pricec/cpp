#pragma once

#include <linux/netlink.h>

namespace buffer
{
class Buffer;
class BufferSegmentFactory;
}

namespace netlink
{

class NetlinkMessageHeader
{
public:
    NetlinkMessageHeader();
    NetlinkMessageHeader(
        buffer::BufferSegmentFactory &bufFac,
        const buffer::Buffer &buf
    );
    NetlinkMessageHeader(const NetlinkMessageHeader &other);
    NetlinkMessageHeader& operator=(const NetlinkMessageHeader &rhs);

    uint32_t nlmsg_len()   const;
    uint16_t nlmsg_type()  const;
    uint16_t nlmsg_flags() const;
    uint32_t nlmsg_seq()   const;
    uint32_t nlmsg_pid()   const;

    const struct nlmsghdr* ptr() const;

    template <typename T>
    const T* nlmsg_data() const
    {
        return (const T *)NLMSG_DATA(m_hdr);
    }

private:
    buffer::Buffer m_buf;
    struct nlmsghdr *m_hdr;
};

}
