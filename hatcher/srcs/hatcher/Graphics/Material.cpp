#include "Material.hpp"

#include "Core/ShaderProgram.hpp"
#include "Texture.hpp"
#include "hatcher/assert.hpp"

namespace hatcher
{

Material::Material(const ShaderProgram* shaderProgram)
    : m_shaderProgram(shaderProgram)
{
}

Material::~Material() = default;

bool Material::IsUsed() const
{
    return m_shaderProgram->IsCurrentlyUsed();
}

void Material::Use() const
{
    m_shaderProgram->Use();
    for (auto uniform : m_floatUniforms)
    {
        m_shaderProgram->SetFloatUniform(uniform.first, uniform.second);
    }
    for (auto uniform : m_vec4Uniforms)
    {
        m_shaderProgram->SetVector4Uniform(uniform.first, glm::value_ptr(uniform.second));
    }
    for (auto texture : m_textures)
    {
        texture.second->Bind();
    }
}

void Material::SetTransformationMatrix(const char* name, const glm::mat4& matrix) const
{
    m_shaderProgram->SetMatrix4Uniform(name, glm::value_ptr(matrix));
}

GLint Material::PositionAttribLocation() const
{
    return m_shaderProgram->GetAttribLocation("vertPosition");
}

GLint Material::TextureCoordsAttribLocation() const
{
    return m_shaderProgram->GetAttribLocation("vertTextureCoord");
}

GLint Material::InstancedAttribLocation(const char* name) const
{
    return m_shaderProgram->GetAttribLocation(name);
}

void Material::AddUniform(const char* name, float value)
{
    HATCHER_ASSERT(m_floatUniforms.find(name) == m_floatUniforms.end());
    m_floatUniforms[name] = value;
}

void Material::AddUniform(const char* name, const glm::vec4& value)
{
    HATCHER_ASSERT(m_vec4Uniforms.find(name) == m_vec4Uniforms.end());
    m_vec4Uniforms[name] = value;
}

void Material::AddTexture(const char* name, const Texture* texture)
{
    HATCHER_ASSERT(texture != nullptr);
    HATCHER_ASSERT(m_textures.find(name) == m_textures.end());
    m_textures[name] = texture;
}

} // namespace hatcher
