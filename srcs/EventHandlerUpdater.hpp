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

    void HandleEvents(std::span<const SDL_Event> events, hatcher::IEntityManager* entityManager,
                      hatcher::ComponentManager* componentManager, const hatcher::Clock& clock,
                      hatcher::IFrameRenderer& frameRenderer) override;

private:
    void HandleQuitEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                         hatcher::ComponentManager* componentManager,
                         const glm::mat4& previousProjectionMatrix);
    void HandleMouseWheelEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                               hatcher::ComponentManager* componentManager,
                               const glm::mat4& previousProjectionMatrix);
    void HandleMouseMotionEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                                hatcher::ComponentManager* componentManager,
                                const glm::mat4& previousProjectionMatrix);
    void HandleMouseButtonUpEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                                  hatcher::ComponentManager* componentManager,
                                  const glm::mat4& previousProjectionMatrix);
    void HandleMouseButtonDownEvent(const SDL_Event& event, hatcher::IEntityManager* entityManager,
                                    hatcher::ComponentManager* componentManager,
                                    const glm::mat4& previousProjectionMatrix);

    glm::mat4 CalculateProjectionMatrix();

    hatcher::GameApplication* m_application;

    using EventHandlerFunction = void (EventHandlerUpdater::*)(
        const SDL_Event& event, hatcher::IEntityManager* entityManager,
        hatcher::ComponentManager* componentManager, const glm::mat4& previousProjectionMatrix);
    std::unordered_map<uint, EventHandlerFunction> m_eventFunctions;

    glm::vec2 m_fixedPosition = glm::vec2(0.f, 0.f);

    float m_windowWidth = 800.f;
    float m_windowHeight = 600.f;
    float m_pixelSize = 0.01f;

    std::unique_ptr<SelectionRectangleHandler> m_selectionHandler;
};
