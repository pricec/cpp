#pragma once
#include "libcommon/Endianness.hpp"
#include <string>

namespace net
{

class MacAddress
{
public:
    MacAddress(const void *buf, common::Endianness end);
    MacAddress(const MacAddress &other);
    MacAddress& operator=(const MacAddress &rhs);
    bool operator==(const MacAddress &other) const;
    std::string str() const;

private:
    static const int s_macaddr_len = 6;
    uint8_t bytes[s_macaddr_len + 1];
};

}
