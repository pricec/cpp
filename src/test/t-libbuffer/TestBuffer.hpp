#pragma once
#include <gtest/gtest.h>

class TestBuffer : public ::testing::Test
{
protected:
  TestBuffer() {}
  ~TestBuffer() {}

private:
  TestBuffer(const TestBuffer &other);
  TestBuffer& operator=(const TestBuffer &rhs);

protected:
  virtual void SetUp();
  virtual void TearDown();
};
