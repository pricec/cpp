#include "TestHash.hpp"
#include "libcrypto/Hash.hpp"

void TestHash::SetUp()
{}

void TestHash::TearDown()
{}

TEST_F(TestHash, TestMD5Hash)
{
    std::string in("The quick brown fox jumps over the lazy dog");
    std::string out;
    ASSERT_EQ(crypto::ComputeHash(crypto::MD5, in, out), 0);
    ASSERT_EQ(out, "9e107d9d372bb6826bd81d3542a419d6");
}

TEST_F(TestHash, TestSHA256Hash)
{
    std::string in("The quick brown fox jumps over the lazy dog");
    std::string out;
    ASSERT_EQ(crypto::ComputeHash(crypto::SHA256, in, out), 0);
    ASSERT_EQ(out, "d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592");
}

TEST_F(TestHash, TestSHA512Hash)
{
    std::string in("The quick brown fox jumps over the lazy dog");
    std::string out;
    ASSERT_EQ(crypto::ComputeHash(crypto::SHA512, in, out), 0);
    ASSERT_EQ(out, "07e547d9586f6a73f73fbac0435ed76951218fb7d0c8d788a309d785436bbb642e93a252a954f23912547d1e8a3b5ed6e1bfd7097821233fa0538f3db854fee6");
}
