using namespace netlink;

template <typename T>
RtnlMessage<T>::RtnlMessage(
    buffer::BufferSegmentFactory &bufFac,
    const buffer::Buffer &buf
)
    : NetlinkMessage(bufFac, buf)
    , m_msg(header().template nlmsg_data<T>())
{}

template <typename T>
RtnlMessage<T>::RtnlMessage(
    buffer::BufferSegmentFactory &bufFac,
    const NetlinkMessage &msg
)
    : NetlinkMessage(bufFac, msg.buffer())
    , m_msg(header().template nlmsg_data<T>())
{}

template <typename T>
RtnlMessage<T>::RtnlMessage(const RtnlMessage<T> &other)
    : NetlinkMessage(other)
    , m_msg(other.m_msg)
    , m_attrs(other.m_attrs)
{}

template <typename T>
RtnlMessage<T>::~RtnlMessage()
{}

template <typename T>
const T* RtnlMessage<T>::msg()
{
    return m_msg;
}

template <typename T>
const std::vector<const struct rtattr*>& RtnlMessage<T>::rtattrs()
{
    if (m_attrs.size() == 0)
    {
        size_t len = m_hdr.nlmsg_len() - NLMSG_LENGTH(sizeof(*m_msg));
        for ( const struct rtattr *rta = IFLA_RTA(m_msg)
            ; RTA_OK(rta, len)
            ; rta = RTA_NEXT(rta, len) )
        {
            m_attrs.emplace_back(rta);
        }
    }
    return m_attrs;
}
