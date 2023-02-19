#include "ComponentSaver.hpp"

namespace hatcher
{

void ComponentSaver::operator<<(float& value)
{
    m_stream << value;
    m_stream << '\n';
}

void ComponentSaver::operator<<(glm::vec3& value)
{
    m_stream << value.x;
    m_stream << ' ';
    m_stream << value.y;
    m_stream << ' ';
    m_stream << value.z;
    m_stream << '\n';
}

std::string ComponentSaver::Result() const
{
    return m_stream.str();
}

} // namespace hatcher
