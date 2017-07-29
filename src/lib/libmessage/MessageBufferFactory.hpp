#pragma once

#include "MessageBuffer.hpp"
#include "IMessageBufferAllocationStrategy.hpp"

namespace message
{

class MessageBufferFactory
{
public:
    MessageBufferFactory(IMessageBufferAllocationStrategy &strategy);
    ~MessageBufferFactory();
    std::shared_ptr<MessageBuffer> allocate(size_t size);

private:
    IMessageBufferAllocationStrategy &m_strategy;
};

}; // namespace message
