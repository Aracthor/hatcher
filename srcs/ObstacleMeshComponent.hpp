#pragma once

#include <iostream>
#include <memory>

namespace hatcher
{
class Mesh;
} // namespace hatcher

struct ObstacleMeshComponent
{
    std::shared_ptr<hatcher::Mesh> mesh;
};
