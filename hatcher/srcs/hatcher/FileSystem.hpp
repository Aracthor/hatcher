#pragma once

#include <string>

namespace hatcher
{

class FileSystem
{
public:
    FileSystem(const std::string& pathToProject);

    std::string PathToFileName(const std::string& fileName) const;

private:
    std::string m_pathToProject;
};

} // namespace hatcher
