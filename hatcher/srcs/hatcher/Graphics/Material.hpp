#pragma once

#include <memory>

#include "Core/gl_types.hpp"
#include "hatcher/glm_pure.hpp"

namespace hatcher
{
class ShaderProgram;

class Material
{
public:
    explicit Material(const std::shared_ptr<const ShaderProgram> shaderProgrma);
    ~Material();

    void Use() const;
    void SetTransformationMatrices(const glm::mat4& modelMatrix, const glm::mat4& viewMatrix,
                                   const glm::mat4& projectionMatrix) const;

    GLint PositionAttribLocation() const;

private:
    std::shared_ptr<const ShaderProgram> m_shaderProgram;
};

} // namespace hatcher
