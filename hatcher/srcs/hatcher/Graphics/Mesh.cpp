#include "Mesh.hpp"

#include "Core/ShaderProgram.hpp"
#include "Core/VertexArrayObject.hpp"
#include "Core/VertexBufferObject.hpp"

namespace hatcher
{

Mesh::Mesh(const std::shared_ptr<const ShaderProgram>& shaderProgram, float* positions,
           uint positionCount, ushort* elements, uint elementCount)
    : m_shaderProgram(shaderProgram)
{
    m_positionVBO = std::make_unique<VertexBufferObject>();
    m_positionVBO->SetData(positions, positionCount);

    m_elementVBO = std::make_unique<VertexBufferObject>();
    m_elementVBO->SetData(elements, elementCount);

    m_VAO = std::make_unique<VertexArrayObject>();
    GLint positionAttribLocation = m_shaderProgram->GetAttribLocation("vertPosition");
    m_VAO->AttribVBO(*m_positionVBO, positionAttribLocation);
}

Mesh::~Mesh() = default;

void Mesh::Draw(const glm::mat4& viewMatrix, const glm::mat4& modelMatrix) const
{
    m_shaderProgram->Use();
    m_shaderProgram->SetMatrix4Uniform("uniViewMatrix", glm::value_ptr(viewMatrix));
    m_shaderProgram->SetMatrix4Uniform("uniModelMatrix", glm::value_ptr(modelMatrix));

    m_elementVBO->Bind();
    m_VAO->DrawElements(m_elementVBO->ElementCount());
}

} // namespace hatcher
