#pragma once

#include "ComponentManager.hpp"

namespace hatcher
{

enum class EComponentList
{
    Gameplay,
    Rendering,
    COUNT
};

class IComponentTypeCreator
{
public:
    virtual void CreateComponentType(ComponentManager* componentManager) const = 0;
};

void RegisterComponentTypeCreator(const IComponentTypeCreator* creator, EComponentList type);
void RegisterWorldComponentTypeCreator(const IComponentTypeCreator* creator, EComponentList type);

template <class ComponentType, EComponentList list>
class ComponentTypeRegisterer final : public IComponentTypeCreator
{
public:
    ComponentTypeRegisterer() { RegisterComponentTypeCreator(this, list); }

    void CreateComponentType(ComponentManager* componentManager) const override
    {
        componentManager->AddComponentType<ComponentType>();
    }
};

template <class ComponentType, EComponentList list>
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
