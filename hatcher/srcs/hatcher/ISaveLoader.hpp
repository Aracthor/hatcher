#pragma once

#include "hatcher/glm_pure.hpp"

namespace hatcher
{

class ISaveLoader
{
public:
    virtual void operator<<(float& value) = 0;
    virtual void operator<<(glm::vec3& value) = 0;
};

} // namespace hatcher
