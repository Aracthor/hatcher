#include "GameApplication.hpp"

#include <unistd.h> // TODO put that usleep somewhere else.

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "Graphics/Clock.hpp"
#include "Graphics/Rendering.hpp"

#include "ApplicationConfiguration.hpp"
#include "FileSystem.hpp"
#include "World.hpp"

namespace hatcher
{

GameApplication::GameApplication(int argc, char** argv)
    : m_configuration(new ApplicationConfiguration(argc, argv))
    , m_fileSystem(new FileSystem(m_configuration->pathToProject))
    , m_world(make_unique<World>())
{
}

GameApplication::~GameApplication() = default;

int GameApplication::Run()
{
    auto mainLoop = [](void* data)
    {
        GameApplication* game = reinterpret_cast<GameApplication*>(data);
        game->Update();
        game->Render();
    };

    m_running = true;
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_timing(EM_TIMING_SETTIMEOUT, m_renderFramerateLimit);
    emscripten_set_main_loop_arg(mainLoop, this, 0, true);
#else
    Clock frameClock;
    while (m_running)
    {
        const float minFrameTimeInMs = 1000.f / m_renderFramerateLimit;
        const float elapsedTimeInMs = frameClock.GetElapsedTime();
        const float timeToWaitInMs = minFrameTimeInMs - elapsedTimeInMs;
        if (timeToWaitInMs > 0.f)
            usleep(timeToWaitInMs * 1000.f);
        frameClock.Update();
        mainLoop(this);
        frameClock.Update();
    }
#endif

    return 0;
}

void GameApplication::Stop()
{
    m_running = false;
#ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop();
#endif
}

void GameApplication::StartRendering(const char* name, int windowWidth, int windowHeight)
{
    m_rendering = make_unique<Rendering>(name, windowWidth, windowHeight, m_fileSystem.get());
    m_world->CreateRenderUpdaters(m_rendering.get());
}

void GameApplication::Update()
{
    if (m_rendering)
    {
        m_rendering->HandleWindowEvents(this, m_world.get());
    }
    m_world->Update();
    if (m_rendering)
    {
        m_rendering->UpdateWorldRendering(m_world.get());
    }
}

void GameApplication::Render()
{
    if (m_rendering)
    {
        m_rendering->RenderWorld();
    }
}

} // namespace hatcher
