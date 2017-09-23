#pragma once
#include <gtest/gtest.h>

class TestHash : public ::testing::Test
{
protected:
  TestHash() {}
  ~TestHash() {}

private:
  TestHash(const TestHash &other);
  TestHash& operator=(const TestHash &rhs);

protected:
  virtual void SetUp();
  virtual void TearDown();
};
