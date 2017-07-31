#pragma once
#include <gtest/gtest.h>

class TestLocklessRingBuffer : public ::testing::Test
{
protected:
  TestLocklessRingBuffer() {}
  ~TestLocklessRingBuffer() {}

private:
  TestLocklessRingBuffer(const TestLocklessRingBuffer &other);
  TestLocklessRingBuffer& operator=(const TestLocklessRingBuffer &rhs);

protected:
  virtual void SetUp();
  virtual void TearDown();
};
