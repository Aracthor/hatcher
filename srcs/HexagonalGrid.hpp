#pragma once

#include "hatcher/IWorldComponent.hpp"
#include "hatcher/glm_pure.hpp"

#include <unordered_map>

using namespace hatcher;

class HexagonalGrid : public IWorldComponent
{
public:
    struct TileCoord
    {
        struct Hash
        {
            std::size_t operator()(TileCoord coord) const;
        };

        bool operator==(TileCoord other) const;

        TileCoord() = default;
        TileCoord(int q, int r);

        int q;
        int r;
    };

    struct TileData
    {
        bool walkable : 1;
    };

    static TileData defaultTile;

    HexagonalGrid();
    ~HexagonalGrid();

    TileCoord PositionToTileCoords(glm::vec2 position) const;
    glm::vec2 TileCoordToPosition(TileCoord coord) const;

    bool HasTileData(TileCoord coord) const;
    const TileData& GetTileData(TileCoord coord) const;
    const TileData& GetTileData(glm::vec2 position) const;
    TileData& GetTileData(TileCoord coord);
    TileData& GetTileData(glm::vec2 position);

    glm::vec2 GetTileCenter(glm::vec2 position) const;
    glm::vec2 GetHexaAngle(TileCoord tile, int angleIndex) const;

private:
    float m_hexaSize = 1.f;
    std::unordered_map<TileCoord, TileData, TileCoord::Hash> m_tilesData;

    glm::mat2 m_hexToPosMatrix;
    glm::mat2 m_posToHexMatrix;
};
