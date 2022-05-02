#include "EventHandlerUpdater.hpp"

#include <SDL2/SDL_events.h>

#include "GridDisplay.hpp"
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

EventHandlerUpdater::EventHandlerUpdater(hatcher::GameApplication* application,
                                         const hatcher::IRendering* rendering)
    : m_application(application)
{
    m_eventFunctions[SDL_QUIT] = &EventHandlerUpdater::HandleQuitEvent;
    m_eventFunctions[SDL_MOUSEWHEEL] = &EventHandlerUpdater::HandleMouseWheelEvent;
    m_eventFunctions[SDL_MOUSEMOTION] = &EventHandlerUpdater::HandleMouseMotionEvent;
    m_eventFunctions[SDL_MOUSEBUTTONUP] = &EventHandlerUpdater::HandleMouseButtonUpEvent;
    m_eventFunctions[SDL_MOUSEBUTTONDOWN] = &EventHandlerUpdater::HandleMouseButtonDownEvent;
    m_eventFunctions[SDL_KEYDOWN] = &EventHandlerUpdater::HandleKeyDownEvent;

    m_selectionHandler = std::make_unique<SelectionRectangleHandler>(rendering);
    m_gridDisplay = std::make_unique<GridDisplay>(rendering);
}

EventHandlerUpdater::~EventHandlerUpdater() = default;

void EventHandlerUpdater::HandleEvents(const hatcher::span<const SDL_Event>& events,
                                       hatcher::IEntityManager* entityManager,
                                       hatcher::ComponentManager* componentManager,
                                       hatcher::IFrameRenderer& frameRenderer,
                                       const hatcher::IRendering& rendering)
{
    const Uint8* keyState = SDL_GetKeyboardState(NULL);

    HandleCameraMotion(frameRenderer.GetClock(), keyState);

    frameRenderer.SetProjectionMatrix(CalculateProjectionMatrix(frameRenderer));

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
    frameRenderer.SetViewMatrix(glm::lookAt(m_cameraPosition, m_cameraTarget, m_cameraUp));

    if (keyState[SDL_SCANCODE_ESCAPE])
        m_application->Stop();

    for (const SDL_Event& event : events)
    {
        auto functionIt = m_eventFunctions.find(event.type);
        if (functionIt != m_eventFunctions.end())
        {
            EventHandlerFunction handlerFunction = functionIt->second;
            (this->*handlerFunction)(event, entityManager, componentManager, frameRenderer);
        }
    }

    m_selectionHandler->DrawSelectionRectangle(frameRenderer);
    m_gridDisplay->DrawGrid(frameRenderer, m_cameraTarget.x, m_cameraTarget.y);
}

void EventHandlerUpdater::HandleCameraMotion(const hatcher::Clock* clock, const Uint8* keyState)
{
    const float elapsedTime = clock->GetElapsedTime();
    const float movementAmplitude = elapsedTime * m_pixelSize;
    const glm::vec2 cameraUp = m_cameraUp;
    const glm::vec2 cameraRight = {m_cameraUp.y, -m_cameraUp.x};
    glm::vec2 cameraMovement = glm::vec2(0.f);

    if (keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W])
        cameraMovement += cameraUp;
    if (keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S])
        cameraMovement -= cameraUp;
    if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D])
        cameraMovement += cameraRight;
    if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
        cameraMovement -= cameraRight;

    m_cameraTarget += glm::vec3(cameraMovement, 0.f) * movementAmplitude;
}

void EventHandlerUpdater::HandleQuitEvent(const SDL_Event& event,
                                          hatcher::IEntityManager* entityManager,
                                          hatcher::ComponentManager* componentManager,
                                          const hatcher::IFrameRenderer& frameRenderer)
{
    m_application->Stop();
}

void EventHandlerUpdater::HandleMouseWheelEvent(const SDL_Event& event,
                                                hatcher::IEntityManager* entityManager,
                                                hatcher::ComponentManager* componentManager,
                                                const hatcher::IFrameRenderer& frameRenderer)
{
    int verticalScroll = event.wheel.y;

    // We don't use verticalScroll actual value because with emscripten,
    // depending of browsers, this value can make no sense...
    if (verticalScroll < 0)
        m_pixelSize *= 4.f / 3.f;
    else if (verticalScroll > 0)
        m_pixelSize *= 3.f / 4.f;

    m_pixelSize = std::clamp(m_pixelSize, 0.001f, 0.1f);
}

