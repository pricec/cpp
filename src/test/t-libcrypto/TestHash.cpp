#include "TestHash.hpp"
#include "libcrypto/Hash.hpp"

void TestHash::SetUp()
{}

void TestHash::TearDown()
{}

TEST_F(TestHash, TestHashFunction)
{
    std::string in("The quick brown fox jumps over the lazy dog");
    std::string out;
    ASSERT_EQ(crypto::ComputeHash("md5", in, out), 0);
    ASSERT_EQ(out, "9e107d9d372bb6826bd81d3542a419d6");
}


