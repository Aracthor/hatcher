#include "Movement2DComponent.hpp"

#include "hatcher/ComponentLoader.hpp"
#include "hatcher/ComponentSaver.hpp"

void operator<<(ComponentSaver& saver, const Movement2DComponent& component)
{
    saver << component.path;
}

void operator>>(ComponentLoader& loader, Movement2DComponent& component)
{
    loader >> component.path;
}
