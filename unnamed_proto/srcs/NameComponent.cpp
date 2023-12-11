#include "NameComponent.hpp"

#include "hatcher/ComponentLoader.hpp"
#include "hatcher/ComponentSaver.hpp"

using namespace hatcher;

void operator<<(ComponentSaver& saver, const NameComponent& component)
{
    saver << component.name;
}

void operator>>(ComponentLoader& loader, NameComponent& component)
{
    loader >> component.name;
}
