#pragma once
#include <deque>
#include <mutex>

#include "IWorkItem.hpp"
#include "WorkQueue.hpp"

namespace common
{

class WorkItem : public IWorkItem
{
public:
    WorkItem(int n);
    WorkItem(WorkQueue *wq);
    ~WorkItem();

    void run() override;
    bool defer(std::function<void(void)> f) override;

private:
    std::mutex m_mtx;
    std::deque< std::function<void(void)> > m_q;
    WorkQueue *m_wq;
    bool m_ownQ;
    bool m_onQ;
}; // name WorkItem

}; // namespace common
