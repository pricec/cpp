/** \brief Message Buffer Allocation Strategy interface
 *
 *  Since message buffers will be allocated quite frequently,
 *  some kind of pooling strategy may be required to reduce
 *  heap contention and fragmentation. This simple interface
 *  is meant to admit an allocation strategy.
 */
#pragma once
#include <memory>

#include "libmessage/MessageBuffer.hpp"

namespace message
{

class IMessageBufferAllocationStrategy
{
public:
    virtual ~IMessageBufferAllocationStrategy() {};
    virtual void* allocate(size_t size) = 0;
    virtual void deallocate(void *buf) = 0;
};

}; //namespace mssage
