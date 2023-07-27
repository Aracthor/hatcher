#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility> // std::pair

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

    std::unique_ptr<Material> CreateMaterial(const std::string& vertexShaderFileName,
                                             const std::string& fragmentShaderFileName);
    const Texture* TextureFromFile(const std::string& fileName);

private:
    using ProgramKey = std::pair<std::string, std::string>;
    std::map<ProgramKey, std::shared_ptr<ShaderProgram>> m_shaderProgramLibrary;
    std::map<std::string, std::unique_ptr<Texture>> m_textureLibrary;
};

} // namespace hatcher
