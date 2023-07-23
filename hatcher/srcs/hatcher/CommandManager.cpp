#include "CommandManager.hpp"

#include "ICommand.hpp"

namespace hatcher
{

void CommandManager::AddCommand(ICommand* command)
{
    m_commands.emplace_back(command);
}

void CommandManager::ExecuteCommands(IEntityManager* entityManager, ComponentManager* componentManager,
                                     ComponentManager* renderingComponentManager)
{
    for (auto& command : m_commands)
    {
        command->Execute(entityManager, componentManager, renderingComponentManager);
    }
    m_commands.clear();
}

} // namespace hatcher
