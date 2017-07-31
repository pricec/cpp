#include "WorkItem.hpp"

using namespace common;

WorkItem::WorkItem(WorkQueue &wq)
    : m_wq(wq)
    , m_onQ(false)
{}

WorkItem::~WorkItem()
{}

void WorkItem::run()
{
    std::function<void(void)> work([](){});
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        if (!m_q.empty())
        {
            work = m_q.back();
            m_q.pop_back();
        }
        if (m_q.empty())
        {
            m_onQ = false;
        }
        else
        {
            m_wq.enqueue(*this);
        }
    }
    work();
}

bool WorkItem::defer(std::function<void(void)> f)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    m_q.push_front(f);
    if (!m_onQ)
    {
        m_onQ = true;
        m_wq.enqueue(*this);
    }
    return true;
}

