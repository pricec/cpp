#include "TestWorkQueue.hpp"
#include "libcommon/WorkQueue.hpp"
#include "libcommon/WorkItem.hpp"
#include <thread>

void TestWorkQueue::SetUp()
{}

void TestWorkQueue::TearDown()
{}

TEST_F(TestWorkQueue, TestJobsAreExecuted)
{
    int n = 10;
    std::mutex mtx;
    std::condition_variable cv;
    common::WorkQueue wq(1);
    common::WorkItem wi(&wq);
    int waiting = n;
    for (int i = 0; i < n; ++i)
    {
        wi.defer(
            [&mtx, &cv, &waiting] ()
            {
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    --waiting;
                }
                cv.notify_one();
            }
        );
    }

    wq.start();
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(
        lock,
        [&waiting] ()
        {
            return waiting == 0;
        }
    );
}

TEST_F(TestWorkQueue, TestJobsExecutedInParallel)
{
    std::mutex mtx;
    std::condition_variable cv;
    bool notified[2] = {false, false};
    common::WorkItem wi(2);
    wi.defer(
        [&mtx, &cv, &notified] ()
        {
            notified[0] = true;
            cv.notify_all();
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(
                    lock,
                    [&notified] ()
                    {
                        return notified[1];
                    }
                );
            }
        }
    );
    wi.defer(
        [&mtx, &cv, &notified] ()
        {
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(
                    lock,
                    [&notified] ()
                    {
                        return notified[0];
                    }
                );
            }
            notified[1] = true;
            cv.notify_all();
        }
    );

    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(
        lock,
        [&notified] ()
        {
            return notified[0] && notified[1];
        }
    );
}
