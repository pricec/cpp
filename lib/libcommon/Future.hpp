#include "Event.hpp"

namespace common
{

template <typename T>
class Future
{
public:
  Future() {}

  void set(T value)
  {
    m_value = value;
    m_event.signal();
  }

  T value()
  {
    m_event.wait();
    return m_value;
  }

private:
  Event m_event;
  T m_value;
};

}; // namespace common
