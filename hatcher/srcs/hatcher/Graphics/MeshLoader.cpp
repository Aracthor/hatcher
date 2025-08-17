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
        std::optional<glm::vec3> color;
        std::optional<glm::vec2> textureCoord;
        std::optional<glm::vec3> normal;

        bool operator==(const Vertex& other) const
        {
            return position == other.position && color == other.color && textureCoord == other.textureCoord &&
                   normal == other.normal;
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

int getOrCreateVertex(std::vector<MeshData::Vertex>& vertices, glm::vec3 position, std::optional<glm::vec3> color,
                      std::optional<glm::vec2> texCoord, std::optional<glm::vec3> normal)
{
    MeshData::Vertex vertex({position, color, texCoord, normal});
    auto it = std::find(vertices.begin(), vertices.end(), vertex);
    if (it != vertices.end())
    {
        return it - vertices.begin();
    }
    vertices.push_back(vertex);
    return vertices.size() - 1;
}

MeshData readFile(const std::string& fileName)
{
    MeshData meshData;
    std::ifstream ifs;
    ifs.open(fileName, std::ifstream::in);
    if (ifs.rdstate() & std::ios::failbit)
        throw std::runtime_error(std::string("Couldn't open mesh file '") + fileName + "'.");

    std::vector<glm::vec3> wavefrontPositions;
    std::vector<glm::vec3> wavefrontColors;
    std::vector<glm::vec2> wavefrontTexCoords;
    std::vector<glm::vec3> wavefrontNormals;

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
                if (!line.empty())
                {
                    const std::string r = getNextToken(line, fileName);
                    const std::string g = getNextToken(line, fileName);
                    const std::string b = getNextToken(line, fileName);
                    wavefrontColors.emplace_back(::atof(r.c_str()), ::atof(g.c_str()), ::atof(b.c_str()));
                }
            }
            else if (command == "vt")
            {
                const std::string x = getNextToken(line, fileName);
                const std::string y = getNextToken(line, fileName);
                wavefrontTexCoords.emplace_back(::atof(x.c_str()), ::atof(y.c_str()));
            }
            else if (command == "vn")
            {
                const std::string x = getNextToken(line, fileName);
                const std::string y = getNextToken(line, fileName);
                const std::string z = getNextToken(line, fileName);
                wavefrontNormals.emplace_back(::atof(x.c_str()), ::atof(y.c_str()), ::atof(z.c_str()));
            }
            else if (command == "f")
            {
                std::vector<int> faceIndices;
                while (!line.empty())
                {
                    const std::string vertexToken = getNextToken(line, fileName);
                    std::istringstream iss(vertexToken);
                    std::string index;
                    std::getline(iss, index, '/');
                    const int positionIndex = ::atoi(index.c_str());
                    // - 1 because for some reason, in wavefront, indices start at 1 instead of 0...
                    const glm::vec3 position = wavefrontPositions[positionIndex - 1];
                    const std::optional<glm::vec3> color =
                        wavefrontColors.empty() ? std::optional<glm::vec3>() : wavefrontColors[positionIndex - 1];
                    std::optional<glm::vec2> texCoord;
                    std::optional<glm::vec3> normal;
                    if (std::getline(iss, index, '/') && !index.empty())
                    {
                        HATCHER_ASSERT(!wavefrontTexCoords.empty());
                        const int texCoordIndex = ::atoi(index.c_str());
                        texCoord = wavefrontTexCoords[texCoordIndex - 1];
                        texCoord->y = 1 - texCoord->y; // Because wavefront. I guess.
                    }
                    if (std::getline(iss, index, '/') && !index.empty())
                    {
                        HATCHER_ASSERT(!wavefrontNormals.empty());
                        const int normalIndex = ::atoi(index.c_str());
                        normal = wavefrontNormals[normalIndex - 1];
                    }
                    const int vertexIndex = getOrCreateVertex(meshData.vertices, position, color, texCoord, normal);
                    faceIndices.push_back(vertexIndex);
                }
                switch (faceIndices.size())
                {
                case 3:
                    meshData.indices.push_back(faceIndices[0]);
                    meshData.indices.push_back(faceIndices[1]);
                    meshData.indices.push_back(faceIndices[2]);
                    break;
                case 4:
                    // TODO check if the face can be cutted on those indices ?
                    meshData.indices.push_back(faceIndices[0]);
                    meshData.indices.push_back(faceIndices[1]);
                    meshData.indices.push_back(faceIndices[2]);
                    meshData.indices.push_back(faceIndices[0]);
                    meshData.indices.push_back(faceIndices[2]);
                    meshData.indices.push_back(faceIndices[3]);
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

unique_ptr<Mesh> MeshLoader::LoadWavefront(const Material* material, const std::string& fileName) const
{
    try
    {
        const std::string pathToFile = m_fileSystem->PathToFileName(fileName);
        MeshData meshData = readFile(pathToFile);
        std::vector<float> positionsData;
        std::vector<float> colorData;
        std::vector<float> textureCoordsData;
        std::vector<float> normalsData;

        positionsData.reserve(meshData.vertices.size() * 3);
        colorData.reserve(meshData.vertices.size() * 3);
        textureCoordsData.reserve(meshData.vertices.size() * 2);
        normalsData.reserve(meshData.vertices.size() * 3);
        for (const MeshData::Vertex& vertex : meshData.vertices)
        {
            positionsData.push_back(vertex.position.x);
            positionsData.push_back(vertex.position.y);
            positionsData.push_back(vertex.position.z);
            if (vertex.color)
            {
                colorData.push_back(vertex.color->x);
                colorData.push_back(vertex.color->y);
                colorData.push_back(vertex.color->z);
            }
            if (vertex.textureCoord)
            {
                textureCoordsData.push_back(vertex.textureCoord->x);
                textureCoordsData.push_back(vertex.textureCoord->y);
            }
            if (vertex.normal)
            {
                normalsData.push_back(vertex.normal->x);
                normalsData.push_back(vertex.normal->y);
                normalsData.push_back(vertex.normal->z);
            }
        }

        HATCHER_ASSERT(colorData.empty() || colorData.size() == positionsData.size());
        HATCHER_ASSERT(textureCoordsData.empty() || textureCoordsData.size() == positionsData.size() / 3 * 2);
        Mesh* mesh = new Mesh(material, Primitive::Triangles);
        mesh->Set3DPositions(positionsData.data(), positionsData.size());
        if (!colorData.empty())
            mesh->SetColors(colorData.data(), colorData.size());
        if (!textureCoordsData.empty())
            mesh->SetTextureCoords(textureCoordsData.data(), textureCoordsData.size());
        if (!normalsData.empty())
            mesh->SetNormals(normalsData.data(), normalsData.size());
        mesh->SetIndices(meshData.indices.data(), meshData.indices.size());
        return unique_ptr<Mesh>(mesh);
    }
    catch (const std::exception& exception)
    {
        std::ostringstream oss;
        oss << "Error reading file '" << fileName << "': " << exception.what();
        throw std::runtime_error(oss.str());
    }
}

} // namespace hatcher
