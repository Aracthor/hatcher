#include "Material.hpp"

#include "Core/ShaderProgram.hpp"

namespace hatcher
{

Material::Material(const std::shared_ptr<const ShaderProgram> shaderProgram)
    : m_shaderProgram(shaderProgram)
{
}

Material::~Material() = default;

void Material::Use() const
{
    m_shaderProgram->Use();
}

void Material::SetTransformationMatrices(const glm::mat4& modelMatrix, const glm::mat4& viewMatrix,
                                         const glm::mat4& projectionMatrix) const
{
    m_shaderProgram->SetMatrix4Uniform("uniProjectionMatrix", glm::value_ptr(projectionMatrix));
    m_shaderProgram->SetMatrix4Uniform("uniViewMatrix", glm::value_ptr(viewMatrix));
    m_shaderProgram->SetMatrix4Uniform("uniModelMatrix", glm::value_ptr(modelMatrix));
}

GLint Material::PositionAttribLocation() const
{
    return m_shaderProgram->GetAttribLocation("vertPosition");
}

} // namespace hatcher
