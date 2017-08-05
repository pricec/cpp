#pragma once
#include <mutex>
#include <condition_variable>

#include "LocklessRingBuffer.hpp"

namespace buffer
{

template<typename T>
class RingBuffer
{
public:
    explicit RingBuffer(size_t size)
        : m_buf(size)
    {}

    RingBuffer(const RingBuffer &other)
        : m_buf(other.m_buf)
    {}

    bool insert(T item)
    {
        bool result;
        {
            std::lock_guard<std::mutex> lock(m_mtx);
            result = m_buf.insert(item);
        }
        if (result)
        {
            m_cv.notify_one();
        }
        return result;
    }

    T next()
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        m_cv.wait(lock, [this](){
            return m_buf.hasItem();
        });
        return m_buf.next();
    }

private:
    std::mutex m_mtx;
    std::condition_variable m_cv;
    LocklessRingBuffer<T> m_buf;
};

}; //namespace buffer
