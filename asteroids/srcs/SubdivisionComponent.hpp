#pragma once

#include "hatcher/EntityDescriptorID.hpp"

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

struct SubdivisionComponent
{
    hatcher::EntityDescriptorID SubdivisionID;
    int count;
};

void operator<<(hatcher::ISaveLoader& saveLoader, SubdivisionComponent& component);
