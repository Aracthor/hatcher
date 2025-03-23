#include "CommandSaveLoad.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "DataLoader.hpp"
#include "DataSaver.hpp"

namespace hatcher
{
namespace
{
std::unordered_map<uint, CommandCreator>& CommandCreators()
{
    static std::unordered_map<uint, CommandCreator> commandCreators;
    return commandCreators;
}
} // namespace

void RegisterCommand(uint identifier, CommandCreator commandCreator)
{
    HATCHER_ASSERT(CommandCreators().find(identifier) == CommandCreators().end());
    CommandCreators()[identifier] = commandCreator;
}

CommandSaver::CommandSaver(const std::string& fileName)
    : m_fileName(fileName)
    , m_dataSaver(new DataSaver())
{
}

CommandSaver::~CommandSaver()
{
    std::ofstream fstream(m_fileName, std::ios::binary);
    fstream.write(reinterpret_cast<const char*>(m_dataSaver->Result().data()), m_dataSaver->Result().size());
    if (fstream.bad())
    {
        std::cerr << "Error saving command file '" << m_fileName << "'" << std::endl;
        std::abort();
    }
}

void CommandSaver::Save(int tick, const ICommand* command)
{
    *m_dataSaver << tick;
    *m_dataSaver << command->Identifier();
    command->Save(*m_dataSaver);
}

CommandLoader::CommandLoader(const std::string& fileName)
{
    const std::uintmax_t fileSize = std::filesystem::file_size(fileName);
    std::vector<ubyte> fileData(fileSize);
    std::ifstream fstream(fileName, std::ios::binary);
    fstream.read(reinterpret_cast<char*>(fileData.data()), fileSize);
    if (fstream.bad())
    {
        std::cerr << "Error loading command file '" << fileName << "'" << std::endl;
        std::abort();
    }

    DataLoader m_dataLoader(fileData);
    while (!m_dataLoader.Empty())
    {
        int tick;
        uint identifier;
        m_dataLoader >> tick;
        m_dataLoader >> identifier;
        HATCHER_ASSERT(CommandCreators().find(identifier) != CommandCreators().end());
        unique_ptr<ICommand> command(CommandCreators()[identifier]());
        command->Load(m_dataLoader);
        m_commands[tick].emplace_back(std::move(command));
    }
}

std::vector<ICommand*> CommandLoader::CommandsForTick(int tick)
{
    std::vector<ICommand*> result;
    const auto it = m_commands.find(tick);
    if (it != m_commands.end())
    {
        for (unique_ptr<ICommand>& command : it->second)
            result.push_back(command.release());
    }
    return result;
}

} // namespace hatcher
