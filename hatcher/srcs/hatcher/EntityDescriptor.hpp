#pragma once

#include <memory>
#include <string>

#include "ComponentSaver.hpp"

namespace hatcher
{

class IEntityDescriptor
{
public:
    virtual ~IEntityDescriptor() = default;

    virtual const std::string& GetComponentData() const = 0;
    virtual const std::string& GetRenderingComponentData() const = 0;
};

class EntityDescriptorBuilder
{
public:
    template <class Component>
    void AddComponent(const Component& component)
    {
        Component copy = component;
        m_componentCount++;
        m_saver.SaveComponent(copy);
    }

    template <class Component>
    void AddRenderingComponent(const Component& component)
    {
        Component copy = component;
        m_renderingComponentCount++;
        m_renderingSaver.SaveComponent(copy);
    }

    std::unique_ptr<IEntityDescriptor> CreateDescriptor();

private:
    int m_componentCount = 0;
    int m_renderingComponentCount = 0;
    ComponentSaver m_saver;
    ComponentSaver m_renderingSaver;
};

} // namespace hatcher
