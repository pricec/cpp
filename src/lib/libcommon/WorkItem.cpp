#include "WorkItem.hpp"

using namespace common;

WorkItem::WorkItem(size_t n)
    : m_wq(new WorkQueue(n))
    , m_ownQ(true)
    , m_onQ(false)
{
    m_wq->start();
}

WorkItem::WorkItem(WorkQueue *wq)
    : m_wq(wq)
    , m_ownQ(false)
    , m_onQ(false)
{}

WorkItem::~WorkItem()
{
    if (m_ownQ)
    {
        m_wq->stop();
        delete m_wq;
    }
}

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
            m_wq->enqueue(*this);
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
        m_wq->enqueue(*this);
    }
    return true;
}

