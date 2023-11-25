#include "Component.hpp"

namespace hatcher
{
template <class Component>
void ComponentSaver::SaveComponent(Component& component)
{
    uint key = ComponentKey<Component>();
    *this << key;
    *this << component;
}

template <typename T>
void ComponentSaver::AddSimpleData(T value)
{
    const std::size_t size = m_data.size();
    m_data.resize(size + sizeof(T));
    reinterpret_cast<T&>(m_data.data()[size]) = value;
}

} // namespace hatcher
