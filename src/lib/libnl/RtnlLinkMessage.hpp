#include "NetlinkMessage.hpp"
#include "libbuffer/Buffer.hpp"

#include <linux/rtnetlink.h>

#include <vector>

namespace netlink
{

class RtnlLinkMessage : public NetlinkMessage
{
public:
    RtnlLinkMessage(
        buffer::BufferSegmentFactory &bufFac,
        const buffer::Buffer &buf
    );
    RtnlLinkMessage(
        buffer::BufferSegmentFactory &bufFac,
        const NetlinkMessage &msg
    );
    RtnlLinkMessage(const RtnlLinkMessage &other);
    ~RtnlLinkMessage();

    RtnlLinkMessage& operator=(const RtnlLinkMessage &rhs);

    const struct ifinfomsg* ifinfomsg();
    const std::vector<const struct rtattr*>& rtattrs();

private:
    const struct ifinfomsg            *m_ifinfomsg;
    std::vector<const struct rtattr*>  m_attrs;
};

}
