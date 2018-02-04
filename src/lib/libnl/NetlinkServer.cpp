#include "libbuffer/BufferSegmentFactory.hpp"
#include "NetlinkServer.hpp"
#include "NetlinkMessage.hpp"
#include "NetlinkSocket.hpp"
#include <string.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

using namespace netlink;

NetlinkServer::NetlinkServer(buffer::BufferSegmentFactory &bufFac)
    : common::WorkItem(1)
    , m_running(false)
    , m_bufFac(bufFac)
    , m_donefd(0)
    , m_epollfd(0)
    , m_sockets()
    , m_rx_cb([] (NetlinkMessage msg) {})
{}

NetlinkServer::~NetlinkServer()
{
    if (m_running)
    {
        stop();
    }
}

bool NetlinkServer::start()
{
    if (m_running)
    {
        return true;
    }

    m_donefd  = ::eventfd(0, 0);
    if (m_donefd < 1)
    {
        return false;
    }

    m_epollfd = ::epoll_create1(0);
    if (m_epollfd < 1)
    {
        ::close(m_donefd);
        m_donefd = 0;
        return false;
    }

    m_running = true;

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = m_donefd;

    epoll_ctl(m_epollfd, EPOLL_CTL_ADD, m_donefd, &ev);

    defer([this] () { this->worker(); });
    return true;
}

bool NetlinkServer::stop()
{
    if (m_running)
    {
        uint64_t val = 1;
        ::write(m_donefd, &val, sizeof(uint64_t));
        m_running = false;
    }
    return true;
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
        int nfds = epoll_wait(m_epollfd, events, s_max_events, -1);
        if (nfds == -1)
        {
            // TODO: log this?
            goto cleanup;
        }

        for (int i = 0; i < nfds; ++i)
        {
            if (events[i].data.fd == m_donefd)
            {
                goto cleanup;
            }

            struct iovec iov = { recv_buf, s_buf_size };
            struct sockaddr_nl sa;
            struct msghdr msg;
            struct nlmsghdr *nh;

            msg = { &sa, sizeof(sa), &iov, 1, NULL, 0, 0 };
            ssize_t n = ::recvmsg(events[i].data.fd, &msg, 0);

            for (
                nh = (struct nlmsghdr *)recv_buf;
                NLMSG_OK(nh, n);
                nh = NLMSG_NEXT(nh, n)
            ) {
                if (nh->nlmsg_type == NLMSG_DONE)
                {
                    break;
                }

                if (nh->nlmsg_type == NLMSG_ERROR)
                {
                    //TODO: error handling
                    continue;
                }

                auto buf(m_bufFac.allocate(nh->nlmsg_len));
                memcpy(buf->ptr<void>(), nh, nh->nlmsg_len);
                m_rx_cb(NetlinkMessage(buf));
            }
        }
    }

cleanup:
    if (m_donefd > 0)  { ::close(m_donefd);  m_donefd = 0;  }
    if (m_epollfd > 0) { ::close(m_epollfd); m_epollfd = 0; }
}
