#include "ComponentLoader.hpp"

namespace hatcher
{

ComponentLoader::ComponentLoader(const std::vector<ubyte>& data)
    : m_data(data)
{
}

void ComponentLoader::operator>>(std::string& string)
{
    int size;
    *this >> size;
    string.resize(size);
    for (char& value : string)
    {
        *this >> value;
    }
}

void ComponentLoader::LoadData(void* value, int size)
{
    ubyte* bytes = reinterpret_cast<ubyte*>(value);
    while (size-- > 0)
    {
        *bytes = m_data[m_currentIndex];
        bytes++;
        m_currentIndex++;
    }
}

} // namespace hatcher
