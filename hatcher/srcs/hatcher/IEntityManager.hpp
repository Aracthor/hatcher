#pragma once

#include "Entity.hpp"

namespace hatcher
{

class IEntityManager
{
public:
    virtual ~IEntityManager() = default;

    virtual Entity CreateNewEntity() = 0;
};

} // namespace hatcher
