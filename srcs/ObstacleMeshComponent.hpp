#pragma once

#include <iostream>
#include <memory>

namespace hatcher
{
class Mesh;
} // namespace hatcher

using namespace hatcher;

struct ObstacleMeshComponent
{
    std::shared_ptr<Mesh> mesh;
};
