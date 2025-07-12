#include "Mesh.hpp"

#include "Core/VertexArrayObject.hpp"
#include "Core/VertexBufferObject.hpp"
#include "Material.hpp"

namespace hatcher
{

Mesh::Mesh(const Material* material, Primitive primitive, bool dynamic /* = false */)
    : m_material(material)
    , m_dynamic(dynamic)
{
    m_VAO = make_unique<VertexArrayObject>(primitive);
}

Mesh::~Mesh() = default;

void Mesh::Set2DPositions(const float* positions, int positionCount)
{
    SetPositions(2);
    Fill2DPositions(positions, positionCount);
    m_box = {};
    for (int i = 0; i < positionCount / 2; i++)
    {
        const int vertexIndex = i * 2;
        m_box.AddPoint(glm::vec3(positions[vertexIndex], positions[vertexIndex + 1], 0.f));
    }
}

void Mesh::Set3DPositions(const float* positions, int positionCount)
{
    SetPositions(3);
    Fill3DPositions(positions, positionCount);
    m_box = {};
    for (int i = 0; i < positionCount / 3; i++)
    {
        const int vertexIndex = i * 3;
        m_box.AddPoint(glm::vec3(positions[vertexIndex], positions[vertexIndex + 1], positions[vertexIndex + 2]));
    }
}

void Mesh::SetTextureCoords(const float* textureCoords, int textureCoordsCount)
{
    m_VAO->Bind();

    GLint textureCoordsAttribLocation = m_material->TextureCoordsAttribLocation();
    m_textureCoordsVBO = make_unique<VertexBufferObject>(false);
    m_textureCoordsVBO->Bind();
    m_VAO->AttribVBO(*m_textureCoordsVBO, 2, textureCoordsAttribLocation);

    m_VAO->Unbind();

    FillTextureCoords(textureCoords, textureCoordsCount);
}

void Mesh::SetIndices(const ushort* elements, int elementCount)
{
    m_VAO->Bind();

    m_elementVBO = make_unique<VertexBufferObject>(true);
    m_elementVBO->Bind();

    m_VAO->Unbind();

    FillIndices(elements, elementCount);
}

void Mesh::Fill2DPositions(const float* positions, int positionCount)
{
    HATCHER_ASSERT(m_positionVBO);
    m_elementCount = positionCount / 2;
    m_positionVBO->SetData(positions, positionCount, m_dynamic);
}

void Mesh::Fill3DPositions(const float* positions, int positionCount)
{
    HATCHER_ASSERT(m_positionVBO);
    m_elementCount = positionCount / 3;
    m_positionVBO->SetData(positions, positionCount, m_dynamic);
}

void Mesh::FillTextureCoords(const float* textureCoords, int textureCoordsCount)
{
    HATCHER_ASSERT(m_textureCoordsVBO);
    HATCHER_ASSERT(m_elementCount == (int)textureCoordsCount / 2);
    m_textureCoordsVBO->SetData(textureCoords, textureCoordsCount, m_dynamic);
}

void Mesh::FillIndices(const ushort* elements, int elementCount)
{
    HATCHER_ASSERT(m_elementVBO);
    m_elementVBO->SetData(elements, elementCount, m_dynamic);
}

void Mesh::Draw(const glm::mat4& modelMatrix) const
{
    HATCHER_ASSERT(m_material->IsUsed());

    m_material->SetTransformationMatrix("uniModelMatrix", modelMatrix);

    if (m_elementVBO)
        m_VAO->DrawElements(m_elementVBO->ElementCount());
    else
        m_VAO->DrawArrays(m_elementCount);
}

void Mesh::SetPositions(int componentCount)
{
    m_VAO->Bind();

    GLint positionAttribLocation = m_material->PositionAttribLocation();
    m_positionVBO = make_unique<VertexBufferObject>(false);
    m_positionVBO->Bind();
    m_VAO->AttribVBO(*m_positionVBO, componentCount, positionAttribLocation);

    m_VAO->Unbind();
}

} // namespace hatcher
