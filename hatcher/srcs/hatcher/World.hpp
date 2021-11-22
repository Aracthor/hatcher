#pragma once

#include <memory>
#include <string>
#include <vector>

namespace hatcher
{
class ComponentManager;
class Updater;

class World final
{
public:
    World(const char* name);
    ~World();

    ComponentManager* GetComponentManager() { return m_componentManager.get(); }
    const ComponentManager* GetComponentManager() const { return m_componentManager.get(); }

    void AddRenderingUpdater(Updater* updater);

    void UpdateRendering();

private:
    std::string m_name;

    std::unique_ptr<ComponentManager> m_componentManager;

    std::vector<std::unique_ptr<Updater>> m_renderingUpdaters;
};

} // namespace hatcher
