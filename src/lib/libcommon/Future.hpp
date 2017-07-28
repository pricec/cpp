#include "Event.hpp"

namespace common
{

template <typename T>
class Future
{
public:
  Future() {}

  bool set(T value)
  {
    m_value = value;
    m_event.signal();
  }

  bool cancel()
  {
    m_cancelled = true;
    m_event.signal();
  }

  bool wait()
  {
    m_event.wait();
    return !m_cancelled;
  }

  T value()
  {
    return m_value;
  }

private:
  Event m_event;
  T m_value;
  bool m_cancelled;
};

}; // namespace common