void EventHandlerUpdater::HandleMouseMotionEvent(const SDL_Event& event,
                                                 hatcher::IEntityManager* entityManager,
                                                 hatcher::ComponentManager* componentManager,
                                                 const hatcher::IFrameRenderer& frameRenderer)
{
    if (m_selectionHandler->IsSelecting())
    {
        const glm::vec2 winCoords = {event.motion.x, frameRenderer.Resolution().y - event.motion.y};
        m_selectionHandler->MoveSelection(winCoords);
    }
}

void EventHandlerUpdater::HandleMouseButtonUpEvent(const SDL_Event& event,
                                                   hatcher::IEntityManager* entityManager,
                                                   hatcher::ComponentManager* componentManager,
                                                   const hatcher::IFrameRenderer& frameRenderer)
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
                const glm::mat4 modelMatrix =
                    glm::translate(glm::vec3(positionComponent->position, 0.f));
                const hatcher::Box2f selectionBox =
                    frameRenderer.ProjectBox3DToWindowCoords(selectableComponent->box, modelMatrix);
                selectableComponent->selected = selectionRectangle.Touches(selectionBox);
            }
        }

        m_selectionHandler->EndSelection();
    }
}

void EventHandlerUpdater::HandleMouseButtonDownEvent(const SDL_Event& event,
                                                     hatcher::IEntityManager* entityManager,
                                                     hatcher::ComponentManager* componentManager,
                                                     const hatcher::IFrameRenderer& frameRenderer)
{
    const glm::vec2 worldCoords2D =
        MouseCoordsToWorldCoords(event.button.x, event.button.y, frameRenderer);

    if (event.button.button == SDL_BUTTON_LEFT)
    {
        const glm::vec2 winCoords = {event.motion.x, frameRenderer.Resolution().y - event.motion.y};
        m_selectionHandler->StartSelection(winCoords);
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
            if (selectableComponent && selectableComponent->selected && movementComponent)
            {
                HATCHER_ASSERT(positionComponent);
                movementComponent->orientation =
                    glm::normalize(worldCoords2D - positionComponent->position);
                movementComponent->path = {worldCoords2D};
            }
        }
    }

    if (event.button.button == SDL_BUTTON_MIDDLE)
    {
        hatcher::Entity newEntity = entityManager->CreateNewEntity();
        Position2DComponent position2D{worldCoords2D};
        Movement2DComponent movement2D;
        movement2D.orientation = glm::vec2(1.f, 0.f);
        movement2D.speed = 0.f;
        Selectable2DComponent selectable2D;
        selectable2D.selected = false;
        selectable2D.box = hatcher::Box3f(glm::vec3(-1.f, -1.f, 0.f), glm::vec3(1.f, 1.f, 1.f));

        componentManager->AttachComponent<Position2DComponent>(newEntity, position2D);
        componentManager->AttachComponent<Movement2DComponent>(newEntity, movement2D);
        componentManager->AttachComponent<Selectable2DComponent>(newEntity, selectable2D);
    }
}

void EventHandlerUpdater::HandleKeyDownEvent(const SDL_Event& event,
                                             hatcher::IEntityManager* entityManager,
                                             hatcher::ComponentManager* componentManager,
                                             const hatcher::IFrameRenderer& frameRenderer)
{
    if (event.key.keysym.scancode == SDL_SCANCODE_U)
    {
        m_gridDisplay->SetEnabled(!m_gridDisplay->Enabled());
    }
}

glm::mat4
EventHandlerUpdater::CalculateProjectionMatrix(const hatcher::IFrameRenderer& frameRenderer)
{
    const glm::ivec2 resolution = frameRenderer.Resolution();
    const float halfWidth = resolution.x / 2.f * m_pixelSize;
    const float halfHeight = resolution.y / 2.f * m_pixelSize;

    const float right = halfWidth;
    const float left = -halfWidth;
    const float bottom = -halfHeight;
    const float top = halfHeight;
    const float zNear = 0.1f;
    const float zFar = 1000.f;
    return glm::ortho(left, right, bottom, top, zNear, zFar);
}

glm::vec2
EventHandlerUpdater::MouseCoordsToWorldCoords(int x, int y,
                                              const hatcher::IFrameRenderer& frameRenderer) const
{
    const glm::vec3 worldCoords = frameRenderer.WindowCoordsToWorldCoords(glm::vec2(x, y));
    const glm::vec3 cameraToTarget = m_cameraPosition - m_cameraTarget;
    const float t = worldCoords.z / cameraToTarget.z;
    const glm::vec3 projectedWorldCoords = worldCoords - cameraToTarget * t;
    return static_cast<glm::vec2>(projectedWorldCoords);
}
