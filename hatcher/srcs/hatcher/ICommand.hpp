#pragma once

#include "span.hpp"

namespace hatcher
{
class ComponentManager;
class Entity;
class IEntityManager;

class ICommand
{
public:
    virtual ~ICommand() = default;

    virtual void Execute(IEntityManager* entityManager, ComponentManager* componentManager,
                         ComponentManager* renderingComponentManager) = 0;

    virtual span<const Entity> AffectedEntities() const { return {}; }
};

} // namespace hatcher
