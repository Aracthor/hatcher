#pragma once

#include <memory>

#include "hatcher/glm_pure.hpp"

#include "hatcher/Graphics/IEventUpdater.hpp"

namespace hatcher
{
class GameApplication;
class Mesh;
class MeshBuilder;
} // namespace hatcher

class SelectionRectangleHandler;

class EventHandlerUpdater final : public hatcher::IEventUpdater
{
public:
    EventHandlerUpdater(hatcher::GameApplication* application,
                        const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder);
    ~EventHandlerUpdater();

    void Update(hatcher::ComponentManager* componentManager, const hatcher::Clock& clock,
                hatcher::IFrameRenderer& frameRenderer) override;

private:
    void DrawSelectionRectangle(hatcher::IFrameRenderer& frameRenderer);
    glm::mat4 CalculateProjectionMatrix();

    hatcher::GameApplication* m_application;

    glm::vec2 m_fixedPosition = glm::vec2(0.f, 0.f);

    float m_windowWidth = 800.f;
    float m_windowHeight = 600.f;
    float m_pixelSize = 0.01f;

    std::unique_ptr<SelectionRectangleHandler> m_selectionHandler;
    std::unique_ptr<hatcher::Mesh> m_selectionRectangleMesh;
};
