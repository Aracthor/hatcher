#include "FileSystem.hpp"

namespace hatcher
{

FileSystem::FileSystem(const char* commandLine)
{
#ifndef __EMSCRIPTEN__
    if (commandLine)
    {
        const std::string commandLineStr = commandLine;
        const std::size_t lastSlash = commandLineStr.rfind('/');
        if (lastSlash != std::string::npos)
        {
            m_pathToProject = commandLineStr.substr(0, lastSlash + 1);
        }
    }
    m_pathToProject += "../";
#endif
}

std::string FileSystem::PathToFileName(const std::string& fileName) const
{
    return m_pathToProject + fileName;
}

} // namespace hatcher
