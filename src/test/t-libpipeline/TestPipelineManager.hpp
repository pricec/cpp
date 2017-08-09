#pragma once
#include <gtest/gtest.h>

class TestPipelineManager : public ::testing::Test
{
protected:
  TestPipelineManager() {}
  ~TestPipelineManager() {}

private:
  TestPipelineManager(const TestPipelineManager &other);
  TestPipelineManager& operator=(const TestPipelineManager &rhs);

protected:
  virtual void SetUp();
  virtual void TearDown();
};
