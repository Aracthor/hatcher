#include <SDL2/SDL_events.h>

#include "hatcher/CommandRegisterer.hpp"
#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/EntityDescriptorID.hpp"
#include "hatcher/EntityEgg.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshLoader.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/ICommand.hpp"
#include "hatcher/ICommandManager.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/assert.hpp"

#include "RenderUpdaterOrder.hpp"

#include "Components/Position2DComponent.hpp"

#include "WorldComponents/Blueprint.hpp"
#include "WorldComponents/Camera.hpp"
#include "WorldComponents/SquareGrid.hpp"

using namespace hatcher;

namespace
{
class CreateBuildingFromBlueprint final : public ICommand
{
public:
    CreateBuildingFromBlueprint(const EntityDescriptorID& entityDescriptor, glm::vec2 spawnPosition)
        : m_entityDescriptor(entityDescriptor)
        , m_spawnPosition(spawnPosition)
    {
    }

    void Save(DataSaver& saver) const override
    {
        saver << m_entityDescriptor;
        saver << m_spawnPosition;
    }

    void Load(DataLoader& loader) override
    {
        loader >> m_entityDescriptor;
        loader >> m_spawnPosition;
    }

    void Execute(IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
        EntityEgg entityEgg = entityManager->CreateNewEntity(m_entityDescriptor);
        entityEgg.GetComponent<Position2DComponent>()->position = m_spawnPosition;
    }

private:
    EntityDescriptorID m_entityDescriptor;
    glm::vec2 m_spawnPosition;

    COMMAND_HEADER(CreateBuildingFromBlueprint)
};
REGISTER_COMMAND(CreateBuildingFromBlueprint);

class BlueprintEventListener : public IEventListener
{
    void GetEvent(const SDL_Event& event, IApplication* application, ICommandManager* commandManager,
                  const ComponentAccessor* componentAccessor, ComponentAccessor* renderComponentAccessor,
                  const IFrameRenderer& frameRenderer) override
    {
        Blueprint* blueprint = renderComponentAccessor->WriteWorldComponent<Blueprint>();
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_B)
        {
            blueprint->active = !blueprint->active;
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            const Camera* camera = renderComponentAccessor->ReadWorldComponent<Camera>();
            const SquareGrid* grid = componentAccessor->ReadWorldComponent<SquareGrid>();
            const glm::vec2 worldCoords2D =
                camera->MouseCoordsToWorldCoords(event.motion.x, event.motion.y, frameRenderer);
            blueprint->position = grid->GetTileCenter(worldCoords2D);
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && blueprint->active)
        {
            blueprint->active = false;
            commandManager->AddCommand(
                new CreateBuildingFromBlueprint(EntityDescriptorID::Create("LoggingHut"), blueprint->position));
        }
    }
};

class BlueprintRenderUpdater final : public RenderUpdater
{
public:
    BlueprintRenderUpdater(const IRendering* rendering)
    {
        m_material = rendering->GetMaterialFactory()->CreateMaterial("shaders/colored.vert", "shaders/colored.frag");
        m_mesh = rendering->GetMeshLoader()->LoadWavefront(m_material.get(), "assets/meshes/hut.obj");
    }

    void Update(IApplication* application, const ComponentAccessor* componentAccessor,
                ComponentAccessor* renderComponentAccessor, IFrameRenderer& frameRenderer) override
    {
        const Blueprint* blueprint = renderComponentAccessor->WriteWorldComponent<Blueprint>();
        if (blueprint->active)
        {
            frameRenderer.PrepareSceneDraw(m_material.get());
            const glm::mat4 modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(blueprint->position, 0.f));
            m_mesh->Draw(modelMatrix);
        }
    }

private:
    unique_ptr<Material> m_material;
    unique_ptr<Mesh> m_mesh;
};

EventListenerRegisterer<BlueprintEventListener> eventRegisterer;
RenderUpdaterRegisterer<BlueprintRenderUpdater> updateRegisterer((int)ERenderUpdaterOrder::Camera);

} // namespace