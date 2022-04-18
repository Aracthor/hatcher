#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>

#include "hatcher/basic_types.hpp"

#include "Primitive.hpp"

namespace hatcher
{

class Material;
class Mesh;
class ShaderProgram;

class MeshBuilder
{
public:
    std::shared_ptr<Material> CreateMaterial(const std::string& vertexShaderFileName,
                                             const std::string& fragmentShaderFileName);

    void SetDynamic();
    void SetPrimitive(Primitive::Type type);
    void SetMaterial(const std::shared_ptr<const Material>& material);

    Mesh* Create();

private:
    using ProgramKey = std::pair<std::string, std::string>;
    std::map<ProgramKey, std::shared_ptr<ShaderProgram>> m_shaderProgramLibrary;

    bool m_dynamic = false;
    std::optional<Primitive::Type> m_primitive;
    std::shared_ptr<const Material> m_material;
};

} // namespace hatcher
