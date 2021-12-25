#include "EventHandlerUpdater.hpp"

#include <SDL2/SDL_events.h>

#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"
#include "Selectable2DComponent.hpp"
#include "SelectionRectangleHandler.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/GameApplication.hpp"
#include "hatcher/Graphics/Clock.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Maths/Box.hpp"
#include "hatcher/assert.hpp"

namespace
{

glm::vec2 MouseCoordsToWorldCoords(int x, int y, const glm::mat4& previousProjectionMatrix)
{
    const float windowWidth = 800;
    const float windowHeight = 600;

    const glm::vec3 winCoords(x, windowHeight - y, 0.f);
    const glm::mat4 modelMatrix = glm::mat4(1.f);
    const glm::vec4 viewport = {0.f, 0.f, windowWidth, windowHeight};
    const glm::vec3 worldCoords =
        glm::unProject(winCoords, modelMatrix, previousProjectionMatrix, viewport);
    return static_cast<glm::vec2>(worldCoords);
}

} // namespace

EventHandlerUpdater::EventHandlerUpdater(hatcher::GameApplication* application,
                                         const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder)
    : m_application(application)
{
    m_eventFunctions[SDL_QUIT] = &EventHandlerUpdater::HandleQuitEvent;
    m_eventFunctions[SDL_MOUSEWHEEL] = &EventHandlerUpdater::HandleMouseWheelEvent;
    m_eventFunctions[SDL_MOUSEMOTION] = &EventHandlerUpdater::HandleMouseMotionEvent;
    m_eventFunctions[SDL_MOUSEBUTTONUP] = &EventHandlerUpdater::HandleMouseButtonUpEvent;
    m_eventFunctions[SDL_MOUSEBUTTONDOWN] = &EventHandlerUpdater::HandleMouseButtonDownEvent;

    m_selectionHandler = std::make_unique<SelectionRectangleHandler>(meshBuilder);
}

EventHandlerUpdater::~EventHandlerUpdater() = default;

void EventHandlerUpdater::HandleEvents(std::span<const SDL_Event> events,
                                       hatcher::IEntityManager* entityManager,
                                       hatcher::ComponentManager* componentManager,
                                       const hatcher::Clock& clock,
                                       hatcher::IFrameRenderer& frameRenderer)
{
    const glm::mat4 previousProjectionMatrix = CalculateProjectionMatrix();

    const float elapsedTime = clock.GetElapsedTime();
    const Uint8* keyState = SDL_GetKeyboardState(NULL);

    if (keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W])
        m_fixedPosition.y += 0.01f * elapsedTime;
    if (keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S])
        m_fixedPosition.y -= 0.01f * elapsedTime;
    if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D])
        m_fixedPosition.x += 0.01f * elapsedTime;
    if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
        m_fixedPosition.x -= 0.01f * elapsedTime;
    if (keyState[SDL_SCANCODE_ESCAPE])
        m_application->Stop();

    for (const SDL_Event& event : events)
    {
        auto functionIt = m_eventFunctions.find(event.type);
        if (functionIt != m_eventFunctions.end())
        {
            EventHandlerFunction handlerFunction = functionIt->second;
            (this->*handlerFunction)(event, entityManager, componentManager,
                                     previousProjectionMatrix);
        }
    }

    m_selectionHandler->DrawSelectionRectangle(frameRenderer);

    const glm::mat4 newProjectionMatrix = CalculateProjectionMatrix();
    frameRenderer.SetProjectionMatrix(newProjectionMatrix);
}

void EventHandlerUpdater::HandleQuitEvent(const SDL_Event& event,
                                          hatcher::IEntityManager* entityManager,
                                          hatcher::ComponentManager* componentManager,
                                          const glm::mat4& previousProjectionMatrix)
{
    (void)event;
    (void)entityManager;
    (void)componentManager;
    (void)previousProjectionMatrix;

    m_application->Stop();
}

void EventHandlerUpdater::HandleMouseWheelEvent(const SDL_Event& event,
                                                hatcher::IEntityManager* entityManager,
                                                hatcher::ComponentManager* componentManager,
                                                const glm::mat4& previousProjectionMatrix)
{
    (void)entityManager;
    (void)componentManager;
    (void)previousProjectionMatrix;

    int verticalScroll = event.wheel.y;

    // We don't use verticalScroll actual value because with emscripten,
    // depending of browsers, this value can make no sense...
    if (verticalScroll < 0)
        m_pixelSize *= 4.f / 3.f;
    else if (verticalScroll > 0)
        m_pixelSize *= 3.f / 4.f;
}

