#include <SDL2/SDL_events.h>

#include "Camera.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/Clock.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/IEventUpdater.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"

using namespace hatcher;

namespace
{
class CameraEventListener final : public IEventListener
{
public:
    void GetEvent(const SDL_Event& event, IEntityManager* entityManager, ComponentManager* componentManager,
                  ComponentManager* renderComponentManager, const IFrameRenderer& frameRenderer) override
    {
        Camera* camera = renderComponentManager->WriteWorldComponent<Camera>();
        if (event.type == SDL_MOUSEWHEEL)
        {
            const int verticalScroll = event.wheel.y;

            // We don't use verticalScroll actual value because with emscripten,
            // depending of browsers, this value can make no sense...
            if (verticalScroll < 0)
                camera->pixelSize *= 4.f / 3.f;
            else if (verticalScroll > 0)
                camera->pixelSize *= 3.f / 4.f;

            camera->pixelSize = std::clamp(camera->pixelSize, 0.001f, 0.1f);
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F)
        {
            m_verticalMode = !m_verticalMode;
            if (m_verticalMode)
            {
                camera->position = glm::vec3(0, 0, 100);
                camera->up = glm::vec3(0, 1, 0);
            }
            else
            {
                camera->position = glm::vec3(-90, -90, 100);
                camera->up = glm::vec3(sqrtf(2.f) / 2.f, sqrtf(2.f) / 2.f, 0.f);
            }
            camera->position += camera->target;
        }
    }

    span<const SDL_EventType> EventTypesToListen() const override
    {
        static const SDL_EventType events[] = {
            SDL_MOUSEWHEEL,
            SDL_KEYDOWN,
        };
        return span<const SDL_EventType>(events, std::size(events));
    }

private:
    bool m_verticalMode = false;
};

class CameraRenderUpdater final : public RenderUpdater
{
public:
    CameraRenderUpdater(const IRendering* rendering, IEventUpdater* eventUpdater)
    {
        eventUpdater->RegisterListener(std::make_shared<CameraEventListener>());
    }

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        Camera* camera = renderComponentManager->WriteWorldComponent<Camera>();

        HandleCameraMotion(camera, frameRenderer.GetClock());

        frameRenderer.SetProjectionMatrix(CalculateProjectionMatrix(camera, frameRenderer));
        frameRenderer.SetViewMatrix(glm::lookAt(camera->position, camera->target, camera->up));
    }

private:
    void HandleCameraMotion(Camera* camera, const Clock* clock) const
    {
        const Uint8* keyState = SDL_GetKeyboardState(NULL);

        const float elapsedTime = clock->GetElapsedTime();
        const float movementAmplitude = elapsedTime * camera->pixelSize;
        const glm::vec2 cameraUp = camera->up;
        const glm::vec2 cameraRight = {cameraUp.y, -cameraUp.x};
        glm::vec2 cameraMovement = glm::vec2(0.f);

        if (!keyState[SDL_SCANCODE_LCTRL] && !keyState[SDL_SCANCODE_RCTRL])
        {
            if (keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W])
                cameraMovement += cameraUp;
            if (keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S])
                cameraMovement -= cameraUp;
            if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D])
                cameraMovement += cameraRight;
            if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
                cameraMovement -= cameraRight;
        }

        cameraMovement *= movementAmplitude;
        camera->position += glm::vec3(cameraMovement, 0.f);
        camera->target += glm::vec3(cameraMovement, 0.f);
    }

    glm::mat4 CalculateProjectionMatrix(const Camera* camera, const IFrameRenderer& frameRenderer) const
    {
        const glm::ivec2 resolution = frameRenderer.Resolution();
        const float halfWidth = resolution.x / 2.f * camera->pixelSize;
        const float halfHeight = resolution.y / 2.f * camera->pixelSize;

        const float right = halfWidth;
        const float left = -halfWidth;
        const float bottom = -halfHeight;
        const float top = halfHeight;
        const float zNear = 0.1f;
        const float zFar = 1000.f;
        return glm::ortho(left, right, bottom, top, zNear, zFar);
    }
};

const int dummy = RegisterRenderUpdater<CameraRenderUpdater>();

} // namespace