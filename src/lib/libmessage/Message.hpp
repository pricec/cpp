/** \brief Message class
 *
 *  This class is for holding bytes of data, presumably
 *  part of some kind of message being passed from one
 *  process to another. It employs a copy-on-write pattern
 *  using the other classes in this directory.
 *
 *  Please note that this class is not safe for concurrent
 *  access, except by concurrent calls to const methods.
 */

#pragma once
#include <deque>

#include "MessageBufferFactory.hpp"
#include "MessageBuffer.hpp"

namespace message
{

struct MessageBufferHolder
{
    MessageBufferHolder() {}
    MessageBufferHolder(
        const std::shared_ptr<MessageBuffer> &buf,
        size_t offset, size_t length
    )
        : m_buf(buf)
        , m_offset(offset)
        , m_length(length)
    {}
    std::shared_ptr<MessageBuffer> m_buf;
    size_t m_offset;
    size_t m_length;
};

class Message
{
public:
    Message();
    Message(const Message &other);
    ~Message();

    bool append(
        const std::shared_ptr<MessageBuffer> &mb,
        size_t offset, size_t length
    );
    bool append(const std::shared_ptr<MessageBuffer> &mb);
    bool append(const Message &m);

    char getByte(size_t offset) const;

private:
    static const size_t NUM_STATIC = 5;
    MessageBufferHolder m_bufs[NUM_STATIC];
    size_t m_numStatic;
    std::deque<MessageBufferHolder> m_dbufs;
};

}; // namespace message
