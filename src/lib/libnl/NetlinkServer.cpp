#include "libbuffer/BufferSegmentFactory.hpp"
#include "NetlinkServer.hpp"
#include "NetlinkMessage.hpp"
#include "NetlinkSocket.hpp"
#include <string.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include <stdio.h>

using namespace netlink;

NetlinkServer::NetlinkServer(buffer::BufferSegmentFactory &bufFac)
    : common::WorkItem(1)
    , m_bufFac(bufFac)
{
    // TODO: error checking (rewrite this library)
    m_donefd  = ::eventfd(0, 0);
    m_epollfd = ::epoll_create1(0);

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = m_donefd;

    epoll_ctl(m_epollfd, EPOLL_CTL_ADD, m_donefd, &ev);

    m_rx_cb = [] (NetlinkMessage msg) {};
}

NetlinkServer::~NetlinkServer()
{
    if (m_donefd > 0)
    {
        ::write(m_donefd, 0, sizeof(int));
        ::close(m_donefd);
    }
    if (m_epollfd > 0)
    {
        ::close(m_epollfd);
    }
}

void NetlinkServer::start()
{
    defer([this] () { this->worker(); });
}

void NetlinkServer::set_rx_cb(std::function<void(NetlinkMessage)> f)
{
    m_rx_cb = f;
}

void NetlinkServer::listen(int netlink_family, uint32_t groups)
{
    if (m_sockets.count(netlink_family) == 0)
    {
        auto s = m_sockets.emplace(netlink_family, netlink_family).first;
        const NetlinkSocket &sock = s->second;

        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = sock.fd();

        ::epoll_ctl(m_epollfd, EPOLL_CTL_ADD, sock.fd(), &ev);
    }

    auto it = m_sockets.find(netlink_family);
    if (it != m_sockets.end())
    {
        it->second.listen(groups);
    }
}

void NetlinkServer::ignore(int netlink_family)
{
    auto it = m_sockets.find(netlink_family);
    if (it != m_sockets.end())
    {
        const NetlinkSocket &sock(it->second);

        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = sock.fd();

        ::epoll_ctl(m_epollfd, EPOLL_CTL_DEL, sock.fd(), &ev);
        m_sockets.erase(netlink_family);
    }
}

void NetlinkServer::worker()
{
    struct epoll_event events[s_max_events];
    char recv_buf[s_buf_size];

    while (true)
    {
        printf("waiting for epoll\n");
        int nfds = epoll_wait(m_epollfd, events, s_max_events, -1);
        printf("got epoll notification\n");
        if (nfds == -1)
        {
            // TODO: log this?
            return;
        }

        for (int i = 0; i < nfds; ++i)
        {
            if (events[i].data.fd == m_donefd)
            {
                return;
            }

            ssize_t n = ::read(events[i].data.fd, recv_buf, s_buf_size);
            auto buf(m_bufFac.allocate(n));
            memcpy(buf->ptr<void>(), recv_buf, n);
            m_rx_cb(NetlinkMessage(buf));
        }
    }
}
