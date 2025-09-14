#include "RenderUpdaterOrder.hpp"

#include "Components/Movement2DComponent.hpp"
#include "Components/Position2DComponent.hpp"
#include "RenderComponents/SelectableComponent.hpp"
#include "utils/TransformationHelper.hpp"

#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/Graphics/FrameRenderer.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"
#include "hatcher/unique_ptr.hpp"

using namespace hatcher;

namespace
{

class
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
} selectionRectangle;

class SelectionRectangleEventListener : public IEventListener
{
    void GetEvent(const SDL_Event& event, IApplication* application, ICommandManager* commandManager,
                  const ComponentAccessor* componentAccessor, ComponentAccessor* renderComponentAccessor,
                  const IFrameRenderer& frameRenderer) override
    {
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            const glm::vec2 winCoords = {event.motion.x, event.motion.y};
            selectionRectangle.StartSelection(winCoords);
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            if (selectionRectangle.IsSelecting())
            {
                const glm::vec2 winCoords = {event.motion.x, event.motion.y};
                selectionRectangle.MoveSelection(winCoords);
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
        {
            ComponentWriter<SelectableComponent> selectableComponents =
                renderComponentAccessor->WriteComponents<SelectableComponent>();
            ComponentReader<Position2DComponent> positionComponents =
                componentAccessor->ReadComponents<Position2DComponent>();
            const Box2f selectionBox = selectionRectangle.GetCurrentSelection();

            HATCHER_ASSERT(componentAccessor->Count() == renderComponentAccessor->Count());
            for (int i = 0; i < componentAccessor->Count(); i++)
            {
                std::optional<SelectableComponent>& selectableComponent = selectableComponents[i];
                const std::optional<Position2DComponent>& positionComponent = positionComponents[i];
                if (selectableComponent && positionComponent)
                {
                    const glm::mat4 modelMatrix = TransformationHelper::ModelFromComponents(*positionComponent);

                    const Box2f entitySelectionBox =
                        frameRenderer.ProjectBox3DToWindowCoords(selectableComponent->box, modelMatrix);
                    selectableComponent->selected = selectionBox.Touches(entitySelectionBox);
                }
            }

            selectionRectangle.EndSelection();
        }
    }
};

class SelectionRectangleRenderUpdater : public RenderUpdater
{
public:
    SelectionRectangleRenderUpdater(const IRendering* rendering)
    {
        m_material =
            rendering->GetMaterialFactory()->CreateMaterial("shaders/grounded.vert", "shaders/const_color.frag");
        m_material->SetUniform("uniColor", glm::vec4(1.0, 1.0, 1.0, 1.0));
        m_selectionRectangleMesh = make_unique<Mesh>(m_material.get(), Primitive::Lines);

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

    void Update(IApplication* application, const ComponentAccessor* componentAccessor,
                ComponentAccessor* renderComponentAccessor, IFrameRenderer& frameRenderer) override
    {
        frameRenderer.DisableDepthTest();
        if (selectionRectangle.IsSelecting())
        {
            const Box2f selectionBox = selectionRectangle.GetCurrentSelection();
            const glm::vec2 rectangleCenter = selectionBox.Center();
            const glm::vec2 rectangleSize = selectionBox.Extents();

            const glm::vec3 position = {rectangleCenter.x, rectangleCenter.y, 0.f};
            const glm::vec3 scale = {rectangleSize.x / 2.f, rectangleSize.y / 2.f, 0.f};

            glm::mat4 modelMatrix = glm::mat4(1.f);
            modelMatrix = glm::translate(modelMatrix, position);
            modelMatrix = glm::scale(modelMatrix, scale);

            frameRenderer.PrepareUIDraw(m_material.get());
            m_selectionRectangleMesh->Draw(modelMatrix);
        }
        frameRenderer.EnableDepthTest();
    }

private:
    unique_ptr<Material> m_material;
    unique_ptr<Mesh> m_selectionRectangleMesh;
};

EventListenerRegisterer<SelectionRectangleEventListener> eventRegisterer;
RenderUpdaterRegisterer<SelectionRectangleRenderUpdater> updaterRegisterer((int)ERenderUpdaterOrder::Interface);

} // namespace
