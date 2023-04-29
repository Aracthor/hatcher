#pragma once

#include <optional>
#include <vector>

#include "basic_types.hpp"
#include "span.hpp"

namespace hatcher
{
class ISaveLoader;

class IComponentList
{
public:
    virtual void AddEntities(int count) = 0;
    virtual void RemoveEntity(uint index) = 0;
    virtual void ClearEntities() = 0;
    virtual bool HasComponent(uint index) const = 0;
    virtual void SaveLoad(uint index, ISaveLoader& saveLoader) = 0;

    virtual ~IComponentList() = default;
};

template <class Component>
class IdentifiableComponentList final : public IComponentList
{
public:
    void AddEntities(int count) override;
    void RemoveEntity(uint index) override;
    void ClearEntities() override;
    bool HasComponent(uint index) const override;
    void SaveLoad(uint index, ISaveLoader& saveLoader) override;

    void AttachComponent(uint index, Component& component);
    span<const std::optional<Component>> GetComponentList() const;
    span<std::optional<Component>> GetComponentList();

private:
    std::vector<std::optional<Component>> m_components;
};

} // namespace hatcher

#include "ComponentList.ipp"
