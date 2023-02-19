#pragma once

#include <array>
#include <unordered_map>

#include "hatcher/IWorldComponent.hpp"
#include "hatcher/glm_pure.hpp"

#include "Pathfinding.hpp"

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

        std::array<TileCoord, 6> Neighbours() const;

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

    glm::vec2 GetTileCenter(glm::vec2 position) const;
    glm::vec2 GetHexaAngle(TileCoord tile, int angleIndex) const;

    std::vector<glm::vec2> GetPathIfPossible(TileCoord start, TileCoord end) const;

    void SetTileWalkable(TileCoord coord, bool walkable);

private:
    TileData& GetOrCreateData(TileCoord coord);

    float m_hexaSize = 1.f;
    std::unordered_map<TileCoord, TileData, TileCoord::Hash> m_tilesData;

    Pathfinding m_pathfinding;

    glm::mat2 m_hexToPosMatrix;
    glm::mat2 m_posToHexMatrix;
};
