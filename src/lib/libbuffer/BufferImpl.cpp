using namespace buffer;

template <typename T>
T* Buffer::getDataAs(
    BufferSegmentFactory &bufFac, size_t offset, size_t length
) {
    flatten(bufFac, offset, length);

    size_t startOffset = 0;
    for (size_t i = 0; i < m_numStatic; ++i)
    {
        const BufferSegmentHolder &bsh(m_bufs[i]);
        if (startOffset + bsh.m_length > offset)
        {
            return &bsh.m_bs->ptr<T>()[offset-startOffset+bsh.m_offset];
        }
        startOffset += bsh.m_length;
    }
    if (offset >= startOffset)
    {
        for (const auto &bsh : m_dbufs)
        {
            if (startOffset + bsh.m_length > offset)
            {
                return &bsh.m_bs->ptr<T>()[offset-startOffset+bsh.m_offset];
            }
            startOffset += bsh.m_length;
        }
    }
    return NULL;
}
