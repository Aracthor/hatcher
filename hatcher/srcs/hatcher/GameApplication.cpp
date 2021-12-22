#include "GameApplication.hpp"

#include <unistd.h> // TODO put that usleep somewhere else.

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "Graphics/Rendering.hpp"

#include "World.hpp"

namespace hatcher
{

GameApplication::GameApplication() = default;

GameApplication::~GameApplication() = default;

int GameApplication::Run()
{
    auto mainLoop = [](void* data) {
        GameApplication* game = reinterpret_cast<GameApplication*>(data);
        game->Update();
        game->Render();
    };

    m_running = true;
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_timing(EM_TIMING_SETTIMEOUT, m_renderFramerateLimit);
    emscripten_set_main_loop_arg(mainLoop, this, 0, true);
#else
    while (m_running)
    {
        usleep(1000000 / m_renderFramerateLimit);
        mainLoop(this);
    }
#endif

    return 0;
}

std::shared_ptr<World> GameApplication::CreateNewWorld(const char* name)
{
    m_worlds.emplace_back(new World(name));
    return m_worlds.back();
}

void GameApplication::SetWatchedWorld(std::shared_ptr<World> world)
{
    m_watchedWorld = world;
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
    m_rendering = std::make_unique<Rendering>(name, windowWidth, windowHeight);
}

void GameApplication::Update()
{
    if (m_watchedWorld && m_rendering)
    {
        m_rendering->UpdateWorldRendering(m_watchedWorld.get());
    }
}

void GameApplication::Render()
{
    if (m_watchedWorld && m_rendering)
    {
        m_rendering->RenderWorld();
    }
}

} // namespace hatcher
