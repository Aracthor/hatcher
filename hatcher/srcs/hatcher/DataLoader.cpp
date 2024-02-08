#include "DataLoader.hpp"

namespace hatcher
{

DataLoader::DataLoader(const std::vector<ubyte>& data)
    : m_data(data)
{
}

void DataLoader::operator>>(std::string& string)
{
    int size;
    *this >> size;
    string.resize(size);
    for (char& value : string)
    {
        *this >> value;
    }
}

void DataLoader::LoadData(void* value, int size)
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
