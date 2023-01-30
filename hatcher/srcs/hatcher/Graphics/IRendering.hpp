#pragma once

#include <memory>

#include "hatcher/glm_pure.hpp"

namespace hatcher
{
class MaterialFactory;
class MeshLoader;

class IRendering
{
public:
    virtual ~IRendering() = default;

    virtual const std::unique_ptr<MaterialFactory>& GetMaterialFactory() const = 0;
    virtual const std::unique_ptr<MeshLoader>& GetMeshLoader() const = 0;
};

} // namespace hatcher
