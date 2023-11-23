#pragma once

#include <unordered_map>

#include "EntityDescriptorID.hpp"
#include "unique_ptr.hpp"

namespace hatcher
{
class IEntityDescriptor;

class EntityDescriptorCatalog
{
public:
    EntityDescriptorCatalog();
    ~EntityDescriptorCatalog();

    void AddEntityDescriptor(EntityDescriptorID id, IEntityDescriptor* descriptor);

    const IEntityDescriptor* GetDescriptor(EntityDescriptorID id) const;

private:
    std::unordered_map<EntityDescriptorID, unique_ptr<IEntityDescriptor>, EntityDescriptorID::Hasher> m_catalog;
};

} // namespace hatcher
