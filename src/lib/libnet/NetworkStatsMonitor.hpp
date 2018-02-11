#pragma once

#include "libbuffer/BufferSegmentFactory.hpp"
#include "libnl/NetlinkServer.hpp"
#include "NetworkInterface.hpp"
#include "NetworkInterfaceStatsBuffer.hpp"

#include <chrono>

namespace net
{

class NetworkStatsMonitor
{
public:
    NetworkStatsMonitor(
        buffer::BufferSegmentFactory    &bufFac,
        netlink::NetlinkServer          &nlServer,
        NetworkInterface                 interface,
        std::chrono::duration<uint64_t>  interval,
        uint16_t                         sampleSize
    );

    ~NetworkStatsMonitor();

    netlink::NetlinkMessage makeNlRequest();
    void statsCallback(netlink::NetlinkMessage nlm);
    bool start(std::function<void(NetworkInterfaceStatsBuffer)> cb);
    bool stop();
    bool waitStop();

private:
    buffer::BufferSegmentFactory                     &m_bufFac;
    netlink::NetlinkServer                           &m_nlServer;
    NetworkInterface                                  m_interface;
    NetworkInterfaceStatsBuffer                       m_stats;
    std::chrono::duration<uint64_t>                   m_interval;
    uint16_t                                          m_sampleSize;
    std::function<void(NetworkInterfaceStatsBuffer)>  m_cb;
    bool                                              m_wantExit;
    int                                               m_donefd;
    uint32_t                                          m_seqNum;
};

}
