#include "WorkItem.hpp"

using namespace common;

WorkItem::WorkItem(WorkQueue &wq)
  : m_wq(wq)
{}

WorkItem::~WorkItem()
{}

void WorkItem::doWork()
{
  std::function<void(void)> work([](){});
  {
    std::lock_guard<std::mutex> lock(m_mtx);
    if (!m_q.empty())
    {
      work = m_q.back();
      m_q.pop_back();
    }
  }
  work();
}

bool WorkItem::enqueue(std::function<void(void)> f)
{
  std::lock_guard<std::mutex> lock(m_mtx);
  m_q.push_front(f);
  m_wq.enqueue(*this);
  return true;
}

