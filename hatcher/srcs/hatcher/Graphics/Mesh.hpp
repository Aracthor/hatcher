#pragma once

#include "hatcher/Maths/Box.hpp"
#include "hatcher/Maths/glm_pure.hpp"
#include "hatcher/basic_types.hpp"
#include "hatcher/unique_ptr.hpp"

#include "Primitive.hpp"

namespace hatcher
{
class Material;
class VertexArrayObject;
class VertexBufferObject;

class Mesh
{
public:
    Mesh(const Material* material, Primitive primitive, bool dynamic = false);
    ~Mesh();

    void Set2DPositions(const float* positions, uint positionCount);
    void Set3DPositions(const float* positions, uint positionCount);
    void SetTextureCoords(const float* positions, uint positionCount);
    void SetIndices(const ushort* elements, uint elementCount);

    const Box3f& Box() const { return m_box; }
    void Draw(const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const;

private:
    void SetPositions(const float* positions, uint positionCount, int componentCount);

    unique_ptr<VertexArrayObject> m_VAO;
    unique_ptr<VertexBufferObject> m_positionVBO;
    unique_ptr<VertexBufferObject> m_textureCoordsVBO;
    unique_ptr<VertexBufferObject> m_elementVBO;
    int m_elementCount;
    const Material* m_material;
    Box3f m_box;
    bool m_dynamic;
};

} // namespace hatcher
