#include "TestMacAddress.hpp"
#include "libcommon/Endianness.hpp"
#include "libnet/MacAddress.hpp"

void TestMacAddress::SetUp()
{}

void TestMacAddress::TearDown()
{}

TEST_F(TestMacAddress, TestBigEndian)
{
    const uint8_t s[6] = { 1, 2, 3, 4, 5, 6 };
    std::string result("01:02:03:04:05:06");
    net::MacAddress mac(s, common::Endianness::BIG);
    ASSERT_EQ(mac.str(), result);
}

TEST_F(TestMacAddress, TestLittleEndian)
{
    const uint8_t s[6] = { 6, 5, 4, 3, 2, 1 };
    std::string result("01:02:03:04:05:06");
    net::MacAddress mac(s, common::Endianness::LITTLE);
    ASSERT_EQ(mac.str(), result);
}

TEST_F(TestMacAddress, TestCopyConstructor)
{
    const uint8_t s[6] = { 100, 110, 120, 130, 140, 150 };
    std::string result("64:6e:78:82:8c:96");
    net::MacAddress mac(s, common::Endianness::BIG);
    net::MacAddress copy(mac);
    ASSERT_TRUE(mac == copy);
}

TEST_F(TestMacAddress, TestEqualsOperator)
{
    const uint8_t s[6] = { 1, 2, 3, 4, 5, 6 };
    net::MacAddress mac(s, common::Endianness::BIG);
    net::MacAddress mac2(s, common::Endianness::LITTLE);
    ASSERT_FALSE(mac == mac2);
    mac = mac2;
    ASSERT_TRUE(mac == mac2);
}

