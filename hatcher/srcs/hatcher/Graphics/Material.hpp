#pragma once

#include <map>
#include <memory>

#include "Core/gl_types.hpp"
#include "hatcher/glm_pure.hpp"

namespace hatcher
{
class ShaderProgram;
class Texture;

class Material
{
public:
    explicit Material(const std::shared_ptr<const ShaderProgram> shaderProgrma);
    ~Material();

    void Use() const;
    void SetTransformationMatrices(const glm::mat4& modelMatrix, const glm::mat4& viewMatrix,
                                   const glm::mat4& projectionMatrix) const;

    GLint PositionAttribLocation() const;
    GLint TextureCoordsAttribLocation() const;

    void AddUniform(const char* name, float value);
    void AddUniform(const char* name, const glm::vec4& value);
    void AddTexture(const char* name, const std::shared_ptr<const Texture>& texture);

private:
    std::shared_ptr<const ShaderProgram> m_shaderProgram;
    // TODO find a better way to handle different types of uniforms
    std::map<const char*, float> m_floatUniforms;
    std::map<const char*, glm::vec4> m_vec4Uniforms;
    std::map<const char*, std::shared_ptr<const Texture>> m_textures;
};

} // namespace hatcher
