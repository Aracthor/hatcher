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
    Mesh* LoadWavefront(const std::shared_ptr<const Material>& material,
                        const std::string& fileName) const;
};

} // namespace hatcher
