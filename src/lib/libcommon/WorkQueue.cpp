#include "IWorkItem.hpp"
#include "WorkQueue.hpp"

using namespace common;

WorkQueue::WorkQueue(uint8_t numThreads)
    : m_numThreads(numThreads)
    , m_running(false)
    , m_exit(false)
{
    m_threads.reserve(m_numThreads);
}

WorkQueue::~WorkQueue()
{
    stop();
}

bool WorkQueue::start()
{
    if (!m_running)
    {
        m_exit = false;
        for (int i = 0; i < m_numThreads; ++i)
        {
            m_threads.push_back(std::thread(&WorkQueue::workLoop, this));
        }
        m_running = true;
    }
    return m_running;
}

bool WorkQueue::stop()
{
    requestStop();
    return waitStop();
}

bool WorkQueue::requestStop()
{
    if (m_running)
    {
        m_exit = true;
        m_cv.notify_all();
    }
    return m_exit;
}

bool WorkQueue::waitStop()
{
    if (m_exit && m_running)
    {
        for (int i = 0; i < m_numThreads; ++i)
        {
            m_threads[i].join();
        }
        m_threads.clear();
        m_q.clear();
        m_running = false;
    }
    return m_exit;
}

bool WorkQueue::enqueue(IWorkItem &item)
{
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_q.push_front(&item);
    }
    m_cv.notify_one();
    return true;
}

void WorkQueue::workLoop()
{
    IWorkItem *work;
    while (true)
    {
        {
            std::unique_lock<std::mutex> lock(m_mtx);
            m_cv.wait(lock, [&] () {
                    return m_exit || !m_q.empty();
                });

            if (m_exit) break;

            work = m_q.back();
            m_q.pop_back();
        }
        work->doWork();
    }
}
