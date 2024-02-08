#pragma once

#include <string>

namespace hatcher
{
class DataLoader;
class DataSaver;
} // namespace hatcher

struct NameComponent
{
    std::string name;
};

void operator<<(hatcher::DataSaver& saver, const NameComponent& component);
void operator>>(hatcher::DataLoader& loader, NameComponent& component);
