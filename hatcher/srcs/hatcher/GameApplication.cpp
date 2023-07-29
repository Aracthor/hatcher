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

World* GameApplication::CreateNewWorld(const char* name)
{
    m_world = make_unique<World>(name);
    return m_world.get();
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
    m_rendering = make_unique<Rendering>(name, windowWidth, windowHeight);
    m_world->CreateRenderUpdaters(m_rendering.get());
}

const IRendering* GameApplication::GetRendering() const
{
    return m_rendering.get();
}

void GameApplication::Update()
{
    m_world->Update();
    if (m_rendering)
    {
        m_rendering->UpdateWorldRendering(this, m_world.get());
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
