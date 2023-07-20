#pragma once

#include <vector>

#include "IFrameRenderer.hpp"

#include "hatcher/glm_pure.hpp"

namespace hatcher
{

class FrameRenderer : public IFrameRenderer
{
public:
    FrameRenderer(const Clock* clock, const glm::vec2& resolution);
    ~FrameRenderer();

    void AddMeshToRender(const Mesh* mesh, const glm::mat4& modelMatrix) override;
    void AddUIMeshToRender(const Mesh* mesh, const glm::mat4& modelMatrix) override;
    void SetProjectionMatrix(const glm::mat4& matrix) override;
    void SetViewMatrix(const glm::mat4& matrix) override;

    const Clock* GetClock() const override { return m_clock; }
    glm::ivec2 Resolution() const override { return m_resolution; }
    glm::vec2 WorldCoordsToWindowCoords(const glm::vec3& worldCoords, const glm::mat4& modelMatrix) const override;
    Box2f ProjectBox3DToWindowCoords(const Box3f& box, const glm::mat4& modelMatrix) const override;
    glm::vec3 WindowCoordsToWorldCoords(const glm::vec2 windowCoords) const override;

    void RenderScene() const;
    void RenderUI() const;
    void Clear();

private:
    struct MeshToRender
    {
        const Mesh* mesh;
        glm::mat4 modelMatrix;
    };

    const Clock* m_clock;
    const glm::vec2 m_resolution;

    std::vector<MeshToRender> m_meshesToRender;
    std::vector<MeshToRender> m_UImeshesToRender;
    glm::mat4 m_viewMatrix = glm::mat4(1.f);
    glm::mat4 m_projectionMatrix = glm::mat4(1.f);
};

} // namespace hatcher
