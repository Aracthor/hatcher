#pragma once

#include <cstdint>

namespace hatcher
{

// It could have been a typedef of a uint,
// but it could lead to abuse of entity AS a uint.
class Entity
{
  public:
    using IdType = std::uint32_t;

  private:
    IdType m_ID;

  public:
    explicit Entity(IdType id) : m_ID(id) {}

    IdType ID() const { return m_ID; }
};

} // namespace hatcher
