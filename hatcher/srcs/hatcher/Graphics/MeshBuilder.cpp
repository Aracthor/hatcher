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

    if (!m_shaderProgramLibrary.contains(key))
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

void MeshBuilder::SetPositions(float* positions, uint positionCount)
{
    m_positions.clear();
    m_positions.insert(m_positions.begin(), positions, positions + positionCount);
}

void MeshBuilder::SetIndices(ushort* indices, uint indexCount)
{
    m_indices.clear();
    m_indices.insert(m_indices.begin(), indices, indices + indexCount);
}

Mesh* MeshBuilder::Create()
{
    HATCHER_ASSERT_MESSAGE(m_primitive, "MeshBuilder::Create was called without a primitive.");
    HATCHER_ASSERT_MESSAGE(m_programToUse != nullptr,
                           "MeshBuilder::Create was called without a ShaderProgram.");
    HATCHER_ASSERT_MESSAGE(!m_positions.empty(),
                           "MeshBuilder::Create was called without positions data.");
    HATCHER_ASSERT_MESSAGE(!m_indices.empty(),
                           "MeshBuilder::Create was called without indices data.");

    Mesh* newMesh = new Mesh(m_programToUse, m_positions.data(), m_positions.size(),
                             m_indices.data(), m_indices.size(), *m_primitive);

    m_primitive.reset();
    m_programToUse = nullptr;
    m_positions.clear();
    m_indices.clear();

    return newMesh;
}

} // namespace hatcher
