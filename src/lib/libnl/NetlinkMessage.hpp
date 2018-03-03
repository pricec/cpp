/** \brief NetlinkMessage class (see man 7 netlink)
 *
 * This class is meant to represent a NetlinkMessage,
 * either incoming from a netlink socket, or intended
 * to be transmitted to the kernel via a netlink socket.
 */
#pragma once
#include "libbuffer/Buffer.hpp"
#include "libbuffer/BufferSegmentFactory.hpp"
#include "NetlinkMessageHeader.hpp"
#include <linux/netlink.h>
#include <stdint.h>

namespace netlink
{

class NetlinkMessage
{
public:
    /**
     * Create a netlink message from a buffer. It is expected
     * that the buffer consists of the header plus the body
     * of the message. It is most advisable to read off a netlink
     * socket into a buffer allocated using a the buffer
     * segment factory provided by libbuffer.
     */
    NetlinkMessage(
        buffer::BufferSegmentFactory &bufFac,
        const buffer::Buffer &buf
    );
    NetlinkMessage(const NetlinkMessage &other);
    virtual ~NetlinkMessage();

    NetlinkMessage& operator=(const NetlinkMessage &rhs);

    /**
     * Get a reference to the objective representation
     * of the nlmsghdr struct
     */
    const NetlinkMessageHeader& header() const;

    buffer::Buffer buffer() const { return m_msg; }

    uint32_t length() const { return header().nlmsg_len(); }

protected:
    buffer::Buffer       m_msg;
    NetlinkMessageHeader m_hdr;
};

}
