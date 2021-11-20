#include "GameApplication.hpp"

#include <unistd.h> // TODO put that usleep somewhere else.

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "Graphics/Window.hpp"

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
        game->HandleEvents();
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
    RenderUpdate();
    m_window->Refresh();
}

} // namespace hatcher
