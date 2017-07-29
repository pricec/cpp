#include "TestEvent.hpp"
#include "libcommon/Event.hpp"
#include <thread>

void TestEvent::SetUp()
{}

void TestEvent::TearDown()
{}

TEST_F(TestEvent, TestBankedSignal)
{
  common::Event e;
  e.signal();
  e.wait();
}

TEST_F(TestEvent, TestWaitThenSignal)
{
  common::Event e;
  std::thread t([&e](){
      e.wait(); 
  });
  e.signal();
  t.join();
}


