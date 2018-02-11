#pragma once
#include <random>

namespace common
{

class UUID
{
public:
    UUID()
        : m_major()
        , m_minor()
    {
        std::mt19937 rng;
        rng.seed(std::random_device()());
        std::uniform_int_distribution<
            std::mt19937::result_type
        > d(0, UINT64_MAX);

        m_major = d(rng);
        m_minor = d(rng);
    }

    UUID(const UUID &other)
        : m_major(other.m_major)
        , m_minor(other.m_minor)
    {}

    UUID(uint64_t major, uint64_t minor)
        : m_major(major)
        , m_minor(minor)
    {}

    UUID& operator=(const UUID &rhs)
    {
        m_major = rhs.m_major;
        m_minor = rhs.m_minor;
        return *this;
    }

    bool operator<(const UUID &rhs) const
    {
        if (m_major < rhs.m_major)
        {
            return true;
        }
        else if (m_major > rhs.m_major)
        {
            return false;
        }
        return m_minor < rhs.m_minor;
    }

    std::string str() const
    {
        std::string result;
        result.append(std::to_string(m_major));
        result.append(std::to_string(m_minor));
        return result;
    }

private:
    uint64_t m_major;
    uint64_t m_minor;
};

}; // namespace common
