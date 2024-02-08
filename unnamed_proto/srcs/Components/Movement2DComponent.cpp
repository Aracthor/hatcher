#include "Movement2DComponent.hpp"

#include "hatcher/DataLoader.hpp"
#include "hatcher/DataSaver.hpp"

void operator<<(DataSaver& saver, const Movement2DComponent& component)
{
    saver << component.path;
}

void operator>>(DataLoader& loader, Movement2DComponent& component)
{
    loader >> component.path;
}
