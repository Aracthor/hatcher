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
class IRendering;
} // namespace hatcher

class GridDisplay;
class SelectionRectangleHandler;

class EventHandlerUpdater final : public hatcher::AbstractEventUpdater
{
public:
    EventHandlerUpdater(hatcher::GameApplication* application,
                        const hatcher::IRendering* rendering);
    ~EventHandlerUpdater();

    void HandleEvents(const hatcher::span<const SDL_Event>& events,
                      hatcher::IEntityManager* entityManager,
                      hatcher::ComponentManager* componentManager,
                      hatcher::IFrameRenderer& frameRenderer) override;

private:
    void HandleCameraMotion(const hatcher::Clock* clock, const Uint8* keyState);

    void HandleQuitEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                         hatcher::ComponentManager* componentManager,
                         const hatcher::IFrameRenderer& frameRenderer);
    void HandleMouseWheelEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                               hatcher::ComponentManager* componentManager,
                               const hatcher::IFrameRenderer& frameRenderer);
    void HandleMouseMotionEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                                hatcher::ComponentManager* componentManager,
                                const hatcher::IFrameRenderer& frameRenderer);
    void HandleMouseButtonUpEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                                  hatcher::ComponentManager* componentManager,
                                  const hatcher::IFrameRenderer& frameRenderer);
    void HandleMouseButtonDownEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                                    hatcher::ComponentManager* componentManager,
                                    const hatcher::IFrameRenderer& frameRenderer);
    void HandleKeyDownEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                            hatcher::ComponentManager* componentManager,
                            const hatcher::IFrameRenderer& frameRenderer);

    glm::mat4 CalculateProjectionMatrix(const hatcher::IFrameRenderer& frameRenderer);

    glm::vec2 MouseCoordsToWorldCoords(int x, int y,
                                       const hatcher::IFrameRenderer& frameRenderer) const;

    hatcher::GameApplication* m_application;

    using EventHandlerFunction = void (EventHandlerUpdater::*)(
        const SDL_Event& event, hatcher::IEntityManager* entityManager,
        hatcher::ComponentManager* componentManager, const hatcher::IFrameRenderer& frameRenderer);
    std::unordered_map<uint, EventHandlerFunction> m_eventFunctions;

    glm::vec3 m_cameraPosition = glm::vec3(0.f, 0.f, 100.f);
    glm::vec3 m_cameraTarget = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 m_cameraUp = glm::vec3(0.f, 1.f, 0.f);

    float m_pixelSize = 0.01f;

    std::unique_ptr<SelectionRectangleHandler> m_selectionHandler;
    std::unique_ptr<GridDisplay> m_gridDisplay;
};
