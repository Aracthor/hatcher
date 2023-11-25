#pragma once

#include <vector>

#include "ComponentSaver.hpp"
#include "EntityDescriptorID.hpp"

namespace hatcher
{

class IEntityDescriptor
{
public:
    virtual ~IEntityDescriptor() = default;

    virtual const std::vector<ubyte>& GetComponentData() const = 0;
    virtual const std::vector<ubyte>& GetRenderingComponentData() const = 0;
};

class ComponentDescriptorList
{
public:
    ComponentDescriptorList() = default;

    template <class Component, class... Others>
    ComponentDescriptorList(const Component& component, Others... others)
        : ComponentDescriptorList(others...)
    {
        m_componentCount++;
        Component copy = component;
        m_saver.SaveComponent(copy);
    }

    std::vector<ubyte> Result() const;

private:
    int m_componentCount = 0;
    ComponentSaver m_saver;
};

struct EntityDescriptorRegisterer
{
    EntityDescriptorRegisterer(EntityDescriptorID id, const ComponentDescriptorList& componentList,
                               const ComponentDescriptorList& renderingComponentList);
};

} // namespace hatcher
