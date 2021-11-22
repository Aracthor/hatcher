#include "GameApplication.hpp"

#include <unistd.h> // TODO put that usleep somewhere else.

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "Graphics/Core/Window.hpp"

#include "World.hpp"

namespace hatcher
{

GameApplication::GameApplication(const char* name, int windowWidth, int windowHeight)
{
    m_window.reset(new Window(name, windowWidth, windowHeight));
}

GameApplication::~GameApplication() = default;

int GameApplication::Run()
{
    auto mainLoop = [](void* data) {
        GameApplication* game = reinterpret_cast<GameApplication*>(data);
        game->Update();
    };

    m_running = true;
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(mainLoop, this, m_renderFramerateLimit, true);
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

void GameApplication::Update()
{
    m_window->Clear();
    if (m_watchedWorld)
    {
        m_watchedWorld->UpdateRendering();
    }
    m_window->Refresh();
}

} // namespace hatcher
