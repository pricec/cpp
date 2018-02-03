/** \brief NetlinkMessage class (see man 7 netlink)
 *
 * This class is meant to represent a NetlinkMessage,
 * either incoming from a netlink socket, or intended
 * to be transmitted to the kernel via a netlink socket.
 */
#pragma once
#include "libbuffer/Buffer.hpp"
#include "libbuffer/BufferSegmentFactory.hpp"
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
    NetlinkMessage(const buffer::Buffer &buf);
    NetlinkMessage(const NetlinkMessage &other);
    ~NetlinkMessage();

    NetlinkMessage& operator=(const NetlinkMessage &rhs);

    /**
     * Get a pointer to the const header, casted into a
     * nlmsghdr for easy use.
     */
    const struct nlmsghdr* header(buffer::BufferSegmentFactory &bufFac);

    /**
     * Get the body (at the specified offset) as
     * a const pointer to the template type.
     */
    template <typename T>
    const T* bodyAs(buffer::BufferSegmentFactory &bufFac, size_t offset)
    {
        return m_body.getDataAs<T>(bufFac, offset, sizeof(T));
    }

private:
    buffer::Buffer m_hdr;
    buffer::Buffer m_body;
};

}