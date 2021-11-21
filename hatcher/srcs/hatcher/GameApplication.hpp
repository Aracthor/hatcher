#pragma once

#include <memory>
#include <vector>

#include "basic_types.hpp"

namespace hatcher
{
class Window;
class World;

class GameApplication
{
public:
    GameApplication(const char* name, int windowWidth, int windowHeight);
    virtual ~GameApplication();

    std::shared_ptr<World> CreateNewWorld(const char* name);
    void SetWatchedWorld(std::shared_ptr<World> parWorld);

    int Run();

    void Stop();

private:
    void Update();

    std::vector<std::shared_ptr<World>> m_worlds;
    std::shared_ptr<World> m_watchedWorld;

    std::unique_ptr<Window> m_window;

    uint m_renderFramerateLimit = 60;
    bool m_running = false;
};

} // namespace hatcher