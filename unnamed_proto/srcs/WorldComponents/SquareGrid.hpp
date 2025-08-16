#pragma once

#include <array>

#include "hatcher/IWorldComponent.hpp"
#include "hatcher/Maths/Box.hpp"
#include "hatcher/Maths/glm_pure.hpp"

#include "Pathfinding.hpp"

using namespace hatcher;

class SquareGrid final : public IWorldComponent
{
public:
    struct TileData
    {
        bool walkable : 1;
    };

    static TileData defaultTile;

    SquareGrid();
    ~SquareGrid();

    bool HasTileData(glm::vec2 position) const;
    const TileData& GetTileData(glm::vec2 position) const;

    glm::vec2 GetTileCenter(glm::vec2 position) const;
    glm::vec2 GetTileCoordMin() const { return {MIN_WIDTH, MIN_HEIGHT}; }
    glm::vec2 GetTileCoordMax() const { return {MAX_WIDTH, MAX_HEIGHT}; }

    std::vector<glm::vec2> GetPathIfPossible(glm::vec2 start, glm::vec2 end, float distance = 0.f) const;

    void SetTileWalkable(glm::vec2 position, bool walkable);

    void Save(DataSaver& saver) const override;
    void Load(DataLoader& loader) override;

private:
    static constexpr float MIN_WIDTH = -5.f;
    static constexpr float MAX_WIDTH = 5.f;
    static constexpr float MIN_HEIGHT = -5.f;
    static constexpr float MAX_HEIGHT = 5.f;
    static constexpr int TILE_COUNT = static_cast<int>((MAX_WIDTH - MIN_WIDTH) * (MAX_HEIGHT - MIN_HEIGHT));
    static constexpr Box2f MIN_MAX = Box2f({MIN_WIDTH, MIN_HEIGHT}, {MAX_WIDTH, MAX_HEIGHT});

    TileData& GetData(glm::vec2 position);
    int CoordToTileIndex(glm::vec2 position) const;

    void UpdatePathfind();

    std::array<TileData, TILE_COUNT> m_tilesData;

    Pathfinding m_pathfinding;
};
