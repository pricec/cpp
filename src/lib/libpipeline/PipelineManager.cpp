#include "PipelineManager.hpp"
#include "libbuffer/BasicBufferSegmentAllocationStrategy.hpp"

using namespace pipeline;

PipelineManager::PipelineManager(
    std::vector<IPipelineStage *> stages,
    PipelineCallback cb
)
    : WorkItem(stages.size() + 1)
    , m_cb(cb)
    , m_stages(stages)
    , m_buffers()
    , m_active()
    , m_running()
    , m_wantExit(false)
{
    for (size_t i = 0; i < m_stages.size() + 1; ++i)
    {
        m_buffers.emplace_back(
            buffer::RingBuffer<PipelineStruct *>(m_bufSize)
        );
    }

    for (size_t i = 0; i < m_stages.size(); ++i)
    {
        m_running.push_back(true);
        defer(
            [this, i] ()
            {
                std::string name("stage" + i);
                runStage(name, m_stages[i], m_buffers[i], m_buffers[i+1]);
                m_running[i] = false;
                m_cv.notify_one();
            }
        );
    }

    m_running.push_back(true);
    defer(
        [this] ()
        {
            while (true)
            {
                m_buffers[m_buffers.size()-1].next();
                PipelineStruct ps(m_active.back());
                m_active.pop_back();
                if (m_wantExit)
                {
                    break;
                }
                if (m_cb)
                {
                    m_cb(ps);
                }
            }
            m_running[m_buffers.size()-1] = false;
            m_cv.notify_one();
        }
    );
}

PipelineManager::~PipelineManager()
{
    m_wantExit = true;
    buffer::Buffer buf;
    submit(buf);
    std::unique_lock<std::mutex> lock(m_mtx);
    m_cv.wait(
        lock,
        [this] ()
        {
            for (const auto &b : m_running)
            {
                if (b)
                {
                    return false;
                }
            }
            return true;
        }
    );
}

void PipelineManager::runStage(
    const std::string &stageName,
    IPipelineStage *stage,
    PipelineBuffer &input,
    PipelineBuffer &output
)
{
    while (!m_wantExit)
    {
        PipelineStruct *ps(input.next());
        if (!ps->m_dropped)
        {       
            ps->m_dropped = !stage->runStage(ps->m_buf);
        }
        if (!output.insert(ps))
        {
            // TODO: log this event
        }
    }
}

common::UUID PipelineManager::submit(const buffer::Buffer &buf)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    m_active.emplace_front(buf);
    PipelineStruct &item(m_active.front());
    m_buffers[0].insert(&item);
    return item.m_uuid;
}
