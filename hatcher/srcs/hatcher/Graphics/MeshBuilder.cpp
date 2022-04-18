#include "MeshBuilder.hpp"

#include "Core/ShaderProgram.hpp"
#include "hatcher/assert.hpp"

#include "Material.hpp"
#include "Mesh.hpp"

namespace hatcher
{

std::shared_ptr<Material> MeshBuilder::CreateMaterial(const std::string& vertexShaderFileName,
                                                      const std::string& fragmentShaderFileName)
{
    ProgramKey key = ProgramKey(vertexShaderFileName, fragmentShaderFileName);

    if (m_shaderProgramLibrary.find(key) == m_shaderProgramLibrary.end())
    {
        m_shaderProgramLibrary[key] = std::make_shared<ShaderProgram>(
            vertexShaderFileName.data(), fragmentShaderFileName.data());
    }
    HATCHER_ASSERT(m_shaderProgramLibrary.contains(key));
    return std::make_unique<Material>(m_shaderProgramLibrary[key]);
}

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
