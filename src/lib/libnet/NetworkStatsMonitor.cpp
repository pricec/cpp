#include <string.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/socket.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include "NetworkStatsMonitor.hpp"
#include "libnl/RtnlLinkMessage.hpp"

using namespace net;

NetworkStatsMonitor::NetworkStatsMonitor(
    buffer::BufferSegmentFactory    &bufFac,
    netlink::NetlinkServer          &nlServer,
    NetworkInterface                 interface,
    std::chrono::duration<uint64_t>  interval,
    uint16_t                         sampleSize
)
    : m_bufFac(bufFac)
    , m_nlServer(nlServer)
    , m_interface(interface)
    , m_stats(bufFac, sampleSize)
    , m_interval(interval)
    , m_sampleSize(sampleSize)
{}

NetworkStatsMonitor::~NetworkStatsMonitor()
{
    stop();
    waitStop();
}

netlink::NetlinkMessage NetworkStatsMonitor::makeNlRequest()
{
    struct {
        struct nlmsghdr hdr;
        struct rtgenmsg msg;
    } req;

    ::memset(&req, 0, sizeof(req));
    req.hdr.nlmsg_len   = NLMSG_LENGTH(sizeof(struct rtgenmsg));
    req.hdr.nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    req.hdr.nlmsg_type  = RTM_GETLINK;
    req.hdr.nlmsg_seq   = ++m_seqNum;
    req.hdr.nlmsg_pid   = 0;
    req.msg.rtgen_family = AF_UNSPEC;

    buffer::Buffer buf(m_bufFac.allocate(req.hdr.nlmsg_len));
    ::memcpy(
        buf.getDataAs<char>(m_bufFac, 0, req.hdr.nlmsg_len),
        &req.hdr,
        req.hdr.nlmsg_len
    );
    return RtnlLinkMessage(m_bufFac, buf);
}

void NetworkStatsMonitor::statsCallback(netlink::NetlinkMessage nlm)
{
    if (nlm.header()->nlmsg_type == NLMSG_DONE && !this->m_wantExit)
    {
        // TODO: defer waiting to a different thread.
        //       Right now this makes the nlServer unusable
        //       by anyone else.
        std::this_thread::sleep_for(m_interval);
        m_nlServer.send(
            NETLINK_ROUTE,
            RTM_NEWLINK,
            m_bufFac,
            makeNlRequest(),
            [&] (NetlinkMessage nlm) { this->statsCallback(nlm); }
        );
    }
    else if (nlm.header()->nlmsg_type != NLMSG_DONE)
    {
        RtnlLinkMessage linkmsg(m_bufFac, nlm);
        const struct ifinfomsg *info = linkmsg.msg();
        if (info->ifi_index == m_interface.index())
        {
            if (!m_stats.insert(linkmsg.stats()))
            {
                // TODO: error handling
                printf("Error inserting\n");
            }

            if (m_stats.full())
            {
                m_cb(m_stats);
                m_stats =
                    NetworkInterfaceStatsBuffer(
                        m_bufFac,
                        m_sampleSize
                    );
            }
        }
    }
    else
    {
        uint64_t val = 1;
        ::write(m_donefd, &val, sizeof(uint64_t));
    }    
}

bool NetworkStatsMonitor::start(
    std::function<void(NetworkInterfaceStatsBuffer)> cb
) {
    m_cb = cb;
    m_wantExit = false;
    return m_nlServer.send(
        NETLINK_ROUTE,
        RTM_NEWLINK,
        m_bufFac,
        makeNlRequest(),
        [&] (NetlinkMessage nlm) { this->statsCallback(nlm); }
    );
}

bool NetworkStatsMonitor::stop()
{
    m_donefd   = ::eventfd(0, 0);
    m_wantExit = true;
    return true;
}

bool NetworkStatsMonitor::waitStop()
{
    uint64_t val = 0;
    ssize_t n = ::read(m_donefd, &val, sizeof(uint64_t));
    ::close(m_donefd);
    return n == sizeof(uint64_t);
}
