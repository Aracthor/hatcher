#include "HexagonalGrid.hpp"

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
} // namespace

HexagonalGrid::TileCoord::TileCoord(int q, int r)
    : q(q)
    , r(r)
{
}

HexagonalGrid::HexagonalGrid()
{
    const glm::vec2 qVector = glm::vec2(sqrtf(3.f), 0.f) * m_hexaSize;
    const glm::vec2 rVector = glm::vec2(qVector.x / 2.f, 3.f / 2.f) * m_hexaSize;
    m_hexToPosMatrix[0][0] = qVector.x;
    m_hexToPosMatrix[0][1] = rVector.x;
    m_hexToPosMatrix[1][0] = qVector.y;
    m_hexToPosMatrix[1][1] = rVector.y;
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
    // const glm::vec2 qVector = glm::vec2(sqrtf(3.f), 0.f) * m_hexaSize;
    // const glm::vec2 rVector = glm::vec2(sqrtf(3.f) / 2.f, 3.f / 2.f) * m_hexaSize;
    return glm::vec2(coord.q, coord.r) * m_hexToPosMatrix;
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
