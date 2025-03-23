#pragma once

#include "basic_types.hpp"
#include "span.hpp"

namespace hatcher
{
class ComponentManager;
class DataLoader;
class DataSaver;
class Entity;
class IEntityManager;

class ICommand
{
public:
    virtual ~ICommand() = default;

    virtual uint Identifier() const = 0;
    virtual void Save(DataSaver& saver) const {}
    virtual void Load(DataLoader& loader) {}

    virtual void Execute(IEntityManager* entityManager, ComponentManager* componentManager,
                         ComponentManager* renderingComponentManager) = 0;
};

} // namespace hatcher
