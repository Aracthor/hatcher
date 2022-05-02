#pragma once

#include <vector>

#include "IFrameRenderer.hpp"

#include "hatcher/glm_pure.hpp"

namespace hatcher
{

class FrameRenderer : public IFrameRenderer
{
public:
    FrameRenderer(const glm::vec2& resolution);
    ~FrameRenderer();

    void AddMeshToRender(const Mesh* mesh, const glm::mat4& modelMatrix) override;
    void AddUIMeshToRender(const Mesh* mesh, const glm::mat4& modelMatrix) override;
    void SetProjectionMatrix(const glm::mat4& matrix) override;
    void SetViewMatrix(const glm::mat4& matrix) override;

    glm::ivec2 Resolution() const override { return m_resolution; }
    glm::vec2 WorldCoordsToWindowCoords(const glm::vec3& worldCoords,
                                        const glm::mat4& modelMatrix) const override;
    hatcher::Box2f ProjectBox3DToWindowCoords(const hatcher::Box3f& box,
                                              const glm::mat4& modelMatrix) const override;
    glm::vec3 WindowCoordsToWorldCoords(const glm::vec2 windowCoords) const override;

    const glm::mat4& ProjectionMatrix() const { return m_projectionMatrix; }
    const glm::mat4& ViewMatrix() const { return m_viewMatrix; }

    void Render() const;

private:
    struct MeshToRender
    {
        const Mesh* mesh;
        glm::mat4 modelMatrix;
    };

    const glm::vec2 m_resolution;

    std::vector<MeshToRender> m_meshesToRender;
    std::vector<MeshToRender> m_UImeshesToRender;
    glm::mat4 m_viewMatrix = glm::mat4(1.f);
    glm::mat4 m_projectionMatrix = glm::mat4(1.f);
};

} // namespace hatcher
