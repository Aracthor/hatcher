#pragma once

#include <memory>

#include "IRendering.hpp"
#include "hatcher/glm_pure.hpp"

namespace hatcher
{
class Clock;
class FrameRenderer;
class MeshBuilder;
class Window;
class World;

class Rendering : public IRendering
{
public:
    Rendering(const char* name, int windowWidth, int windowHeight);
    ~Rendering();

    void UpdateWorldRendering(World* parWorld);
    void RenderWorld();

    glm::ivec2 Resolution() const override;
    const Clock* GetClock() const override { return m_clock.get(); }

    glm::vec2 WorldCoordsToWindowCoords(const glm::vec3& worldCoords,
                                        const glm::mat4& modelMatrix) const override;
    glm::vec3 WindowCoordsToWorldCoords(const glm::vec2 windowCoords) const override;

    const std::unique_ptr<MeshBuilder>& GetMeshBuilder() const override { return m_meshBuilder; }

private:
    std::unique_ptr<Clock> m_clock;
    std::unique_ptr<MeshBuilder> m_meshBuilder;
    std::unique_ptr<FrameRenderer> m_frameRenderer;
    std::unique_ptr<Window> m_window;

    glm::mat4 m_projectionMatrix = glm::mat4(1.f);
    glm::mat4 m_viewMatrix = glm::mat4(1.f);
};

} // namespace hatcher
