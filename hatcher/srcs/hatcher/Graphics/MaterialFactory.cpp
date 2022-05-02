#include "MaterialFactory.hpp"

#include "hatcher/Graphics/Core/ShaderProgram.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/assert.hpp"

namespace hatcher
{

std::shared_ptr<Material> MaterialFactory::CreateMaterial(const std::string& vertexShaderFileName,
                                                          const std::string& fragmentShaderFileName)
{
    ProgramKey key = ProgramKey(vertexShaderFileName, fragmentShaderFileName);

    if (m_shaderProgramLibrary.find(key) == m_shaderProgramLibrary.end())
    {
        m_shaderProgramLibrary[key] = std::make_shared<ShaderProgram>(
            vertexShaderFileName.data(), fragmentShaderFileName.data());
    }
    HATCHER_ASSERT(m_shaderProgramLibrary.find(key) != m_shaderProgramLibrary.end());
    return std::make_unique<Material>(m_shaderProgramLibrary[key]);
}

} // namespace hatcher
