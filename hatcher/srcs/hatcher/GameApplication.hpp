#pragma once

#include <memory>

#include "basic_types.hpp"

namespace hatcher
{
class Window;

class GameApplication
{
public:
    GameApplication(const char* name, int windowWidth, int windowHeight);
    virtual ~GameApplication();

    int Run();

    void Stop();

private:
    // TODO use only updaters instead.
    virtual void HandleEvents() = 0;
    virtual void RenderUpdate() = 0;

    void Update();

    std::unique_ptr<Window> m_window;

    uint m_renderFramerateLimit = 60;
    bool m_running = false;
};

} // namespace hatcher