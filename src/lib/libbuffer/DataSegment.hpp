/** \brief Buffer Buffer
 *
 *  This class wraps a void* to hold message data. It
 *  uses the passed in IBufferSegmentAllocationStrategy
 *  in order to allocate memory at construction time and
 *  to deallocate the same at destruction time.
 */
#pragma once
#include <memory>

#include "libbuffer/IBufferSegmentAllocationStrategy.hpp"

namespace buffer
{

class DataSegment
{
public:
    DataSegment(IBufferSegmentAllocationStrategy &strategy, size_t size)
        : m_strategy(strategy)
        , m_buf(strategy.allocate(size))
        , m_size(size)
    {}

    ~DataSegment() { m_strategy.deallocate(m_buf); }
    template<typename T> T* ptr() { return static_cast<T*>(m_buf); }
    size_t size() { return m_size; }

private:
    IBufferSegmentAllocationStrategy &m_strategy;
    void *m_buf;
    size_t m_size;
};

}; // namespace message
