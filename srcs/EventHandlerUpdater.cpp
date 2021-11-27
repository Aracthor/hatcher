#include "EventHandlerUpdater.hpp"

#include <SDL2/SDL_events.h>

#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/GameApplication.hpp"
#include "hatcher/Graphics/Clock.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/assert.hpp"

EventHandlerUpdater::EventHandlerUpdater(hatcher::GameApplication* application)
    : m_application(application)
{
}

void EventHandlerUpdater::Update(hatcher::ComponentManager* componentManager,
                                 const hatcher::Clock& clock,
                                 hatcher::IFrameRenderer& frameRenderer)
{
    const glm::mat4 previousProjectionMatrix = CalculateProjectionMatrix();

    const float elapsedTime = clock.GetElapsedTime();
    const Uint8* keyState = SDL_GetKeyboardState(NULL);

    if (keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W])
        m_fixedPosition.y -= 0.01f * elapsedTime;
    if (keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S])
        m_fixedPosition.y += 0.01f * elapsedTime;
    if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D])
        m_fixedPosition.x += 0.01f * elapsedTime;
    if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
        m_fixedPosition.x -= 0.01f * elapsedTime;
    if (keyState[SDL_SCANCODE_ESCAPE])
        m_application->Stop();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            m_application->Stop();
        }

        if (event.type == SDL_MOUSEWHEEL)
        {
            int verticalScroll = event.wheel.y;

            // We don't use verticalScroll actual value because with emscripten,
            // depending of browsers, this value can make no sense...
            if (verticalScroll < 0)
                m_pixelSize *= 4.f / 3.f;
            else if (verticalScroll > 0)
                m_pixelSize *= 3.f / 4.f;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            const float windowWidth = 800;
            const float windowHeight = 600;

            glm::vec3 winCoords(event.button.x, windowHeight - event.button.y, 0.f);
            glm::mat4 modelMatrix = glm::mat4(1.f);
            glm::vec4 viewport = {0.f, 0.f, windowWidth, windowHeight};
            glm::vec3 worldCoords =
                glm::unProject(winCoords, modelMatrix, previousProjectionMatrix, viewport);

            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                hatcher::Entity newEntity = componentManager->CreateNewEntity();
                Position2DComponent position2D{static_cast<glm::vec2>(worldCoords)};
                componentManager->AttachComponent<Position2DComponent>(newEntity, position2D);
            }

            if (event.button.button == SDL_BUTTON_MIDDLE)
            {
                hatcher::Entity newEntity = componentManager->CreateNewEntity();
                Position2DComponent position2D{static_cast<glm::vec2>(worldCoords)};
                Movement2DComponent movement2D;
                movement2D.Orientation = glm::vec2(1.f, 0.f);
                movement2D.Speed = 0.f;
                componentManager->AttachComponent<Position2DComponent>(newEntity, position2D);
                componentManager->AttachComponent<Movement2DComponent>(newEntity, movement2D);
            }
        }
    }

    const glm::mat4 newProjectionMatrix = CalculateProjectionMatrix();
    frameRenderer.SetProjectionMatrix(newProjectionMatrix);
}

glm::mat4 EventHandlerUpdater::CalculateProjectionMatrix()
{
    const float halfWidth = m_windowWidth / 2.f * m_pixelSize;
    const float halfHeight = m_windowHeight / 2.f * m_pixelSize;

    const float right = m_fixedPosition.x + halfWidth;
    const float left = m_fixedPosition.x - halfWidth;
    const float bottom = m_fixedPosition.y + halfHeight;
    const float top = m_fixedPosition.y - halfHeight;
    return glm::ortho(left, right, bottom, top);
}
