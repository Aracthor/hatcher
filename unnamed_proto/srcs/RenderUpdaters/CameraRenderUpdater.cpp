#include <SDL2/SDL_events.h>

#include "WorldComponents/Camera.hpp"

#include "hatcher/Clock.hpp"
#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"

using namespace hatcher;

namespace
{

class CameraRenderUpdater final : public RenderUpdater
{
public:
    CameraRenderUpdater(const IRendering* rendering) {}

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        Camera* camera = renderComponentManager->WriteWorldComponent<Camera>();

        HandleCameraMotion(camera, frameRenderer.GetClock());

        frameRenderer.SetProjectionMatrix(CalculateProjectionMatrix(camera, frameRenderer));
        frameRenderer.SetViewMatrix(glm::lookAt(camera->Position(), camera->Target(), camera->Up()));
    }

    void GetEvent(const SDL_Event& event, ICommandManager* commandManager, const ComponentManager* componentManager,
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
        else if (event.type == SDL_MOUSEMOTION && event.motion.state & SDL_BUTTON_MIDDLE)
        {
            const float cameraRotationSpeed = 0.01f;
            camera->angles.x -= cameraRotationSpeed * event.motion.xrel;
            camera->angles.y -= cameraRotationSpeed * event.motion.yrel;
            camera->angles.y = std::clamp(camera->angles.y, 0.f, float(M_PI) / 2.f);
        }
    }

private:
    void HandleCameraMotion(Camera* camera, const Clock* clock) const
    {
        const Uint8* keyState = SDL_GetKeyboardState(NULL);

        const float elapsedTime = clock->GetElapsedTime();
        const float movementAmplitude = elapsedTime * camera->pixelSize;
        const glm::vec2 cameraUp = camera->Up();
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

RenderUpdaterRegisterer<CameraRenderUpdater> registerer;

} // namespace