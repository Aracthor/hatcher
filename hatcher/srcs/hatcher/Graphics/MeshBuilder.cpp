#include "MeshBuilder.hpp"

#include "Core/ShaderProgram.hpp"
#include "hatcher/assert.hpp"

#include "Mesh.hpp"

namespace hatcher
{

void MeshBuilder::SetProgram(const std::string& vertexShaderFileName,
                             const std::string& fragmentShaderFileName)
{
    ProgramKey key = ProgramKey(vertexShaderFileName, fragmentShaderFileName);

    if (m_shaderProgramLibrary.find(key) == m_shaderProgramLibrary.end())
    {
        m_shaderProgramLibrary[key] = std::make_shared<ShaderProgram>(
            vertexShaderFileName.data(), fragmentShaderFileName.data());
    }
    HATCHER_ASSERT(m_shaderProgramLibrary.contains(key));
    m_programToUse = m_shaderProgramLibrary[key];
}

void MeshBuilder::SetPrimitive(Primitive::Type primitive)
{
    m_primitive = primitive;
}

void MeshBuilder::SetDynamic()
{
    m_dynamic = true;
}

Mesh* MeshBuilder::Create()
{
    HATCHER_ASSERT_MESSAGE(m_primitive, "MeshBuilder::Create was called without a primitive.");
    HATCHER_ASSERT_MESSAGE(m_programToUse != nullptr,
                           "MeshBuilder::Create was called without a ShaderProgram.");

    Mesh* newMesh;
    newMesh = new Mesh(m_programToUse, m_dynamic, *m_primitive);

    m_dynamic = false;
    m_primitive.reset();
    m_programToUse = nullptr;

    return newMesh;
}

} // namespace hatcher
