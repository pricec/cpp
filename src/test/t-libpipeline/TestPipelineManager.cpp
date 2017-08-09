#include "TestPipelineManager.hpp"
#include "libbuffer/Buffer.hpp"
#include "libbuffer/BufferSegmentFactory.hpp"
#include "libbuffer/BasicBufferSegmentAllocationStrategy.hpp"
#include "libpipeline/IPipelineStage.hpp"
#include "libpipeline/PipelineManager.hpp"

#include <chrono>
#include <thread>

using namespace pipeline;

void TestPipelineManager::SetUp()
{}

void TestPipelineManager::TearDown()
{}

TEST_F(TestPipelineManager, TestZeroStages)
{
    std::vector<IPipelineStage *> stages;
    bool done = false;
    std::condition_variable cv;
    PipelineManager mgr(
        stages,
        [&done, &cv] (PipelineStruct ps)
        {
            done = true;
            cv.notify_one();
        }
    );

    buffer::BasicBufferSegmentAllocationStrategy strategy;
    buffer::BufferSegmentFactory fac(strategy);
    buffer::Buffer buf;
    mgr.submit(buf);

    std::mutex mtx;
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(
        lock,
        [&done] ()
        {
            return done;
        }
    );
}


TEST_F(TestPipelineManager, TestOneStage)
{
    class Stage1 : public IPipelineStage
    {
    public:
        Stage1()
            : m_strategy()
            , m_fac(m_strategy)
        {}

        bool runStage(buffer::Buffer &buf) override
        {
            buffer::BufferSegment bs(m_fac.allocate(1));
            bs->ptr<char>()[0] = 'a';
            buf.append(bs);
            return true;
        }
    private:
        buffer::BasicBufferSegmentAllocationStrategy m_strategy;
        buffer::BufferSegmentFactory m_fac;
    };

    Stage1 stage;
    std::vector<IPipelineStage *> stages;
    stages.push_back(&stage);

    bool done = false;
    std::condition_variable cv;
    PipelineManager mgr(
        stages,
        [&done, &cv] (PipelineStruct ps)
        {
            ASSERT_EQ('a', ps.m_buf.getByte(0));
            done = true;
            cv.notify_one();
        }
    );


    buffer::BasicBufferSegmentAllocationStrategy strategy;
    buffer::BufferSegmentFactory fac(strategy);
    buffer::Buffer buf;
    mgr.submit(buf);

    std::mutex mtx;
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(
        lock,
        [&done] ()
        {
            return done;
        }
    );
}
