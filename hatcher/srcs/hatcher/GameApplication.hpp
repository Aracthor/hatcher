#pragma once

#include "IApplication.hpp"
#include "unique_ptr.hpp"

namespace hatcher
{
struct ApplicationConfiguration;
class FileSystem;
class IRendering;
class Rendering;
class World;

class GameApplication : public IApplication
{
public:
    GameApplication(int argc, char** argv);
    ~GameApplication();

    int Run();

    void Stop() override;

    void StartRendering(const char* name, int windowWidth, int windowHeight);

private:
    void Update();
    void Render();

    unique_ptr<ApplicationConfiguration> m_configuration;
    unique_ptr<FileSystem> m_fileSystem;
    unique_ptr<World> m_world;

    unique_ptr<Rendering> m_rendering;

    int m_renderFramerateLimit = 60;
    bool m_running = false;
};

} // namespace hatcher
