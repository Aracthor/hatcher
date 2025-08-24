#pragma once

#include <cstdint>

namespace hatcher
{
class DataLoader;
class DataSaver;

class IWorldComponent
{
public:
    virtual ~IWorldComponent() = default;

    virtual void Save(DataSaver& saver) const = 0;
    virtual void Load(DataLoader& loader) = 0;
};

} // namespace hatcher
