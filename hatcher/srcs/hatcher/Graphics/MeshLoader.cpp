#include "MeshLoader.hpp"

#include <algorithm> // std::find
#include <fstream>
#include <optional>
#include <sstream>
#include <vector>

#include "Mesh.hpp"
#include "hatcher/FileSystem.hpp"
#include "hatcher/assert.hpp"

namespace hatcher
{
namespace
{
struct MeshData
{
    struct Vertex
    {
        glm::vec3 position;
        std::optional<glm::vec2> textureCoord;

        bool operator==(const Vertex& other) const
        {
            return position == other.position && textureCoord == other.textureCoord;
        }
    };
    std::vector<Vertex> vertices;
    std::vector<ushort> indices;
};

std::string getNextToken(std::string& line, const std::string& fileName)
{
    if (line.empty())
        throw std::runtime_error(std::string("Missing token on wavefront file '") + fileName + "'.");

    std::string token;
    const std::size_t tokenEnd = line.find(' ');
    if (tokenEnd == std::string::npos)
    {
        token = line;
        line = "";
    }
    else
    {
        token = line.substr(0, tokenEnd);
        line = line.substr(tokenEnd + 1, line.size() - tokenEnd - 1);
    }
    return token;
}

int getOrCreateVertex(std::vector<MeshData::Vertex>& vertices, glm::vec3 position, std::optional<glm::vec2> texCoord)
{
    MeshData::Vertex vertex({position, texCoord});
    auto it = std::find(vertices.begin(), vertices.end(), vertex);
    if (it != vertices.end())
    {
        return it - vertices.begin();
    }
    vertices.push_back(vertex);
    return vertices.size() - 1;
}

MeshData readFile(const std::string& fileName, Primitive::Type primitive)
{
    HATCHER_ASSERT(primitive == Primitive::Lines || primitive == Primitive::Triangles);
    MeshData meshData;
    std::ifstream ifs;
    ifs.exceptions(std::ios::failbit | std::ios::badbit);
    ifs.open(fileName, std::ifstream::in);

    std::vector<glm::vec3> wavefrontPositions;
    std::vector<glm::vec2> wavefrontTexCoords;

    std::string line;
    while (std::getline(ifs, line))
    {
        if (!line.empty())
        {
            const std::string command = getNextToken(line, fileName);
            if (command == "#" || command == "o" || command == "s")
            {
                continue;
            }
            else if (command == "v")
            {
                const std::string x = getNextToken(line, fileName);
                const std::string y = getNextToken(line, fileName);
                const std::string z = getNextToken(line, fileName);
                wavefrontPositions.emplace_back(::atof(x.c_str()), ::atof(y.c_str()), ::atof(z.c_str()));
            }
            else if (command == "vt")
            {
                const std::string x = getNextToken(line, fileName);
                const std::string y = getNextToken(line, fileName);
                wavefrontTexCoords.emplace_back(::atof(x.c_str()), ::atof(y.c_str()));
            }
            else if (command == "f")
            {
                std::vector<int> faceIndices;
                while (!line.empty())
                {
                    const std::string vertexToken = getNextToken(line, fileName);
                    const std::size_t separatorIndex = vertexToken.find('/');
                    const std::size_t afterSeparator = vertexToken.size() - separatorIndex - 1;
                    const int positionIndex = ::atoi(vertexToken.substr(0, separatorIndex).c_str());
                    const int texCoordIndex = ::atoi(vertexToken.substr(separatorIndex + 1, afterSeparator).c_str());
                    // - 1 because for some reason, in wavefront, indices start at 1 instead of 0...
                    const glm::vec3 position = wavefrontPositions[positionIndex - 1];
                    std::optional<glm::vec2> texCoord;
                    if (!wavefrontTexCoords.empty())
                    {
                        texCoord = wavefrontTexCoords[texCoordIndex - 1];
                        texCoord->y = 1 - texCoord->y; // Because wavefront. I guess.
                    }
                    const int vertexIndex = getOrCreateVertex(meshData.vertices, position, texCoord);
                    faceIndices.push_back(vertexIndex);
                }
                switch (faceIndices.size())
                {
                case 3:
                    if (primitive == Primitive::Lines)
                    {
                        meshData.indices.push_back(faceIndices[0]);
                        meshData.indices.push_back(faceIndices[1]);
                        meshData.indices.push_back(faceIndices[1]);
                        meshData.indices.push_back(faceIndices[2]);
                        meshData.indices.push_back(faceIndices[2]);
                        meshData.indices.push_back(faceIndices[0]);
                    }
                    else
                    {
                        meshData.indices.push_back(faceIndices[0]);
                        meshData.indices.push_back(faceIndices[1]);
                        meshData.indices.push_back(faceIndices[2]);
                    }
                    break;
                case 4:
                    if (primitive == Primitive::Lines)
                    {
                        meshData.indices.push_back(faceIndices[0]);
                        meshData.indices.push_back(faceIndices[1]);
                        meshData.indices.push_back(faceIndices[1]);
                        meshData.indices.push_back(faceIndices[2]);
                        meshData.indices.push_back(faceIndices[2]);
                        meshData.indices.push_back(faceIndices[3]);
                        meshData.indices.push_back(faceIndices[3]);
                        meshData.indices.push_back(faceIndices[0]);
                    }
                    else
                    {
                        // TODO check if the face can be cutted on those indices ?
                        meshData.indices.push_back(faceIndices[0]);
                        meshData.indices.push_back(faceIndices[1]);
                        meshData.indices.push_back(faceIndices[2]);
                        meshData.indices.push_back(faceIndices[0]);
                        meshData.indices.push_back(faceIndices[2]);
                        meshData.indices.push_back(faceIndices[3]);
                    }
                    break;
                default:
                {
                    std::ostringstream oss;
                    oss << "Invalid vertex count for face: " << faceIndices.size();
                    throw std::runtime_error(oss.str());
                }
                }
            }
        }
    }

    return meshData;
}

} // namespace

MeshLoader::MeshLoader(const FileSystem* fileSystem)
    : m_fileSystem(fileSystem)
{
}

unique_ptr<Mesh> MeshLoader::LoadWavefront(const Material* material, const std::string& fileName,
                                           Primitive::Type primitive) const
{
    const std::string pathToFile = m_fileSystem->PathToFileName(fileName);
    MeshData meshData = readFile(pathToFile, primitive);
    std::vector<float> positionsData;
    std::vector<float> textureCoordsData;

    positionsData.reserve(meshData.vertices.size() * 3);
    textureCoordsData.reserve(meshData.vertices.size() * 2);
    for (const MeshData::Vertex& vertex : meshData.vertices)
    {
        positionsData.push_back(vertex.position.x);
        positionsData.push_back(vertex.position.y);
        positionsData.push_back(vertex.position.z);
        if (primitive != Primitive::Lines && vertex.textureCoord)
        {
            textureCoordsData.push_back(vertex.textureCoord->x);
            textureCoordsData.push_back(vertex.textureCoord->y);
        }
    }

    HATCHER_ASSERT(textureCoordsData.empty() || textureCoordsData.size() == positionsData.size() / 3 * 2);
    Mesh* mesh = new Mesh(material, primitive);
    mesh->Set3DPositions(positionsData.data(), positionsData.size());
    if (!textureCoordsData.empty())
        mesh->SetTextureCoords(textureCoordsData.data(), textureCoordsData.size());
    mesh->SetIndices(meshData.indices.data(), meshData.indices.size());
    return unique_ptr<Mesh>(mesh);
}

} // namespace hatcher
