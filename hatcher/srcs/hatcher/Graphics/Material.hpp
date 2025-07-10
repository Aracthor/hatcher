#pragma once

#include <map>

#include "Core/gl_types.hpp"
#include "hatcher/Maths/glm_pure.hpp"

namespace hatcher
{
class ShaderProgram;
class Texture;

class Material
{
public:
    explicit Material(const ShaderProgram* shaderProgrma);
    ~Material();

    bool IsUsed() const;
    void Use() const;
    void SetTransformationMatrix(const char* name, const glm::mat4& matrix) const;

    GLint PositionAttribLocation() const;
    GLint TextureCoordsAttribLocation() const;

    void AddUniform(const char* name, float value);
    void AddUniform(const char* name, const glm::vec4& value);
    void AddTexture(const char* name, const Texture* texture);

private:
    const ShaderProgram* m_shaderProgram;
    // TODO find a better way to handle different types of uniforms
    std::map<const char*, float> m_floatUniforms;
    std::map<const char*, glm::vec4> m_vec4Uniforms;
    std::map<const char*, const Texture*> m_textures;
};

} // namespace hatcher
