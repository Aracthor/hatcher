#pragma once

#include <memory>

#include "hatcher/basic_types.hpp"
#include "hatcher/glm_pure.hpp"

#include "Primitive.hpp"

namespace hatcher
{
class ShaderProgram;
class VertexArrayObject;
class VertexBufferObject;

class Mesh
{
public:
    Mesh(const std::shared_ptr<const ShaderProgram>& shaderProgram, bool dynamic,
         Primitive::Type primitive);
    ~Mesh();

    void Set2DPositions(float* positions, uint positionCount);
    void Set3DPositions(float* positions, uint positionCount);
    void SetIndices(ushort* elements, uint elementCount);

    void Draw(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix,
              const glm::mat4& modelMatrix) const;

private:
    void SetPositions(float* positions, uint positionCount, int componentCount);

    std::unique_ptr<VertexArrayObject> m_VAO;
    std::unique_ptr<VertexBufferObject> m_positionVBO;
    std::unique_ptr<VertexBufferObject> m_elementVBO;
    std::shared_ptr<const ShaderProgram> m_shaderProgram;
    bool m_dynamic;
};

} // namespace hatcher
