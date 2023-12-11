#pragma once

namespace hatcher
{
class ComponentLoader;
class ComponentSaver;

class IWorldComponent
{
public:
    virtual ~IWorldComponent() = default;

    virtual void Save(ComponentSaver& saver) const = 0;
    virtual void Load(ComponentLoader& loader) = 0;
};

} // namespace hatcher
