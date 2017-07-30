#include "MessageBufferFactory.hpp"

using namespace message;

MessageBufferFactory::MessageBufferFactory(
    IMessageBufferAllocationStrategy &strategy
)
    : m_strategy(strategy)
{}

MessageBufferFactory::~MessageBufferFactory()
{}

ManagedBuffer MessageBufferFactory::allocate(size_t size)
{
    return ManagedBuffer(new MessageBuffer(m_strategy, size));
}

