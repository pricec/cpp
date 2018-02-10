#pragma once
#include "libcommon/Endianness.hpp"
#include <string>

namespace net
{

class IPv4Address
{
public:
    IPv4Address();
    IPv4Address(uint32_t address, common::Endianness endianness);
    IPv4Address(const IPv4Address &other);
    IPv4Address& operator=(const IPv4Address &rhs);
    bool operator==(const IPv4Address &other) const;
    std::string str() const;

private:
    uint32_t           m_address;
    common::Endianness m_endianness;
};

}
