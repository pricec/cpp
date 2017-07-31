#include "TestLocklessRingBuffer.hpp"
#include "libbuffer/LocklessRingBuffer.hpp"
#include <thread>
#include <chrono>

using namespace buffer;

void TestLocklessRingBuffer::SetUp()
{}

void TestLocklessRingBuffer::TearDown()
{}

TEST_F(TestLocklessRingBuffer, TestGeneral)
{
    LocklessRingBuffer<int> buf(5);
    ASSERT_TRUE(buf.insert(1));
    ASSERT_TRUE(buf.insert(2));
    ASSERT_TRUE(buf.insert(3));
    ASSERT_TRUE(buf.insert(4));
    ASSERT_TRUE(buf.insert(5));
    ASSERT_FALSE(buf.insert(6));
    ASSERT_EQ(buf.next(), 1);
    ASSERT_TRUE(buf.hasItem());
    ASSERT_EQ(buf.next(), 2);
    ASSERT_TRUE(buf.hasItem());
    ASSERT_EQ(buf.next(), 3);
    ASSERT_TRUE(buf.hasItem());
    ASSERT_EQ(buf.next(), 4);
    ASSERT_TRUE(buf.hasItem());
    ASSERT_EQ(buf.next(), 5);
    ASSERT_FALSE(buf.hasItem());
}
