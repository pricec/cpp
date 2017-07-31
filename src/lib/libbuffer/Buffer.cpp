#include "Buffer.hpp"

using namespace buffer;

Buffer::Buffer()
    : m_numStatic(0)
{}

Buffer::Buffer(const Buffer &other)
    : m_numStatic(0)
{
    *this = other;
}

Buffer::~Buffer()
{}

Buffer& Buffer::operator=(const Buffer &rhs)
{
    m_numStatic = rhs.m_numStatic;
    for (size_t i = 0; i < m_numStatic; ++i)
    {
        m_bufs[i] = rhs.m_bufs[i];
    }
    std::deque<DataSegmentHolder> otherBufs(rhs.m_dbufs);
    m_dbufs.swap(otherBufs);
    return *this;
}

bool Buffer::append(
    const ManagedBuffer &mb,
    size_t offset,
    size_t length
)
{
    if (m_dbufs.empty() && m_numStatic < NUM_STATIC)
    {
        DataSegmentHolder &buf(m_bufs[m_numStatic++]);
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

bool Buffer::append(const ManagedBuffer &mb)
{
    return append(mb, 0, mb->size());
}

bool Buffer::append(const Buffer &m)
{
    for (size_t i = 0; i < m.m_numStatic; ++i)
    {
        const DataSegmentHolder &mbh(m.m_bufs[i]);
        append(mbh.m_buf, mbh.m_offset, mbh.m_length);
    }
    for (const auto &mbh : m.m_dbufs)
    {
        append(mbh.m_buf, mbh.m_offset, mbh.m_length);
    }
    return true;
}

char Buffer::getByte(size_t offset) const
{
    char byte = '\0';
    size_t startOffset = 0;
    for (size_t i = 0; i < m_numStatic; ++i)
    {
        const DataSegmentHolder &mbh(m_bufs[i]);
        if (startOffset + mbh.m_length > offset)
        {
            // The byte is in this DataSegmentHolder
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
                // The byte is in this DataSegmentHolder
                return mbh.m_buf->ptr<char>()[offset-startOffset+mbh.m_offset];
            }
            startOffset += mbh.m_length;
        }
    }
    return byte;
}

Buffer Buffer::getData(size_t offset, size_t length) const
{
    Buffer m;
    size_t startOffset = 0;
    for (size_t i = 0; i < m_numStatic; ++i)
    {
        const DataSegmentHolder &mbh(m_bufs[i]);
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
