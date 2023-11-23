#include "EntityDescriptorCatalog.hpp"

#include <iostream>

#include "EntityDescriptor.hpp"

namespace hatcher
{
EntityDescriptorCatalog::EntityDescriptorCatalog() = default;

EntityDescriptorCatalog::~EntityDescriptorCatalog() = default;

void EntityDescriptorCatalog::AddEntityDescriptor(EntityDescriptorID id, IEntityDescriptor* descriptor)
{
    if (m_catalog.find(id) != m_catalog.end())
    {
        std::cerr << "EntityDescriptorID doublon: '" << id.Name() << "'" << std::endl;
        std::abort();
    }
    m_catalog.emplace(id, descriptor);
}

const IEntityDescriptor* EntityDescriptorCatalog::GetDescriptor(EntityDescriptorID id) const
{
    if (m_catalog.find(id) == m_catalog.end())
    {
        std::cerr << "EntityDescriptorID not in catalog: '" << id.Name() << "'" << std::endl;
        std::abort();
    }
    return m_catalog.at(id).get();
}
} // namespace hatcher
