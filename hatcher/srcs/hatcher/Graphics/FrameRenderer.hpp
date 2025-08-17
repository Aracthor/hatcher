#pragma once

#include "IFrameRenderer.hpp"

#include "hatcher/Maths/glm_pure.hpp"

namespace hatcher
{
class GLContext;

class FrameRenderer : public IFrameRenderer
{
public:
    FrameRenderer(GLContext* context, const Clock* clock, const glm::vec2& resolution);
    ~FrameRenderer();

    void SetCurrentTick(int currentTick) { m_currentTick = currentTick; }
    void PrepareSceneDraw(const Material* material) const override;
    void PrepareUIDraw(const Material* material) const override;
    void SetProjectionMatrix(const glm::mat4& matrix) override;
    void SetViewMatrix(const glm::mat4& matrix) override;
    void EnableDepthTest() override;
    void DisableDepthTest() override;

    int CurrentTick() const override { return m_currentTick; }
    const Clock* GetClock() const override { return m_clock; }
    glm::ivec2 Resolution() const override { return m_resolution; }
    glm::vec2 WorldCoordsToWindowCoords(const glm::vec3& worldCoords, const glm::mat4& modelMatrix) const override;
    Box2f ProjectBox3DToWindowCoords(const Box3f& box, const glm::mat4& modelMatrix) const override;
    glm::vec3 WindowCoordsToWorldCoords(const glm::vec2 windowCoords) const override;

private:
    GLContext* m_context;
    int m_currentTick = 0;
    const Clock* m_clock;
    const glm::vec2 m_resolution;

    glm::mat4 m_viewMatrix = glm::mat4(1.f);
    glm::mat4 m_projectionMatrix = glm::mat4(1.f);
};

} // namespace hatcher
