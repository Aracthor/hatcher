#pragma once

#include "ComponentManager.hpp"

namespace hatcher
{

class IComponentTypeCreator
{
public:
    virtual void CreateComponentType(ComponentManager* componentManager) const = 0;
};

void RegisterComponentTypeCreator(const IComponentTypeCreator* creator);
void RegisterRenderComponentTypeCreator(const IComponentTypeCreator* creator);
void RegisterWorldComponentTypeCreator(const IComponentTypeCreator* creator);
void RegisterRenderWorldComponentTypeCreator(const IComponentTypeCreator* creator);

template <class ComponentType>
class ComponentTypeRegisterer final : public IComponentTypeCreator
{
public:
    ComponentTypeRegisterer() { RegisterComponentTypeCreator(this); }

    void CreateComponentType(ComponentManager* componentManager) const override
    {
        componentManager->AddComponentType<ComponentType>();
    }
};

template <class ComponentType>
class RenderComponentTypeRegisterer final : public IComponentTypeCreator
{
public:
    RenderComponentTypeRegisterer() { RegisterRenderComponentTypeCreator(this); }

    void CreateComponentType(ComponentManager* componentManager) const override
    {
        componentManager->AddComponentType<ComponentType>();
    }
};

template <class ComponentType>
class WorldComponentTypeRegisterer final : public IComponentTypeCreator
{
public:
    WorldComponentTypeRegisterer() { RegisterWorldComponentTypeCreator(this); }

    void CreateComponentType(ComponentManager* componentManager) const override
    {
        componentManager->AddWorldComponent<ComponentType>();
    }
};

template <class ComponentType>
class RenderWorldComponentTypeRegisterer final : public IComponentTypeCreator
{
public:
    RenderWorldComponentTypeRegisterer() { RegisterRenderWorldComponentTypeCreator(this); }

    void CreateComponentType(ComponentManager* componentManager) const override
    {
        componentManager->AddWorldComponent<ComponentType>();
    }
};

} // namespace hatcher
