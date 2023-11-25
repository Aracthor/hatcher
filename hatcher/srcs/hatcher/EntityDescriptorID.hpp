#pragma once

#include "basic_types.hpp"
#include "constexpr_hash.hpp"

namespace hatcher
{
class EntityDescriptorID
{
public:
    EntityDescriptorID()
        : m_id(0)
        , m_name("##INVALID")
    {
    }

    template <int N>
    static constexpr EntityDescriptorID Create(const char (&name)[N])
    {
        return EntityDescriptorID(constexpr_hash<N>(name), name);
    }

    const char* Name() const { return m_name; }

    bool operator==(const EntityDescriptorID& other) const { return m_id == other.m_id; }

    struct Hasher
    {
        std::size_t operator()(const EntityDescriptorID& k) const { return k.m_id; }
    };

private:
    constexpr EntityDescriptorID(uint id, const char* name)
        : m_id(id)
        , m_name(name)
    {
    }

    uint m_id;
    const char* m_name;
};

} // namespace hatcher
