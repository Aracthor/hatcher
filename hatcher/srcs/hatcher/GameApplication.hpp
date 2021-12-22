#pragma once

#include <memory>
#include <vector>

#include "basic_types.hpp"

namespace hatcher
{
class IRendering;
class World;

class GameApplication
{
public:
    GameApplication();
    virtual ~GameApplication();

    std::shared_ptr<World> CreateNewWorld(const char* name);
    void SetWatchedWorld(std::shared_ptr<World> parWorld);

    int Run();

    void Stop();

protected:
    void StartRendering(const char* name, int windowWidth, int windowHeight);

    const std::unique_ptr<IRendering>& GetRendering() { return m_rendering; }

private:
    void Update();
    void Render();

    std::vector<std::shared_ptr<World>> m_worlds;
    std::shared_ptr<World> m_watchedWorld;

    std::unique_ptr<IRendering> m_rendering;

    uint m_renderFramerateLimit = 60;
    bool m_running = false;
};

} // namespace hatcher
