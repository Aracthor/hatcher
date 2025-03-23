#pragma once

#include <string>
#include <unordered_map>

#include "CommandRegisterer.hpp"

#include "unique_ptr.hpp"

namespace hatcher
{
class DataLoader;
class DataSaver;

class CommandSaver
{
public:
    CommandSaver(const std::string& fileName);
    ~CommandSaver();

    void Save(int tick, const ICommand* command);

private:
    const std::string m_fileName;
    unique_ptr<DataSaver> m_dataSaver;
};

class CommandLoader
{
public:
    CommandLoader(const std::string& fileName);

    std::vector<ICommand*> CommandsForTick(int tick);

private:
    std::unordered_map<uint, std::vector<unique_ptr<ICommand>>> m_commands;
};

} // namespace hatcher
