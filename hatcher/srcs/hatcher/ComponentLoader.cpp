#include "ComponentLoader.hpp"

#include <iomanip>
#include <limits>

namespace hatcher
{

ComponentLoader::ComponentLoader(const std::string& data)
    : m_stream(data)
{
    m_stream >> std::setprecision(std::numeric_limits<float>::digits);
}

void ComponentLoader::separator(char c)
{
    m_stream.ignore();
}

void ComponentLoader::operator<<(bool& value)
{
    m_stream >> value;
    separator(' ');
}

void ComponentLoader::operator<<(char& value)
{
    value = m_stream.get();
}

void ComponentLoader::operator<<(ubyte& value)
{
    m_stream >> value;
    separator(' ');
}

void ComponentLoader::operator<<(int& value)
{
    m_stream >> value;
    separator(' ');
}

void ComponentLoader::operator<<(uint& value)
{
    m_stream >> value;
    separator(' ');
}

void ComponentLoader::operator<<(float& value)
{
    m_stream >> value;
    separator(' ');
}

} // namespace hatcher
