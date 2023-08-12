#include "MaterialFactory.hpp"

#include "hatcher/FileSystem.hpp"
#include "hatcher/Graphics/Core/ShaderProgram.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/Texture.hpp"
#include "hatcher/assert.hpp"

namespace hatcher
{

MaterialFactory::MaterialFactory(const FileSystem* fileSystem)
    : m_fileSystem(fileSystem)
{
}

MaterialFactory::~MaterialFactory() = default;

unique_ptr<Material> MaterialFactory::CreateMaterial(const std::string& vertexShaderFileName,
                                                     const std::string& fragmentShaderFileName)
{
    ProgramKey key = ProgramKey(vertexShaderFileName, fragmentShaderFileName);

    if (m_shaderProgramLibrary.find(key) == m_shaderProgramLibrary.end())
    {
        const std::string pathToVertexFile = m_fileSystem->PathToFileName(vertexShaderFileName);
        const std::string pathToFragmentFile = m_fileSystem->PathToFileName(fragmentShaderFileName);
        m_shaderProgramLibrary[key] = make_unique<ShaderProgram>(pathToVertexFile.data(), pathToFragmentFile.data());
    }
    HATCHER_ASSERT(m_shaderProgramLibrary.find(key) != m_shaderProgramLibrary.end());
    return make_unique<Material>(m_shaderProgramLibrary[key].get());
}

const Texture* MaterialFactory::TextureFromFile(const std::string& fileName)
{
    if (m_textureLibrary.find(fileName) == m_textureLibrary.end())
    {
        const std::string pathToFile = m_fileSystem->PathToFileName(fileName);
        m_textureLibrary[fileName] = make_unique<Texture>(pathToFile.c_str());
    }
    HATCHER_ASSERT(m_textureLibrary.find(fileName) != m_textureLibrary.end());
    return m_textureLibrary[fileName].get();
}

} // namespace hatcher
