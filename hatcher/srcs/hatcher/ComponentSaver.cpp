#include "ComponentSaver.hpp"

#include <iomanip>
#include <limits>

namespace hatcher
{

void ComponentSaver::separator(char c)
{
    AddSimpleData<>(c);
}

void ComponentSaver::operator<<(bool& value)
{
    AddSimpleData<>(value);
    separator(' ');
}

void ComponentSaver::operator<<(char& value)
{
    AddSimpleData<>(value);
}

void ComponentSaver::operator<<(ubyte& value)
{
    AddSimpleData<>(value);
    separator(' ');
}

void ComponentSaver::operator<<(int& value)
{
    AddSimpleData<>(value);
    separator(' ');
}

void ComponentSaver::operator<<(uint& value)
{
    AddSimpleData<>(value);
    separator(' ');
}

void ComponentSaver::operator<<(float& value)
{
    AddSimpleData<>(value);
    separator(' ');
}

const std::vector<ubyte>& ComponentSaver::Result() const
{
    return m_data;
}

} // namespace hatcher
