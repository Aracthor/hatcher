#pragma once

#include "hatcher/glm_pure.hpp"
#include "hatcher/unique_ptr.hpp"

namespace hatcher
{
class MaterialFactory;
class MeshLoader;

class IRendering
{
public:
    virtual ~IRendering() = default;

    virtual const unique_ptr<MaterialFactory>& GetMaterialFactory() const = 0;
    virtual const unique_ptr<MeshLoader>& GetMeshLoader() const = 0;
};

} // namespace hatcher
