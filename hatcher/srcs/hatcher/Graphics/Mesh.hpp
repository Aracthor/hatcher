#pragma once

#include <string>
#include <unordered_map>

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

    void Set2DPositions(const float* positions, int positionCount);
    void Set3DPositions(const float* positions, int positionCount);
    void SetTextureCoords(const float* textureCoords, int textureCoordCount);
    void SetIndices(const ushort* elements, int elementCount);
    void AddInstancedBuffer(const std::string& name, int componentSize);

    void Fill2DPositions(const float* positions, int positionCount);
    void Fill3DPositions(const float* positions, int positionCount);
    void FillTextureCoords(const float* textureCoords, int textureCoordCount);
    void FillIndices(const ushort* elements, int elementCount);
    void FillInstancedBuffer(const std::string& name, const float* data, int length);

    const Box3f& Box() const { return m_box; }
    void Draw(const glm::mat4& modelMatrix) const;
    void DrawInstanced(int instanceCount) const;

private:
    void SetPositions(int componentCount);

    unique_ptr<VertexArrayObject> m_VAO;
    unique_ptr<VertexBufferObject> m_positionVBO;
    unique_ptr<VertexBufferObject> m_textureCoordsVBO;
    unique_ptr<VertexBufferObject> m_elementVBO;
    std::unordered_map<std::string, unique_ptr<VertexBufferObject>> m_instancedVBO;
    int m_elementCount;
    const Material* m_material;
    Box3f m_box;
    bool m_dynamic;
};

} // namespace hatcher
