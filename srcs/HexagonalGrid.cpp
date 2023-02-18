#include "HexagonalGrid.hpp"

#include "hatcher/assert.hpp"

namespace
{
float DegreeToRadian(float angle)
{
    return angle / 180.f * M_PI;
}
} // namespace

HexagonalGrid::TileCoord::TileCoord(int q, int r)
    : q(q)
    , r(r)
{
}

glm::vec2 HexagonalGrid::TileCoordToPosition(TileCoord coord) const
{
    const glm::vec2 qVector = glm::vec2(sqrtf(3.f), 0.f) * m_hexaSize;
    const glm::vec2 rVector = glm::vec2(sqrtf(3.f) / 2.f, 3.f / 2.f) * m_hexaSize;
    return float(coord.q) * qVector + float(coord.r) * rVector;
}

glm::vec2 HexagonalGrid::GetHexaAngle(TileCoord coord, int angleIndex) const
{
    const float angle = DegreeToRadian(30.f + float(angleIndex) * 60.f);
    const glm::vec2 hexagonCenter = TileCoordToPosition(coord);
    return hexagonCenter + glm::vec2(cosf(angle), sinf(angle)) * m_hexaSize;
}
