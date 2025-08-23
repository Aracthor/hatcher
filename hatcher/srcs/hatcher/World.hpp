#pragma once

#include <optional>
#include <string>
#include <vector>

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
class IApplication;
class IFrameRenderer;
class IRendering;
class RenderUpdater;
class Updater;

class World final
{
public:
    World(unique_ptr<EntityManager>&& entityManager, int64_t seed, const std::optional<std::string>& commandSaveFile,
          const std::optional<std::string>& commandLoadFile);
    ~World();

    EntityManager* GetEntityManager() { return m_entityManager.get(); }
    CommandManager* GetCommandManager() { return m_commandManager.get(); }

    void UpdateTickCommands();
    void IncrementTick();

    int CurrentTick() const { return m_tick; }

private:
    int m_tick = 0;

    unique_ptr<EntityManager> m_entityManager;

    unique_ptr<CommandManager> m_commandManager;
    std::optional<unique_ptr<CommandSaver>> m_commandSaver;
    std::optional<unique_ptr<CommandLoader>> m_commandLoader;
};

} // namespace hatcher
