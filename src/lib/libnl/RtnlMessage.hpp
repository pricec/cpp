#include "NetlinkMessage.hpp"
#include "libbuffer/Buffer.hpp"

#include <linux/rtnetlink.h>
#include <vector>

namespace netlink
{

template <typename T>
class RtnlMessage : public NetlinkMessage
{
public:
    RtnlMessage(
        buffer::BufferSegmentFactory &bufFac,
        const buffer::Buffer &buf
    );

    RtnlMessage(
        buffer::BufferSegmentFactory &bufFac,
        const NetlinkMessage &msg
    );

    /*
    RtnlMessage(
        buffer::BufferSegmentFactory &bufFac,
        const struct nlmsghdr *hdr,
        const void *msg        
    );
    */

    RtnlMessage(const RtnlMessage<T> &other);

    ~RtnlMessage();

    RtnlMessage<T>& operator=(const RtnlMessage &rhs);

    const T* msg();
    const std::vector<const struct rtattr*>& rtattrs();

private:
    const T                           *m_msg;
    std::vector<const struct rtattr*>  m_attrs;
};

using RtnlLinkMessage    = RtnlMessage<struct ifinfomsg>;
using RtnlAddrMessage    = RtnlMessage<struct ifaddrmsg>;
using RtnlRouteMessage   = RtnlMessage<struct rtmsg>;
using RtnlNeighMessage   = RtnlMessage<struct ndmsg>;
using RtnlRuleMessage    = RtnlMessage<struct rtmsg>;
using RtnlQdiscMessage   = RtnlMessage<struct tcmsg>;
using RtnlTClassMessage  = RtnlMessage<struct tcmsg>;
using RtnlTFilterMessage = RtnlMessage<struct tcmsg>;

}

#include "RtnlMessage.cpp"
