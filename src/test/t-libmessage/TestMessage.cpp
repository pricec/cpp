#include "TestMessage.hpp"
#include "libmessage/Message.hpp"
#include "libmessage/BasicMessageBufferAllocationStrategy.hpp"
#include "libmessage/MessageBufferFactory.hpp"
#include <thread>
#include <chrono>

using namespace message;

void TestMessage::SetUp()
{}

void TestMessage::TearDown()
{}

TEST_F(TestMessage, TestSingleAppend)
{
    std::string str("dfahlsjfhsad");
    BasicMessageBufferAllocationStrategy strategy;
    MessageBufferFactory bufFac(strategy);
    Message m;
    std::shared_ptr<MessageBuffer> mb(bufFac.allocate(str.length()));
    memcpy(mb->ptr<void>(), str.c_str(), str.length());
    m.append(mb);
    for (size_t i = 0; i < str.length(); ++i)
    {
        ASSERT_EQ(m.getByte(i), str[i]);
    }
}

TEST_F(TestMessage, TestManyAppend)
{
    const char *str = "dafhldsahf";
    const char *strStart = str;
    size_t len = strlen(str);
    BasicMessageBufferAllocationStrategy strategy;
    MessageBufferFactory bufFac(strategy);
    Message m;
    for (size_t i = 0; i < len; ++i)
    {
        std::shared_ptr<MessageBuffer> mb(bufFac.allocate(1));
        memcpy(mb->ptr<void>(), str++, 1);
        m.append(mb);
    }
    for (size_t i = 0; i < len; ++i)
    {
        ASSERT_EQ(m.getByte(i), strStart[i]);
    }
}

TEST_F(TestMessage, TestSingleGetData)
{
    std::string str("dfahlsjfhsad");
    BasicMessageBufferAllocationStrategy strategy;
    MessageBufferFactory bufFac(strategy);
    Message m;
    std::shared_ptr<MessageBuffer> mb(bufFac.allocate(str.length()));
    memcpy(mb->ptr<void>(), str.c_str(), str.length());
    m.append(mb);
    for (size_t i = 0; i < str.length(); ++i)
    {
        for (size_t j = 0; i+j < str.length(); ++j)
        {
            Message m2(m.getData(i, j));
            for (size_t k = 0; k < j; ++k)
            {
                ASSERT_EQ(m2.getByte(k), str[i+k]);
            }
        }
    }
}

TEST_F(TestMessage, TestManyGetData)
{
    const char *str = "dafhldsahf";
    size_t len = strlen(str);
    BasicMessageBufferAllocationStrategy strategy;
    MessageBufferFactory bufFac(strategy);
    Message m;
    for (size_t i = 0; i < len; ++i)
    {
        std::shared_ptr<MessageBuffer> mb(bufFac.allocate(1));
        memcpy(mb->ptr<void>(), str+i, 1);
        m.append(mb);
    }
    for (size_t i = 0; i < len; ++i)
    {
        for (size_t j = 0; i+j < len; ++j)
        {
            Message m2(m.getData(i, j));
            for (size_t k = 0; k < j; ++k)
            {
                ASSERT_EQ(m2.getByte(k), m.getByte(i+k));
            }
        }
    }
}
