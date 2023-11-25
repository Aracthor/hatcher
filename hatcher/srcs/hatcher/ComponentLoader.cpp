#include "ComponentLoader.hpp"

namespace hatcher
{

namespace
{
template <typename T>
inline T ReadSimpleData(const std::vector<ubyte>& stream, int& index)
{
    const T result = *reinterpret_cast<const T*>(&stream.data()[index]);
    index += sizeof(T);
    return result;
}
} // namespace

ComponentLoader::ComponentLoader(const std::vector<ubyte>& data)
    : m_data(data)
{
}

void ComponentLoader::operator<<(bool& value)
{
    value = ReadSimpleData<bool>(m_data, m_currentIndex);
}

void ComponentLoader::operator<<(char& value)
{
    value = ReadSimpleData<char>(m_data, m_currentIndex);
}

void ComponentLoader::operator<<(ubyte& value)
{
    value = ReadSimpleData<ubyte>(m_data, m_currentIndex);
}

void ComponentLoader::operator<<(int& value)
{
    value = ReadSimpleData<int>(m_data, m_currentIndex);
}

void ComponentLoader::operator<<(uint& value)
{
    value = ReadSimpleData<uint>(m_data, m_currentIndex);
}

void ComponentLoader::operator<<(float& value)
{
    value = ReadSimpleData<float>(m_data, m_currentIndex);
}

} // namespace hatcher
