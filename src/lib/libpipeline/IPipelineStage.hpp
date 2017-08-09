/** \brief Pipeline Stage interface
 *
 *  Implement this class to create a pipeline stage.
 */
#pragma once

namespace buffer
{
    class Buffer;
}

namespace pipeline
{

class IPipelineStage
{
public:
    virtual ~IPipelineStage() {};

    /*
     * Run the stage. This function should process and/or
     * modify the buffer in any way it wishes (according
     * to the Buffer class API). If your function returns
     * false, then the buffer will not be processed by
     * any further functions in the PipelineManager, and
     * the PipelineStruct passed to the callback will
     * have m_dropped set to true.
     */
    virtual bool runStage(buffer::Buffer &buf) = 0;
};

};
