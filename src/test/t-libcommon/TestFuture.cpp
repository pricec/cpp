#include "TestFuture.hpp"
#include "libcommon/Future.hpp"
#include <thread>

void TestFuture::SetUp()
{}

void TestFuture::TearDown()
{}

TEST_F(TestFuture, TestSet)
{
  common::Future<int> f;
  f.set(4);
  ASSERT_TRUE(f.wait());
  ASSERT_EQ(f.value(), 4);
}

TEST_F(TestFuture, TestCancel)
{
  common::Future<int> f;
  f.cancel();
  ASSERT_FALSE(f.wait());
}
