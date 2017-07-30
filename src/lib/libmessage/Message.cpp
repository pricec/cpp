#include "Message.hpp"

using namespace message;

Message::Message()
    : m_numStatic(0)
{}

Message::Message(const Message &other)
    : m_numStatic(0)
{
    *this = other;
}

Message::~Message()
{}

Message& Message::operator=(const Message &rhs)
{
    m_numStatic = rhs.m_numStatic;
    for (size_t i = 0; i < m_numStatic; ++i)
    {
        m_bufs[i] = rhs.m_bufs[i];
    }
    std::deque<MessageBufferHolder> otherBufs(rhs.m_dbufs);
    m_dbufs.swap(otherBufs);
    return *this;
}

bool Message::append(
    const std::shared_ptr<MessageBuffer> &mb,
    size_t offset,
    size_t length
)
{
    if (m_dbufs.empty() && m_numStatic < NUM_STATIC)
    {
        MessageBufferHolder &buf(m_bufs[m_numStatic++]);
        buf.m_buf = mb;
        buf.m_offset = offset;
        buf.m_length = length;
    }
    else
    {
        m_dbufs.emplace_back(mb, offset, length);
    }
    return true;
}

bool Message::append(const std::shared_ptr<MessageBuffer> &mb)
{
    return append(mb, 0, mb->size());
}

bool Message::append(const Message &m)
{
    for (size_t i = 0; i < m.m_numStatic; ++i)
    {
        const MessageBufferHolder &mbh(m.m_bufs[i]);
        append(mbh.m_buf, mbh.m_offset, mbh.m_length);
    }
    for (const auto &mbh : m.m_dbufs)
    {
        append(mbh.m_buf, mbh.m_offset, mbh.m_length);
    }
    return true;
}

char Message::getByte(size_t offset) const
{
    char byte = '\0';
    size_t startOffset = 0;
    for (size_t i = 0; i < m_numStatic; ++i)
    {
        const MessageBufferHolder &mbh(m_bufs[i]);
        if (startOffset + mbh.m_length > offset)
        {
            // The byte is in this MessageBufferHolder
            return mbh.m_buf->ptr<char>()[offset-startOffset+mbh.m_offset];
        }
        startOffset += mbh.m_length;
    }
    if (offset >= startOffset)
    {
        for (const auto &mbh : m_dbufs)
        {
            if (startOffset + mbh.m_length > offset)
            {
                // The byte is in this MessageBufferHolder
                return mbh.m_buf->ptr<char>()[offset-startOffset+mbh.m_offset];
            }
            startOffset += mbh.m_length;
        }
    }
    return byte;
}

Message Message::getData(size_t offset, size_t length) const
{
    Message m;
    size_t startOffset = 0;
    for (size_t i = 0; i < m_numStatic; ++i)
    {
        const MessageBufferHolder &mbh(m_bufs[i]);
        size_t numCopy = 0;
        size_t newOffset = mbh.m_offset;
        size_t newLength = mbh.m_length;
        if (startOffset < offset && startOffset + mbh.m_length > offset)
        {
            newOffset += offset - startOffset;
            newLength += startOffset - offset;
        }
        if (startOffset + mbh.m_length > offset)
        {
            numCopy = std::min(length, newLength);
            m.append(mbh.m_buf, newOffset, numCopy);
            length -= numCopy;
        }
        startOffset += mbh.m_length;
    }
    if (length > 0)
    {
        for (const auto &mbh : m_dbufs)
        {
            size_t numCopy = 0;
            size_t newOffset = mbh.m_offset;
            size_t newLength = mbh.m_length;
            if (startOffset < offset && startOffset + mbh.m_length > offset)
            {
                newOffset += offset - startOffset;
                newLength += startOffset - offset;
            }
            if (startOffset + mbh.m_length > offset)
            {
                numCopy = std::min(length, newLength);
                m.append(mbh.m_buf, newOffset, numCopy);
                length -= numCopy;
            }
            startOffset += mbh.m_length;
        }
    }
    return m;
}
