#pragma once

#include <memory>
#include <string>

namespace hatcher
{
class Material;
class Mesh;

class MeshLoader
{
public:
    std::unique_ptr<Mesh> LoadWavefront(const Material* material, const std::string& fileName) const;
};

} // namespace hatcher
