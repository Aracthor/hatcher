#include "Material.hpp"

#include "Core/ShaderProgram.hpp"
#include "Texture.hpp"
#include "hatcher/assert.hpp"

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
    for (auto uniform : m_uniforms)
    {
        m_shaderProgram->SetVector4Uniform(uniform.first, glm::value_ptr(uniform.second));
    }
    for (auto texture : m_textures)
    {
        texture.second->Bind();
    }
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

GLint Material::TextureCoordsAttribLocation() const
{
    return m_shaderProgram->GetAttribLocation("vertTextureCoord");
}

void Material::AddUniform(const char* name, const glm::vec4& value)
{
    HATCHER_ASSERT(m_uniforms.find(name) == m_uniforms.end());
    m_uniforms[name] = value;
}

void Material::AddTexture(const char* name, const std::shared_ptr<const Texture>& texture)
{
    HATCHER_ASSERT(m_textures.find(name) == m_textures.end());
    m_textures[name] = texture;
}

} // namespace hatcher
