#include <memory>

#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"
#include "Selectable2DComponent.hpp"
#include "SelectionRectangle.hpp"
#include "TransformationHelper.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/FrameRenderer.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/IEventUpdater.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshBuilder.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"

using namespace hatcher;

namespace
{

class SelectionRectangleEventListener final : public IEventListener
{
public:
    void GetEvent(const SDL_Event& event, IEntityManager* entityManager,
                  ComponentManager* componentManager, ComponentManager* renderComponentManager,
                  const IFrameRenderer& frameRenderer) override
    {
        SelectionRectangle* selectionRectangle =
            renderComponentManager->WriteWorldComponent<SelectionRectangle>();
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            const glm::vec2 winCoords = {event.motion.x,
                                         frameRenderer.Resolution().y - event.motion.y};
            selectionRectangle->StartSelection(winCoords);
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            if (selectionRectangle->IsSelecting())
            {
                const glm::vec2 winCoords = {event.motion.x,
                                             frameRenderer.Resolution().y - event.motion.y};
                selectionRectangle->MoveSelection(winCoords);
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
        {
            ComponentWriter<Selectable2DComponent> selectableComponents =
                renderComponentManager->WriteComponents<Selectable2DComponent>();
            ComponentReader<Position2DComponent> positionComponents =
                componentManager->ReadComponents<Position2DComponent>();
            ComponentReader<Movement2DComponent> movementComponents =
                componentManager->ReadComponents<Movement2DComponent>();
            const Box2f selectionBox = selectionRectangle->GetCurrentSelection();

            HATCHER_ASSERT(componentManager->Count() == renderComponentManager->Count());
            for (int i = 0; i < componentManager->Count(); i++)
            {
                std::optional<Selectable2DComponent>& selectableComponent = selectableComponents[i];
                if (selectableComponent)
                {
                    HATCHER_ASSERT(positionComponents[i]);
                    const glm::mat4 modelMatrix = TransformationHelper::ModelFromComponents(
                        positionComponents[i], movementComponents[i]);

                    const Box2f entitySelectionBox = frameRenderer.ProjectBox3DToWindowCoords(
                        selectableComponent->box, modelMatrix);
                    selectableComponent->selected = selectionBox.Touches(entitySelectionBox);
                }
            }

            selectionRectangle->EndSelection();
        }
    }

    span<const SDL_EventType> EventTypesToListen() const override
    {
        static const SDL_EventType events[] = {
            SDL_MOUSEBUTTONDOWN,
            SDL_MOUSEBUTTONUP,
            SDL_MOUSEMOTION,
        };
        return span<const SDL_EventType>(events, std::size(events));
    }
};

class SelectionRectangleRenderUpdater final : public RenderUpdater
{
public:
    SelectionRectangleRenderUpdater(const IRendering* rendering, IEventUpdater* eventUpdater)
    {
        eventUpdater->RegisterListener(std::make_shared<SelectionRectangleEventListener>());

        MeshBuilder* meshBuilder = rendering->GetMeshBuilder().get();
        meshBuilder->SetPrimitive(Primitive::Lines);

        meshBuilder->SetMaterial(rendering->GetMaterialFactory()->CreateMaterial(
            "shaders/hello_world_2D.vert", "shaders/hello_world.frag"));
        m_selectionRectangleMesh.reset(meshBuilder->Create());

        float positions[] = {
            -1.f, -1.f,

            1.f,  -1.f,

            1.f,  1.f,

            -1.f, 1.f,
        };
        ushort indices[] = {0, 1, 1, 2, 2, 3, 3, 0};

        m_selectionRectangleMesh->Set2DPositions(positions, std::size(positions));
        m_selectionRectangleMesh->SetIndices(indices, std::size(indices));
    }

    ~SelectionRectangleRenderUpdater() = default;

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        const SelectionRectangle* selectionRectangle =
            renderComponentManager->ReadWorldComponent<SelectionRectangle>();
        if (selectionRectangle->IsSelecting())
        {
            const Box2f selectionBox = selectionRectangle->GetCurrentSelection();
            const glm::vec2 rectangleCenter = selectionBox.Center();
            const glm::vec2 rectangleSize = selectionBox.Extents();

            const glm::vec3 position = {rectangleCenter.x, rectangleCenter.y, 0.f};
            const glm::vec3 scale = {rectangleSize.x / 2.f, rectangleSize.y / 2.f, 0.f};

            glm::mat4 modelMatrix = glm::mat4(1.f);
            modelMatrix = glm::translate(modelMatrix, position);
            modelMatrix = glm::scale(modelMatrix, scale);

            frameRenderer.AddUIMeshToRender(m_selectionRectangleMesh.get(), modelMatrix);
        }
    }

private:
    std::unique_ptr<Mesh> m_selectionRectangleMesh;
};

const int dummy = RegisterRenderUpdater<SelectionRectangleRenderUpdater>("SelectionRectangle");

} // namespace
