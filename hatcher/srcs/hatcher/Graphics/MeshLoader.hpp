#pragma once

#include <string>

#include "hatcher/unique_ptr.hpp"

namespace hatcher
{
class FileSystem;
class Material;
class Mesh;

class MeshLoader
{
public:
    explicit MeshLoader(const FileSystem* fileSystem);

    unique_ptr<Mesh> LoadWavefront(const Material* material, const std::string& fileName) const;

private:
    const FileSystem* m_fileSystem;
};

} // namespace hatcher
