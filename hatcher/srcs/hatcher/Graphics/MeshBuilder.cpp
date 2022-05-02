#include "MeshBuilder.hpp"

#include "hatcher/assert.hpp"

#include "Material.hpp"
#include "Mesh.hpp"

namespace hatcher
{

void MeshBuilder::SetPrimitive(Primitive::Type primitive)
{
    m_primitive = primitive;
}

void MeshBuilder::SetDynamic()
{
    m_dynamic = true;
}

void MeshBuilder::SetMaterial(const std::shared_ptr<const Material>& material)
{
    m_material = material;
}

Mesh* MeshBuilder::Create()
{
    HATCHER_ASSERT_MESSAGE(m_primitive, "MeshBuilder::Create was called without a primitive.");
    HATCHER_ASSERT_MESSAGE(m_material != nullptr,
                           "MeshBuilder::Create was called without a Material.");

    Mesh* newMesh;
    newMesh = new Mesh(m_material, m_dynamic, *m_primitive);

    m_dynamic = false;
    m_primitive.reset();
    m_material = nullptr;

    return newMesh;
}

} // namespace hatcher
