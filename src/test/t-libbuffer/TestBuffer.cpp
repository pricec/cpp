#include "TestBuffer.hpp"
#include "libbuffer/Buffer.hpp"
#include "libbuffer/BasicBufferSegmentAllocationStrategy.hpp"
#include "libbuffer/BufferSegmentFactory.hpp"
#include <thread>
#include <chrono>

using namespace buffer;

void TestBuffer::SetUp()
{}

void TestBuffer::TearDown()
{}

TEST_F(TestBuffer, TestSingleAppend)
{
    std::string str("dfahlsjfhsad");
    BasicBufferSegmentAllocationStrategy strategy;
    BufferSegmentFactory bufFac(strategy);
    Buffer m;
    std::shared_ptr<DataSegment> mb(bufFac.allocate(str.length()));
    memcpy(mb->ptr<void>(), str.c_str(), str.length());
    m.append(mb);
    for (size_t i = 0; i < str.length(); ++i)
    {
        ASSERT_EQ(m.getByte(i), str[i]);
    }
}

TEST_F(TestBuffer, TestManyAppend)
{
    const char *str = "dafhldsahf";
    const char *strStart = str;
    size_t len = strlen(str);
    BasicBufferSegmentAllocationStrategy strategy;
    BufferSegmentFactory bufFac(strategy);
    Buffer m;
    for (size_t i = 0; i < len; ++i)
    {
        std::shared_ptr<DataSegment> mb(bufFac.allocate(1));
        memcpy(mb->ptr<void>(), str++, 1);
        m.append(mb);
    }
    for (size_t i = 0; i < len; ++i)
    {
        ASSERT_EQ(m.getByte(i), strStart[i]);
    }
}

TEST_F(TestBuffer, TestSingleGetData)
{
    std::string str("dfahlsjfhsad");
    BasicBufferSegmentAllocationStrategy strategy;
    BufferSegmentFactory bufFac(strategy);
    Buffer m;
    std::shared_ptr<DataSegment> mb(bufFac.allocate(str.length()));
    memcpy(mb->ptr<void>(), str.c_str(), str.length());
    m.append(mb);
    for (size_t i = 0; i < str.length(); ++i)
    {
        for (size_t j = 0; i+j < str.length(); ++j)
        {
            Buffer m2(m.getData(i, j));
            for (size_t k = 0; k < j; ++k)
            {
                ASSERT_EQ(m2.getByte(k), str[i+k]);
            }
        }
    }
}

TEST_F(TestBuffer, TestManyGetData)
{
    const char *str = "dafhldsahf";
    size_t len = strlen(str);
    BasicBufferSegmentAllocationStrategy strategy;
    BufferSegmentFactory bufFac(strategy);
    Buffer m;
    for (size_t i = 0; i < len; ++i)
    {
        std::shared_ptr<DataSegment> mb(bufFac.allocate(1));
        memcpy(mb->ptr<void>(), str+i, 1);
        m.append(mb);
    }
    for (size_t i = 0; i < len; ++i)
    {
        for (size_t j = 0; i+j < len; ++j)
        {
            Buffer m2(m.getData(i, j));
            for (size_t k = 0; k < j; ++k)
            {
                ASSERT_EQ(m2.getByte(k), m.getByte(i+k));
            }
        }
    }
}
