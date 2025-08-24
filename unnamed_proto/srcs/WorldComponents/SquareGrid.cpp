#include "SquareGrid.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/DataLoader.hpp"
#include "hatcher/DataSaver.hpp"
#include "hatcher/assert.hpp"

namespace
{
const glm::vec2 tileNeighbours[] = {
    {-1.f, 0.f},
    {1.f, 0.f},
    {0.f, -1.f},
    {0.f, 1.f},
};
} // namespace

static_assert(sizeof(SquareGrid::TileData) == 1);
SquareGrid::TileData SquareGrid::defaultTile = {
    .walkable = false,
};

SquareGrid::SquareGrid(int64_t seed)
{
    m_tilesData.fill(defaultTile);
    for (int y = MIN_HEIGHT; y < MAX_HEIGHT; y++)
    {
        for (int x = MIN_WIDTH; x < MAX_WIDTH; x++)
        {
            SetTileWalkable({x, y}, true);
        }
    }
}

SquareGrid::~SquareGrid() = default;

bool SquareGrid::HasTileData(glm::vec2 position) const
{
    return MIN_MAX.Contains(position);
}

const SquareGrid::TileData& SquareGrid::GetTileData(glm::vec2 position) const
{
    if (!HasTileData(position))
        return defaultTile;
    const int index = CoordToTileIndex(position);
    return m_tilesData[index];
}

glm::vec2 SquareGrid::GetTileCenter(glm::vec2 position) const
{
    const float x = std::floor(position.x) + 0.5f;
    const float y = std::floor(position.y) + 0.5f;
    return {x, y};
}

std::vector<glm::vec2> SquareGrid::GetPathIfPossible(glm::vec2 start, glm::vec2 end, float distance /*= 0.f*/) const
{
    const glm::vec2 startPos = GetTileCenter(start);
    const glm::vec2 endPos = GetTileCenter(end);
    return m_pathfinding.GetPath(startPos, endPos, distance);
}

void SquareGrid::SetTileWalkable(glm::vec2 position, bool walkable)
{
    HATCHER_ASSERT(HasTileData(position));
    const glm::vec2 tilePosition = GetTileCenter(position);
    TileData& data = GetData(tilePosition);
    if (data.walkable == walkable)
        return;

    data.walkable = walkable;
    if (walkable)
    {
        m_pathfinding.CreateNode(tilePosition);
        for (glm::vec2 neighbour : tileNeighbours)
        {
            const glm::vec2 neighbourPosition = tilePosition + neighbour;
            if (HasTileData(neighbourPosition) && GetTileData(neighbourPosition).walkable)
            {
                m_pathfinding.LinkNodes(tilePosition, neighbourPosition);
                m_pathfinding.LinkNodes(neighbourPosition, tilePosition);
            }
        }
    }
    else
    {
        if (m_pathfinding.ContainsNode(tilePosition))
            m_pathfinding.DeleteNode(tilePosition);
    }
}

void SquareGrid::Save(DataSaver& saver) const
{
    saver << m_tilesData;
}

void SquareGrid::Load(DataLoader& loader)
{
    loader >> m_tilesData;
    UpdatePathfind();
}

SquareGrid::TileData& SquareGrid::GetData(glm::vec2 position)
{
    HATCHER_ASSERT(HasTileData(position));
    const int tileIndex = CoordToTileIndex(position);
    return m_tilesData[tileIndex];
}

int SquareGrid::CoordToTileIndex(glm::vec2 position) const
{
    const int x = static_cast<int>(std::floor(position.x) - MIN_WIDTH);
    const int y = static_cast<int>(std::floor(position.y) - MIN_HEIGHT);
    const int rowLength = static_cast<int>(MAX_WIDTH - MIN_WIDTH);
    return y * rowLength + x;
}

void SquareGrid::UpdatePathfind()
{
    m_pathfinding = {};

    for (int y = MIN_HEIGHT; y < MAX_HEIGHT; y++)
    {
        for (int x = MIN_WIDTH; x < MAX_WIDTH; x++)
        {
            const glm::vec2 tilePosition = GetTileCenter({x, y});
            if (GetData(tilePosition).walkable)
                m_pathfinding.CreateNode(tilePosition);
        }
    }

    for (int y = MIN_HEIGHT; y < MAX_HEIGHT; y++)
    {
        for (int x = MIN_WIDTH; x < MAX_WIDTH; x++)
        {
            const glm::vec2 tilePosition = GetTileCenter({x, y});
            if (GetData(tilePosition).walkable)
            {
                for (glm::vec2 neighbour : tileNeighbours)
                {
                    const glm::vec2 neighbourPosition = tilePosition + neighbour;
                    if (m_pathfinding.ContainsNode(neighbourPosition))
                        m_pathfinding.LinkNodes(tilePosition, neighbourPosition);
                }
            }
        }
    }
}

namespace
{
WorldComponentTypeRegisterer<SquareGrid, EComponentList::Gameplay> registerer;
} // namespace
