#include <mutex>

#include "Event.hpp"

namespace common
{

template <typename T>
class Future
{
public:
    Future() {}

    /**
     * Set the value of the future. This function
     * is intended to be called by the producer
     * when computation of the value is complete.
     * Returns true if the value of the future was
     * successfully set and false otherwise.
     *
     * NOTE: the value of a future may only be set
     *       once, so subsequent calls to this function
     *       will return false.
     */
    bool set(T value)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        bool signalled = m_event.signalled();
        if (!signalled)
        {
            m_value = value;
            m_event.signal();
        }
        return !signalled;
    }

    /**
     * Cancel the future. This function is meant to
     * be called by a producer if the value cannot or
     * will not be computed. Note that this function
     * is idempotent. Returns true if this call or a
     * previous call has cancelled the future, and false
     * otherwise.
     *
     * NOTE: false will be returned in the case that
     *       the future value has already been set
     *       by a call to set().
     */
    bool cancel()
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        if (!m_event.signalled())
        {
            m_cancelled = true;
            m_event.signal();
        }
        return m_cancelled;
    }

    /**
     * Determine if a call to wait() will block. Returns
     * true if the call will return immediately and
     * false otherwise.
     */
    bool tryWait()
    {
        return m_event.signalled();
    }

    /**
     * Wait for the future to be computed. This function
     * will block until either the future value is set
     * or until the future is cancelled. Returns true if
     * the future value has been computed and false if
     * it has been cancelled.
     */
    bool wait()
    {
        m_event.wait();
        return !m_cancelled;
    }

    /**
     * Get the future value. Note that, before wait()
     * returns or if wait() returns false, then the
     * value will be a default-constructed T. After
     * wait() returns true, value() returns the
     * future value.
     */
    T value()
    {
        return m_value;
    }

private:
    std::mutex m_mtx;
    Event m_event;
    T m_value;
    bool m_cancelled;
};

}; // namespace common
