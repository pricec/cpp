#include "NetlinkSocket.hpp"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

using namespace netlink;

NetlinkSocket::NetlinkSocket()
{}

NetlinkSocket::NetlinkSocket(const NetlinkSocket &other)
{
    this->m_sockfd = other.m_sockfd;
}

NetlinkSocket::~NetlinkSocket()
{
    if (m_sockfd > 0)
    {
        ::close(m_sockfd);
    }
}

NetlinkSocket& NetlinkSocket::operator=(const NetlinkSocket &other)
{
    this->m_sockfd = other.m_sockfd;
    return *this;
}

bool NetlinkSocket::allocate(int netlink_family)
{
    m_sockfd = ::socket(AF_NETLINK, SOCK_DGRAM, netlink_family);
    return m_sockfd > -1;
}

bool NetlinkSocket::write(const void *buf, size_t buf_len)
{
    if (m_sockfd > 0)
    {
        return buf_len == size_t(::write(m_sockfd, buf, buf_len));
    }
    return false;
}

bool NetlinkSocket::listen(uint32_t groups)
{
    if (m_sockfd > 0)
    {
        struct sockaddr_nl sa;

        ::memset(&sa, 0, sizeof(sa));
        sa.nl_family = AF_NETLINK;
        sa.nl_groups = groups;

        return ::bind(m_sockfd, (struct sockaddr *)&sa, sizeof(sa)) != -1;
    }
    return false;
}
