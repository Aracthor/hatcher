#pragma once

namespace hatcher
{
class ComponentManager;
class IEntityManager;

class ICommand
{
public:
    virtual ~ICommand() = default;

    virtual void Execute(IEntityManager* entityManager, ComponentManager* componentManager,
                         ComponentManager* renderingComponentManager) = 0;
};

} // namespace hatcher
