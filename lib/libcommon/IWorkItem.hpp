/** \brief Work Item interface
 *
 *  Inherit from this class if you wish work in
 *  your class to be serviced by a generic WorkQueue.
 *  It is not necessary to provide any implementations;
 *  queue up your void function pointers using the
 *  enqueue function. Then queue your class up on
 *  a WorkQueue, and wait for the job to spool.
 *
 * NOTE: It is up to you to keep your IWorkItem in
 *       scope until either the job spools or the
 *       WorkQueue is stopped.
 */
#pragma once
#include <deque>
#include <functional>
#include <mutex>

namespace common
{

class IWorkItem
{
public:
  IWorkItem() {}
  IWorkItem(const IWorkItem &other)
  {
    *this = other;
  }

  virtual ~IWorkItem() {}

  virtual void doWork()
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

  virtual void enqueue(std::function<void(void)> f)
  {
    std::lock_guard<std::mutex> lock(m_mtx);
    m_q.push_front(f);
  }

  virtual IWorkItem& operator=(const IWorkItem& other)
  {
    std::deque< std::function<void(void)> > otherQ(other.m_q);
    {
      std::lock_guard<std::mutex> lock(m_mtx);
      m_q.swap(otherQ);
    }
    return *this;
  }

private:
  std::mutex m_mtx;
  std::deque< std::function<void(void)> > m_q;
}; // class IWorkItem

}; // namespace common

