#pragma once

#include <string>

#include "Primitive.hpp"

#include "hatcher/unique_ptr.hpp"

namespace hatcher
{
class Material;
class Mesh;

class MeshLoader
{
public:
    unique_ptr<Mesh> LoadWavefront(const Material* material, const std::string& fileName,
                                   Primitive::Type primitive) const;
};

} // namespace hatcher
