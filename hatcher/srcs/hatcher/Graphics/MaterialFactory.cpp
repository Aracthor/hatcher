#include "MaterialFactory.hpp"

#include "hatcher/Graphics/Core/ShaderProgram.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/Texture.hpp"
#include "hatcher/assert.hpp"

namespace hatcher
{

MaterialFactory::MaterialFactory() = default;
MaterialFactory::~MaterialFactory() = default;

std::unique_ptr<Material> MaterialFactory::CreateMaterial(const std::string& vertexShaderFileName,
                                                          const std::string& fragmentShaderFileName)
{
    ProgramKey key = ProgramKey(vertexShaderFileName, fragmentShaderFileName);

    if (m_shaderProgramLibrary.find(key) == m_shaderProgramLibrary.end())
    {
        m_shaderProgramLibrary[key] =
            std::make_unique<ShaderProgram>(vertexShaderFileName.data(), fragmentShaderFileName.data());
    }
    HATCHER_ASSERT(m_shaderProgramLibrary.find(key) != m_shaderProgramLibrary.end());
    return std::make_unique<Material>(m_shaderProgramLibrary[key].get());
}

const Texture* MaterialFactory::TextureFromFile(const std::string& fileName)
{
    if (m_textureLibrary.find(fileName) == m_textureLibrary.end())
    {
        m_textureLibrary[fileName] = std::make_unique<Texture>(fileName.c_str());
    }
    HATCHER_ASSERT(m_textureLibrary.find(fileName) != m_textureLibrary.end());
    return m_textureLibrary[fileName].get();
}

} // namespace hatcher
