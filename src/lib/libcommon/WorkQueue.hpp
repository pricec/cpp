/** \brief Basic variable-threaded workqueue
 *
 *  You will want to have one or more of these
 *  work queues as part of your application. They
 *  can be created with a variable number of
 *  threads. If you want an object to be single-
 *  threaded, then just inherit from IWorkItem
 *  and always queue the class up on a single-
 *  threaded WorkQueue.
 */
#pragma once
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>
#include <vector>

class IWorkItem;

namespace common
{

class WorkQueue
{
public:
    WorkQueue(uint8_t numThreads);
    ~WorkQueue();

    /**
     * Start the WorkQueue running. This means
     * workers will be running to service any
     * enqueued items. Returns true if the
     * WorkQueue is running and false otherwise.
     */
    bool start();

    /**
     * Stop the WorkQueue. This means instructing
     * all the workers currently servicing the
     * queue to exit and waiting for them to
     * exit. Returns true if the queue is stopped
     * and false otherwise.
     *
     * NOTE: The WorkQueue will be stopped and the
     *       items on the queue will be discarded!
     */
    bool stop();

    /**
     * Request an async stop of the WorkQueue. This
     * means asking the workers currently servicing
     * the queue to stop doing so. Returns true if
     * a stop has been requested and false otherwise.
     */
    bool requestStop();

    /**
     * Wait for a requested async stop. Returns true
     * if the stop was requested and the WorkQueue is
     * stopped and false otherwise.
     *
     * NOTE: The WorkQueue will be stopped and the
     *       items on the queue will be discarded!
     */
    bool waitStop();

    /**
     * Enqueue an IWorkItem onto the WorkQueue. Returns
     * true if the item was successfully enqueued and
     * false otherwise.
     */
    bool enqueue(IWorkItem &item);

private:
    void workLoop();

private:
    std::mutex m_mtx;
    std::deque<IWorkItem*> m_q;
    std::condition_variable m_cv;
    std::vector<std::thread> m_threads;
    uint8_t m_numThreads;
    bool m_running;
    bool m_exit;
}; // class WorkQueue

}; // namespace common
