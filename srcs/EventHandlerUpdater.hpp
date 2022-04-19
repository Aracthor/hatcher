#pragma once

#include <memory>
#include <unordered_map>

#include <SDL2/SDL_events.h>

#include "hatcher/Graphics/AbstractEventUpdater.hpp"
#include "hatcher/Maths/Box.hpp"
#include "hatcher/glm_pure.hpp"

namespace hatcher
{
class Clock;
class GameApplication;
class MeshBuilder;
} // namespace hatcher

class GridDisplay;
class SelectionRectangleHandler;

class EventHandlerUpdater final : public hatcher::AbstractEventUpdater
{
public:
    EventHandlerUpdater(hatcher::GameApplication* application,
                        const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder);
    ~EventHandlerUpdater();

    void HandleEvents(const hatcher::span<const SDL_Event>& events,
                      hatcher::IEntityManager* entityManager,
                      hatcher::ComponentManager* componentManager,
                      hatcher::IFrameRenderer& frameRenderer,
                      const hatcher::IRendering& rendering) override;

private:
    void HandleCameraMotion(const hatcher::Clock* clock, const Uint8* keyState);

    void HandleQuitEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                         hatcher::ComponentManager* componentManager,
                         const hatcher::IRendering& rendering);
    void HandleMouseWheelEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                               hatcher::ComponentManager* componentManager,
                               const hatcher::IRendering& rendering);
    void HandleMouseMotionEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                                hatcher::ComponentManager* componentManager,
                                const hatcher::IRendering& rendering);
    void HandleMouseButtonUpEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                                  hatcher::ComponentManager* componentManager,
                                  const hatcher::IRendering& rendering);
    void HandleMouseButtonDownEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                                    hatcher::ComponentManager* componentManager,
                                    const hatcher::IRendering& rendering);
    void HandleKeyDownEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                            hatcher::ComponentManager* componentManager,
                            const hatcher::IRendering& rendering);

    glm::mat4 CalculateProjectionMatrix(const hatcher::IRendering& rendering);

    hatcher::Box2f ProjectBox3DToScreenSpace(const hatcher::Box3f& box,
                                             const glm::mat4& modelMatrix,
                                             const hatcher::IRendering& rendering) const;
    glm::vec2 WorldCoordsToWindowCoords(const glm::vec3& worldCoords, const glm::mat4& modelMatrix,
                                        const hatcher::IRendering& rendering) const;

    glm::vec2 MouseCoordsToWorldCoords(int x, int y, const hatcher::IRendering& rendering) const;

    hatcher::GameApplication* m_application;

    using EventHandlerFunction = void (EventHandlerUpdater::*)(
        const SDL_Event& event, hatcher::IEntityManager* entityManager,
        hatcher::ComponentManager* componentManager, const hatcher::IRendering& rendering);
    std::unordered_map<uint, EventHandlerFunction> m_eventFunctions;

    glm::vec3 m_cameraPosition = glm::vec3(0.f, 0.f, 100.f);
    glm::vec3 m_cameraTarget = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 m_cameraUp = glm::vec3(0.f, 1.f, 0.f);
    glm::mat4 m_projectionMatrix = glm::mat4(1.f);
    glm::mat4 m_viewMatrix = glm::mat4(1.f);

    float m_pixelSize = 0.01f;

    std::unique_ptr<SelectionRectangleHandler> m_selectionHandler;
    std::unique_ptr<GridDisplay> m_gridDisplay;
};
