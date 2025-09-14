#include "EntityDescriptorCatalog.hpp"

#include <stdexcept>

#include "EntityDescriptor.hpp"

namespace hatcher
{
EntityDescriptorCatalog::EntityDescriptorCatalog() = default;

EntityDescriptorCatalog::~EntityDescriptorCatalog() = default;

void EntityDescriptorCatalog::AddEntityDescriptor(EntityDescriptorID id, IEntityDescriptor* descriptor)
{
    if (m_catalog.find(id) != m_catalog.end())
        throw std::runtime_error("Duplicate EnttiyDescriptorID");
    m_catalog.emplace(id, descriptor);
}

const IEntityDescriptor* EntityDescriptorCatalog::GetDescriptor(EntityDescriptorID id) const
{
    if (m_catalog.find(id) == m_catalog.end())
        throw std::runtime_error("EntityDescriptorID not in catalog");
    return m_catalog.at(id).get();
}
} // namespace hatcher
