#include "ApplicationConfiguration.hpp"

namespace hatcher
{

ApplicationConfiguration::ApplicationConfiguration(int argc, char** argv)
{
#ifndef __EMSCRIPTEN__
    const std::string commandLineStr = argv[0];
    const std::size_t lastSlash = commandLineStr.rfind('/');
    if (lastSlash != std::string::npos)
    {
        pathToProject = commandLineStr.substr(0, lastSlash + 1);
    }
    pathToProject += "../";
#endif
}

} // namespace hatcher
