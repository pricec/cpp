/** \brief Netlink Socket class
 *
 * This class is meant to abstract a netlink socket in
 * a friendly way for use by the netlink server. It
 * employs an RAII pattern to make managing sockets as
 * simple as possible.
 */
#pragma once
#include <stdint.h>
#include <linux/netlink.h>
#include <sys/socket.h>

namespace netlink
{

class NetlinkSocket
{
public:
    NetlinkSocket(int netlink_family);
    NetlinkSocket(const NetlinkSocket &other);
    ~NetlinkSocket();
    NetlinkSocket& operator=(const NetlinkSocket &other);

    int fd() const { return m_sockfd; }
    bool write(const void *buf, size_t buf_len);
    bool listen(uint32_t groups);

private:
    int m_sockfd;
};

}
