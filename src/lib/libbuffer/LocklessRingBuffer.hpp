/** \brief Lockless Ring Buffer class
 *
 *  This class is meant to be used by a single producer
 *  and a single consumer (which may be distinct) as a ring
 *  buffer. It is protected against overflow and is threadsafe
 *  for use by the single producer and consumer.
 */
#pragma once
#include <vector>
#include <cstdio>

namespace buffer
{

template<typename T>
class LocklessRingBuffer
{
public:
    LocklessRingBuffer(size_t size)
        : m_size(size)
        , m_head(0)
        , m_tail(0)
        , m_empty(true)
    {
        m_items.reserve(m_size);
    }

    ~LocklessRingBuffer()
    {}

    /**
     * Insert the argument item. Returns true if
     * the item was inserted, and false otherwise.
     * The function will return false if the buffer
     * is full.
     *
     * NOTE: This function should only be called
     *       from the producer.
     */
    bool insert(T item)
    {
        if (!m_empty && m_head == m_tail) // Then the buffer is full
        {
            return false;
        }

        m_items[m_head] = item;
        m_empty = false;
        m_head = (m_head + 1) % m_size;
        return true;
    }

    /**
     * Get the next item. It is up to you to check
     * hasItem first to ensure the returned item
     * will be valid.
     *
     * NOTE: This function should only be called
     *       from the consumer.
     */
    T next()
    {
        T item;
        if (!m_empty)
        {
            m_empty = (m_head == ((m_tail + 1) % m_size));
            size_t oldTail = m_tail;
            m_tail = (m_tail + 1) % m_size;
            item = m_items[oldTail];
        }
        return item;
    }

    /**
     * Returns true if the queue has a valid item in
     * it and false otherwise.
     *
     * NOTE: This function should only be called
     *       from the consumer.
     */
    bool hasItem() const
    {
        return !m_empty;
    }

private:
    std::vector<T> m_items;
    size_t m_size;
    size_t m_head; // Insert at the head
    size_t m_tail; // Next is at tail
    bool m_empty;
};

}; // namespace buffer
