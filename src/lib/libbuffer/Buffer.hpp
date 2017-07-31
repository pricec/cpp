/** \brief Buffer class
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

#include "BufferSegmentFactory.hpp"
#include "DataSegment.hpp"

namespace buffer
{

/**
 * This struct is meant to hold a message buffer, together
 * with an offset and a length. This is because messages
 * will potentially be sliced up in different ways, and
 * we may need to refer to only part of a DataSegment.
 * Additionally, multiple message may share the same
 * DataSegment, but with different length and offset.
 */
struct DataSegmentHolder
{
    DataSegmentHolder() {}
    DataSegmentHolder(const DataSegmentHolder &other)
        : m_buf(other.m_buf)
        , m_offset(other.m_offset)
        , m_length(other.m_length)
    {}
    DataSegmentHolder(
        const ManagedBuffer &buf,
        size_t offset, size_t length
    )
        : m_buf(buf)
        , m_offset(offset)
        , m_length(length)
    {}

    DataSegmentHolder& operator=(const DataSegmentHolder &other)
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

class Buffer
{
public:
    Buffer();
    Buffer(const Buffer &other);
    ~Buffer();

    /**
     * The equals operator simply makes a read-only
     * shallow copy of the Buffer. Refcounts are adjusted
     * so that the underlying buffer cannot be freed before
     * both the copy and the original Buffer are deleted.
     */
    Buffer& operator=(const Buffer &rhs);

    /**
     * Append the provided DataSegment with the provided
     * offset and length to this Buffer.
     */
    bool append(
        const ManagedBuffer &mb,
        size_t offset, size_t length
    );
    /**
     * Append the entire provided DataSegment (that is,
     * offset = 0 and length = mb.size()) to this Buffer.
     */
    bool append(const ManagedBuffer &mb);
    /**
     * Append the entire provided Buffer to this Buffer.
     */
    bool append(const Buffer &m);

    /**
     * Get the byte at the specified offset, as a character.
     */
    char getByte(size_t offset) const;

    /**
     * Get a new Buffer whose data is equal to this Buffer
     * starting at the provided offset and for the provided
     * length.
     */
    Buffer getData(size_t offset, size_t length) const;

private:
    static const size_t NUM_STATIC = 5;
    DataSegmentHolder m_bufs[NUM_STATIC];
    size_t m_numStatic;
    std::deque<DataSegmentHolder> m_dbufs;
};

}; // namespace message
