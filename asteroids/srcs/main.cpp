#include "hatcher/EntityDescriptor.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/GameApplication.hpp"
#include "hatcher/World.hpp"

#include "CollidableComponent.hpp"
#include "PlayerComponent.hpp"
#include "PositionComponent.hpp"

using namespace hatcher;

unique_ptr<IEntityDescriptor> PlayerEntityDescriptor()
{
    EntityDescriptorBuilder builder;

    PositionComponent position;
    position.position = {400.f, 300.f};
    position.angle = 0.f;
    position.speed = {0.f, 0.f};
    builder.AddComponent<>(position);

    PlayerComponent player;
    player.turningLeft = false;
    player.turningRight = false;
    player.accelerating = false;
    builder.AddComponent<>(player);

    CollidableComponent collidable;
    collidable.size = 24.f;
    builder.AddComponent<>(collidable);

    return builder.CreateDescriptor();
}

int main(int argc, char** argv)
{
    GameApplication demo(argv[0]);
    demo.StartRendering("hatcher - asteroids", 800, 600);
    demo.GetWorld()->GetEntityManager()->CreateNewEntity(PlayerEntityDescriptor().get());
    return demo.Run();
}
