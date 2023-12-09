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

} // namespace hatcher
