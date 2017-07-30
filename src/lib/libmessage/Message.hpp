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

/**
 * This struct is meant to hold a message buffer, together
 * with an offset and a length. This is because messages
 * will potentially be sliced up in different ways, and
 * we may need to refer to only part of a MessageBuffer.
 * Additionally, multiple message may share the same
 * MessageBuffer, but with different length and offset.
 */
struct MessageBufferHolder
{
    MessageBufferHolder() {}
    MessageBufferHolder(const MessageBufferHolder &other)
        : m_buf(other.m_buf)
        , m_offset(other.m_offset)
        , m_length(other.m_length)
    {}
    MessageBufferHolder(
        const ManagedBuffer &buf,
        size_t offset, size_t length
    )
        : m_buf(buf)
        , m_offset(offset)
        , m_length(length)
    {}

    MessageBufferHolder& operator=(const MessageBufferHolder &other)
    {
        m_buf = other.m_buf;
        m_offset = other.m_offset;
        m_length = other.m_length;
        return *this;
    }

    ManagedBuffer m_buf;
    size_t m_offset;
    size_t m_length;
};

class Message
{
public:
    Message();
    Message(const Message &other);
    ~Message();

    /**
     * The equals operator simply makes a read-only
     * shallow copy of the Message. Refcounts are adjusted
     * so that the underlying buffer cannot be freed before
     * both the copy and the original Message are deleted.
     */
    Message& operator=(const Message &rhs);

    /**
     * Append the provided MessageBuffer with the provided
     * offset and length to this Message.
     */
    bool append(
        const ManagedBuffer &mb,
        size_t offset, size_t length
    );
    /**
     * Append the entire provided MessageBuffer (that is,
     * offset = 0 and length = mb.size()) to this Message.
     */
    bool append(const ManagedBuffer &mb);
    /**
     * Append the entire provided Message to this Message.
     */
    bool append(const Message &m);

    /**
     * Get the byte at the specified offset, as a character.
     */
    char getByte(size_t offset) const;

    /**
     * Get a new Message whose data is equal to this Message
     * starting at the provided offset and for the provided
     * length.
     */
    Message getData(size_t offset, size_t length) const;

private:
    static const size_t NUM_STATIC = 5;
    MessageBufferHolder m_bufs[NUM_STATIC];
    size_t m_numStatic;
    std::deque<MessageBufferHolder> m_dbufs;
};

}; // namespace message
