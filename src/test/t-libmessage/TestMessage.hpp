#pragma once
#include <gtest/gtest.h>

class TestMessage : public ::testing::Test
{
protected:
  TestMessage() {}
  ~TestMessage() {}

private:
  TestMessage(const TestMessage &other);
  TestMessage& operator=(const TestMessage &rhs);

protected:
  virtual void SetUp();
  virtual void TearDown();
};
