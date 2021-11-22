#pragma once

#include "hatcher/Updater.hpp"

namespace hatcher
{
class GameApplication;
}

class EventHandlerUpdater final : public hatcher::Updater
{
public:
    EventHandlerUpdater(hatcher::GameApplication* application);

    void Update(hatcher::ComponentManager* componentManager) override;

    hatcher::GameApplication* m_application;
};
