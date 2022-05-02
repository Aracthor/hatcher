#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility> // std::pair

namespace hatcher
{

class Material;
class ShaderProgram;

class MaterialFactory
{
public:
    std::shared_ptr<Material> CreateMaterial(const std::string& vertexShaderFileName,
                                             const std::string& fragmentShaderFileName);

private:
    using ProgramKey = std::pair<std::string, std::string>;
    std::map<ProgramKey, std::shared_ptr<ShaderProgram>> m_shaderProgramLibrary;
};

} // namespace hatcher
