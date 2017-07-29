#pragma once
#include <thread>
#include <gtest/gtest.h>

class TestWorkQueue : public ::testing::Test
{
protected:
  TestWorkQueue() {}
  ~TestWorkQueue() {}

private:
  TestWorkQueue(const TestWorkQueue &other);
  TestWorkQueue& operator=(const TestWorkQueue &rhs);

protected:
  virtual void SetUp();
  virtual void TearDown();
};
