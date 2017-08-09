/* \brief Pipeline Runner
 *
 *  This class exists to contain an execution pipeline;
 *  a pipeline is a series of functions called 'stages,'
 *  each of which receives input from the previous (or
 *  from the producer, for the first stage), and produces
 *  output for the next stage (or the consumer, for the
 *  last stage). This manager runs each function and the
 *  callback function in its own thread to maximize
 *  throughput.
 */
#pragma once

#include "libbuffer/Buffer.hpp"
#include "libbuffer/RingBuffer.hpp"
#include "libcommon/WorkItem.hpp"
#include "libcommon/WorkQueue.hpp"
#include "libcommon/UUID.hpp"
#include "libpipeline/IPipelineStage.hpp"

namespace pipeline
{

/*
 * This struct exists just to carry metadata down
 * the pipline. It holds the UUID returned to the
 * caller of the submit function (see below), so
 * that the caller can asynchronously receive the
 * callback and identify which buffer it originally
 * submitted.
 */
using PipelineStruct =
struct PipelineStruct
{
    PipelineStruct()
        : m_uuid(0, 0)
        , m_buf()
        , m_dropped(false)
    {}

    PipelineStruct(const buffer::Buffer &buf)
        : m_uuid()
        , m_buf(buf)
        , m_dropped(false)
    {}

    PipelineStruct(const PipelineStruct &other)
        : m_uuid(other.m_uuid)
        , m_buf(other.m_buf)
        , m_dropped(other.m_dropped)
    {}

    common::UUID m_uuid;
    buffer::Buffer m_buf;
    bool m_dropped;
};

using PipelineCallback = std::function<void(PipelineStruct)>;
using PipelineBuffer = buffer::RingBuffer<PipelineStruct *>;

/*
 * The PipelineManager implements IWorkItem by way of
 * WorkItem; it uses a WorkQueue consisting of # of
 * pipeline stages + 1 worker threads in order to
 * dedicate a thread to each stage.
 */
class PipelineManager : public common::WorkItem
{
public:
    /*
     * Create the PipelineManager and start it running.
     * Obviously, it is up to you to keep the IPipelineStages
     * in scope as long as the manager is alive. The callback
     * will be invoked once for each submitted buffer. It is up
     * to you to handle the case where the buffer was dropped.
     */
    PipelineManager(std::vector<IPipelineStage *> stages, PipelineCallback cb);

    /*
     * The destructor stops the pipeline. Note that any submitted
     * buffers which have not egressed te pipeline may be dropped.
     */
    ~PipelineManager();

    /*
     * Submit a buffer for processing by the pipeline. This
     * method returns a UUID which identifies the buffer. When
     * you receive a callback, the m_uuid field of the corresponding
     * PipelineStruct will be populated with this value.
     */
    common::UUID submit(const buffer::Buffer &buf);

private:
    void runStage(
        const std::string &stageName,
        IPipelineStage *stage,
        PipelineBuffer &input,
        PipelineBuffer &output
    );

private:
    std::mutex m_mtx;
    std::condition_variable m_cv;
    PipelineCallback m_cb;
    std::vector<IPipelineStage *> m_stages;
    std::vector<PipelineBuffer> m_buffers;
    std::deque<PipelineStruct> m_active;
    std::vector<bool> m_running;
    bool m_wantExit;
    static const size_t m_bufSize = 100;
};

}; // namespace pipeline
