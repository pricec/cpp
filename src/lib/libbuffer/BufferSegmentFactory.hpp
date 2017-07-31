/** \brief Buffer Buffer Factory
 *
 *  This class is meant to use the IBufferSegmentAllocationStrategy
 *  it is given to manage buffer allocations. The void* returned by
 *  the strategy is stored inside the DataSegment class; the
 *  DataSegment itself is managed by a shared_ptr returned by the
 *  allocate function. The Buffer class makes up a message from
 *  many DataSegments; the use of shared pointers allows the
 *  sharing of DataSegments between Buffers and also ensures
 *  safe cleanup.
 */
#pragma once

#include "DataSegment.hpp"
#include "IBufferSegmentAllocationStrategy.hpp"

namespace buffer
{

using BufferSegment = std::shared_ptr<DataSegment>;

class BufferSegmentFactory
{
public:
    BufferSegmentFactory(IBufferSegmentAllocationStrategy &strategy);
    ~BufferSegmentFactory();

    /**
     * Allocate a DataSegment which can store at least
     * size bytes. Returns a shared_ptr to the allocated
     * DataSegment.
     */
    BufferSegment allocate(size_t size);

private:
    IBufferSegmentAllocationStrategy &m_strategy;
};

}; // namespace message
