#pragma once

#include <memory>

#include "hatcher/Maths/Box.hpp"
#include "hatcher/basic_types.hpp"
#include "hatcher/glm_pure.hpp"

#include "Primitive.hpp"

namespace hatcher
{
class Material;
class VertexArrayObject;
class VertexBufferObject;

class Mesh
{
public:
    Mesh(const Material* material, Primitive::Type primitive, bool dynamic = false);
    ~Mesh();

    void Set2DPositions(const float* positions, uint positionCount);
    void Set3DPositions(const float* positions, uint positionCount);
    void SetTextureCoords(const float* positions, uint positionCount);
    void SetIndices(const ushort* elements, uint elementCount);

    const Box3f& Box() const { return m_box; }
    void Draw(const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const;

private:
    void SetPositions(const float* positions, uint positionCount, int componentCount);

    std::unique_ptr<VertexArrayObject> m_VAO;
    std::unique_ptr<VertexBufferObject> m_positionVBO;
    std::unique_ptr<VertexBufferObject> m_textureCoordsVBO;
    std::unique_ptr<VertexBufferObject> m_elementVBO;
    const Material* m_material;
    Box3f m_box;
    bool m_dynamic;
};

} // namespace hatcher
