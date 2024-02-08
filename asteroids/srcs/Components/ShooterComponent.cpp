#include "ShooterComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/DataLoader.hpp"
#include "hatcher/DataSaver.hpp"

using namespace hatcher;

void operator<<(DataSaver& saver, const ShooterComponent& component)
{
    saver << component.shoots;
}

void operator>>(DataLoader& loader, ShooterComponent& component)
{
    loader >> component.shoots;
}
