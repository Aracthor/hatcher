#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/assert.hpp"

#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"
#include "Selectable2DComponent.hpp"
#include "SelectionRectangle.hpp"
#include "TransformationHelper.hpp"

namespace
{

using namespace hatcher;

class SelectionRectangleEventListener final : public IEventListener
{
public:
    void GetEvent(const SDL_Event& event, IApplication* gameApplication,
                  IEntityManager* entityManager, ComponentManager* componentManager,
                  ComponentManager* renderComponentManager,
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

const int dummy = RegisterEventListener<SelectionRectangleEventListener>("SelectionRectangle");

} // namespace