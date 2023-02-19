#include "ComponentLoader.hpp"

namespace hatcher
{

ComponentLoader::ComponentLoader(const std::string& data)
    : m_stream(data)
{
}

void ComponentLoader::operator<<(float& value)
{
    m_stream >> value;
    m_stream.ignore();
}

void ComponentLoader::operator<<(glm::vec3& value)
{
    m_stream >> value.x;
    m_stream.ignore();
    m_stream >> value.y;
    m_stream.ignore();
    m_stream >> value.z;
    m_stream.ignore();
}

} // namespace hatcher
