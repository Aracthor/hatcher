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
    explicit Entity(IDType id)
        : m_ID(id)
    {
    }

    IDType ID() const { return m_ID; }
};

} // namespace hatcher
