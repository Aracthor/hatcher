#include "FileSystem.hpp"

namespace hatcher
{

FileSystem::FileSystem(const std::string& pathToProject)
    : m_pathToProject(pathToProject)
{
}

std::string FileSystem::PathToFileName(const std::string& fileName) const
{
    return m_pathToProject + fileName;
}

} // namespace hatcher
