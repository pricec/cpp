#include "Buffer.hpp"
#include <string.h>

using namespace buffer;

Buffer::Buffer()
    : m_numStatic(0)
{}

Buffer::Buffer(const Buffer &other)
    : m_numStatic(0)
{
    *this = other;
}

Buffer::Buffer(const BufferSegment &bs)
    : m_numStatic(0)
{
    this->append(bs, 0, bs->size());
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
    std::deque<BufferSegmentHolder> otherBufs(rhs.m_dbufs);
    m_dbufs.swap(otherBufs);
    return *this;
}

size_t Buffer::length() const
{
    size_t length = 0;
    for (size_t i = 0; i < m_numStatic; ++i)
    {
        const BufferSegmentHolder &bsh(m_bufs[i]);
        length += bsh.m_length;
    }
    for (const auto &bsh : m_dbufs)
    {
        length += bsh.m_length;
    }
    return length;
}

void Buffer::clear()
{
    m_numStatic = 0;
    m_dbufs.clear();
}

bool Buffer::append(
    const BufferSegment &bs,
    size_t offset,
    size_t length
)
{
    if (m_dbufs.empty() && m_numStatic < NUM_STATIC)
    {
        BufferSegmentHolder &buf(m_bufs[m_numStatic++]);
        buf.m_bs = bs;
        buf.m_offset = offset;
        buf.m_length = length;
    }
    else
    {
        m_dbufs.emplace_back(bs, offset, length);
    }
    return true;
}

bool Buffer::append(const BufferSegment &bs)
{
    return append(bs, 0, bs->size());
}

bool Buffer::append(const Buffer &m)
{
    for (size_t i = 0; i < m.m_numStatic; ++i)
    {
        const BufferSegmentHolder &bsh(m.m_bufs[i]);
        append(bsh.m_bs, bsh.m_offset, bsh.m_length);
    }
    for (const auto &bsh : m.m_dbufs)
    {
        append(bsh.m_bs, bsh.m_offset, bsh.m_length);
    }
    return true;
}

char Buffer::getByte(size_t offset) const
{
    char byte = '\0';
    size_t startOffset = 0;
    for (size_t i = 0; i < m_numStatic; ++i)
    {
        const BufferSegmentHolder &bsh(m_bufs[i]);
        if (startOffset + bsh.m_length > offset)
        {
            // The byte is in this BufferSegmentHolder
            return bsh.m_bs->ptr<char>()[offset-startOffset+bsh.m_offset];
        }
        startOffset += bsh.m_length;
    }
    if (offset >= startOffset)
    {
        for (const auto &bsh : m_dbufs)
        {
            if (startOffset + bsh.m_length > offset)
            {
                // The byte is in this BufferSegmentHolder
                return bsh.m_bs->ptr<char>()[offset-startOffset+bsh.m_offset];
            }
            startOffset += bsh.m_length;
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
        const BufferSegmentHolder &bsh(m_bufs[i]);
        size_t numCopy = 0;
        size_t newOffset = bsh.m_offset;
        size_t newLength = bsh.m_length;
        if (startOffset < offset && startOffset + bsh.m_length > offset)
        {
            newOffset += offset - startOffset;
            newLength += startOffset - offset;
        }
        if (startOffset + bsh.m_length > offset)
        {
            numCopy = std::min(length, newLength);
            m.append(bsh.m_bs, newOffset, numCopy);
            length -= numCopy;
        }
        startOffset += bsh.m_length;
    }

    if (length > 0)
    {
        for (const auto &bsh : m_dbufs)
        {
            size_t numCopy = 0;
            size_t newOffset = bsh.m_offset;
            size_t newLength = bsh.m_length;
            if (startOffset < offset && startOffset + bsh.m_length > offset)
            {
                newOffset += offset - startOffset;
                newLength += startOffset - offset;
            }
            if (startOffset + bsh.m_length > offset)
            {
                numCopy = std::min(length, newLength);
                m.append(bsh.m_bs, newOffset, numCopy);
                length -= numCopy;
            }
            startOffset += bsh.m_length;
        }
    }
    return m;
}

void Buffer::flatten(
    BufferSegmentFactory &bufFac, size_t offset, size_t length
) {
    Buffer buf(getData(offset,length));
    if (buf.m_numStatic + m_dbufs.size() > 1)
    {
        Buffer start(getData(0, offset));
        Buffer end(getData(offset + length, this->length()));

        int spot = 0;
        BufferSegment bs(bufFac.allocate(length));
        for (size_t i = 0; i < m_numStatic; ++i)
        {
            auto const &bsh(m_bufs[i]);
            ::memcpy(
                bs->ptr<void>(),
                bsh.m_bs->ptr<char>() + bsh.m_offset,
                bsh.m_length
            );
            spot += bsh.m_length;
        }

        for (auto const &bsh : m_dbufs)
        {
            ::memcpy(
                bs->ptr<void>(),
                bsh.m_bs->ptr<char>() + bsh.m_offset,
                bsh.m_length
            );
            spot += bsh.m_length;
        }

        clear();
        append(start);
        append(bs);
        append(end);
    }
}
