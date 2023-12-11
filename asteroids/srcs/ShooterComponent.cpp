#include "ShooterComponent.hpp"

#include "hatcher/ComponentLoader.hpp"
#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ComponentSaver.hpp"

using namespace hatcher;

void operator<<(ComponentSaver& saver, const ShooterComponent& component)
{
    saver << component.shoots;
}

void operator>>(ComponentLoader& loader, ShooterComponent& component)
{
    loader >> component.shoots;
}
