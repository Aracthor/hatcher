#include "NameComponent.hpp"

#include "hatcher/DataLoader.hpp"
#include "hatcher/DataSaver.hpp"

using namespace hatcher;

void operator<<(DataSaver& saver, const NameComponent& component)
{
    saver << component.name;
}

void operator>>(DataLoader& loader, NameComponent& component)
{
    loader >> component.name;
}
