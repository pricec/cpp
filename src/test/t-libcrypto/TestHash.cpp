#include "TestHash.hpp"
#include "libcrypto/Hash.hpp"

void TestHash::SetUp()
{}

void TestHash::TearDown()
{}

TEST_F(TestHash, TestMD5Hash)
{
    std::string in("password");
    std::string salt("aBcD1234");
    std::string out;
    ASSERT_EQ(crypto::ComputePasswordHash(crypto::MD5, in, salt, out), 0);
    ASSERT_EQ(out, "$1$aBcD1234$J.j26XNFCjmaBGrJB4/D.0");
}

TEST_F(TestHash, TestSHA256Hash)
{
    std::string in("password");
    std::string salt("AbCd1234");
    std::string out;
    ASSERT_EQ(crypto::ComputePasswordHash(crypto::SHA256, in, salt, out), 0);
    ASSERT_EQ(out, "$5$AbCd1234$CsoaALTs5AygZUGoAKZvHs9QMiRGhQByKrSAVluGZr2");
}

TEST_F(TestHash, TestSHA512Hash)
{
    std::string in("password");
    std::string salt("xTIwWQSN");
    std::string out;
    ASSERT_EQ(crypto::ComputePasswordHash(crypto::SHA512, in, salt, out), 0);
    ASSERT_EQ(out, "$6$xTIwWQSN$M2bneJgIQRysu92MVy8UO.7syFQrT1.scEy7bWMrdqCRpwoIyOZPiWL3bZEqKNvS3Ti/y.Qw5Pv9H8yw9NY2M.");
}
