#include "Mesh.hpp"

#include "Core/VertexArrayObject.hpp"
#include "Core/VertexBufferObject.hpp"
#include "Material.hpp"

namespace hatcher
{

Mesh::Mesh(const std::shared_ptr<const Material>& material, bool dynamic, Primitive::Type primitive)
    : m_material(material)
    , m_dynamic(dynamic)
{
    m_VAO = std::make_unique<VertexArrayObject>(primitive);
    m_VAO->Bind();

    m_positionVBO = std::make_unique<VertexBufferObject>();

    m_VAO->Unbind();
}

Mesh::~Mesh() = default;

void Mesh::Set2DPositions(float* positions, uint positionCount)
{
    SetPositions(positions, positionCount, 2);
}

void Mesh::Set3DPositions(float* positions, uint positionCount)
{
    SetPositions(positions, positionCount, 3);
}

void Mesh::SetTextureCoords(float* textureCoords, uint textureCoordsCount)
{
    m_VAO->Bind();

    GLint textureCoordsAttribLocation = m_material->TextureCoordsAttribLocation();
    m_textureCoordsVBO = std::make_unique<VertexBufferObject>();
    m_textureCoordsVBO->SetData(textureCoords, textureCoordsCount, m_dynamic);
    m_VAO->AttribVBO(*m_textureCoordsVBO, 2, textureCoordsAttribLocation);

    m_VAO->Unbind();
}

void Mesh::SetIndices(ushort* elements, uint elementCount)
{
    m_VAO->Bind();

    m_elementVBO = std::make_unique<VertexBufferObject>();
    m_elementVBO->SetData(elements, elementCount, m_dynamic);

    m_VAO->Unbind();
}

void Mesh::Draw(const glm::mat4& modelMatrix, const glm::mat4& viewMatrix,
                const glm::mat4& projectionMatrix) const
{
    m_material->Use();
    m_material->SetTransformationMatrices(modelMatrix, viewMatrix, projectionMatrix);

    if (m_elementVBO)
        m_VAO->DrawElements(m_elementVBO->ElementCount());
    else
        m_VAO->DrawArrays();
}

void Mesh::SetPositions(float* positions, uint positionCount, int componentCount)
{
    m_VAO->Bind();

    GLint positionAttribLocation = m_material->PositionAttribLocation();
    m_positionVBO->SetData(positions, positionCount, m_dynamic);
    m_VAO->AttribVBO(*m_positionVBO, componentCount, positionAttribLocation);

    m_VAO->Unbind();
}

} // namespace hatcher
