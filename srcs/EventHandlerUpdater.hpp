#pragma once

#include "hatcher/Graphics/RenderUpdater.hpp"

namespace hatcher
{
class GameApplication;
}

class EventHandlerUpdater final : public hatcher::RenderUpdater
{
public:
    EventHandlerUpdater(hatcher::GameApplication* application);

    void Update(hatcher::ComponentManager* componentManager,
                hatcher::IFrameRenderer& frameRenderer) override;

    hatcher::GameApplication* m_application;
};
