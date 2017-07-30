/** \brief Basic Message Buffer Allocation strategy
 *
 *  This class implements IMessageBufferAllocationStrategy
 *  in the simplest way possible; a call to allocate simply
 *  calls malloc and returns the resulting void*.
 */
#pragma once

#include "libmessage/IMessageBufferAllocationStrategy.hpp"

namespace message
{

class BasicMessageBufferAllocationStrategy :
        public IMessageBufferAllocationStrategy
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
