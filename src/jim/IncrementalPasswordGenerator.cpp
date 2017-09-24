#include "IncrementalPasswordGenerator.hpp"

using namespace pj;

IncrementalPasswordGenerator::IncrementalPasswordGenerator()
    : m_maxLength(DEFAULT_MAXLENGTH)
    , m_charset(DEFAULT_CHARSET)
    , m_curLength()
    , m_curPass()
{
    increaseLength();
}

IncrementalPasswordGenerator::IncrementalPasswordGenerator(size_t maxLength)
    : m_maxLength(maxLength)
    , m_charset(DEFAULT_CHARSET)
    , m_curLength()
    , m_curPass()
{
//    assert(maxLength <= DEFAULT_MAXLENGTH);
    increaseLength();
}

IncrementalPasswordGenerator::IncrementalPasswordGenerator(
    std::vector<unsigned int> charset)
    : m_maxLength(DEFAULT_MAXLENGTH)
    , m_charset(charset)
    , m_curLength()
    , m_curPass()
{
    printf("charset constructor\n");
    increaseLength();
}

IncrementalPasswordGenerator::IncrementalPasswordGenerator(
    size_t maxLength,
    std::vector<unsigned int> charset
)
    : m_maxLength(maxLength)
    , m_charset(charset)
    , m_curLength()
    , m_curPass()
{
    increaseLength();
}

IncrementalPasswordGenerator::~IncrementalPasswordGenerator()
{
    if (m_curPass)
    {
        free(m_curPass);
        m_curPass = NULL;
    }
}

bool IncrementalPasswordGenerator::hasNext()
{
    return !isLast();
}

std::string IncrementalPasswordGenerator::next()
{
    // Find the first character from the end
    // of the string which is NOT m_charset.back()
    int increment = -1;
    for (size_t i = m_curLength - 1; i >= 0; --i)
    {
        if (m_curPass[i] != m_charset.back())
        {
            increment = i;
            break;
        }
    }

    if (increment == -1)
    {
        increaseLength();
    }
    else
    {
        // Increment the character at 'increment'
        for (auto it = m_charset.cbegin(); it != m_charset.cend(); ++it)
        {
            if (*it == m_curPass[increment])
            {
                m_curPass[increment] = *(++it);
                break;
            }
        }
        // reset the characters at [increment+1,m_curLength)
        for (size_t i = increment + 1; i < m_curLength; ++i)
        {
            m_curPass[i] = m_charset.front();
        }
    }
    return std::string(m_curPass);
}

void IncrementalPasswordGenerator::increaseLength()
{
    ++m_curLength;
    if (m_curPass)
    {
        free(m_curPass);
    }
    m_curPass = (char *)malloc(m_curLength);
    for (int i = 0; i < m_curLength; ++i)
    {
        m_curPass[i] = m_charset.front();
    }
}

bool IncrementalPasswordGenerator::isLast()
{
    if (m_curLength < m_maxLength)
    {
        return false;
    }
    if (m_curPass)
    {
        for (int i = 0; i < m_curLength; ++i)
        {
            if (m_curPass[i] != m_charset.back())
            {
                return false;
            }
        }
    }

    return true;
}

