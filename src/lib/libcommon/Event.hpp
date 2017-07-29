#include <mutex>
#include <condition_variable>

namespace common
{

class Event
{
public:
  Event()
    : m_signalled(false)
  {}

  void signal()
  {
    m_signalled = true;
    m_cv.notify_one();
  }

  void wait()
  {
    std::unique_lock<std::mutex> lock(m_mtx);
    m_cv.wait(lock, [this] () {
      return m_signalled;
    });
  }

private:
  std::mutex m_mtx;
  std::condition_variable m_cv;
  bool m_signalled;
};

}; // namespace common
