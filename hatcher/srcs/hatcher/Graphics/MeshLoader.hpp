#pragma once

#include <string>

namespace hatcher
{
class Material;
class Mesh;

class MeshLoader
{
public:
    Mesh* LoadWavefront(const Material* material, const std::string& fileName) const;
};

} // namespace hatcher
