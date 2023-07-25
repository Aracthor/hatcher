#include "Component.hpp"

namespace hatcher
{
template <class Component>
void ComponentSaver::SaveComponent(Component& component)
{
    uint key = ComponentKey<Component>();
    *this << key;
    separator('\n');
    *this << component;
    separator('\n');
}
} // namespace hatcher
