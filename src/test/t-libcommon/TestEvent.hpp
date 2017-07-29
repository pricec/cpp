#pragma once
#include <gtest/gtest.h>

class TestEvent : public ::testing::Test
{
protected:
  TestEvent() {}
  ~TestEvent() {}

private:
  TestEvent(const TestEvent &other);
  TestEvent& operator=(const TestEvent &rhs);

protected:
  virtual void SetUp();
  virtual void TearDown();
};
