#include <SDL2/SDL_events.h>

#include "Camera.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/Clock.hpp"
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

        if (keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W])
            cameraMovement += cameraUp;
        if (keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S])
            cameraMovement -= cameraUp;
        if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D])
            cameraMovement += cameraRight;
        if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
            cameraMovement -= cameraRight;

        camera->target += glm::vec3(cameraMovement, 0.f) * movementAmplitude;
    }

    glm::mat4 CalculateProjectionMatrix(const Camera* camera,
                                        const IFrameRenderer& frameRenderer) const
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

const int dummy = RegisterRenderUpdater<CameraRenderUpdater>("Camera");

} // namespace