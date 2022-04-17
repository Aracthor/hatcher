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

glm::vec2 MouseCoordsToWorldCoords(int x, int y, const glm::vec3& cameraPosition,
                                   const glm::vec3& cameraTarget,
                                   const glm::mat4& previousViewMatrix,
                                   const glm::mat4& previousProjectionMatrix)
{
    const float windowWidth = 800;
    const float windowHeight = 600;

    const glm::vec3 winCoords(x, windowHeight - y, 0.f);
    const glm::mat4 modelViewMatrix = previousViewMatrix;
    const glm::vec4 viewport = {0.f, 0.f, windowWidth, windowHeight};
    const glm::vec3 worldCoords =
        glm::unProject(winCoords, modelViewMatrix, previousProjectionMatrix, viewport);

    const glm::vec3 cameraToTarget = cameraPosition - cameraTarget;
    const float t = worldCoords.z / cameraToTarget.z;
    const glm::vec3 projectedWorldCoords = worldCoords - cameraToTarget * t;
    return static_cast<glm::vec2>(projectedWorldCoords);
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

void EventHandlerUpdater::HandleEvents(const hatcher::span<const SDL_Event>& events,
                                       hatcher::IEntityManager* entityManager,
                                       hatcher::ComponentManager* componentManager,
                                       const hatcher::Clock& clock,
                                       hatcher::IFrameRenderer& frameRenderer)
{
    const float elapsedTime = clock.GetElapsedTime();
    const Uint8* keyState = SDL_GetKeyboardState(NULL);

    if (keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W])
        m_cameraTarget.y += 0.01f * elapsedTime;
    if (keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S])
        m_cameraTarget.y -= 0.01f * elapsedTime;
    if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D])
        m_cameraTarget.x += 0.01f * elapsedTime;
    if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
        m_cameraTarget.x -= 0.01f * elapsedTime;
    if (keyState[SDL_SCANCODE_ESCAPE])
        m_application->Stop();

    for (const SDL_Event& event : events)
    {
        auto functionIt = m_eventFunctions.find(event.type);
        if (functionIt != m_eventFunctions.end())
        {
            EventHandlerFunction handlerFunction = functionIt->second;
            (this->*handlerFunction)(event, entityManager, componentManager);
        }
    }

    m_selectionHandler->DrawSelectionRectangle(frameRenderer);

    m_projectionMatrix = CalculateProjectionMatrix();
    frameRenderer.SetProjectionMatrix(m_projectionMatrix);

    if (keyState[SDL_SCANCODE_F])
    {
        m_cameraPosition = glm::vec3(-90, -90, 100);
        m_cameraUp = glm::vec3(sqrtf(2.f) / 2.f, sqrtf(2.f) / 2.f, 0.f);
    }
    else
    {
        m_cameraPosition = glm::vec3(0, 0, 100);
        m_cameraUp = glm::vec3(0, 1, 0);
    }
    m_cameraPosition += m_cameraTarget;
    m_viewMatrix = glm::lookAt(m_cameraPosition, m_cameraTarget, m_cameraUp);
    frameRenderer.SetViewMatrix(m_viewMatrix);
}

void EventHandlerUpdater::HandleQuitEvent(const SDL_Event& event,
                                          hatcher::IEntityManager* entityManager,
                                          hatcher::ComponentManager* componentManager)
{
    m_application->Stop();
}

void EventHandlerUpdater::HandleMouseWheelEvent(const SDL_Event& event,
                                                hatcher::IEntityManager* entityManager,
                                                hatcher::ComponentManager* componentManager)
{
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
                                                 hatcher::ComponentManager* componentManager)
{
    if (m_selectionHandler->IsSelecting())
    {
        const glm::vec2 worldCoords2D =
            MouseCoordsToWorldCoords(event.motion.x, event.motion.y, m_cameraPosition,
                                     m_cameraTarget, m_viewMatrix, m_projectionMatrix);

        m_selectionHandler->MoveSelection(worldCoords2D);
    }
}

void EventHandlerUpdater::HandleMouseButtonUpEvent(const SDL_Event& event,
                                                   hatcher::IEntityManager* entityManager,
                                                   hatcher::ComponentManager* componentManager)
{
    if (event.button.button == SDL_BUTTON_LEFT)
    {
        hatcher::span<std::optional<Selectable2DComponent>> selectableComponents =
            componentManager->GetComponents<Selectable2DComponent>();
        hatcher::span<std::optional<Position2DComponent>> positionComponents =
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
                                                     hatcher::ComponentManager* componentManager)
{
    const glm::vec2 worldCoords2D =
        MouseCoordsToWorldCoords(event.button.x, event.button.y, m_cameraPosition, m_cameraTarget,
                                 m_viewMatrix, m_projectionMatrix);

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
                movementComponent->Path = {worldCoords2D};
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

    const float right = halfWidth;
    const float left = -halfWidth;
    const float bottom = -halfHeight;
    const float top = halfHeight;
    const float zNear = 0.1f;
    const float zFar = 1000.f;
    return glm::ortho(left, right, bottom, top, zNear, zFar);
}
