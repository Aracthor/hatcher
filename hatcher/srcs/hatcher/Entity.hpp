#pragma once

#include <cstdint>

namespace hatcher
{

// It could have been a typedef of a uint,
// but it could lead to abuse of entity AS a uint.
class Entity
{
public:
    using IDType = int;

private:
    IDType m_ID;

public:
    static Entity Invalid() { return Entity(-1); }

    explicit Entity(IDType id)
        : m_ID(id)
    {
    }

    IDType ID() const { return m_ID; }

    bool operator==(Entity other) const { return ID() == other.ID(); }
    bool operator!=(Entity other) const { return ID() != other.ID(); }
};

} // namespace hatcher
