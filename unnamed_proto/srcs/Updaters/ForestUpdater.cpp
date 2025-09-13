#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/EntityDescriptorID.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/Maths/RandomGenerator.hpp"
#include "hatcher/Updater.hpp"

#include <vector>

#include "Components/GrowableComponent.hpp"
#include "Components/Position2DComponent.hpp"
#include "WorldComponents/SquareGrid.hpp"

using namespace hatcher;

namespace
{
constexpr float density = 0.05f;

class ForestUpdater final : public Updater
{
    void CreateWorld(int64_t seed, IEntityManager* entityManager, ComponentAccessor* componentAccessor) const override
    {
        RandomGenerator random(seed);
        std::vector<glm::ivec2> positions;

        const SquareGrid* grid = componentAccessor->ReadWorldComponent<SquareGrid>();
        int treesToCreate = grid->TileCount() * density;
        while (treesToCreate-- > 0)
        {
            EntityEgg tree = entityManager->CreateNewEntity(EntityDescriptorID::Create("Tree"));
            const glm::ivec2 position = [&]()
            {
                glm::ivec2 result;
                do
                {
                    const int x = random.RandomInt(grid->GetTileCoordMin().x, grid->GetTileCoordMax().x - 1);
                    const int y = random.RandomInt(grid->GetTileCoordMin().y, grid->GetTileCoordMax().y - 1);
                    result = {x, y};
                } while (std::find(positions.begin(), positions.end(), result) != positions.end());
                return result;
            }();
            positions.push_back(position);
            tree.GetComponent<Position2DComponent>()->position = grid->GetTileCenter(position);
            tree.GetComponent<GrowableComponent>()->maturity = 1.f;
        }
    }

    void Update(IEntityManager* entityManager, ComponentAccessor* componentAccessor) override {}
};

UpdaterRegisterer<ForestUpdater> registerer;

} // namespace
