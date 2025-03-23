#pragma once

#include "basic_types.hpp"
#include "constexpr_hash.hpp"

namespace hatcher
{
class EntityDescriptorID
{
public:
    EntityDescriptorID() = default;

    template <int N>
    static constexpr EntityDescriptorID Create(const char (&name)[N])
    {
        return EntityDescriptorID(constexpr_hash<N>(name));
    }

    bool operator==(const EntityDescriptorID& other) const { return m_id == other.m_id; }

    struct Hasher
    {
        std::size_t operator()(const EntityDescriptorID& k) const { return k.m_id; }
    };

private:
    constexpr EntityDescriptorID(uint id)
        : m_id(id)
    {
    }

    uint m_id = 0;
};

} // namespace hatcher
