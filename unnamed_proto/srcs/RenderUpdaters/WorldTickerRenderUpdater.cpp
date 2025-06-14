#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/IApplication.hpp"

using namespace hatcher;

namespace
{
class WorldTickerRenderUpdater final : public RenderUpdater
{
public:
    WorldTickerRenderUpdater(const IRendering* rendering) {}

private:
    void Update(IApplication* application, const ComponentManager* componentManager,
                ComponentManager* renderComponentManager, IFrameRenderer& frameRenderer) override
    {
    }

    void GetEvent(const SDL_Event& event, IApplication* application, ICommandManager* commandManager,
                  const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                  const IFrameRenderer& frameRenderer) override
    {
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_KP_PLUS)
            {
                m_tickrate *= 2.f;
                application->SetUpdateTickrate(m_tickrate);
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_KP_MINUS)
            {
                m_tickrate /= 2.f;
                application->SetUpdateTickrate(m_tickrate);
            }
        }
    }

    float m_tickrate = 60.f;
};

RenderUpdaterRegisterer<WorldTickerRenderUpdater> registerer;

} // namespace