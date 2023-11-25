#pragma once

#include "hatcher/EntityDescriptorID.hpp"

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

struct WreckageGeneratorComponent
{
    hatcher::EntityDescriptorID WreckageID;
    int count;
};

void operator<<(hatcher::ISaveLoader& saveLoader, WreckageGeneratorComponent& component);
