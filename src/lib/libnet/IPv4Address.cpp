#include "IPv4Address.hpp"

using namespace net;

IPv4Address::IPv4Address()
    : m_address(0)
    , m_endianness(common::Endianness::BIG)
{}

IPv4Address::IPv4Address(uint32_t address, common::Endianness endianness)
    : m_address(address)
    , m_endianness(endianness)
{}

IPv4Address::IPv4Address(const IPv4Address &other)
    : m_address(other.m_address)
    , m_endianness(other.m_endianness)
{}

IPv4Address& IPv4Address::operator=(const IPv4Address &rhs)
{
    this->m_address    = rhs.m_address;
    this->m_endianness = rhs.m_endianness;
    return *this;
}

bool IPv4Address::operator==(const IPv4Address &other) const
{
    return this->m_address    == other.m_address
        && this->m_endianness == other.m_endianness;
}

std::string IPv4Address::str() const
{
    char addr[16];
    if (m_endianness == common::Endianness::BIG)
    {
        ::sprintf(
            addr,
            "%d.%d.%d.%d",
            (m_address & 0xff000000) >> 24,
            (m_address & 0x00ff0000) >> 16,
            (m_address & 0x0000ff00) >> 8,
            (m_address & 0x000000ff)
        );
    }
    else
    {
        ::sprintf(
            addr,
            "%d.%d.%d.%d",
            (m_address & 0x000000ff),
            (m_address & 0x0000ff00) >> 8,
            (m_address & 0x00ff0000) >> 16,
            (m_address & 0xff000000) >> 24
        );
    }
    return std::string(addr);
}
