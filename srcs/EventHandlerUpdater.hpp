#pragma once

#include <memory>
#include <unordered_map>

#include <SDL2/SDL_events.h>

#include "hatcher/Graphics/AbstractEventUpdater.hpp"
#include "hatcher/glm_pure.hpp"

namespace hatcher
{
class GameApplication;
class MeshBuilder;
} // namespace hatcher

class SelectionRectangleHandler;

class EventHandlerUpdater final : public hatcher::AbstractEventUpdater
{
public:
    EventHandlerUpdater(hatcher::GameApplication* application,
                        const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder);
    ~EventHandlerUpdater();

    void HandleEvents(const hatcher::span<const SDL_Event>& events,
                      hatcher::IEntityManager* entityManager,
                      hatcher::ComponentManager* componentManager, const hatcher::Clock& clock,
                      hatcher::IFrameRenderer& frameRenderer) override;

private:
    void HandleQuitEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                         hatcher::ComponentManager* componentManager);
    void HandleMouseWheelEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                               hatcher::ComponentManager* componentManager);
    void HandleMouseMotionEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                                hatcher::ComponentManager* componentManager);
    void HandleMouseButtonUpEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                                  hatcher::ComponentManager* componentManager);
    void HandleMouseButtonDownEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                                    hatcher::ComponentManager* componentManager);

    glm::mat4 CalculateProjectionMatrix();

    hatcher::GameApplication* m_application;

    using EventHandlerFunction = void (EventHandlerUpdater::*)(
        const SDL_Event& event, hatcher::IEntityManager* entityManager,
        hatcher::ComponentManager* componentManager);
    std::unordered_map<uint, EventHandlerFunction> m_eventFunctions;

    glm::vec2 m_fixedPosition = glm::vec2(0.f, 0.f);
    glm::mat4 m_projectionMatrix = glm::mat4(1.f);
    glm::mat4 m_viewMatrix = glm::mat4(1.f);

    float m_windowWidth = 800.f;
    float m_windowHeight = 600.f;
    float m_pixelSize = 0.01f;

    std::unique_ptr<SelectionRectangleHandler> m_selectionHandler;
};
