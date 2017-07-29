#pragma once
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

    /**
     * Returns true if the event has been signalled
     * and false otherwise. Note that wait() will not
     * block if and only if this function returns true.
     */
    bool signalled()
    {
        return m_signalled;
    }

    /**
     * Signal the event. This function is meant to
     * be called by the producer of the event. Returns
     * true if the event was signalled, and false if it
     * has already been signalled.
     */
    bool signal()
    {
        bool signalled;
        {
            std::lock_guard<std::mutex> lock(m_mtx);
            signalled = this->signalled();
            m_signalled = true;
        }
        m_cv.notify_all();
        return !signalled;
    }

    /**
     * Wait for the event to occur. This function
     * is meant to be used by the consumer of the
     * event and will block until the event occurs.
     */
    void wait()
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        m_cv.wait(lock, [this] () {
                return signalled();
            });
    }

private:
    std::mutex m_mtx;
    std::condition_variable m_cv;
    bool m_signalled;
};

}; // namespace common
