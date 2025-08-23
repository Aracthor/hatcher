#pragma once

#include <optional>
#include <string>
#include <vector>

#include "span.hpp"
#include "unique_ptr.hpp"

union SDL_Event;
namespace hatcher
{
class EventUpdater;
class IApplication;
class IFrameRenderer;
class IRendering;
class RenderUpdater;
class Updater;
class World;

class WorldManager
{
public:
    WorldManager();
    ~WorldManager();

    void CreateRenderUpdaters(const IRendering* rendering);

    unique_ptr<World> CreateWorld(int64_t seed, const std::optional<std::string>& commandSaveFile,
                                  const std::optional<std::string>& commandLoadFile) const;

    void Update(World* world);
    void UpdateFromEvents(span<const SDL_Event> events, IApplication* application, World* world,
                          const IFrameRenderer& frameRenderer);
    void UpdateRendering(IApplication* application, World* world, IFrameRenderer& frameRenderer);

private:
    std::vector<unique_ptr<Updater>> m_updaters;

    unique_ptr<EventUpdater> m_eventUpdater;
    std::vector<unique_ptr<RenderUpdater>> m_renderUpdaters;
};

} // namespace hatcher
