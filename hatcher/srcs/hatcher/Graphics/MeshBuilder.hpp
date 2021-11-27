#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "hatcher/basic_types.hpp"

#include "Primitive.hpp"

namespace hatcher
{

class Mesh;
class ShaderProgram;

class MeshBuilder
{
public:
    void SetProgram(const std::string& vertexShaderFileName,
                    const std::string& fragmentShaderFileName);

    void SetPrimitive(Primitive::Type type);
    void SetPositions(float* positions, uint positionCount);
    void SetIndices(ushort* indices, uint indexCount);

    Mesh* Create();

private:
    using ProgramKey = std::pair<std::string, std::string>;
    std::map<ProgramKey, std::shared_ptr<ShaderProgram>> m_shaderProgramLibrary;

    std::optional<Primitive::Type> m_primitive;
    std::shared_ptr<const ShaderProgram> m_programToUse;
    std::vector<float> m_positions;
    std::vector<ushort> m_indices;
};

} // namespace hatcher
