#pragma once

#include <memory>
#include <optional>

#include "hatcher/basic_types.hpp"

#include "Primitive.hpp"

namespace hatcher
{

class Material;
class Mesh;

class MeshBuilder
{
public:
    void SetDynamic();
    void SetPrimitive(Primitive::Type type);
    void SetMaterial(const std::shared_ptr<const Material>& material);

    Mesh* Create();

private:
    bool m_dynamic = false;
    std::optional<Primitive::Type> m_primitive;
    std::shared_ptr<const Material> m_material;
};

} // namespace hatcher
