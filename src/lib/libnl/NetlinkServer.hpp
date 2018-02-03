/** \brief Netlink Server class for sending/receiving netlink messages
 *
 * This class is meant to make interaction with the kernel
 * via netlink sockets more simple. It provides a high-level
 * abstraction of the netlink operations. It allows the caller
 * to send netlink messages and subscribe to specified families
 * and groups of messages. The implementation at this time
 * is completely asynchronous (it will create a single thread
 * of execution), and message passing is handled via a
 * callback interface.
 */
#pragma once
#include <functional>
#include <map>
#include "libcommon/WorkItem.hpp"

namespace common
{
class UUID;
}

namespace buffer
{
class BufferSegmentFactory;
}

namespace netlink
{

class NetlinkMessage;
class NetlinkSocket;

class NetlinkServer : public common::WorkItem
{
public:
    /**
     * Create a Netlink Server. Note that this will not
     * actually start the server running. The argument
     * buffer segment factory will be used to allocate
     * data segements for received netlink messages.
     */
    NetlinkServer(buffer::BufferSegmentFactory &bufFac);
    ~NetlinkServer();

    NetlinkServer(const NetlinkServer &other) = delete;
    NetlinkServer& operator=(const NetlinkServer &rhs) = delete;

    /**
     * Start the server running.
     */
    bool start();

    /**
     * Stop the server. At present, there are a few minor
     * race conditions possible between start and stop.
     * Fortunately most issues are handled by the fact
     * that we are using our own WorkQueue; the WorkQueue
     * will wait for the worker thread to return (which
     * ensures the necessary cleanup) before the destructor
     * exits.
     *
     * NOTE: be careful when making changes to accept
     *       a WorkQueue into the constructor; this may
     *       introduce race conditions.
     */
    bool stop();

    /**
     * Set the callback function that will be invoked for
     * every received message. At this time, more granular
     * callbacks are not supported.
     */
    void set_rx_cb(std::function<void(NetlinkMessage)> f);

    /**
     * Listen for netlink messages from the given family
     * and groups. Any messages originating from the kernel
     * with this signature will result in an invocation of
     * the rx callback.
     */
    void listen(int netlink_family, uint32_t groups);

    /**
     * Ignore netlink messages from the given family. This
     * will cause any sockets associated with the argument
     * family to be closed and the corresponding object to
     * be destroyed.
     */
    void ignore(int netlink_family);

private:
    void worker();

private:
    static const uint16_t               s_max_events = 10;
    static const uint32_t               s_buf_size   = 32768;
    bool                                m_running;
    buffer::BufferSegmentFactory       &m_bufFac;
    int                                 m_donefd;
    int                                 m_epollfd;
    std::map<int, NetlinkSocket>        m_sockets; // netlink_family -> sockfd
    std::function<void(NetlinkMessage)> m_rx_cb;
};

}
