using namespace netlink;

template <typename T>
RtnlMessage<T>::RtnlMessage(
    buffer::BufferSegmentFactory &bufFac,
    const buffer::Buffer &buf
)
    : NetlinkMessage(bufFac, buf)
    , m_msg((const T*)NLMSG_DATA(header()))
{}

template <typename T>
RtnlMessage<T>::RtnlMessage(
    buffer::BufferSegmentFactory &bufFac,
    const NetlinkMessage &msg
)
    : NetlinkMessage(bufFac, msg.buffer())
    , m_msg((const T*)NLMSG_DATA(header()))
{}

template <typename T>
RtnlMessage<T>::RtnlMessage(const RtnlMessage<T> &other)
    : NetlinkMessage(other)
{
    *this = other;
}

template <typename T>
RtnlMessage<T>::~RtnlMessage()
{}

template <typename T>
RtnlMessage<T>& RtnlMessage<T>::operator=(const RtnlMessage<T> &rhs)
{
    this->m_msg   = rhs.m_msg;
    this->m_attrs = rhs.m_attrs;
    return *this;
}

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
        size_t len = m_hdr->nlmsg_len - NLMSG_LENGTH(sizeof(*m_msg));
        for (
            const struct rtattr *rta = IFLA_RTA(m_msg);
            RTA_OK(rta, len);
            rta = RTA_NEXT(rta, len)
        ) {
            m_attrs.emplace_back(rta);
        }
    }
    return m_attrs;
}
