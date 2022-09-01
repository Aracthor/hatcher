#include <algorithm> // std::clamp

#include "Camera.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/assert.hpp"

namespace
{

using namespace hatcher;

class CameraEventListener final : public IEventListener
{
public:
    void GetEvent(const SDL_Event& event, IEntityManager* entityManager,
                  ComponentManager* componentManager, ComponentManager* renderComponentManager,
                  const IFrameRenderer& frameRenderer) override
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
    bool m_verticalMode = true;
};

const int dummy = RegisterEventListener<CameraEventListener>("Camera");

} // namespace