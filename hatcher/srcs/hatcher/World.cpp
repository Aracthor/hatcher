#include "World.hpp"

#include <vector>

#include "CommandManager.hpp"
#include "CommandSaveLoad.hpp"
#include "EntityManager.hpp"
#include "assert.hpp"

namespace hatcher
{

World::World(unique_ptr<EntityManager>&& entityManager, int64_t seed, const std::optional<std::string>& commandSaveFile,
             const std::optional<std::string>& commandLoadFile)
    : m_entityManager(std::move(entityManager))
{
    m_commandManager = make_unique<CommandManager>();
    if (commandSaveFile)
        m_commandSaver.emplace(new CommandSaver(*commandSaveFile));
    if (commandLoadFile)
        m_commandLoader.emplace(new CommandLoader(*commandLoadFile));
}

World::~World() = default;

void World::UpdateTickCommands()
{
    if (m_commandSaver)
    {
        for (const unique_ptr<ICommand>& command : m_commandManager->CurrentCommands())
        {
            (*m_commandSaver)->Save(m_tick, command.get());
        }
    }
    if (m_commandLoader)
    {
        for (ICommand* command : (*m_commandLoader)->CommandsForTick(m_tick))
        {
            m_commandManager->AddCommand(command);
        }
    }
    m_commandManager->ExecuteCommands(m_entityManager.get(), m_entityManager->GetComponentAccessor());
}

void World::IncrementTick()
{
    m_tick++;
}

} // namespace hatcher
