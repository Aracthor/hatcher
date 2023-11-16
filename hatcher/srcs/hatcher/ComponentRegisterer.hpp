#pragma once

#include "ComponentManager.hpp"

namespace hatcher
{

namespace EComponentList
{
enum Type
{
    Gameplay,
    Rendering,
    COUNT
};
} // namespace EComponentList

class IComponentTypeCreator
{
public:
    virtual void CreateComponentType(ComponentManager* componentManager) const = 0;
};

void RegisterComponentTypeCreator(const IComponentTypeCreator* creator, EComponentList::Type type);
void RegisterWorldComponentTypeCreator(const IComponentTypeCreator* creator, EComponentList::Type type);

template <class ComponentType, EComponentList::Type list>
class ComponentTypeRegisterer final : public IComponentTypeCreator
{
public:
    ComponentTypeRegisterer() { RegisterComponentTypeCreator(this, list); }

    void CreateComponentType(ComponentManager* componentManager) const override
    {
        componentManager->AddComponentType<ComponentType>();
    }
};

template <class ComponentType, EComponentList::Type list>
class WorldComponentTypeRegisterer final : public IComponentTypeCreator
{
public:
    WorldComponentTypeRegisterer() { RegisterWorldComponentTypeCreator(this, list); }

    void CreateComponentType(ComponentManager* componentManager) const override
    {
        componentManager->AddWorldComponent<ComponentType>();
    }
};

} // namespace hatcher
