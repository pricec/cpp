#pragma once
#include <gtest/gtest.h>

class TestMacAddress : public ::testing::Test
{
protected:
    TestMacAddress() {}
    ~TestMacAddress() {}
    TestMacAddress(const TestMacAddress &other) = delete;
    TestMacAddress& operator=(const TestMacAddress &rhs) = delete;

protected:
    virtual void SetUp();
    virtual void TearDown();
};
