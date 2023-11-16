#pragma once

#include "IApplication.hpp"
#include "basic_types.hpp"
#include "unique_ptr.hpp"

namespace hatcher
{
class FileSystem;
class IRendering;
class Rendering;
class World;

class GameApplication : public IApplication
{
public:
    GameApplication(const char* commandName);
    virtual ~GameApplication();

    World* CreateNewWorld();

    int Run();

    void Stop() override;

protected:
    void StartRendering(const char* name, int windowWidth, int windowHeight);

    const IRendering* GetRendering() const;

private:
    void Update();
    void Render();

    unique_ptr<FileSystem> m_fileSystem;
    unique_ptr<World> m_world;

    unique_ptr<Rendering> m_rendering;

    uint m_renderFramerateLimit = 60;
    bool m_running = false;
};

} // namespace hatcher
