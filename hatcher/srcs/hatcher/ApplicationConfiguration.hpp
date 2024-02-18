#pragma once

#include <string>

namespace hatcher
{
struct ApplicationConfiguration
{
    ApplicationConfiguration(int argc, char** argv);

    std::string pathToProject;
};
} // namespace hatcher
