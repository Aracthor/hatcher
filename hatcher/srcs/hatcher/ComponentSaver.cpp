#include "ComponentSaver.hpp"

namespace hatcher
{

void ComponentSaver::operator<<(char value)
{
    m_stream << value;
}

void ComponentSaver::operator<<(bool& value)
{
    m_stream << value;
    m_stream << '\n';
}

void ComponentSaver::operator<<(ubyte& value)
{
    m_stream << value;
    m_stream << '\n';
}

void ComponentSaver::operator<<(int& value)
{
    m_stream << value;
    m_stream << '\n';
}

void ComponentSaver::operator<<(uint& value)
{
    m_stream << value;
    m_stream << '\n';
}

void ComponentSaver::operator<<(float& value)
{
    m_stream << value;
    m_stream << '\n';
}

std::string ComponentSaver::Result() const
{
    return m_stream.str();
}

} // namespace hatcher
