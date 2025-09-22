#include "GameApplication.hpp"

#include <unistd.h> // TODO put that usleep somewhere else.

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <iostream>
#endif

#include "Graphics/Rendering.hpp"

#include "ApplicationConfiguration.hpp"
#include "Clock.hpp"
#include "FileSystem.hpp"
#include "World.hpp"
#include "WorldManager.hpp"

namespace hatcher
{

GameApplication::GameApplication(int argc, char** argv)
    : m_configuration(new ApplicationConfiguration(argc, argv))
    , m_fileSystem(new FileSystem(m_configuration->pathToProject))
    , m_worldManager(new WorldManager())
{
}

GameApplication::~GameApplication() = default;

int GameApplication::Run()
{
    m_world = m_worldManager->CreateWorld(m_configuration->seed, m_configuration->commandSaveFile,
                                          m_configuration->commandLoadFile);
    auto mainLoop = [](void* data)
    {
        try
        {
            GameApplication* game = reinterpret_cast<GameApplication*>(data);
            game->Update();
        }
        catch (const std::exception& exception)
        {
#ifdef __EMSCRIPTEN__
            std::cerr << "Fatal error: " << exception.what() << std::endl;
            std::abort();
#endif
            throw;
        }
    };

    m_running = true;
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_timing(EM_TIMING_SETTIMEOUT, m_renderFramerateLimit);
    emscripten_set_main_loop_arg(mainLoop, this, m_renderFramerateLimit, true);
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

void GameApplication::Restart()
{
    m_world = m_worldManager->CreateWorld(m_configuration->seed, m_configuration->commandSaveFile,
                                          m_configuration->commandLoadFile);
}

void GameApplication::SetUpdateTickrate(float tickrate)
{
    m_updateTicker.SetTickrate(tickrate);
}

float GameApplication::GetUpdateTickrate() const
{
    return m_updateTicker.GetTickrate();
}

void GameApplication::StartRendering(const char* name, int windowWidth, int windowHeight)
{
    m_rendering = make_unique<Rendering>(name, windowWidth, windowHeight, m_fileSystem.get());
    m_worldManager->CreateRenderUpdaters(m_rendering.get());
}

void GameApplication::Update()
{
    if (m_rendering)
    {
        m_rendering->HandleWindowEvents(this, m_worldManager.get(), m_world.get());
    }
    {
        const float maxUpdateTime = 0.1f;
        float updateTime = 0.f;
        Clock clock;
        int ticks = m_updateTicker.TickCount();
        while (ticks-- > 0 && updateTime < maxUpdateTime)
        {
            m_worldManager->Update(m_world.get());
            clock.Update();
            updateTime += clock.GetElapsedTime();
        }
    }
    if (m_rendering)
    {
        m_rendering->UpdateWorldRendering(this, m_worldManager.get(), m_world.get());
    }
}

} // namespace hatcher
