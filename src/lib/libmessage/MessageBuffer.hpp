#pragma once
#include <memory>

namespace message
{

class MessageBuffer
{
public:
    MessageBuffer(void *buf, size_t size)
        : m_buf(buf)
        , m_size(size)
    {}

    ~MessageBuffer() { free(m_buf); }

    template<typename T> T* ptr() { return static_cast<T*>(m_buf); }
    size_t size() { return m_size; }

private:
    void *m_buf;
    size_t m_size;
};

}; // namespace message
