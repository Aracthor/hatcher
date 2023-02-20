#include "HexagonalGrid.hpp"

#include "hatcher/ISaveLoader.hpp"
#include "hatcher/assert.hpp"

namespace
{
float DegreeToRadian(float angle)
{
    return angle / 180.f * M_PI;
}

HexagonalGrid::TileCoord HexaCoordRound(float q, float r)
{
    const float s = -q - r;
    float qRound = ::round(q);
    float rRound = ::round(r);
    const float sRound = ::round(s);
    const float qDiff = std::abs(q - qRound);
    const float rDiff = std::abs(r - rRound);
    const float sDiff = std::abs(s - sRound);

    if ((qDiff > rDiff) && (qDiff > sDiff))
    {
        qRound = -rRound - sRound;
    }
    else if (rDiff > sDiff)
    {
        rRound = -qRound - sRound;
    }

    return {int(qRound), int(rRound)};
}

std::size_t hashCombine(std::size_t hashA, std::size_t hashB)
{
    return hashB ^ (hashA + 0x9e3779b9 + (hashB << 6) + (hashB >> 2));
}

} // namespace

static_assert(sizeof(HexagonalGrid::TileData) == 1);
HexagonalGrid::TileData HexagonalGrid::defaultTile = {
    .walkable = false,
};

std::size_t HexagonalGrid::TileCoord::Hash::operator()(TileCoord coord) const
{
    return hashCombine(std::hash<int>()(coord.q), std::hash<int>()(coord.r));
}

bool HexagonalGrid::TileCoord::operator==(TileCoord other) const
{
    return q == other.q && r == other.r;
}

HexagonalGrid::TileCoord::TileCoord(int q, int r)
    : q(q)
    , r(r)
{
}

std::array<HexagonalGrid::TileCoord, 6> HexagonalGrid::TileCoord::Neighbours() const
{
    return {
        TileCoord(q + 1, r + 0), TileCoord(q + 1, r - 1), TileCoord(q + 0, r + 1),
        TileCoord(q + 0, r - 1), TileCoord(q - 1, r + 0), TileCoord(q - 1, r + 1),
    };
}

HexagonalGrid::HexagonalGrid()
{
    const glm::vec2 qVector = glm::vec2(sqrtf(3.f), 0.f);
    const glm::vec2 rVector = glm::vec2(qVector.x / 2.f, 3.f / 2.f);
    m_hexToPosMatrix[0][0] = qVector.x;
    m_hexToPosMatrix[0][1] = rVector.x;
    m_hexToPosMatrix[1][0] = qVector.y;
    m_hexToPosMatrix[1][1] = rVector.y;
    m_hexToPosMatrix *= m_hexaSize;
    m_posToHexMatrix = glm::inverse(m_hexToPosMatrix);
}

HexagonalGrid::~HexagonalGrid() = default;

HexagonalGrid::TileCoord HexagonalGrid::PositionToTileCoords(glm::vec2 position) const
{
    const glm::vec2 convertedCoords = position * m_posToHexMatrix;
    return HexaCoordRound(convertedCoords.x, convertedCoords.y);
}

glm::vec2 HexagonalGrid::TileCoordToPosition(TileCoord coord) const
{
    return glm::vec2(coord.q, coord.r) * m_hexToPosMatrix;
}

bool HexagonalGrid::HasTileData(TileCoord coords) const
{
    return m_tilesData.find(coords) != m_tilesData.end();
}

const HexagonalGrid::TileData& HexagonalGrid::GetTileData(TileCoord coords) const
{
    if (!HasTileData(coords))
        return defaultTile;
    return m_tilesData.at(coords);
}

const HexagonalGrid::TileData& HexagonalGrid::GetTileData(glm::vec2 position) const
{
    return GetTileData(PositionToTileCoords(position));
}

glm::vec2 HexagonalGrid::GetTileCenter(glm::vec2 position) const
{
    return TileCoordToPosition(PositionToTileCoords(position));
}

glm::vec2 HexagonalGrid::GetHexaAngle(TileCoord coord, int angleIndex) const
{
    const float angle = DegreeToRadian(30.f + float(angleIndex) * 60.f);
    const glm::vec2 hexagonCenter = TileCoordToPosition(coord);
    return hexagonCenter + glm::vec2(cosf(angle), sinf(angle)) * m_hexaSize;
}

std::vector<glm::vec2> HexagonalGrid::GetPathIfPossible(TileCoord start, TileCoord end) const
{
    const glm::vec2 startPos = TileCoordToPosition(start);
    const glm::vec2 endPos = TileCoordToPosition(end);
    return m_pathfinding.GetPath(startPos, endPos);
}

void HexagonalGrid::SetTileWalkable(TileCoord coord, bool walkable)
{
    GetOrCreateData(coord).walkable = walkable;
    const glm::vec2 tilePosition = TileCoordToPosition(coord);
    if (walkable)
    {
        m_pathfinding.CreateNode(tilePosition);
        for (TileCoord neighbour : coord.Neighbours())
        {
            if (GetTileData(neighbour).walkable)
            {
                const glm::vec2 neighbourPosition = TileCoordToPosition(neighbour);
                m_pathfinding.LinkNodes(tilePosition, neighbourPosition);
                m_pathfinding.LinkNodes(neighbourPosition, tilePosition);
            }
        }
    }
    else
    {
        m_pathfinding.DeleteNode(tilePosition);
    }
}

void HexagonalGrid::SaveLoad(ISaveLoader& saveLoader)
{
    saveLoader << m_hexaSize;
    saveLoader << m_tilesData;
}

void HexagonalGrid::PostLoad()
{
    UpdatePathfind();
}

HexagonalGrid::TileData& HexagonalGrid::GetOrCreateData(TileCoord coord)
{
    if (!HasTileData(coord))
        m_tilesData[coord] = defaultTile;
    return m_tilesData[coord];
}

void HexagonalGrid::UpdatePathfind()
{
    m_pathfinding = {};

    for (const auto& tileData : m_tilesData)
    {
        if (tileData.second.walkable)
        {
            const glm::vec2 tilePosition = TileCoordToPosition(tileData.first);
            m_pathfinding.CreateNode(tilePosition);
        }
    }

    for (const auto& tileData : m_tilesData)
    {
        if (tileData.second.walkable)
        {
            const glm::vec2 tilePosition = TileCoordToPosition(tileData.first);
            m_pathfinding.CreateNode(tilePosition);
            for (TileCoord neighbour : tileData.first.Neighbours())
            {
                const glm::vec2 neighbourPosition = TileCoordToPosition(neighbour);
                m_pathfinding.LinkNodes(tilePosition, neighbourPosition);
            }
        }
    }
}

void operator<<(ISaveLoader& saveLoader, HexagonalGrid::TileCoord& coord)
{
    saveLoader << coord.q;
    saveLoader << coord.r;
}

void operator<<(ISaveLoader& saveLoader, HexagonalGrid::TileData& data)
{
    static_assert(sizeof(HexagonalGrid::TileData) == sizeof(ubyte));
    ubyte& byte = reinterpret_cast<ubyte&>(data);
    saveLoader << byte;
}
