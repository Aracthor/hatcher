#pragma once

#include <optional>
#include <string>
#include <vector>

#include "WorldSettings.hpp"
#include "span.hpp"
#include "unique_ptr.hpp"

union SDL_Event;
namespace hatcher
{
class CommandLoader;
class CommandManager;
class CommandSaver;
class EntityManager;
class EventUpdater;
class IFrameRenderer;
class IRendering;
class RenderUpdater;
class Updater;

class World final
{
public:
    World(int64_t seed, const std::optional<std::string>& commandSaveFile,
          const std::optional<std::string>& commandLoadFile);
    ~World();

    void CreateRenderUpdaters(const IRendering* rendering);

    void Update();
    void UpdateFromEvents(span<const SDL_Event> events, IFrameRenderer& frameRenderer);
    void UpdateRendering(IFrameRenderer& frameRenderer);

private:
    int m_tick = 0;
    WorldSettings m_settings;

    unique_ptr<EntityManager> m_entityManager;

    std::vector<unique_ptr<Updater>> m_updaters;
    unique_ptr<CommandManager> m_commandManager;
    std::optional<unique_ptr<CommandSaver>> m_commandSaver;
    std::optional<unique_ptr<CommandLoader>> m_commandLoader;

    unique_ptr<EventUpdater> m_eventUpdater;
    std::vector<unique_ptr<RenderUpdater>> m_renderUpdaters;
};

} // namespace hatcher
