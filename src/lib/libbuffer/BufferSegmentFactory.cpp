#include "BufferSegmentFactory.hpp"

using namespace buffer;

BufferSegmentFactory::BufferSegmentFactory(
    IBufferSegmentAllocationStrategy &strategy
)
    : m_strategy(strategy)
{}

BufferSegmentFactory::~BufferSegmentFactory()
{}

ManagedBuffer BufferSegmentFactory::allocate(size_t size)
{
    return ManagedBuffer(new DataSegment(m_strategy, size));
}

