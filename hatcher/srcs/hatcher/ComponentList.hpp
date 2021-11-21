#pragma once

#include <optional>
#include <span>
#include <vector>

#include "basic_types.hpp"

namespace hatcher
{

class IComponentList
{
public:
    virtual void AddEntities(int count) = 0;

    virtual ~IComponentList() = default;
};

template <class Component>
class IdentifiableComponentList final : public IComponentList
{
public:
    void AddEntities(int count) override;

    void AttachComponent(uint index, Component& component);
    std::span<const std::optional<Component>> GetComponentList() const;

private:
    std::vector<std::optional<Component>> m_components;
};

} // namespace hatcher

#include "ComponentList.ipp"
