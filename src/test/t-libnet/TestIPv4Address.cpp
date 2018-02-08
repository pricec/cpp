#include "TestIPv4Address.hpp"
#include "libcommon/Endianness.hpp"
#include "libnet/IPv4Address.hpp"

void TestIPv4Address::SetUp()
{}

void TestIPv4Address::TearDown()
{}

TEST_F(TestIPv4Address, TestBigEndian)
{
    uint32_t s = 0xc0a80102;
    std::string result("192.168.1.2");
    net::IPv4Address addr(s, common::Endianness::BIG);
    ASSERT_EQ(addr.str(), result);
}

TEST_F(TestIPv4Address, TestLittleEndian)
{
    uint32_t s = 0x0201a8c0;
    std::string result("192.168.1.2");
    net::IPv4Address addr(s, common::Endianness::LITTLE);
    ASSERT_EQ(addr.str(), result);
}

TEST_F(TestIPv4Address, TestCopyConstructor)
{
    uint32_t s = 0xfcfdfeff;
    std::string result("252.253.254.255");
    net::IPv4Address addr(s, common::Endianness::BIG);
    net::IPv4Address copy(addr);
    ASSERT_TRUE(addr == copy);
}

TEST_F(TestIPv4Address, TestEqualsOperator)
{
    uint32_t s = 0x01020304;
    net::IPv4Address addr(s, common::Endianness::BIG);
    net::IPv4Address addr2(s, common::Endianness::LITTLE);
    ASSERT_FALSE(addr == addr2);
    addr = addr2;
    ASSERT_TRUE(addr == addr2);
}

