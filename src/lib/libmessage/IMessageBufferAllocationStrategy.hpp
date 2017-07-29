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
};

}; //namespace mssage
