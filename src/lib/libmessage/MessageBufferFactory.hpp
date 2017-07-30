/** \brief Message Buffer Factory
 *
 *  This class is meant to use the IMessageBufferAllocationStrategy
 *  it is given to manage buffer allocations. The void* returned by
 *  the strategy is stored inside the MessageBuffer class; the
 *  MessageBuffer itself is managed by a shared_ptr returned by the
 *  allocate function. The Message class makes up a message from
 *  many MessageBuffers; the use of shared pointers allows the
 *  sharing of MessageBuffers between Messages and also ensures
 *  safe cleanup.
 */
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

    /**
     * Allocate a MessageBuffer which can store at least
     * size bytes. Returns a shared_ptr to the allocated
     * MessageBuffer.
     */
    std::shared_ptr<MessageBuffer> allocate(size_t size);

private:
    IMessageBufferAllocationStrategy &m_strategy;
};

}; // namespace message
