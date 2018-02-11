#include "NetworkInterfaceStatsBuffer.hpp"
#include <string.h>

using namespace net;

NetworkInterfaceStatsBuffer::NetworkInterfaceStatsBuffer(
    buffer::BufferSegmentFactory &bufFac,
    uint16_t numEntries
)
    : m_bufFac(bufFac)
    , m_numEntries(numEntries)
    , m_offset(0)
    , m_buf(bufFac.allocate(sizeof(struct rtnl_link_stats) * numEntries))
{}

NetworkInterfaceStatsBuffer::~NetworkInterfaceStatsBuffer()
{}

const NetworkInterfaceStatsBuffer& NetworkInterfaceStatsBuffer::operator=(
    const NetworkInterfaceStatsBuffer &other
) {
    this->m_numEntries = other.m_numEntries;
    this->m_offset     = other.m_offset;
    this->m_buf        = other.m_buf;
    return *this;
}

bool NetworkInterfaceStatsBuffer::insert(const void *buf)
{
    if (m_offset >= m_numEntries) { return false; }

    ::memcpy(
        m_buf.getDataAs<char>(
            m_bufFac,
            m_offset * sizeof(struct rtnl_link_stats),
            sizeof(struct rtnl_link_stats)
        ),
        buf,
        sizeof(struct rtnl_link_stats)
    );

    m_offset++;
    return true;
}

const struct rtnl_link_stats* NetworkInterfaceStatsBuffer::operator[](
    size_t index
) {
    return
        m_buf.getDataAs<struct rtnl_link_stats>(
            m_bufFac,
            index,
            sizeof(struct rtnl_link_stats)
        );
}
