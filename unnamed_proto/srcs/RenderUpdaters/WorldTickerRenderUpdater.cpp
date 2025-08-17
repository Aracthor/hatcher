#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/IApplication.hpp"

#include "imgui.h"

using namespace hatcher;

namespace
{
const float baseTickSpeed = 60.f;

bool almostEquals(float a, float b, float epsilon)
{
    return std::abs(a - b) < epsilon;
}

class WorldTickerEventListener : public IEventListener
{
    void GetEvent(const SDL_Event& event, IApplication* application, ICommandManager* commandManager,
                  const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                  const IFrameRenderer& frameRenderer) override
    {
        if (event.type == SDL_KEYDOWN)
        {
            float tickRate = application->GetUpdateTickrate();
            if (event.key.keysym.scancode == SDL_SCANCODE_KP_PLUS)
            {
                if (tickRate == 0.f)
                    tickRate = 30.f;
                else if (tickRate < baseTickSpeed * 8.f)
                    tickRate *= 2.f;
                application->SetUpdateTickrate(tickRate);
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_KP_MINUS)
            {
                if (almostEquals(tickRate, baseTickSpeed * 0.5f, 000.1f))
                    tickRate = 0.0f;
                else if (tickRate > 30.f)
                    tickRate /= 2.f;
                application->SetUpdateTickrate(tickRate);
            }
        }
    }
};

void DisplaySpeedButton(IApplication* application, const char* label, float tickSpeed)
{
    const bool selected = almostEquals(application->GetUpdateTickrate(), tickSpeed, 0.001f);
    ImGui::BeginDisabled(selected);
    if (ImGui::Button(label))
        application->SetUpdateTickrate(tickSpeed);
    ImGui::EndDisabled();
    ImGui::SameLine();
}

class WorldTickerRenderUpdater : public RenderUpdater
{
public:
    WorldTickerRenderUpdater(const IRendering* rendering) {}

    void Update(IApplication* application, const ComponentManager* componentManager,
                ComponentManager* renderComponentManager, IFrameRenderer& frameRenderer) override
    {
        const float windowWidth = frameRenderer.Resolution().x;
        const ImVec2 windowSize(200, 50);
        ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
        ImGui::SetNextWindowPos({windowWidth - windowSize.x, 0}, ImGuiCond_Always);
        if (ImGui::Begin("Time", nullptr, ImGuiWindowFlags_NoDecoration))
        {
            ImGui::Text("Tick: %d\n", frameRenderer.CurrentTick());
            DisplaySpeedButton(application, "x0", baseTickSpeed * 0.f);
            DisplaySpeedButton(application, "x0.5", baseTickSpeed * 0.5f);
            DisplaySpeedButton(application, "x1", baseTickSpeed * 1.f);
            DisplaySpeedButton(application, "x2", baseTickSpeed * 2.f);
            DisplaySpeedButton(application, "x4", baseTickSpeed * 4.f);
            DisplaySpeedButton(application, "x8", baseTickSpeed * 8.f);
        }
        ImGui::End();
    }
};

EventListenerRegisterer<WorldTickerEventListener> eventListenerRegisterer;
RenderUpdaterRegisterer<WorldTickerRenderUpdater> renderUpdaterRegisterer;

} // namespace