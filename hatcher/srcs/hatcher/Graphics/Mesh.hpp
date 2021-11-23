#pragma once

#include <memory>

#include "hatcher/basic_types.hpp"
#include "hatcher/glm_pure.hpp"

namespace hatcher
{
class ShaderProgram;
class VertexArrayObject;
class VertexBufferObject;

class Mesh
{
public:
    Mesh(const std::shared_ptr<const ShaderProgram>& shaderProgram, float* positions,
         uint positionCount, ushort* elements, uint elementCount);
    ~Mesh();

    void Draw(const glm::mat4& viewMatrix, const glm::mat4& modelMatrix) const;

private:
    std::unique_ptr<VertexArrayObject> m_VAO;
    std::unique_ptr<VertexBufferObject> m_positionVBO;
    std::unique_ptr<VertexBufferObject> m_elementVBO;
    std::shared_ptr<const ShaderProgram> m_shaderProgram;
};

} // namespace hatcher