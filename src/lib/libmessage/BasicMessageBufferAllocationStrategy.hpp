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
};

}; // namespace message
