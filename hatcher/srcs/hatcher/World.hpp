#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace hatcher
{
class Entity;
class EntityIDRegistry;
class Updater;

class World
{
public:
    World(const char* name);
    ~World();

    Entity CreateNewEntity();
    void AddRenderingUpdater(Updater* updater);

    void UpdateRendering();

private:
    std::string m_name;
    std::unique_ptr<EntityIDRegistry> m_entityIDRegistry;

    std::vector<std::unique_ptr<Updater>> m_renderingUpdaters;
};

} // namespace hatcher
