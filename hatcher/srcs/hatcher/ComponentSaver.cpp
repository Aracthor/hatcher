#include "ComponentSaver.hpp"

namespace hatcher
{

void ComponentSaver::operator<<(const std::string& string)
{
    int size = string.size();
    *this << size;
    for (const char value : string)
        *this << value;
}

void ComponentSaver::SaveData(const void* data, int size)
{
    std::size_t currentSize = m_data.size();
    m_data.resize(currentSize + size);

    const ubyte* bytes = reinterpret_cast<const ubyte*>(data);
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
