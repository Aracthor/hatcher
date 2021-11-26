#pragma once

#include <memory>
#include <string>
#include <vector>

namespace hatcher
{
class ComponentManager;
class IFrameRenderer;
class RenderUpdater;
class Updater;

class World final
{
public:
    World(const char* name);
    ~World();

    ComponentManager* GetComponentManager() { return m_componentManager.get(); }
    const ComponentManager* GetComponentManager() const { return m_componentManager.get(); }

    void AddRenderUpdater(RenderUpdater* updater);

    void UpdateRendering(IFrameRenderer& frameRenderer);

private:
    std::string m_name;

    std::unique_ptr<ComponentManager> m_componentManager;

    std::vector<std::unique_ptr<RenderUpdater>> m_renderUpdaters;
};

} // namespace hatcher
