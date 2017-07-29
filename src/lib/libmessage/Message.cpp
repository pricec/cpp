#include "Message.hpp"

using namespace message;

Message::Message()
    : m_numStatic(0)
{}

Message::Message(const Message &other)
    : m_numStatic(0)
{
    append(other);
}

Message::~Message()
{}

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
    for (const auto &mbh : m.m_bufs)
    {
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
    for (const auto &mbh : m_bufs)
    {
        if (startOffset + mbh.m_length >= offset)
        {
            // The byte is in this MessageBufferHolder
            return mbh.m_buf->ptr<char>()[offset-startOffset+mbh.m_offset];
        }
        startOffset += mbh.m_length;
    }
    if (offset > startOffset)
    {
        for (const auto &mbh : m_dbufs)
        {
            if (startOffset + mbh.m_length >= offset)
            {
                // The byte is in this MessageBufferHolder
                return mbh.m_buf->ptr<char>()[offset-startOffset+mbh.m_offset];
            }
            startOffset += mbh.m_length;
        }
    }
    return byte;
}
