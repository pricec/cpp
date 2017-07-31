/** \brief Basic Buffer Buffer Allocation strategy
 *
 *  This class implements IBufferSegmentAllocationStrategy
 *  in the simplest way possible; a call to allocate simply
 *  calls malloc and returns the resulting void*.
 */
#pragma once

#include "libbuffer/IBufferSegmentAllocationStrategy.hpp"

namespace buffer
{

class BasicBufferSegmentAllocationStrategy :
        public IBufferSegmentAllocationStrategy
{
public:
    void* allocate(size_t size)
    {
        return malloc(size);
    }

    void deallocate(void *buf)
    {
        free(buf);
    }
};

}; // namespace message
