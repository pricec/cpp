#include "BufferSegmentFactory.hpp"

using namespace buffer;

BufferSegmentFactory::BufferSegmentFactory(
    IBufferSegmentAllocationStrategy &strategy
)
    : m_strategy(strategy)
{}

BufferSegmentFactory::~BufferSegmentFactory()
{}

BufferSegment BufferSegmentFactory::allocate(size_t size)
{
    return BufferSegment(new DataSegment(m_strategy, size));
}

