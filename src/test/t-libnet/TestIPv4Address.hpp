#pragma once
#include <gtest/gtest.h>

class TestIPv4Address : public ::testing::Test
{
protected:
    TestIPv4Address() {}
    ~TestIPv4Address() {}
    TestIPv4Address(const TestIPv4Address &other) = delete;
    TestIPv4Address& operator=(const TestIPv4Address &rhs) = delete;

protected:
    virtual void SetUp();
    virtual void TearDown();
};