void EventHandlerUpdater::HandleMouseMotionEvent(const SDL_Event& event,
                                                 hatcher::IEntityManager* entityManager,
                                                 hatcher::ComponentManager* componentManager,
                                                 const glm::mat4& previousProjectionMatrix)
{
    (void)entityManager;
    (void)componentManager;

    if (m_selectionHandler->IsSelecting())
    {
        const glm::vec2 worldCoords2D =
            MouseCoordsToWorldCoords(event.motion.x, event.motion.y, previousProjectionMatrix);

        m_selectionHandler->MoveSelection(worldCoords2D);
    }
}

void EventHandlerUpdater::HandleMouseButtonUpEvent(const SDL_Event& event,
                                                   hatcher::IEntityManager* entityManager,
                                                   hatcher::ComponentManager* componentManager,
                                                   const glm::mat4& previousProjectionMatrix)
{
    (void)entityManager;
    (void)previousProjectionMatrix;

    if (event.button.button == SDL_BUTTON_LEFT)
    {
        std::span<std::optional<Selectable2DComponent>> selectableComponents =
            componentManager->GetComponents<Selectable2DComponent>();
        std::span<std::optional<Position2DComponent>> positionComponents =
            componentManager->GetComponents<Position2DComponent>();
        const hatcher::Box2f selectionRectangle = m_selectionHandler->GetCurrentSelection();

        HATCHER_ASSERT(selectableComponents.size() == positionComponents.size());
        for (uint i = 0; i < selectableComponents.size(); i++)
        {
            std::optional<Selectable2DComponent>& selectableComponent = selectableComponents[i];
            std::optional<Position2DComponent>& positionComponent = positionComponents[i];
            if (selectableComponent)
            {
                HATCHER_ASSERT(positionComponent);
                const hatcher::Box2f entityBox =
                    selectableComponent->Box.Translated(positionComponent->Position);
                selectableComponent->Selected = selectionRectangle.Touches(entityBox);
            }
        }

        m_selectionHandler->EndSelection();
    }
}

void EventHandlerUpdater::HandleMouseButtonDownEvent(const SDL_Event& event,
                                                     hatcher::IEntityManager* entityManager,
                                                     hatcher::ComponentManager* componentManager,
                                                     const glm::mat4& previousProjectionMatrix)
{
    const glm::vec2 worldCoords2D =
        MouseCoordsToWorldCoords(event.button.x, event.button.y, previousProjectionMatrix);

    if (event.button.button == SDL_BUTTON_LEFT)
    {
        m_selectionHandler->StartSelection(worldCoords2D);
    }

    if (event.button.button == SDL_BUTTON_RIGHT)
    {
        auto movementComponents = componentManager->GetComponents<Movement2DComponent>();
        auto selectableComponents = componentManager->GetComponents<Selectable2DComponent>();
        auto positionComponents = componentManager->GetComponents<Position2DComponent>();

        HATCHER_ASSERT(movementComponents.size() == positionComponents.size());
        HATCHER_ASSERT(selectableComponents.size() == positionComponents.size());
        for (uint i = 0; i < selectableComponents.size(); i++)
        {
            std::optional<Movement2DComponent>& movementComponent = movementComponents[i];
            std::optional<Selectable2DComponent>& selectableComponent = selectableComponents[i];
            std::optional<Position2DComponent>& positionComponent = positionComponents[i];
            if (selectableComponent && selectableComponent->Selected && movementComponent)
            {
                HATCHER_ASSERT(positionComponent);
                movementComponent->Orientation =
                    glm::normalize(worldCoords2D - positionComponent->Position);
            }
        }
    }

    if (event.button.button == SDL_BUTTON_MIDDLE)
    {
        hatcher::Entity newEntity = entityManager->CreateNewEntity();
        Position2DComponent position2D{worldCoords2D};
        Movement2DComponent movement2D;
        movement2D.Orientation = glm::vec2(1.f, 0.f);
        movement2D.Speed = 0.f;
        Selectable2DComponent selectable2D;
        selectable2D.Selected = false;
        selectable2D.Box = hatcher::Box2f(glm::vec2(-1.f, -1.f), glm::vec2(1.f, 1.f));

        componentManager->AttachComponent<Position2DComponent>(newEntity, position2D);
        componentManager->AttachComponent<Movement2DComponent>(newEntity, movement2D);
        componentManager->AttachComponent<Selectable2DComponent>(newEntity, selectable2D);
    }
}

glm::mat4 EventHandlerUpdater::CalculateProjectionMatrix()
{
    const float halfWidth = m_windowWidth / 2.f * m_pixelSize;
    const float halfHeight = m_windowHeight / 2.f * m_pixelSize;

    const float right = m_fixedPosition.x + halfWidth;
    const float left = m_fixedPosition.x - halfWidth;
    const float bottom = m_fixedPosition.y - halfHeight;
    const float top = m_fixedPosition.y + halfHeight;
    return glm::ortho(left, right, bottom, top);
}
