#include "ComponentSaver.hpp"

namespace hatcher
{

void ComponentSaver::SaveLoadData(void* data, int size)
{
    std::size_t currentSize = m_data.size();
    m_data.resize(currentSize + size);

    ubyte* bytes = reinterpret_cast<ubyte*>(data);
    while (size-- > 0)
    {
        m_data[currentSize] = *bytes;
        bytes++;
        currentSize++;
    }
}

const std::vector<ubyte>& ComponentSaver::Result() const
{
    return m_data;
}

} // namespace hatcher
