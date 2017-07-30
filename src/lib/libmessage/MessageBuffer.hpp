/** \brief Message Buffer
 *
 *  This class wraps a void* to hold message data. It
 *  uses the passed in IMessageBufferAllocationStrategy
 *  in order to allocate memory at construction time and
 *  to deallocate the same at destruction time.
 */
#pragma once
#include <memory>

#include "libmessage/IMessageBufferAllocationStrategy.hpp"

namespace message
{

class MessageBuffer
{
public:
    MessageBuffer(IMessageBufferAllocationStrategy &strategy, size_t size)
        : m_strategy(strategy)
        , m_buf(strategy.allocate(size))
        , m_size(size)
    {}

    ~MessageBuffer() { m_strategy.deallocate(m_buf); }
    template<typename T> T* ptr() { return static_cast<T*>(m_buf); }
    size_t size() { return m_size; }

private:
    IMessageBufferAllocationStrategy &m_strategy;
    void *m_buf;
    size_t m_size;
};

}; // namespace message
