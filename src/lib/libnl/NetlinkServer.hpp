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
#include "libcommon/UUID.hpp"

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
     * Listen for netlink messages from the given family
     * and groups. Any messages originating from the kernel
     * with this signature will result in an invocation of
     * the rx callback.
     *
     * NOTE: You MUST call this function after calling
     *       start. Whenever you call stop, all listeners
     *       are lost.
     */
    bool listen(
        common::UUID &uuid,
        int netlink_family,
        uint32_t groups,
        std::function<void(NetlinkMessage)> rx_cb
    );

    /**
     * Send the argument netlink message. This message should
     * be of the argument family and group. The callback will
     * be invoked once for each netlink message received in
     * response, including the terminating NLMSG_DONE message.
     * The function will handle allocating and deallocating a
     * netlink socket. If a problem occurs during setup, false
     * will be returned.
     */
    bool send(
        int netlink_family,
        uint32_t groups,
        buffer::BufferSegmentFactory &bufFac,
        NetlinkMessage msg,
        std::function<void(NetlinkMessage)> rx_cb
    );

    /**
     * Ignore netlink messages from the given family. This
     * will cause any sockets associated with the argument
     * family to be closed and the corresponding object to
     * be destroyed.
     */
    void ignore(common::UUID uuid);

private:
    void worker();

private:
    static const uint16_t                 s_max_events = 10;
    static const uint32_t                 s_buf_size   = 32768;
    bool                                  m_running;
    buffer::BufferSegmentFactory         &m_bufFac;
    int                                   m_donefd;
    int                                   m_epollfd;
    std::map<int, common::UUID>           m_uuids;  // socket fd -> UUID
    std::map<common::UUID, NetlinkSocket> m_sockets;
    std::map<common::UUID, std::function<void(NetlinkMessage)> > m_callbacks;
};

}
