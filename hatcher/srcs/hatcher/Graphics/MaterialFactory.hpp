#pragma once

#include <map>
#include <string>
#include <utility> // std::pair

#include "hatcher/unique_ptr.hpp"

namespace hatcher
{

class Material;
class ShaderProgram;
class Texture;

class MaterialFactory
{
public:
    MaterialFactory();
    ~MaterialFactory();

    unique_ptr<Material> CreateMaterial(const std::string& vertexShaderFileName,
                                        const std::string& fragmentShaderFileName);
    const Texture* TextureFromFile(const std::string& fileName);

private:
    using ProgramKey = std::pair<std::string, std::string>;
    std::map<ProgramKey, unique_ptr<ShaderProgram>> m_shaderProgramLibrary;
    std::map<std::string, unique_ptr<Texture>> m_textureLibrary;
};

} // namespace hatcher
