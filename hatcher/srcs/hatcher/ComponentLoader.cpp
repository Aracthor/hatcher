#include "ComponentLoader.hpp"

namespace hatcher
{

ComponentLoader::ComponentLoader(const std::string& data)
    : m_stream(data)
{
}

void ComponentLoader::operator<<(char value)
{
    m_stream.ignore();
}

void ComponentLoader::operator<<(ubyte& value)
{
    m_stream >> value;
    m_stream.ignore();
}

void ComponentLoader::operator<<(bool& value)
{
    m_stream >> value;
    m_stream.ignore();
}

void ComponentLoader::operator<<(int& value)
{
    m_stream >> value;
    m_stream.ignore();
}

void ComponentLoader::operator<<(uint& value)
{
    m_stream >> value;
    m_stream.ignore();
}

void ComponentLoader::operator<<(float& value)
{
    m_stream >> value;
    m_stream.ignore();
}

} // namespace hatcher
