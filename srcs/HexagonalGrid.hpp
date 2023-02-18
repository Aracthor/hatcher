#pragma once

#include "hatcher/IWorldComponent.hpp"
#include "hatcher/glm_pure.hpp"

using namespace hatcher;

class HexagonalGrid : public IWorldComponent
{
public:
    struct TileCoord
    {
        TileCoord() = default;
        TileCoord(int q, int r);

        int q;
        int r;
    };

    HexagonalGrid();
    ~HexagonalGrid();

    TileCoord PositionToTileCoords(glm::vec2 position) const;
    glm::vec2 TileCoordToPosition(TileCoord coord) const;

    glm::vec2 GetTileCenter(glm::vec2 position) const;
    glm::vec2 GetHexaAngle(TileCoord tile, int angleIndex) const;

private:
    float m_hexaSize = 1.f;

    glm::mat2 m_hexToPosMatrix;
    glm::mat2 m_posToHexMatrix;
};
