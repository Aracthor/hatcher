#pragma once

#include <string>
#include <unordered_map>

#include "hatcher/unique_ptr.hpp"

namespace hatcher
{
class FileSystem;
class Material;
class Mesh;

struct MeshData;

class MeshLoader
{
public:
    explicit MeshLoader(const FileSystem* fileSystem);
    ~MeshLoader();

    unique_ptr<Mesh> LoadWavefront(const Material* material, const std::string& fileName);

private:
    const FileSystem* m_fileSystem;
    std::unordered_map<std::string, unique_ptr<MeshData>> m_dataLibrary;
};

} // namespace hatcher
