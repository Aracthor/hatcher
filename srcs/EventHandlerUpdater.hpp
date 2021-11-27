#pragma once

#include "hatcher/glm_pure.hpp"

#include "hatcher/Graphics/IEventUpdater.hpp"

namespace hatcher
{
class GameApplication;
}

class EventHandlerUpdater final : public hatcher::IEventUpdater
{
public:
    EventHandlerUpdater(hatcher::GameApplication* application);

    void Update(hatcher::ComponentManager* componentManager,
                hatcher::IFrameRenderer& frameRenderer) override;

private:
    void SendProjectionMatrix(hatcher::IFrameRenderer& frameRenderer);

    hatcher::GameApplication* m_application;

    glm::vec2 m_fixedPosition = glm::vec2(0.f, 0.f);

    float m_windowWidth = 800.f;
    float m_windowHeight = 600.f;
    float m_pixelSize = 0.01f;
};
