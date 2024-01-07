#pragma once

#include <string>

namespace hatcher
{
class ComponentLoader;
class ComponentSaver;
} // namespace hatcher

struct NameComponent
{
    std::string name;
};

void operator<<(hatcher::ComponentSaver& saver, const NameComponent& component);
void operator>>(hatcher::ComponentLoader& loader, NameComponent& component);
