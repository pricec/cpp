#pragma once
#include <gtest/gtest.h>

class TestFuture : public ::testing::Test
{
protected:
  TestFuture() {}
  ~TestFuture() {}

private:
  TestFuture(const TestFuture &other);
  TestFuture& operator=(const TestFuture &rhs);

protected:
  virtual void SetUp();
  virtual void TearDown();
};
