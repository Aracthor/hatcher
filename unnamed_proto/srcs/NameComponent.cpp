#include "NameComponent.hpp"

#include "hatcher/ISaveLoader.hpp"

using namespace hatcher;

void operator<<(ISaveLoader& saveLoader, NameComponent& component)
{
    saveLoader << component.name;
}
