#include <memory>

#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"
#include "SelectableComponent.hpp"
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
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"

using namespace hatcher;

namespace
{

class SelectionRectangle
{
public:
    void StartSelection(const glm::vec2& position)
    {
        m_isSelecting = true;
        m_selectionStart = position;
        m_currentRectangle = Box2f(m_selectionStart);
    }

    void MoveSelection(const glm::vec2& position)
    {
        HATCHER_ASSERT(m_isSelecting);
        m_currentRectangle = Box2f(m_selectionStart);
        m_currentRectangle.AddPoint(position);
    }

    void EndSelection() { m_isSelecting = false; }

    bool IsSelecting() const { return m_isSelecting; }
    const Box2f GetCurrentSelection() const { return m_currentRectangle; }

private:
    bool m_isSelecting = false;
    glm::vec2 m_selectionStart;
    Box2f m_currentRectangle;
};

class SelectionRectangleEventListener final : public IEventListener
{
public:
    SelectionRectangleEventListener(SelectionRectangle& selectionRectangle)
        : m_selectionRectangle(selectionRectangle)
    {
    }

    void GetEvent(const SDL_Event& event, IEntityManager* entityManager, ComponentManager* componentManager,
                  ComponentManager* renderComponentManager, const IFrameRenderer& frameRenderer) override
    {
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            const glm::vec2 winCoords = {event.motion.x, event.motion.y};
            m_selectionRectangle.StartSelection(winCoords);
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            if (m_selectionRectangle.IsSelecting())
            {
                const glm::vec2 winCoords = {event.motion.x, event.motion.y};
                m_selectionRectangle.MoveSelection(winCoords);
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
        {
            ComponentWriter<SelectableComponent> selectableComponents =
                renderComponentManager->WriteComponents<SelectableComponent>();
            ComponentReader<Position2DComponent> positionComponents =
                componentManager->ReadComponents<Position2DComponent>();
            ComponentReader<Movement2DComponent> movementComponents =
                componentManager->ReadComponents<Movement2DComponent>();
            const Box2f selectionBox = m_selectionRectangle.GetCurrentSelection();

            HATCHER_ASSERT(componentManager->Count() == renderComponentManager->Count());
            for (int i = 0; i < componentManager->Count(); i++)
            {
                std::optional<SelectableComponent>& selectableComponent = selectableComponents[i];
                if (selectableComponent)
                {
                    HATCHER_ASSERT(positionComponents[i]);
                    const glm::mat4 modelMatrix =
                        TransformationHelper::ModelFromComponents(positionComponents[i], movementComponents[i]);

                    const Box2f entitySelectionBox =
                        frameRenderer.ProjectBox3DToWindowCoords(selectableComponent->box, modelMatrix);
                    selectableComponent->selected = selectionBox.Touches(entitySelectionBox);
                }
            }

            m_selectionRectangle.EndSelection();
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

private:
    SelectionRectangle& m_selectionRectangle;
};

class SelectionRectangleRenderUpdater final : public RenderUpdater
{
public:
    SelectionRectangleRenderUpdater(const IRendering* rendering, IEventUpdater* eventUpdater)
    {
        eventUpdater->RegisterListener(std::make_shared<SelectionRectangleEventListener>(m_selectionRectangle));

        const std::shared_ptr<Material> material =
            rendering->GetMaterialFactory()->CreateMaterial("shaders/hello_world_2D.vert", "shaders/hello_world.frag");
        m_selectionRectangleMesh = std::make_unique<Mesh>(material, Primitive::Lines);

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
        if (m_selectionRectangle.IsSelecting())
        {
            const Box2f selectionBox = m_selectionRectangle.GetCurrentSelection();
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
    SelectionRectangle m_selectionRectangle;
    std::unique_ptr<Mesh> m_selectionRectangleMesh;
};

RenderUpdaterRegisterer<SelectionRectangleRenderUpdater> registerer;

} // namespace
