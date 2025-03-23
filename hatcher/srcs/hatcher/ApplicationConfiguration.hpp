#pragma once

#include <optional>
#include <string>

namespace hatcher
{
struct ApplicationConfiguration
{
    ApplicationConfiguration(int argc, char** argv);

    std::string pathToProject;
    std::optional<std::string> commandSaveFile;
    std::optional<std::string> commandLoadFile;
};
} // namespace hatcher
