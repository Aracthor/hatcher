#include "Mesh.hpp"

#include "Core/ShaderProgram.hpp"
#include "Core/VertexArrayObject.hpp"
#include "Core/VertexBufferObject.hpp"

namespace hatcher
{

Mesh::Mesh(const std::shared_ptr<const ShaderProgram>& shaderProgram, bool dynamic,
           Primitive::Type primitive)
    : m_shaderProgram(shaderProgram)
    , m_dynamic(dynamic)
{
    m_VAO = std::make_unique<VertexArrayObject>(primitive);
    m_VAO->Bind();

    m_positionVBO = std::make_unique<VertexBufferObject>();

    m_VAO->Unbind();
}

void Mesh::SetPositions(float* positions, uint positionCount)
{
    m_VAO->Bind();

    GLint positionAttribLocation = m_shaderProgram->GetAttribLocation("vertPosition");
    m_positionVBO->SetData(positions, positionCount, m_dynamic);
    m_VAO->AttribVBO(*m_positionVBO, positionAttribLocation);

    m_VAO->Unbind();
}

void Mesh::SetIndices(ushort* elements, uint elementCount)
{
    m_VAO->Bind();

    m_elementVBO = std::make_unique<VertexBufferObject>();
    m_elementVBO->SetData(elements, elementCount, m_dynamic);

    m_VAO->Unbind();
}

Mesh::~Mesh() = default;

void Mesh::Draw(const glm::mat4& projectionMatrix, const glm::mat4& modelMatrix) const
{
    m_shaderProgram->Use();
    m_shaderProgram->SetMatrix4Uniform("uniProjectionMatrix", glm::value_ptr(projectionMatrix));
    m_shaderProgram->SetMatrix4Uniform("uniModelMatrix", glm::value_ptr(modelMatrix));

    if (m_elementVBO)
        m_VAO->DrawElements(m_elementVBO->ElementCount());
    else
        m_VAO->DrawArrays();
}

} // namespace hatcher
