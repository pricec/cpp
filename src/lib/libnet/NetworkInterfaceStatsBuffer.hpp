#pragma once

#include "libbuffer/Buffer.hpp"
#include "libbuffer/BufferSegmentFactory.hpp"

#include <linux/if_link.h>

namespace net
{

class NetworkInterfaceStatsBuffer
{
public:
    NetworkInterfaceStatsBuffer(
        buffer::BufferSegmentFactory &bufFac,
        uint16_t numEntries
    );
    ~NetworkInterfaceStatsBuffer();

    const NetworkInterfaceStatsBuffer& operator=(
        const NetworkInterfaceStatsBuffer &other
    );

    size_t size() const { return m_numEntries; }
    bool full() const { return m_numEntries <= m_offset; }

    bool insert(const void *buf);

    const struct rtnl_link_stats* operator[](size_t index);

private:
    buffer::BufferSegmentFactory &m_bufFac;
    uint16_t                      m_numEntries;
    uint16_t                      m_offset;
    buffer::Buffer                m_buf;
};

}
