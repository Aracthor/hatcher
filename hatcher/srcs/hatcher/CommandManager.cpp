#include "CommandManager.hpp"

#include "ICommand.hpp"

namespace hatcher
{

void CommandManager::AddCommand(ICommand* command)
{
    m_commands.emplace_back(command);
}

void CommandManager::ExecuteCommands(IEntityManager* entityManager, ComponentAccessor* componentAccessor)
{
    for (auto& command : m_commands)
    {
        command->Execute(entityManager, componentAccessor);
    }
    m_commands.clear();
}

} // namespace hatcher
