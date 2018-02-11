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
    , m_callbacks()
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
        m_epollfd = 0;
        m_donefd = 0;
        return false;
    }

    m_running = true;

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = m_donefd;

    ::epoll_ctl(m_epollfd, EPOLL_CTL_ADD, m_donefd, &ev);

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
        m_sockets.clear();
        m_callbacks.clear();
    }
    return true;
}

bool NetlinkServer::listen(
    common::UUID &uuid,
    int netlink_family,
    uint32_t groups,
    std::function<void(NetlinkMessage)> rx_cb
) {
    auto s = m_sockets.emplace(common::UUID(), netlink_family).first;
    NetlinkSocket &sock = s->second;

    if (!sock.listen(groups))
    {
        m_sockets.erase(s->first);
        return false;
    }

    uuid = s->first;
    m_uuids.emplace(sock.fd(), uuid);
    m_callbacks.emplace(uuid, rx_cb);

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = sock.fd();

    ::epoll_ctl(m_epollfd, EPOLL_CTL_ADD, sock.fd(), &ev);

    return true;
}

bool NetlinkServer::send(
    int netlink_family,
    uint32_t group,
    buffer::BufferSegmentFactory &bufFac,
    NetlinkMessage msg,
    std::function<void(NetlinkMessage)> rx_cb
) {
    common::UUID uuid;
    if (listen(
            uuid,
            netlink_family,
            group,
            [uuid, netlink_family, group, rx_cb, this] (NetlinkMessage nlm)
            {
                rx_cb(nlm);
                if (nlm.header()->nlmsg_type == NLMSG_DONE)
                {
                    ignore(uuid);
                }
            }
        )
    ) {
        auto it = m_sockets.find(uuid);
        if (it != m_sockets.end())
        {
            // TODO: iovec send
            msg.buffer().flatten(bufFac);

            NetlinkSocket &sock(it->second);
            return sock.write(
                msg.buffer().getDataAs<char>(
                    bufFac,
                    0,
                    msg.length()
                ),
                msg.length()
            );
        }
    }
    ignore(uuid);
    return false;
}

void NetlinkServer::ignore(common::UUID uuid)
{
    auto it = m_sockets.find(uuid);
    if (it != m_sockets.end())
    {
        const NetlinkSocket &sock(it->second);

        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = sock.fd();

        ::epoll_ctl(m_epollfd, EPOLL_CTL_DEL, sock.fd(), &ev);

        m_uuids.erase(sock.fd());
        m_callbacks.erase(uuid);
        m_sockets.erase(uuid);
    }
}

void NetlinkServer::worker()
{
    struct epoll_event events[s_max_events];
    char recv_buf[s_buf_size];

    while (true)
    {
        int nfds = ::epoll_wait(m_epollfd, events, s_max_events, -1);
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
                if (nh->nlmsg_type == NLMSG_ERROR)
                {
                    //TODO: error handling
                    continue;
                }

                auto buf(m_bufFac.allocate(nh->nlmsg_len));
                ::memcpy(buf->ptr<void>(), nh, nh->nlmsg_len);

                auto uuid_it = m_uuids.find(events[i].data.fd);
                if (uuid_it != m_uuids.end())
                {
                    auto it = m_callbacks.find(uuid_it->second);
                    if (it != m_callbacks.end())
                    {
                        it->second(NetlinkMessage(m_bufFac, buf));
                    }
                }
            }
        }
    }

cleanup:
    if (m_donefd > 0)  { ::close(m_donefd);  m_donefd = 0;  }
    if (m_epollfd > 0) { ::close(m_epollfd); m_epollfd = 0; }
}
