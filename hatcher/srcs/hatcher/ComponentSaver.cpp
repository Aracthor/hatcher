#include "ComponentSaver.hpp"

#include <iomanip>
#include <limits>

namespace hatcher
{

void ComponentSaver::operator<<(bool& value)
{
    AddSimpleData<>(value);
}

void ComponentSaver::operator<<(char& value)
{
    AddSimpleData<>(value);
}

void ComponentSaver::operator<<(ubyte& value)
{
    AddSimpleData<>(value);
}

void ComponentSaver::operator<<(int& value)
{
    AddSimpleData<>(value);
}

void ComponentSaver::operator<<(uint& value)
{
    AddSimpleData<>(value);
}

void ComponentSaver::operator<<(float& value)
{
    AddSimpleData<>(value);
}

const std::vector<ubyte>& ComponentSaver::Result() const
{
    return m_data;
}

} // namespace hatcher
