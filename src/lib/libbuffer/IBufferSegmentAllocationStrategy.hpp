/** \brief Buffer Segment Allocation Strategy interface
 *
 *  Since message buffers will be allocated quite frequently,
 *  some kind of pooling strategy may be required to reduce
 *  heap contention and fragmentation. This simple interface
 *  is meant to admit an allocation strategy.
 */
#pragma once
#include <memory>

#include "libbuffer/DataSegment.hpp"

namespace buffer
{

class IBufferSegmentAllocationStrategy
{
public:
    virtual ~IBufferSegmentAllocationStrategy() {};
    virtual void* allocate(size_t size) = 0;
    virtual void deallocate(void *buf) = 0;
};

}; //namespace
