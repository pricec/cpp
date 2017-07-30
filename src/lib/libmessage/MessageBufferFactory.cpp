#include "MessageBufferFactory.hpp"

using namespace message;

MessageBufferFactory::MessageBufferFactory(
    IMessageBufferAllocationStrategy &strategy
)
    : m_strategy(strategy)
{}

MessageBufferFactory::~MessageBufferFactory()
{}

std::shared_ptr<MessageBuffer> MessageBufferFactory::allocate(size_t size)
{
    return std::shared_ptr<MessageBuffer>(new MessageBuffer(m_strategy, size));
}

