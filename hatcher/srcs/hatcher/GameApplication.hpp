#pragma once

#include <memory>
#include <vector>

#include "IApplication.hpp"
#include "basic_types.hpp"

namespace hatcher
{
class IRendering;
class Rendering;
class World;

class GameApplication : public IApplication
{
public:
    GameApplication();
    virtual ~GameApplication();

    World* CreateNewWorld(const char* name);

    int Run();

    void Stop() override;

protected:
    void StartRendering(const char* name, int windowWidth, int windowHeight);

    const IRendering* GetRendering() const;

private:
    void Update();
    void Render();

    std::unique_ptr<World> m_world;

    std::unique_ptr<Rendering> m_rendering;

    uint m_renderFramerateLimit = 60;
    bool m_running = false;
};

} // namespace hatcher
