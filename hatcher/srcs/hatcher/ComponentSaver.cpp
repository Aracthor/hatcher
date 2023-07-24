#include "ComponentSaver.hpp"

#include <iomanip>
#include <limits>

namespace hatcher
{

ComponentSaver::ComponentSaver()
{
    m_stream << std::setprecision(std::numeric_limits<float>::digits);
}

void ComponentSaver::separator(char c)
{
    m_stream << c;
}

void ComponentSaver::operator<<(bool& value)
{
    m_stream << value;
    separator(' ');
}

void ComponentSaver::operator<<(ubyte& value)
{
    m_stream << value;
    separator(' ');
}

void ComponentSaver::operator<<(int& value)
{
    m_stream << value;
    separator(' ');
}

void ComponentSaver::operator<<(uint& value)
{
    m_stream << value;
    separator(' ');
}

void ComponentSaver::operator<<(float& value)
{
    m_stream << value;
    separator(' ');
}

std::string ComponentSaver::Result() const
{
    return m_stream.str();
}

} // namespace hatcher
