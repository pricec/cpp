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



