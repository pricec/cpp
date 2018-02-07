#include "MacAddress.hpp"

#include <string.h>

using namespace net;

MacAddress::MacAddress(const void *buf, common::Endianness end)
{
    bytes[s_macaddr_len] = '\0';
    for (int i = 0; i < s_macaddr_len; ++i)
    {
        if (end == common::Endianness::BIG)
        {
            bytes[i] = ((char *)buf)[i];
        }
        else
        {
            bytes[i] = ((char *)buf)[s_macaddr_len - 1 - i];
        }
    }
}

MacAddress::MacAddress(const MacAddress &other)
{
    *this = other;
}

MacAddress& MacAddress::operator=(const MacAddress &rhs)
{
    ::memcpy(this->bytes, rhs.bytes, s_macaddr_len + 1);
    return *this;
}

bool MacAddress::operator==(const MacAddress &other) const
{
    return ::memcmp(this->bytes, other.bytes, s_macaddr_len) == 0;
}

std::string MacAddress::str() const
{
    char byte[3];
    std::string str;
    for (int i = 0; i < s_macaddr_len; ++i)
    {
        if (i > 0)
        {
            str.append(":");
        }
        sprintf(byte, "%02x", bytes[i]);
        str.append(byte);
    }
    return str;
}
