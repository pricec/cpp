#pragma once

#include "libbuffer/BufferSegmentFactory.hpp"
#include "libcommon/WorkItem.hpp"
#include "libcommon/WorkQueue.hpp"
#include "libnl/NetlinkServer.hpp"
#include "NetworkInterface.hpp"
#include "NetworkInterfaceStatsBuffer.hpp"

#include <chrono>

namespace net
{

class NetworkStatsMonitor : public common::WorkItem
{
public:
    /**
     * Create a new statistics monitor. This will not actually
     * start monitoring the argument NetworkInterface until you
     * call start. The interface statistics will be collected
     * (roughly) every interval. Once sampleSize samples have
     * been collected, the callback (passed to start) will be
     * invoked.
     *
     * NOTE: The waiting (that is, between durations) will be
     *       deferred to the argument WorkQueue. Be careful
     *       not to exhaust the queue's threads with waiters.
     */
    NetworkStatsMonitor(
        common::WorkQueue               &wq,
        buffer::BufferSegmentFactory    &bufFac,
        netlink::NetlinkServer          &nlServer,
        NetworkInterface                 interface,
        std::chrono::duration<uint64_t>  interval,
        uint16_t                         sampleSize
    );
    ~NetworkStatsMonitor();

    /**
     * Start the monitor running. The argument callback will
     * be invoked according to the schedule documented above.
     * Returns true for success, false otherwise.
     */
    bool start(std::function<void(NetworkInterfaceStatsBuffer)> cb);

    /**
     * Stop the monitor. If this function returns true, you
     * can be sure that the monitor will stop eventually.
     */
    bool stop();

    /**
     * Wait for the monitor to stop. When this function returns
     * true, the monitor is stopped and the object can be
     * safely destroyed. Note that the destructor takes care
     * of stopping and waiting.
     */
    bool waitStop();

private:
    netlink::NetlinkMessage makeNlRequest();
    void statsCallback(netlink::NetlinkMessage nlm);

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
