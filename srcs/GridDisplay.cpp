#include "GridDisplay.hpp"

#include "hatcher/Graphics/FrameRenderer.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshBuilder.hpp"

#include <vector>

GridDisplay::GridDisplay(const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder)
{
    meshBuilder->SetPrimitive(hatcher::Primitive::Lines);

    meshBuilder->SetMaterial(
        meshBuilder->CreateMaterial("shaders/hello_world_2D.vert", "shaders/hello_world.frag"));
    m_gridMesh.reset(meshBuilder->Create());

    std::vector<float> positions;
    positions.reserve(m_gridSize * 8);
    for (int i = 0; i < m_gridSize; i++)
    {
        // x
        positions.push_back(i);
        positions.push_back(0);
        positions.push_back(i);
        positions.push_back(m_gridSize - 1);
        // y
        positions.push_back(0);
        positions.push_back(i);
        positions.push_back(m_gridSize - 1);
        positions.push_back(i);
    }

    m_gridMesh->Set2DPositions(positions.data(), std::size(positions));
}

GridDisplay::~GridDisplay() {}

void GridDisplay::DrawGrid(hatcher::IFrameRenderer& frameRenderer, float parPositionX,
                           float parPositionY)
{
    if (m_enabled)
    {
        int gridPositionX = std::floor(parPositionX) - m_gridSize / 2 + 1;
        int gridPositionY = std::floor(parPositionY) - m_gridSize / 2 + 1;
        glm::mat4 modelMatrix = glm::translate(glm::vec3(gridPositionX, gridPositionY, 0.f));

        frameRenderer.AddMeshToRender(m_gridMesh.get(), modelMatrix);
    }
}
