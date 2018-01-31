/** \brief NetlinkMessage class (see man 7 netlink)
 *
 * This class is meant to represent a NetlinkMessage,
 * either incoming from a netlink socket, or intended
 * to be transmitted to the kernel via a netlink socket.
 */
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

private:
    buffer::Buffer m_hdr;
    buffer::Buffer m_body;
};

}
